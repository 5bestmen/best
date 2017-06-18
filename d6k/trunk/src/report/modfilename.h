
/*! @file modfilename.h
<PRE>
********************************************************************************
模块名       :
文件名       :  modfilename.h
文件实现功能 :  修改报表名称
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   修改报表名称
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/


#ifndef MODFILENAME_H
#define MODFILENAME_H

#include <QDialog>
#include "ui_modfilename.h"

class QModFileName : public QDialog
{
    Q_OBJECT

public:
    QModFileName(QWidget *parent = 0);
    
    ~QModFileName();

	QString& GetMyFileName();

private:
    Ui::QModFileName ui;
    QString m_szFileName;

private slots:
    void slot_on_ok();
    void slot_on_cancel();
};

#endif // MODFILENAME_H
