/*! @file
<PRE>
********************************************************************************
模块名       :  装置配点
文件名       :  devicedialog.cpp
文件实现功能 :  装置配点
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.18
*/

#include "devicedialog.h"
#include "tableview.h"
#include "scadastudio.h"
#include "tabwidget.h"

/*! \fn 
********************************************************************************************************* 
** \brief CDeviceDialog::CDeviceDialog 
** \details 
** \param parent 
** \param pUi 
** \param strDesc 
** \param strTableName 
** \param nType 
** \return  
** \author gw
** \date 2015年11月24日 
** \note 
********************************************************************************************************/
CDeviceDialog::CDeviceDialog(QWidget *parent, CScadastudio *pUi, QString strDesc, QString strTableName, int nType)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pUI = pUi;

	//模拟量表
	m_pAIView = new CTableView(this, strDesc, nType, ANALOG_INPUT);
	m_pAIView->GetFieldInfo(ANALOG_INPUT);

	//数字量表
	m_pDIView = new CTableView(this, strDesc, nType, DIGITAL_INPUT);
	m_pDIView->GetFieldInfo(DIGITAL_INPUT);

	//模拟量输出表
	m_pAOView = new CTableView(this, strDesc, nType, DIGITAL_OUTPUT);
	m_pAOView->GetFieldInfo(DIGITAL_OUTPUT);

	//开关量输出表
	m_pDOView = new CTableView(this, strDesc, nType, ANALOG_OUTPUT);
	m_pDOView->GetFieldInfo(ANALOG_OUTPUT);
	
	Init();
}

/*! \fn CDeviceDialog::~CDeviceDialog()
********************************************************************************************************* 
** \brief CDeviceDialog::~CDeviceDialog 
** \details 
** \return  
** \author gw
** \date 2015年11月18日 
** \note 
********************************************************************************************************/
CDeviceDialog::~CDeviceDialog()
{

}

/*! \fn void CDeviceDialog::Init()
********************************************************************************************************* 
** \brief CDeviceDialog::Init 
** \details 
** \return void 
** \author gw
** \date 2015年11月18日 
** \note 
********************************************************************************************************/
void CDeviceDialog::Init()
{
	//中心tab
	m_pTabCenter = new QTabWidget(this);

	m_pTabCenter->setTabPosition(QTabWidget::South);

	//模拟量表
	m_pTabCenter->addTab(m_pAIView, QStringLiteral("模拟量表"));

	m_pTabCenter->addTab(m_pDIView, QStringLiteral("开关量表"));

	m_pTabCenter->addTab(m_pAOView, QStringLiteral("模拟量输出表"));

	m_pTabCenter->addTab(m_pDOView, QStringLiteral("开关量输出表"));

	setCentralWidget(m_pTabCenter);

	m_pUI->GetTabWidget();

	m_pUI->GetTabWidget()->addTab(this, "test");
}

/*! \fn void CDeviceDialog::closeEvent(QCloseEvent * event)
********************************************************************************************************* 
** \brief CDeviceDialog::closeEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月24日 
** \note 
********************************************************************************************************/
void CDeviceDialog::closeEvent(QCloseEvent * event)
{
	if(event == NULL)
	{
		return;
	}
	
	this->deleteLater();
}