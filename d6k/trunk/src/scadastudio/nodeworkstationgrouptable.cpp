#include "nodeworkstationgrouptable.h"
#include "nodeworkstationgroupnmodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "confignodeservicedialog.h"
#include "config_data.h"

#include <QMenu>

CNodeWorkstationGroupTable::CNodeWorkstationGroupTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pWorkstationData
	, Config::CNodeConfigData *pNodeConfig)
	: QTableView(parent), m_pSortModel(nullptr), m_pWorkstationData(nullptr)
{
	Q_ASSERT(pWorkstationData);
	if (!pWorkstationData)
	{
		return;
	}
	m_pWorkstationData = pWorkstationData;

	Q_ASSERT(pNodeConfig);
	if (!pNodeConfig)
	{
		return;
	}
	m_pNodeConfig = pNodeConfig;

	m_pModel = new CNodeWorkstationGroupModel(this, pCore, pWorkstationData, pNodeConfig);

	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);
	
	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

CNodeWorkstationGroupTable::~CNodeWorkstationGroupTable()
{

}

/*! \fn void CNodeWorkstationGroupTable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CNodeWorkstationGroupTable::ShowMouseRightButton 
** \details 右键槽
** \param point 
** \return void 
** \author gw
** \date 2016年12月9日 
** \note 
********************************************************************************************************/
void CNodeWorkstationGroupTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSortModel->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(NULL);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);

	pAddPoint->setIcon(QIcon(NODE_PNG));

	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);

	pMenu->addAction(pDeletePoint);

	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));

	pMenu->addAction(pDeletePoint);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//添加模拟量点
		AddAnalogPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//删除模拟量点
		DeleteAnalogPoint(indexSelect);
	}

	pMenu->deleteLater();
}

void CNodeWorkstationGroupTable::AddAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	auto nSize = m_pModel->GetSize();

	m_pModel->InsertRows(nSize, 1);
}

void CNodeWorkstationGroupTable::DeleteAnalogPoint(QModelIndex &index)
{
	auto nSize = m_pModel->GetSize();
	if (nSize == 0)
	{
		return;
	}

	if (!index.isValid())
	{
		return;
	}

	auto nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

/*! \fn void CNodeWorkstationGroupTable::DoubleClicked(const QModelIndex & index)
********************************************************************************************************* 
** \brief CNodeWorkstationGroupTable::DoubleClicked 
** \details server和workstation的service配置对话框是一样的
** \param index 
** \return void 
** \author gw
** \date 2017年2月25日 
** \note 
********************************************************************************************************/
void CNodeWorkstationGroupTable::DoubleClicked(const QModelIndex & index)
{
	if (!index.isValid())
	{
		return;
	}

	int nCol = index.column();
	auto indexTmp = m_pSortModel->mapToSource(index);
	if (nCol == CNodeWorkstationGroupModel::COLUMN::Service)
	{
		auto pNode = m_pWorkstationData->m_arrItem[indexTmp.row()];

		CConfigNodeServiceDialog dialog(nullptr, m_pNodeConfig, pNode);
		dialog.exec();
	}
}
