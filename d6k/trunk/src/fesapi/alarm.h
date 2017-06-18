/*! @file alarm.h
<PRE>
********************************************************************************
模块名       :  告警处理
文件名       :  alarm.h
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

#ifndef FES_ALARM_H
#define FES_ALARM_H
#pragma once

#include "datatypes.h" 
#include <vector>
#include <cassert>
#include <functional>
#include <map>
#include <QDebug>
 
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;
struct AIN_ALARM;
struct DIN_ALARM;
struct AIN_ALARM_LIMIT;
struct DIN_ALARM_LIMIT;
struct AINALARM_MSG;
struct DINALARM_MSG;
struct CHANNEL;
struct TIMEPAK;

template<typename IddTypeT,  typename AlarmT, typename AlarmItemT>
class CAlarm
{
public:
	CAlarm()
	{
		m_nChannelMailBoxID = 0;
		m_pFB = nullptr;
		m_pAlarm = nullptr;                 
		m_nNBSvcMailBoxID = 0;
	}

	virtual ~CAlarm()
	{

	}
public:
	/*! \fn bool AddItem(Item *pItem)
	********************************************************************************************************* 
	** \brief CAlarm<T, Item>::AddItem 
	** \details 添加报警限值，按occno排序，从低开始
	** \param pItem 
	** \return bool 
	** \author LiJin 
	** \date 2016年10月15日 
	** \note 
	********************************************************************************************************/
	bool AddItem(AlarmItemT *pItem)
	{
		assert(pItem);
		if (pItem == nullptr)
			return false;
#ifdef DEBUG
		auto it = std::find(m_arrItems.begin(), m_arrItems.end(), pItem);
		if (it != m_arrItems.end())
		{
			assert(false);
			return false;
		}
		assert(m_pAlarm);
		if (m_pAlarm)
		{
			assert(pItem->AlarmOccNo == m_pAlarm->OccNo);
		}
		// 开关报警只有2个限值
		if (sizeof(AlarmItemT) == sizeof(DIN_ALARM_LIMIT))
		{
		//	static_assert(m_arrItems.size() <= 1);
			assert(m_arrItems.size() <= 1);
		}
		for (auto i : m_arrItems)
		{
			if (i->OccNo == pItem->OccNo)
			{
				assert(false);
				return false;
			}
		}
#endif		
		m_arrItems.push_back(pItem);

	//	m_mapItems.insert(std::make_pair(pItem->OccNo, pItem));

		return true;
	}
	int32u GetOccNo()
	{
		assert(m_pAlarm);
		if (m_pAlarm)
		{
			return m_pAlarm->OccNo;
		}
		return 0;
	}
public:
	std::function< bool(int32u nChannelOccNo, CHANNEL& ch) > m_fnGetChannel;
