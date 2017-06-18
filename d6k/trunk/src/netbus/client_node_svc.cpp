/*! @file client_node_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  client_node_svc.cpp
�ļ�ʵ�ֹ��� :  ����վ�ڵ���������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����վ�ڵ���������
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
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
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
