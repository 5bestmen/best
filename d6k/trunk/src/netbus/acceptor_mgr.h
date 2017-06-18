/*! @file acceptor_mgr.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  acceptor_mgr.h
�ļ�ʵ�ֹ��� :  ��Ϊ�����ȥ�ȴ������ڵ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��Ϊ�����ȥ�ȴ������ڵ������
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
*   \brief ����� */
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

	// ��������
	int SendData(unsigned char *pBuf, unsigned int nLen);

	// ��ϵͳ�н�������
	int RecvData(unsigned char *pBuf, unsigned int nBufLen, unsigned int nTimeout);

	// ��ȡ��ǰ���������磬��A��������B��
	int GetCurrentWorkingNet()
	{
		return m_nCurWorkingNet;
	}

	// �л���������
	bool SwitchWorkingNet();

	// �л�����Ҫ�Ĺ�������
	bool SwitchWorkingNet(int nDestWorkingNet);

protected:
 
	// AB��ͬʱ��������
	std::shared_ptr<CAcceptorTask> m_pAcceptorA;
	std::shared_ptr<CAcceptorTask> m_pAcceptorB;

private:
	bool m_bIsDoubleNet;     //! �Ƿ�˫��

	std::atomic<int>  m_nCurWorkingNet;   //! ��ǰ����������  

	ACE_INET_Addr m_addrA; // A����ַ
	ACE_INET_Addr m_addrB; // B����ַ

	// ���AB����ַ
	std::vector<ACE_INET_Addr> m_vecAddress;
}; 

#endif // CONNECTOR_MGR_H


/** @}*/