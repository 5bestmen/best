
/*! @file modreporttype.h
<PRE>
********************************************************************************
模块名       :
文件名       :  modreporttype.h
文件实现功能 :  修改报表属性
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   修改报表属性
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef MODREPORTTYPE_H
#define MODREPORTTYPE_H

#include <QDialog>
#include "ui_modreporttype.h"

class QModReportType : public QDialog
{
    Q_OBJECT

public:
    int  getResult();
    QModReportType(QWidget *parent = 0);
    ~QModReportType();

private:
    Ui::QModReportType ui;
    int selIndex;

private slots:
    void slot_on_ok();
};

#endif // MODREPORTTYPE_H
