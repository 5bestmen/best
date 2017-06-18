
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
		if (m_104Cache.GetCurByteLength() == 1)//ֻ��һ�������ַ�
		{
			m_104Cache.CopyData(pBuff, 1);//���Ƴ�����
			pBuff++;
			nLen--;
		}

		int nNeedBytes = m_104Cache.GetNeedByteLength();
		if (nNeedBytes <= 0)
			return false;
		if (nNeedBytes > nLen)//��Ҫ�ĳ��ȴ��ڽ��յ��ĳ���
		{
			m_104Cache.CopyData(pBuff, nLen);
			return true;
		}
		else//���յ��ĳ��ȴ�����Ҫ�ĳ���
		{
			//������Ҫ�Ĳ���
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

			//����ʣ��Ĳ���
			return OnReceive(pBuff, nLen);
		}
	}
	else//������Ϊ��
	{
		//�ڽ�����Ѱ�������ַ�
		char cStartChar = STARTCHAR;
		char* pNewBuff = std::find(pBuff, pBuff + nLen, cStartChar);

		if (pNewBuff == pBuff + nLen)
			return false;//û���ҵ������ַ�

		nLen = nLen - (pNewBuff - pBuff);
		pBuff = pNewBuff;

		//���������ַ���������
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

	//�ڷ�������һ֡����֮��
	if (!m_pImpl->GetCurCommand()->m_bCommandFinished)
	{
		m_pImpl->GetCurCommand()->m_nCtrlType = TELECTRL_REQUEST_EXECUTE;
		m_pImpl->GetCurCommand()->m_bCommandFinished = true;
		bool bRet = m_pImpl->OnCommand(m_pImpl->GetCurCommand());

	}
	else
	{
		//TODO
		//����������յ�ѡ��ȷ�� �� ��һ�����͸�֪
	}
	//m_pImpl->LogN(MCB_RECV,nType,"After OnRecvFrame");

	return true;
}

bool CRecver::AnalyseI(char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)(pBuff);

	//log
	m_pImpl->LogI(MCB_RECV, pBuff, pApci->GetApduLength() + 2);

	//��������
	AnalyseAsdu(pBuff, nLength);

	return true;
}

bool CRecver::AnalyseU(char* pBuff, int nLength)
{
	APCI* pApci = (APCI*)pBuff;

	//1��Log
	m_pImpl->LogU(MCB_RECV, pBuff, nLength);

	//2������
	int nUType = pApci->GetUType();

	switch (nUType)
	{
	case TESTFR_CON://�յ�TESTFR ȷ��
	{
		//m_pHost->removeTimer(TIMER_ID_T1);//�رճ�ʱ��ʱ��
	}
	break;
	case TESTFR_ACT://�յ�TESTFR ����
	{
		//����TESTFRȷ��
		m_pImpl->GetSender()->Send_U(TESTFR_CON);
	}
	break;
	case STARTDT_CON://�յ�STARTDTȷ��
	{
		//m_pHost->removeTimer(TIMER_ID_T1);//�رճ�ʱ��ʱ��
		//��������״̬
		m_pImpl->m_bIsRunning = true;//���ƶ��������̽���

		//����ʱ����
		NBM_TELECTRL telectrl;
		telectrl.m_nStationID = m_pHost->GetGroupID();
		telectrl.m_nDeviceID = m_pHost->GetDeviceID();
		telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
		bool IsOK = m_pImpl->OnCommand(&telectrl);
	}
	break;
	case STARTDT_ACT://�յ�STARTDT����
	{
		m_pImpl->GetSender()->Send_U(STARTDT_CON);
		m_pImpl->m_bIsRunning = true;//���Ӷ��������̽���
	}
	break;
	case STOPDT_CON://�յ�STOPDTȷ��
	{
		//m_pHost->removeTimer(TIMER_ID_T1);//�رճ�ʱ��ʱ��
		//��������״̬
		m_pImpl->m_bIsRunning = false;//���ƶ˹رչ��̽���
	}
	break;
	case STOPDT_ACT://�յ�STOPDT����
	{
		m_pImpl->GetSender()->Send_U(STARTDT_CON);
		m_pImpl->m_bIsRunning = false;//���Ӷ˹رչ��̽���
	}
	break;
	}

	return true;
}

