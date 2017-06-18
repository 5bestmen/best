#ifndef REALTIMESOE_H
#define REALTIMESOE_H

#include <QWidget>
#include <QMap>
#include <QDateTime>

class CCommThread;
class QTableView;
class QStandardItemModel;
class QStandardItem;
class CPointInfo;
class CInfoConfigWgt;

class CRealTimeSOE : public QWidget
{
    Q_OBJECT

public:
    CRealTimeSOE(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt);
    ~CRealTimeSOE();

    void AnalyseRealSeoBinaryData(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
    void InitWidget();
    public slots:
    void Slot_AnalyseSeoBinaryData(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
    //��ղ���
    void Slot_ContextMenuRequest(const QPoint &);
    //���act
    void Slot_clearAction();

private:
    //ͨ�Ų��
    CCommThread *m_pCommunicate;
    //tableview
    QTableView *m_pTableView;
    //model
    QStandardItemModel *m_pTableModel;
    //
    CPointInfo *m_pPointInfo;
	//
	CInfoConfigWgt *m_pConfgWgt;


};

#endif // REALTIMESOE_H
