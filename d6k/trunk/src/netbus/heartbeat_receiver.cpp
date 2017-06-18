
#include "heartbeat_receiver.h"

#include <time.h>
#include <stdio.h>

#include "ace/Date_Time.h"


CUdpRecver::CUdpRecver(const unsigned short nPort, NET_CONFIG* pNetCfg, const int32u nMyNodeIndex, const int32u nAnotherIndex, const int nABNet)
{
	m_isInit = false;
	m_pUdpSocket = nullptr;
	m_pNetCfg = pNetCfg;
	m_nPort = nPort;
	m_nMyNodeIndex = nMyNodeIndex;
	m_nAnotherIndex = nAnotherIndex;
	m_nABNet = nABNet;
	m_nTimeLoop = 0;

	m_bHostSlave = false;

	if (m_nAnotherIndex >= 0 && m_nAnotherIndex < pNetCfg->NodeCount && m_nAnotherIndex != m_nMyNodeIndex)
	{ // ��������
		m_bHostSlave = true;
	}
}

CUdpRecver::~CUdpRecver()
{
	m_isInit = false;

	if (m_pUdpSocket != nullptr)
	{
		delete m_pUdpSocket;
		m_pUdpSocket = nullptr;
	}
}

void CUdpRecver::InitSocket()
{
	m_pUdpSocket = new QUdpSocket(this);

	m_isInit = true;

	m_pUdpSocket->bind(QHostAddress::Any, m_nPort);
	QObject::connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(Recv()));
}


void CUdpRecver::Recv()
{
	Q_ASSERT(m_pUdpSocket != nullptr);
	if (m_pUdpSocket == nullptr || !m_isInit)
	{
		return;
	}

	// �������ݰ�����ʱ����true
	while (m_pUdpSocket->hasPendingDatagrams())
	{
		QByteArray datagram;

		// resize Ϊdatageam���ô�С,pendingDatagramSize ����udosocket��һ�����ݰ��Ĵ�С
		datagram.resize(m_pUdpSocket->pendingDatagramSize());

		// readDatagram �����ݰ�
		m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
		
		if (datagram.size() != 32)
		{
			continue;
		}

		HandleMsg((int8u*)datagram.data());
	}
}


