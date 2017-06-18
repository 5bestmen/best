/*! @file tag.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  tag.h
�ļ�ʵ�ֹ��� :  ��㶨��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��㶨��
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef TAG_DEF_H
#define TAG_DEF_H
   

#include "datatypes.h"
//#include "fesapi/fesdatadef.h"
#include "log/log.h"
#include <string>
#include <memory>

template<typename T>
class CVar
{
public:
	CVar()
	{
	}

	virtual ~CVar()
	{

	}
public:
	T GetValue() const
	{
		return m_Value;
	}
	// �����OccNo����ʼ��ʱ����
	virtual bool GetOccNoByTagName() = 0;
	// ʵʱ��ȡʵ��ֵ����ʱѭ������
	virtual bool GetRealTimeData() = 0;

	void LogMsg(const char *szLogTxt, int nLevel)
	{		
		::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
	}

protected:
	T   m_Value;       //!  ��ֵ

	//struct TAGITEM  m_Item;
	std::shared_ptr<struct TAGITEM > m_pOccNoItem;

	std::string m_szTagName;

	std::string m_szDescription;
};

class CDinData:public CVar<int8u>
{
public:
	CDinData();
	~CDinData();
public:
	virtual bool GetRealTimeData();
	virtual bool GetOccNoByTagName();
private:
	
};

class CAinData :public CVar<fp64>
{
public:
	CAinData();
	~CAinData();
public:
	virtual bool GetRealTimeData();
	virtual bool GetOccNoByTagName();
private:

};

#endif // TAG_DEF_H

/** @}*/

