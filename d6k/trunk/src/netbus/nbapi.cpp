/*! @file nbapi.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  nbapi.cpp
文件实现功能 :  网络总线
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   网络总线
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "nb_svc.h"
#include "log/log.h" 

#include "netbus/nbdef.h"
#include "netbus/nbapi.h"

#include "fesapi/fescnst.h"

#include <QObject> 
#include <QString> 

#include <vector>
#include <memory> 

#include <ace/Init_ACE.h>

#include "connector.h" 
#include "msg_queue.h"

std::vector<CNetbusSvc*> g_arrNetServices;

class CAceInit
{
public: 
	CAceInit()
	{
		ACE::init();
	}
	~CAceInit()
	{
		ACE::fini();
	}
};

static CAceInit s_PreInit;

extern "C"
{
	/*! \fn NBAPI bool StartNetBus(NET_CONFIG * pConf, const char *pszProjPath, const char * pszPredicateName, 
	                                                                      int32u nNodeType, int32u nStartMode)
	********************************************************************************************************* 
	** \brief StartNetBus 
	** \details 启动网络总线
	** \param pConf  网络配置情况
	** \param pszProjPath 工程配置情况
	** \param pszPredicateName  SCADA\FES
	** \param nNodeType 
	** \param nStartMode 
	** \return NBAPI bool 
	** \author LiJin 
	** \date 2016年11月7日 
	** \note 
	********************************************************************************************************/
	NBAPI bool StartNetBus(NET_CONFIG * pConf, const char *pszProjPath, const char * pszPredicateName, int32u nNodeType, int32u nStartMode)
	{
		Q_ASSERT(pConf);
		Q_ASSERT(pszProjPath);
		Q_ASSERT(pszPredicateName);

		if (pConf == nullptr || pszProjPath == nullptr || pszPredicateName == nullptr)
			return false;

		if (strlen(pszPredicateName) == 0)
			return false;

		Q_ASSERT(pConf->MyNodeOccNo > INVALID_OCCNO && pConf->MyNodeOccNo <= MAX_NODE_OCCNO);
		if (pConf->MyNodeOccNo == INVALID_OCCNO || pConf->MyNodeOccNo > MAX_NODE_OCCNO)
			return false;
	 
		// 判断是否已经加载过
		for (auto i : g_arrNetServices)
		{
			if (i->GetName().compare(pszPredicateName) == 0)
			{
				Q_ASSERT(false);
				return false;
			}
		}

		//std::shared_ptr<CNetbusSvc> pSvc = std::make_shared<CNetbusSvc>(pszPredicateName);
		CNetbusSvc* pSvc = new CNetbusSvc(pszPredicateName);

		bool bRet = pSvc->Initialize(pConf,pszProjPath, nNodeType, nStartMode);
		if (bRet)
		{
			g_arrNetServices.push_back(pSvc);
		}
		else
		{
			Q_ASSERT(false);
			delete pSvc;
		}

		return bRet;
	}

	NBAPI void StopNetBus(const char * pszPredicateName)
	{
		Q_ASSERT(pszPredicateName);
		if (strlen(pszPredicateName) == 0)
			return;


		/*for (auto i:g_arrNetServices)
		{
			if (i->GetName().compare(pszPredicateName)==0)
			{/
				i->Shutdown();
				i.reset();
			}
		}*/


		for (auto it = g_arrNetServices.begin(); it != g_arrNetServices.end();++it)
		{
			if ((*it)->GetName().compare(pszPredicateName) == 0)
			{
				(*it)->Shutdown();

				delete *it;

				g_arrNetServices.erase(it);				
				
				return;
			}
		}

		//Q_ASSERT(false);
		return;
	}

	// 动态增加系统客户端节点
	NBAPI void AddNode(NET_CONFIG * pConf)
	{

	}

	// 发送接口
	NBAPI int NBSendData(const char * pszPredicateName, const int8u * pMsg, int32u nMsgLen)
	{
		Q_ASSERT(pszPredicateName && pMsg);

		if (pszPredicateName == nullptr || strlen(pszPredicateName) == 0)
		{
			return 0;
		}

		for (auto i : g_arrNetServices)
		{
			if (i->GetName().compare(pszPredicateName) == 0)
			{ // 找到
				return i->SendData(pMsg, nMsgLen);
			}
		}

		Q_ASSERT(false);	

		return 0;
	}

	// 接收接口，
	NBAPI bool NBRecvData(const char * pszPredicateName, const int8u * pMsg, int32u nMsgLen, int32u *pCopySize, int32u ulTimeout)
	{
		Q_ASSERT(pszPredicateName && pMsg);

		if (pszPredicateName == nullptr || strlen(pszPredicateName) == 0)
		{
			return 0;
		}

#if 1
		for (auto i : g_arrNetServices)
		{
			if (i->GetShutdownFlag())
			{
				return false;
			}

			if (i->GetName().compare(pszPredicateName) == 0)
			{ // 找到
				return i->RecvData(const_cast<int8u*>(pMsg), nMsgLen, *pCopySize, ulTimeout);
			}
		}

		//Q_ASSERT(false);
#endif

		return false;
	}

	// 手动切换
	NBAPI bool ManualSwitchNode(char *pszPredicateName)
	{
		Q_ASSERT(pszPredicateName);

		if (pszPredicateName == nullptr || strlen(pszPredicateName) == 0)
		{
			return 0;
		}

#if 1
		for (auto i : g_arrNetServices)
		{
			if (i->GetShutdownFlag())
			{
				return false;
			}

			if (i->GetName().compare(pszPredicateName) == 0)
			{ // 找到
				return i->ManualSwitchNode();
			}
		}

		//Q_ASSERT(false);
#endif

		return false;
	}
}
 

/** @}*/
