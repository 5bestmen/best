
#include "104impl.h"
#include "sender.h"
#include "mail/mail.h"
#include "fesapi/fesapi.h"
#include <ace/Date_Time.h> 
#include <ace/OS_NS_sys_time.h>
#include <qglobal.h>
#include <QObject>
#include <QString>
#include "ihost.h"


CSender::CSender(IHost* pHost, C104Impl* pImpl)
{
	m_pHost = pHost;
	m_pImpl = pImpl;
}

CSender::~CSender()
{

}

bool CSender::Send_U(int nType)
{
	APCI apci;
	apci.head = STARTCHAR;
	apci.len = 0x04;
	apci.control0 = nType;
	apci.control1 = 0;
	apci.control2 = 0;
	apci.control3 = 0;

	if (!m_pHost->SendMessage((char*)(&apci), sizeof(APCI), m_pImpl->GetConnectID()))
		return false;

	m_pImpl->LogU(MCB_SEND, (char*)(&apci), sizeof(APCI));

	m_pImpl->OnSendFrame(TYPE_U, nType);

	//m_pImpl->LogN(MCB_SEND,TYPE_U,"After OnSendFrame TYPE_U");

	return true;
}

bool CSender::Send_S()
{
	APCI apci;
	apci.head = STARTCHAR;
	apci.len = 0x04;
	apci.control0 = 0x01;
	apci.control1 = 0;
	apci.SetRecvNum(m_pImpl->m_nRecvNum);

	if (!m_pHost->SendMessage((char*)(&apci), sizeof(APCI), m_pImpl->GetConnectID()))
	{
		return false;
	}

	m_pImpl->LogS(MCB_SEND, (char*)(&apci), sizeof(APCI));

	m_pImpl->OnSendFrame(TYPE_S);

	//m_pImpl->LogN(MCB_SEND,TYPE_S,"After OnSendFrame TYPE_S");

	return true;
}
bool CSender::Send_S_BeforeClose(int nRecvNum)
{
	APCI apci;
	apci.head = STARTCHAR;
	apci.len = 0x04;
	apci.control0 = 0x01;
	apci.control1 = 0;
	apci.SetRecvNum(nRecvNum);

	if (!m_pHost->SendMessage((char*)(&apci), sizeof(APCI), m_pImpl->GetConnectID()))
	{
		return false;
	}

	m_pImpl->LogS(MCB_SEND, (char*)(&apci), sizeof(APCI));

	m_pImpl->OnSendFrame(TYPE_S);

	//m_pImpl->LogN(MCB_SEND,TYPE_S,"After OnSendFrame TYPE_S");

	return true;
}
int CSender::Send_I(char* pBuff, int nLen)
{
	APCI* pApci = (APCI*)pBuff;

	//组织APCI
	FormatAPCI(pBuff, nLen);

	//是否能发送
	//if (m_pImpl->m_k >= DEFAULT_K)  //大于设定的值将不发送I帧格式数据
	if (m_pImpl->m_k >= m_pImpl->GetParam()->m_nDefaultK)  //大于设定的值将不发送I帧格式数据
		return SEND_ERROR_K;

	if (!m_pHost->SendMessage(pBuff, pApci->len + 2, m_pImpl->GetConnectID()))
	{
		/*IKernel* pKernel = m_pHost->getKernel();
		if(pKernel != NULL)
				pKernel->Log2File("log/Send_I.log","m_pHost->sendMessage failed",true);
   */     return SEND_ERROR_SOCKET;
	}

	//Log
	m_pImpl->LogI(MCB_SEND, pBuff, pApci->GetApduLength() + 2);

	m_pImpl->OnSendFrame(TYPE_I);

	//m_pImpl->LogN(MCB_SEND,TYPE_I,"After OnSendFrame TYPE_I");

	return SEND_OK;
}

