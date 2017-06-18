#ifndef SCADANONLINEARSCALESETDIALOG_H
#define SCADANONLINEARSCALESETDIALOG_H

#include <QDialog>
#include "ui_scadanonlinearscalesetdialog.h"

namespace Config
{
	class CScadaTransform;
}

class CScadaNonlinearScaleSetDialog : public QDialog
{
	Q_OBJECT

public:
	CScadaNonlinearScaleSetDialog(QWidget *parent = 0);
	~CScadaNonlinearScaleSetDialog();

	void Init(Config::CScadaTransform *pTransform);

private slots:
	void OnOk();

	void OnCancel();

	void OnAdd();

	void OnDele();
private:
	Ui::CScadaNonlinearScaleSetDialog ui;
	Config::CScadaTransform *m_pTransform;
	int m_nType;
};

#endif // SCADANONLINEARSCALESETDIALOG_H
