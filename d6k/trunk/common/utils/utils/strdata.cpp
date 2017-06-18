
#include "strdata.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"

#include "ace/streams.h"
#include "ace/OS_Memory.h"


CStrData::CStrData ()
{
	m_pStrMsg = NULL;

	SetMsg("");
}

CStrData::~CStrData (void)
{
	if (m_pStrMsg)
	{
		delete [] m_pStrMsg;
		m_pStrMsg = NULL;
	}
}

const char * CStrData::GetMsg(void) const
{
	return m_pStrMsg;
}

int CStrData::SetMsg(const char *strMsg)
{
	if (m_pStrMsg)
	{
		delete [] m_pStrMsg;
		m_pStrMsg = NULL;
	}

	if(strMsg == NULL)
		return 0;

	size_t const nlen = ACE_OS::strlen (strMsg)+1;  // Will need room for '\0'

	ACE_NEW_RETURN (m_pStrMsg, char[nlen], -1);

	ACE_OS::strcpy (m_pStrMsg, strMsg); //strcpy将在m_pStrMsg后加'\0';

	return 0;
}

size_t CStrData::GetMsgLen(void) const 
{
	if(m_pStrMsg==NULL)
		return 0;

	return ACE_OS::strlen (this->m_pStrMsg);
}

int operator<< (ACE_OutputCDR &cdr, const CStrData &strdata)
{  
	ACE_CDR::ULong u_msglen =  strdata.GetMsgLen ();
	cdr << u_msglen;
	
	cdr.write_char_array (strdata.GetMsg(), u_msglen);
	char* pbuffer = (char*)(cdr.buffer());

	return cdr.good_bit ();
}

int operator>> (ACE_InputCDR &cdr, CStrData &strdata)
{
	ACE_CDR::ULong u_msglen;

	if ( cdr >> u_msglen ) 
	{
		char* buffer;
		ACE_NEW_RETURN (buffer, char[u_msglen+1], -1);

		auto_ptr<char> log_msg_p (buffer);

		cdr.read_char_array (buffer, u_msglen);

		buffer[u_msglen] = '\0';
		if (-1 == strdata.SetMsg(buffer))
			return -1;
	}
	return cdr.good_bit ();
}
