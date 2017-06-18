/*! @file createreport.h
<PRE>
********************************************************************************
模块名       :
文件名       :  createreport.h
文件实现功能 :  创建新报表对话框
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  创建新报表对话框
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/


#ifndef CREATEREPORT_H
#define CREATEREPORT_H

#include <QDialog>
#include "ui_createreport.h"
#include "scadastudio/define.h"


typedef struct
            {
                QString tableName;
                qint32 tableType;
                qint16 tableRow;
                qint16 tableColumn;
}CreateTableInfo;


class QCreateReport : public QDialog
{
    Q_OBJECT
public:
	QCreateReport(QWidget *parent = 0);
	~QCreateReport();
	CreateTableInfo& GetTableInfo();
private:
	Ui::QCreateReport ui;
	CreateTableInfo m_TableInfo;
private slots:
        void slot_on_ok();
        void slot_on_cancel();
};

#endif // CREATEREPORT_H
