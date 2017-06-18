#ifndef REALTIMEANALOG_H
#define REALTIMEANALOG_H

#include <QWidget>
#include <QMap>
#include <QDialog>

#define  ZONEBEGINNUM  0x5001

class CCommThread;
class QTableView;
class QStandardItemModel;
class QStandardItem;
class CPointInfo;
class CRemotePonitTableAnalyse;
class CInfoConfigWgt;
class QLineEdit;
class QComboBox;
class QPushButton;

class CAnalogZoneWgt : public QDialog
{
    Q_OBJECT
public:
    CAnalogZoneWgt(QWidget *parent = 0);
    void InitWgt();

    float GetZoneData();
    int GetQpmData();


    ~CAnalogZoneWgt();
	//
signals:
    void Signal_Comform();
public slots :
    //点击确定
    void Slot_PushComform();

private:
    //短浮点
    QLineEdit *m_pLineEdZone;
    //QPM
    QComboBox *m_pComboQpm;
    //确定
    QPushButton *m_pPushComform;
};

class CRealTimeAnalog : public QWidget
{
	Q_OBJECT

public:
    enum 
    {
        NORMAANALOG,       //归一化遥信
        SCALEDANALOG,      //标度化遥信
        FOLATANALOG        //浮点遥信
    };
    CRealTimeAnalog(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CRemotePonitTableAnalyse *pChooseWgt, CInfoConfigWgt *pConfgWgt);
	~CRealTimeAnalog();
	void AnalyseRealAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty, const QString &strType,int nAnalogFlag);
	void InitWidget();

	void InitData();

public slots:
    //归一化
    void Slot_AnalyseNormalAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty);
    //浮点化
	void Slot_AnalyseShortFloatAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty);
    //标度化
    void Slot_AnalyseScaledAnalogData(int iDeviceID, int nPointID, float nValue, int iQuilty);
    //右击菜单设置死区
    void Slot_ContextMenuRequest(const QPoint &pPoint);
    //遥测死区act
    void Slot_DeadZoneAct();
    //
    void Slot_ZoomArea();
    //死区反馈
    void Slot_ZoneFeedback(int nAddr, float fValue, int qpm, int nCot);

private:
	//通信插件
	CCommThread *m_pCommunicate;
	//id item
	QMap<int, QStandardItem*> m_mapIdItemObj;
	//tableview
	QTableView *m_pTableView;
	//model
	QStandardItemModel *m_pTableModel;
	//当前个数
	int m_iCurrentPointsNum;
	//点表数据
	CPointInfo *m_pPointInfo;
    //基础表
    CRemotePonitTableAnalyse *m_pChooseWgt;
    CInfoConfigWgt *m_pConfgWgt;
    //
    CAnalogZoneWgt *m_pZoneWgt;
};

#endif // REALTIMEANALOG_H
