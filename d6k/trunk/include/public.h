
#ifndef _PUBLIC_H
#define _PUBLIC_H

#include "datatypes.h"

#pragma pack (1)
struct TIMEPAK
{
	int16u Year;
	int16u Month;

	int16u DayOfWeek;
	int16u Day;

	int16u Hour;
	int16u Minute;

	int16u Second;
	int16u Milliseconds;
};
#pragma pack ()


 
#endif // _PUBLIC_H


