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
#ifndef INVERTER_DEF_H
#define INVERTER_DEF_H

#include "tag.h"

/*! \struct  CInverter
*   \brief �����������Ϣ */
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
	//! ������ֺ�״̬ 
	CDinData m_IsOpen;
	//!  �й�
	CAinData m_ActivePower;
	//! �޹�
	CAinData m_ReactivePower;
};


#endif // INVERTER_DEF_H



/** @}*/

