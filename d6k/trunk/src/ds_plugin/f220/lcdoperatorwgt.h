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
    //��¼
    void WriteTableLog(const QString &strContext);
    //������յ�������
    void AnalyseLcdRecvInfo(const QByteArray & btData);
    //
    void AnalyseLEDRecvInfo(const QByteArray &BtData);
    //��ȡ���Ƽ�
    CLcdControlWgt *GetControl();
    //
    void SedLEDLight(int nLedID, int nOnOff);
    //

public slots:
    void Slot_ContextMenuRequest(const QPoint &tPoint);
    //�������¼
    void Slot_ClearAct();

private:
    CLcdDisplayWgt *m_pDiswgt;
    CLcdControlWgt *m_pControlWgt;
    //���ļ�¼
    QTableWidget *m_pTableInfo;
};

#endif // LCDOPERATORWGT_H
