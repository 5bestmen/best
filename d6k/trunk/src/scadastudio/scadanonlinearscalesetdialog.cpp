#include "scadanonlinearscalesetdialog.h"
#include "scadavirable_data.h"
#include "delegate.h"

#include <QMessageBox>

CScadaNonlinearScaleSetDialog::CScadaNonlinearScaleSetDialog(QWidget *parent)
	: QDialog(parent), m_pTransform(nullptr)
{
	ui.setupUi(this);

	m_nType = ScaleType::NONLINEAR;

	//3 小数点位数
	auto pDelegate = new CLineEditDelegate(this, DOUBLETYPE, THREE_COUNT);
	ui.tableWidget->setItemDelegateForColumn(0, pDelegate);
	ui.tableWidget->setItemDelegateForColumn(1, pDelegate);

	connect(ui.tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(ItemChanged(QTableWidgetItem *)));
}

CScadaNonlinearScaleSetDialog::~CScadaNonlinearScaleSetDialog()
{

}

void CScadaNonlinearScaleSetDialog::Init(Config::CScadaTransform *pTransform)
{
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	m_pTransform = pTransform;

	QStringList header;
	header << "Month" << "Description";

	ui.tableWidget->setHorizontalHeaderLabels(header);

	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行

	Config::CScadaNonlinearTransform *pTmp = pTransform->GetNonLinear();

	for (size_t i = 0; i < pTmp->m_arrPNonliear.size(); i++)
	{
		Config::SCADA_NONLINEAR_SCALE *pScale = pTmp->m_arrPNonliear[i];

		int nRows = ui.tableWidget->rowCount();

		ui.tableWidget->insertRow(nRows);

		QString strRaw = QString("%1").arg(pScale->RawValue);
		QString strScale = QString("%1").arg(pScale->ScaleValue);

		ui.tableWidget->setItem(nRows, 0, new QTableWidgetItem(strRaw));
		ui.tableWidget->setItem(nRows, 1, new QTableWidgetItem(strScale));
	}

	connect(ui.m_pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.m_pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.m_pushButtonAdd, SIGNAL(clicked()), this, SLOT(OnAdd()));
	connect(ui.m_pushButtonDelete, SIGNAL(clicked()), this, SLOT(OnDele()));
}

void CScadaNonlinearScaleSetDialog::OnOk()
{
	int nRows = ui.tableWidget->rowCount();

	auto pNonlinear = m_pTransform->GetNonLinear();
	Q_ASSERT(pNonlinear);
	if (!pNonlinear)
	{
		return;
	}

	pNonlinear->m_arrPNonliear.clear();

	m_nType = ScaleType::NONLINEAR;

	m_pTransform->m_nType = m_nType;

	bool bFlag = false;
	for (int i = 0; i < nRows; i++)
	{
		Config::SCADA_NONLINEAR_SCALE *pScaleTmp = new Config::SCADA_NONLINEAR_SCALE;

		auto strTmp = ui.tableWidget->item(i, 0)->data(Qt::EditRole).toString();

		strTmp.toDouble(&bFlag);
		if (!bFlag)
		{
			QMessageBox box;
			box.setText(QString("row %1 raw %2 is not numeric!!!").arg(i + 1).arg(strTmp));

			return;
		}

		strTmp = ui.tableWidget->item(i, 1)->data(Qt::EditRole).toString();
		strTmp.toDouble(&bFlag);
		if (!bFlag)
		{
			QMessageBox box;
			box.setText(QString("row %1 raw %2 is not numeric!!!").arg(i + 1).arg(strTmp));

			return;
		}

		pScaleTmp->RawValue = ui.tableWidget->item(i, 0)->data(Qt::EditRole).toDouble();
		pScaleTmp->ScaleValue = ui.tableWidget->item(i, 1)->data(Qt::EditRole).toDouble();

		pNonlinear->m_arrPNonliear.push_back(pScaleTmp);
	}

	accept();

	deleteLater();
}

void CScadaNonlinearScaleSetDialog::OnCancel()
{
	reject();
	deleteLater();
}

void CScadaNonlinearScaleSetDialog::OnAdd()
{
	QString strTmp = ui.m_lineEditRawNon->text();
	bool bFlag = false;
	double dbTmp = strTmp.toDouble(&bFlag);
	if (!bFlag || strTmp.isEmpty())
	{
		QMessageBox box;
		box.setText(QString(tr("raw value %1 is not numeric!!!")).arg(strTmp));
		box.exec();

		return;
	}

	strTmp = ui.m_lineEditNon->text();
	bFlag = false;
	dbTmp = strTmp.toDouble(&bFlag);
	if (!bFlag || strTmp.isEmpty())
	{
		QMessageBox box;
		box.setText(QString(tr("value %1 is not numeric!!!")).arg(strTmp));
		box.exec();

		return;
	}

	m_pTransform->GetNonLinear();

	int nRows = ui.tableWidget->rowCount();

	ui.tableWidget->insertRow(nRows);

	ui.tableWidget->setItem(nRows, 0, new QTableWidgetItem(ui.m_lineEditRawNon->text()));
	ui.tableWidget->setItem(nRows, 1, new QTableWidgetItem(ui.m_lineEditNon->text()));
}

void CScadaNonlinearScaleSetDialog::OnDele()
{
	int rowIndex = ui.tableWidget->currentRow();

	if (rowIndex != -1)
	{
		ui.tableWidget->removeRow(rowIndex);
	}
}
