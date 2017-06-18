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

#include "forwading_ai_table.h"
#include "sortfiltermodel.h"
#include "forwarding_aimodel.h"
#include "alarm.h"
#include "channel.h"
#include "selectscaledialog.h"
#include "selectalarmdialog.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "forwarding_select_ai_tagname_dialog.h"

#include <QMenu>

/*! \fn 
********************************************************************************************************* 
** \brief CAITable::CAITable 
** \details 
** \param parent 
** \param arrAIs 
** \param pCore 
** \param pModule 
** \param pFes 
** \return  
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
CForwadingAITable::CForwadingAITable(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
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
	
	m_pModel = new CForwardingAIModel(this, pChannelData, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CForwadingAITable::~CForwadingAITable()
{

}

/*! \fn void CAITable::mouseReleaseEvent(QMouseEvent * event)
********************************************************************************************************* 
** \brief CAITable::mouseReleaseEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwadingAITable::mouseReleaseEvent(QMouseEvent * event)
{	
	QSet<int32s> set;

	QModelIndexList indexList = selectionModel()->selectedIndexes();
	int row;
	foreach(QModelIndex index, indexList) {
		index = m_pSortModel->mapToSource(index);

		row = index.row();

		set.insert(row);
	}

	if (set.count())
	{
		//emit SendSelectedRows(set, DEVICE_AI, (void *)m_pModel);
	}

	QAbstractItemView::mouseReleaseEvent(event);
}

void CForwadingAITable::Refresh()
{
	m_pModel->RefrushModel();
}

/*! \fn void CAITable::DoubleClicked(const QModelIndex & index)
********************************************************************************************************* 
** \brief CAITable::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年6月1日 
** \note 
********************************************************************************************************/
void CForwadingAITable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();

	auto indexTmp = m_pSortModel->mapToSource(index);

	if (nCol == CForwardingAIModel::COLUMN::ScaleTagName)
	{
		auto arrAIs = m_pModel->GetAIs();

		arrAIs[index.row()]->m_strScaleTagName;

		CSelectScaleDialog *pDialog = new CSelectScaleDialog(NULL, m_pFes, arrAIs[indexTmp.row()]->m_strScaleTagName,
			arrAIs[indexTmp.row()]->m_nScaleType, false, AISELECTSCALE);
		//
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			if (arrAIs[indexTmp.row()]->m_strScaleTagName == pDialog->GetTagName())
			{
				pDialog->deleteLater();

				return;
			}
			
			if (arrAIs[indexTmp.row()]->m_strScaleTagName.isEmpty())
			{
				arrAIs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				arrAIs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();

				//添加转换tagname关联转发AIs
				m_pFes->AddForwardingAIRelationScaleArr(arrAIs[indexTmp.row()]->m_strScaleTagName.toStdString(), arrAIs[indexTmp.row()]);
			}
			else
			{
				auto strTagName = pDialog->GetTagName();
				//修改转换tagname关联AIs
				bool bFlag = m_pFes->ModifyForwardingAIsRelationScaleArr(arrAIs[indexTmp.row()], strTagName.toStdString(),
					arrAIs[indexTmp.row()]->m_strScaleTagName.toStdString(), std::string(),
					pDialog->GetType());
				
				if (!bFlag)
				{
					auto strError = QObject::tr("AIs relation scale %1 modify fail!!!").arg(arrAIs[indexTmp.row()]->m_strScaleTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				arrAIs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				arrAIs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
			}
		}

		pDialog->deleteLater();
	}
	else if (nCol == CForwardingAIModel::COLUMN::SourceTagName)
	{
		auto arrAIs = m_pModel->GetAIs();

		arrAIs[indexTmp.row()]->m_strScaleTagName;

		CForwarding_select_ai_tagname_dialog *pDialog = new CForwarding_select_ai_tagname_dialog(nullptr, m_pFes);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			if (arrAIs[indexTmp.row()]->m_strSourceTagName == pDialog->m_strTagName)
			{
				return;
			}
			
			if (arrAIs[indexTmp.row()]->m_strSourceTagName.isEmpty())
			{
				arrAIs[indexTmp.row()]->m_strSourceTagName = pDialog->m_strTagName;
				m_pFes->AddForwardingAIRelationSourceTagNameArr(arrAIs[indexTmp.row()]->m_strSourceTagName.toStdString(), arrAIs[indexTmp.row()]);
			}
			else
			{
				if (!m_pFes->ModifyForwardingAIRelationSourceTagNameArr(pDialog->m_strTagName.toStdString(), arrAIs[indexTmp.row()]->m_strSourceTagName.toStdString(), arrAIs[indexTmp.row()]))
				{
					auto strError = QObject::tr("Source hash tagname %1 of Forwarding AIs tagname %2 modify fail!!!").arg(arrAIs[indexTmp.row()]->m_strSourceTagName).arg(arrAIs[indexTmp.row()]->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				arrAIs[indexTmp.row()]->m_strSourceTagName = pDialog->m_strTagName;
			}
		}

		pDialog->deleteLater();
	}
}

/*! \fn void CAITable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CAITable::ShowMouseRightButton 
** \details 遥测表添加删除菜单
** \param point 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwadingAITable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSortModel->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(nullptr);

	QAction *pAddPoint = new QAction(QObject::tr("Single Add"), pMenu);
	pAddPoint->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);
	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pDeletePoint);

	QAction *pBatchAddPoint = new QAction(QObject::tr("Batch Add"), pMenu);
	pBatchAddPoint->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pBatchAddPoint);

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
	else if (selectedItem == pBatchAddPoint)
	{

	}

	pMenu->deleteLater();
}

/*! \fn void CAITable::AddAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAITable::AddAnalogPoint 
** \details 添加模拟量点
** \param index 
** \return void 
** \author gw
** \date 2016年8月11日 
** \note 
********************************************************************************************************/
void CForwadingAITable::AddAnalogPoint(QModelIndex &index)
{
	//int nRow = index.row();
	auto test = m_pModel->GetAIs();
	int nRow = (int)test.size();
	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CAITable::DeleteAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAITable::DeleteAnalogPoint 
** \details 删除模拟量点
** \param index 
** \return void 
** \author gw
** \date 2016年8月12日 
** \note 
********************************************************************************************************/
void CForwadingAITable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}

	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

void CForwadingAITable::BatchAddPoint()
{
	//弹个对话框

}
