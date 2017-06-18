/*! @file node_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  node_svc.cpp
文件实现功能 :  节点基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   节点基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#include "node_svc.h"
#include "log/log.h" 
#include "nb_svc.h"

#include <fesapi/fescnst.h>

#include <QObject> 
#include <QString> 

 

CNodeSvc::CNodeSvc(NET_CONFIG* pNetConfig, int32u nCurrentNodeIndex) : m_pNetConfig(pNetConfig), m_nCurrentNodeIndex(nCurrentNodeIndex)
{
	ACE_ASSERT(m_pNetConfig != nullptr);

	m_nNodeState = STATE_UNKOWN;

}

CNodeSvc::~CNodeSvc( )
{

}
/*! \fn bool CNodeSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CNodeSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CNodeSvc::Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode)
{
	 

	return true;
}

void CNodeSvc::Run()
{
 
}

void CNodeSvc::Shutdown()
{ 
	 
}

void CNodeSvc::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("NetBus", szLogTxt, nLevel, nullptr);
}

void CNodeSvc::SetNodeState(int nState)
{
	Q_ASSERT(nState >= 0 && nState <= STATE_SLAVE);
	if (nState < 0 || nState > STATE_SLAVE)
		nState = 0;

	m_nNodeState = nState;
}

bool CNodeSvc::NodeSplit()
{
	ACE_ASSERT(m_pNetConfig && m_pNetConfig->pNodeConfig && m_nCurrentNodeIndex >= 0);
	if (m_pNetConfig == nullptr || m_pNetConfig->pNodeConfig == nullptr)
	{
		return false;
	}
	for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
	{
		if (i == m_nCurrentNodeIndex)
		{
			continue;
		}

		if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_FES)
		{
			// todo:暂时只考虑A网的情况
			ACE_UINT64 addr = ACE_OS::inet_addr(m_pNetConfig->pNodeConfig[i].chIP[0]);

			NODE_INFO info;
			info.addr = addr;
			info.nDestOccNo = m_pNetConfig->pNodeConfig[i].OccNo;

			m_vecFesNodeIP.push_back(info);
		}
		else if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_CLIENT)
		{
			// todo:暂时只考虑A网的情况
			ACE_UINT64 addr = ACE_OS::inet_addr(m_pNetConfig->pNodeConfig[i].chIP[0]);

			NODE_INFO info;
			info.addr = addr;
			info.nDestOccNo = m_pNetConfig->pNodeConfig[i].OccNo;

			m_vecWorkStationNodeIP.push_back(info);
		}
		else
		{
			continue;
		}
	}

	return true;
}

bool CNodeSvc::SendData(const int8u * pMsg, int32u nLen)
{
	Q_ASSERT(false);
	return 0;
}

bool CNodeSvc::RecvData( int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout)
{
	Q_ASSERT(false);
	return false;
}

/** @}*/
