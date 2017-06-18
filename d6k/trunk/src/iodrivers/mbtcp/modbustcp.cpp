
#include "modbustcp.h"

#include "fesapi/fesapi.h"
#include "mail/mail.h"
#include "utils/misc.h"
#include "ace/Thread_Manager.h"


CModbusTCP::CModbusTCP(CModbusTcpThreadParam* pThreadParam)
{
	m_pThreadParam = pThreadParam;
	m_nChannelOccNo = m_pThreadParam->GetChannelOccNo();
}

CModbusTCP::~CModbusTCP(void)
{
	if (m_pThreadParam != NULL)
	{
		delete m_pThreadParam;
		m_pThreadParam = NULL;
	}
}

void ModbusTCPThreadProc(void * lpParameter)
{
	CModbusTCP* pModbusTCP = new CModbusTCP((CModbusTcpThreadParam*)lpParameter);

	if (!pModbusTCP->Initialize())
		return;

	if (!pModbusTCP->Connect())
		return;

	pModbusTCP->Run();
	
	pModbusTCP->Shutdown();

	delete pModbusTCP;
}

bool CModbusTCP::Initialize()
{
	if (OpenIO(m_nChannelOccNo) == K_FALSE)
		return false;

	if (!m_channeldata.LoadDevices(&m_modbusparam,m_nChannelOccNo))
		return false;

	return true;
}

bool CModbusTCP::Connect()
{
	std::string strIp("127.0.0.1");
	int nPort = 502;

	//GetChannelIpandPort(nChannelID,strIp,nPort);

	while (true)
	{
		if (m_tcpclient.Open((char*)strIp.c_str(), nPort))
			break;

		if (IsDriverQuit(m_nChannelOccNo)==K_TRUE)
			return false;

		Utils::USleep(1000);
	}

	return true;
}

bool CModbusTCP::Run()
{
	while (IsDriverQuit(m_nChannelOccNo)==K_FALSE)
	{
		//////////////////////////////////////////////////////////////////////////
		//按装置轮询
		std::map<int32u, CDeviceData*>* pDeviceMap = m_channeldata.GetMapDevices();
		std::map<int32u, CDeviceData*>::iterator it;
		for (it = pDeviceMap->begin(); it != pDeviceMap->end(); ++it)
		{
			int32u nDeviceOccNo = it->first;
			CDeviceData* pDeviceData = it->second;
			if (pDeviceData == NULL)
				continue;

			if (pDeviceData->IsDeviceHalt())
				continue;

			std::vector<CFrameData> vecFrameData = pDeviceData->GetFrameData();

			for (int i = 0; i < vecFrameData.size(); i++)
			{
				ProcessTeleCmd();

				CFrameData framedata = vecFrameData[i];

				if (SendFrame(&framedata))
				{
					Utils::USleep(10);

					if (RecvFrame(&framedata))
					{
						Utils::USleep(1);

						ProcessFrame(&framedata);
					}
				}
			}
		}

		Utils::USleep(1);
	}

	return true;
}

bool CModbusTCP::Shutdown()
{
	CloseIO(m_nChannelOccNo);
	
	m_channeldata.DeleteDevices();

	return true;
}

int CModbusTCP::GetFuncode(int nDataType)
{
	if (nDataType == DATATYPE_AIN)
		return m_modbusparam.m_nFunCodeAIN;

	if (nDataType == DATATYPE_DIN)
		return m_modbusparam.m_nFunCodeDIN;

	return 0x0;
}

void CModbusTCP::ProcessTeleCmd()
{
	/*
	NBM_TELECTRL command;
	if (!m_pKernel->PopCommand(m_nGroupID, m_nChannelID, nDeviceID, command))
		return;

	switch (command.m_nCtrlType)
	{
	case TELECTRL_REQUEST_SELECT:
		{
			//回确认
			command.m_nCtrlType = TELECTRL_ACK_SELECT;
			m_pKernel->PushCommand(-1, -1, -1, command);
		}
		break;
	case TELECTRL_REQUEST_UNSELECT:
		{
			//回确认
			command.m_nCtrlType = TELECTRL_ACK_UNSELECT;
			m_pKernel->PushCommand(-1, -1, -1, command);
		}
		break;
	case TELECTRL_REQUEST_EXECUTE:
		{
			//下发遥控
			//modbus忽略nCmdType
			int nStartAddrBias = (command.m_nDataType == DATATYPE_ANALOG) ? GetParam()->m_nAnalogTelectrlStartAddr : GetParam()->m_nBinaryTelectrlStartAddr;
			command.m_nDataID = command.m_nDataID + nStartAddrBias;
			bool IsOK = SendTeleCmd(command.m_nDeviceID, command.m_nDataID, command.m_nDataType, command.m_fValue);

			char buf[128];
			sprintf(buf, "[telectrl] DeviceID:%d DataID:%d CtrlType:%d fValue:%f",
			command.m_nDeviceID, command.m_nDataID, command.m_nCtrlType, command.m_fValue);
			string strMsg(buf);
			strMsg += IsOK ? "[OK]" : "[FAILED]";
			m_pKernel->Log2File("log/modbus_telectrl.log", (char*)strMsg.c_str(), true);
	
			//回确认
			command.m_nCtrlType = TELECTRL_ACK_EXECUTE;
			m_pKernel->PushCommand(-1, -1, -1, command);
		}
		break;
	}
	*/
}

