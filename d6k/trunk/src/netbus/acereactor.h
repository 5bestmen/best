
#ifndef __ACEREACTOR_H_
#define __ACEREACTOR_H_

#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Task.h"

class CAceReactor : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CAceReactor();
	virtual ~CAceReactor();

public:
	virtual int svc(void);

public:
	bool StartReactor();
	bool StopReactor();

public:
	ACE_Reactor* GetReactor() { return m_pReactor; }
private:
	ACE_Reactor* m_pReactor;

public:
	bool IsRunning() { return m_bIsLooping; }
private:
	bool m_bIsLooping;
};
#endif /* __ACEREACTOR_H_ */
