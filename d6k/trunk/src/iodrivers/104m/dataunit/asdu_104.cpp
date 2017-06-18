#include "asdu_104.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/CDR_Stream.h"
#include "ace/Auto_Ptr.h"
#include "ace/Truncate.h"

#include "ace/streams.h"
#include "ace/OS_Memory.h"

#include "ace/Date_Time.h" 
#include "ace/OS_NS_sys_time.h"

//////////////////////////////////////////////////////////////////////////
void ASDU1_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU1_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU1_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU1_SQ0)
		return;
	m_data[nIndex].siq.SPI = nValue;
}
int ASDU1_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU1_SQ0)
		return 0;
	return m_data[nIndex].siq.SPI;
}
SIQ ASDU1_SQ0::GetSIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU1_SQ0)
		return (*(SIQ*)(&a));

	return m_data[nIndex].siq;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU1_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU1_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
void ASDU1_SQ1::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU1_SQ1)
		return;
	m_data[nIndex].SPI = nValue;
}
int ASDU1_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU1_SQ1)
		return 0;

	return m_data[nIndex].SPI;
}
SIQ ASDU1_SQ1::GetSIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU1_SQ1)
		return (*(SIQ*)(&a));

	return m_data[nIndex];
}
int  ASDU1_SQ1::GetAsdu1_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(unsigned char);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU2_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU2_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU2_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU2_SQ0)
		return;
	m_data[nIndex].m_siq.SPI = nValue;
}
int ASDU2_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU2_SQ0)
		return 0;
	return m_data[nIndex].m_siq.SPI;
}
void ASDU2_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU2_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}
SIQ ASDU2_SQ0::GetSIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU2_SQ0)
		return (*(SIQ*)(&a));

	return m_data[nIndex].m_siq;
}
int  ASDU2_SQ0::GetAsdu2_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU2_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU3_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU3_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU3_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU3_SQ0)
		return;
	m_data[nIndex].diq.DPI = nValue;
}
int ASDU3_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU3_SQ0)
		return 0;

	return m_data[nIndex].diq.DPI;
}
DIQ ASDU3_SQ0::GetDIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU3_SQ0)
		return (*(DIQ*)(&a));

	return m_data[nIndex].diq;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU3_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU3_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
void ASDU3_SQ1::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU3_SQ1)
		return;
	m_data[nIndex].DPI = nValue;
}
int  ASDU3_SQ1::GetAsdu3_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(DIQ);
	return nSize;
}
int ASDU3_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU3_SQ1)
		return 0;

	return m_data[nIndex].DPI;
}
DIQ ASDU3_SQ1::GetDIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU3_SQ1)
		return (*(DIQ*)(&a));

	return m_data[nIndex];
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU4_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU4_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU4_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU4_SQ0)
		return;
	m_data[nIndex].m_diq.DPI = nValue;
}
int ASDU4_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU4_SQ0)
		return 0;
	return m_data[nIndex].m_diq.DPI;
}
void ASDU4_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU4_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}

DIQ ASDU4_SQ0::GetDIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU4_SQ0)
		return (*(DIQ*)(&a));

	return m_data[nIndex].m_diq;
}

int  ASDU4_SQ0::GetAsdu4_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU4_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU5_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU5_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU5_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU5_SQ0)
		return;
	m_data[nIndex].m_vti.value = nValue;
}
int ASDU5_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU5_SQ0)
		return 0;
	return m_data[nIndex].m_vti.value;
}
QDS ASDU5_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU5_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU5_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU5_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
void ASDU5_SQ1::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU5_SQ1)
		return;
	m_data[nIndex].m_vti.value = nValue;
}
int  ASDU5_SQ1::GetAsdu5_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU5_SQ1_DATA);
	return nSize;
}
int ASDU5_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU5_SQ1)
		return 0;

	return m_data[nIndex].m_vti.value;
}
QDS ASDU5_SQ1::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU5_SQ1)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU6_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU6_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU6_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU6_SQ0)
		return;
	m_data[nIndex].m_vti.value = nValue;
}
int ASDU6_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU6_SQ0)
		return 0;
	return m_data[nIndex].m_vti.value;
}
void ASDU6_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU6_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}

