/*! @file
<PRE>
********************************************************************************
ģ����       : ��㶨��
�ļ���       :
�ļ�ʵ�ֹ��� :  
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author xingzhibing
*  @version 1.0
*  @date    2016.10.24
*/

#ifndef FES_TAG_H
#define FES_TAG_H
#pragma once

#include "datatypes.h"
#include "fesapi/fesdatadef.h"
 
#include <memory>
#include <QDebug>
#include <QString>

template<typename T>
class CIoItem
{
public:
	CIoItem()
	{
		m_nOccNo = INVALID_OCCNO;
		m_pTag = nullptr;
		m_nAddress = 0;
	}
	explicit CIoItem(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual ~CIoItem()
	{

	}
public:
	int32u GetOccNo() const
	{
		return m_nOccNo;
	}	
	int32u LoadAddress()
	{
		Q_ASSERT(m_pTag);
		if (m_pTag)
		{
			Q_ASSERT(strlen(m_pTag->Address) > 0);
			if (strlen(m_pTag->Address))
			{
				QString szAddr(m_pTag->Address);
				m_nAddress = szAddr.toInt();
			}
		}
		return m_nAddress;
	}

	int32u GetAddress()const
	{
		return m_nAddress;
	}

	virtual void LoadData() = 0;
protected:
	int32u m_nOccNo;  //! ���к�
	int32u m_nAddress;//! ��ַ
	T * m_pTag;       //! �ڴ����ݿ�ָ�� 
};


class CIoAin :public CIoItem<AIN>
{
public:
	CIoAin()
	{
		m_nOccNo = INVALID_OCCNO;
	}
	explicit CIoAin(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual~CIoAin()
	{

	}
public:
	virtual void LoadData();
};

class CIoDin :public CIoItem<DIN>
{
public:
	CIoDin()
	{
		m_nOccNo = INVALID_OCCNO;
	}
	explicit CIoDin(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual~CIoDin()
	{

	}
public:
	virtual void LoadData();
};

#if 0
class CIoTag
{
public:
	CIoTag()
	{
		m_nOccNo = INVALID_OCCNO;
	}
	virtual ~CIoTag()
	{

	}
public:
	int32u GetOccNo() const
	{
		return m_nOccNo;
	}
protected:
	int32u m_nOccNo;
};
 
 
class CIoAin:public CIoTag
{
public:
	CIoAin()
	{
		m_nOccNo = INVALID_OCCNO;
	}
	explicit CIoAin(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual~CIoAin()
	{

	}
};

class CIoDin :public CIoTag
{
public:
	CIoDin()
	{
		m_nOccNo = INVALID_OCCNO;
	}
	explicit CIoDin(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual~CIoDin()
	{

	}
public:


};

class CIoAout :public CIoTag
{
public:
	CIoAout()
	{

	}
	explicit CIoAout(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual~CIoAout()
	{

	}
public:
};

class CIoDout :public CIoTag
{
public:
	CIoDout()
	{

	}
	explicit CIoDout(int32u nOccNo)
	{
		m_nOccNo = nOccNo;
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	}
	virtual ~CIoDout()
	{

	}
public:
};

#endif

/** @}*/

#endif // FES_TAG_H
