
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

// ��ȡ�첽������
NETBUS_INLINE
ACE_Asynch_Write_Stream& CNodeManager::Get_Asynch_Write_Stream()
{
	return *m_asychWriteStream;
}

// ��ȡʹ��״̬
NETBUS_INLINE
bool CNodeManager::GetNetUsing() const
{
	return m_bNetUsed;
}

// ����ʹ��״̬
NETBUS_INLINE
void CNodeManager::SetNetUsing(const bool bUsed/* = true*/)
{
	m_bNetUsed = bUsed;
}

// ���ý��յ��ͻ������ӵ�ʱ��
NETBUS_INLINE
void CNodeManager::SetConnectTime(ACE_UINT64 time)
{
	m_lConnTime = time;
}

// ��ȡ���յ��ͻ������ӵ�ʱ��
NETBUS_INLINE
ACE_UINT64 CNodeManager::GetConnectTime() const
{
	return m_lConnTime;
}

// ������ͬIP�����ӵĴ���
NETBUS_INLINE
void CNodeManager::SetConnCnt()
{
	++m_nConnCnt;
}

// ��ȡ��ͬIP�����ӵĴ���
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

// ��ȡ�첽������
NETBUS_INLINE
ACE_Asynch_Write_Stream& CNodeManager::Get_Asynch_Write_Stream()
{
	return m_asychWriteStream;
}

// ��ȡ�ڵ�����
NETBUS_INLINE
int32u CNodeManager::GetNodeIndex() const
{
	return m_nNodeIndex;
}

// ��ȡ�ڵ����к�
NETBUS_INLINE
int32u CNodeManager::GetMyNodeOccNo() const
{
	return m_nMyNodeOccNo;
}

// ��ȡʹ��״̬
NETBUS_INLINE
bool CNodeManager::GetNetUsing() const
{
	return m_bNetUsed;
}

// ����ʹ��״̬
NETBUS_INLINE
void CNodeManager::SetNetUsing(const bool bUsed/ * = true* /)
{
	m_bNetUsed = bUsed;
}

// ���ý��յ��ͻ������ӵ�ʱ��
NETBUS_INLINE
void CNodeManager::SetConnectTime(ACE_UINT64 time)
{
	m_lConnTime = time;
}

// ��ȡ���յ��ͻ������ӵ�ʱ��
NETBUS_INLINE
ACE_UINT64 CNodeManager::GetConnectTime() const
{
	return m_lConnTime;
}

// ������ͬIP�����ӵĴ���
NETBUS_INLINE
void CNodeManager::SetConnCnt()
{
	++m_nConnCnt;
}

// ��ȡ��ͬIP�����ӵĴ���
NETBUS_INLINE
int32u CNodeManager::GetConnCnt()
{
	return m_nConnCnt;
}*/
