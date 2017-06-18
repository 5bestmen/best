
/*! @file insertformula.h
<PRE>
********************************************************************************
模块名       :
文件名       :  insertformula.h
文件实现功能 :  插入计算公式
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   插入计算公式
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef INSERTFORMULA_H
#define INSERTFORMULA_H

#include <QDialog>
#include "ui_insertformula.h"


class QInsertFormula : public QDialog
{
	Q_OBJECT

public:

	QString&  GetRsult();
	void SetPlusTrue();
	void SetOtherTrue();

	QInsertFormula(QWidget *parent = 0);
	~QInsertFormula();
protected:
	void InitData();

signals:
	void sig_getresult(const QString&);

private slots:
	void slot_on_ok();

	void slot_on_cancel();

	void slot_on_dealData(const QString&);

	bool eventFilter(QObject *, QEvent *);
private:
	QLineEdit *m_CurrentEdit;
	QList<QLineEdit *>m_Edit;
	Ui::QInsertFormula ui;
	QStringList m_DataList;
	QString m_szResult;
};

#endif // INSERTFORMULA_H
