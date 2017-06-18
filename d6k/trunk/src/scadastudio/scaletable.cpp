#include "scaletable.h"
#include "scalemodel.h"
#include "sortfiltermodel.h"
#include "scale.h"
#include "setscaledialog.h"

/*! \fn CScaleTable::CScaleTable(QWidget *parent, Config::CFesData *pFesData, CQuoteItem *pItem, CScale *pScaleModel, QString strObjectType)
********************************************************************************************************* 
** \brief CScaleTable::CScaleTable 
** \details 
** \param parent 
** \param pFesData 
** \param pItem 
** \param pScaleModel 
** \param strObjectType 
** \return  
** \author gw
** \date 2016年3月25日 
** \note 
********************************************************************************************************/
CScaleTable::CScaleTable(QWidget *parent, Config::CTransform *pTransform, CQuoteItem *pItem, CScale *pScaleModel
	, Config::CFesData *pFesConfig)
	: QTableView(parent), m_pModel(NULL), m_pSortModel(NULL)
{
	CScaleModel *pModel = new CScaleModel(this, pTransform, pItem, pFesConfig);

	m_pModel = pModel;

	m_pSortModel = new CSortFilterModel(this);

	m_pSortModel->setSourceModel(pModel);

	setModel(m_pSortModel);

	//选中行发送到转换模块
	connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, void *)), pScaleModel, SLOT(SendSelectedRows(QSet<int32s> &, int32s, void *)));

	connect(pModel, SIGNAL(SendChangeChannelModelRows(QSet<int32s> &, int32s, void *)), pScaleModel, SLOT(SendChangeChannelModelRows(QSet<int32s> &, int32s, void *)));

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(DoubleClicked(const QModelIndex &)));
}

CScaleTable::~CScaleTable()
{

}

/*! \fn void CScaleTable::mouseReleaseEvent(QMouseEvent *event)
********************************************************************************************************* 
** \brief CScaleTable::mouseReleaseEvent 
** \details 表格单击显示属性框
** \param event 
** \return void 
** \author gw
** \date 2016年3月25日 
** \note 
********************************************************************************************************/
void CScaleTable::mouseReleaseEvent(QMouseEvent *event)
{
	QSet<int> set;

	QModelIndexList indexList = selectionModel()->selectedIndexes();
	int row;
	foreach(QModelIndex index, indexList) {
		index = m_pSortModel->mapToSource(index);

		row = index.row();

		set.insert(row);
	}

	if (set.count())
	{
		emit SendSelectedRows(set, FES_TYPE_SCALE_LINEAR_CHILD_ITEM, m_pModel);
	}

	QAbstractItemView::mouseReleaseEvent(event);
}

void CScaleTable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();

	if (nCol == CScaleModel::COLUMN::ScaleConfig)
	{
		CSetScaleDialog *dialog = new CSetScaleDialog(NULL, m_pModel);

		dialog->setWindowModality(Qt::ApplicationModal);
		
		dialog->SetType(m_pModel->GetType());

		dialog->show();

		if (dialog->exec() == QDialog::Accepted)
		{

		}

		m_pModel->SetType(dialog->GetType());

		dialog->deleteLater();
	}
}

/*! \fn CScaleModel *CScaleTable::GetModel()
********************************************************************************************************* 
** \brief CScaleTable::GetModel 
** \details get model
** \return CScaleModel * 
** \author gw
** \date 2016年8月3日 
** \note 
********************************************************************************************************/
CScaleModel *CScaleTable::GetModel()
{
	return m_pModel;
}