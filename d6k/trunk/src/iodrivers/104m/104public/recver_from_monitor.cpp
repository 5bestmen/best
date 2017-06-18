
#include "recver.h"
#include <algorithm>
#include "104impl.h"
#include "ihost.h"
#include "ace/Date_Time.h" 
#include "ace/OS_NS_sys_time.h"
#include "macrodefine.h"
#include "fesapi/fesapi.h"
#include "mail/mail.h"
#include "channel.h"
//////////////////////////////////////////////////////////////////////////
//Control 从 Monitor 接收到消息
//////////////////////////////////////////////////////////////////////////

//总召确认
void CRecver::OnRecvGeneralCallConfirm(char* pBuff, int nLength)
{
	//IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU100* pAsdu100 = (ASDU100*)(pBuff);
	/*
	//回确认至后台
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nCtrlType = (pAsdu100->cot.GetCot() == COT_ACTCON)?TELECTRL_ACK_GENERALCALL:TELECTRL_ACK_GENERALCALL_OVER;

	//if(!pKernel->PushCommand(-1,-1,telectrl))//GroupID=-1,ChannelID=-1表示前置内核遥控队列
	bool bresult = pKernel->PushCommand(-1,-1,telectrl);//GroupID=-1,ChannelID=-1表示前置内核遥控队列

	if(!bresult)
	{
			char buf[256];
			sprintf(buf,"OnRecvGeneralCallConfirm PushCommand failed: GroupID%d  DeviceID:%d CtrlType:%d",nGroupID,nDeviceID,telectrl.m_nCtrlType);
			m_pHost->LogCallback104(MCB_ERROR,TYPE_I,buf);
	}

	string strMsg = telectrl.Dump();
	strMsg += (bresult)?"[Generall ACK Send to FES OK]":"[Generall ACK Send to FES FAILED]";
	m_pHost->getKernel()->Log2File("log/telectrl_driver_recv_from_device_generalcall.log",(char*)strMsg.c_str(),true);
	*/
}

void CRecver::OnRecvSyncTimeConfirm(char* pBuff, int nLength)
{
	ASDU103* pAsdu103 = (ASDU103*)(pBuff);

	//  IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	/*
	//回确认至后台
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nCtrlType = TELECTRL_ACK_SYNCTIME;

	//if(!pKernel->PushCommand(-1,-1,telectrl))//GroupID=-1,ChannelID=-1表示前置内核遥控队列
	bool bresult = pKernel->PushCommand(-1,-1,telectrl);//GroupID=-1,ChannelID=-1表示前置内核遥控队列

	if(!bresult)
	{
			char buf[256];
			sprintf(buf,"OnRecvSyncTimeConfirm PushCommand failed: GroupID%d  DeviceID:%d CtrlType:%d",nGroupID,nDeviceID,telectrl.m_nCtrlType);
			m_pHost->LogCallback104(MCB_ERROR,TYPE_I,buf);
	}

	string strMsg = telectrl.Dump();
	strMsg += (bresult)?"[Synctime ACK Send to FES OK]":"[Synctime ACK Send to FES FAILED]";
	m_pHost->getKernel()->Log2File("log/telectrl_driver_recv_from_device_synctime.log",(char*)strMsg.c_str(),true);
	*/
}

