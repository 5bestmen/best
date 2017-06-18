/*! @file
<PRE>
********************************************************************************
模块名       :	获得数据库指针对象
文件名       :	singleton.cpp
文件实现功能 :  表格获得数据库指针对象
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
** \date 2015年11月17日 
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