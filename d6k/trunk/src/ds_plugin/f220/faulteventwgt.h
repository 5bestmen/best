#ifndef FAULTEVENTWGT_H
#define FAULTEVENTWGT_H

#include <QWidget>
#include "ui_faulteventwgt.h"

class CCommThread;
class CPointInfo;
class CInfoConfigWgt;

class CFaultEventWgt : public QWidget
{
    Q_OBJECT

public:
    CFaultEventWgt(CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt,QWidget *parent = 0);
    ~CFaultEventWgt();
public slots:
    void Slot_MalFuction(int nBinaryType, int nBinaryAddr, int nBinaryValue, QString strTime, int nAnalogType, QMap<int, int>);

private:
    Ui::CFaultEventWgt ui;
    //
    CCommThread *m_pCommunicate;
};

#endif // FAULTEVENTWGT_H
