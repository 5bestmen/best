/*! @file alarm.cpp
<PRE>
********************************************************************************
ģ����       :  �澯����
�ļ���       :  alarm.cpp
�ļ�ʵ�ֹ��� :
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author LiJin
*  @version 1.0
*  @date    2016.09.22
*/
#include "alarm.h"

#include "data_def.h"
#include "fesapi/fesdatadef.h"
#include "log/log.h"  
#include "mail/mail.h"

#include <QDebug>
#include <QDateTime>
#include <bitset>

#if 0
/*! \fn bool CAinAlarm::SendMailToNBSvc(AINALARM_MSG *pMsg, unsigned int nWaitTime)
********************************************************************************************************* 
** \brief CAinAlarm::SendMailToNBSvc 
** \details ���͸澯��Ϣ���������߷������������߷���ת������scada
** \param pMsg 
** \param nWaitTime 
** \return bool 
** \author LiJin 
** \date 2016��10��18�� 
** \note  �������ᶩ�ĸ���Ϣ
********************************************************************************************************/
bool CAinAlarm::SendMailToNBSvc(AINALARM_MSG *pMsg, unsigned int nWaitTime)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	if (m_nNBSvcMailBoxID == 0)
	{
		m_nNBSvcMailBoxID = QueryMailBoxID("FES", "NB_SVC");
	}
	Q_ASSERT(m_nNBSvcMailBoxID != 0);
	if (m_nNBSvcMailBoxID != 0)
	{
		DMSG dmsg;
		std::memset(&dmsg, 0, sizeof(DMSG));

		dmsg.Type = MSG_EVT1;
		dmsg.SenderID = 0;
		if (m_nChannelMailBoxID!=0)
		{
			dmsg.SenderID = m_nChannelMailBoxID;
		}
		dmsg.RecverID = m_nNBSvcMailBoxID;
		dmsg.Size = sizeof(ALARM_MSG);

		memcpy(dmsg.Buff, pMsg, std::min(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));
		return SendMail("FES", &dmsg, nWaitTime);
	}
	return false;
}
#endif

bool  CAinAlarm::OnAlarm(fp64 fpValue, AINALARM_MSG& AlarmMsg)
{
HighLimitRetry:
	//�ȴ�����  ���޸���
	for (auto i : m_arrGEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{	// �����澯��
				if (GetAlarmLimitState(i->BlockNo) == FES_YES)
				{
					if (fpValue < i->LimitValue && fabs(fpValue - i->LimitValue)>m_dblDeadBand)
					{
						SetAlarmLimitState(i->BlockNo, FES_NO);
						// ���� ���� �ʼ�
						// ���ͱ�����Ϣ  ������ʽ��  ʱ��  ����������� Խ�޸��� ....
						AlarmMsg.EvtCode = AIN_ALARM_OFF;
						AlarmMsg.AlarmLimitOccNo = i->OccNo;

						SendMailToNBSvc(&AlarmMsg);

						goto HighLimit;
					}
				}
			}

		}
	}
LowLimitRetry:
	//���޸���
	for (auto i : m_arrLEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{	// �����澯��
				if (GetAlarmLimitState(i->BlockNo) == FES_YES)
				{
					if (fpValue > i->LimitValue && fabs(fpValue - i->LimitValue) > m_dblDeadBand)
					{
						SetAlarmLimitState(i->BlockNo, FES_NO);
						// ���� ���� �ʼ�
						// ���ͱ�����Ϣ  ������ʽ��  ʱ��  ����������� Խ�޸��� ....
						AlarmMsg.EvtCode = AIN_ALARM_OFF;
						AlarmMsg.AlarmLimitOccNo = i->OccNo;

						SendMailToNBSvc(&AlarmMsg);
					}
				}
			}

		}
	}
HighLimit:
	//�ٴ���Խ��
	for (auto i : m_arrGEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{
				//�Ѿ��������Խ��
				if (GetAlarmLimitState(i->BlockNo)==FES_NO)
				{
					if (fpValue > i->LimitValue && fabs(fpValue - i->LimitValue) > m_dblDeadBand)
					{
						// ���� Խ���ʼ�
						// ���ͱ�����Ϣ  ������ʽ��  ʱ��  ����������� Խ�޸��� ....
						AlarmMsg.EvtCode = AIN_ALARM_ON;
						AlarmMsg.AlarmLimitOccNo = i->OccNo;
						SendMailToNBSvc(&AlarmMsg);

						SetAlarmLimitState(i->BlockNo, FES_YES);

						goto END;

					}
				}				
			}
		}
	}
