/*! @file
<PRE>
********************************************************************************
ģ����       :  ����
�ļ���       :  variable.cpp
�ļ�ʵ�ֹ��� :  ����
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
#include <QModelIndex>
#include <QMenu>
#include "variable.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"
#include "treewidget.h"

/*! \fn CVariable::CVariable(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CVariable::CVariable 
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
CVariable::CVariable(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CVariable::~CVariable()
********************************************************************************************************* 
** \brief CVariable::~CVariable 
** \details 
** \return  
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
CVariable::~CVariable()
{

}

/*! \fn void CVariable::ShowMenu()
********************************************************************************************************* 
** \brief CVariable::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
void CVariable::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == FES_USER_VARIABLE_ITEM)
	{
		QAction *pActionGroup = new QAction(VARIABLE_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionGroup)
		{
			//����û���
			AddUserGroup();
		}
	}
	/*else if(m_index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_CHILD_ITEM)
	{

	}*/

	pMenu->deleteLater();
}

/*! \fn void CVariable::AddUserGroup()
********************************************************************************************************* 
** \brief CVariable::AddUserGroup 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CVariable::AddUserGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole ���
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("�û�������").arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(FES_TYPE_VARIABLE_GROUP_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(m_index);
}