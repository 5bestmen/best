/*! @file
<PRE>
********************************************************************************
ģ����       :  ǰ��ģ��
�ļ���       :	fesmodel.cpp
�ļ�ʵ�ֹ��� :  ǰ��ģ��
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
*  @date    2015.11.13
*/

#include <QMenu>
#include <QModelIndex>
#include "fesmodel.h"
#include "quoteItem.h"
#include "define.h"
#include "treewidget.h"
#include "scadastudio.h"
#include "standarview.h"
#include "tabwidget.h"
#include "devicedialog.h"
#include "alarm.h"
#include "scale.h"
#include "variable.h"
#include "channel.h"
#include "device.h"

/*! \fn CFesModel::CFesModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CFesModel::CFesModel 
** \details ǰ�ýڵ�
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
CFesModel::CFesModel(CScadastudio *pUi, CQuoteItem *item)
{
	//Test
	QToolBar *pTest = pUi->CreateToolBarInModel("ǰ��");

	QAction *pAct = pUi->CreateActionInModel("test", pTest, REPORT_PNG);

	connect(pAct, SIGNAL(triggered()), this, SLOT(test()));

	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//ǰ�ýڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("ǰ����̬"));

	item->appendRow(m_pTopFesItem);
	m_pTopFesItem->setEditable(false);

	Init();
}


/*! \fn CFesModel::~CFesModel(void)
********************************************************************************************************* 
** \brief CFesModel::~CFesModel 
** \details 
** \param  
** \return  
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
CFesModel::~CFesModel(void)
{

}

/*! \fn void CFesModel::Init()
********************************************************************************************************* 
** \brief CFesModel::init 
** \details ��ʼ��FES����
** \return void 
** \author gw
** \date 2015��10��23�� 
** \note 
********************************************************************************************************/
void CFesModel::Init()
{
	CreateOneLevelNode();
}

