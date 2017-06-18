#ifndef ANALYSEMSG_H
#define ANALYSEMSG_H


#include "alarmdataconfig.h"
#include <atomic>
#include <QThread>
#include <memory>
struct TIMEPAK;
//�����߳�
class CAnalyseTask : public QThread
{
	Q_OBJECT

public:
	CAnalyseTask(QObject *parent, std::shared_ptr< CAlarmInfoQueue>& alarmQueue, std::shared_ptr< CAlarmInfoQueue>& alarmViewQueue );
	~CAnalyseTask();

public:
	void run();
	void SetThreadState(bool state)
	{
		m_bStopped = state;
	}
private:
	QString GetEvtDescByCode(int32u);
	QString GetEventDesc(int32u nOccNo,int32u nAlarmOccNo,int32u nAlarmLimitOccNo);
	void FormatTimeStamp(TIMEPAK* pak,QString& szTimeStamp);
private:
	//�߳�����״̬���
	std::atomic <bool> m_bStopped;        
	//���ڽ��ܶ���
	std::shared_ptr<CAlarmInfoQueue> & m_pAlarmQueue;

	std::shared_ptr< CAlarmInfoQueue>& m_pAlarmViewQueue;
};

#endif // ANALYSEMSG_H
