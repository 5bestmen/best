#include <QMenu>
#include "alarm.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "treewidget.h"
/*! \fn CAlarm::CAlarm(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CAlarm::CAlarm 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
CAlarm::CAlarm(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	: QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CAlarm::~CAlarm()
********************************************************************************************************* 
** \brief CAlarm::~CAlarm 
** \details 
** \return  
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
CAlarm::~CAlarm()
{

}

/*! \fn void CAlarm::AddAlarmGroup()
********************************************************************************************************* 
** \brief CAlarm::AddAlarmGroup 
** \details 
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CAlarm::AddAlarmGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("告警组").arg(0);

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

/*! \fn void CAlarm::ShowMenu()
********************************************************************************************************* 
** \brief CAlarm::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CAlarm::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}
	
	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == FES_ALARM_ITEM ||
		m_index.data(Qt::UserRole).toInt() == FES_TYPE_ALARM_GROUP_ITEM)
	{
		QAction *pActionAlarm = new QAction(ALARM_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(ALARM_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//添加报警
			AddAlarm();
		}
		else if(selectedItem == pActionGroup)
		{
			//添加报警组
			AddAlarmGroup();
		}
	}
	else if(m_index.data(Qt::UserRole).toInt() == FES_TYPE_ALARM_CHILD_ITEM)
	{

	}

	pMenu->deleteLater();
}

/*! \fn void CAlarm::AddAlarm()
********************************************************************************************************* 
** \brief CAlarm::AddAlarm 
** \details 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CAlarm::AddAlarm()
{
	//添加通道
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("报警").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(FES_TYPE_ALARM_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}