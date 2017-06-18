#include "nodefestable.h"
#include "nodefesmodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"

#include <QMenu>

CNodeFesTable::CNodeFesTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pData
	, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig)
	: QTableView(parent), m_pModel(nullptr), m_pSortModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pModel = new CNodeFesModel(this, pCore, pData, pItem, pNodeConfig);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);
	setSortingEnabled(true);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
}

CNodeFesTable::~CNodeFesTable()
{

}

void CNodeFesTable::ShowMouseRightButton(const QPoint& point)
{
	Q_UNUSED(point);
	
	//QModelIndex indexSelect = m_pSortModel->mapToSource(this->indexAt(point));

	//QMenu *pMenu = new QMenu(NULL);

	//QAction *pAddPoint = new QAction(tr("add point"), pMenu);

	//pAddPoint->setIcon(QIcon(NODE_PNG));

	//pMenu->addAction(pAddPoint);

	//QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);

	//pMenu->addAction(pDeletePoint);

	//pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));

	//pMenu->addAction(pDeletePoint);

	//QAction* selectedItem = pMenu->exec(QCursor::pos());

	//if (selectedItem == pAddPoint)
	//{
	//	//添加模拟量点
	//	AddAnalogPoint(indexSelect);
	//}
	//else if (selectedItem == pDeletePoint)
	//{
	//	//删除模拟量点
	//	DeleteAnalogPoint(indexSelect);
	//}

	//pMenu->deleteLater();
}

void CNodeFesTable::AddAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	//auto nSize = m_pModel->GetSize();

	//m_pModel->InsertRows(nSize, 1);
}

void CNodeFesTable::DeleteAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	auto nSize = m_pModel->GetSize();

	m_pModel->RemoveRows(0, nSize);
}