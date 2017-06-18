
/*! @file ctrlparadialog.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  ctrlparadialog.h
�ļ�ʵ�ֹ��� :  �ؼ����öԻ���
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ؼ����öԻ���
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
	void slot_OnCtrlTypeSel();           //! <<�ؼ�����
	void slot_OnFileSrcSel();            //��<<��ֵѡȡ����
	void slot_OnReportSel();
	bool eventFilter(QObject *, QEvent *);
	void slot_OnDealData(const QString& );
private:
	CCtrlData* m_CtrlData;
	Ui::CCtrlParaDialog ui;
	QLineEdit* m_pCurrentLineEdit;
};




#endif // CTRLPARADIALOG_H
