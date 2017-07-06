#include <QMenu>
#include "aotable.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "channel.h"
#include "aomodel.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "selectscaledialog.h"

CAOTable::CAOTable(QWidget *parent, std::vector <Config::CAnalogOutput *> &arrAOs, IMainModuleInterface *pCore, void *pModule, Config::CFesData *pFes)
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

	m_pModel = new CAOModel(this, arrAOs, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	this->setColumnHidden(CAOModel::COLUMN::ChannelOccNo, true);
	this->setColumnHidden(CAOModel::COLUMN::DeviceOccNo, true);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CAOTable::~CAOTable()
{

}

void CAOTable::mouseReleaseEvent(QMouseEvent * event)
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
		//emit SendSelectedRows(set, DEVICE_AO, (void *)m_pModel);
	}

	QAbstractItemView::mouseReleaseEvent(event);
}

/*! \fn void CDITable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CDITable::ShowMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CAOTable::ShowMouseRightButton(const QPoint& point)
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
	if (indexSelect.column() == CAOModel::COLUMN::ScaleTagName)
	{
		pClearRelationPoint = new QAction(QObject::tr("Clear Relation"), pMenu);
		pClearRelationPoint->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pClearRelationPoint);
	}

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//���ģ������
		AddAnalogPoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//ɾ��ģ������
		DeleteAnalogPoint(indexSelect);
	}
	else if (selectedItem == pClearRelationPoint && pClearRelationPoint != nullptr)
	{
		//��չ���
		m_pModel->setData(indexSelect, Qt::EditRole);
	}

	pMenu->deleteLater();
}

/*! \fn void CAOTable::AddAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAOTable::AddAnalogPoint 
** \details ���ģ������
** \param index 
** \return void 
** \author gw
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CAOTable::AddAnalogPoint(QModelIndex &index)
{
	int nRow = index.row();

	auto arr = m_pModel->GetAOs();
	nRow = (int)arr.size();

	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CAOTable::DeleteAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAOTable::DeleteAnalogPoint 
** \details ɾ��ң����
** \param index 
** \return void 
** \author gw
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CAOTable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

void CAOTable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();

	auto indexTmp = m_pSortModel->mapToSource(index);

	if (nCol == CAOModel::COLUMN::ScaleTagName)
	{
		auto arrAOs = m_pModel->GetAOs();

		arrAOs[index.row()]->m_strScaleTagName;

		CSelectScaleDialog *pDialog = new CSelectScaleDialog(NULL, m_pFes, arrAOs[indexTmp.row()]->m_strScaleTagName,
			arrAOs[indexTmp.row()]->m_nScaleType, arrAOs[indexTmp.row()]->m_bNewPointScale, AOSELECTSCALE);
		//
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			if (arrAOs[indexTmp.row()]->m_strScaleTagName == pDialog->GetTagName())
			{
				pDialog->deleteLater();

				return;
			}

			if (arrAOs[indexTmp.row()]->m_strScaleTagName.isEmpty())
			{
				arrAOs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				arrAOs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
				arrAOs[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
				arrAOs[indexTmp.row()]->m_bNewPointScale = false;	//�����¼ӵ���

				m_pFes->AddAORelationScaleArr(arrAOs[indexTmp.row()]->m_strScaleTagName.toStdString(), arrAOs[indexTmp.row()]);
			}
			else
			{
				
				m_pFes->ModifyAOsRelationScaleArr(pDialog->GetTagName().toStdString(), arrAOs[indexTmp.row()]->m_strScaleTagName.toStdString(),
					pDialog->GetDesc().toStdString(), arrAOs[indexTmp.row()]);

				arrAOs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				arrAOs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
				arrAOs[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
				arrAOs[indexTmp.row()]->m_bNewPointScale = false;	//�����¼ӵ���
			}
		}

		pDialog->deleteLater();
	}
}