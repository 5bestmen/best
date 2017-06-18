/*! @file inverter.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  inverter.h
�ļ�ʵ�ֹ��� :  ���������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���������
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
** \details ��ȡʵʱ����
** \return bool 
** \author LiJin 
** \date 2017��3��24�� 
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

