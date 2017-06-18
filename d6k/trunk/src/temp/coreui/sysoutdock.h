#ifndef SYSOUTDOCK_H
#define SYSOUTDOCK_H

#include <QDockWidget>

class CSystemOutputView;
class CSystemOutputModel;
class QTcpServer;
class QTabWidget;

class CSysOutDock : public QDockWidget
{
	Q_OBJECT

public:
	CSysOutDock(QWidget *parent);
	~CSysOutDock();

public slots:
	void newConnection();

	void readyRead();

private:
	void  createCommandView();

	void createLogCommandView();

private:
	QTabWidget	*m_messageTab;

	CSystemOutputView *m_commandView;
	CSystemOutputModel *m_commandModel;

	CSystemOutputView *m_logView;
	CSystemOutputModel *m_logModel;

	QTabWidget *m_messsageTab;

	QTcpServer *m_tcpServer;
};

#endif // SYSOUTDOCK_H
