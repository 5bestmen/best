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
** \details ��ȡʵʱ����
** \return bool 
** \author LiJin 
** \date 2017��5��31�� 
** \note 
********************************************************************************************************/
bool CPVStation::GetRealTimeData()
{
	bool bRet = false;

 
 

	return bRet;
}

/** @}*/