//归一化测量值
void CRecver::OnRecvAnalogNormalized(char* pBuff, int nLength)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
	if (pAsduBase->IsSQ1())
	{
		ASDU9_SQ1* pAsdu9SQ1 = (ASDU9_SQ1*)pBuff;

		int nDeviceAddr = pAsdu9SQ1->asduAddr.GetAddr();

		int nCount = pAsdu9SQ1->GetItemCount();

		int nPointID = pAsdu9SQ1->infoaddr.GetAddr();

		nPointID = nPointID - nAnalogStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu9SQ1->GetValue(i);

			QDS qds = pAsdu9SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);
			
			IoSetAinValue(nOccNo, fValue, nQuality);
	
			nPointID++;
		}
	}
	else
	{
		ASDU9_SQ0* pAsdu9SQ0 = (ASDU9_SQ0*)pBuff;

		int nDeviceAddr = pAsdu9SQ0->asduAddr.GetAddr();

		int nCount = pAsdu9SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu9SQ0->m_data[i].infoaddr.GetAddr();
			nPointID = nPointID - nAnalogStartAddr;

			float fValue = pAsdu9SQ0->GetValue(i);

			QDS qds = pAsdu9SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, nQuality);
		}
	}
}
//带短时标的归一化测量值
bool CRecver::OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU10_SQ0* pAsdu10SQ0 = (ASDU10_SQ0*)pBuff;

	int nCount = pAsdu10SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu10SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu10SQ0->GetValue(i);

		QDS qds = pAsdu10SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

		IoSetAinValue(nOccNo, fValue, nQuality);
	}

	return true;
}
//标度化测量值
void CRecver::OnRecvAnalogScaled(char* pBuff, int nLength)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
	if (pAsduBase->IsSQ1())
	{
		ASDU11_SQ1* pAsdu11SQ1 = (ASDU11_SQ1*)pBuff;

		int nDeviceAddr = pAsdu11SQ1->asduAddr.GetAddr();

		int nCount = pAsdu11SQ1->GetItemCount();

		int nPointID = pAsdu11SQ1->infoaddr.GetAddr();

		nPointID = nPointID - nAnalogStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu11SQ1->GetValue(i);

			QDS qds = pAsdu11SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, nQuality);

			nPointID++;
		}
	}
	else
	{
		ASDU11_SQ0* pAsdu11SQ0 = (ASDU11_SQ0*)pBuff;

		int nDeviceAddr = pAsdu11SQ0->asduAddr.GetAddr();

		int nCount = pAsdu11SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu11SQ0->m_data[i].infoaddr.GetAddr();
			nPointID = nPointID - nAnalogStartAddr;

			float fValue = pAsdu11SQ0->GetValue(i);

			QDS qds = pAsdu11SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, nQuality);
		}
	}
}

//带短时标的标度化测量值
bool CRecver::OnRecvAnalogScaledShortTime(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU12_SQ0* pAsdu12SQ0 = (ASDU12_SQ0*)pBuff;

	int nCount = pAsdu12SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu12SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu12SQ0->GetValue(i);

		QDS qds = pAsdu12SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

		IoSetAinValue(nOccNo, fValue, nQuality);
	}


	return true;
}
//短浮点数测量值
void CRecver::OnRecvAnalogShortFloat(char* pBuff, int nLength)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
	if (pAsduBase->IsSQ1())
	{
		ASDU13_SQ1* pAsdu13SQ1 = (ASDU13_SQ1*)pBuff;

		int nDeviceAddr = pAsdu13SQ1->asduAddr.GetAddr();

		int nCount = pAsdu13SQ1->GetItemCount();

		int nPointID = pAsdu13SQ1->infoaddr.GetAddr();

		nPointID = nPointID - nAnalogStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu13SQ1->GetValue(i);

			QDS qds = pAsdu13SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, nQuality);

			nPointID++;
		}
	}
	else
	{
		ASDU13_SQ0* pAsdu13SQ0 = (ASDU13_SQ0*)pBuff;

		int nDeviceAddr = pAsdu13SQ0->asduAddr.GetAddr();

		int nCount = pAsdu13SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu13SQ0->m_data[i].infoaddr.GetAddr();
			nPointID = nPointID - nAnalogStartAddr;

			float fValue = pAsdu13SQ0->GetValue(i);

			QDS qds = pAsdu13SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, nQuality);
		}
	}
}

//带短时标的短浮点数测量值
bool CRecver::OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU14_SQ0* pAsdu14SQ0 = (ASDU14_SQ0*)pBuff;

	int nCount = pAsdu14SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu14SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu14SQ0->GetValue(i);

		QDS qds = pAsdu14SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

		IoSetAinValue(nOccNo, fValue, nQuality);
	}

	return true;
}
//不带品质描述词的归一化测量值
bool CRecver::OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
	if (pAsduBase->IsSQ1())
	{
		ASDU21_SQ1* pAsdu21SQ1 = (ASDU21_SQ1*)pBuff;

		int nCount = pAsdu21SQ1->GetItemCount();

		int nPointID = pAsdu21SQ1->m_infoaddr.GetAddr();

		nPointID = nPointID - nAnalogStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu21SQ1->GetValue(i);

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, 0);

			nPointID++;
		}
	}
	else
	{
		ASDU21_SQ0* pAsdu21SQ0 = (ASDU21_SQ0*)pBuff;

		int nCount = pAsdu21SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu21SQ0->m_data[i].m_infoaddr.GetAddr();
			nPointID = nPointID - nAnalogStartAddr;

			float fValue = pAsdu21SQ0->GetValue(i);

			int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

			IoSetAinValue(nOccNo, fValue, 0);
		}
	}

	return true;
}

