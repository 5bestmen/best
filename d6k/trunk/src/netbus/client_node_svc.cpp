/*! @file client_node_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  client_node_svc.cpp
文件实现功能 :  工作站节点的网络服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   工作站节点的网络服务
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#include "client_node_svc.h"
#include "log/log.h" 
#include "nb_svc.h"
#include <QObject> 
#include <QString> 

 
CClientNodeSvc::CClientNodeSvc(NET_CONFIG* &pNetConfig, int nCrtNodeIdx) : m_pNetCfg(pNetConfig), CNodeSvc(pNetConfig, nCrtNodeIdx)
{
	Q_ASSERT(pNetConfig);
	m_nAnotherNodeIndex = -1;

}

CClientNodeSvc::~CClientNodeSvc()
{

}

/*! \fn bool CFesNodeSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CFesNodeSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CClientNodeSvc::Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode)
{
	 

	return true;
}

void CClientNodeSvc::Run()
{
 
}

void CClientNodeSvc::Shutdown()
{ 
	 
}
 


/** @}*/
