
#ifndef _LOG_DEFINE_H
#define _LOG_DEFINE_H

#include "public.h"

enum
{
	MAX_LOG_MODULE_NAME_LEN = 128,   //!< 
	MAX_LOG_TEXT_LEN = 512,           //!< LOG长度限制
};


struct MSG_LOG
{
	unsigned int Level;
	TIMEPAK Time;
	
	unsigned long long ProcessID;
	unsigned long long ThreadID;

	char ModuleName[MAX_LOG_MODULE_NAME_LEN];
	char LogText[MAX_LOG_TEXT_LEN];

};


#endif // _LOG_DEFINE_H

