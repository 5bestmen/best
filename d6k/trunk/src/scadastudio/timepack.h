
#ifndef TIME_PACK_H
#define TIME_PACK_H 

#include <qdatetime.h>
#include "public.h"



class CTimePack : public TIMEPAK
{
public:
	CTimePack();
	~CTimePack();

public:
	static void GetCurrentDateTime(TIMEPAK & tm)
	{
		tm.Year = QDate::currentDate().year();
		tm.Month = QDate::currentDate().month();
		tm.Day = QDate::currentDate().day();
		tm.Hour = QTime::currentTime().hour();
		tm.Minute = QTime::currentTime().minute();
		tm.Second = QTime::currentTime().second();
		tm.Milliseconds = QTime::currentTime().msec();
	}

private:

};

CTimePack::CTimePack()
{
}

CTimePack::~CTimePack()
{
}

#endif // _PUBLIC_H