//带长时标的归一化测量值
bool CRecver::OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU34_SQ0* pAsdu34SQ0 = (ASDU34_SQ0*)pBuff;

	int nCount = pAsdu34SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu34SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu34SQ0->GetValue(i);

		QDS qds = pAsdu34SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

		IoSetAinValue(nOccNo, fValue, nQuality);
	}

	return true;
}

//带长时标的标度化测量值
bool CRecver::OnRecvAnalogScaledLongTime(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU35_SQ0* pAsdu35SQ0 = (ASDU35_SQ0*)pBuff;

	int nCount = pAsdu35SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu35SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu35SQ0->GetValue(i);

		QDS qds = pAsdu35SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

		IoSetAinValue(nOccNo, fValue, nQuality);
	}


	return true;
}

//带长时标的短浮点数测量值
bool CRecver::OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen)
{
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU36_SQ0* pAsdu36SQ0 = (ASDU36_SQ0*)pBuff;

	int nCount = pAsdu36SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu36SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu36SQ0->GetValue(i);

		QDS qds = pAsdu36SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetAinOccNoByAddr(nPointID);

		IoSetAinValue(nOccNo, fValue, nQuality);
	}


	return true;
}

void CRecver::OnRecvBinarySinglePoint(char* pBuff, int nLength)
{
	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU1_SQ1* pAsdu1SQ1 = (ASDU1_SQ1*)pBuff;

		int nDeviceAddr = pAsdu1SQ1->asduAddr.GetAddr();

		int nCount = pAsdu1SQ1->GetItemCount();

		int nPointID = pAsdu1SQ1->infoaddr.GetAddr();

		nPointID = nPointID - nBinaryStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu1SQ1->GetValue(i);//>m_siq[i];

			SIQ siq = pAsdu1SQ1->GetSIQ(i);

			int nQuality = QUALITY_NORMAL;

			if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
			if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (siq.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

			bool bRet=IoSetDinValue(nOccNo, nValue, nQuality);


			if (!bRet)
			{
				nPointID++;
				continue;
			}

			//TODO SOE  MAIL 

			//往debugger发COS
			char cosbuf[256];
			sprintf(cosbuf, "PointID:%d Value:%d", nPointID, nValue);
			//  m_pHost->getKernel()->SoeMsg(SOE_COS,cosbuf);

			nPointID++;
		}
	}
	else
	{
		ASDU1_SQ0* pAsdu1SQ0 = (ASDU1_SQ0*)pBuff;

		int nDeviceAddr = pAsdu1SQ0->asduAddr.GetAddr();

		int nCount = pAsdu1SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu1SQ0->m_data[i].infoaddr.GetAddr();
			nPointID = nPointID - nBinaryStartAddr;
			int nValue = pAsdu1SQ0->GetValue(i);//>m_siq[i].siq;

			SIQ siq = pAsdu1SQ0->GetSIQ(i);

			int nQuality = QUALITY_NORMAL;

			if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
			if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (siq.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

			bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

			if (!bRet)
			{
				nPointID++;
				continue;
			}

			//往debugger发COS
			char cosbuf[256];
			sprintf(cosbuf, "PointID:%d Value:%d", nPointID, nValue);
		}
	}
}
void CRecver::OnRecvBinaryDoublePoint(char* pBuff, int nLength)
{
	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU3_SQ1* pAsdu3SQ1 = (ASDU3_SQ1*)pBuff;

		int nDeviceAddr = pAsdu3SQ1->asduAddr.GetAddr();

		int nCount = pAsdu3SQ1->GetItemCount();

		int nPointID = pAsdu3SQ1->infoaddr.GetAddr();

		nPointID = nPointID - nBinaryStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu3SQ1->GetValue(i);//>m_siq[i];

			DIQ diq = pAsdu3SQ1->GetDIQ(i);

			int nQuality = QUALITY_NORMAL;

			if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
			if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (diq.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

			bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

			if (!bRet)
			{
				nPointID++;
				continue;
			}

		}
	}
	else
	{
		ASDU3_SQ0* pAsdu3SQ0 = (ASDU3_SQ0*)pBuff;

		int nDeviceAddr = pAsdu3SQ0->asduAddr.GetAddr();

		int nCount = pAsdu3SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu3SQ0->m_data[i].infoaddr.GetAddr();
			nPointID = nPointID - nBinaryStartAddr;
			int nValue = pAsdu3SQ0->GetValue(i);//>m_siq[i].siq;

			DIQ diq = pAsdu3SQ0->GetDIQ(i);

			int nQuality = QUALITY_NORMAL;

			if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
			if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (diq.IV == 1) nQuality |= QUALITY_INVALID;

			int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

			bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

			if (!bRet)
			{
				nPointID++;
				continue;
			}
		}
	}
}
void CRecver::OnRecvBinaryGroupPoint(char* pBuff, int nLength)
{
	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU20_SQ1* pAsdu20SQ1 = (ASDU20_SQ1*)pBuff;

		int nCount = pAsdu20SQ1->GetItemCount();

		int nPointID = pAsdu20SQ1->m_infoaddr.GetAddr();

		nPointID = nPointID - nBinaryStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			nPointID += 16 * i;

			int nValue = pAsdu20SQ1->GetValue(i);//m_siq[i].SPI;

			QDS qds = pAsdu20SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			for (int k = 0; k < 16; k++)
			{
				int nBitValue = (nValue & (1 << k)) >> k;

				int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID+k);

				bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);
			}
		}
	}
	else
	{
		ASDU20_SQ0* pAsdu20SQ0 = (ASDU20_SQ0*)pBuff;

		int nCount = pAsdu20SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu20SQ0->m_data[i].m_infoaddr.GetAddr();
			nPointID = nPointID - nBinaryStartAddr;
			int nValue = pAsdu20SQ0->GetValue(i);//>m_siq[i].siq;

			QDS qds = pAsdu20SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			for (int k = 0; k < 16; k++)
			{
				int nBitValue = (nValue & (1 << k)) >> k;

				int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID + k);

				bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);
			}
		}
	}
}
void CRecver::OnRecvBinarySinglePointTime(char* pBuff, int nLength)
{
	ASDU30_SQ0* pAsdu30SQ0 = (ASDU30_SQ0*)pBuff;

	int nDeviceAddr = pAsdu30SQ0->asduAddr.GetAddr();

	int nCount = pAsdu30SQ0->GetItemCount();

	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu30SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu30SQ0->GetValue(i);//>m_data[i].m_siq;
		CP56Time2a time = pAsdu30SQ0->GetTime(i);

		SIQ siq = pAsdu30SQ0->GetSIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (siq.IV == 1) nQuality |= QUALITY_INVALID;

		//保存值
		int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

		bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

		if (!bRet)
		{
			continue;
		}

		//往SCADA发变位
	   /* NBM_FES_EVENT event;
		event.m_nMsgType = NBMT_FES_EVENT;
		event.m_nMsgLen = sizeof(NBM_FES_EVENT) - sizeof(NBM_BASE);

		event.m_nStationID = m_pHost->getGroupID();
		event.m_nDeviceID = nDeviceID;
		event.m_nDataID = nPointID;
		event.m_nDataType = DATATYPE_BINARY;
		event.m_fValue = nValue;
		event.m_cpTime = time;

		if(m_pHost->getKernel()->PushScdEvent(&event))
		{
				char buf[256];
				sprintf(buf,"PUSH  StationID:%d DeviceID:%d PointID:%d",m_pHost->getGroupID(),nDeviceID,nPointID);

				m_pHost->getKernel()->TraceMsg(TRACE_DATA,buf);
		}

		//往debugger发SOE
		char soebuf[256];
		sprintf(soebuf,"PointID:%d Value:%d Time:%s",nPointID,nValue,time.Dump().c_str());
		m_pHost->getKernel()->SoeMsg(SOE_SOE,soebuf);*/
	}
}

