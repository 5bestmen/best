#include "104master.h"
#include "fesapi/fescnst.h"
#include "fesapi/fesapi.h"
#include "log/log.h"
#include "../../scdpub/variant_data.h"
#include "macrodefine.h"
#include <QDateTime>

C104Master::C104Master(QObject*parent ,int32u nOccno, std::shared_ptr<C104Channel> pIoChannel):C104Server(parent,nOccno,pIoChannel)
{
	Q_ASSERT(nOccno!=INVALID_OCCNO && nOccno <=MAX_CHANNEL_OCCNO);
	if (nOccno==INVALID_OCCNO || nOccno >MAX_CHANNEL_OCCNO)
	{
		LogMsg(QObject::tr("invalid channel occno").toStdString().c_str(),0);
	}
	Q_ASSERT(pIoChannel);
	if (!pIoChannel)
	{
		LogMsg(QObject::tr("invalid data pool").toStdString().c_str(), 0);
	}
	m_pChannel = pIoChannel;
	m_nChannelOccNo = nOccno;
	m_bClosed = false;
	m_p104Impl = new C104Impl(this, PEER_CONTROL, 0);
	m_pCurrentCommand = std::make_shared<NBM_TELECTRL>();
	
	//默认都是一次性遥控完成
	m_pCurrentCommand->m_bCommandFinished = true;
}

C104Master::~C104Master()
{
	if (m_p104Impl != NULL) 
	{ 
		delete m_p104Impl; 
		m_p104Impl = NULL; 
	}
}

bool C104Master::OnOpen()
{
	if (m_p104Impl == NULL)
		return false;
	bool bRet=IoSetChannelQua(m_nChannelOccNo, CHANNEL_STATE_RUN);
	if (!bRet)
	{
		LogMsg(QObject::tr("set channelstate error").toStdString().c_str(), 0);
	}
	//暂时用默认参数
	m_nParamID = Default_Protocol;

	m_nDeviceAddr = Default_Protocol;

	m_p104Impl->Initialise(m_nParamID);

	StartTimer(TIMER_ID_GETCMD, TIMER_INTERVAL_GETCMD);

	StartTimer(TIMER_ID_GENERALCALL, m_p104Impl->GetParam()->m_nCycleGeneralCall);

	StartTimer(TIMER_ID_KWH_GENERALCALL, m_p104Impl->GetParam()->m_nCycleKwhGeneralCall);

	StartTimer(TIMER_ID_SYNCTIME, m_p104Impl->GetParam()->m_nCycleSyncTime);

	bool bResult = m_p104Impl->Open();

	return bResult;

}

int C104Master::OnClose()
{
	//todo 设置通道状态

	m_bClosed = true;

	//KillTimer(TIMER_ID_GETCMD);
	KillTimer(TIMER_ID_GENERALCALL);
	KillTimer(TIMER_ID_KWH_GENERALCALL);
	KillTimer(TIMER_ID_SYNCTIME);

	IoSetChannelQua(m_nChannelOccNo, CHANNEL_STATE_STOP);

	if (m_p104Impl == NULL)
		return 0;

	return m_p104Impl->Close();
}

bool C104Master::OnReceive(char* pBuff, int nLen)
{
	if (m_p104Impl == NULL)
		return false;

	return m_p104Impl->OnReceive(pBuff, nLen);
}

bool C104Master::OnTimer(int nTimerID)
{
	if (m_p104Impl == NULL)
		return false;

	switch (nTimerID)
	{
	case TIMER_ID_GETCMD:
	{
		SetTimer(TIMER_ID_GETCMD, TIMER_INTERVAL_GETCMD);
		return OnTimerGetCmd();
	}
	case TIMER_ID_GENERALCALL:
	{
		SetTimer(TIMER_ID_GENERALCALL, m_p104Impl->GetParam()->m_nCycleGeneralCall);
		return OnTimerGeneralCall();
	}
	case TIMER_ID_KWH_GENERALCALL:
	{
		SetTimer(TIMER_ID_KWH_GENERALCALL, m_p104Impl->GetParam()->m_nCycleKwhGeneralCall);
		return OnTimerKwhGeneralCall();
	}
	case TIMER_ID_SYNCTIME:
	{
		SetTimer(TIMER_ID_SYNCTIME, m_p104Impl->GetParam()->m_nCycleSyncTime);
		return OnTimerSyncTime();
	}
	}
	return m_p104Impl->OnTimer(nTimerID);
}

bool C104Master::SetTimer(int nTimerID, int nSecond)
{
	return StartTimer(nTimerID, nSecond);
}

void C104Master::RemoveTimer(int nTimerID)
{
	KillTimer(nTimerID);
}

bool C104Master::SendMessage(char* pBuff, int nLen, int nConnectID /*= 0*/)
{
	return Send(pBuff, nLen);
}

int C104Master::GetGroupID()
{
	return m_nGroupID;
}

int C104Master::GetChannelID()
{
	return m_nChannelID;
}

int C104Master::GetDeviceID()
{
	return m_nDeviceID;
}

int C104Master::GetParamID()
{
	return m_nParamID;
}

