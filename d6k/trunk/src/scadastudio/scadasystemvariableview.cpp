#include "scadasystemvariableview.h"
#include "scadasystemvariabletable.h"
#include "sortfiltermodel.h"


CScadaSystemVariableView::CScadaSystemVariableView(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pConfigData
	, Config::CScadaData *pData)
	: QMainWindow(parent)
{
	m_pTable = new CScadaSystemVariableTable(this, pCore, pConfigData, pData);

	setCentralWidget(m_pTable);
}

CScadaSystemVariableView::~CScadaSystemVariableView()
{
	//if (m_pTable)
	//{
	//	m_pTable->setModel(nullptr);
	//	m_pTable->GetSortModel()->setSourceModel(nullptr);
	//	delete m_pTable;
	//	m_pTable;
	//}
}

/*! \fn void CScadaSystemVariableView::Delete()
********************************************************************************************************* 
** \brief CScadaSystemVariableView::Delete 
** \details �ر�tabwidgetʱ,model��view�ֿ�
** \return void 
** \author gw
** \date 2016��10��11�� 
** \note 
********************************************************************************************************/
void CScadaSystemVariableView::Delete()
{
	m_pTable->setModel(nullptr);
}