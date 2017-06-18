/*! @file net_config.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  net_config.h
�ļ�ʵ�ֹ��� :  ϵͳ�������ü�״̬
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ϵͳ�������ü�״̬
*  @author  LiJin
*  @version 1.0
*  @date   2016.11.26
*******************************************************************************/
#ifndef NET_CONFIG_H
#define NET_CONFIG_H  

#include "datatypes.h"
#include "netbus/nbdef.h"

#include <string>
#include <atomic>
#include <map>
#include <memory>

#if 0
struct NODE_CONFIG
{
	int32u OccNo;							//!  �ڵ������ݿ��е�ID
	int32u SlaveOccNo;
	int32u NodeType;						//!  �ڵ�����
	int32u NetAIPAddr;                      //! IP address of net1, filled by cfg. 
	int32u NetBIPAddr;                      //! IP address of net2, filled by cfg. 

	int32u NetAIpV6Addr[4];                 //! IPV6 ��A����ַ
	int32u NetBIpV6Addr[4];                 //! IPV6 ��B����ַ

	int8u IsCheckTime;						//�� �Ƿ��ʱ
	int8u IsDefaultHost;					//�� �Ƿ�Ĭ��Ϊ��

	char HostName[NAME_SIZE];		    	//�� ��������
	char TagName[NAME_SIZE];			    //�� ����
	// ʵʱֵ
	int32u HostSlave;						//�� ����״̬
	int8u CardStatus[2];					//�� AB��״̬
};

typedef struct NODE_CONFIG CNodeConfig; 
/*! \struct  NET_CONFIG
*   \brief ����ڵ㶨�� */
struct NET_CONFIG
{
	int32u IsDoubleNet;					//! �Ƿ�˫��
	char CheckTime[NAME_SIZE];	        //! ��ʱ�ڵ����
	int32u NodeCount;					//! �ڵ���
	CNodeConfig* pNodeConfig;			//! �ڵ��ָ��
};

#endif


struct NET_CONFIG;

class CNodeInfo : protected CNodeConfig
{
public:
	CNodeInfo();
	CNodeInfo(NODE_CONFIG *pNode);

	~CNodeInfo();

	enum CardIdx:int32u
	{
		NET_A = 0,
		NET_B = 1,
	};

public:
	int32u GetOccNo() const
	{
		return OccNo;
	}
	// ��ȡ����״̬
	int32u GetCardState(CardIdx nIdx) const
	{
		return m_nCardState[nIdx];
	}
	// ��������״̬
	void SetCardStarte(CardIdx nIdx,int32u nState)
	{
		m_nCardState[nIdx] = nState;

		this->CardStatus[nIdx] = static_cast<int8u> (nState);
	}
	// ��ȡ�ڵ�״̬
	int32u GetNodeState() const
	{
		return m_nHostState;
	}
	// ���ýڵ�״̬
	void SetNodeState(int32u nState)
	{
		m_nHostState = nState;

		this->HostSlave = nState;
	}

private:
	//! ˫����
	std::atomic<int32u> m_nCardState[2];
	//! ����״̬
	std::atomic<int32u> m_nHostState;

};

class CNetState
{
public:
	CNetState();
	CNetState(NET_CONFIG *pConfig);
	~CNetState();
public:


protected:
	NET_CONFIG *m_pNetConfig;

	std::map <int32u, std::shared_ptr<CNodeInfo>> m_mapNetConf;
};

#endif // NET_CONFIG_H


/** @}*/

