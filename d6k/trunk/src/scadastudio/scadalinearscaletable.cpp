#include "scadalinearscaletable.h"
#include "sortfiltermodel.h"
#include "scadalinearscalemodel.h"
#include "scadalinearscalesetdialog.h"

CScadaLinearScaleTable::CScadaLinearScaleTable(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
	, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QTableView(parent), m_pModel(nullptr), m_pSort(nullptr), m_pTransform(nullptr)
{
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	m_pTransform = pTransform;

	m_pSort = new CSortFilterModel(this);
	m_pModel = new CScadaLinearScaleModel(this, pTransform, pItem, pModule, pConfig, pScada);

	m_pSort->setSourceModel(m_pModel);

	setModel(m_pModel);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(DoubleClicked(const QModelIndex &)));
}

CScadaLinearScaleTable::~CScadaLinearScaleTable()
{

}

/*! \fn void CScadaLinearScaleTable::DoubleClicked(const QModelIndex & index)
********************************************************************************************************* 
** \brief CScadaLinearScaleTable::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017Äê1ÔÂ18ÈÕ 
** \note 
********************************************************************************************************/
void CScadaLinearScaleTable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();

	if (nCol == CScadaLinearScaleModel::COLUMN::ScaleType)
	{
		CScadaLinearScaleSetDialog dialog;
		dialog.Init(m_pTransform);

		dialog.exec();
	}
}

void CScadaLinearScaleTable::Refresh()
{
	m_pModel->RefrushModel();
}
