#include "handle_thread.h"

CHandleThread::CHandleThread(QString strIP, int nPort, int nDeviceID)
{
	m_strIP = strIP;
	m_nPort = nPort;
	m_nDeviceID = nDeviceID;
}

//初始化socketmgr、readxml对象
void CHandleThread::run()
{
	CSocketMgr *SocketMgr = new CSocketMgr;
	CReadXML *pReadXML = new CReadXML;
	pReadXML->ReadXML(CONFIG_NAME);
	SocketMgr->Init(m_strIP, m_nPort);

	CModbusTrans *pModbusTrans = new CModbusTrans(SocketMgr, pReadXML, m_nDeviceID);
	connect(pModbusTrans, SIGNAL(Signal_ShowDiValue(int, int, int, int, QString)), this, SIGNAL(Signal_ShowDiValue(int, int, int, int, QString)));
	connect(pModbusTrans, SIGNAL(Signal_ShowAiValue(int, int, int, int, QString)), this, SIGNAL(Signal_ShowAiValue(int, int, int, int, QString)));
	connect(pModbusTrans, SIGNAL(Signal_ShowKwhValue(int, int, int, int, QString)), this, SIGNAL(Signal_ShowKwhValue(int, int, int, int, QString)));

	connect(pModbusTrans, SIGNAL(Signal_ConnectSuccess(int, const QString &)), this, SIGNAL(Signal_ConnectSuccess(int, const QString &)));
	connect(pModbusTrans, SIGNAL(Signal_SocketError(int, const QString &, const QString &)), this, SIGNAL(Signal_SocketError(int, const QString &, const QString &)));

	connect(this, SIGNAL(Signal_CloseSocket(int)), SocketMgr->GetSocketClient(), SLOT(Slot_CloseSocket()));
	connect(this, SIGNAL(Signal_ReconSocket(int, QString, int)), SocketMgr->GetSocketClient(), SLOT(Slot_ReconSocket(int, QString, int)));

	connect(this, SIGNAL(Signal_TransBinary(int, int, int, int)), pModbusTrans, SLOT(Slot_TransBinary(int, int, int, int)));
	connect(this, SIGNAL(Signal_TransAnalog(int, int, int, int)), pModbusTrans, SLOT(Slot_TransAnalog(int, int, int, int)));

	exec();
}

CHandleThread::~CHandleThread()
{

}
//关闭socket槽函数
void CHandleThread::Slot_CloseSocket(int nDeviceID)
{
	if (nDeviceID == m_nDeviceID)
	{
		emit Signal_CloseSocket(nDeviceID);

	}
}

//手动重连socket槽函数
void CHandleThread::Slot_ReconSocket(int nDeviceID, QString strIP, int nPort)
{
	if (nDeviceID == m_nDeviceID)
	{
		emit Signal_ReconSocket(nDeviceID, strIP, nPort);

	}
}

//以遥控方式转发遥信点槽函数
void CHandleThread::Slot_TransBinary(int nStationID, int nDeviceID, int nDiID, int nValue)
{
	
	if (nDeviceID == m_nDeviceID)
	{
		emit Signal_TransBinary(nStationID, nDeviceID, nDiID, nValue);

	}
}

//以遥调方式转发遥测点槽函数
void CHandleThread::Slot_TransAnalog(int nStationID, int nDeviceID, int nDiID, int nValue)
{
	if (nDeviceID == m_nDeviceID)
	{
		emit Signal_TransAnalog(nStationID, nDeviceID, nDiID, nValue);

	}
}

