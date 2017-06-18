#include "msg_queue.h"

CTransQueue::CTransQueue()
{
}

CTransQueue::~CTransQueue()
{
}

void CTransQueue::PushPoint(const SPoint* point)
{
	QMutexLocker locker(&m_mutex);
	if (nullptr != point)
	{
		m_queueTransInfo.append(*point);
	}
}

const SPoint* CTransQueue::PopPoint()
{
	QMutexLocker locker(&m_mutex);

	SPoint& point = m_queueTransInfo.front();
	m_queueTransInfo.pop_back();

	return &point;
}