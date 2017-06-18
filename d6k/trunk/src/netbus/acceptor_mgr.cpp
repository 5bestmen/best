/*! @file acceptor_mgr.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  acceptor_mgr.cpp
�ļ�ʵ�ֹ��� :  ��Ϊ�ͻ���ȥ���������ڵ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��Ϊ�ͻ���ȥ���������ڵ�
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
	{ // ˫��
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

	// ��ʼ��connector
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
		// log ����ģʽ���л�ʧ��
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
		// log ��ǰ�����������
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
		// log ���ù����������
		return false;
	}

	return true;
}

/** @}*/
