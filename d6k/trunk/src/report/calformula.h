/*! @file calformula.h
<PRE>
********************************************************************************
模块名       :
文件名       :  calformula.h
文件实现功能 :  基本运算公式对话框
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本运算公式对话框
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef CALFORMULA_H
#define CALFORMULA_H

#include <QDialog>
#include <QStringList>
#include "ui_calformula.h"

enum
{
    PLUS,      // !< 0
    MINUS,     // !< 1
    MULTIPY,   // !< 2
    DIVIDE,    // !< 3
    MAX,       // !< 4
    MIN,       // !< 5
    SUM,       // !< 6
    AVG        // !< 7
};

//typedef QString& XSTRING;

class QCalFormula : public QDialog
{
    Q_OBJECT

public:
	QString&  GetResult();
    QCalFormula(QWidget *parent = 0);
    ~QCalFormula();

private slots:
//interface
    void slot_on_ok();
    void slot_on_cancel();
    void slot_on_listChange(int);
//exchange
    void slot_on_dealData(const QString&);
//innerFunction
    bool eventFilter(QObject *,QEvent *); 

protected:
    void InitData();
    void DealPlus();
    void DealMinus();
    void DealMultipy();
    void DealDivide();
    void DealMax();
    void DealMin();
    void DealSum();
    void DealAvg();

private:
    Ui::QCalFormula ui;
    QLineEdit *m_CurrentEdit;
    QList<QLineEdit *>m_Edit;
    QStringList m_DataList;
    QString m_szResult;
 
};

#endif // CALFORMULA_H
