
#include "recver.h"

#include <algorithm>

#include "104impl.h"

CRecver::CRecver(IHost* pHost, C104Impl* pImpl)
{
	m_pHost = pHost;
	m_pImpl = pImpl;
}

CRecver::~CRecver()
{

}

bool CRecver::OnReceive(char* pBuff, int nLen)
{
	//return AnalyseApdu(pBuff,nLen);

	if (nLen <= 0)
		return false;

	if (!m_104Cache.IsEmpty())
	{
		if (m_104Cache.GetCurByteLength() == 1)//只有一个启动字符
		{
			m_104Cache.CopyData(pBuff, 1);//复制长度域
			pBuff++;
			nLen--;
		}

		int nNeedBytes = m_104Cache.GetNeedByteLength();
		if (nNeedBytes <= 0)
			return false;
		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_104Cache.CopyData(pBuff, nLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_104Cache.CopyData(pBuff, nNeedBytes);
			pBuff += nNeedBytes;
			nLen -= nNeedBytes;

			bool ISOK = AnalyseApdu(m_104Cache.GetDataBuff(), m_104Cache.GetDataLen());
			m_104Cache.Clear();
			if (!ISOK)
			{
				if (m_pImpl->GetErrorCode() != ERROR_NONE)
					return false;
			}

			//解析剩余的部分
			return OnReceive(pBuff, nLen);
		}
	}
	else//缓冲区为空
	{
		//在接收区寻找启动字符
		char cStartChar = STARTCHAR;
		char* pNewBuff = std::find(pBuff, pBuff + nLen, cStartChar);

		if (pNewBuff == pBuff + nLen)
			return false;//没有找到启动字符

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//复制启动字符至缓冲区
		m_104Cache.CopyData(pBuff, 1);
		pBuff += 1;
		nLen -= 1;

		return OnReceive(pBuff, nLen);
	}

	return true;
}

bool CRecver::AnalyseApdu(char* pBuff, int nLength)
{
	if ((pBuff == NULL) || (nLength < sizeof(APCI)) || (pBuff[0] != STARTCHAR))
		return false;

	APCI* pApci = (APCI*)(pBuff);
	int nType = pApci->GetType();

	if ((nType != TYPE_I) && (nType != TYPE_U) && (nType != TYPE_S))
		return false;



	switch (nType)
	{
	case TYPE_I:
	{
		if (!AnalyseI(pBuff, nLength))
			return false;
	}
	break;
	case TYPE_U:
	{
		if (!AnalyseU(pBuff, nLength))
			return false;
	}
	break;
	case TYPE_S:
	{
		if (!AnalyseS(pBuff, nLength))
			return false;
	}
	break;
	default:
		break;
	}

	if (!m_pImpl->OnRecvFrame(nType, pApci->GetSendNum(), pApci->GetRecvNum()))
		return false;

	//在分析完上一帧报文之后
	if (!m_pImpl->GetCurCommand()->m_bCommandFinished)
	{
		m_pImpl->GetCurCommand()->m_nCtrlType = TELECTRL_REQUEST_EXECUTE;
		m_pImpl->GetCurCommand()->m_bCommandFinished = true;
		bool bRet = m_pImpl->OnCommand(m_pImpl->GetCurCommand());

	}
	else
	{
		//TODO
		//正常情况是收到选择确认 向 上一级发送告知
	}
	//m_pImpl->LogN(MCB_RECV,nType,"After OnRecvFrame");

	return true;
}

bool CRecver::AnalyseI(char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)(pBuff);

	//log
	m_pImpl->LogI(MCB_RECV, pBuff, pApci->GetApduLength() + 2);

	//解析数据
	AnalyseAsdu(pBuff, nLength);

	return true;
}

bool CRecver::AnalyseU(char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)pBuff;

	//1、Log
	m_pImpl->LogU(MCB_RECV, pBuff, nLength);

	//2、解析
	int nUType = pApci->GetUType();

	switch (nUType)
	{
	case TESTFR_CON://收到TESTFR 确认
	{
		//m_pHost->removeTimer(TIMER_ID_T1);//关闭超时定时器
	}
	break;
	case TESTFR_ACT://收到TESTFR 激活
	{
		//发送TESTFR确认
		m_pImpl->GetSender()->Send_U(TESTFR_CON);
	}
	break;
	case STARTDT_CON://收到STARTDT确认
	{
		//m_pHost->removeTimer(TIMER_ID_T1);//关闭超时定时器
		//更新运行状态
		m_pImpl->m_bIsRunning = true;//控制端启动过程结束

		//启动时总召
		NBM_TELECTRL telectrl;
		telectrl.m_nStationID = m_pHost->GetGroupID();
		telectrl.m_nDeviceID = m_pHost->GetDeviceID();
		telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
		bool IsOK = m_pImpl->OnCommand(&telectrl);
	}
	break;
	case STARTDT_ACT://收到STARTDT激活
	{
		m_pImpl->GetSender()->Send_U(STARTDT_CON);
		m_pImpl->m_bIsRunning = true;//监视端启动过程结束
	}
	break;
	case STOPDT_CON://收到STOPDT确认
	{
		//m_pHost->removeTimer(TIMER_ID_T1);//关闭超时定时器
		//更新运行状态
		m_pImpl->m_bIsRunning = false;//控制端关闭过程结束
	}
	break;
	case STOPDT_ACT://收到STOPDT激活
	{
		m_pImpl->GetSender()->Send_U(STARTDT_CON);
		m_pImpl->m_bIsRunning = false;//监视端关闭过程结束
	}
	break;
	}

	return true;
}

