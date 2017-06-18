

#include "logdefine.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"
#include "ace/streams.h"
#include "ace/OS_Memory.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int operator<< (ACE_OutputCDR &cdr, const CMsgLog &msgLog)
{  
	cdr << (ACE_CDR::ULong)msgLog.m_nLogLevel;
	
	cdr << msgLog.m_strLogText;
	cdr << msgLog.m_strLogModule;

	return cdr.good_bit ();
}

int operator>> (ACE_InputCDR &cdr, CMsgLog &msgLog)
{
	ACE_CDR::ULong u_loglevel;
	
	CStrData s_strText;
	CStrData s_strModule;

	if ( (cdr >> u_loglevel) && (cdr >> s_strText) && (cdr >> s_strModule ) ) 
	{
		msgLog.m_nLogLevel = u_loglevel;
		msgLog.m_strLogText.SetMsg(s_strText.GetMsg());
		msgLog.m_strLogModule.SetMsg(s_strModule.GetMsg());
	}
	return cdr.good_bit ();
}
