
#ifndef _104_IMPL_H
#define _104_IMPL_H

#include "sender.h"
#include "recver.h"
#include "ontimer.h"
#include "ihost.h"

#include "param104.h"

class IHost;

class C104Impl
{
public:
	C104Impl(IHost* pHost, int nPeerType, int nConnectID = 0);
	virtual ~C104Impl();

public:
	void Initialise(int nParamID);
public:
	bool Open();//����104
	bool Close();//�ر�104
public:
	bool OnReceive(char* pBuff, int nLen);         //������Ϣ����
	bool OnTimer(int nTimerID);                    //��ʱ������
	bool OnCommand(NBM_TELECTRL* pTelectrl);       //������Ϣ����

//104��Լʵ����������(ʵ��socket�����շ�����ʱ���Ȼ����ӿ�)
private:
	IHost* m_pHost;

	//104��Լʵ���Ӷ���
public:
	COnTimer*    GetTimer()  { return m_pTimer; }
	CSender*     GetSender() { return m_pSender; }
	CRecver*     GetRecver() { return m_pRecver; }
	CParam104*   GetParam()  { return &m_param104; }
	NBM_TELECTRL* GetCurCommand() { return m_pCurTelCtrl; }  //��ǰң��
private:
	COnTimer*   m_pTimer;       //��ʱ���߼�
	CSender*    m_pSender;      //�����߼�
	CRecver*    m_pRecver;      //�����߼�
	CParam104   m_param104;     //��������
	NBM_TELECTRL* m_pCurTelCtrl;//��ǰң������
//��Ϣ��¼�ӿ�
public:
	bool LogI(int nType, char* pBuff, int nLength);
	bool LogU(int nType, char* pBuff, int nLength);
	bool LogS(int nType, char* pBuff, int nLength);
	bool LogN(int nType, int nFrameType, string strTag);

	//////////////////////////////////////////////////////////////////////////

public:
	bool OnTimerSend(int nFrameType, int nUtype);
	bool OnTimerRecv(int nFrameType);

	bool OnKwSend(int nFrameType);
	bool OnKwRecv(int nFrameType, int nRecvNum);

	bool OnNumRecv(int nFrameType, int nSendNum);
	bool OnNumSend(int nFrameType);

public:
	bool OnSendFrame(int nFrameType, int nUtype = -1);
	bool OnRecvFrame(int nFrameType, int nSendNum, int nRecvNum);
public:
	enum { MAX_SERIAL_NUMBER = 32767, };//���к�15λ�����ֵ
	//enum { MAX_SERIAL_NUMBER = 30,};//���к�15λ�����ֵ
	unsigned short  m_nRecvNum;//���˽��յ����ٸ�I֡����ʼΪ0�����յ�1����1
	unsigned short  m_nSendNum;//���˷����˶��ٸ�I֡����ʼΪ0��������1����1
//k,wֵ�߼�
public:
	int m_k; //�����˵�I֡�У�����m_k��û���յ��Զ˵�ȷ��
	int m_w;//���յ���I֡�У�����m_w��û�и��Զ˷���ȷ��
//////////////////////////////////////////////////////////////////////////

//�Ƿ�����(����������ȷ����)
public:
	bool IsRunning() { return m_bIsRunning; }
	bool m_bIsRunning;

	//���Ӻ�
public:
	int GetConnectID() { return m_nConnectID; }
private:
	int m_nConnectID;

	//������(Master or Control)
public:
	int GetPeerType() { return m_nPeerType; }
private:
	int m_nPeerType;


	//������
public:
	void SetErrorCode(int nErrorCode) { m_nErrorCode = nErrorCode; }
	int  GetErrorCode() { return m_nErrorCode; }
private:
	int m_nErrorCode;

private:
	string FormatBuffer(char* pBuff, int nMsgLen);
};


#endif//_104_IMPL_H
