#include "systemvariableview.h"
#include "systemvariabletable.h"

CSystemVariableView::CSystemVariableView(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData)
	: QMainWindow(parent)
{
	m_pTable = new CSystemVariableTable(this, pCore, pFesData);

	setCentralWidget(m_pTable);
}

CSystemVariableView::~CSystemVariableView()
{

}

/*! \fn void CSystemVariableView::Delete()
********************************************************************************************************* 
** \brief CSystemVariableView::Delete 
** \details 关闭tabwidget时,model和view分开
** \return void 
** \author gw
** \date 2016年10月11日 
** \note 
********************************************************************************************************/
void CSystemVariableView::Delete()
{
	m_pTable->setModel(nullptr);
}