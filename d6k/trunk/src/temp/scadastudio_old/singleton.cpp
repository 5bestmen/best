/*! @file
<PRE>
********************************************************************************
ģ����       :	������ݿ�ָ�����
�ļ���       :	singleton.cpp
�ļ�ʵ�ֹ��� :  ��������ݿ�ָ�����
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/

#include "singleton.h"

CSingleton* CSingleton:: m_pInstance = nullptr;

CDbapplitonapi *CSingleton::m_pDbi = nullptr;

/*! \fn CSingleton* CSingleton::GetInstance()
********************************************************************************************************* 
** \brief CSingleton::GetInstance 
** \details 
** \return CSingleton* 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
CSingleton* CSingleton::GetInstance()
{
	static QMutex mutex;

	mutex.lock();

	if(nullptr == m_pInstance)
	{
		m_pInstance = new CSingleton;

		m_pDbi = new CDbapplitonapi("globaldbi");

		m_pDbi->Open();
	}

	mutex.unlock();

	return m_pInstance;
}