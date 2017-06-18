#ifndef DATATYPEDI_H
#define DATATYPEDI_H

#include <QDialog>
#include "ui_datatypedi.h"

typedef struct{
    struct
    {
        QString m_StartTime;
        QString m_EndTime;
    } calTime;

    struct
    {
        int m_nIndex;
        QString indexStr;

    }increaseTime;

    qint32 m_DataFlag;
    
}DIDataType;


class QDataTypeDI : public QDialog
{
    Q_OBJECT

public:
    QDataTypeDI(QWidget *parent = 0);
    void SetStaticsFalse();
    void SetStaticsTrue();
	DIDataType& GetDIdataType();
    ~QDataTypeDI();
protected:
    void InitDialog();

private:
    Ui::QDataTypeDI ui;
    DIDataType m_DataType;

private slots:
    void slot_on_settime();
    void slot_on_setbasedatatype();
    void slot_on_setstayicsdata();
    void slot_on_ok();
    void slot_on_cancel();
};

#endif // DATATYPEDI_H
