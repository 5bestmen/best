/*! @file inverter.h
<PRE>
********************************************************************************
模块名       :
文件名       :  inverter.h
文件实现功能 :  逆变器定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   逆变器定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef INVERTER_DEF_H
#define INVERTER_DEF_H

#include "tag.h"

/*! \struct  CInverter
*   \brief 逆变器基本信息 */
class CInverter
{
public:
	CInverter(void);
	~CInverter(void);
public:
	bool GetRealTimeData();
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
private:
	//! 逆变器分合状态 
	CDinData m_IsOpen;
	//!  有功
	CAinData m_ActivePower;
	//! 无功
	CAinData m_ReactivePower;
};


#endif // INVERTER_DEF_H



/** @}*/