LowLimit:
	for (auto i : m_arrLEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{
				//�Ѿ��������Խ��
				if (GetAlarmLimitState(i->BlockNo) == FES_NO)
				{
					if (fpValue > i->LimitValue && fabs(fpValue - i->LimitValue) > m_dblDeadBand)
					{
						// ���� Խ���ʼ�
						// ���ͱ�����Ϣ  ������ʽ��  ʱ��  ����������� Խ�޸��� ....
						AlarmMsg.EvtCode = AIN_ALARM_ON;
						AlarmMsg.AlarmLimitOccNo = i->OccNo;
						SendMailToNBSvc(&AlarmMsg);

						SetAlarmLimitState(i->BlockNo, FES_YES);
					}
				}				
			}
		}
	}
END:
	return true;
}
/*! \fn bool CAinAlarm::Update(fp64 fpValue, int8u nQua)
********************************************************************************************************* 
** \brief CAinAlarm::Update 
** \details ����ģ�����澯
** \param fpValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016��9��22�� 
** \note 
********************************************************************************************************/
bool CAinAlarm::Update(AIN *pFB,fp64 fpValue, int8u nQua, TIMEPAK * pTm)
{
	Q_ASSERT(m_pAlarm);
	if (m_pAlarm == nullptr)
		return false;

//	Q_ASSERT(m_arrItems.empty() == false);
	if (m_arrItems.empty() == true)
		return false;
	
	m_pFB = pFB;
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	AINALARM_MSG AlarmMsg;
	memset(&AlarmMsg, 0, sizeof(AINALARM_MSG));
	QDateTime dtNow = QDateTime::currentDateTime();
	AlarmMsg.Tm.Year = dtNow.date().year();
	AlarmMsg.Tm.Month = dtNow.date().month();
	AlarmMsg.Tm.Day = dtNow.date().day();
	AlarmMsg.Tm.DayOfWeek = dtNow.date().dayOfWeek();

	AlarmMsg.Tm.Hour = dtNow.time().hour();
	AlarmMsg.Tm.Minute = dtNow.time().minute();
	AlarmMsg.Tm.Second = dtNow.time().second();
	AlarmMsg.Tm.Milliseconds = dtNow.time().msec();
	AlarmMsg.NodeOccNo = m_pFB->NodeOccNo;
	AlarmMsg.OccNo = m_pFB->OccNo;
	AlarmMsg.AlarmOccNo = GetOccNo();
	AlarmMsg.Value = fpValue;
	
	if (m_pAlarm->OnQualityGood == FES_YES)
	{// ���ֻ����Ʒ��Ϊ�õ�ʱ��Ŵ���澯
		if (nQua == QUALITY_BAD)
		{
			ClearAinAlarmState();
			return true;
		}
		else
		{
			return OnAlarm(fpValue, AlarmMsg);

		}
	}
	else
	{
		return OnAlarm(fpValue, AlarmMsg);
	}

	return true;
}
#if 0
/*! \fn void  CAinAlarm::ClearAlarmState()
********************************************************************************************************* 
** \brief CAinAlarm::ClearAlarmState 
** \details ������еı���״̬λ
** \return void 
** \author LiJin 
** \date 2016��10��18�� 
** \note 
********************************************************************************************************/
void  CAinAlarm::ClearAlarmState()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	std::memset(m_pFB->AlarmState, 0, sizeof(int32u)*MAX_AIN_ALARMLIMIT_BITCOUNT);
}
/*! \fn bool CAinAlarm::SetAlarmLimitState(int32u nLimitBlockNo, int8u nState)
********************************************************************************************************* 
** \brief CAinAlarm::SetAlarmLimitState 
** \details ���ñ���״̬����BlockNoΪ��Ű�λ�洢����״̬��
** \param nLimitBlockNo 
** \param nState 
** \return bool 
** \author LiJin 
** \date 2016��10��18�� 
** \note 
********************************************************************************************************/
bool CAinAlarm::SetAlarmLimitState(int32u nLimitBlockNo, int8u nState)
{
	Q_ASSERT(nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO);
	if (nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO)
	{
		Q_ASSERT(nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT);
		if (nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT)
		{
			unsigned int nBit = nLimitBlockNo % 32 ;
			unsigned int nIdx = nLimitBlockNo / 32;
			// ��λ
			std::bitset<sizeof(m_pFB->AlarmState[nIdx])> bitState = (m_pFB->AlarmState[nIdx]);
			bitState.set(nBit, !!nState);
			int32u nTemp = m_pFB->AlarmState[nIdx];
			m_pFB->AlarmState[nIdx] = bitState.to_ulong();

			Q_ASSERT(m_pFB->AlarmState[nIdx] != nTemp);
 
			return true;
		}
	}
	return false;
}
/*! \fn int8u CAinAlarm::GetAlarmLimitState(int32u nLimitBlockNo)
********************************************************************************************************* 
** \brief CAinAlarm::GetAlarmLimitState 
** \details ��ȡ�澯״̬λ��״̬λ�ǰ���BlockNo˳������
** \param nLimitBlockNo 
** \return int8u 
** \author LiJin 
** \date 2016��10��18�� 
** \note 
********************************************************************************************************/
int8u CAinAlarm::GetAlarmLimitState(int32u nLimitBlockNo)
{
	Q_ASSERT(nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO);
	if (nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO)
	{
		Q_ASSERT(nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT);
		if (nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT)
		{
			unsigned int nBit = nLimitBlockNo % 32;
			unsigned int nIdx = nLimitBlockNo / 32;
			// ��λ
			std::bitset<sizeof(m_pFB->AlarmState[nIdx])> bitState = (m_pFB->AlarmState[nIdx]);			

			return bitState.test(nBit);
		}
	}

	return 0;
}
#endif
/*! \fn void CAinAlarm::LoadConfig(AIN *pFB)
********************************************************************************************************* 
** \brief CAinAlarm::LoadConfig 
** \details ����������Ϣ
** \param pFB 
** \return void 
** \author LiJin 
** \date 2016��10��18�� 
** \note 
********************************************************************************************************/
void CAinAlarm::LoadConfig(AIN *pFB)
{
	Q_ASSERT(pFB);
	if (pFB == nullptr)
		return;
	m_pFB = pFB;

	Q_ASSERT(m_pAlarm);
	if (m_pAlarm == nullptr)
		return;

	// ��ȡ�����ID
	Q_ASSERT(m_pFB->ChannelOccNo != INVALID_OCCNO && m_pFB->ChannelOccNo <= MAX_OCCNO);
	if (m_pFB->ChannelOccNo != INVALID_OCCNO && m_pFB->ChannelOccNo <= MAX_OCCNO)
	{
		Q_ASSERT(m_fnGetChannel);
		if (m_fnGetChannel)
		{
			CHANNEL chan;
			bool bRet = m_fnGetChannel(m_pFB->ChannelOccNo, chan);
			Q_ASSERT(bRet);
			if (bRet)
			{
				m_nChannelMailBoxID = chan.MailBoxID;
			}
		}
	}

	// ��������
	if (m_pAlarm->DeadBandType == DEADBAND_RELATIVE)
	{
		m_dblDeadBand = m_pAlarm->DeadBand*(pFB->RangeH - pFB->RangeL);
	}
	else
	{
		m_dblDeadBand = m_pAlarm->DeadBand;
	}
	// ����ֵ��������
	m_arrGEItems.clear();
	m_arrLEItems.clear();

	for (auto i: m_arrItems)
	{
		if (i->Condition == CONDITION_GE)
		{
			m_arrGEItems.push_back(i);
		}
		else if (i->Condition == CONDITION_LE)
		{
			m_arrLEItems.push_back(i);
		}
	}
	
	std::stable_sort(m_arrGEItems.begin(), m_arrGEItems.end(), [](const AIN_ALARM_LIMIT *pLeft, const AIN_ALARM_LIMIT *pRight)->bool
	{
		return pLeft->LimitValue > pRight->LimitValue;
	}	);
	std::stable_sort(m_arrLEItems.begin(), m_arrLEItems.end(), [](const AIN_ALARM_LIMIT *pLeft, const AIN_ALARM_LIMIT *pRight)->bool
	{
		return pLeft->LimitValue < pRight->LimitValue;
	});
}