int32u C104Master::GetChannelOccNo()
{
	return m_nChannelOccNo;
}

void C104Master::LogCallback104(int nSendOrRecv, int nMsgType, string strMsg)
{
	//类型标签
	string strTag;
	switch (nSendOrRecv)
	{
		case MCB_ERROR: strTag = "[ERROR]: "; break;
		case MCB_SEND:   strTag = "[SEND]: ";  break;
		case MCB_RECV:   strTag = "[RECV]: ";   break;
	}

	char buf[256];
	//时间戳
	//string strTimestamp = FormatTimestamp();
	std::string strAsioTimestamp = QDateTime::currentDateTime().toString().toStdString();// boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()); 
	sprintf(buf, "[%s] ", (char*)strAsioTimestamp.c_str());
	std::string strTimestamp(buf);

	//ID
	sprintf(buf, "[GroupID:%d ChannelID:%d DeviceID:%d]", m_nGroupID, m_nChannelID, m_nDeviceID);
	string strID(buf);

	//Trace
	strMsg = strTimestamp + strTag + strID + strMsg;

	LogMsg((char*)strMsg.c_str(),0);

}

std::shared_ptr<C104Channel> C104Master::getKernel()
{
	return m_pChannel;
}

//定时获取队列的遥控指令
bool C104Master::OnTimerGetCmd()
{
	if (!m_p104Impl->IsRunning())
	{
		//判断规约停止运行，但是命令需要一直接受，然后舍弃掉
		SETVAL_MSG pMsg;
		ReadHostCmd(m_nChannelOccNo, &pMsg, 0);
		return true;
	}
		
	if (!m_p104Impl->GetCurCommand()->m_bCommandFinished)
	{
		return true;
	}
	//先处理当前遥控队列 ,然后在从队列接收处理

	SETVAL_MSG pMsg;
	bool bRet=ReadHostCmd(m_nChannelOccNo, &pMsg,0);
	
	if (bRet)
	{
		m_p104Impl->GetCurCommand()->Att = pMsg.Att;
		m_p104Impl->GetCurCommand()->Datatype  = pMsg.Datatype;
		m_p104Impl->GetCurCommand()->IddType  =  pMsg.IddType;
		m_p104Impl->GetCurCommand()->DoutOccNo = pMsg.Occno;
		m_p104Impl->GetCurCommand()->NodeOccNo = pMsg.NodeOccNo;
		m_p104Impl->GetCurCommand()->Source1   = pMsg.Source1;
		m_p104Impl->GetCurCommand()->Value[0] = pMsg.Value[0];

		m_p104Impl->GetCurCommand()->m_nDataID = QString(GetDoutAddress(pMsg.Occno)).toUInt();

		if (pMsg.IddType==IDD_DOUT)
		{
			m_p104Impl->GetCurCommand()->m_nDataType = DATATYPE_BINARY;
		}

		CVariant nVar(pMsg.Value[0]);
		
		switch (nVar.operator unsigned char())
		{
		case 1://执行
		{
		//	str += tr("TELECTRL_REQUEST_EXECUTE");
			m_p104Impl->GetCurCommand()->m_nCtrlType = TELECTRL_REQUEST_EXECUTE;
			break;
		}
		case 2://选择
		{
		//	str += tr("TELECTRL_REQUEST_SELECT");
			m_p104Impl->GetCurCommand()->m_nCtrlType = TELECTRL_REQUEST_SELECT;
			break;
		}			
		case 3://选择后执行
		{
			//telectrl.m_nCommandType = TELECTRL_REQUEST_SELECT;
			break;
		}
		case 4:
		{
			//第一步 选择
			m_p104Impl->GetCurCommand()->m_nCtrlType = TELECTRL_REQUEST_SELECT;
			m_p104Impl->GetCurCommand()->m_bCommandFinished = false;
			break;
		}			
		default:
			break;
		}

		bRet = m_p104Impl->OnCommand(m_p104Impl->GetCurCommand());

		return bRet;
	}
	return true;
}

bool C104Master::OnTimerGeneralCall()
{
	if (!m_p104Impl->IsRunning())
		return true;

	NBM_TELECTRL telectrl;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
	bool IsOK = m_p104Impl->OnCommand(&telectrl);

	return true;
}

bool C104Master::OnTimerKwhGeneralCall()
{
	if (!m_p104Impl->IsRunning())
		return true;

	NBM_TELECTRL telectrl;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_KWHGENERALCALL;
	bool IsOK = m_p104Impl->OnCommand(&telectrl);

	return true;
}

bool C104Master::OnTimerSyncTime()
{
	if (!m_p104Impl->IsRunning())
		return true;

	NBM_TELECTRL telectrl;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_SYNCTIME;
	bool IsOK = m_p104Impl->OnCommand(&telectrl);

	return true;
}

void C104Master::LogMsg(const char* pszText, int nLevel)
{
	Q_ASSERT(pszText && strlen(pszText) >0 );
	if (pszText || strlen( pszText)==0)
	{
		return ;
	}
	::LogMsg("104_iodrv_api", pszText, nLevel, nullptr);
}
