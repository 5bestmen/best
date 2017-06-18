#include "confignodeservicedialog.h"
#include "config_data.h"
#include "confignodeservicetablemodel.h"
#include "addservicedialog.h"
#include "addserviceplandialog.h"

#include <QMessageBox>

CConfigNodeServiceDialog::CConfigNodeServiceDialog(QWidget *parent, Config::CNodeConfigData *pConfig, Config::CNode *pNode)
	: QDialog(parent), m_pConfig(nullptr), m_pNode(nullptr), m_pModel(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	m_pConfig = pConfig;
	
	Q_ASSERT(pNode);
	if (!pNode)
	{
		return;
	}
	m_pNode = pNode;

	ui.setupUi(this);

	m_pModel = new CConfigNodeServiceTableModel(ui.tableView, m_pConfig, pNode);
	ui.tableView->setModel(m_pModel);

	ui.tableView->horizontalHeader()->setStretchLastSection(true);

	connect(ui.tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(ClickedTable(const QModelIndex &)));
	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(ClickedAdd()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(ClickedDelete()));
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(ClickedOnOk()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(ClickedOnCancel()));
	connect(ui.pushButton_addPlan, SIGNAL(clicked()), this, SLOT(ClickedOnAddPlan()));

	ui.lineEdit_name->setReadOnly(true);
	ui.lineEdit_load->setReadOnly(true);
	ui.lineEdit_argument->setReadOnly(true);
	ui.lineEdit_description->setReadOnly(true);
	ui.lineEdit_option->setReadOnly(true);
}

CConfigNodeServiceDialog::~CConfigNodeServiceDialog()
{

}

void CConfigNodeServiceDialog::ClickedTable(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	auto &arr = m_pModel->GetArr();

	auto nRow = index.row();
	auto bFlag = nRow >= 0 && nRow < arr.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return;
	}

	ui.lineEdit_name->setText(arr[index.row()]->Name);
	ui.lineEdit_load->setText(arr[index.row()]->LoadType);
	ui.lineEdit_argument->setText(arr[index.row()]->Argument);
	ui.lineEdit_description->setText(arr[index.row()]->Description);
	ui.lineEdit_option->setText(arr[index.row()]->Option);
}

/*! \fn void CConfigNodeServiceDialog::ClickedAdd()
********************************************************************************************************* 
** \brief CConfigNodeServiceDialog::ClickedAdd 
** \details 添加服务
** \return void 
** \author gw
** \date 2017年2月22日 
** \note 
********************************************************************************************************/
void CConfigNodeServiceDialog::ClickedAdd()
{
	CAddServiceDialog dialog(nullptr, m_pConfig, m_pNode);
	if (dialog.exec() == QDialog::Accepted)
	{
		m_pModel->appendData(dialog.GetArr());
	}
}

/*! \fn void CConfigNodeServiceDialog::ClickedDelete()
********************************************************************************************************* 
** \brief CConfigNodeServiceDialog::ClickedDelete 
** \details 
** \return void 
** \author gw
** \date 2017年2月23日 
** \note 
********************************************************************************************************/
void CConfigNodeServiceDialog::ClickedDelete()
{
	auto selectModel = ui.tableView->selectionModel();
	auto &list = selectModel->selectedRows();
	qSort(list.begin(), list.end(), qGreater<QModelIndex>()); // so that rows are removed from highest index
	auto &arr = m_pModel->GetArr();

	QMessageBox box;
	QString strError;
	for each (auto var in list)
	{
		int nRow = var.row();
		auto bFlag = nRow >= 0 && nRow < arr.size();
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return;
		}

		if (arr[nRow]->Option.compare(OPTION_TRUE, Qt::CaseInsensitive) == 0)
		{
			strError = QObject::tr("Service %1 can not delete!!!").arg(arr[nRow]->Name);
			box.setText(strError);
			
			box.exec();
			continue;
		}

		m_pModel->deleteData(arr[nRow]);
		//var.row()
	}

	m_pModel->RefrushModel();
}

void CConfigNodeServiceDialog::ClickedOnOk()
{
	auto &arr = m_pModel->GetArr();
	if (arr.empty())
	{
		reject();

		close();

		return;
	}
	
	m_pNode->m_arrNodeServiceRole.clear();
	m_pNode->m_arrNodeServiceRole = m_pModel->GetArr();

	accept();

	close();
}

void CConfigNodeServiceDialog::ClickedOnCancel()
{
	reject();

	close();
}

/*! \fn void CConfigNodeServiceDialog::ClickedOnAddPlan()
********************************************************************************************************* 
** \brief CConfigNodeServiceDialog::ClickedOnAddPlan 
** \details 添加服务方案
** \return void 
** \author gw
** \date 2017年2月24日 
** \note 
********************************************************************************************************/
void CConfigNodeServiceDialog::ClickedOnAddPlan()
{
	CAddServicePlanDialog dialog(nullptr, m_pConfig);
	if (dialog.exec() == QDialog::Accepted)
	{
		auto strTmp = QObject::tr("The selected service will be deleted!!! Do u contuinue?");
		if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "title", strTmp, QMessageBox::Yes | QMessageBox::No).exec())
		{
			auto pTmp = dialog.GetTemplate();
			//Q_ASSERT(pTmp);
			if (!pTmp)
			{
				return;
			}

			for each (auto var in m_pNode->m_arrNodeServiceRole)
			{
				m_pModel->deleteData(var);
			}

			m_pModel->appendData(pTmp->m_arrServiceRole);
		}
	}
}