QDS ASDU6_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU6_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU8_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU8_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU8_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU8_SQ0)
		return;
	m_data[nIndex].m_bsi = nValue;
}
int ASDU8_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU8_SQ0)
		return 0;
	return m_data[nIndex].m_bsi;
}
void ASDU8_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU8_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}

QDS ASDU8_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU8_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ASDU9_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU9_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
short ASDU9_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ0)
		return 0;
	return m_data[nIndex].nValue;
}
void ASDU9_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ0)
		return;
	m_data[nIndex].nValue = nValue;
}
void ASDU9_SQ0::SetQDS(int nIndex, unsigned char nQds)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ0)
		return;
	m_data[nIndex].m_qds = *(QDS*)(&nQds);
}
QDS ASDU9_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
int  ASDU9_SQ0::GetAsdu9_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU9_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void ASDU9_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU9_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
short ASDU9_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ1)
		return 0;
	return m_data[nIndex].m_nValue;
}
void ASDU9_SQ1::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ1)
		return;
	m_data[nIndex].m_nValue = nValue;
}
void ASDU9_SQ1::SetQDS(int nIndex, unsigned char nQds)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ1)
		return;
	m_data[nIndex].m_qds = *(QDS*)(&nQds);
}
QDS ASDU9_SQ1::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU9_SQ1)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
int  ASDU9_SQ1::GetAsdu9_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU9_SQ1_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU10_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU10_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU10_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU10_SQ0)
		return;
	m_data[nIndex].m_nva = nValue;
}
short ASDU10_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU10_SQ0)
		return 0;
	return m_data[nIndex].m_nva;
}
void ASDU10_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU10_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}

QDS ASDU10_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU10_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}

int  ASDU10_SQ0::GetAsdu10_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU10_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void ASDU11_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU11_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
short ASDU11_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ0)
		return 0;
	return m_data[nIndex].m_nValue;
}
void ASDU11_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ0)
		return;
	m_data[nIndex].m_nValue = nValue;
}
void ASDU11_SQ0::SetQDS(int nIndex, unsigned char nQds)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ0)
		return;
	m_data[nIndex].m_qds = *(QDS*)(&nQds);
}
QDS ASDU11_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
int  ASDU11_SQ0::GetAsdu11_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU11_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void ASDU11_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU11_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
short ASDU11_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ1)
		return 0;
	return m_data[nIndex].m_nValue;
}
void ASDU11_SQ1::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ1)
		return;
	m_data[nIndex].m_nValue = nValue;
}
void ASDU11_SQ1::SetQDS(int nIndex, unsigned char nQds)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ1)
		return;
	m_data[nIndex].m_qds = *(QDS*)(&nQds);
}
QDS ASDU11_SQ1::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU11_SQ1)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
int  ASDU11_SQ1::GetAsdu11_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU11_SQ1_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU12_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU12_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU12_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU12_SQ0)
		return;
	m_data[nIndex].m_sva = nValue;
}
short ASDU12_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU12_SQ0)
		return 0;
	return m_data[nIndex].m_sva;
}
void ASDU12_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU12_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}

QDS ASDU12_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU12_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}

int  ASDU12_SQ0::GetAsdu12_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU12_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ASDU13_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU13_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
float ASDU13_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU13_SQ0)
		return 0;
	return m_data[nIndex].m_fValue;
}
void ASDU13_SQ0::SetValue(int nIndex, float fValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU13_SQ0)
		return;
	m_data[nIndex].m_fValue = fValue;
}
QDS ASDU13_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU13_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}

int  ASDU13_SQ0::GetAsdu13_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU13_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void ASDU13_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU13_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
float ASDU13_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU13_SQ1)
		return 0;
	return m_data[nIndex].m_fValue;
}
void ASDU13_SQ1::SetValue(int nIndex, float fValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU13_SQ1)
		return;
	m_data[nIndex].m_fValue = fValue;
}
QDS ASDU13_SQ1::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU13_SQ1)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}

