#include "forwarding_kwhtable.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "channel.h"
#include "forwarding_kwhmodel.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "selectscaledialog.h"

#include <QMenu>

CForwardingKWHTable::CForwardingKWHTable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
	: QTableView(parent), m_pFes(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	m_pFes = pFes;

	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CForwardingKWHModel(this, pCore, pChannelData, pFes);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CForwardingKWHTable::~CForwardingKWHTable()
{

}

void CForwardingKWHTable::refresh()
{
	m_pModel->RefrushModel();
}

void CForwardingKWHTable::mouseReleaseEvent(QMouseEvent * event)
{
	//QSet<int32s> set;

	//QModelIndexList indexList = selectionModel()->selectedIndexes();
	//int row;
	//foreach(QModelIndex index, indexList) {
	//	index = m_pSortModel->mapToSource(index);

	//	row = index.row();

	//	set.insert(row);
	//}

	//if (set.count())
	//{
	//	//emit SendSelectedRows(set, DEVICE_AO, (void *)m_pModel);
	//}

	QAbstractItemView::mouseReleaseEvent(event);
}

/*! \fn void CDITable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CDITable::ShowMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwardingKWHTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSortModel->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(NULL);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);

	pAddPoint->setIcon(QIcon(CHANNEL_PNG));

	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);

	pMenu->addAction(pDeletePoint);

	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));

	pMenu->addAction(pDeletePoint);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//添加模拟量点
		AddKWHPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//删除模拟量点
		DeleteKWHPoint(indexSelect);
	}

	pMenu->deleteLater();
}

/*! \fn void CAOTable::AddAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAOTable::AddAnalogPoint 
** \details 添加模拟量点
** \param index 
** \return void 
** \author gw
** \date 2016年8月12日 
** \note 
********************************************************************************************************/
void CForwardingKWHTable::AddKWHPoint(QModelIndex &index)
{
	int nRow = index.row();

	auto arr = m_pModel->GetKWHs();
	nRow = (int)arr.size();

	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CAOTable::DeleteAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAOTable::DeleteAnalogPoint 
** \details 删除遥调点
** \param index 
** \return void 
** \author gw
** \date 2016年8月12日 
** \note 
********************************************************************************************************/
void CForwardingKWHTable::DeleteKWHPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

void CForwardingKWHTable::DoubleClicked(const QModelIndex & index)
{
	//int nCol = index.column();

	//auto indexTmp = m_pSortModel->mapToSource(index);

	//if (nCol == CAOModel::COLUMN::ScaleDesc)
	//{
	//	auto arrAOs = m_pModel->GetAOs();

	//	arrAOs[index.row()]->m_strScaleTagName;

	//	CSelectScaleDialog *pDialog = new CSelectScaleDialog(NULL, m_pFes, arrAOs[indexTmp.row()]->m_strScaleTagName,
	//		arrAOs[indexTmp.row()]->m_nScaleType, arrAOs[indexTmp.row()]->m_bNewPointScale, AOSELECTSCALE);
	//	//
	//	pDialog->setWindowModality(Qt::ApplicationModal);

	//	if (pDialog->exec() == QDialog::Accepted)
	//	{
	//		if (arrAOs[indexTmp.row()]->m_strScaleTagName == pDialog->GetTagName())
	//		{
	//			pDialog->deleteLater();

	//			return;
	//		}

	//		if (arrAOs[indexTmp.row()]->m_strScaleTagName.isEmpty())
	//		{
	//			arrAOs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
	//			arrAOs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
	//			arrAOs[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
	//			arrAOs[indexTmp.row()]->m_bNewPointScale = false;	//不是新加点了

	//			m_pFes->AddAORelationScaleArr(arrAOs[indexTmp.row()]->m_strScaleTagName.toStdString(), arrAOs[indexTmp.row()]);
	//		}
	//		else
	//		{
	//			
	//			m_pFes->ModifyAOsRelationScaleArr(pDialog->GetTagName().toStdString(), arrAOs[indexTmp.row()]->m_strScaleTagName.toStdString(),
	//				pDialog->GetDesc().toStdString(), arrAOs[indexTmp.row()]);

	//			arrAOs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
	//			arrAOs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
	//			arrAOs[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
	//			arrAOs[indexTmp.row()]->m_bNewPointScale = false;	//不是新加点了
	//		}
	//	}

	//	pDialog->deleteLater();
	//}
}