void CRecver::OnRecvBinaryDoublePointTime(char* pBuff, int nLength)
{
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU31_SQ0* pAsdu31SQ0 = (ASDU31_SQ0*)pBuff;

	int nDeviceAddr = pAsdu31SQ0->asduAddr.GetAddr();

	int nCount = pAsdu31SQ0->GetItemCount();

	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu31SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu31SQ0->GetValue(i);//>m_data[i].m_siq;
		CP56Time2a time = pAsdu31SQ0->m_data[i].m_time;

		DIQ diq = pAsdu31SQ0->GetDIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (diq.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

		bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

		if (!bRet)
		{
			continue;
		}

		DIN* pDin;

		GetDinPtr(nOccNo, &pDin);

		QString str = QObject::tr("Channel:%1,Point:%2,Occurs:SOE Event,Value:%3").
							arg(m_pHost->GetChannelOccNo()).
							arg(nOccNo).
							arg(nValue);

		IoAlarmMsg(m_pHost->GetChannelOccNo(), MSG_EVT6,str.toStdString().c_str(),nullptr);


		/*  NBM_FES_EVENT event;
		  event.m_nMsgType = NBMT_FES_EVENT;
		  event.m_nMsgLen = sizeof(NBM_FES_EVENT) - sizeof(NBM_BASE);

		  event.m_nStationID = m_pHost->getGroupID();
		  event.m_nDeviceID = nDeviceID;
		  event.m_nDataID = nPointID;
		  event.m_nDataType = DATATYPE_BINARY;
		  event.m_fValue = nValue;
		  event.m_cpTime = time;

		  if(m_pHost->getKernel()->PushScdEvent(&event))
		  {
				  char buf[256];
				  sprintf(buf,"PUSH  StationID:%d DeviceID:%d PointID:%d",m_pHost->getGroupID(),nDeviceID,nPointID);

				  m_pHost->getKernel()->TraceMsg(TRACE_DATA,buf);
		  }

		  //往debugger发SOE
		  char soebuf[256];
		  sprintf(soebuf,"PointID:%d Value:%d Time:%s",nPointID,nValue,time.Dump().c_str());
		  m_pHost->getKernel()->SoeMsg(SOE_SOE,soebuf);*/
	}
}

