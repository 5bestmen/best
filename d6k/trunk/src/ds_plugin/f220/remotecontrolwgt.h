#ifndef REMOTECONTROLWGT_H
#define REMOTECONTROLWGT_H

#include <QWidget>
#include "ui_remotecontrolwgt.h"

class QStandardItemModel;
class CCommThread;
class QMenu;
class QTimer;
class CInfoConfigWgt;
class CPointInfo;

class CRemoteControlWgt : public QWidget
{
	Q_OBJECT

public:
    CRemoteControlWgt(CCommThread *pCommunicate, QWidget *parent , CInfoConfigWgt *pConfgWgt);
	~CRemoteControlWgt();
	void InitWgt();
	//往表格中插入数据
	void InsertDataToTable(const QString &strContrTpe,const QString &strPointNum,const QString &operateType);
	//反馈信息插入表格
	void InsertDataFromFeedBack(int iControlType, int iPointNum, int iOperateType, const QString &strStatus);
    //遥控描述
    void UpdateRemotePointDesc();
    //
    void SetRemoteTableInfo(CPointInfo *pRemoteAnalyse);
private slots:
    //点击确定
    void Slot_ClickConform();
	//table右击菜单
	void Slot_ContextMenuRequest(const QPoint &cPointCurrent);
	//clear action
	void Slot_clearAction();
	//timeOut
	void Slot_WaitForResponseTimeOut();
    //
    void Slot_ComboboxItemChange(const QString &text);
signals:
	void Signal_ControlOperateConform(int iControlType, int ipointNum, int OperateType);

private:
	Ui::CRemoteControlWgt ui;
	//model
	QStandardItemModel *m_pTableModel;
	//
	CCommThread *m_pCommThread;
	//右击菜单
	QMenu *m_pMTable;
	//超时处理
	QTimer *m_pTimeOut;
    //
    CInfoConfigWgt *m_pConfgWgt;
    //远动点表
    CPointInfo *m_pRemoteAnalyse;
    // 遥控描述  点号
    QMap<QString, int> m_mapDescPoint;
};

#endif // REMOTECONTROLWGT_H
