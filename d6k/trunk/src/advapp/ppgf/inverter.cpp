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

#include "inverter.h" 
#include "log/log.h"
#include <QDebug>

CInverter::CInverter()
{

}

CInverter::~CInverter()
{
 
} 

void CInverter::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
}
/*! \fn bool CInverter::GetRealTimeData()
********************************************************************************************************* 
** \brief CInverter::GetRealTimeData 
** \details 获取实时数据
** \return bool 
** \author LiJin 
** \date 2017年3月24日 
** \note 
********************************************************************************************************/
bool CInverter::GetRealTimeData()
{
	bool bRet = false;

	bRet |= m_IsOpen.GetRealTimeData();
	bRet |= m_ActivePower.GetRealTimeData();
	bRet |= m_ReactivePower.GetRealTimeData();

	return bRet;
}

/** @}*/

