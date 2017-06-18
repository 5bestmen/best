#ifndef EXTENDSERVICECONFIGDIALOG_H
#define EXTENDSERVICECONFIGDIALOG_H

#include <QDialog>
#include "ui_extendserviceconfigdialog.h"

class CExtendServiceModuleMgr;

class CExtendServiceConfigDialog : public QDialog
{
	Q_OBJECT

public:
	CExtendServiceConfigDialog(QWidget *parent = 0);
	~CExtendServiceConfigDialog();

	void Init(CExtendServiceModuleMgr *pMgr);

private slots:
	void CurrentIndexChanged(int index);
	void OnOk();
	void OnCancel();
private:
	Ui::CExtendServiceConfigDialog ui;
	CExtendServiceModuleMgr *m_pMgr;
};

#endif // EXTENDSERVICECONFIGDIALOG_H
