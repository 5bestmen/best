
#include "104impl.h"


C104Impl::C104Impl(IHost* pHost, int nPeerType, int nConnectID)
{
	m_pHost = pHost;
	m_nConnectID = nConnectID;

	m_k = 0;
	m_w = 0;

	m_nRecvNum = 0;
	m_nSendNum = 0;

	m_pTimer =  new COnTimer(pHost, this);
	m_pSender = new CSender(pHost, this);
	m_pRecver = new CRecver(pHost, this);
	m_pCurTelCtrl = new NBM_TELECTRL;
	m_pCurTelCtrl->m_bCommandFinished = true;
	m_bIsRunning = false;

	m_nPeerType = nPeerType;
}

C104Impl::~C104Impl()
{
	if (m_pTimer != NULL)  { delete m_pTimer;   m_pTimer = NULL; }
	if (m_pSender != NULL) { delete m_pSender;  m_pSender = NULL; }
	if (m_pRecver != NULL) { delete m_pRecver;  m_pRecver = NULL; }
}

void C104Impl::Initialise(int nParamID)
{
	/* IKernel* pKernel = m_pHost->getKernel();
	 pKernel->GetParam104(nParamID,m_param104);*/
	if (nParamID==1)
	{
		int nnn = 0;
	}
}

bool C104Impl::Open()
{
	m_nErrorCode = ERROR_NONE;

	return m_pSender->Send_U(STARTDT_ACT);
}

bool C104Impl::Close()
{
	m_k = 0;
	m_w = 0;

	m_nRecvNum = 0;
	m_nSendNum = 0;

	m_bIsRunning = false;

	LogN(MCB_SEND, TYPE_U, "C104Impl::Close");

	return true;
}

bool C104Impl::OnReceive(char* pBuff, int nLen)
{
	if (!m_pRecver->OnReceive(pBuff, nLen))
	{
		int nErrorCode = GetErrorCode();
		if (nErrorCode == ERROR_SERIALNUMBER)
			return false;//接收过程中，只有序列号错误，才认为错误
	}

	return true;
}

bool C104Impl::OnCommand(NBM_TELECTRL* pTelectrl)
{
	return m_pSender->OnCommand(pTelectrl);
}

bool C104Impl::OnTimer(int nTimerID)
{
	return m_pTimer->OnTimer(nTimerID);
}

//////////////////////////////////////////////////////////////////////////

bool C104Impl::OnNumRecv(int nFrameType, int nSendNum)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		if (nSendNum != m_nRecvNum)//接收到的发送序号，不等于本地的接收序号
		{
			char buf[128];
			sprintf(buf, "序列号错误 ConnectID:%d  LOCAL m_nRecvNum:[%d] REMOTE nSendNum:[%d] m_K:%d m_w:%d", this->GetConnectID(), m_nRecvNum, nSendNum, m_k, m_w);
			m_pHost->LogCallback104(MCB_ERROR, TYPE_ERROR, buf);
			m_pHost->LogCallback104(MCB_ERROR, TYPE_ERROR, buf);
			m_pHost->LogCallback104(MCB_ERROR, TYPE_ERROR, buf);

			//设置错误码
			SetErrorCode(ERROR_SERIALNUMBER);

			//断链前给通讯管理机发最后一帧的确认，使其清除缓存
			this->GetSender()->Send_S_BeforeClose(nSendNum);

			//m_pHost->getKernel()->Sleep(500);

			//若发现序号错误，以装置那边序号为准
			m_nRecvNum = nSendNum;
			m_nRecvNum += 1;

			return true;
		}
		else
		{
			m_nRecvNum += 1;//本地接收序列号递增
		}

		if (m_nRecvNum > MAX_SERIAL_NUMBER) m_nRecvNum = 0;
	}
	break;
	case TYPE_S:
		break;
	}
	return true;
}
bool C104Impl::OnNumSend(int nFrameType)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		m_nSendNum += 1;
		if (m_nSendNum > MAX_SERIAL_NUMBER) m_nSendNum = 0;
	}
	break;
	case TYPE_S:
		break;
	}
	return true;
}

