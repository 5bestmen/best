#ifndef CRECVINFO_H
#define CRECVINFO_H


#include "alarmdataconfig.h"
#include <QThread>
#include <memory>
#include <atomic>

//此辅助线程专用于接受告警信息

class CRecvInfoTask : public QThread
{
	Q_OBJECT

public:
	CRecvInfoTask(QObject *parent, std::shared_ptr< CAlarmInfoQueue>& alarmQueue);
	~CRecvInfoTask();
public:
	void SetThreadState( bool state)
	{
		m_bStopped = state;
	}
public:
	void run();
private:
	//接收缓冲区
	unsigned char m_buff[MAIL_MAX_SIZE*2];
	//用于线程运行停止
	 std::atomic <bool>  m_bStopped;
	//用于接受队列
	std::shared_ptr<CAlarmInfoQueue> &m_pAlarmQueue;

	int m_nMailBoxID;
};

#endif // CRECVINFO_H
