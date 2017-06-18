
#ifndef _STRING_DEFINE_H
#define _STRING_DEFINE_H

#include "Utils/strdata.h"

#include "datatypes.h"

enum
{
	SHAREMEM_BASE_ID_STRPOOL = 10000,
};

struct STRINGDATA
{
	char buf[1024];
};


struct STRING_ITEM
{
	int	 m_nOccNo;
	CStrData m_string;
};
int operator>> (ACE_InputCDR &cdr, STRING_ITEM &strItem);
int operator<< (ACE_OutputCDR &cdr, const STRING_ITEM &strItem);


#endif // _STRING_DEFINE_H


