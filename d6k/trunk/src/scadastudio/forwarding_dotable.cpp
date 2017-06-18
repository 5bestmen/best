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
*  @date	2017.4.3
*/

#include <QMenu>
#include "forwarding_dotable.h"
#include "sortfiltermodel.h"
#include "forwarding_domodel.h"
#include "alarm.h"
#include "channel.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "forwarding_select_do_tagname_dialog.h"
#include "forwarding_select_di_tagname_dialog.h"
#include "scadastudio/icore.h"

CForwardingDOTable::CForwardingDOTable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
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

	m_pModel = new CForwardingDOModel(this, pCore, pChannelData, pFes);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CForwardingDOTable::~CForwardingDOTable()
{

}

void CForwardingDOTable::refresh()
{
	m_pModel->RefrushModel();
}

void CForwardingDOTable::mouseReleaseEvent(QMouseEvent * event)
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
	//	//emit SendSelectedRows(set, DEVICE_DO, (void *)m_pModel);
	//}

	QAbstractItemView::mouseReleaseEvent(event);
}

void CForwardingDOTable::ShowMouseRightButton(const QPoint& point)
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
		AddPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//删除模拟量点
		DeletePoint(indexSelect);
	}

	pMenu->deleteLater();
}

void CForwardingDOTable::DoubleClicked(const QModelIndex & index)
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

	int nCol = index.column();

	if (nCol == CForwardingDOModel::COLUMN::SourceTagName)
	{
		auto arrDOs = m_pModel->GetDOs();

		CForwarding_select_do_tagname_dialog *pDialog = new CForwarding_select_do_tagname_dialog(nullptr, m_pFes);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			int nRow = indexTmp.row();
			
			if (arrDOs[nRow]->m_strSourceTagName == pDialog->m_strTagName)
			{
				return;
			}
			

			if (arrDOs[nRow]->m_strSourceTagName.isEmpty())
			{
				if (!m_pFes->AddForwardingDORelationSourceTagNameArr(pDialog->m_strTagName.toStdString(), arrDOs[nRow]))
				{
					auto strError = QObject::tr("Source hash tagname %1 of Forwarding DOs tagname %2 add fail!!!").arg(arrDOs[nRow]->m_strSourceTagName).arg(arrDOs[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}
			else
			{
				if (!m_pFes->ModifyForwardingDORelationSourceTagNameArr(pDialog->m_strTagName.toStdString(), arrDOs[nRow]->m_strSourceTagName.toStdString(), arrDOs[nRow]))
				{
					auto strError = QObject::tr("Source hash tagname %1 of Forwarding DOs tagname %2 modify fail!!!").arg(arrDOs[nRow]->m_strSourceTagName).arg(arrDOs[nRow]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}
			
			arrDOs[indexTmp.row()]->m_strSourceTagName = pDialog->m_strTagName;
		}

		pDialog->deleteLater();
	}
	else if (nCol == CForwardingDOModel::COLUMN::AssociatedDI)
	{
		//关联遥控是否成功的遥信
		
		auto arrDOs = m_pModel->GetDOs();

		CForwarding_select_di_tagname_dialog *pDialog = new CForwarding_select_di_tagname_dialog(nullptr, m_pFes);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			memset(arrDOs[indexTmp.row()]->m_szAssociatedDI, 0, sizeof(arrDOs[indexTmp.row()]->m_szAssociatedDI));
			int nSize = pDialog->m_strTagName.size();
			strncpy(arrDOs[indexTmp.row()]->m_szAssociatedDI, pDialog->m_strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		}

		pDialog->deleteLater();
	}
	else if (nCol == CForwardingDOModel::COLUMN::BlockingSignalTag)
	{
		//暂时从DI里选择遥信点
		auto arrDOs = m_pModel->GetDOs();

		CForwarding_select_di_tagname_dialog *pDialog = new CForwarding_select_di_tagname_dialog(nullptr, m_pFes);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			memset(arrDOs[indexTmp.row()]->m_szBlockingSignalTag, 0, sizeof(arrDOs[indexTmp.row()]->m_szBlockingSignalTag));
			int nSize = pDialog->m_strTagName.size();
			strncpy(arrDOs[indexTmp.row()]->m_szBlockingSignalTag, pDialog->m_strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
		}

		pDialog->deleteLater();
	}
}

void CForwardingDOTable::AddPoint(QModelIndex &index)
{
	int nRow = index.row();

	auto arr = m_pModel->GetDOs();
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
void CForwardingDOTable::DeletePoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}