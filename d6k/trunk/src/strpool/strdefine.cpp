

#include "strdefine.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"
#include "ace/streams.h"
#include "ace/OS_Memory.h"

int operator<< (ACE_OutputCDR &cdr, const STRING_ITEM &strItem)
{
	cdr << (ACE_CDR::ULong)strItem.m_nOccNo;

	cdr << strItem.m_string;

	return cdr.good_bit();
}

int operator>> (ACE_InputCDR &cdr, STRING_ITEM &strItem)
{
	ACE_CDR::ULong u_occno;
	CStrData s_string;

	if ((cdr >> u_occno) && (cdr >> s_string))
	{
		strItem.m_nOccNo = u_occno;
	
		strItem.m_string.SetMsg(s_string.GetMsg());
	}
	return cdr.good_bit();
}
