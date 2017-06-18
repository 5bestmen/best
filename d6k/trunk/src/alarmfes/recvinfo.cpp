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
** \details 接收线程
** \return void 
** \author xingzhibing
** \date 2016年10月30日 
** \note 
********************************************************************************************************/
void CRecvInfoTask::run()
{
	bool bRet = false;

	if (m_nMailBoxID == 0)
	{
		m_nMailBoxID = QueryMailBoxID("FES", "ALARM_WIN");
		Q_ASSERT(m_nMailBoxID > 0);
	} 

//	SubscribeMail("FES", "ALARM_WIN", 1);

	DMSG msg;
	msg.RecverID = m_nMailBoxID;

	while (m_bStopped)
	{
		msg.RecverID = m_nMailBoxID;

		bRet = RecvMail("FES", &msg, 10);

		//成功收到一封邮件
		if (bRet)
		{
			std::shared_ptr<CAlarmDataConfig> pMsg = std::make_shared<CAlarmDataConfig>();
			//新接收报文均未确认状态
			pMsg->m_nAlarmState = alarm_unconfim;
			//报文主体 
			memcpy(&pMsg->m_Msg,&msg,sizeof DMSG);
		//	pMsg->m_Msg = *((DMSG*)m_buff);
			//读取时间
			//pMsg->m_szTimeTag = QDateTime::currentDateTime();

			m_pAlarmQueue->push_back(pMsg);
		}
		else
		{
			QThread::sleep(1);
		//	boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
		}
	}
}
