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
#ifndef PV_STATION_DEF_H
#define PV_STATION_DEF_H

#include "tag.h"

/*! \struct  CPVStation
*   \brief   一个光伏电站 */
class CPVStation
{
public:
	CPVStation(void);
	~CPVStation(void);
public:
	bool GetRealTimeData();
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
private:
	 
};


#endif // INVERTER_DEF_H



/** @}*/

