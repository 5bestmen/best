#include "scadalinearscalesetdialog.h"
#include "scadavirable_data.h"

#include <QMessageBox>

CScadaLinearScaleSetDialog::CScadaLinearScaleSetDialog(QWidget *parent)
	: QDialog(parent), m_pLinear(nullptr)
{
	ui.setupUi(this);

	connect(ui.pOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	//connect(ui.enableFactor, SIGNAL(clicked(bool)), this, SLOT(ToggledEnableFactor(bool)));
	//connect(ui.enableInverseFactor, SIGNAL(clicked(bool)), this, SLOT(ToggledEnableInverseFactor(bool)));
}

CScadaLinearScaleSetDialog::~CScadaLinearScaleSetDialog()
{

}

void CScadaLinearScaleSetDialog::OnOk()
{
	bool bValid = false;
	bool bRight = true;

	auto dbMinRaw = ui.minRaw->text().toDouble(&bValid);
	if (!bValid)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("min raw %1 is not numeric!!!").arg(ui.minRaw->text())));
		box.exec();

		bRight = false;
	}

	auto dbMaxRaw = ui.maxRaw->text().toDouble(&bValid);
	if (!bValid)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("max raw %1 is not numeric!!!").arg(ui.maxRaw->text())));
		box.exec();

		bRight = false;
	}

	auto dbMinValue = ui.minValue->text().toDouble(&bValid);
	if (!bValid)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("min value %1 is not numeric!!!").arg(ui.minValue->text())));
		box.exec();

		bRight = false;
	}

	auto dbMaxValue = ui.maxValue->text().toDouble(&bValid);
	if (!bValid)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("max value %1 is not numeric!!!").arg(ui.maxValue->text())));
		box.exec();

		bRight = false;
	}

	auto dbGain = ui.gain->text().toDouble(&bValid);
	if (!bValid)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("max value %1 is not numeric!!!").arg(ui.gain->text())));
		box.exec();

		bRight = false;
	}

	auto dbOffset = ui.offset->text().toDouble(&bValid);
	if (!bValid)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("max value %1 is not numeric!!!").arg(ui.offset->text())));
		box.exec();

		bRight = false;
	}

	if (ui.enableFactor->checkState() == ui.enableInverseFactor->checkState() &&
		ui.enableFactor->checkState() == Qt::CheckState::Checked)
	{
		QMessageBox box;
		box.setText(QString("%1").arg(tr("enableFactor and enableInverseFactor can not be true at the same time !!!")));
		box.exec();

		bRight = false;
	}

	if (!bRight)
	{
		return;
	}

	m_pLinear->m_dbMinRaw = dbMinRaw;
	m_pLinear->m_dbMaxRaw = dbMaxRaw;
	m_pLinear->m_dbMinScale = dbMinValue;
	m_pLinear->m_dbMaxScale = dbMaxValue;
	m_pLinear->m_dbGain = dbGain;
	m_pLinear->m_dbOffset = dbOffset;

	m_pLinear->m_bEnableInverse = ui.enableInverse->checkState();
	m_pLinear->m_bEnableFactor = ui.enableFactor->checkState();
	m_pLinear->m_bEnableInverseFactor = ui.enableInverseFactor->checkState();

	accept();
}

void CScadaLinearScaleSetDialog::OnCancel()
{
	reject();
}

void CScadaLinearScaleSetDialog::Init(Config::CScadaTransform *pTransform)
{
	auto pLinear = pTransform->GetLinear();
	Q_ASSERT(pLinear);
	if (!pLinear)
	{
		return;
	}
	         
	m_pLinear = pLinear;

	//3 小数精度
	ui.minRaw->setText(QString::number(pLinear->m_dbMinRaw, 'f', 3));
	ui.maxRaw->setText(QString::number(pLinear->m_dbMaxRaw, 'f', 3));
	ui.minValue->setText(QString::number(pLinear->m_dbMinScale, 'f', 3));
	ui.maxValue->setText(QString::number(pLinear->m_dbMaxScale, 'f', 3));
	//是否反比线性转换
	ui.enableInverse->setChecked(pLinear->m_bEnableInverse);
	//启用计算系数
	ui.enableFactor->setChecked(pLinear->m_bEnableFactor);
	//启用反比系数
	ui.enableInverseFactor->setChecked(pLinear->m_bEnableInverseFactor);
	ui.gain->setText(QString::number(pLinear->m_dbGain, 'f', 3));
	ui.offset->setText(QString::number(pLinear->m_dbOffset, 'f', 3));
}

void CScadaLinearScaleSetDialog::ToggledEnableFactor(bool checked)
{
	Q_ASSERT(m_pLinear);
	if (!m_pLinear)
	{
		return;
	}
	
	m_pLinear->m_bEnableFactor = checked;
	ui.enableInverseFactor->setChecked(!checked);

	m_pLinear->m_bEnableInverseFactor = !checked;
}

void CScadaLinearScaleSetDialog::ToggledEnableInverseFactor(bool checked)
{
	Q_ASSERT(m_pLinear);
	if (!m_pLinear)
	{
		return;
	}
	
	m_pLinear->m_bEnableInverseFactor = checked;
	ui.enableFactor->setChecked(!checked);

	m_pLinear->m_bEnableFactor = !checked;
}