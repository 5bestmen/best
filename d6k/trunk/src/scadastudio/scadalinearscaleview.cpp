#include "scadalinearscaleview.h"
#include "scadalinearscaletable.h"

CScadaLinearScaleView::CScadaLinearScaleView(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
	, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CScadaLinearScaleTable(this, pTransform, pItem, pModule, pConfig, pScada);

	setCentralWidget(m_pTable);
}

CScadaLinearScaleView::~CScadaLinearScaleView()
{

}

/*! \fn void CScadaLinearScaleView::Delete()
********************************************************************************************************* 
** \brief CScadaLinearScaleView::Delete 
** \details 
** \return void 
** \author gw
** \date 2017Äê1ÔÂ18ÈÕ 
** \note 
********************************************************************************************************/
void CScadaLinearScaleView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CScadaLinearScaleView::Save()
{

}

void CScadaLinearScaleView::Refresh()
{
	m_pTable->Refresh();
}
