#ifndef ATTRWGT_H
#define ATTRWGT_H

#include <QDialog>
#include "ui_attrwgt.h"

//任务属性 （mast  fast aux 右击属性）
class CAttrWgt : public QDialog
{
    Q_OBJECT

public:
    enum 
    {
        PERIOD_ITEM = 1,    //peroid
        CYCLE_ITEM  =2,      //cycle

        MAST_ATTR = 0,      //mast item
        FAST_ATTR = 1,      //fast  
        AUX_ATTR  = 2,      //AUX
    };

    CAttrWgt(QWidget *parent = 0);
    ~CAttrWgt();
    void InitMast();
    void InitFast();
    void InitAux();
    void SetComboName(const QString &strName);

    int GetRepeatType() const;
    int GetMsec() const;
    //
    const QString& GetComment();

    //1 为 period   2为 cycle
    void SetCycleType(int nType);
    //
    void SetComment(const QString &strComment);
    //时间
    void SetMsec(int nMsec);
signals:
    void Signal_Conform();
public slots:
    void Slot_Conform();

private:
    Ui::CAttrWgt ui;

    //循环类型   1:period   2:cycle
    int m_nRepertType;
    //间隔
    int m_nMsec;
    //注释
    QString m_strComment;

};

#endif // ATTRWGT_H
