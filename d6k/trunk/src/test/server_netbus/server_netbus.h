#ifndef SERVER_NETBUS_H
#define SERVER_NETBUS_H

#include <QtWidgets/QMainWindow>
#include "ui_server_netbus.h"

#include <QLibrary>
#include <QThread>

class CNBRecvThread :public QThread
{
	Q_OBJECT

public:
	CNBRecvThread();
	~CNBRecvThread();
public:
	void run();

private:
	QLibrary* m_pNetbus;
signals:
	void sigDataRecv(QString);
};

class server_netbus : public QMainWindow
{
	Q_OBJECT

public:
	server_netbus(QWidget *parent = 0);
	~server_netbus();

public:

	//connect(ui-, SIGNAL(pressed()), this, SLOT(on_pressed()));
	//connect(this, SIGNAL(released()), this, SLOT(on_released()));
	
	bool InitNetBus();

	bool FiniNetBus();

	bool StartNetbus();

	bool StopNetbus();
	
private slots:
	void SLOT_StartNetBus();
	void SLOT_StopNetBus();
	void SLOT_SendData();

	void SLOT_MsgSizeRadio();
	void SLOT_ServerRadio();
	void SLOT_FesRadio();
	void SLOT_ClientRadio();

	void SLOT_DataReceive(QString data);
private:
	QLibrary* m_pNetbus;

	bool m_bStartFlag;

	CNBRecvThread* m_pNBRecv;
private:
	Ui::server_netbusClass ui;
};

#endif // SERVER_NETBUS_H
