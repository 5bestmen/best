/*! @file
<PRE>
********************************************************************************
模块名       :  前置模块
文件名       :	fesmodel.cpp
文件实现功能 :  前置模块
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
** \details 前置节点
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CFesModel::CFesModel(CScadastudio *pUi, CQuoteItem *item)
{
	//Test
	QToolBar *pTest = pUi->CreateToolBarInModel("前置");

	QAction *pAct = pUi->CreateActionInModel("test", pTest, REPORT_PNG);

	connect(pAct, SIGNAL(triggered()), this, SLOT(test()));

	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//前置节点
	m_pTopFesItem = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("前置组态"));

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
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CFesModel::~CFesModel(void)
{

}

/*! \fn void CFesModel::Init()
********************************************************************************************************* 
** \brief CFesModel::init 
** \details 初始化FES界面
** \return void 
** \author gw
** \date 2015年10月23日 
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
** \date 2015年10月26日 
** \note 
********************************************************************************************************/
void CFesModel::CreateOneLevelNode()
{
	//通道
	CQuoteItem *itemChannel = new CQuoteItem(QStringLiteral("通道"));
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(":images/channel.png"));
	itemChannel->setEditable(false);

	m_pTopFesItem->appendRow(itemChannel);

	//报警
	CQuoteItem *itemAlarm = new CQuoteItem(QStringLiteral("报警"));
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(":images/alarm.png"));
	itemAlarm->setEditable(false);

	m_pTopFesItem->appendRow(itemAlarm);

	//转换关系
	CQuoteItem *itemScale = new CQuoteItem(QStringLiteral("转换"));
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	itemScale->setIcon(QIcon(":images/scale.png"));
	itemScale->setEditable(false);

	m_pTopFesItem->appendRow(itemScale);

	//变量
	CQuoteItem *itemVariableList = new CQuoteItem(QStringLiteral("变量"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemVariableList->setEditable(false);
	
	m_pTopFesItem->appendRow(itemVariableList);

	//系统变量
	CQuoteItem *itemSysVariableList = new CQuoteItem(QStringLiteral("系统变量"));
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemSysVariableList->setEditable(false);

	itemVariableList->appendRow(itemSysVariableList);

	//用户变量
	CQuoteItem *itemUserVariableList = new CQuoteItem(QStringLiteral("用户变量"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemUserVariableList->setEditable(false);

	//用户root
	CQuoteItem *itemUserVariableRoot = new CQuoteItem(QStringLiteral("root"));
	itemUserVariableRoot->setData(FES_USER_VARIBALE_ROOT_ITEM, Qt::UserRole);
	itemUserVariableRoot->setIcon(QIcon(":images/variablelist.png"));
	itemUserVariableRoot->setEditable(false);

	itemUserVariableList->appendRow(itemUserVariableRoot);

	itemVariableList->appendRow(itemUserVariableList);

	//历史存储
	CQuoteItem *itemHis = new CQuoteItem(QStringLiteral("历史存储"));
	itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	itemHis->setIcon(QIcon(":images/hissave.png"));
	itemHis->setEditable(false);

	///////////////////////////////////////////////////////////////////
	CQuoteItem *itemHisTmp = new CQuoteItem(QStringLiteral("模拟量报表数据保存"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("模拟量曲线数据保存"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("模拟量最值统计"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("模拟量越限值统计"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("模拟量累计值统计"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("开关量累计值统计"));
	itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("电度量累计值统计"));
	itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(":images/hissave.png"));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	///////////////////////////////////////////////////////////////////



	m_pTopFesItem->appendRow(itemHis);

	//事故追忆
	CQuoteItem *itemPdr = new CQuoteItem(QStringLiteral("事故追忆"));
	itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	itemPdr->setIcon(QIcon(":images/pdr.png"));
	itemPdr->setEditable(false);

	m_pTopFesItem->appendRow(itemPdr);
}

/*! \fn void CFesModel::GetFesType(int type, QModelIndex &indexSelect, int mouseClickType)
********************************************************************************************************* 
** \brief CFesModel::getFesType 
** \details 添加通道通道组 CTreeWidget::sendFesType
** \param type 
** \return void 
** \author gw
** \date 2015年10月26日 
** \note 
********************************************************************************************************/
void CFesModel::GetFesType(int nType, QModelIndex &indexSelect, int mouseClickType)
{
	if(nType == FES_ALARM_ITEM || nType == FES_TYPE_ALARM_CHILD_ITEM || nType == FES_TYPE_ALARM_GROUP_ITEM)
	{
		//告警
		CAlarm *pAlarm = new CAlarm(this, m_pUi, indexSelect, mouseClickType);

		pAlarm->ShowMenu();

		pAlarm->deleteLater();
	}
	else if(nType == FES_SCALE_ITEM || nType == FES_TYPE_SCALE_CHILD_ITEM || nType == FES_TYPE_SCALE_GROUP_ITEM)
	{
		//转换
		CSacle *pScale = new CSacle(this, m_pUi, indexSelect, mouseClickType);

		pScale->ShowMenu();

		pScale->deleteLater();
	}
	else if(nType == FES_USER_VARIABLE_ITEM)
	{
		//用户变量
		CVariable *pVariable = new CVariable(this, m_pUi, indexSelect, mouseClickType);

		pVariable->ShowMenu();

		pVariable->deleteLater();
	}
	else if(nType == FES_CHANNEL_ITEM || nType == FES_CHANNEL_GROUP_ITEM || nType == FES_CHANNEL_CHILD_ITEM)
	{
		//通道
		CChannel *pChannel = new CChannel(this, m_pUi, indexSelect, mouseClickType);

		pChannel->ShowMenu();

		pChannel->deleteLater();
	}
	else if(nType == FES__CHANNEL_CHILD_DEVICE_ITEM)
	{
		//装置点表
		CDevice *pDevice = new CDevice(this, m_pUi, indexSelect, mouseClickType);

		//pDevice->InitDevice();

		pDevice->ShowMenu();
	}
	
	return;
	
	if(nType == FES_CHANNEL_ITEM || nType == FES_CHANNEL_GROUP_ITEM)
	{
		if(mouseClickType != CTreeWidget::LEFTDOUBLECLICK)
		{
			//通道组 通道定义 添加menu
			QMenu *qMenu = new QMenu(NULL);

			QAction *pActionChannel =AddChannelAction(qMenu);

			qMenu->addAction(pActionChannel);

			QAction *pActionGroup = AddChannelGroupAction(qMenu);

			qMenu->addAction(pActionGroup);

			QAction* selectedItem = qMenu->exec(QCursor::pos());

			if(selectedItem == pActionChannel)
			{
				//添加通道
				AddChannel(indexSelect, nType);
			}
			else if(selectedItem == pActionGroup)
			{
				//添加通道组
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

			//添加channel dialog到tabwidget
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
				//添加装置
				AddDevice(indexSelect, nType);
			}
			else if(selectedItem == pActionChannelPara)
			{
				//配置通道
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

			QAction *pActionDevice = AddAction(qMenu, "配置装置", "");

			qMenu->addAction(pActionDevice);
			
			QAction* selectedItem = qMenu->exec(QCursor::pos());

			if(selectedItem == pActionDevice)
			{
				//配置装置
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
** \date 2015年11月16日 
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
** \date 2015年11月16日 
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
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CFesModel::AddChannel(QModelIndex &index, int nType)
{
	if(index.data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
	{
		return;
	}

	//添加通道
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
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CFesModel::AddChannelGroup(QModelIndex &index, int nType)
{
	CQuoteItem *item = (CQuoteItem *)m_pModel->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
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
** \details 添加通道数据
** \param channelName 
** \return void 
** \author MouMou
** \date 2015年10月30日 
** \note 
********************************************************************************************************/
void CFesModel::AddChannelDialog(QString strChannelName, QString strGroup)
{
	////添加channel dialog到tabwidget
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
** \param group 组
** \return void 
** \author gw
** \date 2015年11月3日 
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
** \details 添加装置
** \param index 
** \param nType 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CFesModel::AddDevice(QModelIndex &index, int nType)
{
	//添加装置
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
** \date 2015年11月16日 
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
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
QAction *CFesModel::AddAction(QMenu *pMenu, QString strName, QString strIcon)
{
	QAction *pAction = new QAction(strName, pMenu);

	pAction->setIcon(QIcon(strIcon));

	pMenu->addAction(pAction);

	return pAction;
}