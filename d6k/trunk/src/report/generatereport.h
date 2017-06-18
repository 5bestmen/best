/*! @file generatereport.h
<PRE>
********************************************************************************
模块名       :
文件名       :  generatereport.h
文件实现功能 :  获取报表产生的时间列
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   获取报表产生的时间列
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/


#ifndef GENERATEREPORT_H
#define GENERATEREPORT_H

#include <QDialog>
#include "ui_generatereport.h"

class QGenerateReport : public QDialog
{
    Q_OBJECT

public:
	QDate& getResult();
    QGenerateReport(QWidget *parent = 0);
    ~QGenerateReport();
protected:
    void InitDialog();

private:
    Ui::QGenerateReport ui;
    QDate m_Time;

private slots:
    void slot_on_produceReport();
  
};

#endif // GENERATEREPORT_H