void CRecver::OnRecvBinarySinglePointShortTime(char* pBuff, int nLength)
{
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU2_SQ0* pAsdu2SQ0 = (ASDU2_SQ0*)pBuff;

	int nDeviceAddr = pAsdu2SQ0->asduAddr.GetAddr();

	int nCount = pAsdu2SQ0->GetItemCount();

	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu2SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu2SQ0->GetValue(i);//>m_data[i].m_siq;
		CP24Time2a time = pAsdu2SQ0->GetTime(i);

		SIQ siq = pAsdu2SQ0->GetSIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (siq.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

		bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

		if (!bRet)
		{
			continue;
		}

		DIN* pDin;

		GetDinPtr(nOccNo, &pDin);

		QString str = QObject::tr("Channel:%1,Point:%2,Occurs:SOE Event,Value:%3").
			arg(m_pHost->GetChannelOccNo()).
			arg(nOccNo).
			arg(nValue);

		IoAlarmMsg(m_pHost->GetChannelOccNo(), MSG_EVT6, str.toStdString().c_str(), nullptr);

		/*  NBM_FES_EVENT event;
		  event.m_nMsgType = NBMT_FES_EVENT;
		  event.m_nMsgLen = sizeof(NBM_FES_EVENT) - sizeof(NBM_BASE);

		  event.m_nStationID = m_pHost->getGroupID();
		  event.m_nDeviceID = nDeviceID;
		  event.m_nDataID = nPointID;
		  event.m_nDataType = DATATYPE_BINARY;
		  event.m_fValue = nValue;

		  ACE_Date_Time tvTime(ACE_OS::gettimeofday());
		  event.m_cpTime.year = tvTime.year();
		  event.m_cpTime.month = tvTime.month();
		  event.m_cpTime.day = tvTime.day();
		  event.m_cpTime.hour = tvTime.hour();

		  event.m_cpTime.msH = time.msH;
		  event.m_cpTime.msL = time.msL;
		  event.m_cpTime.minute = time.minute;


		  if(m_pHost->getKernel()->PushScdEvent(&event))
		  {
				  char buf[256];
				  sprintf(buf,"PUSH  StationID:%d DeviceID:%d PointID:%d",m_pHost->getGroupID(),nDeviceID,nPointID);

				  m_pHost->getKernel()->TraceMsg(TRACE_DATA,buf);
		  }

		  //往debugger发COS
		  char cosbuf[256];
		  sprintf(cosbuf,"PointID:%d Value:%d Time:%s",nPointID,nValue,time.Dump().c_str());
		  m_pHost->getKernel()->SoeMsg(SOE_COS,cosbuf);*/
	}
}

