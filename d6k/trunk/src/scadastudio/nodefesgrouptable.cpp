#include "nodefesgrouptable.h"
#include "nodefesgroupmodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"

#include <QMenu>

CNodeFesGroupTable::CNodeFesGroupTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pFesGroupData
	, Config::CNodeConfigData *pNodeConfig)
	: QTableView(parent), m_pModel(nullptr), m_pSortModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pModel = new CNodeFesGroupModel(this, pCore, pFesGroupData, pNodeConfig);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);
	
	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
}

CNodeFesGroupTable::~CNodeFesGroupTable()
{

}

void CNodeFesGroupTable::ShowMouseRightButton(const QPoint& point)
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

void CNodeFesGroupTable::AddAnalogPoint(QModelIndex &index)
{
	Q_UNUSED(index);
	
	auto nSize = m_pModel->GetSize();

	m_pModel->InsertRows(nSize, 1);
}

/*! \fn void CNodeFesGroupTable::DeleteAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeFesGroupTable::DeleteAnalogPoint 
** \details 
** \param index 
** \return void 
** \author guowei
** \date 2016年12月13日 
** \note 
********************************************************************************************************/
void CNodeFesGroupTable::DeleteAnalogPoint(QModelIndex &index)
{
	//auto nSize = m_pModel->GetSize();

	if (!index.isValid())
	{
		return;
	}

	m_pModel->RemoveRows(index.row(), 1);
}