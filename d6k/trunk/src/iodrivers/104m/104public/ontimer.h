
#ifndef _ON_TIMER__H
#define _ON_TIMER__H

#include "define_104.h"


class IHost;//host
class C104Impl;  //impl

class COnTimer
{
public:
	COnTimer(IHost* pHost, C104Impl* pImpl);
	virtual ~COnTimer();

public:
	bool OnTimer(int nTimerID);
private:
	bool OnTimerT1();
	bool OnTimerT2();
	bool OnTimerT3();

private:
	IHost* m_pHost;
	C104Impl*   m_pImpl;
};


#endif//_ON_TIMER__H
