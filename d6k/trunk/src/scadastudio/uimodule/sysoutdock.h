#ifndef SYSOUTDOCK_H
#define SYSOUTDOCK_H

#include <QDockWidget>

class CSystemOutputView;
class CSystemOutputModel;
class QTcpServer;
class QTabWidget;
struct MSG_LOG;

class CSysOutDock : public QDockWidget
{
	Q_OBJECT

public:
	CSysOutDock(QWidget *parent);
	~CSysOutDock();

	void AddItem(MSG_LOG &log, QColor &color = QColor());

	void resizeEvent(QResizeEvent *event);

public slots:
	void newConnection();

	void readyRead();

public slots:
	void ModelRowsInserted(const QModelIndex & parent, int start, int end);
	//ÓÒ»÷²Ëµ¥
	void Slot_ContextMenuRequest(const QPoint &cPoint);
	//copy
	void Slot_CpyAction();
	//clear
	void Slot_ClearItemAction();
	//run
	void Slot_StartRunAction();
	//stop
	void Slot_StopAction();
	//Export
	void Slot_ExportAction();
protected:
	void closeEvent(QCloseEvent *event)
	{
		this->setHidden(true);
		
		QDockWidget::closeEvent(event);
	}

	void SetShow(bool bFlag)
	{
		this->SetShow(bFlag);
	}
private:
	void CreateCommandView();

	void CreateLogCommandView();

private:
	QTabWidget	*m_pMessageTab;

	CSystemOutputView *m_commandView;
	CSystemOutputModel *m_commandModel;

	CSystemOutputView *m_pLogView;
	CSystemOutputModel *m_pLogModel;

	QTcpServer *m_pTcpServer;
	//ÊÇ·ñ¹ö¶¯
	bool m_bSrcollFlag;
};

#endif // SYSOUTDOCK_H
