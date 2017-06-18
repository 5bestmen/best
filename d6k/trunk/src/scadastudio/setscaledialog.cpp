#include "setscaledialog.h"
#include "scalemodel.h"
#include "config_data.h"

CSetScaleDialog::CSetScaleDialog(QWidget *parent, CScaleModel *pModel)
	: QDialog(parent), m_pModel(NULL)
{
	Q_ASSERT(pModel);

	if (!pModel)
	{
		return;
	}
	
	ui.setupUi(this);
	
	InitPara(pModel);
	
	m_pModel = pModel;
}

CSetScaleDialog::~CSetScaleDialog()
{

}

void CSetScaleDialog::InitPara(CScaleModel *pModel)
{
	Q_ASSERT(pModel);

	if (!pModel)
	{
		return;
	}

	QStringList header;
	header << "Month" << "Description";

	ui.tableWidget->setHorizontalHeaderLabels(header);
	
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行

	if (pModel->GetType() == CScaleModel::LINEAR)
	{
		ui.m_pLinearRadio->setChecked(true);
		ui.m_pNonLinearRadio->setChecked(false);

		Config::CLinearTransform *pTmp = NULL;

		pTmp = pModel->GetLinear();

		Q_ASSERT(pTmp);
		if (pTmp)
		{
			ui.m_lineEditMax->setText(QString::number(pTmp->m_dbMaxScale, 10, 6));
			
			ui.m_lineEditMin->setText(QString::number(pTmp->m_dbMinScale, 10, 6));

			ui.m_lineEditMaxRaw->setText(QString::number(pTmp->m_dbMaxRaw, 10, 6));
		
			auto strTmp = QString::number(pTmp->m_dbMinRaw, 10, 6);
			ui.m_lineEditMinRaw->setText(strTmp);
			
			strTmp = QString("%1").arg(pTmp->m_bInverseFactor);
			ui.m_checkBoxInverse->setChecked(pTmp->m_bInverseFactor);

			strTmp = QString("%1").arg(pTmp->m_dbGain);
			ui.m_lineEditGain->setText(strTmp);

			strTmp = QString("%1").arg(pTmp->m_dbOffset);
			ui.m_lineEditOffset->setText(strTmp);
		}
		else
		{
			return;
		}
	}
	else
	{
		ui.m_pLinearRadio->setChecked(false);
		ui.m_pNonLinearRadio->setChecked(true);

		Config::CNonlinearTransform *pTmp = pModel->GetNonLinear();
		Q_ASSERT(pTmp);
		if (pTmp)
		{
			auto &arr = pTmp->m_arrPNonliear;

			for each (auto var in arr)
			{
				int nRows = ui.tableWidget->rowCount();

				ui.tableWidget->insertRow(nRows);

				QString strRaw = QString("%1").arg(var->RawValue);
				QString strScale = QString("%1").arg(var->ScaleValue);

				ui.tableWidget->setItem(nRows, 0, new QTableWidgetItem(strRaw));
				ui.tableWidget->setItem(nRows, 1, new QTableWidgetItem(strScale));
			}
		}
	}

	connect(ui.m_pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.m_pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.m_pushButtonAdd, SIGNAL(clicked()), this, SLOT(OnAdd()));
	connect(ui.m_pushButtonDelete, SIGNAL(clicked()), this, SLOT(OnDele()));
	connect(ui.m_pushButtonModify, SIGNAL(clicked()), this, SLOT(OnModify()));
}

/*! \fn void CSetScaleDialog::OnOk()
********************************************************************************************************* 
** \brief CSetScaleDialog::OnOk 
** \details 确定
** \return void 
** \author gw
** \date 2016年4月19日 
** \note 
********************************************************************************************************/
void CSetScaleDialog::OnOk()
{
	if (ui.m_pLinearRadio->isChecked())
	{
		Config::CLinearTransform *pTmp = m_pModel->GetLinear();
		
		pTmp->m_dbMaxRaw = ui.m_lineEditMaxRaw->text().toDouble();

		//QString strTmp = QString::number(pTmp->m_dbMaxRaw, 'f', 6);
		////double test = strTmp.toDouble();

		pTmp->m_dbMinRaw = ui.m_lineEditMinRaw->text().toDouble();
		pTmp->m_dbMaxScale = ui.m_lineEditMax->text().toDouble();
		pTmp->m_dbMinScale = ui.m_lineEditMin->text().toDouble();
		pTmp->m_bInverseFactor = ui.m_checkBoxInverse->isChecked();
		pTmp->m_dbGain = ui.m_lineEditGain->text().toDouble();
		pTmp->m_dbOffset = ui.m_lineEditOffset->text().toDouble();

		//m_pModel->SetType(ScaleType::LINEAR);

		m_nType = ScaleType::LINEAR;
	}
	else
	{
		int nRows = ui.tableWidget->rowCount();
		
		Config::CNonlinearTransform *pTmp = m_pModel->GetNonLinear();

		pTmp->m_arrPNonliear.clear();

		m_nType = ScaleType::NONLINEAR;
		

		for (int i = 0; i < nRows; i++)
		{
			Config::NONLINEAR_SCALE *pScaleTmp = new Config::NONLINEAR_SCALE;
			
			pScaleTmp->RawValue = ui.tableWidget->item(i, 0)->data(Qt::EditRole).toDouble();
			pScaleTmp->ScaleValue = ui.tableWidget->item(i, 1)->data(Qt::EditRole).toDouble();

			pTmp->m_arrPNonliear.push_back(pScaleTmp);
		}
	}

	this->accept();
}

/*! \fn void CSetScaleDialog::OnCancel()
********************************************************************************************************* 
** \brief CSetScaleDialog::OnCancel 
** \details 
** \return void 
** \author gw
** \date 2016年4月19日 
** \note 
********************************************************************************************************/
void CSetScaleDialog::OnCancel()
{
	this->rejected();

	this->close();
}

/*! \fn void CSetScaleDialog::OnAdd()
********************************************************************************************************* 
** \brief CSetScaleDialog::OnAdd 
** \details 修改非线性转换
** \return void 
** \author gw
** \date 2016年4月19日 
** \note 
********************************************************************************************************/
void CSetScaleDialog::OnAdd()
{
	if (ui.m_pNonLinearRadio->isChecked())
	{
		m_pModel->GetNonLinear();

		int nRows = ui.tableWidget->rowCount();

		ui.tableWidget->insertRow(nRows);

		QString strTmp = ui.m_lineEditRawNon->text();

		ui.tableWidget->setItem(nRows, 0, new QTableWidgetItem(ui.m_lineEditRawNon->text()));
		ui.tableWidget->setItem(nRows, 1, new QTableWidgetItem(ui.m_lineEditNon->text()));
	}
}

/*! \fn void CSetScaleDialog::OnDele()
********************************************************************************************************* 
** \brief CSetScaleDialog::OnDele 
** \details 删除非线性转换
** \return void 
** \author gw
** \date 2016年4月19日 
** \note 
********************************************************************************************************/
void CSetScaleDialog::OnDele()
{
	int rowIndex = ui.tableWidget->currentRow();
		
	if (rowIndex != -1)
	{
		ui.tableWidget->removeRow(rowIndex);
	}

}

/*! \fn void CSetScaleDialog::OnModify()
********************************************************************************************************* 
** \brief CSetScaleDialog::OnModify 
** \details 修改非线性转换
** \return void 
** \author gw
** \date 2016年4月19日 
** \note 
********************************************************************************************************/
void CSetScaleDialog::OnModify()
{

}