/*! @file connector_mgr.h
<PRE>
********************************************************************************
模块名       :
文件名       :  connector_mgr.h
文件实现功能 :  作为客户端去连接其他节点
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   作为客户端去连接其他节点 
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.08
*******************************************************************************/
#ifndef CONNECTOR_MGR_H
#define CONNECTOR_MGR_H     
 
#include "connector.h"

#include <vector>
#include <string>
#include <memory>
#include <atomic>

#include "msg_queue.h"
#include "connector.h"
#include "netbus/nbdef.h"

class CConnectorTask;

/*! \struct  CConnectorMgr
*   \brief 客户端连接器，一个客户端一个线程 */
class CConnectorMgr  
{
public:
	CConnectorMgr(ACE_INET_Addr addr, CNodeConfig* pNodeConfig);
	CConnectorMgr(bool bDoubleNet, std::vector<ACE_INET_Addr>& vecAddress, CNodeConfig* pNodeConfig);
	~CConnectorMgr();

public:
	bool Initialize();

	bool Run();

	void Shutdown();

	// 发送数据
	bool SendData(const int8u *pBuf, size_t nLen, int32u msgLevel = GENERAL_MSG);
	/*! \fn int RecvData(unsigned char *pBuf, unsigned int nBufLen, unsigned int nTimeout = 0);
	*********************************************************************************************************
	** \brief RecvData
	** \details 从系统中接收数据
	** \param pBuf 接收数据buf
	** \param nBufLen 接收数据buf的总长度
	** \param nTimeout  超时时间
	** \return int	接收到的长度
	** \author Zhu Yongjin
	** \date 2017年1月23日
	** \note
	********************************************************************************************************/
	size_t RecvData(unsigned char *pBuf, size_t nBufLen, int32u nTimeout = 0);

	// 把消息放到【发送】消息队列
	bool PutMsgToSendQueue(const int8u *pBuf, size_t nLen, int32u msgLevel = GENERAL_MSG);

	// 从【发送】消息队列中取数据
	bool GetMsgFromSendQueue(const int8u * pBuf, size_t nBufLen, size_t *pCopySize, int32u ulTimeout);

	// 从【发送】消息队列中取数据
	int GetMsgFromSendQueue(ACE_Message_Block *&mb, ACE_Time_Value* timeout = 0);

	// 把消息放到【接收】消息队列
	bool PutMsgToRecvQueue(const int8u *pBuf, size_t nLen, int32u msgLevel = GENERAL_MSG);

	// 从【接收】消息队列中取数据
	bool GetMsgFromRecvQueue(const int8u * pBuf, int32u nBufLen, int32u *pCopySize, int32u ulTimeout);

	// 从【接收】消息队列中取数据
	int GetMsgFromRecvQueue(ACE_Message_Block *&mb, ACE_Time_Value* timeout = 0);

	// 设置当前工作的网络，是A网，还是B网
	/*void SetCurrentWorkingNet(int nWorkingNet)
	{
		m_nCurWorkingNet = nWorkingNet;
	}*/

public:
	// 获取当前工作的网络，是A网，还是B网
	int GetCurrentWorkingNet()
	{
		return m_nCurWorkingNet;
	}

	// 切换工作网络
	bool SwitchWorkingNet();

	// 切换到需要的工作网络
	bool SwitchWorkingNet(int nDestWorkingNet);

	// 获取消息队列
	CMsgQueueLite* GetMsgQueue()
	{
		return m_pRecvMsgQueue;
	}

	// 获取A网状态
	bool Get_A_NetState()
	{
		ACE_ASSERT(m_pNodeConfig);
		if (m_pNodeConfig == nullptr)
		{
			return false;
		}

		ACE_Guard<ACE_Thread_Mutex> guard(m_mutex);

		return m_pNodeConfig->CardStatus[NET_A] == 1 ? true : false;
	}

	// 获取B网状态
	bool Get_B_NetState()
	{
		ACE_ASSERT(m_pNodeConfig);
		if (m_pNodeConfig == nullptr)
		{
			return false;
		}

		ACE_Guard<ACE_Thread_Mutex> guard(m_mutex);

		return m_pNodeConfig->CardStatus[NET_B] == 1 ? true : false;
	}

	// 设置A网状态
	void Set_A_NetState(bool bState = true)
	{
		ACE_ASSERT(m_pNodeConfig);
		ACE_Guard<ACE_Thread_Mutex> guard(m_mutex);
		if (bState)
		{
			m_pNodeConfig->CardStatus[NET_A] = 1;
		}
		else
		{
			m_pNodeConfig->CardStatus[NET_A] = 0;
		}
	}

	// 设置B网状态
	void Set_B_NetState(bool bState = true)
	{
		ACE_ASSERT(m_pNodeConfig);
		ACE_Guard<ACE_Thread_Mutex> guard(m_mutex);
		if (bState)
		{
			m_pNodeConfig->CardStatus[NET_B] = 1;
		}
		else
		{
			m_pNodeConfig->CardStatus[NET_B] = 0;
		}
	}
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
	void LogDebugMsg(const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc);

protected:
	CConnectorTask* m_pConnTaskA;  //! 网络A
	CConnectorTask* m_pConnTaskB;  //! 网络B

	bool m_bIsDoubleNet;     //! 是否双网, true:是,false:否
	std::atomic<int>  m_nCurWorkingNet;   //! 当前工作的网络

	ACE_INET_Addr m_addrA;  //! A网地址
	ACE_INET_Addr m_addrB;  //! B网地址
	std::vector<ACE_INET_Addr> m_arrAddress;  //! 与上相同，服务器A、B网地址

	//! 是否shutdown
	std::atomic<bool> m_bShutdown;   

	// 从socket收到数据之后，扔到这里来
	CMsgQueueLite* m_pRecvMsgQueue;

	// 需要发送出去的数据暂存在这个队列
	CMsgQueueLite* m_pSendMsgQueue;

	// 线程互斥锁
	ACE_Thread_Mutex m_mutex; 

	// 节点信息
	CNodeConfig* m_pNodeConfig;
}; 

#endif // CONNECTOR_MGR_H


/** @}*/