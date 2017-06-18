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
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/define.h"

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
CUser::CUser(IMainWindow *pUi)
	: QObject(pUi)
{
	m_pUi = pUi;

	connect(pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
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
void CUser::AddUserGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole ���
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(USER_GROUP_NAME).arg(0);

	if (item->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);

		newItem->setData(USER_MANAGE_CHILD_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(index);
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
void CUser::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);

	if (index.data(Qt::UserRole).toInt() == USER_MANAGE_ITEM)
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
			//����û�
			AddUser(index);
		}
		else if(selectedItem == pActionGroup)
		{
			//����û���
			AddUserGroup(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == USER_MANAGE_GROUP_ITEM)
	{
		QAction *pActionAlarm = new QAction(USER_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//���ת��
			AddUser(index);
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
void CUser::AddUser(QModelIndex &index)
{
	//����û�
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg(USER_NAME).arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(USER_MANAGE_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CUser::showMouseRightButton(const QPoint &point)
********************************************************************************************************* 
** \brief CUser::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
void CUser::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == USER_MANAGE_ITEM)
	{
		ShowMenu(indexSelect);
	}
}