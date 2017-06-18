/*! @file
<PRE>
********************************************************************************
ģ����       :  �û�
�ļ���       :  user.cpp
�ļ�ʵ�ֹ��� :  �û�
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> ͨ�� װ��
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

#include <QModelIndex>
#include <QMenu>
#include "user.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "treewidget.h"

/*! \fn CUser::CUser(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CUser::CUser 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CUser::CUser(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CUser::~CUser()
********************************************************************************************************* 
** \brief CUser::~CUser 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CUser::~CUser()
{

}

/*! \fn void CUser::AddUserGroup()
********************************************************************************************************* 
** \brief CUser::AddUserGroup 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CUser::AddUserGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole ���
	int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("�û���").arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(USER_GROUP_CHILD_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CUser::ShowMenu()
********************************************************************************************************* 
** \brief CUser::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CUser::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == USER_ITEM)
	{
		QAction *pActionAlarm = new QAction(USER_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(USER_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//���ת��
			AddUser();
		}
		else if(selectedItem == pActionGroup)
		{
			//���ת����
			AddUserGroup();
		}
	}
	else if(m_index.data(Qt::UserRole).toInt() == USER_GROUP_CHILD_ITEM)
	{
		QAction *pActionAlarm = new QAction(USER_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//���ת��
			AddUser();
		}
	}

	pMenu->deleteLater();
}

/*! \fn void CUser::AddUser()
********************************************************************************************************* 
** \brief CUser::AddUser 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CUser::AddUser()
{
	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("�û�").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(USER_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}