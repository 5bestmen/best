#ifndef SCADALINEARSCALESETDIALOG_H
#define SCADALINEARSCALESETDIALOG_H

#include <QDialog>
#include "ui_scadalinearscalesetdialog.h"

namespace Config
{
	class CScadaTransform;
	class CScadaLinearTransform;
}

class CScadaLinearScaleSetDialog : public QDialog
{
	Q_OBJECT

public:
	CScadaLinearScaleSetDialog(QWidget *parent = 0);
	~CScadaLinearScaleSetDialog();

	void Init(Config::CScadaTransform *pTransform);

private slots:
	void OnOk();
	void OnCancel();
	void ToggledEnableFactor(bool checked);
	void ToggledEnableInverseFactor(bool checked);
private:
	Ui::CScadaLinearScaleSetDialog ui;
	Config::CScadaLinearTransform *m_pLinear;
};

#endif // SCADALINEARSCALESETDIALOG_H
