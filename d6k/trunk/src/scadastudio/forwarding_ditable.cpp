#include <QMenu>
#include "forwarding_ditable.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "forwarding_dimodel.h"
#include "channel.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "selectalarmdialog.h"
#include "forwarding_select_di_tagname_dialog.h"
#include "global.h"
#include "scadastudio/icore.h"

CForwardingDITable::CForwardingDITable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData
	, Config::CFesData *pFes)
	: QTableView(parent), m_pCore(nullptr)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	m_pFes = pFes;
	
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;
	
	m_pSortModel = new CSortFilterModel(this);
	m_pSortModel->clear();

	m_pModel = new CForwardingDIModel(this, pChannelData, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));

	setSelectionMode(ContiguousSelection);
	
	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CForwardingDITable::~CForwardingDITable()
{

}

void CForwardingDITable::Refresh()
{
	m_pModel->RefrushModel();
}

#include <qdebug.h>
void CForwardingDITable::mouseReleaseEvent(QMouseEvent * event)
{
	QSet<int32s> set;

	static int i = 0;

	qDebug() << i;

	QModelIndexList indexList = selectionModel()->selectedIndexes();
	int row;
	foreach(QModelIndex index, indexList) {
		index = m_pSortModel->mapToSource(index);

		row = index.row();

		set.insert(row);
	}

	if (set.count())
	{
		//emit SendSelectedRows(set, DEVICE_DI, (void *)m_pModel);
	}

	QAbstractItemView::mouseReleaseEvent(event);
}

/*! \fn void CDITable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CDITable::ShowMouseRightButton 
** \details 右键添加删除
** \param point 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwardingDITable::ShowMouseRightButton(const QPoint& point)
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
		AddAnalogPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//删除模拟量点
		DeleteAnalogPoint(indexSelect);
	}

	pMenu->deleteLater();
}

/*! \fn void CDITable::AddAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CDITable::AddAnalogPoint 
** \details 添加点
** \param index 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwardingDITable::AddAnalogPoint(QModelIndex &index)
{
	int nRow = index.row();

	nRow = m_pModel->GetDIsCount();
	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CDITable::DeleteAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CDITable::DeleteAnalogPoint 
** \details 删除点
** \param index 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwardingDITable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

/*! \fn void CDITable::DoubleClicked(const QModelIndex & index)
********************************************************************************************************* 
** \brief CDITable::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年11月22日 
** \note 
********************************************************************************************************/
void CForwardingDITable::DoubleClicked(const QModelIndex & index)
{
	if (!index.isValid())
	{
		return;
	}
	
	auto indexTmp = m_pSortModel->mapToSource(index);
	if (!indexTmp.isValid())
	{
		return;
	}

	int nRow = indexTmp.row();
	int nCol = indexTmp.column();

	if (nCol == CForwardingDIModel::COLUMN::SourceTagName)
	{
		CForwarding_select_di_tagname_dialog *pDialog = new CForwarding_select_di_tagname_dialog(nullptr, m_pFes);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			auto &arr = m_pModel->GetDIs();
			
			if (!CheckTagNameIsValid(pDialog->m_strTagName, FES_DESC))
			{
				return;
			}

			if (arr[nRow]->m_strSourceTagName.isEmpty())
			{
				m_pFes->AddForwardingDIRelationSourceTagNameArr(pDialog->m_strTagName.toStdString(), arr[nRow]);
			}
			else
			{
				if (m_pFes->ModifyForwardingDIRelationSourceTagNameArr(pDialog->m_strTagName.toStdString(), arr[nRow]->m_strSourceTagName.toStdString(), arr[nRow]))
				{
					auto strError = QObject::tr("Source hash tagname %1 of Forwarding DIs tagname %2 modify fail!!!").arg(arr[indexTmp.row()]->m_strSourceTagName).arg(arr[indexTmp.row()]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}
			
			m_pModel->setData(index, pDialog->m_strTagName);
		}

		pDialog->deleteLater();
	}
}