/*! @file pv_station.h
<PRE>
********************************************************************************
模块名       :
文件名       :  pv_station.h
文件实现功能 :  光伏电站定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   光伏电站定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.05.24
*******************************************************************************/

#include "pv_station.h" 
#include "log/log.h"
#include <QDebug>

CPVStation::CPVStation()
{

}

CPVStation::~CPVStation()
{
 
} 

void CPVStation::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
}
/*! \fn bool CPVStation::GetRealTimeData()
********************************************************************************************************* 
** \brief CPVStation::GetRealTimeData 
** \details 获取实时数据
** \return bool 
** \author LiJin 
** \date 2017年5月31日 
** \note 
********************************************************************************************************/
bool CPVStation::GetRealTimeData()
{
	bool bRet = false;

 
 

	return bRet;
}

/** @}*/

