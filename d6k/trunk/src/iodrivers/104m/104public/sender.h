
#ifndef _SENDER__H
#define _SENDER__H

#include "define_104.h"
#include "ihost.h"

class C104Impl;  //impl

class CSender
{
public:
	CSender(IHost* pHost, C104Impl* pImpl);
	virtual ~CSender();

public:
	bool OnCommand(NBM_TELECTRL* pTelectrl);

public:
	bool Send_U(int nType);
	bool Send_S();
	bool Send_S_BeforeClose(int nRecvNum);
	int  Send_I(char* pBuff, int nLen);
public:
	bool FormatAPCI(char* pBuff, int nAsduLen);
private:
	bool OnSendGeneralCall(NBM_TELECTRL* pTelectrl);
	bool OnSendKwhGeneralCall(NBM_TELECTRL* pTelectrl);
	bool OnSendSyncTime(NBM_TELECTRL* pTelectrl);
private://ң��
	bool OnSendSetBinarySPRequest(NBM_TELECTRL* pTelectrl);           //!<�������
	bool OnSendSetBinaryDPRequest(NBM_TELECTRL* pTelectrl);           //!<˫�����
	bool OnSendSetAnalogNormalizeRequest(NBM_TELECTRL* pTelectrl);    //!<��һ�����
	bool OnSendSetAnalogScaleRequest(NBM_TELECTRL* pTelectrl);        //!<��Ȼ����
	bool OnSendSetAnalogShortFloatRequest(NBM_TELECTRL* pTelectrl);   //!<�̸��������

private:
	IHost* m_pHost;
	C104Impl*   m_pImpl;
};


#endif//_SENDER__H
