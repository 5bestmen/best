#ifndef HANDLE_THREAD_H
#define HANDLE_THREAD_H

#include <QThread>

#include "socketmgr.h"
#include "modbustrans.h"

class CHandleThread : public QThread
{
Q_OBJECT
public:
	CHandleThread(QString strIP, int nPort, int nDeviceID);
	~CHandleThread();

	void stop();
signals:
	void Signal_ShowDiValue(int StationID, int DeviceID, int DiID, int realData, QString DiName);
	void Signal_ShowAiValue(int StationID, int DeviceID, int AiID, int realData, QString AiName);
	void Signal_ShowKwhValue(int StationID, int DeviceID, int KwhID, int realData, QString KwhName);
	//连接成功
	void Signal_ConnectSuccess(int nDevice, const QString &strDevName);
	//socket错误
	void Signal_SocketError(int nDevice, const QString &strDevName, const QString &strError);

	void Signal_CloseSocket(int nDeviceID);
	void Signal_ReconSocket(int nDeviceID, QString strIP, int nPort);

	void Signal_TransBinary(int nStationID, int nDeviceID, int nDiID, int nValue);
	void Signal_TransAnalog(int nStationID, int nDeviceID, int nDiID, int nValue);
	
public slots:
    void Slot_CloseSocket(int nDeviceID);
	void Slot_ReconSocket(int nDeviceID, QString strIP, int nPort);

	void Slot_TransBinary(int nStationID, int nDeviceID, int nDiID, int nValue);
	void Slot_TransAnalog(int nStationID, int nDeviceID, int nDiID, int nValue);

protected:
	void run();

private:
	QString m_strIP;
	int m_nPort;
	int m_nDeviceID;
};

#endif