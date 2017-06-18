
#ifndef _MODBUS_TCP_H
#define _MODBUS_TCP_H

#include "mbconfig.h"
#include "tcpclient.h"

void ModbusTCPThreadProc(void * lpParameter);

class CModbusTcpThreadParam
{
public:
	CModbusTcpThreadParam(int nChannelOccNo)
	{
		m_nChannelOccNo = nChannelOccNo;
	}
public:
	int GetChannelOccNo() { return m_nChannelOccNo; }
private:
	int m_nChannelOccNo;
};

enum
{
	BIT_ADDRESS = 6,
	BIT_FUNCODE = 7,
	BIT_DATA_LEN = 8,
	BIT_DATA_HEAD = 9,
};


class CModbusTCP
{
public:
	CModbusTCP(CModbusTcpThreadParam* pThreadParam);
	virtual ~CModbusTCP(void);

public:
	bool Initialize();
	bool Connect();
	bool Run();
	bool Shutdown();
	
private:
	void ProcessTeleCmd();
private:
	bool SendFrame(CFrameData* pFrameData);
	bool RecvFrame(CFrameData* pFrameData);
	bool ProcessFrame(CFrameData* pFrameData);
	int  GetFuncode(int nDataType);
	#define BUFFER_SIZE 4096
	unsigned char m_recv_buf[BUFFER_SIZE];
	unsigned char m_send_buf[BUFFER_SIZE];
private:
	void ParseAIN(CFrameData* pFrameData);
	void ParseDIN(CFrameData* pFrameData);

public:
	CModbusTcpThreadParam* GetThreadParam() { return m_pThreadParam; }
private:
	CModbusTcpThreadParam* m_pThreadParam;
	int32u m_nChannelOccNo;

private:
	CChannelData m_channeldata;

private:
	CTcpClient m_tcpclient;

private:
	CModbusParam m_modbusparam;
};

#endif // _MODBUS_TCP_H


