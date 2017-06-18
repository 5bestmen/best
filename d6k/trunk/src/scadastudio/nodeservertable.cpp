#include "nodeservertable.h"
#include "nodeservermodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "nodeservermodel.h"
#include "confignodeservicedialog.h"
#include "config_data.h"

#include <QMenu>
#include <QPushButton>

CNodeServerTable::CNodeServerTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeServer *pServerData
	, Config::CNodeConfigData *pNodeConfig)
	: QTableView(parent), m_pNodeConfig(nullptr), m_pServerData(nullptr)
{
	Q_ASSERT(pNodeConfig);
	if (!pNodeConfig)
	{
		return;
	}
	m_pNodeConfig = pNodeConfig;

	Q_ASSERT(pServerData);
	if (!pServerData)
	{
		return;
	}
	m_pServerData = pServerData;

	m_pModel = new CNodeServerModel(this, pCore, pServerData, pNodeConfig);

	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);
	
	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

CNodeServerTable::~CNodeServerTable()
{

}

void CNodeServerTable::ShowMouseRightButton(const QPoint& point)
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

void CNodeServerTable::AddAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	auto nSize = m_pModel->GetSize();

	m_pModel->InsertRows(nSize, 1);
}

void CNodeServerTable::DeleteAnalogPoint(QModelIndex &index)
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

	m_pModel->RemoveRows(index.row(), 1);
}

void CNodeServerTable::DoubleClicked(const QModelIndex & index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nCol = index.column();
	auto indexTmp = m_pSortModel->mapToSource(index);
	if (nCol == CNodeServerModel::COLUMN::Service)
	{
		auto pNode = m_pServerData->m_arrNodes[indexTmp.row()];
		
		CConfigNodeServiceDialog dialog(nullptr, m_pNodeConfig, pNode);
		dialog.exec();
	}
}