bool CRecver::AnalyseS(char* pBuff, int nLength)
{
	//�յ�S��ʽ��ȷ�ϱ��ģ�ֻ����ȷ��
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
		//ң��
	case M_SP_NA_1: // ������Ϣ  ASDU1
		OnRecvBinarySinglePoint(pBuff, nLength);
		break;
	case M_DP_NA_1://˫����Ϣ ASDU3
		OnRecvBinaryDoublePoint(pBuff, nLength);
		break;
	case M_PS_NA_1: // ���鵥����Ϣ   ASDU20
		OnRecvBinaryGroupPoint(pBuff, nLength);
		break;
	case M_SP_TA_1://����ʱ��ĵ�����Ϣ ASDU2
		OnRecvBinarySinglePointShortTime(pBuff, nLength);
		break;
	case M_SP_TB_1://����ʱ��ĵ�����Ϣ ASDU30
		OnRecvBinarySinglePointTime(pBuff, nLength);
		break;
	case M_DP_TA_1://����ʱ���˫����Ϣ ASDU4
		OnRecvBinaryDoublePointShortTime(pBuff, nLength);
		break;
	case M_DP_TB_1://����ʱ���˫����Ϣ ASDU31
		OnRecvBinaryDoublePointTime(pBuff, nLength);
		break;

		//ң��
	case M_ME_NA_1://��һ������ֵ ASDU9
		OnRecvAnalogNormalized(pBuff, nLength);
		break;
	case M_ME_TA_1://����ʱ��Ĺ�һ������ֵ ASDU10
		OnRecvAnalogNormalizedShortTime(pBuff, nLength);
		break;
	case M_ME_NB_1://��Ȼ�����ֵ ASDU11
		OnRecvAnalogScaled(pBuff, nLength);
		break;
	case M_ME_TB_1://����ʱ��ı�Ȼ�����ֵ ASDU12
		OnRecvAnalogScaledShortTime(pBuff, nLength);
		break;
	case M_ME_NC_1://�̸���������ֵ ASDU13
		OnRecvAnalogShortFloat(pBuff, nLength);
		break;
	case M_ME_TC_1://����ʱ��Ķ̸���������ֵ ASDU14
		OnRecvAnalogShortFloatShortTime(pBuff, nLength);
		break;
	case M_ME_ND_1://����Ʒ�������ʵĹ�һ������ֵ ASDU21
		OnRecvAnalogNormalizedNoQuality(pBuff, nLength);
		break;
	case M_ME_TD_1://����ʱ��Ĺ�һ������ֵ ASDU34
		OnRecvAnalogNormalizedLongTime(pBuff, nLength);
		break;
	case M_ME_TE_1://����ʱ��ı�Ȼ�����ֵ ASDU35
		OnRecvAnalogScaledLongTime(pBuff, nLength);
		break;
	case M_ME_TF_1://����ʱ��Ķ̸���������ֵ ASDU36
		OnRecvAnalogShortFloatLongTime(pBuff, nLength);
		break;

		//�ۼ���
	case M_IT_NA_1: //�ۼ��� ASDU15
		OnRecvKwh(pBuff, nLength);
		break;
	case M_IT_TA_1: //����ʱ����ۼ��� ASDU16
		OnRecvKwhShortTime(pBuff, nLength);
		break;
	case M_IT_TB_1: //����ʱ����ۼ��� ASDU37
		OnRecvKwhLongTime(pBuff, nLength);
		break;

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
	case C_IC_NA_1:// ���ٻ����� ASDU100
			//(m_pImpl->GetPeerType() == PEER_MONITOR )?OnRecvGeneralCall(pBuff,nLength):OnRecvGeneralCallConfirm(pBuff,nLength);
			//OnRecvGeneralCallConfirm(pBuff,nLength);
		break;
	case C_CI_NA_1:// ���������ٻ����� ASDU101
		OnRecvKwhGeneralCallConfirm(pBuff, nLength);
		break;
	case C_CS_NA_1:// ʱ��ͬ������ ASDU103
		OnRecvSyncTimeConfirm(pBuff, nLength);
		break;
	case C_SC_NA_1://ң��(��������) ASDU45
		OnRecvSetBinarySPAck(pBuff, nLength);
		break;
	case C_DC_NA_1://ң��(˫������) ASDU46
		OnRecvSetBinaryDPAck(pBuff, nLength);
		break;
	case C_SE_NA_1://ң��(��һ���������) ASDU48
		OnRecvSetAnalogNormalizeAck(pBuff, nLength);
		break;
	case C_SE_NB_1://ң��(��Ȼ��������) ASDU49
		OnRecvSetAnalogScaleAck(pBuff, nLength);
		break;
	case C_SE_NC_1://ң��(�̸������������) ASDU50
		OnRecvSetAnalogShortFloatAck(pBuff, nLength);
		break;
	default:
		break;
	}
}