void CRecver::OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength)
{
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU4_SQ0* pAsdu4SQ0 = (ASDU4_SQ0*)pBuff;

	int nDeviceAddr = pAsdu4SQ0->asduAddr.GetAddr();

	int nCount = pAsdu4SQ0->GetItemCount();

	int nBinaryStartAddr = m_pImpl->GetParam()->m_nBinaryStartAddr;

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu4SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu4SQ0->GetValue(i);//>m_data[i].m_siq;
		CP24Time2a time = pAsdu4SQ0->m_data[i].m_time;

		DIQ diq = pAsdu4SQ0->GetDIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (diq.IV == 1) nQuality |= QUALITY_INVALID;

		int32u nOccNo = m_pHost->getKernel()->GetDinOccNoByAddr(nPointID);

		bool bRet = IoSetDinValue(nOccNo, nValue, nQuality);

		if (!bRet)
		{
			continue;
		}
		DIN* pDin;

		GetDinPtr(nOccNo, &pDin);

		QString str = QObject::tr("Channel:%1,Point:%2,Occurs:SOE Event,Value:%3").
			arg(m_pHost->GetChannelOccNo()).
			arg(nOccNo).
			arg(nValue);

		IoAlarmMsg(m_pHost->GetChannelOccNo(), MSG_EVT6, str.toStdString().c_str(), nullptr);
		/*   NBM_FES_EVENT event;
		   event.m_nMsgType = NBMT_FES_EVENT;
		   event.m_nMsgLen = sizeof(NBM_FES_EVENT) - sizeof(NBM_BASE);

		   event.m_nStationID = m_pHost->getGroupID();
		   event.m_nDeviceID = nDeviceID;
		   event.m_nDataID = nPointID;
		   event.m_nDataType = DATATYPE_BINARY;
		   event.m_fValue = nValue;

		   ACE_Date_Time tvTime(ACE_OS::gettimeofday());
		   event.m_cpTime.year = tvTime.year();
		   event.m_cpTime.month = tvTime.month();
		   event.m_cpTime.day = tvTime.day();
		   event.m_cpTime.hour = tvTime.hour();

		   event.m_cpTime.msH = time.msH;
		   event.m_cpTime.msL = time.msL;
		   event.m_cpTime.minute = time.minute;

		   if(m_pHost->getKernel()->PushScdEvent(&event))
		   {
				   char buf[256];
				   sprintf(buf,"PUSH  StationID:%d DeviceID:%d PointID:%d",m_pHost->getGroupID(),nDeviceID,nPointID);

				   m_pHost->getKernel()->TraceMsg(TRACE_DATA,buf);
		   }*/
	}
}

//电能脉冲召唤确认
void CRecver::OnRecvKwhGeneralCallConfirm(char* pBuff, int nLength)
{

}

void CRecver::OnRecvKwh(char* pBuff, int nLength)
{
	//   IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU15_SQ1* pAsdu15SQ1 = (ASDU15_SQ1*)pBuff;
		//int nDeviceID = pAsdu15SQ1->asduAddr.GetAddr();
		int nCount = pAsdu15SQ1->GetItemCount();
		int nPointID = pAsdu15SQ1->infoaddr.GetAddr();

		int nKwhStartAddr = m_pImpl->GetParam()->m_nKwhStartAddr;
		nPointID = nPointID - nKwhStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu15SQ1->m_data[i].m_nValue;
			//  pKernel->SetKwhValue(nGroupID,nChannelID,nDeviceID,nPointID,nValue);

			char buf[128];
			sprintf(buf, "M4C  GroupID:%d ChannelID:%d DeviceID:%d StartAddr:%d PointID:%d nValue:%d Count:%d",
				nGroupID, nChannelID, nDeviceID, nKwhStartAddr, nPointID, nValue, nCount);

			nPointID++;
		}
	}
	else
	{
		ASDU15_SQ0* pAsdu15SQ0 = (ASDU15_SQ0*)pBuff;
		int nCount = pAsdu15SQ0->GetItemCount();
		int nKwhStartAddr = m_pImpl->GetParam()->m_nKwhStartAddr;

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu15SQ0->m_data[i].m_nValue;
			int nPointID = pAsdu15SQ0->m_data[i].m_infoaddr.GetAddr();
			nPointID = nPointID - nKwhStartAddr;

			//    pKernel->SetKwhValue(nGroupID,nChannelID,nDeviceID,nPointID,nValue);

			char buf[128];
			sprintf(buf, "M4C  GroupID:%d ChannelID:%d DeviceID:%d StartAddr:%d PointID:%d nValue:%d Count:%d",
				nGroupID, nChannelID, nDeviceID, nKwhStartAddr, nPointID, nValue, nCount);
		}
	}
}

