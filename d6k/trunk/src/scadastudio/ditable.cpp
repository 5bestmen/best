#include <QMenu>
#include "ditable.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "dimodel.h"
#include "channel.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "selectalarmdialog.h"
#include "fes_di_select_doubledi_dialog.h"
#include "scadastudio/icore.h"
#include "global.h"

CDITable::CDITable(QWidget *parent, std::vector <Config::CDigitalInput *> &arrDIs, IMainModuleInterface *pCore, void *pModule, Config::CFesData *pFes)
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

	m_pModel = new CDIModel(this, arrDIs, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	this->setColumnHidden(CDIModel::COLUMN::ChannelOccNo, true);
	this->setColumnHidden(CDIModel::COLUMN::DeviceOccNo, true);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));

	setSelectionMode(ContiguousSelection);
	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CDITable::~CDITable()
{

}

#include <qdebug.h>
void CDITable::mouseReleaseEvent(QMouseEvent * event)
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
void CDITable::ShowMouseRightButton(const QPoint& point)
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
void CDITable::AddAnalogPoint(QModelIndex &index)
{
	int nRow = index.row();

	auto arr = m_pModel->GetDIs();
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
void CDITable::DeleteAnalogPoint(QModelIndex &index)
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
#include "forwarding_select_di_tagname_dialog.h"
void CDITable::DoubleClicked(const QModelIndex & index)
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

	if (nCol == CDIModel::COLUMN::AlarmTagName)
	{
		auto &arrDIs = m_pModel->GetDIs();

		arrDIs[indexTmp.row()]->m_strAlarmTag;

		CSelectAlarmDialog *pDialog = new CSelectAlarmDialog(NULL, m_pFes, arrDIs[indexTmp.row()], DIALARM);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{	
			if (arrDIs[indexTmp.row()]->m_strAlarmTag == pDialog->GetTagName())
			{
				return;
			}
			
			if (arrDIs[indexTmp.row()]->m_strAlarmTag.isEmpty())
			{
				arrDIs[indexTmp.row()]->m_strAlarmTag = pDialog->GetTagName();
				arrDIs[indexTmp.row()]->m_nAlarmType = pDialog->GetType();
				arrDIs[indexTmp.row()]->m_strAlarmDesc = pDialog->GetDesc();
				arrDIs[indexTmp.row()]->m_bNewPointAlarm = false;

				m_pFes->AddDIRelationAlarmArr(arrDIs[indexTmp.row()]->m_strAlarmTag.toStdString(), arrDIs[indexTmp.row()]);
			}
			else
			{
				m_pFes->ModifyDIsRelationAlarmArr(pDialog->GetTagName().toStdString(), arrDIs[indexTmp.row()]->m_strAlarmTag.toStdString(),
					arrDIs[indexTmp.row()]->m_strAlarmDesc.toStdString(), arrDIs[indexTmp.row()]);

				arrDIs[indexTmp.row()]->m_strAlarmTag = pDialog->GetTagName();
				arrDIs[indexTmp.row()]->m_nAlarmType = pDialog->GetType();
				arrDIs[indexTmp.row()]->m_strAlarmDesc = pDialog->GetDesc();
				arrDIs[indexTmp.row()]->m_bNewPointAlarm = false;
			}
		}
	}
	else if (nCol == CDIModel::COLUMN::DoubleDI)
	{
		auto &arrDIs = m_pModel->GetDIs();

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
			if (arrDIs[indexTmp.row()]->m_szDoubleDI == strTagName)
			{
				return;
			}

			QString strDoubleDITagName = arrDIs[indexTmp.row()]->m_szDoubleDI;

			if (strDoubleDITagName.isEmpty())
			{
				if (!m_pFes->AddDIRelationDoubleDIArr(dialog.GetTagName().toStdString(), arrDIs[nRow]))
				{
					auto strError = QObject::tr("Add DIs DoubleDI relation  %1 fail!!!").arg(strTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				memset(arrDIs[indexTmp.row()]->m_szDoubleDI, 0, sizeof(arrDIs[indexTmp.row()]->m_szDoubleDI));
				int nSize = strTagName.size();
				strncpy(arrDIs[indexTmp.row()]->m_szDoubleDI, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}
			else
			{
				if (!m_pFes->ModifyDIRelationDoubleDIArr(dialog.GetTagName().toStdString(), arrDIs[indexTmp.row()]->m_szDoubleDI, arrDIs[nRow]))
				{
					auto strError = QObject::tr("Modify DIs DoubleDI relation  %1 fail!!!").arg(strTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				memset(arrDIs[indexTmp.row()]->m_szDoubleDI, 0, sizeof(arrDIs[indexTmp.row()]->m_szDoubleDI));
				int nSize = strTagName.size();
				strncpy(arrDIs[indexTmp.row()]->m_szDoubleDI, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			}
		}
	}
	else if (nCol == CDIModel::COLUMN::DoubleDI)
	{

	}
}