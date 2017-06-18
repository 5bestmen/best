
/*! @file ctrlparadialog.h
<PRE>
********************************************************************************
模块名       :
文件名       :  ctrlparadialog.h
文件实现功能 :  控件配置对话框
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   控件配置对话框
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/
#ifndef CTRLPARADIALOG_H
#define CTRLPARADIALOG_H

#include <QWidget>
#include <QDialog>
#include "ui_ctrlparadialog.h"

class CCtrlData;
class CCtrlParaDialog : public QDialog
{
	Q_OBJECT

public:
	CCtrlParaDialog(CCtrlData* pData,QWidget *parent = 0);
	~CCtrlParaDialog();
public:
	CCtrlData* GetCtrlData();
private:
	void InitUI();

private Q_SLOTS:
	void slot_OnOk();
	void slot_OnCancel();
	void slot_OnCtrlTypeSel();           //! <<控件类型
	void slot_OnFileSrcSel();            //！<<数值选取类型
	void slot_OnReportSel();
	bool eventFilter(QObject *, QEvent *);
	void slot_OnDealData(const QString& );
private:
	CCtrlData* m_CtrlData;
	Ui::CCtrlParaDialog ui;
	QLineEdit* m_pCurrentLineEdit;
};




#endif // CTRLPARADIALOG_H
