
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
	bool Open();//启动104
	bool Close();//关闭104
public:
	bool OnReceive(char* pBuff, int nLen);         //接收消息处理
	bool OnTimer(int nTimerID);                    //定时器处理
	bool OnCommand(NBM_TELECTRL* pTelectrl);       //发送消息处理

//104规约实现宿主对象(实现socket数据收发，定时器等基本接口)
private:
	IHost* m_pHost;

	//104规约实现子对象
public:
	COnTimer*    GetTimer()  { return m_pTimer; }
	CSender*     GetSender() { return m_pSender; }
	CRecver*     GetRecver() { return m_pRecver; }
	CParam104*   GetParam()  { return &m_param104; }
	NBM_TELECTRL* GetCurCommand() { return m_pCurTelCtrl; }  //当前遥控
private:
	COnTimer*   m_pTimer;       //定时器逻辑
	CSender*    m_pSender;      //接收逻辑
	CRecver*    m_pRecver;      //发送逻辑
	CParam104   m_param104;     //参数配置
	NBM_TELECTRL* m_pCurTelCtrl;//当前遥控命令
//消息记录接口
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
	enum { MAX_SERIAL_NUMBER = 32767, };//序列号15位，最大值
	//enum { MAX_SERIAL_NUMBER = 30,};//序列号15位，最大值
	unsigned short  m_nRecvNum;//本端接收到多少个I帧，初始为0，接收到1个加1
	unsigned short  m_nSendNum;//本端发送了多少个I帧，初始为0，发送了1个加1
//k,w值逻辑
public:
	int m_k; //发送了的I帧中，还有m_k个没有收到对端的确认
	int m_w;//接收到的I帧中，还有m_w个没有给对端发送确认
//////////////////////////////////////////////////////////////////////////

//是否运行(启动过程正确结束)
public:
	bool IsRunning() { return m_bIsRunning; }
	bool m_bIsRunning;

	//连接号
public:
	int GetConnectID() { return m_nConnectID; }
private:
	int m_nConnectID;

	//端类型(Master or Control)
public:
	int GetPeerType() { return m_nPeerType; }
private:
	int m_nPeerType;


	//错误码
public:
	void SetErrorCode(int nErrorCode) { m_nErrorCode = nErrorCode; }
	int  GetErrorCode() { return m_nErrorCode; }
private:
	int m_nErrorCode;

private:
	string FormatBuffer(char* pBuff, int nMsgLen);
};


#endif//_104_IMPL_H
