#include "addservicedialog.h"
#include "addservicemodel.h"
#include "config_data.h"

#include <QMessageBox>

CAddServiceDialog::CAddServiceDialog(QWidget *parent, Config::CNodeConfigData *pConfig, Config::CNode *pNode)
	: QDialog(parent)
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

	ui.setupUi(this);

	m_vecReturn.clear();

	m_vec.clear();
	m_vec = pNode->m_arrNodeServiceRole;

	CAddServiceModel *pModel = new CAddServiceModel(this, pConfig);
	ui.tableView->setModel(pModel);

	ui.tableView->horizontalHeader()->setStretchLastSection(true);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(SelectTableItem(const QModelIndex &)));
}

CAddServiceDialog::~CAddServiceDialog()
{

}

void CAddServiceDialog::OnOk()
{
	auto selectModel = ui.tableView->selectionModel();
	auto &list = selectModel->selectedRows();

	auto &arr = m_pConfig->m_arrServerService;

	for each (auto var in list)
	{
		int nRow = var.row();

		auto bFlag = nRow >= 0 && nRow < arr.size();
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return;
		}

		//ÅÐ¶ÏÊÇ·ñ´æÔÚservice
		for each (auto var in m_vec)
		{
			if (arr[nRow]->Name.compare(var->Name, Qt::CaseInsensitive) == 0)
			{
				QMessageBox box;
				box.setText(QObject::tr("service %1 is existed!!!").arg(var->Name));

				box.exec();

				return;
			}
		}

		arr[nRow]->Option = OPTION_FALSE;
		m_vecReturn.push_back(arr[nRow]);
	}

	accept();

	close();
}

void CAddServiceDialog::OnCancel()
{
	reject();

	close();
}

void CAddServiceDialog::SelectTableItem(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	auto nRow = index.row();
	auto &arr = m_pConfig->m_arrServerService;

	auto bFlag = nRow >= 0 && nRow < arr.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return;
	}

	for each (auto var in m_vec)
	{
		if (arr[nRow]->Name.compare(var->Name, Qt::CaseInsensitive) == 0)
		{
			QMessageBox box;
			box.setText(QObject::tr("service %1 is existed!!!").arg(var->Name));

			box.exec();

			return;
		}
	}

	arr[nRow]->Option = OPTION_FALSE;
	m_vecReturn.push_back(arr[nRow]);

	accept();

	close();
}

std::vector<Config::NODE_SERVICE_ROLE *> &CAddServiceDialog::GetArr()
{
	return m_vecReturn;
}
