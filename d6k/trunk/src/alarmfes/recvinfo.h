#ifndef CRECVINFO_H
#define CRECVINFO_H


#include "alarmdataconfig.h"
#include <QThread>
#include <memory>
#include <atomic>

//�˸����߳�ר���ڽ��ܸ澯��Ϣ

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
	//���ջ�����
	unsigned char m_buff[MAIL_MAX_SIZE*2];
	//�����߳�����ֹͣ
	 std::atomic <bool>  m_bStopped;
	//���ڽ��ܶ���
	std::shared_ptr<CAlarmInfoQueue> &m_pAlarmQueue;

	int m_nMailBoxID;
};

#endif // CRECVINFO_H
