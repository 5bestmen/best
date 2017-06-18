#ifndef ALARMWINDOW_H
#define ALARMWINDOW_H

#include <QMainWindow>
#include <memory>
#include "alarmdataconfig.h"
#include "ui_alarmwindow.h"

class QTableView;
class CAlarmInfoModel;
class CStarDelegate;
class QTimer;
class QLabel;
class CRecvInfoTask;
class CAnalyseTask;

class CAlarmWindow : public QMainWindow
{
	Q_OBJECT

public:
	CAlarmWindow(QWidget *parent = 0,QString szProjName=NULL);
	~CAlarmWindow();

private Q_SLOTS:
	void slot_OnTimer();
	void slot_onTimerAdd();
	void slot_onTimerDel();
	void slot_OnRecvMsg(void* pMsg);
	void slot_OnFesStatus(const QString& str);
private:
	void InitView();
	void SetHeaderList();
	void SetGridData();
	void closeEvent(QCloseEvent *event);
private:
	Ui::CAlarmWindow ui;
private:
	//界面
	QTableView*      m_pMainView;
	//模型
	CAlarmInfoModel* m_pDataModel;
	//定时刷新
	QTimer* m_pTimer;
	//显示label
	QLabel * m_pLabel;
	//接收线程
	CRecvInfoTask* m_pRecvThread;
	//处理线程
	CAnalyseTask* m_pHandleThread;
	//消息队列
	std::shared_ptr<CAlarmInfoQueue> m_pAlarmDataQueue;	
	//展示队列
	std::shared_ptr<CAlarmInfoQueue> m_pAlarmViewQueue;

	QTimer* m_pRandTimer;
};

#endif // ALARMWINDOW_H
