
#include "heartbeat_sender.h"

#include "msg_queue.h"
#include "fesapi/fesdatadef.h"

#include <ace/Date_Time.h>

CMsgQueue& g_msgQueueHB = CMsgQueue::GetInstance();

CUdpSender::CUdpSender(const QString& strIP, const unsigned short nPort, NET_CONFIG* pNetCfg, 
	int32u nMyNodeIdx, int32s nAnotherIndex, bool bCheckTime/* = false*/, bool isExistMainSlave/* = false*/) :
	m_strIP(strIP),	m_nPort(nPort), m_pNetCfg(pNetCfg), m_nMyNodeIdx(nMyNodeIdx),
	m_nAnotherIndex(nAnotherIndex), m_bCheckTime(bCheckTime), m_isExistMainSlave(isExistMainSlave)
{
	Q_ASSERT(m_pNetCfg != nullptr && m_pNetCfg->pNodeConfig != nullptr && m_nMyNodeIdx >=0 && m_nMyNodeIdx < pNetCfg->NodeCount);

	m_isInit = false;
	m_pUdpSocket = nullptr;
}

CUdpSender::~CUdpSender()
{
	m_isInit = false;

	if (m_pUdpSocket != nullptr)
	{
		delete m_pUdpSocket;
		m_pUdpSocket = nullptr;
	}
}

void CUdpSender::InitSocket()
{
	if (!m_isInit)
	{
		m_pUdpSocket = new QUdpSocket();

		m_isInit = true;
	}
}

void CUdpSender::Send()
{
	if (!m_isInit || m_pUdpSocket == nullptr)
	{
		return;
	}
	// 如何存在主备节点，发送心跳前需判断是否进行主备切换
	if (m_isExistMainSlave)
	{
		Q_ASSERT(m_nAnotherIndex >= 0 && m_nAnotherIndex < m_pNetCfg->NodeCount);
		if (!g_msgQueueHB.GetSwitchQueue().is_empty())
		{
			int8u cMsg[MAX_EMSG_L + 1] = { 0 };

			int32u nCopySize = 0;

			ACE_Time_Value timeout = ACE_Time_Value(1);
			g_msgQueueHB.GetFromSwitchMQ((void*)cMsg, MAX_EMSG_L, &timeout);

			EMSG_BUF* msg = (EMSG_BUF*)cMsg;
			if (msg->MsgType == RECV_SWITCH_CMD_RES || msg->MsgType == RECV_SWITCH_CMD_RES)
			{
				// 缓冲1s后进行主备切换
				ACE_OS::sleep(1);

				// 进行主备切换
				if (m_pNetCfg->pNodeConfig[m_nMyNodeIdx].HostSlave != MAIN)
				{
					m_pNetCfg->pNodeConfig[m_nMyNodeIdx].HostSlave = MAIN;
				}

				if (m_pNetCfg->pNodeConfig[m_nAnotherIndex].HostSlave != SLAVE)
				{
					m_pNetCfg->pNodeConfig[m_nAnotherIndex].HostSlave = SLAVE;
				}
			}
		}
	}

	// 拼接心跳报文	
	int8u data[g_nSize] = { 0 };

	data[0] = HEARTBEAT_CODE;				// 特征码
	data[1] = m_pNetCfg->pNodeConfig[m_nMyNodeIdx].HostSlave;		// 主从状态
	strncpy((char*)&data[9], m_pNetCfg->pNodeConfig[m_nMyNodeIdx].HostName, HOST_NAME_SIZE);
	if (data[1] == MAIN && m_bCheckTime)
	{
		ACE_Date_Time tvTime(ACE_OS::gettimeofday());
		//buf[25] = 1;
		long year = tvTime.year();
		long month = tvTime.month();
		long day = tvTime.day();
		long hour = tvTime.hour();
		long minute = tvTime.minute();
		long second = tvTime.second();

		memcpy(&data[2], &year, 2);
		memcpy(&data[4], &month, 1);
		memcpy(&data[5], &day, 1);
		memcpy(&data[6], &hour, 1);
		memcpy(&data[7], &minute, 1);
		memcpy(&data[8], &second, 1);

		// 对时标志
		data[9 + HOST_NAME_SIZE] = 1;
	}
	else {
		data[9 + HOST_NAME_SIZE] = 0;
	}

	int nChecksum = 0;
	// 校验和
	for (int i = 0; i < g_nSize; i++)
	{
		nChecksum = nChecksum + data[i];
	}

	ACE_OS::memcpy(&data[g_nSize - 2], &nChecksum, 2);

	QByteArray msgArray((char*)data, g_nSize);

	qint64 len = m_pUdpSocket->writeDatagram(msgArray, QHostAddress(m_strIP), m_nPort);
	int i = 0;
}