void CRecver::OnRecvKwhShortTime(char* pBuff, int nLength)
{
	//   IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU16_SQ0* pAsdu16SQ0 = (ASDU16_SQ0*)pBuff;
	int nCount = pAsdu16SQ0->GetItemCount();
	int nKwhStartAddr = m_pImpl->GetParam()->m_nKwhStartAddr;

	for (int i = 0; i < nCount; i++)
	{
		float fValue = pAsdu16SQ0->m_data[i].m_fValue;
		int nPointID = pAsdu16SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nKwhStartAddr;

		//     pKernel->SetKwhValue(nGroupID,nChannelID,nDeviceID,nPointID,fValue);

		char buf[128];
		sprintf(buf, "M4C  GroupID:%d ChannelID:%d DeviceID:%d StartAddr:%d PointID:%d fValue:%f Count:%d",
			nGroupID, nChannelID, nDeviceID, nKwhStartAddr, nPointID, fValue, nCount);
	}
}

void CRecver::OnRecvKwhLongTime(char* pBuff, int nLength)
{
	//     IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU37_SQ0* pAsdu37SQ0 = (ASDU37_SQ0*)pBuff;
	int nCount = pAsdu37SQ0->GetItemCount();
	int nKwhStartAddr = m_pImpl->GetParam()->m_nKwhStartAddr;

	for (int i = 0; i < nCount; i++)
	{
		float fValue = pAsdu37SQ0->m_data[i].m_bcr.number;
		int nPointID = pAsdu37SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nKwhStartAddr;

		//        pKernel->SetKwhValue(nGroupID,nChannelID,nDeviceID,nPointID,fValue);

		char buf[128];
		sprintf(buf, "M4C  GroupID:%d ChannelID:%d DeviceID:%d StartAddr:%d PointID:%d fValue:%f Count:%d",
			nGroupID, nChannelID, nDeviceID, nKwhStartAddr, nPointID, fValue, nCount);
	}
}
//收到单点命令确认
void CRecver::OnRecvSetBinarySPAck(char* pBuff, int nLength)
{
	ASDU45* pAsdu45 = (ASDU45*)(pBuff);

	int nBinaryTelectrlStartAddr = m_pImpl->GetParam()->m_nBinaryTelectrlStartAddr;
	int nDataID = pAsdu45->m_infoaddr.GetAddr() - nBinaryTelectrlStartAddr;
	if (nDataID < 0)
	{
		return;
	}
	int nRecvAckCot = pAsdu45->cot.GetCot();
	int nRecvAckType = pAsdu45->m_sco.SelectOrExcute;

	if (m_pImpl->GetCurCommand()->m_nDataID != nDataID)
	{
		m_pImpl->GetCurCommand()->m_bCommandFinished = true;
		return;
	}

	//接收到终止指令，不予判断
	if (nRecvAckCot == COT_ACTTERM)
	{
		m_pImpl->GetCurCommand()->m_bCommandFinished = true;
		return;
	}

	if ((nRecvAckCot & 0x40) == 0x40)
	{
		m_pImpl->GetCurCommand()->m_nCtrlType = -1;
	}
	else
	{
		if (nRecvAckCot == COT_DEACTCON)
			m_pImpl->GetCurCommand()->m_nCtrlType = TELECTRL_ACK_UNSELECT;
		else if (nRecvAckType == 0) //执行
			m_pImpl->GetCurCommand()->m_nCtrlType = TELECTRL_ACK_EXECUTE;
		else if (nRecvAckType == 1)//选择
			m_pImpl->GetCurCommand()->m_nCtrlType = TELECTRL_ACK_SELECT;
		else
			m_pImpl->GetCurCommand()->m_nCtrlType = -1;
	}	
}


//收到双点命令确认
void CRecver::OnRecvSetBinaryDPAck(char* pBuff, int nLength)
{
	//    IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU46* pAsdu46 = (ASDU46*)(pBuff);
	int nBinaryTelectrlStartAddr = m_pImpl->GetParam()->m_nBinaryTelectrlStartAddr;
	int nDataID = pAsdu46->m_infoaddr.GetAddr() - nBinaryTelectrlStartAddr;
	if (nDataID < 0)
	{
		return;
	}

	int nRecvAckCot = pAsdu46->cot.GetCot();
	int nRecvAckType = pAsdu46->m_dco.SelectOrExcute;

	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_BINARY;

	if ((nRecvAckCot & 0x40) == 0x40)
	{
		telectrl.m_nCtrlType = -1;
	}
	else
	{
		if (nRecvAckCot == COT_DEACTCON)
			telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
		else if (nRecvAckType == 0) //执行
			telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
		else if (nRecvAckType == 1)//选择
			telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
		else
			telectrl.m_nCtrlType = -1;
	}

	/*     if(!pKernel->PushCommand(-1,-1,-1,telectrl))//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
		 {
				 char buf[256];
				 sprintf(buf,"OnRecvSetBinaryDPAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d",nGroupID,nDeviceID,nDataID,telectrl.m_nCtrlType);
				 m_pHost->LogCallback104(MCB_ERROR,TYPE_I,buf);
		 }*/
}

