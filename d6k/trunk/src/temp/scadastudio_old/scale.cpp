#include <QModelIndex>
#include <QMenu>
#include "scale.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "treewidget.h"

/*! \fn CSacle::CSacle(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CSacle::CSacle 
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
CSacle::CSacle(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CSacle::~CSacle()
********************************************************************************************************* 
** \brief CSacle::~CSacle 
** \details 
** \return  
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
CSacle::~CSacle()
{

}

/*! \fn void CSacle::AddScaleGroup()
********************************************************************************************************* 
** \brief CSacle::AddScale 
** \details 
** \return void 
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
void CSacle::AddScaleGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole ���
	int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("ת����").arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(FES_TYPE_ALARM_GROUP_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CSacle::ShowMenu()
********************************************************************************************************* 
** \brief CSacle::AddScaleGroup 
** \details 
** \return void 
** \author gw
** \date 2015��11��22�� 
** \note 
********************************************************************************************************/
void CSacle::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == FES_SCALE_ITEM ||
		m_index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_GROUP_ITEM)
	{
		QAction *pActionAlarm = new QAction(SCALE_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(SCALE_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//���ת��
			AddScale();
		}
		else if(selectedItem == pActionGroup)
		{
			//���ת����
			AddScaleGroup();
		}
	}
	else if(m_index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_CHILD_ITEM)
	{

	}

	pMenu->deleteLater();
}

/*! \fn void CSacle::AddScale()
********************************************************************************************************* 
** \brief CSacle::AddScale 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CSacle::AddScale()
{
	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("ת��").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(FES_TYPE_SCALE_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}