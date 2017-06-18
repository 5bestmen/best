/*! @file tag.h
<PRE>
********************************************************************************
模块名       :
文件名       :  tag.h
文件实现功能 :  测点定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   测点定义
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
	// 换算成OccNo，初始化时调用
	virtual bool GetOccNoByTagName() = 0;
	// 实时获取实测值，定时循环调用
	virtual bool GetRealTimeData() = 0;

	void LogMsg(const char *szLogTxt, int nLevel)
	{		
		::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
	}

protected:
	T   m_Value;       //!  测值

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

