#ifndef _104API_H
#define _104API_H
#include "datatypes.h"
#include "104impl.h"
#include "104server.h"
#include <QObject>
#include <QThread>
class C104Impl;
class C104Channel;
class C104Master;

enum 
{
	Default_Protocol = 1,
};


class C104Master: public IHost, public C104Server
{
	enum
	{
		TIMER_ID_GETCMD = 2000, TIMER_INTERVAL_GETCMD = 1,          //!<ң�أ����٣���ʱ���������ʱ��
		TIMER_ID_GENERALCALL = 3000,                                //!<��ʱ����
		TIMER_ID_KWH_GENERALCALL = 4000,                            //!<��ʱ���������ٻ�
		TIMER_ID_SYNCTIME = 5000,                                   //!<��ʱ��ʱ
	};
public:
	C104Master(QObject*parent,int32u nOccNo,std::shared_ptr<C104Channel> pIoChannel);
	virtual  ~C104Master();
public:
	virtual bool OnOpen();                           
	virtual int  OnClose();                          
	virtual bool OnReceive(char* pBuff, int nLen);    
	virtual bool OnTimer(int nTimerID);             					
public:
	virtual bool SetTimer(int nTimerID, int nSecond);
	virtual void RemoveTimer(int nTimerID);
	virtual bool SendMessage(char* pBuff, int nLen, int nConnectID = 0);
	virtual int  GetGroupID();
	virtual int  GetChannelID();
	virtual int  GetDeviceID();
	virtual int  GetParamID();
	virtual int32u GetChannelOccNo();
	virtual void LogCallback104(int nSendOrRecv, int nMsgType, string strMsg);
	virtual std::shared_ptr<C104Channel> getKernel();
private:
	bool OnTimerGetCmd();              //!<��ʱ��ѯң�ض���
	bool OnTimerGeneralCall();         //!<��ʱ����
	bool OnTimerKwhGeneralCall();      //!<��ʱ���������ٻ�
	bool OnTimerSyncTime();            //!<��ʱ��ʱ
	void LogMsg(const char* pszText,int nLevel);
private:
	C104Impl* m_p104Impl;
private:
	int32u m_nChannelOccNo;

	std::shared_ptr<C104Channel> m_pChannel;

	std::shared_ptr<NBM_TELECTRL> m_pCurrentCommand;

	int m_nGroupID;
	int m_nChannelID;
	int m_nDeviceID;

	int m_nParamID;
	int m_nDeviceAddr;
	bool m_bClosed;

};

#endif // _104API_H
