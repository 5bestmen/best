/*! @file createtimecolumn.h
<PRE>
********************************************************************************
模块名       :
文件名       :  createtimecolumn.h
文件实现功能 :  创建报表时间列
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  创建报表时间列
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/


#ifndef CREATETIMECOLUMN_H
#define CREATETIMECOLUMN_H

#include <QDialog>
#include "ui_createtimecolumn.h"

typedef
    struct  
    {
        qint32 dataType;
        qint32 stepIncrease;
    }
TimeIncrease;


class QCreateTimeColumn : public QDialog
{
    Q_OBJECT

public:
	TimeIncrease& getResult();

    QCreateTimeColumn(QWidget *parent = 0);
    ~QCreateTimeColumn();
protected:
    void InitDay();
    void InitMonth();
    void InitYear();
    void InitSpecial();

private:
    Ui::QCreateTimeColumn ui;
    TimeIncrease NTIME;
private slots:
    void slot_on_reportTypeChanged(int);
    void slot_on_produce();
    
};

#endif // CREATETIMECOLUMN_H
