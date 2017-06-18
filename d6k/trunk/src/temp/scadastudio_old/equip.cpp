/*! @file
<PRE>
********************************************************************************
ģ����       :  �豸
�ļ���       :  equip.cpp
�ļ�ʵ�ֹ��� :  �豸
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
#include "equip.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "treewidget.h"

/*! \fn CEquip::CEquip(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CEquip::CEquip 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
CEquip::CEquip(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;
 
	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CEquip::~CEquip()
********************************************************************************************************* 
** \brief CEquip::~CEquip 
** \details 
** \return  
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
CEquip::~CEquip()
{

}

/*! \fn void CEquip::AddEquip()
********************************************************************************************************* 
** \brief CEquip::AddEquip 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CEquip::AddEquip()
{
	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("�豸").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(EQUIPMENT_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CEquip::AddEquipGroup()
********************************************************************************************************* 
** \brief CEquip::AddEquipGroup 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CEquip::AddEquipGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole ���
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("ת����").arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(EQUIPMENT_GROUP_CHILD_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CEquip::ShowMenu()
********************************************************************************************************* 
** \brief CEquip::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CEquip::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == EQUIPMENT_ITEM ||
		m_index.data(Qt::UserRole).toInt() == EQUIPMENT_GROUP_CHILD_ITEM)
	{
		QAction *pActionAlarm = new QAction(EQUIPMENT_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(EQUIPMENT_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//���ת��
			AddEquip();
		}
		else if(selectedItem == pActionGroup)
		{
			//���ת����
			AddEquipGroup();
		}
	}
	else if(m_index.data(Qt::UserRole).toInt() == EQUIPMENT_CHILD_ITEM)
	{
		QAction *pActionGroup = new QAction(EQUIPMENT_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionGroup)
		{
			//���ת����
			AddEquipGroup();
		}
	}

	pMenu->deleteLater();
}