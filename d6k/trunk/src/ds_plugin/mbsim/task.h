
#ifndef TASK_H
#define TASK_H

#include <memory>
#include <atomic>



// we use this functor class as entity of thread
class CTask
{
public:
	virtual ~CTask() = default;

	// the executer
	virtual bool operator()(void* arg = nullptr) = 0;

	virtual void Stop() 
	{
		m_bAtomicStopFlag.store(true, std::memory_order_release);
	}

	bool IsStopped() 
	{
		return m_bAtomicStopFlag.load(std::memory_order_acquire);
	}

protected:
	std::atomic<bool> m_bAtomicStopFlag{ false };
};

 

#endif  // TASK_H