protected:
	// 根据 报警限值 occno 修改状态位
	bool SetAlarmLimitState(int32u nLimitBlockNo, int8u nState)
	{
		Q_ASSERT(nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO);
		if (nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO)
		{
			Q_ASSERT(nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT);
			if (nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT)
			{
				unsigned int nBit = (nLimitBlockNo - 1) % 32;
				unsigned int nIdx = (nLimitBlockNo - 1) / 32;
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

	int8u GetAlarmLimitState(int32u nLimitBlockNo)
	{
		Q_ASSERT(nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO);
		if (nLimitBlockNo != INVALID_OCCNO && nLimitBlockNo <= MAX_OCCNO)
		{
			Q_ASSERT(nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT);
			if (nLimitBlockNo <= MAX_AIN_ALARM_LIMIT_COUNT)
			{
				unsigned int nBit = (nLimitBlockNo-1) % 32;
				unsigned int nIdx = (nLimitBlockNo-1) / 32;
				// 置位
				std::bitset<sizeof(m_pFB->AlarmState[nIdx])> bitState = (m_pFB->AlarmState[nIdx]);

				return bitState.test(nBit);
			}
		}

		return 0;
	}

	void ClearAinAlarmState()
	{
		Q_ASSERT(m_pFB);
		if (m_pFB == nullptr)
			return;

		std::memset(m_pFB->AlarmState, 0, sizeof(int32u)*MAX_AIN_ALARMLIMIT_BITCOUNT);
	}
	void ClearDinAlarmState()
	{
		Q_ASSERT(m_pFB);
		if (m_pFB == nullptr)
			return;

	//	std::memset(m_pFB->AlarmState, 0, sizeof(int32u)*MAX_AIN_ALARMLIMIT_BITCOUNT);
	}

	/*! \fn bool SendMailToNBSvc(DINALARM_MSG *pMsg, unsigned int nWaitTime = 60)
	********************************************************************************************************* 
	** \brief CAlarm<IddTypeT, AlarmT, AlarmItemT>::SendMailToNBSvc 
	** \details 发送开关量报警事件到网络总线
	** \param pMsg 
	** \param nWaitTime 
	** \return bool 
	** \author LiJin 
	** \date 2016年12月12日 
	** \note 
	********************************************************************************************************/
	bool SendMailToNBSvc(DINALARM_MSG *pMsg, unsigned int nWaitTime = 60)
	{
		Q_ASSERT(pMsg);
		if (pMsg == nullptr)
			return false;

		if (m_nNBSvcMailBoxID == INVALID_MAILBOX_ID)
		{
			m_nNBSvcMailBoxID = QueryMailBoxID("FES", "NB_SVC");
		}
		Q_ASSERT(m_nNBSvcMailBoxID != INVALID_MAILBOX_ID);
		if (m_nNBSvcMailBoxID != 0)
		{
			DMSG dmsg;
			std::memset(&dmsg, 0, sizeof(DMSG));

			dmsg.Type = MSG_EVT_DIGI;
			dmsg.SenderID = 0;
			if (m_nChannelMailBoxID != 0)
			{
				dmsg.SenderID = m_nChannelMailBoxID;
			}
			dmsg.RecverID = m_nNBSvcMailBoxID;
			dmsg.Size = sizeof(DINALARM_MSG);

			memcpy(dmsg.Buff, pMsg, std::min(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));
			return SendMail("FES", &dmsg, nWaitTime);
		}
		return false;
	}
	/*! \fn bool SendMailToNBSvc(AINALARM_MSG *pMsg, unsigned int nWaitTime = 60)
	********************************************************************************************************* 
	** \brief CAlarm<IddTypeT, AlarmT, AlarmItemT>::SendMailToNBSvc 
	** \details 发送模拟量报警事件到网络总线
	** \param pMsg 
	** \param nWaitTime 
	** \return bool 
	** \author LiJin 
	** \date 2016年11月2日 
	** \note 
	********************************************************************************************************/
	bool SendMailToNBSvc(AINALARM_MSG *pMsg, unsigned int nWaitTime = 60)
	{
		Q_ASSERT(pMsg);
		if (pMsg == nullptr)
			return false;

		if (m_nNBSvcMailBoxID == INVALID_MAILBOX_ID)
		{
			m_nNBSvcMailBoxID = QueryMailBoxID("FES", "NB_SVC");
		}
		Q_ASSERT(m_nNBSvcMailBoxID != INVALID_MAILBOX_ID);
		if (m_nNBSvcMailBoxID != 0)
		{
			DMSG dmsg;
			std::memset(&dmsg, 0, sizeof(DMSG));

			dmsg.Type = MSG_EVT_ANA;
			dmsg.SenderID = 0;
			if (m_nChannelMailBoxID != 0)
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
protected:
	IddTypeT *m_pFB;
	AlarmT * m_pAlarm;                  //! 内存数据库指针 
	int m_nChannelMailBoxID;
	int m_nNBSvcMailBoxID;
	std::vector<AlarmItemT*> m_arrItems;       //! 内存数据库的指针成员
//	std::map<int32u, Item *> m_mapItems;
};

/*! \struct  CAinAlarm
*   \brief  模拟量的报警定义 */
class CAinAlarm : public CAlarm<AIN,AIN_ALARM,AIN_ALARM_LIMIT>
{
public:
	CAinAlarm()
	{
		m_pFB = nullptr;
		m_nNBSvcMailBoxID = 0;
		m_dblDeadBand = 0.0f;
	}
	explicit CAinAlarm(AIN_ALARM *pRaw)
	{
		m_pAlarm = pRaw;
		m_dblDeadBand = 0;	
		m_pFB = nullptr;
		m_nNBSvcMailBoxID = 0;
		m_dblDeadBand = 0.0f;
	}
	~CAinAlarm()
	{
		m_arrItems.clear();
	}
public:
	bool Update(AIN *pFB,fp64 fpValue, int8u nQua, TIMEPAK * pTm);
	void LoadConfig(AIN *pFB);
protected:
	bool OnAlarm(fp64 fpValue, AINALARM_MSG& AlarmMsg);

private:
	double  m_dblDeadBand;
//	AIN *m_pFB; //! 模拟量的共享内存

	std::vector<AIN_ALARM_LIMIT*> m_arrGEItems;
	std::vector<AIN_ALARM_LIMIT*> m_arrLEItems;

//	int m_nNBSvcMailBoxID;
};


class CDinAlarm : public CAlarm<DIN,DIN_ALARM, DIN_ALARM_LIMIT>
{
public:
	CDinAlarm();
	explicit CDinAlarm(DIN_ALARM *pRaw);
	~CDinAlarm();

public:
	bool Update(DIN *pFB,int8u nValue, int8u nQua, TIMEPAK * pTm);
	void LoadConfig(DIN *pFB);
private:
//	DIN *m_pFB; //! 开关量的共享内存

//	int m_nNBSvcMailBoxID;

};
/** @}*/

#endif // FES_ALARM_H