bool C104Impl::OnKwSend(int nFrameType)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		m_k += 1;
		m_w = 0;
	}
	break;
	case TYPE_S:
	{
		m_w = 0;
	}
	break;
	}
	return true;
}
bool C104Impl::OnKwRecv(int nFrameType, int nRecvNum)//nRecvNum:接收到的报文中的接收序列号
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		m_w += 1;
		if (nRecvNum > m_nSendNum)
		{
			m_k = 1;
		}
		else
		{
			m_k = m_nSendNum - nRecvNum;
		}
		if (m_w >= GetParam()->m_nDefaultW)
		{
			this->GetSender()->Send_S();
		}
	}
	break;
	case TYPE_S:
	{
		if (nRecvNum > m_nSendNum)
		{
			m_k = 1;
		}
		else
		{
			m_k = m_nSendNum - nRecvNum;
		}
	}
	break;
	}
	return true;
}

bool C104Impl::OnTimerSend(int nFrameType, int nUtype)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		//发送I帧，重启T1(等待确认)
		m_pHost->RemoveTimer(TIMER_ID_T1);

		m_pHost->SetTimer(TIMER_ID_T1, GetParam()->m_T1);
		//发送I帧，关T2(回复确认)
		m_pHost->RemoveTimer(TIMER_ID_T2);
	}
	break;
	case TYPE_S:
	{
		//发送S帧，关T2(回复确认)
		m_pHost->RemoveTimer(TIMER_ID_T2);
	}
	break;
	case TYPE_U:
	{
		//发送U帧，重启T1(等待确认)
		if (nUtype == TESTFR_ACT || nUtype == STARTDT_ACT)//需要确认的
		{
			m_pHost->RemoveTimer(TIMER_ID_T1);

			m_pHost->SetTimer(TIMER_ID_T1, GetParam()->m_T1);
		}
	}
	break;
	}

	return true;
}

bool C104Impl::OnTimerRecv(int nFrameType)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		//接收I帧，重启T2(回复确认)
		m_pHost->RemoveTimer(TIMER_ID_T2);

		m_pHost->SetTimer(TIMER_ID_T2, GetParam()->m_T2);

		//接收I帧，关闭T1(等待确认)
		m_pHost->RemoveTimer(TIMER_ID_T1);

		//接收I帧，重启T3(测试过程)
		m_pHost->RemoveTimer(TIMER_ID_T3);

		m_pHost->SetTimer(TIMER_ID_T3, GetParam()->m_T3);
	}
	break;
	case TYPE_S:
	{
		//接收S帧，关闭T1(等待确认)
		m_pHost->RemoveTimer(TIMER_ID_T1);

		//接收S帧，重启T3(测试过程)
		m_pHost->RemoveTimer(TIMER_ID_T3);

		m_pHost->SetTimer(TIMER_ID_T3, GetParam()->m_T3);
	}
	break;
	case TYPE_U:
	{
		//接收U帧，关闭T1(等待确认)
		m_pHost->RemoveTimer(TIMER_ID_T1);

		//接收U帧，重启T3(测试过程)
		m_pHost->RemoveTimer(TIMER_ID_T3);

		m_pHost->SetTimer(TIMER_ID_T3, GetParam()->m_T3);
	}
	break;
	}
	return true;
}

bool C104Impl::OnSendFrame(int nFrameType, int nUtype)
{
	//序列号
	if (!OnNumSend(nFrameType))
		return false;

	//KW值
	if (!OnKwSend(nFrameType))
		return false;

	//计时器
	if (!OnTimerSend(nFrameType, nUtype))
		return false;

	return true;
}

bool C104Impl::OnRecvFrame(int nFrameType, int nSendNum, int nRecvNum)
{
	//序列号
	if (!OnNumRecv(nFrameType, nSendNum))
		return false;

	//KW值
	if (!OnKwRecv(nFrameType, nRecvNum))
		return false;

	//计时器
	if (!OnTimerRecv(nFrameType))
		return false;

	return true;
}

