/*! @file
<PRE>
********************************************************************************
模块名       :	AI表
文件名       :	aitable.cpp
文件实现功能 :  表
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.05.10
*/

#include <QMenu>
#include "dotable.h"
#include "sortfiltermodel.h"
#include "domodel.h"
#include "alarm.h"
#include "channel.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "fes_di_select_doubledi_dialog.h"
#include "global.h"
#include "scadastudio/icore.h"
#include "fesDo_BlockingSignalTag_Select_Dialog.h"

CDOTable::CDOTable(QWidget *parent, std::vector <Config::CDigitalOutput *> &arrDOs, IMainModuleInterface *pCore, void *pModule, Config::CFesData *pFes)
	: QTableView(parent), m_pSortModel(nullptr), m_pModel(nullptr), m_pFes(nullptr), m_pCore(nullptr)
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

	m_pModel = new CDOModel(this, arrDOs, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	this->setColumnHidden(CDOModel::COLUMN::ChannelOccNo, true);
	this->setColumnHidden(CDOModel::COLUMN::DeviceOccNo, true);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CDOTable::~CDOTable()
{

}

void CDOTable::mouseReleaseEvent(QMouseEvent * event)
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
	//	emit SendSelectedRows(set, DEVICE_DO, (void *)m_pModel);
	//}

	QAbstractItemView::mouseReleaseEvent(event);
}

void CDOTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSortModel->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(NULL);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);
	pAddPoint->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);
	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pDeletePoint);

	QAction *pClearRelationPoint = nullptr;
	if (indexSelect.column() == CDOModel::COLUMN::AssociatedDI
		|| indexSelect.column() == CDOModel::COLUMN::BlockingSignalTag)
	{
		pClearRelationPoint = new QAction(QObject::tr("Clear Relation"), pMenu);
		pClearRelationPoint->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pClearRelationPoint);
	}

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
	else if (selectedItem == pClearRelationPoint && pClearRelationPoint != nullptr)
	{
		//清空关联
		m_pModel->setData(indexSelect, Qt::EditRole);
	}

	pMenu->deleteLater();
}

void CDOTable::DoubleClicked(const QModelIndex & index)
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

	if (index.column() == CDOModel::AssociatedDI)
	{
		auto &arrDOs = m_pModel->GetDOs();

		CFes_di_select_doubledi_dialog dialog(nullptr, m_pFes);
		dialog.setWindowModality(Qt::ApplicationModal);

		if (dialog.exec() == QDialog::Accepted)
		{
			auto strTagName = dialog.GetTagName();
			if (!CheckTagNameIsValid(strTagName, FES_DESC))
			{
				return;
			}

			//相等没有改变
			if (arrDOs[indexTmp.row()]->m_szAssociatedDI == strTagName)
			{
				return;
			}

			QString strDoubleDITagName = arrDOs[indexTmp.row()]->m_szAssociatedDI;

			if (strDoubleDITagName.isEmpty())
			{
				//Do AssociatedDI添加关联前置DI关系
				if (!m_pFes->AddDORelationAssociatedDIArr(dialog.GetTagName().toStdString(), arrDOs[nRow]))
				{
					auto strError = QObject::tr("Add DOs AssociatedDI relation  %1 fail!!!").arg(strTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				memset(arrDOs[indexTmp.row()]->m_szAssociatedDI, 0, sizeof(arrDOs[indexTmp.row()]->m_szAssociatedDI));
				int nSize = strTagName.size();
				strncpy(arrDOs[indexTmp.row()]->m_szAssociatedDI, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}
			else
			{
				//Do AssociatedDI修改关联前置DI关系
				if (!m_pFes->ModifyDORelationAssociatedDIArr(dialog.GetTagName().toStdString(), arrDOs[indexTmp.row()]->m_szAssociatedDI, arrDOs[nRow]))
				{
					auto strError = QObject::tr("Modify DIs DoubleDI relation  %1 fail!!!").arg(strTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				memset(arrDOs[indexTmp.row()]->m_szAssociatedDI, 0, sizeof(arrDOs[indexTmp.row()]->m_szAssociatedDI));
				int nSize = strTagName.size();
				strncpy(arrDOs[indexTmp.row()]->m_szAssociatedDI, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}
		}
	}
	else if (index.column() == CDOModel::BlockingSignalTag)
	{
		auto &arrDOs = m_pModel->GetDOs();
		
		//闭锁开入
		CFesDo_BlockingSignalTag_Select_Dialog dialog(nullptr, m_pFes);
		if (dialog.exec() == QDialog::Accepted)
		{
			auto strTagName = dialog.GetTagName();
			if (!CheckTagNameIsValid(strTagName, FES_DESC))
			{
				return;
			}

			//相等没有改变
			if (strTagName == arrDOs[nRow]->m_szBlockingSignalTag)
			{
				return;
			}

			QString strBlockingSignalTag = arrDOs[nRow]->m_szBlockingSignalTag;
			if (strBlockingSignalTag.isEmpty())
			{
				//添加关联关系
				if (!m_pFes->AddDORelationBlockingSignalTagArr(strTagName.toStdString(), arrDOs[nRow]))
				{
					auto strError = QObject::tr("Add DO Relation BlockingSignalTag %1 fail!!!").arg(strTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}
			else
			{
				//修改关联关系
				if (!m_pFes->ModifyDORelationBlockingSignalTagArr(strTagName.toStdString(), arrDOs[nRow]->m_szBlockingSignalTag, arrDOs[nRow]))
				{
					auto strError = QObject::tr("Modify DO Relation BlockingSignalTag %1 fail!!!").arg(strTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}

			memset(arrDOs[nRow]->m_szBlockingSignalTag, 0, sizeof(arrDOs[nRow]->m_szTagName));
			int nSize = strTagName.size();
			strncpy(arrDOs[nRow]->m_szBlockingSignalTag, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		}
	}
}

void CDOTable::AddAnalogPoint(QModelIndex &index)
{
	int nRow = index.row();

	auto arr = m_pModel->GetAIs();
	nRow = (int)arr.size();

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
void CDOTable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}