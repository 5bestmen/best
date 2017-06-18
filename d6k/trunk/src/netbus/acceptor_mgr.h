/*! @file acceptor_mgr.h
<PRE>
********************************************************************************
模块名       :
文件名       :  acceptor_mgr.h
文件实现功能 :  作为服务端去等待其他节点的连接
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   作为服务端去等待其他节点的连接
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.08
*******************************************************************************/
#ifndef ACCEPTOR_MGR_H
#define ACCEPTOR_MGR_H     
 
#include "acceptor.h"

#include <vector>
#include <string>
#include <memory>
 
#include <atomic>

/*! \struct  CAcceptorMgr
*   \brief 服务端 */
class CAcceptorMgr  
{
public:
	CAcceptorMgr(ACE_INET_Addr& addr);
	CAcceptorMgr(bool bDoubleNet, std::vector<ACE_INET_Addr>& vecAddress);
	~CAcceptorMgr();
	
	enum :int
	{
		NET_A = 0,
		NET_B = 1,
	};

public:
	bool Initialize();

	bool Run();

	void Shutdown();

	// 发送数据
	int SendData(unsigned char *pBuf, unsigned int nLen);

	// 从系统中接收数据
	int RecvData(unsigned char *pBuf, unsigned int nBufLen, unsigned int nTimeout);

	// 获取当前工作的网络，是A网，还是B网
	int GetCurrentWorkingNet()
	{
		return m_nCurWorkingNet;
	}

	// 切换工作网络
	bool SwitchWorkingNet();

	// 切换到需要的工作网络
	bool SwitchWorkingNet(int nDestWorkingNet);

protected:
 
	// AB网同时监听服务
	std::shared_ptr<CAcceptorTask> m_pAcceptorA;
	std::shared_ptr<CAcceptorTask> m_pAcceptorB;

private:
	bool m_bIsDoubleNet;     //! 是否双网

	std::atomic<int>  m_nCurWorkingNet;   //! 当前工作的网络  

	ACE_INET_Addr m_addrA; // A网地址
	ACE_INET_Addr m_addrB; // B网地址

	// 存放AB网地址
	std::vector<ACE_INET_Addr> m_vecAddress;
}; 

#endif // CONNECTOR_MGR_H


/** @}*/