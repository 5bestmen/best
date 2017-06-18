#include "scadaview.h"
#include "scadatable.h"
#include "scadastudiodefine.h"
#include "scadastudio/icore.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/imainwindow.h"
#include "fesmodule.h"
#include "scadastudio/define.h"

/*! \fn CFesView::CFesView(IMainModuleInterface *pCore, Config::CFesData *pFesData, CQuoteItem *pItem)
********************************************************************************************************* 
** \brief CFesView::CFesView 
** \details 
** \param parent 
** \param pFesData 
** \return  
** \author gw
** \date 2016年2月14日 
** \note 
********************************************************************************************************/
CScadaView::CScadaView(IMainModuleInterface *pCore, Config::CScadaData *pData, CQuoteItem *pItem
	, Config::CScadaVariableConfigData *pConfig)
	: QMainWindow((QWidget *)pCore->GetUIMgr()), m_pCore(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pData);
	Q_ASSERT(pItem);

	m_pCore = pCore;
	
	m_pTable = new CScadaTable(this, pData, pItem, FES, pCore, pConfig);

	setCentralWidget(m_pTable);

	m_pScadaData = pData;
}

/*! \fn CFesView::~CFesView()
********************************************************************************************************* 
** \brief CFesView::~CFesView 
** \details 
** \return  
** \author gw
** \date 2016年2月14日 
** \note 
********************************************************************************************************/
CScadaView::~CScadaView()
{	
	delete m_pTable;
	m_pTable = nullptr;
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesView::Save 
** \details 
** \return void 
** \author gw
** \date 2016年2月18日 
** \note 
********************************************************************************************************/
void CScadaView::Save()
{
	//m_pFesData->Save();
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesView::Refresh 
** \details 
** \return void 
** \author gw
** \date 2016年2月18日 
** \note 
********************************************************************************************************/
void CScadaView::Refresh()
{
	m_pTable->Refresh();
}

void CScadaView::Delete()
{
	m_pTable->setModel(nullptr);
}