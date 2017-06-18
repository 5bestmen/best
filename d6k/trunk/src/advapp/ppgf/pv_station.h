/*! @file pv_station.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  pv_station.h
�ļ�ʵ�ֹ��� :  �����վ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����վ����
*  @author  LiJin
*  @version 1.0
*  @date    2017.05.24
*******************************************************************************/
#ifndef PV_STATION_DEF_H
#define PV_STATION_DEF_H

#include "tag.h"

/*! \struct  CPVStation
*   \brief   һ�������վ */
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