int  ASDU13_SQ1::GetAsdu13_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU13_SQ1_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU14_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU14_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU14_SQ0::SetValue(int nIndex, float fValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU14_SQ0)
		return;
	m_data[nIndex].m_std = fValue;
}
float ASDU14_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU14_SQ0)
		return 0;
	return m_data[nIndex].m_std;
}
void ASDU14_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU14_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}
QDS ASDU14_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU14_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
int  ASDU14_SQ0::GetAsdu14_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU14_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU15_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU15_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU15_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU15_SQ0)
		return;
	m_data[nIndex].m_nValue = nValue;
}
int ASDU15_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU15_SQ0)
		return 0;
	return m_data[nIndex].m_nValue;
}
int  ASDU15_SQ0::GetAsdu15_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU15_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU15_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU15_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
int  ASDU15_SQ1::GetAsdu15_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU15_SQ1_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU16_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU16_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU16_SQ0::SetValue(int nIndex, float fValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU16_SQ0)
		return;
	m_data[nIndex].m_fValue = fValue;
}
float ASDU16_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU16_SQ0)
		return 0;
	return m_data[nIndex].m_fValue;
}
void ASDU16_SQ0::SetTime(int nIndex)
{
	CP24Time2a cp24time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp24time2a.Initialize(tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp24time2a;
}

CP24Time2a ASDU16_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}
int  ASDU16_SQ0::GetAsdu16_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU16_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU20_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU20_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU20_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU20_SQ0)
		return;
	m_data[nIndex].m_nSCD = nValue;
}
int ASDU20_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU20_SQ0)
		return 0;
	return m_data[nIndex].m_nSCD;
}
QDS ASDU20_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU20_SQ0)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU20_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU20_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
void ASDU20_SQ1::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU20_SQ1)
		return;
	m_data[nIndex].m_nSCD = nValue;
}
int ASDU20_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU20_SQ1)
		return 0;
	return m_data[nIndex].m_nSCD;
}
QDS ASDU20_SQ1::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU20_SQ1)
		return (*(QDS*)(&a));
	return m_data[nIndex].m_qds;
}
int  ASDU20_SQ1::GetAsdu20_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU20_SQ1_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU21_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU21_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU21_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU21_SQ0)
		return;
	m_data[nIndex].m_nva = nValue;
}
short ASDU21_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU21_SQ0)
		return 0;
	return m_data[nIndex].m_nva;
}
int  ASDU21_SQ0::GetAsdu21_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU21_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU21_SQ1::SetItemCount(int nCount)
{
	vsq = nCount | 0x80;
}
int ASDU21_SQ1::GetItemCount() const
{
	return  vsq & 0x7f; //应用数据单元数目
}
void ASDU21_SQ1::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU21_SQ1)
		return;
	m_data[nIndex].m_nva = nValue;
}
short ASDU21_SQ1::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU21_SQ1)
		return 0;
	return m_data[nIndex].m_nva;
}
int  ASDU21_SQ1::GetAsdu21_SQ1Length()
{
	int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR) + GetItemCount() * sizeof(ASDU21_SQ1_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ASDU30_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU30_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU30_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU30_SQ0)
		return;
	m_data[nIndex].m_siq.SPI = nValue;
}
int ASDU30_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU30_SQ0)
		return 0;

	return m_data[nIndex].m_siq.SPI;
}
void ASDU30_SQ0::SetAddress(int nIndex, int nAddr)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU30_SQ0)
		return;

	m_data[nIndex].m_infoaddr.SetAddr(nAddr);
}

int ASDU30_SQ0::GetAddress(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU30_SQ0)
		return 0;

	return m_data[nIndex].m_infoaddr.GetAddr();
}
void ASDU30_SQ0::SetTime(int nIndex)
{
	CP56Time2a cp56time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp56time2a.Initialize(tvTime.year(), tvTime.month(), tvTime.day(), tvTime.hour(), tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp56time2a;
}

CP56Time2a ASDU30_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}
SIQ ASDU30_SQ0::GetSIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU30_SQ0)
		return (*(SIQ*)(&a));

	return m_data[nIndex].m_siq;
}
int  ASDU30_SQ0::GetAsdu30_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU30_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////


