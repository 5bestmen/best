
#ifndef _NODE_MANAGER_H_
#define _NODE_MANAGER_H_

#include <string>
#include <atomic>

#include "ace/Basic_Types.h"
#include "ace/Map_Manager.h"
#include "ace/Singleton.h"
#include "ace/Unbounded_Set.h"
#include "ace/Asynch_IO.h"

#include "netbus/nbdef.h"

class CNodeManager
{
public:

	CNodeManager(ACE_Asynch_Write_Stream* asychWriteStream);

	~CNodeManager();

	// ��ȡ�첽������
	ACE_Asynch_Write_Stream& Get_Asynch_Write_Stream();

	// ��ȡʹ��״̬
	bool GetNetUsing() const;

	// ����ʹ��״̬
	void SetNetUsing(const bool bUsed = true);

	// ���ý��յ��ͻ������ӵ�ʱ��
	void SetConnectTime(ACE_UINT64 time);

	// ��ȡ���յ��ͻ������ӵ�ʱ��
	ACE_UINT64 GetConnectTime() const;

	// ������ͬIP�����ӵĴ���
	void SetConnCnt();

	// ��ȡ��ͬIP�����ӵĴ���
	int32u GetConnCnt();

private:

	std::atomic<bool> m_bNetUsed;								//! true������ʹ�ã�false��û��ʹ��
	std::atomic<int32u> m_nConnCnt;								//! �����Ӵ���
	std::atomic<ACE_UINT64> m_lConnTime;						//! ����ʱ��
	ACE_Asynch_Write_Stream* m_asychWriteStream;				//! �첽������
};

/*class CNodeManager
{
public:

	CNodeManager(ACE_Asynch_Write_Stream asychWriteStream, int32u nNodeIndex, int32u nMyNodeOccNo);

    ~CNodeManager();

	// ��ȡ�첽������
	ACE_Asynch_Write_Stream& Get_Asynch_Write_Stream();

	// ��ȡ�ڵ�����
	int32u GetNodeIndex() const;

	// ��ȡ�ڵ����к�
	int32u GetMyNodeOccNo() const;

	// ��ȡʹ��״̬
	bool GetNetUsing() const;

	// ����ʹ��״̬
    void SetNetUsing(const bool bUsed = true);

	// ���ý��յ��ͻ������ӵ�ʱ��
    void SetConnectTime(ACE_UINT64 time);

	// ��ȡ���յ��ͻ������ӵ�ʱ��
    ACE_UINT64 GetConnectTime() const;

	// ������ͬIP�����ӵĴ���
	void SetConnCnt();

	// ��ȡ��ͬIP�����ӵĴ���
	int32u GetConnCnt();

private:
	
	std::atomic<bool> m_bNetUsed;								//! true������ʹ�ã�false��û��ʹ��
	std::atomic<int32u> m_nConnCnt;								//! �����Ӵ���
	std::atomic<int32u> m_nNodeIndex;							//! �ڵ��±�
	std::atomic<int32u> m_nMyNodeOccNo;							//! ���ڵ�����к�
	std::atomic<ACE_UINT64> m_lConnTime;						//! ����ʱ��
	ACE_Asynch_Write_Stream m_asychWriteStream;					//! �첽������
};*/

// �ڵ����
typedef ACE_UINT64 CONNECTION_IP;

typedef ACE_Map_Manager<CONNECTION_IP, CNodeManager*, ACE_Thread_Mutex>  CONNECTION_MAP;

class CGlobleConn
{
public:
	CONNECTION_MAP m_connMap;
};

typedef ACE_Unmanaged_Singleton<CGlobleConn, ACE_Thread_Mutex> M_SINGLETON;

typedef ACE_Map_Iterator<CONNECTION_IP, CNodeManager*, ACE_Thread_Mutex> CONNECTION_MAP_ITERATOR;

typedef ACE_Map_Entry<CONNECTION_IP, CNodeManager*> CONNECTION_MAP_ENTRY;

#include "node_manager.inl"

#endif