/*! \fn void CFesModel::CreateOneLevelNode()
********************************************************************************************************* 
** \brief CFesModel::createOneLevelNode 
** \details 
** \return void 
** \author gw
** \date 2015��10��26�� 
** \note 
********************************************************************************************************/
void CFesModel::CreateOneLevelNode()
{
	//ͨ��
	CQuoteItem *itemChannel = new CQuoteItem(QStringLiteral("ͨ��"));
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(":images/channel.png"));
	itemChannel->setEditable(false);

	m_pTopFesItem->appendRow(itemChannel);

	//����
	CQuoteItem *itemAlarm = new CQuoteItem(QStringLiteral("����"));
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(":images/alarm.png"));
	itemAlarm->setEditable(false);

	m_pTopFesItem->appendRow(itemAlarm);

	//ת����ϵ
	CQuoteItem *itemScale = new CQuoteItem(QStringLiteral("ת��"));
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	itemScale->setIcon(QIcon(":images/scale.png"));
	itemScale->setEditable(false);

	m_pTopFesItem->appendRow(itemScale);

	//����
	CQuoteItem *itemVariableList = new CQuoteItem(QStringLiteral("����"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemVariableList->setEditable(false);
	
	m_pTopFesItem->appendRow(itemVariableList);

	//ϵͳ����
	CQuoteItem *itemSysVariableList = new CQuoteItem(QStringLiteral("ϵͳ����"));
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemSysVariableList->setEditable(false);

	itemVariableList->appendRow(itemSysVariableList);

	//�û�����
	CQuoteItem *itemUserVariableList = new CQuoteItem(QStringLiteral("�û�����"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemUserVariableList->setEditable(false);

	//�û�root
	CQuoteItem *itemUserVariableRoot = new CQuoteItem(QStringLiteral("root"));
	itemUserVariableRoot->setData(FES_USER_VARIBALE_ROOT_ITEM, Qt::UserRole);
	itemUserVariableRoot->setIcon(QIcon(":images/variablelist.png"));
	itemUserVariableRoot->setEditable(false);

	itemUserVariableList->appendRow(itemUserVariableRoot);

	itemVariableList->appendRow(itemUserVariableList);

	//��ʷ�洢
	CQuoteItem *itemHis = new CQuoteItem(QStringLiteral("��ʷ�洢"));
	itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	itemHis->setIcon(QIcon(":images/hissave.png"));
	itemHis->setEditable(false);

	///////////////////////////////////////////////////////////////////
	CQuoteItem *itemHisTmp = new CQuoteItem(QStringLiteral("ģ�����������ݱ���"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ�����������ݱ���"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ������ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ����Խ��ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ�����ۼ�ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("�������ۼ�ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("������ۼ�ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	///////////////////////////////////////////////////////////////////



	m_pTopFesItem->appendRow(itemHis);

	//�¹�׷��
	CQuoteItem *itemPdr = new CQuoteItem(QStringLiteral("�¹�׷��"));
	itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	itemPdr->setIcon(QIcon(":images/pdr.png"));
	itemPdr->setEditable(false);

	m_pTopFesItem->appendRow(itemPdr);
}

/*! \fn void CFesModel::GetFesType(int type, QModelIndex &indexSelect, int mouseClickType)
********************************************************************************************************* 
** \brief CFesModel::getFesType 
** \details ���ͨ��ͨ���� CTreeWidget::sendFesType
** \param type 
** \return void 
** \author gw
** \date 2015��10��26�� 
** \note 
********************************************************************************************************/
void CFesModel::GetFesType(int nType, QModelIndex &indexSelect, int mouseClickType)
{
	if(nType == FES_ALARM_ITEM || nType == FES_TYPE_ALARM_CHILD_ITEM || nType == FES_TYPE_ALARM_GROUP_ITEM)
	{
		//�澯
		CAlarm *pAlarm = new CAlarm(this, m_pUi, indexSelect, mouseClickType);

		pAlarm->ShowMenu();

		pAlarm->deleteLater();
	}
	else if(nType == FES_SCALE_ITEM || nType == FES_TYPE_SCALE_CHILD_ITEM || nType == FES_TYPE_SCALE_GROUP_ITEM)
	{
		//ת��
		CSacle *pScale = new CSacle(this, m_pUi, indexSelect, mouseClickType);

		pScale->ShowMenu();

		pScale->deleteLater();
	}
	else if(nType == FES_USER_VARIABLE_ITEM)
	{
		//�û�����
		CVariable *pVariable = new CVariable(this, m_pUi, indexSelect, mouseClickType);

		pVariable->ShowMenu();

		pVariable->deleteLater();
	}
	else if(nType == FES_CHANNEL_ITEM || nType == FES_CHANNEL_GROUP_ITEM || nType == FES_CHANNEL_CHILD_ITEM)
	{
		//ͨ��
		CChannel *pChannel = new CChannel(this, m_pUi, indexSelect, mouseClickType);

		pChannel->ShowMenu();

		pChannel->deleteLater();
	}
	else if(nType == FES__CHANNEL_CHILD_DEVICE_ITEM)
	{
		//װ�õ��
		CDevice *pDevice = new CDevice(this, m_pUi, indexSelect, mouseClickType);

		//pDevice->InitDevice();

		pDevice->ShowMenu();
	}
	
	return;
	
	if(nType == FES_CHANNEL_ITEM || nType == FES_CHANNEL_GROUP_ITEM)
	{
		if(mouseClickType != CTreeWidget::LEFTDOUBLECLICK)
		{
			//ͨ���� ͨ������ ���menu
			QMenu *qMenu = new QMenu(NULL);

			QAction *pActionChannel =AddChannelAction(qMenu);

			qMenu->addAction(pActionChannel);

			QAction *pActionGroup = AddChannelGroupAction(qMenu);

			qMenu->addAction(pActionGroup);

			QAction* selectedItem = qMenu->exec(QCursor::pos());

			if(selectedItem == pActionChannel)
			{
				//���ͨ��
				AddChannel(indexSelect, nType);
			}
			else if(selectedItem == pActionGroup)
			{
				//���ͨ����
				AddChannelGroup(indexSelect, nType);
			}

			qMenu->deleteLater();
		}
	}
	else if(nType == FES_CHANNEL_CHILD_ITEM)
	{	
		if(mouseClickType != CTreeWidget::RIGHTCLICK)
		{
			QString strTmp = indexSelect.data().toString();

			QString strGroup;

			GetGroup(indexSelect, strGroup);

			//���channel dialog��tabwidget
			AddChannelDialog(strTmp, strGroup);
		}
		else
		{
			QMenu *qMenu = new QMenu(NULL);

			QAction *pActionDevice = AddDeviceAction(qMenu);
			
			QAction *pActionChannelPara = AddAction(qMenu, CHANNEL_CONFIGURATION, "");
				
			qMenu->addAction(pActionDevice);

			QAction* selectedItem = qMenu->exec(QCursor::pos());

			if(selectedItem == pActionDevice)
			{
				//���װ��
				AddDevice(indexSelect, nType);
			}
			else if(selectedItem == pActionChannelPara)
			{
				//����ͨ��
				QString strChannelDesc = indexSelect.data().toString();
				
				CStandardView *pDialog = new CStandardView(NULL, strChannelDesc, FES_CHANNEL, nType);

				pDialog->show();

				//pDialog->deleteLater();
			}

			qMenu->deleteLater();
		}
	}
	else if(nType == FES__CHANNEL_CHILD_DEVICE_ITEM)
	{
		if(mouseClickType == CTreeWidget::LEFTDOUBLECLICK)
		{
			QString strDesc = indexSelect.data().toString();


			CDeviceDialog *pDevice = new CDeviceDialog(NULL, m_pUI, strDesc, ANALOG_INPUT, nType);


			pDevice->show();
		}
		else if(mouseClickType == CTreeWidget::RIGHTCLICK)
		{
			QMenu *qMenu = new QMenu(NULL);

			QAction *pActionDevice = AddAction(qMenu, "����װ��", "");

			qMenu->addAction(pActionDevice);
			
			QAction* selectedItem = qMenu->exec(QCursor::pos());

			if(selectedItem == pActionDevice)
			{
				//����װ��
				QString strDesc = indexSelect.data().toString();

				CStandardView *pDialog = new CStandardView(NULL, strDesc, FES_DEVICE, nType);

				pDialog->show();
			}

			qMenu->deleteLater();
		}
		


		//pDevice->deleteLater();
	}
	else
	{
		return;
	}
}

/*! \fn QAction *CFesModel::AddChannelAction(QMenu *pMenu)
********************************************************************************************************* 
** \brief CFesModel::AddChannelAction 
** \details 
** \param pMenu 
** \return QAction * 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
QAction *CFesModel::AddChannelAction(QMenu *pMenu)
{
	QAction *pAction = new QAction(CHANNEL_NAME, pMenu);

	pAction->setIcon(QIcon(CHANNEL_PNG));

	pMenu->addAction(pAction);

	return pAction;
}

/*! \fn QAction *CFesModel::AddChannelGroupAction(QMenu *pMenu)
********************************************************************************************************* 
** \brief CFesModel::AddChannelGroupAction 
** \details 
** \param pMenu 
** \return QAction * 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
QAction *CFesModel::AddChannelGroupAction(QMenu *pMenu)
{
	QAction *pAction = new QAction(GROUP_NAME, pMenu);

	pAction->setIcon(QIcon(CLOSE_GROUP_PNG));

	pMenu->addAction(pAction);

	return pAction;
}

/*! \fn void CFesModel::AddChannel(QModelIndex &index, int nType)
********************************************************************************************************* 
** \brief CFesModel::AddChannel 
** \details 
** \param index 
** \param nType 
** \return void 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
void CFesModel::AddChannel(QModelIndex &index, int nType)
{
	if(index.data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
	{
		return;
	}

	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pModel->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("test").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(FES_CHANNEL_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pTree->expand(index);
}

/*! \fn void CFesModel::AddChannelGroup(QModelIndex &index, int nType)
********************************************************************************************************* 
** \brief CFesModel::AddChannelGroup 
** \details 
** \param index 
** \param nType 
** \return void 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
void CFesModel::AddChannelGroup(QModelIndex &index, int nType)
{
	CQuoteItem *item = (CQuoteItem *)m_pModel->itemFromIndex(index);

	//Qt::WhatsThisRole ���
	int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("Group").arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(FES_CHANNEL_GROUP_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pTree->expand(index);
}

/*! \fn void CFesModel::AddChannelDialog(QString strChannelName, QString strGroup)
********************************************************************************************************* 
** \brief CFesModel::addChannelDialog 
** \details ���ͨ������
** \param channelName 
** \return void 
** \author MouMou
** \date 2015��10��30�� 
** \note 
********************************************************************************************************/
void CFesModel::AddChannelDialog(QString strChannelName, QString strGroup)
{
	////���channel dialog��tabwidget
	//CChannelView *pView = new CChannelView(m_pTabWidget, strChannelName, FES_TYPE_CHANNEL_ITEM, FES_CHANNEL, strGroup);

	//pView->GetFieldInfo();

	//pView->GetTableName(FES_CHANNEL);

	//m_pTabWidget->addTab(pView, strChannelName);

	//m_pTabWidget->setCurrentWidget(pView);
}

/*! \fn void CFesModel::GetGroup(QModelIndex &index, QString &strGroup)
********************************************************************************************************* 
** \brief CFesModel::getGroup 
** \details 
** \param index 
** \param group ��
** \return void 
** \author gw
** \date 2015��11��3�� 
** \note 
********************************************************************************************************/
void CFesModel::GetGroup(QModelIndex &index, QString &strGroup)
{
	if(!strGroup.isEmpty())
	{
		strGroup += ".";
	}

	if(index.parent().isValid() && index.parent().data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
	{
		QString strTmp;

		strTmp = index.parent().data().toString();

		strGroup += strTmp;

		GetGroup(index.parent(), strGroup);
	}
	else
	{
		
	}
}

/*! \fn void CFesModel::AddDevice(QModelIndex &index, int nType)
********************************************************************************************************* 
** \brief CFesModel::AddDevice 
** \details ���װ��
** \param index 
** \param nType 
** \return void 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
void CFesModel::AddDevice(QModelIndex &index, int nType)
{
	//���װ��
	CQuoteItem *item = (CQuoteItem *)m_pModel->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("Device").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(FES__CHANNEL_CHILD_DEVICE_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(DEVICE_PNG));

	item->appendRow(newItem);

	m_pTree->expand(index);
}

/*! \fn void CFesModel::AddDeviceAction(QMenu *pMenu)
********************************************************************************************************* 
** \brief CFesModel::AddDeviceAction 
** \details 
** \param pMenu 
** \return void 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
QAction *CFesModel::AddDeviceAction(QMenu *pMenu)
{
	QAction *pAction = new QAction(DEVICE_NAME, pMenu);

	pAction->setIcon(QIcon(DEVICE_PNG));

	pMenu->addAction(pAction);

	return pAction;
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesModel::AddAction 
** \details 
** \param pMenu 
** \param strName 
** \param strIcon 
** \return QAction * 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
QAction *CFesModel::AddAction(QMenu *pMenu, QString strName, QString strIcon)
{
	QAction *pAction = new QAction(strName, pMenu);

	pAction->setIcon(QIcon(strIcon));

	pMenu->addAction(pAction);

	return pAction;
}