//收到归一化设点命令确认
void CRecver::OnRecvSetAnalogNormalizeAck(char* pBuff, int nLength)
{
	//    IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU48* pAsdu48 = (ASDU48*)(pBuff);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	int nDataID = pAsdu48->m_infoaddr.GetAddr() - nAnalogTelectrlStartAddr;
	if (nDataID < 0)
	{
		return;
	}
	int nRecvAckCot = pAsdu48->cot.GetCot();
	int nRecvAckType = pAsdu48->m_qos.SelectOrExcute;

	//1、上送遥控确认至前置内核
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_ANALOG;

	if (nRecvAckCot == COT_DEACTCON)
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
	else if (nRecvAckType == 0) //执行
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
	else if (nRecvAckType == 1)//选择
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
	else
		telectrl.m_nCtrlType = -1;

	bool bresult;// = pKernel->PushCommand(-1, -1, -1, telectrl);//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
	if (!bresult)
	{
		char buf[256];
		sprintf(buf, "OnRecvSetAnalogNormalizeAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d", nGroupID, nDeviceID, nDataID, telectrl.m_nCtrlType);
		m_pHost->LogCallback104(MCB_ERROR, TYPE_I, buf);
	}

	string strMsg = telectrl.Dump();
	strMsg += (bresult) ? "[Recv Analog ACK Send to FES OK]" : "[Recv Analog ACK Send to FES FAILED]";
	//   m_pHost->getKernel()->Log2File("log/telectrl_driver_recv_from_device_analog.log",(char*)strMsg.c_str(),true);
}


//收到标度化设点命令确认
void CRecver::OnRecvSetAnalogScaleAck(char* pBuff, int nLength)
{
	//  IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU49* pAsdu49 = (ASDU49*)(pBuff);
	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	int nDataID = pAsdu49->m_infoaddr.GetAddr() - nAnalogTelectrlStartAddr;
	if (nDataID < 0)
	{
		return;
	}
	int nRecvAckCot = pAsdu49->cot.GetCot();
	int nRecvAckType = pAsdu49->m_qos.SelectOrExcute;

	//1、上送遥控确认至前置内核
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_ANALOG;

	if (nRecvAckCot == COT_DEACTCON)
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
	else if (nRecvAckType == 0) //执行
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
	else if (nRecvAckType == 1)//选择
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
	else
		telectrl.m_nCtrlType = -1;

	/* if(!pKernel->PushCommand(-1,-1,-1,telectrl))//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
	 {
			 char buf[256];
			 sprintf(buf,"OnRecvSetAnalogScaleAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d",nGroupID,nDeviceID,nDataID,telectrl.m_nCtrlType);
			 m_pHost->LogCallback104(MCB_ERROR,TYPE_I,buf);
	 }*/
}



//收到短浮点数设点命令确认
void CRecver::OnRecvSetAnalogShortFloatAck(char* pBuff, int nLength)
{
	//   IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->GetGroupID();
	int nChannelID = m_pHost->GetChannelID();
	int nDeviceID = m_pHost->GetDeviceID();

	ASDU50* pAsdu50 = (ASDU50*)(pBuff);
	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	int nDataID = pAsdu50->m_infoaddr.GetAddr() - nAnalogTelectrlStartAddr;
	if (nDataID < 0)
	{
		return;
	}
	int nRecvAckCot = pAsdu50->cot.GetCot();
	int nRecvAckType = pAsdu50->m_qos.SelectOrExcute;

	//1、上送遥控确认至前置内核
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_ANALOG;

	if (nRecvAckCot == COT_DEACTCON)
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
	else if (nRecvAckType == 0) //执行
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
	else if (nRecvAckType == 1)//选择
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
	else
		telectrl.m_nCtrlType = -1;

	/*   if(!pKernel->PushCommand(-1,-1,-1,telectrl))//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
	   {
			   char buf[256];
			   sprintf(buf,"OnRecvSetAnalogShortFloatAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d",nGroupID,nDeviceID,nDataID,telectrl.m_nCtrlType);
			   m_pHost->LogCallback104(MCB_ERROR,TYPE_I,buf);
	   }*/
}

