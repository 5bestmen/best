#ifndef OUTPUTMESSAGERCLIENT_H
#define OUTPUTMESSAGERCLIENT_H

#include <QObject>
class QTcpSocket;

class COutputMessagerClient : public QObject
{
	Q_OBJECT

public:
	COutputMessagerClient(QObject *parent =NULL);
	~COutputMessagerClient();

	bool checkConnect();

	bool connectHost(QString host="127.0.0.1", int port = 19999);

	void sendData(char *data, int length);

private:
	QTcpSocket *m_tcpSocket;
};

#endif // OUTPUTMESSAGERCLIENT_H
