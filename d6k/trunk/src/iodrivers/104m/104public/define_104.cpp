#include "define_104.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"

#include "ace/streams.h"
#include "ace/OS_Memory.h"


int operator<< (ACE_OutputCDR &cdr, const APCI &apci)
{
	cdr << apci.head;
	cdr << apci.len;
	cdr << apci.control0;
	cdr << apci.control1;
	cdr << apci.control2;
	cdr << apci.control3;

	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, APCI &apci)
{
	ACE_CDR::Char  head;
	ACE_CDR::Char  len;
	ACE_CDR::Char  control0;
	ACE_CDR::Char  control1;
	ACE_CDR::Char  control2;
	ACE_CDR::Char  control3;

	if ((cdr >> head) && (cdr >> len) && (cdr >> control0) && (cdr >> control1) && (cdr >> control2) && (cdr >> control3))
	{
		apci.head = head;
		apci.len = len;
		apci.control0 = control0;
		apci.control1 = control1;
		apci.control2 = control2;
		apci.control3 = control3;
	}

	return cdr.good_bit();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
