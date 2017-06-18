
#include "forwaridng_channel_view.h"
#include "forwarding_channel_table.h"
#include "forwarding_channel_model.h"
#include "scadastudio/quoteItem.h"
#include "fesmodule.h"
#include "scadastudio/define.h"

CForwardingChannelView::CForwardingChannelView(QWidget *parent, Config::CForwardingChannelData *pChannel, CQuoteItem *pItem, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QMainWindow(parent), m_pFes(pFes)
{
	Q_ASSERT(pChannel);
	
	m_pTable = new CForwardingChannelTable(parent, pChannel, pItem, pFes, pCore);
	
	setCentralWidget(m_pTable);

	m_pChannel = pChannel;

	m_pItem = pItem;

	m_nNumber = 1234;
}

CForwardingChannelView::~CForwardingChannelView()
{

}

/*! \fn void CChannelView::Refresh()
********************************************************************************************************* 
** \brief CChannelView::Refresh 
** \details 刷新
** \return void 
** \author gw
** \date 2016年2月15日 
** \note 
********************************************************************************************************/
void CForwardingChannelView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CForwardingChannelView::Save()
{

}

//void CChannelView::ItemChanged(QStandardItem *item)
//{
//	m_pChannel->m_strDescription = item->data(Qt::EditRole).toString();
//
//	m_pTable->GetModel()->RefrushModel();
//}

void CForwardingChannelView::Delete()
{
	m_pTable->setModel(nullptr);
}