bool CModbusTCP::SendFrame(CFrameData* pFrameData)
{
	int32u  nSendLen = 6;

	m_send_buf[nSendLen++] = pFrameData->m_nDeviceAddress;
	m_send_buf[nSendLen++] = GetFuncode(pFrameData->m_nDataType);
	m_send_buf[nSendLen++] = HIBYTE(pFrameData->m_nStartAddr);
	m_send_buf[nSendLen++] = LOBYTE(pFrameData->m_nStartAddr);
	m_send_buf[nSendLen++] = HIBYTE(pFrameData->m_nCount);
	m_send_buf[nSendLen++] = LOBYTE(pFrameData->m_nCount);

	/*
	unsigned short usCRC = CRC16(m_send_buf, m_send_len);
	m_send_buf[m_send_len++] = LOBYTE(usCRC);
	m_send_buf[m_send_len++] = HIBYTE(usCRC);
	*/

	m_send_buf[0] = 0;
	m_send_buf[1] = 0;
	m_send_buf[2] = 0;
	m_send_buf[3] = 0;
	m_send_buf[4] = HIBYTE(nSendLen - 6); //0
	m_send_buf[5] = LOBYTE(nSendLen - 6);//6

	m_tcpclient.SendMsg((char*)(m_send_buf), nSendLen);

	std::string strBuff = Utils::FormatBuffer((char*)(m_send_buf), nSendLen);

	return true;
}

bool CModbusTCP::RecvFrame(CFrameData* pFrameData)
{
	int32u nRecvLen = m_tcpclient.RecvMsg((char*)(m_recv_buf), BUFFER_SIZE);

	std::string strBuff = Utils::FormatBuffer((char*)(m_recv_buf), nRecvLen);

	return true;
}

bool CModbusTCP::ProcessFrame(CFrameData* pFrameData)
{
	int8u devAddr, funCode; //功能码

	devAddr = m_recv_buf[BIT_ADDRESS];
	funCode = m_recv_buf[BIT_FUNCODE];

	if (funCode == m_modbusparam.m_nFunCodeAIN)
	{
		ParseAIN(pFrameData);
	}
	else if (funCode == m_modbusparam.m_nFunCodeDIN)
	{
		ParseDIN(pFrameData);
	}

	return true;
}

void CModbusTCP::ParseAIN(CFrameData* pFrameData)
{
	int16s realData;

	int nAinCount = MAKEWORD(m_recv_buf[5], m_recv_buf[4]) / 2;

	if (nAinCount > m_modbusparam.m_nPerFrameAIN)
	{
		nAinCount = m_modbusparam.m_nPerFrameAIN;
	}

	int8u* pDataBuf = m_recv_buf + BIT_DATA_HEAD;//数据区开始

	for (int i = 0; i < nAinCount; i++)
	{
		realData = pDataBuf[2 * i] * 256 + pDataBuf[2 * i + 1];

		int32u nAddress = pFrameData->m_nStartAddr + i;
		int32u nDeviceOccNo = pFrameData->m_nDeviceOccNo;
		
		//IO设值
		int8u nQuality = 123;
		int32u nAinOccNo = m_channeldata.GetAinOccNo(nDeviceOccNo, nAddress);
		IoSetAINValue(m_pThreadParam->GetChannelOccNo(), nAinOccNo, realData, nQuality);
	}
}

void CModbusTCP::ParseDIN(CFrameData* pFrameData)
{
	int32u ucDataBytes = MAKEWORD(m_recv_buf[5], m_recv_buf[4]);

	if (ucDataBytes > (m_modbusparam.m_nPerFrameDIN + 7) / 8)
	{
		ucDataBytes = (m_modbusparam.m_nPerFrameDIN + 7) / 8;
	}

	unsigned char* pDataBuf = m_recv_buf + BIT_DATA_HEAD;//数据区开始

	int nPointIndex = 0;

	for (int i = 0; i < ucDataBytes; i++)
	{
		unsigned char uChar = pDataBuf[i];
		for (int j = 0; j < 8; j++)
		{
			//if ((8*i+j) < m_pData[nIndex].binary_count-1)  //将最后一点通讯状态点去除掉
			{
				unsigned char realData = (uChar >> j) & 0x01;

				int nAddress = pFrameData->m_nStartAddr + i * 8 + j;
				int nDeviceOccNo = pFrameData->m_nDeviceOccNo;

				//IO设值
				int8u nQuality = 123;
				int32u nDinOccNo = m_channeldata.GetDinOccNo(nDeviceOccNo, nAddress);
				IoSetDINValue(m_pThreadParam->GetChannelOccNo(), nDinOccNo, realData, nQuality);
			}
		}
	}
}


