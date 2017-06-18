
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

	// 获取异步发送流
	ACE_Asynch_Write_Stream& Get_Asynch_Write_Stream();

	// 获取使用状态
	bool GetNetUsing() const;

	// 设置使用状态
	void SetNetUsing(const bool bUsed = true);

	// 设置接收到客户端连接的时间
	void SetConnectTime(ACE_UINT64 time);

	// 获取接收到客户端连接的时间
	ACE_UINT64 GetConnectTime() const;

	// 设置相同IP被连接的次数
	void SetConnCnt();

	// 获取相同IP被连接的次数
	int32u GetConnCnt();

private:

	std::atomic<bool> m_bNetUsed;								//! true：正在使用，false：没有使用
	std::atomic<int32u> m_nConnCnt;								//! 被连接次数
	std::atomic<ACE_UINT64> m_lConnTime;						//! 连接时间
	ACE_Asynch_Write_Stream* m_asychWriteStream;				//! 异步发送流
};

/*class CNodeManager
{
public:

	CNodeManager(ACE_Asynch_Write_Stream asychWriteStream, int32u nNodeIndex, int32u nMyNodeOccNo);

    ~CNodeManager();

	// 获取异步发送流
	ACE_Asynch_Write_Stream& Get_Asynch_Write_Stream();

	// 获取节点索引
	int32u GetNodeIndex() const;

	// 获取节点排行号
	int32u GetMyNodeOccNo() const;

	// 获取使用状态
	bool GetNetUsing() const;

	// 设置使用状态
    void SetNetUsing(const bool bUsed = true);

	// 设置接收到客户端连接的时间
    void SetConnectTime(ACE_UINT64 time);

	// 获取接收到客户端连接的时间
    ACE_UINT64 GetConnectTime() const;

	// 设置相同IP被连接的次数
	void SetConnCnt();

	// 获取相同IP被连接的次数
	int32u GetConnCnt();

private:
	
	std::atomic<bool> m_bNetUsed;								//! true：正在使用，false：没有使用
	std::atomic<int32u> m_nConnCnt;								//! 被连接次数
	std::atomic<int32u> m_nNodeIndex;							//! 节点下标
	std::atomic<int32u> m_nMyNodeOccNo;							//! 本节点的排行号
	std::atomic<ACE_UINT64> m_lConnTime;						//! 连接时间
	ACE_Asynch_Write_Stream m_asychWriteStream;					//! 异步发送流
};*/

// 节点管理
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
