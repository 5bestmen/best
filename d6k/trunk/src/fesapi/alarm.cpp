/*! @file alarm.cpp
<PRE>
********************************************************************************
模块名       :  告警处理
文件名       :  alarm.cpp
文件实现功能 :
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
** \details 发送告警信息到网络总线服务，由网络总线服务转发到到scada
** \param pMsg 
** \param nWaitTime 
** \return bool 
** \author LiJin 
** \date 2016年10月18日 
** \note  报警窗会订阅该消息
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
	//先处理复限  上限复限
	for (auto i : m_arrGEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{	// 曾经告警过
				if (GetAlarmLimitState(i->BlockNo) == FES_YES)
				{
					if (fpValue < i->LimitValue && fabs(fpValue - i->LimitValue)>m_dblDeadBand)
					{
						SetAlarmLimitState(i->BlockNo, FES_NO);
						// 发送 复限 邮件
						// 发送报警消息  报警格式：  时间  测点描述名称 越限复归 ....
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
	//下限复限
	for (auto i : m_arrLEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{	// 曾经告警过
				if (GetAlarmLimitState(i->BlockNo) == FES_YES)
				{
					if (fpValue > i->LimitValue && fabs(fpValue - i->LimitValue) > m_dblDeadBand)
					{
						SetAlarmLimitState(i->BlockNo, FES_NO);
						// 发送 复限 邮件
						// 发送报警消息  报警格式：  时间  测点描述名称 越限复归 ....
						AlarmMsg.EvtCode = AIN_ALARM_OFF;
						AlarmMsg.AlarmLimitOccNo = i->OccNo;

						SendMailToNBSvc(&AlarmMsg);
					}
				}
			}

		}
	}
HighLimit:
	//再处理越限
	for (auto i : m_arrGEItems)
	{
		Q_ASSERT(i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO);
		if (i->BlockNo != INVALID_OCCNO && i->BlockNo <= MAX_OCCNO)
		{
			if (i->ScanEnable == SCAN_IN)
			{
				//已经处理过的越限
				if (GetAlarmLimitState(i->BlockNo)==FES_NO)
				{
					if (fpValue > i->LimitValue && fabs(fpValue - i->LimitValue) > m_dblDeadBand)
					{
						// 发送 越限邮件
						// 发送报警消息  报警格式：  时间  测点描述名称 越限复归 ....
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
				//已经处理过的越限
				if (GetAlarmLimitState(i->BlockNo) == FES_NO)
				{
					if (fpValue > i->LimitValue && fabs(fpValue - i->LimitValue) > m_dblDeadBand)
					{
						// 发送 越限邮件
						// 发送报警消息  报警格式：  时间  测点描述名称 越限复归 ....
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
** \details 处理模拟量告警
** \param fpValue 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2016年9月22日 
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
	{// 如果只有在品质为好的时候才处理告警
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
** \details 清空所有的报警状态位
** \return void 
** \author LiJin 
** \date 2016年10月18日 
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
** \details 设置报警状态，以BlockNo为序号按位存储报警状态，
** \param nLimitBlockNo 
** \param nState 
** \return bool 
** \author LiJin 
** \date 2016年10月18日 
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
			// 置位
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
** \details 获取告警状态位，状态位是按照BlockNo顺序排序
** \param nLimitBlockNo 
** \return int8u 
** \author LiJin 
** \date 2016年10月18日 
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
			// 置位
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
** \details 加载配置信息
** \param pFB 
** \return void 
** \author LiJin 
** \date 2016年10月18日 
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

	// 获取邮箱的ID
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

	// 计算死区
	if (m_pAlarm->DeadBandType == DEADBAND_RELATIVE)
	{
		m_dblDeadBand = m_pAlarm->DeadBand*(pFB->RangeH - pFB->RangeL);
	}
	else
	{
		m_dblDeadBand = m_pAlarm->DeadBand;
	}
	// 对限值进行排序
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

	// 查看下开关量告警限值里面有没有定义
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
	{// 没有定义告警限值
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
	{// 如果只有在品质为好的时候才处理告警
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
