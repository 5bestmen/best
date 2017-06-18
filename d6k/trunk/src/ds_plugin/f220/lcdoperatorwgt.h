#ifndef LCDOPERATORWGT_H
#define LCDOPERATORWGT_H

#include <QWidget>

class CLcdDisplayWgt;
class CLcdControlWgt;
class QTableWidget;

class CLcdoperatorWgt : public QWidget
{
    Q_OBJECT

public:
    CLcdoperatorWgt(QWidget *parent=0);
    ~CLcdoperatorWgt();
    //记录
    void WriteTableLog(const QString &strContext);
    //处理接收到的数据
    void AnalyseLcdRecvInfo(const QByteArray & btData);
    //
    void AnalyseLEDRecvInfo(const QByteArray &BtData);
    //获取控制键
    CLcdControlWgt *GetControl();
    //
    void SedLEDLight(int nLedID, int nOnOff);
    //

public slots:
    void Slot_ContextMenuRequest(const QPoint &tPoint);
    //清除表格记录
    void Slot_ClearAct();

private:
    CLcdDisplayWgt *m_pDiswgt;
    CLcdControlWgt *m_pControlWgt;
    //报文记录
    QTableWidget *m_pTableInfo;
};

#endif // LCDOPERATORWGT_H
