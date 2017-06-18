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
** \details �ر�tabwidgetʱ,model��view�ֿ�
** \return void 
** \author gw
** \date 2016��10��11�� 
** \note 
********************************************************************************************************/
void CSystemVariableView::Delete()
{
	m_pTable->setModel(nullptr);
}