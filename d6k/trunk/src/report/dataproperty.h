#ifndef DATAPROPERTY_H
#define DATAPROPERTY_H

#include <QDialog>
#include "ui_dataproperty.h"


typedef struct
{
//时间选取
        QString m_StartTime;
        QString m_EndTime;
//时间序列   
         QString timeIncrease;
         qint32 m_nIndex;
//设备属
     qint32 nAIType;
}AIType;

typedef AIType& MAIData;

class QDataTypeAI : public QDialog
{
    Q_OBJECT

public:
    QDataTypeAI(QWidget *parent = 0);    
    MAIData getDataType();
    void setTimeIncreaseTrue(){ui.TimeComboBox->setEnabled(true);};
    void setTimeButtonTrue(){ui.setPushButton->setEnabled(true);};

    void InitMonth();
    void InitYear();
    ~QDataTypeAI();

protected:
    void InitDialog();    
    void setMostFalse();//最值设定
    void setMostTrue();
    void setOverFalse();//越线值设定
    void setOverTrue();
    void setTimeTrue();//时间值设定
    void setTimeFalse();

    


private:
    Ui::QDataProperty ui;
    
    AIType m_AIData;
    int AIDataFlag;
    

private slots:
    void slot_on_settime();
    void slot_on_setDataType();
    void slot_on_timeChanged(int);
    void slot_on_setMostValue();
    void slot_on_setOverVal();
    void slot_on_ok();
    void slot_on_cancel();
};

#endif // DATAPROPERTY_H
