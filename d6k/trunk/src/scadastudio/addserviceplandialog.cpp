#include "addserviceplandialog.h"
#include "config_data.h"
#include "addplanservicemodel.h"
#include "clickplanservicemodel.h"

CAddServicePlanDialog::CAddServicePlanDialog(QWidget *parent, Config::CNodeConfigData *pConfig)
	: QDialog(parent), m_pConfig(pConfig), m_pTemplate(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	
	ui.setupUi(this);

	ui.tableView->horizontalHeader()->setStretchLastSection(true);

	CAddPlanServiceModel *pModel = new CAddPlanServiceModel(this, pConfig);
	ui.tableView->setModel(pModel);

	connect(ui.tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(ClickedTable(const QModelIndex &)));
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CAddServicePlanDialog::~CAddServicePlanDialog()
{

}

void CAddServicePlanDialog::ClickedTable(const QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	auto &arr = m_pConfig->m_arrStandTemplateService;

	auto nRow = index.row();
	auto bFlag = nRow >= 0 && nRow < arr.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return;
	}

	auto pModel = ui.tableView_2->model();
	if (pModel)
	{
		ui.tableView_2->setModel(nullptr);
		pModel->deleteLater();
	}

	m_pTemplate = arr[nRow];

	CClickPlanServiceModel *pNewModel = new CClickPlanServiceModel(ui.tableView_2, arr[nRow]);
	ui.tableView_2->setModel(pNewModel);

	//arr[nRow]->
}

void CAddServicePlanDialog::OnOk()
{
	accept();

	close();
}

void CAddServicePlanDialog::OnCancel()
{
	reject();

	close();
}
