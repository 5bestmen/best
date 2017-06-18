
#include "node_manager.h"

#include "netbus/nbdef.h"

NETBUS_INLINE
CNodeManager::CNodeManager(ACE_Asynch_Write_Stream* asychWriteStream)
	: m_asychWriteStream(asychWriteStream),m_bNetUsed(false), m_nConnCnt(0), m_lConnTime(0)
{

}

NETBUS_INLINE
CNodeManager::~CNodeManager()
{

}

// 获取异步发送流
NETBUS_INLINE
ACE_Asynch_Write_Stream& CNodeManager::Get_Asynch_Write_Stream()
{
	return *m_asychWriteStream;
}

// 获取使用状态
NETBUS_INLINE
bool CNodeManager::GetNetUsing() const
{
	return m_bNetUsed;
}

// 设置使用状态
NETBUS_INLINE
void CNodeManager::SetNetUsing(const bool bUsed/* = true*/)
{
	m_bNetUsed = bUsed;
}

// 设置接收到客户端连接的时间
NETBUS_INLINE
void CNodeManager::SetConnectTime(ACE_UINT64 time)
{
	m_lConnTime = time;
}

// 获取接收到客户端连接的时间
NETBUS_INLINE
ACE_UINT64 CNodeManager::GetConnectTime() const
{
	return m_lConnTime;
}

// 设置相同IP被连接的次数
NETBUS_INLINE
void CNodeManager::SetConnCnt()
{
	++m_nConnCnt;
}

// 获取相同IP被连接的次数
NETBUS_INLINE
int32u CNodeManager::GetConnCnt()
{
	return m_nConnCnt;
}


/*NETBUS_INLINE
CNodeManager::CNodeManager(ACE_Asynch_Write_Stream asychWriteStream, int32u nNodeIndex, int32u nMyNodeOccNo)
	: m_asychWriteStream(asychWriteStream), m_nNodeIndex(nNodeIndex), m_nMyNodeOccNo(nMyNodeOccNo),
	m_bNetUsed(false), m_nConnCnt(0), m_lConnTime(0)
{

}

NETBUS_INLINE
CNodeManager::~CNodeManager()
{

}

// 获取异步发送流
NETBUS_INLINE
ACE_Asynch_Write_Stream& CNodeManager::Get_Asynch_Write_Stream()
{
	return m_asychWriteStream;
}

// 获取节点索引
NETBUS_INLINE
int32u CNodeManager::GetNodeIndex() const
{
	return m_nNodeIndex;
}

// 获取节点排行号
NETBUS_INLINE
int32u CNodeManager::GetMyNodeOccNo() const
{
	return m_nMyNodeOccNo;
}

// 获取使用状态
NETBUS_INLINE
bool CNodeManager::GetNetUsing() const
{
	return m_bNetUsed;
}

// 设置使用状态
NETBUS_INLINE
void CNodeManager::SetNetUsing(const bool bUsed/ * = true* /)
{
	m_bNetUsed = bUsed;
}

// 设置接收到客户端连接的时间
NETBUS_INLINE
void CNodeManager::SetConnectTime(ACE_UINT64 time)
{
	m_lConnTime = time;
}

// 获取接收到客户端连接的时间
NETBUS_INLINE
ACE_UINT64 CNodeManager::GetConnectTime() const
{
	return m_lConnTime;
}

// 设置相同IP被连接的次数
NETBUS_INLINE
void CNodeManager::SetConnCnt()
{
	++m_nConnCnt;
}

// 获取相同IP被连接的次数
NETBUS_INLINE
int32u CNodeManager::GetConnCnt()
{
	return m_nConnCnt;
}*/