CDinAlarm::CDinAlarm()
{
	m_pFB = nullptr;
	m_nNBSvcMailBoxID = 0;
}

CDinAlarm::CDinAlarm(DIN_ALARM *pRaw)
{
	Q_ASSERT(pRaw);
	m_pAlarm = pRaw;
}

CDinAlarm::~CDinAlarm()
{
	m_arrItems.clear();
}

bool  CDinAlarm::Update(DIN *pFB,int8u nValue, int8u nQua, TIMEPAK * pTm)
{
	Q_ASSERT(m_pAlarm);
	if (m_pAlarm == nullptr)
		return false;

//	Q_ASSERT(m_arrItems.empty() == false);
	if (m_arrItems.empty() == true)
		return false;

	Q_ASSERT(pFB);
	m_pFB = pFB;
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;

	if (nValue != 0)
		nValue = 1;

	// �鿴�¿������澯��ֵ������û�ж���
	int32u nAlarmLimitOccNo = INVALID_OCCNO;
	for (auto i : m_arrItems)
	{
		Q_ASSERT(i->AlarmOccNo == GetOccNo());
		if (i->Condition == CONDITION_021)
		{
			if (nValue == 1)
			{
				nAlarmLimitOccNo = i->OccNo;
				break;
			}
		}
		else if (i->Condition == CONDITION_120)
		{
			if (nValue == 0)
			{
				nAlarmLimitOccNo = i->OccNo;
				break;
			}
		}
		else
		{
			Q_ASSERT(false);
			return false;
		}
	}

	if (nAlarmLimitOccNo == INVALID_OCCNO)
	{// û�ж���澯��ֵ
		return false;
	}

	Q_ASSERT(nAlarmLimitOccNo <= MAX_OCCNO);

	DINALARM_MSG AlarmMsg;
	memset(&AlarmMsg, 0, sizeof(DINALARM_MSG));

	if (pTm == nullptr)
	{
		QDateTime dtNow = QDateTime::currentDateTime();
		AlarmMsg.Tm.Year = dtNow.date().year();
		AlarmMsg.Tm.Month = dtNow.date().month();
		AlarmMsg.Tm.Day = dtNow.date().day();
		AlarmMsg.Tm.DayOfWeek = dtNow.date().dayOfWeek();

		AlarmMsg.Tm.Hour = dtNow.time().hour();
		AlarmMsg.Tm.Minute = dtNow.time().minute();
		AlarmMsg.Tm.Second = dtNow.time().second();
		AlarmMsg.Tm.Milliseconds = dtNow.time().msec();
	}
	else
	{
		std::memcpy(&AlarmMsg.Tm, pTm, sizeof(TIMEPAK));
	}

	if (m_pAlarm->OnQualityGood == FES_YES)
	{// ���ֻ����Ʒ��Ϊ�õ�ʱ��Ŵ���澯
		if (nQua == QUALITY_BAD)
		{
			ClearDinAlarmState();
			return true;
		}
		else
		{
			AlarmMsg.Value = nValue;
			AlarmMsg.OccNo = m_pFB->OccNo;
			AlarmMsg.AlarmOccNo = GetOccNo();
			AlarmMsg.AlarmLimitOccNo = nAlarmLimitOccNo;
			AlarmMsg.NodeOccNo = m_pFB->NodeOccNo;
			SendMailToNBSvc(&AlarmMsg);
		}
	}
	else
	{
		AlarmMsg.Value = nValue;
		AlarmMsg.OccNo = m_pFB->OccNo;
		AlarmMsg.AlarmOccNo = GetOccNo();
		AlarmMsg.AlarmLimitOccNo = nAlarmLimitOccNo;
		AlarmMsg.NodeOccNo = m_pFB->NodeOccNo;
		SendMailToNBSvc(&AlarmMsg);
	}
	return true;
}

void CDinAlarm::LoadConfig(DIN *pFB)
{
	Q_ASSERT(pFB);
	m_pFB = pFB;
}

/** @}*/
