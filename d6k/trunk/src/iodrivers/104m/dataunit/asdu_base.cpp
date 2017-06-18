#include "asdu_base.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"

#include "ace/streams.h"
#include "ace/OS_Memory.h"

void COT1::SetCot(unsigned char cot)
{
	m_cot = cot;
}
unsigned char COT1::GetCot()
{
	return m_cot;
}

int operator<< (ACE_OutputCDR &cdr, const COT1 &cot1)
{
	cdr << cot1.m_cot;
	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, COT1 &cot1)
{
	ACE_CDR::Char  cot0;
	if (cdr >> cot0)
	{
		cot1.m_cot = cot0;
	}

	return cdr.good_bit();
}

void COT2::SetCot(unsigned short cot)
{
	m_cot0 = MYLO8(cot);
	m_cot1 = MYHI8(cot);
}
unsigned short COT2::GetCot()
{
	return MYMAKE16(m_cot0, m_cot1);
}

int operator<< (ACE_OutputCDR &cdr, const COT2 &cot2)
{
	cdr << cot2.m_cot0;
	cdr << cot2.m_cot1;

	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, COT2 &cot2)
{
	ACE_CDR::Char  cot0;
	ACE_CDR::Char  cot1;

	if ((cdr >> cot0) && (cdr >> cot1))
	{
		cot2.m_cot0 = cot0;
		cot2.m_cot1 = cot1;
	}

	return cdr.good_bit();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDUADDR1::SetAddr(unsigned char addr)
{
	m_asduaddr = addr;
}
unsigned char ASDUADDR1::GetAddr()
{
	return m_asduaddr;
}
int operator<< (ACE_OutputCDR &cdr, const ASDUADDR1 &asduaddr1)
{
	cdr << asduaddr1.m_asduaddr;

	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, ASDUADDR1 &asduaddr1)
{
	ACE_CDR::Char  asduaddr;

	if (cdr >> asduaddr)
	{
		asduaddr1.m_asduaddr = asduaddr;
	}

	return cdr.good_bit();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDUADDR2::SetAddr(unsigned short addr)
{
	m_asduaddr0 = MYLO8(addr);
	m_asduaddr1 = MYHI8(addr);
}
unsigned short ASDUADDR2::GetAddr()
{
	return MYMAKE16(m_asduaddr0, m_asduaddr1);
}
int operator<< (ACE_OutputCDR &cdr, const ASDUADDR2 &asduaddr2)
{
	cdr << asduaddr2.m_asduaddr0;
	cdr << asduaddr2.m_asduaddr1;

	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, ASDUADDR2 &asduaddr2)
{
	ACE_CDR::Char  asduaddr0;
	ACE_CDR::Char  asduaddr1;

	if ((cdr >> asduaddr0) && (cdr >> asduaddr1))
	{
		asduaddr2.m_asduaddr0 = asduaddr0;
		asduaddr2.m_asduaddr1 = asduaddr1;
	}

	return cdr.good_bit();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void INFOADDR2::SetAddr(unsigned short addr)
{
	m_infoaddr0 = MYLO8(addr);
	m_infoaddr1 = MYHI8(addr);
}
unsigned short INFOADDR2::GetAddr()
{
	return MYMAKE16(m_infoaddr0, m_infoaddr1);
}
int operator<< (ACE_OutputCDR &cdr, const INFOADDR2 &infoaddr2)
{
	cdr << infoaddr2.m_infoaddr0;
	cdr << infoaddr2.m_infoaddr1;

	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, INFOADDR2 &infoaddr2)
{
	ACE_CDR::Char  infoaddr0;
	ACE_CDR::Char  infoaddr1;

	if ((cdr >> infoaddr0) && (cdr >> infoaddr1))
	{
		infoaddr2.m_infoaddr0 = infoaddr0;
		infoaddr2.m_infoaddr1 = infoaddr1;
	}

	return cdr.good_bit();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void INFOADDR3::SetAddr(int addr)
{
	unsigned short  lo16 = MYLO16(addr);
	unsigned short  hi16 = MYHI16(addr);

	m_infoaddr0 = MYLO8(lo16);
	m_infoaddr1 = MYHI8(lo16);
	m_infoaddr2 = MYLO8(hi16);
}
int INFOADDR3::GetAddr()
{
	unsigned short  lo16 = MYMAKE16(m_infoaddr0, m_infoaddr1);
	unsigned short  hi16 = MYMAKE16(m_infoaddr2, 0);

	return MYMAKE32(lo16, hi16);
}
int operator<< (ACE_OutputCDR &cdr, const INFOADDR3 &infoaddr3)
{
	cdr << infoaddr3.m_infoaddr0;
	cdr << infoaddr3.m_infoaddr1;
	cdr << infoaddr3.m_infoaddr2;

	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, INFOADDR3 &infoaddr3)
{
	ACE_CDR::Char  infoaddr0;
	ACE_CDR::Char  infoaddr1;
	ACE_CDR::Char  infoaddr2;

	if ((cdr >> infoaddr0) && (cdr >> infoaddr1) && (cdr >> infoaddr2))
	{
		infoaddr3.m_infoaddr0 = infoaddr0;
		infoaddr3.m_infoaddr1 = infoaddr1;
		infoaddr3.m_infoaddr2 = infoaddr2;
	}

	return cdr.good_bit();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
int operator<< (ACE_OutputCDR &cdr, const CP56Time2a &time)
{
		cdr << time.msL;
		cdr << time.msH;
		cdr << time.minute;
		cdr << time.hour;
		cdr << time.day;
		cdr << time.month;
		cdr << time.year;

		return cdr.good_bit ();
}
int operator>> (ACE_InputCDR &cdr, CP56Time2a &time)
{
		ACE_CDR::Char  msL;
		ACE_CDR::Char  msH;
		ACE_CDR::Char  minute;
		ACE_CDR::Char  hour;
		ACE_CDR::Char  day;
		ACE_CDR::Char  month;
		ACE_CDR::Char  year;

		if ((cdr >> msL) && (cdr >> msH) && (cdr >> minute) && (cdr >> hour) && (cdr >> day) && (cdr >> month) && (cdr >> year) )
		{
				time.msL = msL;
				time.msH = msH;
				time.minute = minute;
				time.hour = hour;
				time.day = day;
				time.month = month;
				time.year = year;
		}

		return cdr.good_bit ();
}
*/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int operator<< (ACE_OutputCDR &cdr, const ASDU_BASE &asduBase)
{
	cdr << asduBase.type;
	cdr << asduBase.vsq;
	cdr << asduBase.cot;
	cdr << asduBase.asduAddr;
	return cdr.good_bit();
}
int operator >> (ACE_InputCDR &cdr, ASDU_BASE &asduBase)
{
	ACE_CDR::Char  type;
	ACE_CDR::Char  vsq;
	COT  cot;
	ASDUADDR  asduAddr;

	if ((cdr >> type) && (cdr >> vsq) && (cdr >> cot) && (cdr >> asduAddr))
	{
		asduBase.type = type;
		asduBase.vsq = vsq;
		asduBase.cot = cot;
		asduBase.asduAddr = asduAddr;
	}

	return cdr.good_bit();
}
