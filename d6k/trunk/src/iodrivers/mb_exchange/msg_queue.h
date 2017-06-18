
#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H


#include <QQueue>
#include <QMutex>
#include <QMutexLocker>

#include "data_struct.h"

class CTransQueue
{
public:
	CTransQueue();
	~CTransQueue();

public:
	void PushPoint(const SPoint* point);

	const SPoint* PopPoint();

private:
	QMutex m_mutex;
	QQueue<SPoint> m_queueTransInfo;
};



#endif // MSG_QUEUE_H