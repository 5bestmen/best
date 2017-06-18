#include "forwarding_channel_table.h"
#include "forwarding_channel_model.h"
#include "sortfiltermodel.h"
#include "scadastudio/icore.h"

#include <QHeaderView>

/*! \fn CChannelTable::CChannelTable(QWidget *parent, QString strObjectName, Config::CChannelData *pChannel, CQuoteItem *pItem, CChannel *pChannelModel)
********************************************************************************************************* 
** \brief CChannelTable::CChannelTable 
** \details 
** \param parent 
** \param strObjectName 
** \param pChannel 
** \param pItem 
** \param pChannelModel 
** \return  
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
CForwardingChannelTable::CForwardingChannelTable(QWidget *parent, Config::CForwardingChannelData *pChannel, CQuoteItem *pItem, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QTableView(parent)
{
	Q_ASSERT(pChannel);
	
	m_pChannel = pChannel;

	this->setAlternatingRowColors(true);

	m_pSortModel = new CSortFilterModel(this);

	m_pModel = new CForwardingChannelModel(this, pChannel, pItem, pFes, pCore);

	m_pSortModel->setSourceModel(m_pModel);

	this->setModel(m_pSortModel);

	verticalHeader()->setHidden(true);

	m_pUi = (IMainWindow *)parent;

	//connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, CChannelModel *)), pChannelModel, SLOT(SendSelectedRows(QSet<int32s> &, int32s, CChannelModel *)));

	//connect(m_pModel, SIGNAL(SendChangeChannelModelRows(QSet<int32s> &, int32s, CChannelModel *)), pChannelModel, SLOT(SendChangeChannelModelRows(QSet<int32s> &, int32s, CChannelModel *)));
}

/*! \fn CChannelTable::~CChannelTable()
********************************************************************************************************* 
** \brief CChannelTable::~CChannelTable 
** \details 
** \return  
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
CForwardingChannelTable::~CForwardingChannelTable()
{

}

void CForwardingChannelTable::mouseReleaseEvent(QMouseEvent * event)
{
	QSet<int32s> set;

	QModelIndexList indexList = selectionModel()->selectedIndexes();
	int row;
	foreach(QModelIndex index, indexList) 
	 {
		index = m_pSortModel->mapToSource(index);

		row = index.row();

		set.insert(row);
	}

	if (set.count())
	{
		//emit SendSelectedRows(set, FES_CHANNEL_CHILD_ITEM, m_pModel);
	}

	QAbstractItemView::mouseReleaseEvent(event);
}

CForwardingChannelModel *CForwardingChannelTable::GetModel()
{
	Q_ASSERT(m_pModel);
	
	return m_pModel;
}
