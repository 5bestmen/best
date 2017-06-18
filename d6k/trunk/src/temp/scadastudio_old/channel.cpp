/*! @file
<PRE>
********************************************************************************
ģ����       :  ͨ��
�ļ���       :  nodeparamodel.cpp
�ļ�ʵ�ֹ��� :  ͨ��
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
*  @date    2015.11.23
*/

#include <QMenu>
#include "channel.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "treewidget.h"
#include "standarview.h"

/*! \fn CChannel::CChannel(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CChannel::CChannel 
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
CChannel::CChannel(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CChannel::~CChannel()
********************************************************************************************************* 
** \brief CChannel::~CChannel 
** \details 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CChannel::~CChannel()
{

}

/*! \fn void CChannel::AddChannel()
********************************************************************************************************* 
** \brief CChannel::AddChannel 
** \details 
** \return void 
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
void CChannel::AddChannel()
{
	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg(QStringLiteral("ͨ��")).arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(FES_CHANNEL_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CChannel::AddChannelGroup()
********************************************************************************************************* 
** \brief CChannel::AddChannelGroup 
** \details 
** \return void 
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
void CChannel::AddChannelGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole ���
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(CHANNEL_GROUP_NAME).arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(FES_CHANNEL_GROUP_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(m_index);
}

#include "tabwidget.h"
/*! \fn void CChannel::ShowMenu()
********************************************************************************************************* 
** \brief CChannel::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
void CChannel::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == FES_CHANNEL_ITEM ||
		m_index.data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
	{
		QAction *pChannelAction = new QAction(CHANNEL_NAME, pMenu);

		pChannelAction->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pChannelAction);


		QAction *pActionGroup = new QAction(CHANNEL_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pChannelAction)
		{
			//���ͨ��
			AddChannel();
		}
		else if(selectedItem == pActionGroup)
		{
			//���ͨ����
			AddChannelGroup();
		}
	}
	else if(m_index.data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
	{
		//װ��
		QAction *pDeviceAction = new QAction(DEVICE_NAME, pMenu);

		pMenu->addAction(pDeviceAction);

		pDeviceAction->setIcon(QIcon(DEVICE_PNG));


		//ͨ������
		QAction *pAction = new QAction(CHANNEL_CONFIGURATION, pMenu);

		pMenu->addAction(pAction);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pDeviceAction)
		{
			//���װ��
			AddDevice();
		}
		else if(selectedItem == pAction)
		{
			//ͨ������
			QString strChannelDesc = m_index.data().toString();

			int nType = m_index.data(Qt::UserRole).toInt();

			CStandardView *pDialog = new CStandardView(NULL, strChannelDesc, FES_CHANNEL, nType);

			m_pUi->GetTabWidget()->addTab(pDialog, "testtest");

			pDialog->show();
		}
	}

	pMenu->deleteLater();
}

/*! \fn void CChannel::AddDevice()
********************************************************************************************************* 
** \brief CChannel::AddDevice 
** \details 
** \return void 
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
void CChannel::AddDevice()
{
	//���װ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("Device").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(FES__CHANNEL_CHILD_DEVICE_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(DEVICE_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}