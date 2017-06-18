/*! @file
<PRE>
********************************************************************************
模块名       :  设备
文件名       :  equip.cpp
文件实现功能 :  设备
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
** \date 2015年11月22日 
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
** \date 2015年11月22日 
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CEquip::AddEquip()
{
	//添加通道
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("设备").arg(0);

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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CEquip::AddEquipGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("转换组").arg(0);

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
** \date 2015年11月26日 
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
			//添加转换
			AddEquip();
		}
		else if(selectedItem == pActionGroup)
		{
			//添加转换组
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
			//添加转换组
			AddEquipGroup();
		}
	}

	pMenu->deleteLater();
}