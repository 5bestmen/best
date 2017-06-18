#ifndef OUTPUTMESSAGERSERVER_H
#define OUTPUTMESSAGERSERVER_H

#include <QObject>
#include <QTcpServer>

class COutputMessagerServer : public QObject
{
	Q_OBJECT

public:
	COutputMessagerServer(QObject *parent);
	~COutputMessagerServer();

	QTcpServer *m_tcpServer;
private:
	
private slots:
	void newConnection();

	void readyRead();
};

#endif // OUTPUTMESSAGERSERVER_H