void CUdpRecver::HandleMsg(const int8u* pData)
{
	Q_ASSERT(pData && m_pNetCfg && m_pNetCfg->pNodeConfig);
	if (pData == nullptr || m_pNetCfg == nullptr || m_pNetCfg->pNodeConfig == nullptr)
	{
		return;
	}

	QMutexLocker locker(&m_pMutex);

	if (pData[0] != HEARTBEAT_CODE)
	{
		// log
		return;
	}

	NODE_CONFIG* pConfig = m_pNetCfg->pNodeConfig;
	int32u nCnt = m_pNetCfg->NodeCount;
	for (int32u i = 0; i < nCnt; i++)
	{
		// �Ƚ�������
		// �ſ�ע�Ͳ����Լ������Լ�����������
		//if (ACE_OS::strcasecmp((char*)pData + 9, pConfig[i].HostName) == 0)
		if (ACE_OS::strcasecmp((char*)pData + 9, pConfig[i].HostName) == 0 && m_nMyNodeIndex == i)
		{
			ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
			pConfig[i].SwitchTime = nowTime;

			continue;
		}

		if (ACE_OS::strcasecmp((char*)pData + 9, pConfig[i].HostName) == 0 && m_nMyNodeIndex != i)
		{
			ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
			pConfig[i].SwitchTime = nowTime;

			// �������׼��,��Ҫ���Բ�ڵ�����״̬��
			if (m_bHostSlave)
			{
				if (i == m_nAnotherIndex)
				{
					// �������˫��,��OccNo���Ϊ����
					if (pData[1] == MAIN && m_pNetCfg->pNodeConfig[m_nMyNodeIndex].HostSlave == MAIN)
					{
						if (m_pNetCfg->pNodeConfig[m_nAnotherIndex].OccNo > m_pNetCfg->pNodeConfig[m_nMyNodeIndex].OccNo)
						{
							m_pNetCfg->pNodeConfig[m_nAnotherIndex].HostSlave = SLAVE;
						}
						else
						{
							m_pNetCfg->pNodeConfig[m_nMyNodeIndex].HostSlave = SLAVE;
						}
					}
					// �������˫��,��OccNoС��Ϊ����
					else if (pData[1] == SLAVE && m_pNetCfg->pNodeConfig[m_nMyNodeIndex].HostSlave == SLAVE)
					{
						if (m_pNetCfg->pNodeConfig[m_nAnotherIndex].OccNo > m_pNetCfg->pNodeConfig[m_nMyNodeIndex].OccNo)
						{
							m_pNetCfg->pNodeConfig[m_nMyNodeIndex].HostSlave = MAIN;
						}
						else
						{
							m_pNetCfg->pNodeConfig[m_nAnotherIndex].HostSlave = MAIN;
						}
					}
				}
			}

			// ����ڵ�״̬��������̬,���޸ĳ�����̬
			if (pConfig[i].CardStatus[m_nABNet] != 1)
			{
				// log
				pConfig[i].CardStatus[m_nABNet] = 1;
			}

			// ����״̬�ж�
			if (pData[1] != pConfig[i].HostSlave)
			{
				// log ����״̬�����仯
				pConfig[i].HostSlave = pData[1];
			}

			// ��ʱ�ж�(������Ϊ������ʱ��־��1�����Ƕ�ʱ�ڵ㱾��)
			// �ſ�ע��
			//if ((pData[1] == MAIN) && (pData[9 + NAME_SIZE] == 1))
			if ((pData[1] == MAIN) && (pData[9 + HOST_NAME_SIZE] == 1) && (m_pNetCfg->CheckTimeNodeOccNo != pConfig[i].OccNo))
			{ // ��ʱ
				
				unsigned char bufYear[3] = { 0 };
				unsigned short year = 0;
				ACE_OS::memcpy(&year, &pData[2], 2);
				unsigned short month = (unsigned short)pData[4];
				unsigned short day = (unsigned short)pData[5];
				unsigned short hour = (unsigned short)pData[6];
				unsigned short minute = (unsigned short)pData[7];
				unsigned short second = (unsigned short)pData[8];

				if (year < 1970 || (month < 1 || month > 12) || (day < 1 || day > 31) ||
					(hour < 0 || hour >= 24) || (minute < 0 || minute >= 60) || (second < 0 || second >= 60))
				{
					// log
					return;
				}
#ifdef WIN32
				SYSTEMTIME systime;
				systime.wYear = 0;
				systime.wMonth = 0;
				systime.wDay = 0;
				systime.wHour = 0;
				systime.wMinute = 0;
				systime.wSecond = 0;
				systime.wMilliseconds = 0;

				systime.wYear = year;
				memcpy(&systime.wMonth, &month, 1);
				memcpy(&systime.wDay, &day, 1);				
				memcpy(&systime.wHour, &hour, 1);
				memcpy(&systime.wMinute, &minute, 1);
				memcpy(&systime.wSecond, &second, 1);				

				if (++m_nTimeLoop > 30)
				{
					SetLocalTime(&systime);
					m_nTimeLoop = 0;
				}
#else
				struct tm nowtime;

				time_t t;

				nowtime.tm_sec = second;/* Seconds.[0-60](1 leap second)*/

				nowtime.tm_min = minute;/* Minutes.[0-59] */

				nowtime.tm_hour = hour;/* Hours. [0-23] */

				nowtime.tm_mday = day;/*  Day.[1-31]  */

				nowtime.tm_mon = month - 1;/* Month.[0-11]*/

				nowtime.tm_year = year - 1900;/* Year- 1900.*/

				nowtime.tm_isdst = -1;/*DST.[-1/0/1]*/

				t = mktime(&nowtime);

				stime(&t);
#endif
				
			}
		}
	}
}

