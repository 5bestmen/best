/*! @file sub_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  sub_svc.cpp
文件实现功能 :	订阅服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   订阅服务
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.03
*******************************************************************************/
#include "sub_svc.h"
#include "log/log.h"
 

#include <QObject> 
#include <QString> 




CSubScriptionSvc::CSubScriptionSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	 
}

CSubScriptionSvc::~CSubScriptionSvc(void)
{
 
}
/*! \fn bool CSubScriptionSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbgSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CSubScriptionSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{




	return true;
}

void CSubScriptionSvc::Run()
{

}

void CSubScriptionSvc::Shutdown()
{

}



/** @}*/