bool C104Impl::LogI(int nType, char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)pBuff;

	char buf[128];

	//连接号
	sprintf(buf, "[ConnectID:%d] ", this->GetConnectID());
	string strConnectID(buf);

	//序号
	int nSendNum = pApci->GetSendNum();
	int nRecvNum = pApci->GetRecvNum();
	sprintf(buf, "[SendNum:%d RecvNum:%d] [Local m_nSendNum:%d Local m_nRecvNum:%d]", nSendNum, nRecvNum, m_nSendNum, m_nRecvNum);
	string strNum(buf);

	//K W值
	sprintf(buf, "[K:%d W:%d] ", m_k, m_w);
	string strKW(buf);

	//类型
	ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff + sizeof(APCI));
	int nAsduType = pAsduBase->type;
	sprintf(buf, "[ASDU:%.3d] ", nAsduType);
	string strType(buf);

	//数据
	string strBuff = FormatBuffer(pBuff, nLength);

	string strMsg = strConnectID + strNum + strKW + strType + strBuff;

	m_pHost->LogCallback104(nType, TYPE_I, strMsg);

	return true;
}
bool C104Impl::LogU(int nType, char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)pBuff;

	char buf[64];

	//连接号
	sprintf(buf, "[ConnectID:%d] ", this->GetConnectID());
	string strConnectID(buf);

	//K W值
	sprintf(buf, "[K:%d W:%d] ", m_k, m_w);
	string strKW(buf);

	//类型
	string strType("[U:Unknown] ");
	int nUType = pApci->GetUType();
	switch (nUType)
	{
	case TESTFR_CON:strType = "[U:TESTFR_CON] ";
		break;
	case TESTFR_ACT:strType = "[U:TESTFR_ACT] ";
		break;
	case STOPDT_CON:strType = "[U:STOPDT_CON] ";
		break;
	case STOPDT_ACT:strType = "[U:STOPDT_ACT] ";
		break;
	case STARTDT_CON:strType = "[U:STARTDT_CON] ";
		break;
	case STARTDT_ACT:strType = "[U:STARTDT_ACT] ";
		break;
	}

	//数据
	string strBuff = FormatBuffer(pBuff, nLength);

	string strMsg = strConnectID + strKW + strType + strBuff;

	m_pHost->LogCallback104(nType, TYPE_U, strMsg);

	return true;
}
bool C104Impl::LogS(int nType, char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)pBuff;

	char buf[128];

	//连接号
	sprintf(buf, "[ConnectID:%d] ", this->GetConnectID());
	string strConnectID(buf);

	//K W值
	sprintf(buf, "[K:%d W:%d] ", m_k, m_w);
	string strKW(buf);

	//序号
	int nRecvNum = pApci->GetRecvNum();
	sprintf(buf, "[RecvNum:%d] [Local m_nSendNum:%d Local m_nRecvNum:%d]", nRecvNum, m_nSendNum, m_nRecvNum);
	string strNum(buf);

	//类型
	string strType("[S] ");

	//数据
	string strBuff = FormatBuffer(pBuff, nLength);

	string strMsg = strConnectID + strNum + strKW + strType + strBuff;

	m_pHost->LogCallback104(nType, TYPE_S, strMsg);

	return true;
}


bool C104Impl::LogN(int nType, int nFrameType, string strTag)
{
	string strFrameTag = (nFrameType == TYPE_I) ? "[TYPE_I]" : ((nFrameType == TYPE_S) ? "[TYPE_S]" : "[TYPE_U]");

	char buf[512];
	sprintf(buf, "[%s] %s: [ConnectionID:%d] m_nSendNum:%d m_nRecvNum:%d m_k:%d m_w:%d",
		(char*)strTag.c_str(),
		(char*)strFrameTag.c_str(),
		this->GetConnectID(),
		this->m_nSendNum, this->m_nRecvNum,
		this->m_k, this->m_w);

	m_pHost->LogCallback104(nType, TYPE_N, buf);

	return true;
}
string C104Impl::FormatBuffer(char* pBuff, int nMsgLen)
{
	string strDebugData("");
	char szByte[10] = { '\0' };

	for (int i = 0; i < nMsgLen; i++)
	{
		sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
		strDebugData += szByte;
	}

	return strDebugData;
}