bool CSender::OnCommand(NBM_TELECTRL* pTelectrl)
{
	Q_ASSERT(pTelectrl);
	if (!pTelectrl)
	{
		return false;
	}
	bool IsSendOk = false;

	QString str = QObject::tr("Node:%1,Dut:%2,").arg(pTelectrl->NodeOccNo).arg(pTelectrl->DoutOccNo);

	switch (pTelectrl->m_nCtrlType)
	{
	case TELECTRL_REQUEST_KWHGENERALCALL:// 电能脉冲召唤命令
		IsSendOk = OnSendKwhGeneralCall(pTelectrl);
		break;
	case TELECTRL_REQUEST_GENERALCALL:// 总召唤命令
		IsSendOk = OnSendGeneralCall(pTelectrl);
		break;
	case TELECTRL_REQUEST_SYNCTIME://时间同步
		IsSendOk = OnSendSyncTime(pTelectrl);
		break;
	case TELECTRL_REQUEST_SELECT:
	case TELECTRL_REQUEST_EXECUTE:
	case TELECTRL_REQUEST_UNSELECT:
	{
		int nCommandType = pTelectrl->m_nCommandType;
		if (nCommandType != C_SE_NA_1 && nCommandType != C_SE_NB_1 && nCommandType != C_SE_NC_1
			&& nCommandType != C_SC_NA_1 && nCommandType != C_DC_NA_1)
		{
			if (pTelectrl->m_nDataType == DATATYPE_ANALOG)
				nCommandType = m_pImpl->GetParam()->m_nAnalogTelectrlCommand;
			else
				nCommandType = m_pImpl->GetParam()->m_nBinaryTelectrlCommand;
		}
		switch (nCommandType)
		{
		case C_SE_NA_1://归一化设点48
		{
			str += QObject::tr("SendSetAnalogNormalizeRequest");
			IsSendOk = OnSendSetAnalogNormalizeRequest(pTelectrl);
			DOUT * pDout;
			GetDoutPtr(pTelectrl->DoutOccNo, &pDout);
			IoAlarmMsg(pDout->ChannelOccNo, MSG_EVT7, str.toStdString().c_str(), nullptr);

			break; 
		}
		case C_SE_NB_1://标度化设点49
		{
			str += QObject::tr("SendSetAnalogScaleRequest");
			IsSendOk = OnSendSetAnalogScaleRequest(pTelectrl);
			DOUT * pDout;
			GetDoutPtr(pTelectrl->DoutOccNo, &pDout);
			IoAlarmMsg(pDout->ChannelOccNo, MSG_EVT7, str.toStdString().c_str(), nullptr);

			break;
		}
		case C_SE_NC_1://短浮点数设点50
		{
			str += QObject::tr("SendSetAnalogShortFloatRequest");
			IsSendOk = OnSendSetAnalogShortFloatRequest(pTelectrl);
			DOUT * pDout;
			GetDoutPtr(pTelectrl->DoutOccNo, &pDout);
			IoAlarmMsg(pDout->ChannelOccNo, MSG_EVT7, str.toStdString().c_str(), nullptr);

			break;
		}		
		case C_SC_NA_1://单点命令45
		{
			str += QObject::tr("SendSetBinarySPRequest");
			IsSendOk = OnSendSetBinarySPRequest(pTelectrl);
			DOUT * pDout;
			GetDoutPtr(pTelectrl->DoutOccNo, &pDout);
			IoAlarmMsg(pDout->ChannelOccNo, MSG_EVT7, str.toStdString().c_str(), nullptr);

			break;
		}			
		case C_DC_NA_1://双点命令46
		{
			str += QObject::tr("SendSetBinaryDPRequest");
			IsSendOk = OnSendSetBinaryDPRequest(pTelectrl);
			DOUT * pDout;
			GetDoutPtr(pTelectrl->DoutOccNo, &pDout);
			IoAlarmMsg(pDout->ChannelOccNo, MSG_EVT7, str.toStdString().c_str(), nullptr);

			break;
		}			
		default:
			break;
		}
	}
	break;
	}
	
	return IsSendOk;
}

bool CSender::FormatAPCI(char* pBuff, int nAsduLen)
{
	APCI* pApci = (APCI *)pBuff;

	pApci->head = 0x68;
	pApci->len = 0x04 + nAsduLen;

	pApci->SetSendNum(m_pImpl->m_nSendNum);
	pApci->SetRecvNum(m_pImpl->m_nRecvNum);

	return true;
}

