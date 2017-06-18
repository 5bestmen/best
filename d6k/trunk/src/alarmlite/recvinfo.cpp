#include "recvinfo.h"
//#include "boost/thread.hpp"

CRecvInfoTask::CRecvInfoTask(QObject *parent, std::shared_ptr< CAlarmInfoQueue>& alarmQueue)
	: QThread(parent), m_pAlarmQueue(alarmQueue)
{
	memset(m_buff, 0, sizeof m_buff);
	m_bStopped = false;
	m_nMailBoxID = 0;
}

CRecvInfoTask::~CRecvInfoTask()
{
	m_bStopped = false;
}

/*! \fn void CRecvInfoTask::run()
********************************************************************************************************* 
** \brief CRecvInfoTask::run 
** \details �����߳�
** \return void 
** \author xingzhibing
** \date 2016��10��30�� 
** \note 
********************************************************************************************************/
void CRecvInfoTask::run()
{
	bool bRet = false;

	if (m_nMailBoxID == 0)
	{
		m_nMailBoxID = QueryMailBoxID("SCADA", "ALARM_SCD_WIN");
		Q_ASSERT(m_nMailBoxID > 0);
	} 

	DMSG msg;
	msg.RecverID = m_nMailBoxID;

	while (m_bStopped)
	{
		msg.RecverID = m_nMailBoxID;

		bRet = RecvMail("SCADA", &msg, 10);

		//�ɹ��յ�һ���ʼ�
		if (bRet)
		{
			std::shared_ptr<CAlarmDataConfig> pMsg = std::make_shared<CAlarmDataConfig>();
			//�½��ձ��ľ�δȷ��״̬
			pMsg->m_nAlarmState = alarm_unconfim;
			//�������� 
			memcpy((char*)pMsg.get(),(char*)msg.Buff,sizeof CAlarmDataConfig);

			m_pAlarmQueue->push_back(pMsg);
		}
		else
		{
			QThread::sleep(1);
		}
	}
}
