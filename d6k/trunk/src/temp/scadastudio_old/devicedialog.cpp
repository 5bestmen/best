/*! @file
<PRE>
********************************************************************************
ģ����       :  װ�����
�ļ���       :  devicedialog.cpp
�ļ�ʵ�ֹ��� :  װ�����
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \date 2015��11��24�� 
** \note 
********************************************************************************************************/
CDeviceDialog::CDeviceDialog(QWidget *parent, CScadastudio *pUi, QString strDesc, QString strTableName, int nType)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pUI = pUi;

	//ģ������
	m_pAIView = new CTableView(this, strDesc, nType, ANALOG_INPUT);
	m_pAIView->GetFieldInfo(ANALOG_INPUT);

	//��������
	m_pDIView = new CTableView(this, strDesc, nType, DIGITAL_INPUT);
	m_pDIView->GetFieldInfo(DIGITAL_INPUT);

	//ģ���������
	m_pAOView = new CTableView(this, strDesc, nType, DIGITAL_OUTPUT);
	m_pAOView->GetFieldInfo(DIGITAL_OUTPUT);

	//�����������
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
** \date 2015��11��18�� 
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
** \date 2015��11��18�� 
** \note 
********************************************************************************************************/
void CDeviceDialog::Init()
{
	//����tab
	m_pTabCenter = new QTabWidget(this);

	m_pTabCenter->setTabPosition(QTabWidget::South);

	//ģ������
	m_pTabCenter->addTab(m_pAIView, QStringLiteral("ģ������"));

	m_pTabCenter->addTab(m_pDIView, QStringLiteral("��������"));

	m_pTabCenter->addTab(m_pAOView, QStringLiteral("ģ���������"));

	m_pTabCenter->addTab(m_pDOView, QStringLiteral("�����������"));

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
** \date 2015��11��24�� 
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