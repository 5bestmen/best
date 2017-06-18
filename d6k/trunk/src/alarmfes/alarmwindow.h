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
	//����
	QTableView*      m_pMainView;
	//ģ��
	CAlarmInfoModel* m_pDataModel;
	//��ʱˢ��
	QTimer* m_pTimer;
	//��ʾlabel
	QLabel * m_pLabel;
	//�����߳�
	CRecvInfoTask* m_pRecvThread;
	//�����߳�
	CAnalyseTask* m_pHandleThread;
	//��Ϣ����
	std::shared_ptr<CAlarmInfoQueue> m_pAlarmDataQueue;	
	//չʾ����
	std::shared_ptr<CAlarmInfoQueue> m_pAlarmViewQueue;

	QTimer* m_pRandTimer;
};

#endif // ALARMWINDOW_H
