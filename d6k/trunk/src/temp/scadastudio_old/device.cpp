/*! @file
<PRE>
********************************************************************************
ģ����       :  װ��
�ļ���       :  devicedialog.cpp
�ļ�ʵ�ֹ��� :  װ��
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
*  @date    2015.11.26
*/

#include <QMenu>
#include "device.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "standarview.h"
#include "tabwidget.h"
#include "treewidget.h"
#include "devicedialog.h"

/*! \fn CDevice::CDevice(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CDevice::CDevice 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CDevice::CDevice(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CDevice::~CDevice
********************************************************************************************************* 
** \brief CDevice::~CDevice 
** \details 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CDevice::~CDevice()
{

}

/*! \fn void CDevice::InitDevice()
********************************************************************************************************* 
** \brief CDevice::InitDevice 
** \details 
** \return void 
** \author gw
** \date 2015��11��24�� 
** \note 
********************************************************************************************************/
void CDevice::InitDevice()
{
	//����װ��
	QString strDesc = m_index.data().toString();

	int nType = m_index.data(Qt::UserRole).toInt();

	CStandardView *pDialog = new CStandardView(NULL, strDesc, FES_DEVICE, nType);

	m_pUi->GetTabWidget()->addTab(pDialog, strDesc);

	pDialog->show();
}

/*! \fn void CDevice::ShowMenu()
********************************************************************************************************* 
** \brief CDevice::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015��11��24�� 
** \note 
********************************************************************************************************/
void CDevice::ShowMenu()
{
	QMenu *pMenu = new QMenu(NULL);
	
	if(m_nMouseClickType == CTreeWidget::RIGHTCLICK)
	{
		QMenu *qMenu = new QMenu(NULL);

		QAction *pActionDevice = new QAction(DEVICE_CONFIGURATION, qMenu);

		qMenu->addAction(pActionDevice);

		QAction* selectedItem = qMenu->exec(QCursor::pos());

		if(selectedItem == pActionDevice)
		{
			//����װ��
			QString strDesc = m_index.data().toString();

			int nType = m_index.data(Qt::UserRole).toInt();

			CStandardView *pDialog = new CStandardView(NULL, strDesc, FES_DEVICE, nType);

			pDialog->show();
		}

		qMenu->deleteLater();
	}
	else if(m_nMouseClickType == CTreeWidget::LEFTDOUBLECLICK)
	{
		QString strDesc = m_index.data().toString();

		int nType = m_index.data().toInt();

		CDeviceDialog *pDevice = new CDeviceDialog(NULL, m_pUi, strDesc, ANALOG_INPUT, nType);

		pDevice->Init();


		pDevice->show();
	}

	pMenu->deleteLater();
}