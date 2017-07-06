/*! @file
<PRE>
********************************************************************************
ģ����       :	AI��
�ļ���       :	aitable.cpp
�ļ�ʵ�ֹ��� :  ��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.05.10
*/

#include "aitable.h"
#include "sortfiltermodel.h"
#include "aimodel.h"
#include "alarm.h"
#include "channel.h"
#include "selectscaledialog.h"
#include "selectalarmdialog.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "scadastudio/icore.h"

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
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
CAITable::CAITable(QWidget *parent, std::vector <Config::CAnalogInput *> &arrAIs, IMainModuleInterface *pCore, CChannel *pModule, Config::CFesData *pFes)
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
	
	m_pModel = new CAIModel(this, arrAIs, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);

	this->setColumnHidden(CAIModel::COLUMN::ChannelOccNo, true);
	this->setColumnHidden(CAIModel::COLUMN::DeviceOccNo, true);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), (CChannel *)pModule, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);
}

CAITable::~CAITable()
{

}

/*! \fn void CAITable::mouseReleaseEvent(QMouseEvent * event)
********************************************************************************************************* 
** \brief CAITable::mouseReleaseEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CAITable::mouseReleaseEvent(QMouseEvent * event)
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

void CAITable::Refresh()
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
** \date 2016��6��1�� 
** \note 
********************************************************************************************************/
void CAITable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();

	auto indexTmp = m_pSortModel->mapToSource(index);

	if (nCol == CAIModel::COLUMN::ScaleTagName)
	{
		auto arrAIs = m_pModel->GetAIs();

		arrAIs[index.row()]->m_strScaleTagName;

		CSelectScaleDialog *pDialog = new CSelectScaleDialog(NULL, m_pFes, arrAIs[indexTmp.row()]->m_strScaleTagName,
			arrAIs[indexTmp.row()]->m_nScaleType, arrAIs[indexTmp.row()]->m_bNewPointScale, AISELECTSCALE);
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
				arrAIs[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
				arrAIs[indexTmp.row()]->m_bNewPointScale = false;	//�����¼ӵ���

				//���ת��tagname����AIs
				m_pFes->AddAIRelationScaleArr(arrAIs[indexTmp.row()]->m_strScaleTagName.toStdString(), arrAIs[indexTmp.row()]);
			}
			else
			{
				auto strTagName = pDialog->GetTagName();
				
				//���ת��tagname����AIs
				bool bFlag = m_pFes->ModifyAIsRelationScaleArr(strTagName.toStdString(),
					arrAIs[indexTmp.row()]->m_strScaleTagName.toStdString(), arrAIs[indexTmp.row()]->m_strScaleDesc.toStdString(),
					pDialog->GetType());
				
				if (!bFlag)
				{
					auto strError = QObject::tr("AIs relation scale %1 modify fail!!!").arg(arrAIs[indexTmp.row()]->m_strScaleTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				arrAIs[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				arrAIs[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
				arrAIs[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
				arrAIs[indexTmp.row()]->m_bNewPointScale = false;	//�����¼ӵ���
			}
		}

		pDialog->deleteLater();
	}
	else if (nCol == CAIModel::COLUMN::AlarmTagName)
	{
		auto arrAIs = m_pModel->GetAIs();

		arrAIs[indexTmp.row()]->m_strScaleTagName;

		CSelectAlarmDialog *pDialog = new CSelectAlarmDialog(NULL, m_pFes, arrAIs[indexTmp.row()], AIALARM);
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			if (arrAIs[indexTmp.row()]->m_strAlarmTagName == pDialog->GetTagName())
			{	
				pDialog->deleteLater();

				return;
			}
			
			if (arrAIs[indexTmp.row()]->m_strAlarmTagName.isEmpty())
			{
				arrAIs[indexTmp.row()]->m_strAlarmTagName = pDialog->GetTagName();
				arrAIs[indexTmp.row()]->m_nAlarmType = pDialog->GetType();
				arrAIs[indexTmp.row()]->m_strAlarmDesc = pDialog->GetDesc();
				arrAIs[indexTmp.row()]->m_bNewPointScale = false;

				m_pFes->AddAIRelationAlarmArr(arrAIs[indexTmp.row()]->m_strAlarmTagName.toStdString(), arrAIs[indexTmp.row()]);
			}
			else
			{
				m_pFes->ModifyAIsRelationAlarmArr(pDialog->GetTagName().toStdString(),
					arrAIs[indexTmp.row()]->m_strAlarmTagName.toStdString(), arrAIs[indexTmp.row()]->m_strAlarmDesc.toStdString(), arrAIs[indexTmp.row()]);
				
				arrAIs[indexTmp.row()]->m_strAlarmTagName = pDialog->GetTagName();
				arrAIs[indexTmp.row()]->m_nAlarmType = pDialog->GetType();
				arrAIs[indexTmp.row()]->m_strAlarmDesc = pDialog->GetDesc();
				arrAIs[indexTmp.row()]->m_bNewPointScale = false;
			}
		}

		pDialog->deleteLater();
	}
}

/*! \fn void CAITable::ShowMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CAITable::ShowMouseRightButton 
** \details ң������ɾ���˵�
** \param point 
** \return void 
** \author gw
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CAITable::ShowMouseRightButton(const QPoint& point)
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

	QAction *pClearRelationPoint = nullptr;
	if (indexSelect.column() == CAIModel::COLUMN::AlarmTagName
		|| indexSelect.column() == CAIModel::COLUMN::ScaleTagName)
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
	else if (selectedItem == pBatchAddPoint)
	{

	}
	else if (selectedItem == pClearRelationPoint && pClearRelationPoint != nullptr)
	{
		//��չ���
		m_pModel->setData(indexSelect, Qt::EditRole);
	}

	pMenu->deleteLater();
}

/*! \fn void CAITable::AddAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAITable::AddAnalogPoint 
** \details ���ģ������
** \param index 
** \return void 
** \author gw
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CAITable::AddAnalogPoint(QModelIndex &index)
{
	int nRow = index.row();
	auto test = m_pModel->GetAIs();
	nRow = (int)test.size();
	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CAITable::DeleteAnalogPoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CAITable::DeleteAnalogPoint 
** \details ɾ��ģ������
** \param index 
** \return void 
** \author gw
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CAITable::DeleteAnalogPoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

void CAITable::BatchAddPoint()
{
	//�����Ի���

}
