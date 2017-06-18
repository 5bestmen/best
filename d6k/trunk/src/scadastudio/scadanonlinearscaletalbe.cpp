#include "scadanonlinearscaletalbe.h"
#include "scadanonlinearscalemodel.h"
#include "scadanonlinearscalesetdialog.h"

CScadaNonlinearScaleTable::CScadaNonlinearScaleTable(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
	, Config::CScadaVariableConfigData *pConfig)
	: QTableView(parent), m_pModel(nullptr)
{
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	m_pTransform = pTransform;
	
	m_pModel = new CScadaNonlinearScaleModel(this, pTransform, pItem, pModule, pConfig);
	setModel(m_pModel);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(DoubleClicked(const QModelIndex &)));
}

CScadaNonlinearScaleTable::~CScadaNonlinearScaleTable()
{

}

/*! \fn void CScadaNonlinearScaleTable::DoubleClicked(const QModelIndex & index)
********************************************************************************************************* 
** \brief CScadaNonlinearScaleTable::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月21日 
** \note 
********************************************************************************************************/
void CScadaNonlinearScaleTable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();

	if (nCol == CScadaNonlinearScaleModel::COLUMN::ScaleType)
	{
		CScadaNonlinearScaleSetDialog dialog;
		dialog.Init(m_pTransform);

		dialog.exec();
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaNonlinearScaleTable::Refresh 
** \details 
** \return void 
** \author gw
** \date 2017年2月3日 
** \note 
********************************************************************************************************/
void CScadaNonlinearScaleTable::Refresh()
{
	m_pModel->RefrushModel();
}