/*! @file connector_mgr.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  connector_mgr.h
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
*   \brief �ͻ�����������һ���ͻ���һ���߳� */
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

	// ��������
	bool SendData(const int8u *pBuf, size_t nLen, int32u msgLevel = GENERAL_MSG);
	/*! \fn int RecvData(unsigned char *pBuf, unsigned int nBufLen, unsigned int nTimeout = 0);
	*********************************************************************************************************
	** \brief RecvData
	** \details ��ϵͳ�н�������
	** \param pBuf ��������buf
	** \param nBufLen ��������buf���ܳ���
	** \param nTimeout  ��ʱʱ��
	** \return int	���յ��ĳ���
	** \author Zhu Yongjin
	** \date 2017��1��23��
	** \note
	********************************************************************************************************/
	size_t RecvData(unsigned char *pBuf, size_t nBufLen, int32u nTimeout = 0);

	// ����Ϣ�ŵ������͡���Ϣ����
	bool PutMsgToSendQueue(const int8u *pBuf, size_t nLen, int32u msgLevel = GENERAL_MSG);

	// �ӡ����͡���Ϣ������ȡ����
	bool GetMsgFromSendQueue(const int8u * pBuf, size_t nBufLen, size_t *pCopySize, int32u ulTimeout);

	// �ӡ����͡���Ϣ������ȡ����
	int GetMsgFromSendQueue(ACE_Message_Block *&mb, ACE_Time_Value* timeout = 0);

	// ����Ϣ�ŵ������ա���Ϣ����
	bool PutMsgToRecvQueue(const int8u *pBuf, size_t nLen, int32u msgLevel = GENERAL_MSG);

	// �ӡ����ա���Ϣ������ȡ����
	bool GetMsgFromRecvQueue(const int8u * pBuf, int32u nBufLen, int32u *pCopySize, int32u ulTimeout);

	// �ӡ����ա���Ϣ������ȡ����
	int GetMsgFromRecvQueue(ACE_Message_Block *&mb, ACE_Time_Value* timeout = 0);

	// ���õ�ǰ���������磬��A��������B��
	/*void SetCurrentWorkingNet(int nWorkingNet)
	{
		m_nCurWorkingNet = nWorkingNet;
	}*/

public:
	// ��ȡ��ǰ���������磬��A��������B��
	int GetCurrentWorkingNet()
	{
		return m_nCurWorkingNet;
	}

	// �л���������
	bool SwitchWorkingNet();

	// �л�����Ҫ�Ĺ�������
	bool SwitchWorkingNet(int nDestWorkingNet);

	// ��ȡ��Ϣ����
	CMsgQueueLite* GetMsgQueue()
	{
		return m_pRecvMsgQueue;
	}

	// ��ȡA��״̬
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

	// ��ȡB��״̬
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

	// ����A��״̬
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

	// ����B��״̬
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
	CConnectorTask* m_pConnTaskA;  //! ����A
	CConnectorTask* m_pConnTaskB;  //! ����B

	bool m_bIsDoubleNet;     //! �Ƿ�˫��, true:��,false:��
	std::atomic<int>  m_nCurWorkingNet;   //! ��ǰ����������

	ACE_INET_Addr m_addrA;  //! A����ַ
	ACE_INET_Addr m_addrB;  //! B����ַ
	std::vector<ACE_INET_Addr> m_arrAddress;  //! ������ͬ��������A��B����ַ

	//! �Ƿ�shutdown
	std::atomic<bool> m_bShutdown;   

	// ��socket�յ�����֮���ӵ�������
	CMsgQueueLite* m_pRecvMsgQueue;

	// ��Ҫ���ͳ�ȥ�������ݴ����������
	CMsgQueueLite* m_pSendMsgQueue;

	// �̻߳�����
	ACE_Thread_Mutex m_mutex; 

	// �ڵ���Ϣ
	CNodeConfig* m_pNodeConfig;
}; 

#endif // CONNECTOR_MGR_H


/** @}*/