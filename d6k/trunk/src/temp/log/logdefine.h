
#ifndef _LOG_DEFINE_H
#define _LOG_DEFINE_H

#include "utils/strdata.h"

//每条log信息最大1024字节
class LOGDATA
{
	char buf[1024];
};


class CMsgLog
{
public:
	CMsgLog()
	{

	}
	~CMsgLog()
	{

	}
public:
	int      m_nLogLevel;
	CStrData m_strLogText;
	CStrData m_strLogModule;

	size_t   m_nThreadID;
	size_t   m_nProcessID;

	int GetMsgSize() 
	{
		int nSize1 = sizeof(CMsgLog);
		int nSize2 = m_strLogText.GetMsgLen();
		int nSize3 = m_strLogModule.GetMsgLen();

		int nSize4 = sizeof(m_nThreadID);
		int nSize5 = sizeof(m_nProcessID);
		
		return nSize1 + nSize2 + nSize3 + nSize4 +nSize5;
	}
};
int operator>> (ACE_InputCDR &cdr, CMsgLog &msgLog);
int operator<< (ACE_OutputCDR &cdr, const CMsgLog &msgLog);


#endif // _LOG_DEFINE_H

