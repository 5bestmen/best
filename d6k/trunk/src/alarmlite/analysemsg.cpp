#include "analysemsg.h"
#include "fesapi/fesdatadef.h"
#include "fesapi/fesapi.h"

CAnalyseTask::CAnalyseTask(QObject *parent, std::shared_ptr< CAlarmInfoQueue>& alarmQueue, std::shared_ptr< CAlarmInfoQueue>& alarmViewQueue)
	: QThread(parent), m_pAlarmQueue(alarmQueue), m_pAlarmViewQueue(alarmViewQueue)
{
	m_bStopped = true;
}

CAnalyseTask::~CAnalyseTask()
{
	m_bStopped = false;
}


/*! \fn void CAnalyseTask::run()
********************************************************************************************************* 
** \brief CAnalyseTask::run 
** \details 循环检测接收队列
** \return void 
** \author xingzhibing
** \date 2016年10月30日 
** \note 
********************************************************************************************************/
void CAnalyseTask::run()
{
	while (m_bStopped)
	{
		if ( m_pAlarmQueue->empty() == true)
		{
			QThread::sleep(1);
		}
		else
		{
			AlarmInfo_PTR pMsg = m_pAlarmQueue->front();
			if (pMsg)
			{
				m_pAlarmViewQueue->push_back(pMsg);
			}
			/*if (pMsg)
			{
				if (pMsg->m_Msg.Type == MSG_EVT_DIGI)
				{
					QString szAlarmDesc;
					DINALARM_MSG *pAlarmMsg = reinterpret_cast<DINALARM_MSG*> (pMsg->m_Msg.Buff);
					if (pAlarmMsg)
					{
						QString szTime;
						FormatTimeStamp(&pAlarmMsg->Tm, pMsg->m_szTimeTag);						
					}
					//解析邮件报文
					pMsg->m_fVal = pAlarmMsg->Value;
					strncpy(pMsg->m_szContent, (char*)(pMsg->m_Msg.Buff), qMin((size_t)1024 * 2,strlen((char*)(pMsg->m_Msg.Buff))));

					m_pAlarmViewQueue->push_back(pMsg);
				}
				else if (pMsg->m_Msg.Type == MSG_EVT_ANA)
				{
					QString szAlarmDesc;
					AINALARM_MSG *pAlarmMsg = reinterpret_cast<AINALARM_MSG*> (pMsg->m_Msg.Buff);
					if (pAlarmMsg)
					{
						QString szTime;
						FormatTimeStamp(&pAlarmMsg->Tm, pMsg->m_szTimeTag);
					}
					//解析邮件报文
					pMsg->m_fVal = pAlarmMsg->Value;
					strncpy(pMsg->m_szContent, (char*)(pMsg->m_Msg.Buff), qMin((size_t)1024 * 2, strlen((char*)(pMsg->m_Msg.Buff))));

					m_pAlarmViewQueue->push_back(pMsg);
				}
				else if (pMsg->m_Msg.Type==MSG_EVT_DIAG)
				{
					QString szAlarmDesc;
					DIAG_MSG *pDiagMsg = reinterpret_cast<DIAG_MSG*> (pMsg->m_Msg.Buff);
					if (pDiagMsg)
					{
						QString szTime;
						FormatTimeStamp(&pDiagMsg->Tm, pMsg->m_szTimeTag);
					}
					//解析邮件报文
					pMsg->m_fVal = 1.0;
					strncpy(pMsg->m_szContent, (char*)(pMsg->m_Msg.Buff), qMin((size_t)1024 * 2, strlen((char*)(pMsg->m_Msg.Buff))));

					m_pAlarmViewQueue->push_back(pMsg);
				}
				else if (pMsg->m_Msg.Type == MSG_EVT7 || pMsg->m_Msg.Type == MSG_EVT6)
				{
					QString szAlarmDesc;
					ALARM_MSG *pAlarmMsg = reinterpret_cast<ALARM_MSG*> (pMsg->m_Msg.Buff);
					if (pAlarmMsg)
					{
						QString szTime;
						FormatTimeStamp(&pAlarmMsg->Tm, pMsg->m_szTimeTag);
					}
					//解析邮件报文
					pMsg->m_fVal = 1.0;// pAlarmMsg->Value;
					strncpy(pMsg->m_szContent, (char*)(pMsg->m_Msg.Buff), qMin((size_t )1024 * 2, strlen((char*)(pMsg->m_Msg.Buff))));
					m_pAlarmViewQueue->push_back(pMsg);
				}
			}	*/		
			m_pAlarmQueue->pop_front();
		}
	}
}

QString CAnalyseTask::GetEvtDescByCode(int32u nCode)
{
	if (nCode == AIN_ALARM_OFF)
	{
		return QObject::tr("AIN_ALARM_OFF");
	}
	else if (nCode == AIN_ALARM_ON)
	{
		//!< 模拟量越复限告警
		return QObject::tr("AIN_ALARM_ON");
	}
	return QString::null;
}

QString CAnalyseTask::GetEventDesc(int32u nOccNo, int32u nAlarmOccNo, int32u nAlarmLimitOccNo)
{
	Q_ASSERT(nOccNo !=INVALID_OCCNO && nOccNo <=MAX_OCCNO);
	if (nOccNo ==INVALID_OCCNO || nOccNo <MAX_OCCNO)
	{
		return QString::null;
	}
	Q_ASSERT(nAlarmLimitOccNo!=INVALID_OCCNO &&nAlarmLimitOccNo<= MAX_ALARM_OCCNO);
	if (nAlarmLimitOccNo==INVALID_OCCNO || nAlarmLimitOccNo>MAX_ALARM_OCCNO)
	{
		return QString::null;
	}
	Q_ASSERT(nAlarmOccNo !=INVALID_OCCNO && nAlarmOccNo <=MAX_ALARM_OCCNO);
	if (nAlarmOccNo==INVALID_OCCNO || nAlarmOccNo>MAX_ALARM_OCCNO)
	{
		return QString::null;
	}
	
	return QString::null;

}

void CAnalyseTask::FormatTimeStamp(TIMEPAK* pak, QString& szTimeStamp)
{
	Q_ASSERT(pak);
	if (!pak)
	{
		return;
	}
	szTimeStamp = tr("%1.%2.%3--%4:%5:%6::%7").arg(pak->Year).arg(pak->Month).arg(pak->Day).arg(pak->Hour).arg(pak->Minute).arg(pak->Second).arg(pak->Milliseconds);
}
