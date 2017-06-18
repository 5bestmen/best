/*! @file acceptor_mgr.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  acceptor_mgr.cpp
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

#include "acceptor_mgr.h" 


CAcceptorMgr::CAcceptorMgr(ACE_INET_Addr& addr) : m_addrA(addr)
{
	m_bIsDoubleNet = false;

	m_pAcceptorA = nullptr;
	m_pAcceptorB = nullptr;
}

CAcceptorMgr::CAcceptorMgr(bool bDoubleNet, std::vector<ACE_INET_Addr>& vecAddress)
{
	ACE_ASSERT(vecAddress.size() >= 2);

	m_bIsDoubleNet = bDoubleNet;

	m_addrA = vecAddress[0];
	m_addrB = vecAddress[1];

	m_pAcceptorA = nullptr;
	m_pAcceptorB = nullptr;
}

CAcceptorMgr::~CAcceptorMgr()
{
	Shutdown();
}

bool CAcceptorMgr::Initialize()
{
	if (m_vecAddress.size() == 0)
	{
		m_bIsDoubleNet = false;
	}

	if (m_bIsDoubleNet)
	{ // 双网
		if (m_vecAddress.size() > 1)
		{
			m_pAcceptorA = std::make_shared<CAcceptorTask>(m_vecAddress[0]);

			m_pAcceptorB = std::make_shared<CAcceptorTask>(m_vecAddress[1]);
		}
		else
		{
			m_pAcceptorA = std::make_shared<CAcceptorTask>(m_vecAddress[0]);

			m_bIsDoubleNet = false;
		}
	}
	else
	{
		m_pAcceptorA = std::make_shared<CAcceptorTask>(m_addrA);
	}

	// 初始化connector
	return Run();
}

bool CAcceptorMgr::Run()
{
	if (m_bIsDoubleNet)
	{
		if (m_pAcceptorA->Start() != 0)
		{
			if (m_pAcceptorB->Start() != 0)
			{
				return false;
			}

			m_nCurWorkingNet = NET_B;

			return true;
		}

		m_nCurWorkingNet = NET_A;
	}
	else
	{
		if (m_pAcceptorA->Start() != 0)
		{
			if (m_pAcceptorB->Start() != 0)
			{
				return false;
			}

			m_nCurWorkingNet = NET_B;

			return true;
		}

		m_nCurWorkingNet = NET_A;
	}

	return true;
}

void CAcceptorMgr::Shutdown()
{
	if (m_pAcceptorA != nullptr)
	{
		m_pAcceptorA->Stop();
	}

	if (m_pAcceptorB != nullptr)
	{
		m_pAcceptorB->Stop();
	}
}

bool CAcceptorMgr::SwitchWorkingNet()
{
	if (!m_bIsDoubleNet)
	{
		// log 单网模式，切换失败
		return false;
	}

	if (m_nCurWorkingNet == NET_A)
	{
		m_nCurWorkingNet = NET_B;
	}
	else if (m_nCurWorkingNet == NET_B)
	{
		m_nCurWorkingNet = NET_B;
	}
	else
	{
		// log 当前工作网络错误
		return false;
	}

	return true;
}

bool CAcceptorMgr::SwitchWorkingNet(int nDestWorkingNet)
{
	if (nDestWorkingNet == NET_A)
	{
		if (m_nCurWorkingNet != NET_A)
		{
			m_nCurWorkingNet = NET_A;
		}
	}
	else if (nDestWorkingNet == NET_B)
	{
		if (m_nCurWorkingNet != NET_B)
		{
			m_nCurWorkingNet = NET_B;
		}
	}
	else
	{
		// log 设置工作网络错误
		return false;
	}

	return true;
}

/** @}*/
