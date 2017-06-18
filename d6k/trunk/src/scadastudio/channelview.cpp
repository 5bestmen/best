
#include "channelview.h"
#include "channeltable.h"
#include "channelmodel.h"
#include "scadastudio/quoteItem.h"
#include "fesmodule.h"
#include "scadastudio/define.h"

CChannelView::CChannelView(QWidget *parent, Config::CChannelData *pChannel, CQuoteItem *pItem, CChannel *pChannelModel, Config::CFesData *pFes, IMainModuleInterface *pCore)
	: QMainWindow(parent), m_pFes(pFes)
{
	Q_ASSERT(pChannel);
	
	m_pTable = new CChannelTable(parent, CHANNEL_TYPE, pChannel, pItem, pChannelModel, pFes, pCore);
	
	setCentralWidget(m_pTable);

	m_pChannel = pChannel;

	m_pItem = pItem;

	m_nNumber = 1234;
}

CChannelView::~CChannelView()
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
void CChannelView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CChannelView::Save()
{

}

//void CChannelView::ItemChanged(QStandardItem *item)
//{
//	m_pChannel->m_strDescription = item->data(Qt::EditRole).toString();
//
//	m_pTable->GetModel()->RefrushModel();
//}

void CChannelView::Delete()
{
	m_pTable->setModel(nullptr);
}