bool CRecver::AnalyseS(char* pBuff, int nLength)
{
	//收到S格式的确认报文，只用于确认
	APCI* pApci = (APCI*)pBuff;

	//Log
	m_pImpl->LogS(MCB_RECV, pBuff, nLength);

	return true;
}

void CRecver::AnalyseAsdu(char* pBuff, int nLength)
{
	ASDU_BASE* pAsduBase = (ASDU_BASE*)(pBuff + sizeof(APCI));

	pBuff = pBuff + sizeof(APCI);
	nLength = nLength - sizeof(APCI);

	switch (pAsduBase->type)
	{
		//遥信
	case M_SP_NA_1: // 单点信息  ASDU1
		OnRecvBinarySinglePoint(pBuff, nLength);
		break;
	case M_DP_NA_1://双点信息 ASDU3
		OnRecvBinaryDoublePoint(pBuff, nLength);
		break;
	case M_PS_NA_1: // 成组单点信息   ASDU20
		OnRecvBinaryGroupPoint(pBuff, nLength);
		break;
	case M_SP_TA_1://带短时标的单点信息 ASDU2
		OnRecvBinarySinglePointShortTime(pBuff, nLength);
		break;
	case M_SP_TB_1://带长时标的单点信息 ASDU30
		OnRecvBinarySinglePointTime(pBuff, nLength);
		break;
	case M_DP_TA_1://带短时标的双点信息 ASDU4
		OnRecvBinaryDoublePointShortTime(pBuff, nLength);
		break;
	case M_DP_TB_1://带长时标的双点信息 ASDU31
		OnRecvBinaryDoublePointTime(pBuff, nLength);
		break;

		//遥测
	case M_ME_NA_1://归一化测量值 ASDU9
		OnRecvAnalogNormalized(pBuff, nLength);
		break;
	case M_ME_TA_1://带短时标的归一化测量值 ASDU10
		OnRecvAnalogNormalizedShortTime(pBuff, nLength);
		break;
	case M_ME_NB_1://标度化测量值 ASDU11
		OnRecvAnalogScaled(pBuff, nLength);
		break;
	case M_ME_TB_1://带短时标的标度化测量值 ASDU12
		OnRecvAnalogScaledShortTime(pBuff, nLength);
		break;
	case M_ME_NC_1://短浮点数测量值 ASDU13
		OnRecvAnalogShortFloat(pBuff, nLength);
		break;
	case M_ME_TC_1://带短时标的短浮点数测量值 ASDU14
		OnRecvAnalogShortFloatShortTime(pBuff, nLength);
		break;
	case M_ME_ND_1://不带品质描述词的归一化测量值 ASDU21
		OnRecvAnalogNormalizedNoQuality(pBuff, nLength);
		break;
	case M_ME_TD_1://带长时标的归一化测量值 ASDU34
		OnRecvAnalogNormalizedLongTime(pBuff, nLength);
		break;
	case M_ME_TE_1://带长时标的标度化测量值 ASDU35
		OnRecvAnalogScaledLongTime(pBuff, nLength);
		break;
	case M_ME_TF_1://带长时标的短浮点数测量值 ASDU36
		OnRecvAnalogShortFloatLongTime(pBuff, nLength);
		break;

		//累计量
	case M_IT_NA_1: //累计量 ASDU15
		OnRecvKwh(pBuff, nLength);
		break;
	case M_IT_TA_1: //带短时标的累计量 ASDU16
		OnRecvKwhShortTime(pBuff, nLength);
		break;
	case M_IT_TB_1: //带长时标的累计量 ASDU37
		OnRecvKwhLongTime(pBuff, nLength);
		break;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
	case C_IC_NA_1:// 总召唤命令 ASDU100
			//(m_pImpl->GetPeerType() == PEER_MONITOR )?OnRecvGeneralCall(pBuff,nLength):OnRecvGeneralCallConfirm(pBuff,nLength);
			//OnRecvGeneralCallConfirm(pBuff,nLength);
		break;
	case C_CI_NA_1:// 电能脉冲召唤命令 ASDU101
		OnRecvKwhGeneralCallConfirm(pBuff, nLength);
		break;
	case C_CS_NA_1:// 时钟同步命令 ASDU103
		OnRecvSyncTimeConfirm(pBuff, nLength);
		break;
	case C_SC_NA_1://遥控(单点命令) ASDU45
		OnRecvSetBinarySPAck(pBuff, nLength);
		break;
	case C_DC_NA_1://遥控(双点命令) ASDU46
		OnRecvSetBinaryDPAck(pBuff, nLength);
		break;
	case C_SE_NA_1://遥控(归一化设点命令) ASDU48
		OnRecvSetAnalogNormalizeAck(pBuff, nLength);
		break;
	case C_SE_NB_1://遥控(标度化设点命令) ASDU49
		OnRecvSetAnalogScaleAck(pBuff, nLength);
		break;
	case C_SE_NC_1://遥控(短浮点数设点命令) ASDU50
		OnRecvSetAnalogShortFloatAck(pBuff, nLength);
		break;
	default:
		break;
	}
}

