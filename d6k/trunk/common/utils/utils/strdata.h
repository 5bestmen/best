

#ifndef _STR_DATA_H
#define _STR_DATA_H

#include "ace/OS_NS_string.h"

class CStrData
{
public:
	CStrData (void); 
	CStrData::~CStrData (void);

public:
	const char * GetMsg(void) const;
	int SetMsg(const char *strMsg);
	size_t GetMsgLen(void) const;
	
private:
	char*       m_pStrMsg;

private:
	/// disallow copying...
	CStrData (const CStrData& rhs);
	CStrData& operator= (const CStrData& rhs);
};

// Forward decls.
class ACE_InputCDR;
class ACE_OutputCDR;

int operator>> (ACE_InputCDR &cdr, CStrData &strdata);
int operator<< (ACE_OutputCDR &cdr, const CStrData &strdata);

#endif //_STR_DATA_H