void ASDU31_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU31_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU31_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU31_SQ0)
		return;
	m_data[nIndex].m_diq.DPI = nValue;
}
int ASDU31_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU31_SQ0)
		return 0;

	return m_data[nIndex].m_diq.DPI;
}
void ASDU31_SQ0::SetAddress(int nIndex, int nAddr)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU31_SQ0)
		return;

	m_data[nIndex].m_infoaddr.SetAddr(nAddr);
}

int ASDU31_SQ0::GetAddress(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU31_SQ0)
		return 0;

	return m_data[nIndex].m_infoaddr.GetAddr();
}
void ASDU31_SQ0::SetTime(int nIndex)
{
	CP56Time2a cp56time2a;
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());
	cp56time2a.Initialize(tvTime.year(), tvTime.month(), tvTime.day(), tvTime.hour(), tvTime.minute(), tvTime.second(), tvTime.microsec());
	m_data[nIndex].m_time = cp56time2a;
}

CP56Time2a ASDU31_SQ0::GetTime(int nIndex)
{
	return m_data[nIndex].m_time;
}
DIQ ASDU31_SQ0::GetDIQ(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU31_SQ0)
		return (*(DIQ*)(&a));

	return m_data[nIndex].m_diq;
}
int  ASDU31_SQ0::GetAsdu31_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU31_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU32_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU32_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU32_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU32_SQ0)
		return;
	m_data[nIndex].m_vti.value = nValue;
}
short ASDU32_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU32_SQ0)
		return 0;
	return m_data[nIndex].m_vti.value;
}
QDS ASDU32_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU32_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU34_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU34_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU34_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU34_SQ0)
		return;
	m_data[nIndex].m_nva = nValue;
}
short ASDU34_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU34_SQ0)
		return 0;
	return m_data[nIndex].m_nva;
}
QDS ASDU34_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU34_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
int  ASDU34_SQ0::GetAsdu34_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU34_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU35_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU35_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU35_SQ0::SetValue(int nIndex, short nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU35_SQ0)
		return;
	m_data[nIndex].m_sva = nValue;
}
short ASDU35_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU35_SQ0)
		return 0;
	return m_data[nIndex].m_sva;
}
QDS ASDU35_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU35_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
int  ASDU35_SQ0::GetAsdu35_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU35_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU36_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU36_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU36_SQ0::SetValue(int nIndex, float fValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU36_SQ0)
		return;
	m_data[nIndex].m_fValue = fValue;
}
float ASDU36_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU36_SQ0)
		return 0;
	return m_data[nIndex].m_fValue;
}
QDS ASDU36_SQ0::GetQDS(int nIndex)
{
	int a = 0;
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU36_SQ0)
		return (*(QDS*)(&a));

	return m_data[nIndex].m_qds;
}
int  ASDU36_SQ0::GetAsdu36_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU36_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void ASDU37_SQ0::SetItemCount(int nCount)
{
	vsq = nCount;
}
int ASDU37_SQ0::GetItemCount() const
{
	return  vsq; //应用数据单元数目
}
void ASDU37_SQ0::SetValue(int nIndex, int nValue)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU37_SQ0)
		return;
	m_data[nIndex].m_bcr.number = nValue;
}
int ASDU37_SQ0::GetValue(int nIndex)
{
	if (nIndex < 0 || nIndex >= MAX_DATA_PER_ASDU37_SQ0)
		return 0;
	return m_data[nIndex].m_bcr.number;
}
int  ASDU37_SQ0::GetAsdu37_SQ0Length()
{
	int nSize = sizeof(ASDU_BASE) + GetItemCount() * sizeof(ASDU37_SQ0_DATA);
	return nSize;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



short ASDU48::GetValue()
{
	return m_nValue;
}
void ASDU48::SetValue(short nValue)
{
	m_nValue = nValue;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
short ASDU49::GetValue()
{
	return m_nValue;;
}
void ASDU49::SetValue(short nValue)
{
	m_nValue = nValue;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
float ASDU50::GetValue()
{
	return m_fValue;
}
void ASDU50::SetValue(float fValue)
{
	m_fValue = fValue;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////