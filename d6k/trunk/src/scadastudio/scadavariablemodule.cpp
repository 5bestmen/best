/*! @file
<PRE>
********************************************************************************
模块名       :  SCADA变量模块
文件名       :  scadavariablemodule.cpp
文件实现功能 :  SCADA变量
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
*  @date    2016.12.6
*/

#include "scadavariablemodule.h"
#include "scadastudio/icore.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/iprojmgr.h"
#include "scadastudio/base.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/quoteItem.h"
#include "scadavirable_data.h"
#include "scadasystemvariableview.h"
#include "scadavirable_data.h"
#include "scadauservariableview.h"
#include "scadaanalogalarmview.h"
#include "scadaanalogalarmlimitview.h"
#include "scadadigitalalarmlimitview.h"
#include "scadadigitalalarmview.h"
#include "scadalinearscaleview.h"
#include "scadanonlinearscaleview.h"
#include "scadaview.h"
#include "global.h"

#include <QDir>
#include <QDomDocument>
#include <QMenu>
#include <QMessageBox>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

CScadaVariableModule::CScadaVariableModule(IMainModuleInterface *pCore)
	:m_pScadaVirableConfig(nullptr), m_pCore(nullptr), m_pHash(nullptr), m_pStringPoolVec(nullptr), m_pDescStringPoolOccNo(nullptr)
{
	m_pScadaVirableConfig = new Config::CScadaVariableConfigData;

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	connect(m_pCore->GetUIMgr()->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(m_pCore->GetUIMgr()->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(m_pCore->GetUIMgr()->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
}

CScadaVariableModule::~CScadaVariableModule()
{
	if (m_pScadaVirableConfig)
	{
		delete m_pScadaVirableConfig;
		m_pScadaVirableConfig = nullptr;
	}
}

/*! \fn void CScadaVariableModule::Init(IMainModuleInterface *pUi)
********************************************************************************************************* 
** \brief CScadaVariableModule::Init 
** \details 初始化
** \param pUi 
** \return void 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::Init(IMainModuleInterface *pCore)
{
	Q_UNUSED(pCore);
}

/*! \fn void CScadaVariableModule::UnInit()
********************************************************************************************************* 
** \brief CScadaVariableModule::UnInit 
** \details 
** \return void 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::UnInit()
{

}

/*! \fn bool CScadaVariableModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CScadaVariableModule::SaveProject 
** \details 保存工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	QDomNode n = pRoot->firstChild();

	{
		//if (m_arrFes[i]->GetModifyFlag())
		{
			while (!n.isNull())
			{
				QDomElement e = n.toElement(); // try to convert the node to an element.
				if (!e.isNull())
				{
					QString strTmp = e.tagName();// the node really is an element.


					if (strTmp.compare("scadavariable") == 0)
					{
						if (e.hasChildNodes())
						{
							QDomNodeList list = e.childNodes();
							//auto nSize = list.size();

							while (!list.isEmpty())
							{
								QDomNode node = list.at(0);
								node.parentNode().removeChild(node);
							}
						}

						//保存前置工程文件
						m_pScadaVirableConfig->SaveProjectScadavarialbeNode(pXml, e, szRoot, m_pHash, m_pStringPoolVec, m_pDescStringPoolOccNo);

						//m_pConfig->SaveFesFiles(pXml, e, szRoot, m_pCore);
					}
				}

				n = n.nextSibling();
			}
		}
	}

	
	return true;
}

/*! \fn bool CScadaVariableModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CScadaVariableModule::LoadProject 
** \details 加载工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(szRoot);
	Q_UNUSED(pRoot);
	Q_UNUSED(pXml);

	QDomNode n = m_pCore->GetProjMgr()->GetDocument()->documentElement().firstChild();
	
	QString strPath = "";

	m_pScadaVirableConfig->Clear();
	m_pScadaVirableConfig->Create();

	while (!n.isNull())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if (!e.isNull())
		{
			QString strTmp = e.tagName();// the node really is an element.

			if (strTmp.compare("scadavariable") == 0)
			{
				m_pScadaVirableConfig->LoadProjectScadaNode(e, szRoot);
			}
		}

		n = n.nextSibling();
	}

	//创建UI
	CreateNewUI();

	return true;
}

/*! \fn void CScadaVariableModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
********************************************************************************************************* 
** \brief CScadaVariableModule::CreateNewProject 
** \details 创建工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param pTopItem 
** \return void 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	Q_UNUSED(pTopItem);
	
	QDir dir;
	bool ok = dir.mkdir(szRoot + "/" + "scadavariable");
	if (!ok)
	{
		auto strError = QString(tr("%1/scadavariable folder mkdir failed!!!")).arg(szRoot);

		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
	}

	/////////////////////////////delete tab widget/////////////////////////////////////////////////////////
	auto &mapWidgetInfo = m_pCore->GetUIMgr()->GetTabWidget()->GetWidgetInfo();
	auto pTabWidget = m_pCore->GetUIMgr()->GetTabWidget();
	for (int i = pTabWidget->count(); i > 0; --i)
	{
		//pTabWidget->setCurrentIndex(i - 1);
		auto pWidget = pTabWidget->widget(i - 1);
		if (mapWidgetInfo.contains(pWidget))
		{
			IBase *pBase = dynamic_cast<IBase *>(pWidget);
			Q_ASSERT(pBase);
			if (!pBase)
			{
				auto strError = tr("tab widget close wrong!!!");
				m_pCore->LogMsg("tab widget", strError.toStdString().c_str(), LEVEL_1);

				return;
			}
			pBase->Delete();

			pTabWidget->removeTab(i - 1);

			delete pWidget;
			pWidget = nullptr;
		}
	}
	pTabWidget->ClearMap();
	/////////////////////////////////////////////////////////////////////////////////////////

	//如果已加载数据,需清空
	m_pScadaVirableConfig->Clear();
	m_pScadaVirableConfig->Create();

	QDomElement root = pXml->createElement("scadavariable");
	pRoot->appendChild(root);

	//[0]
	//ScadaViariableConfig
	CQuoteItem *pNodeItem = new CQuoteItem(tr("ScadaVariableConfig"));
	pNodeItem->setData(SCADAVARIABLE_CONFIG, Qt::UserRole);
	pNodeItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(m_pScadaVirableConfig->m_pGroup);
	if (!m_pScadaVirableConfig->m_pGroup)
	{
		return;
	}
	pNodeItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pGroup), POINTERDATA);
	pNodeItem->setIcon(QIcon(FES_PNG));
	pNodeItem->setEditable(false);

	////[1]
	////变量节点
	//CQuoteItem *pVariableItem = new CQuoteItem(tr("variable"));
	//pVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE, Qt::UserRole);
	////pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaUserGroup);
	//if (!m_pScadaVirableConfig->m_pScadaUserGroup)
	//{
	//	return;
	//}
	////pVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaUserGroup), POINTERDATA);
	//pVariableItem->setIcon(QIcon(FES_PNG));
	//pVariableItem->setEditable(false);
	//pNodeItem->appendRow(pVariableItem);
	//
	////变量节点_系统变量
	//CQuoteItem *pVariableSystemVariableItem = new CQuoteItem(tr("system"));
	//pVariableSystemVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_SYSTEM, Qt::UserRole);
	////pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//pVariableSystemVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig), POINTERDATA);
	//pVariableSystemVariableItem->setIcon(QIcon(FES_PNG));
	//pVariableSystemVariableItem->setEditable(false);
	//pVariableItem->appendRow(pVariableSystemVariableItem);

	////变量节点_用户变量
	//CQuoteItem *pVariableUserVariableItem = new CQuoteItem(tr("user"));
	//pVariableUserVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER, Qt::UserRole);
	//pVariableUserVariableItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig);
	//if (!m_pScadaVirableConfig)
	//{
	//	return;
	//}
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaUserGroup);
	//if (!m_pScadaVirableConfig->m_pScadaUserGroup)
	//{
	//	return;
	//}
	//pVariableUserVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig), POINTERDATA);
	//pVariableUserVariableItem->setIcon(QIcon(FES_PNG));
	//pVariableUserVariableItem->setEditable(false);
	//pVariableItem->appendRow(pVariableUserVariableItem);

	////[1]
	////告警节点
	//CQuoteItem *pAlarm = new CQuoteItem(tr("alarm"));
	//pAlarm->setData(SCADAVARIABLE_ITEM_ALARM, Qt::UserRole);
	////pFesItem->setData(ROT_GROUP, GROUP_NUMBER_ROLE);
	////pFesItem->setData(reinterpret_cast<long long>(m_pConfig->m_pFesGroup), POINTERDATA);
	//pAlarm->setIcon(QIcon(FES_PNG));
	//pAlarm->setEditable(false);

	////模拟量报警
	//CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
	//itemAnalogAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM, Qt::UserRole);
	//itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup);
	//if (!m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup)
	//{
	//	return;
	//}
	//itemAnalogAlarm->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup), POINTERDATA);
	//itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
	//itemAnalogAlarm->setEditable(false);
	//pAlarm->appendRow(itemAnalogAlarm);

	////开关量报警
	//CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
	//itemDigtalAlarm->setData(SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM, Qt::UserRole);
	//itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup);
	//if (!m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup)
	//{
	//	return;
	//}
	//itemDigtalAlarm->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup), POINTERDATA);
	//itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
	//itemDigtalAlarm->setEditable(false);
	//pAlarm->appendRow(itemDigtalAlarm);

	//pNodeItem->appendRow(pAlarm);


	////[1]
	////转换
	//CQuoteItem *itemScale = new CQuoteItem(tr("scale list"));
	//itemScale->setData(SCADAVARIABLE_ITEM_SCALE, Qt::UserRole);
	//itemScale->setIcon(QIcon(SCALE_PNG));
	//itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//itemScale->setData(reinterpret_cast<long long>(m_pScadaVirableConfig), POINTERDATA);
	//itemScale->setEditable(false);

	////线性转换 非线性转换
	//auto pLinear = new CQuoteItem(tr("linear node"));
	//pLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT, Qt::UserRole);
	//pLinear->setIcon(QIcon(SCALE_PNG));
	//pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaLinearGroup);
	//if (!m_pScadaVirableConfig->m_pScadaLinearGroup)
	//{
	//	return;
	//}
	//pLinear->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaLinearGroup), POINTERDATA);
	//pLinear->setEditable(false);
	//itemScale->appendRow(pLinear);
	//auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	//pNonLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT, Qt::UserRole);
	//pNonLinear->setIcon(QIcon(SCALE_PNG));
	//pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaNonLinearGroup);
	//if (!m_pScadaVirableConfig->m_pScadaNonLinearGroup)
	//{
	//	return;
	//}
	//pNonLinear->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaNonLinearGroup), POINTERDATA);
	//pNonLinear->setEditable(false);
	//itemScale->appendRow(pNonLinear);

	//pNodeItem->appendRow(itemScale);


	m_pCore->GetUIMgr()->GetTopItem()->appendRow(pNodeItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(pNodeItem));
}

bool CScadaVariableModule::ClearModuleData()
{
	//如果已加载数据,需清空
	if (m_pScadaVirableConfig)
	{
		m_pScadaVirableConfig->Clear();
		m_pScadaVirableConfig->Create();
	}

	return true;
}

/*! \fn bool CScadaVariableModule::GetScadaConfigName(std::vector< std::string > & arrScadaNames)
********************************************************************************************************* 
** \brief CScadaVariableModule::GetScadaConfigName 
** \details scada变量模块cofnig name
** \param arrScadaNames 
** \return bool 
** \author gw
** \date 2017年5月13日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::GetScadaConfigName(std::vector< std::string > & arrScadaNames)
{
	auto arr = m_pScadaVirableConfig->GetScadaArr();

	auto it = arr.begin();
	for (; it != arr.end(); ++it)
	{
		arrScadaNames.push_back(it->first);
	}

	return true;
}

/*! \fn void CScadaVariableModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
********************************************************************************************************* 
** \brief CScadaVariableModule::CloseProject 
** \details 关闭工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param bSaveFlag 
** \return void 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(bSaveFlag);
}


/*! \fn void CScadaVariableModule::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CScadaVariableModule::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::showMouseRightButton(const QPoint& point)
{
	Q_ASSERT(m_pCore);
	if (!m_pCore)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	QModelIndex indexSelect = m_pCore->GetUIMgr()->GetLeftTree()->indexAt(point);
	auto nType = indexSelect.data(Qt::UserRole).toInt();
	
	if (nType == SCADAVARIABLE_CONFIG || nType == SCADA_ITEM_GROUP)
	{
		ShowMouseRightConfig(indexSelect);
	}
	else if (nType == SCADA_ITEM)
	{
		ShowMouseRightScada(indexSelect);
	}
	if (nType == SCADAVARIABLE_ITEM_VARIABLE_USER)
	{
		QAction *pActionGroup = new QAction(ADD_USER_VARIABLE_GROUP, pMenu);
		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionGroup);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionGroup)
		{
			AddScadaVariableUserGroup(indexSelect);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP)
	{
		QAction *pActionGroup = new QAction(ADD_USER_VARIABLE_GROUP, pMenu);
		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionGroup);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionRename);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionGroup)
		{
			auto pGroupParent = reinterpret_cast<Config::CScadaUserVariableGroup *>(indexSelect.data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}

			AddScadaVariableUserGroup(indexSelect, pGroupParent);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionRename)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete)
		{
			DeleteUserVariableGroup(indexSelect);
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_VARIABLE)
	{
		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM
		|| nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM)
	{
		QAction *pActionAdddItem = new QAction(ADD_ALARM_NAME, pMenu);
		pActionAdddItem->setIcon(QIcon(ALARM_PNG));
		pMenu->addAction(pActionAdddItem);

		QAction *pActionAdddGroupItem = new QAction(ADD_ALARM_GROUP_NAME, pMenu);
		pActionAdddGroupItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionAdddGroupItem);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;
		if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM)
		{
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

			pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);
		}

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionAdddItem)
		{
			AddAnalogAlarm(indexSelect);
		}
		else if (selectedItem == pActionAdddGroupItem)
		{
			AddAnalogAlarmGroup(indexSelect);
		}
		else if (pActionRename != nullptr && selectedItem == pActionRename)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete && pActionDelete != nullptr)
		{
			DeleteAnalogAlarmGroup(indexSelect);
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM
		|| nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM)
	{
		QAction *pActionAdddItem = new QAction(ADD_ALARM_NAME, pMenu);
		pActionAdddItem->setIcon(QIcon(ALARM_PNG));
		pMenu->addAction(pActionAdddItem);

		QAction *pActionAdddGroupItem = new QAction(ADD_ALARM_GROUP_NAME, pMenu);
		pActionAdddGroupItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionAdddGroupItem);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;
		if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM)
		{
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

			pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);
		}

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionAdddItem)
		{
			AddDigitalAlarm(indexSelect);
		}
		else if (selectedItem == pActionAdddGroupItem)
		{
			AddDigitalAlarmGroup(indexSelect);
		}
		else if (pActionRename != nullptr && selectedItem == pActionRename)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete && pActionDelete != nullptr)
		{
			DeleteDigitalAlarmGroup(indexSelect);
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD
		|| nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD)
	{
		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionRename);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionRename)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete)
		{
			if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD)
			{
				DeleteAnalogAlarmItem(indexSelect);
			}
			else if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD)
			{
				DeleteDigitalAlarmItem(indexSelect);
			}
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE)
	{
		QAction *pActionGroup = new QAction(SCALE_GROUP_NAME, pMenu);
		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionGroup);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT || nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_GROUP)
	{
		//线性转换
		QAction *pActionAlarm = new QAction(SCALE_NAME, pMenu);
		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));
		pMenu->addAction(pActionAlarm);

		QAction *pActionGroup = new QAction(SCALE_GROUP_NAME, pMenu);
		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionGroup);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;

		if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_GROUP)
		{
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

			pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);
		}


		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionAlarm)
		{
			//添加转换
			AddLinearScale(indexSelect);
		}
		else if (selectedItem == pActionGroup)
		{
			//添加转换组
			AddLinearScaleGroup(indexSelect);
		}
		else if (selectedItem == pActionDelete && pActionDelete != nullptr)
		{
			DeleteScaleLinearGroup(indexSelect);
		}
		else if (selectedItem == pActionRename && pActionDelete != nullptr)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT || nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP)
	{
		//非线性转换
		QAction *pActionScale = new QAction(SCALE_NAME, pMenu);
		pActionScale->setIcon(QIcon(CHANNEL_PNG));
		pMenu->addAction(pActionScale);

		QAction *pActionGroup = new QAction(SCALE_GROUP_NAME, pMenu);
		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionGroup);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;
		if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP)
		{
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

			pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);
		}


		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionScale)
		{
			//添加转换
			AddNonLinearScale(indexSelect);
		}
		else if (selectedItem == pActionGroup)
		{
			//添加转换组
			AddNonLinearScaleGroup(indexSelect);
		}
		else if (selectedItem == pActionRename && pActionRename != nullptr)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete && pActionDelete != nullptr)
		{
			if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP)
			{
				DeleteScaleNonLinearGroup(indexSelect);
			}
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM ||
		nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM)
	{
		QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionRename);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionRename)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete)
		{
			if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM)
			{
				DeleteScaleLinearItem(indexSelect);
			}
			else if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM)
			{
				DeleteScaleNonLinearItem(indexSelect);
			}
		}
	}
	else if (nType == SCADAVARIABLE_ITEM_ALARM)
	{
		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}

	pMenu->deleteLater();
}

void CScadaVariableModule::ShowMouseRightConfig(QModelIndex &index)
{
	QMenu *pMenu = new QMenu;

	QAction *pActionGroup = new QAction(GROUP_NAME, pMenu);
	pMenu->addAction(pActionGroup);
	pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

	QAction *pAction = new QAction(ADD_SCADA_DESC, pMenu);
	pAction->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pAction);

	QAction *pActionDelete = nullptr;
	QAction *pGroupModifyAction = nullptr;
	if (index.data(Qt::UserRole).toInt() == SCADA_ITEM_GROUP)
	{
		pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pActionDelete->setIcon(QIcon(CHANNEL_PNG));
		pMenu->addAction(pActionDelete);

		pGroupModifyAction = new QAction(RENAME_ITEM, pMenu);
		pGroupModifyAction->setIcon(QIcon(CHANNEL_PNG));
		pMenu->addAction(pGroupModifyAction);
	}

	QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
	pMenu->addAction(pActionExpand);

	QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
	pMenu->addAction(pActionCollapse);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAction)
	{
		//添加scada
		AddScadaItem(index);
	}
	else if (selectedItem == pActionGroup)
	{
		//添加scada组
		AddScadaGroup(index);
	}
	else if (selectedItem == pActionExpand)
	{
		ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
	}
	else if (selectedItem == pActionCollapse)
	{
		CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
	}
	else if (selectedItem == pActionDelete && pActionDelete != nullptr)
	{
		DeleteScadaGroup(index);
	}
	else if (selectedItem == pGroupModifyAction && pGroupModifyAction != nullptr)
	{
		m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
	}

	pMenu->deleteLater();
}

/*! \fn void CScadaVariableModule::ShowMouseRightScada(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowMouseRightScada 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年3月9日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowMouseRightScada(QModelIndex &index)
{
	QMenu menu;
	
	QAction *pActionRename = new QAction(RENAME_ITEM, &menu);
	menu.addAction(pActionRename);

	QAction *pActionDelete = new QAction(DELETE_ITEM, &menu);
	menu.addAction(pActionDelete);

	QAction *pActionExpand = new QAction(EXPAND_ITEM, &menu);
	menu.addAction(pActionExpand);

	QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, &menu);
	menu.addAction(pActionCollapse);

	QAction* selectedItem = menu.exec(QCursor::pos());
	if (selectedItem == pActionRename)
	{
		m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
	}
	else if (selectedItem == pActionDelete)
	{
		DeleteScadaItem(index);
	}
	else if (selectedItem == pActionExpand)
	{
		ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
	}
	else if (selectedItem == pActionCollapse)
	{
		CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
	}
}

/*! \fn void CScadaVariableModule::AddScadaGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddScadaGroup 
** \details 添加scada组
** \param index 
** \return void 
** \author gw
** \date 2017年2月25日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddScadaGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	if (item->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
	{
		auto pGroup = reinterpret_cast<Config::CScadaGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		auto pGroupChild = pGroup->CreateGroup();
		Q_ASSERT(pGroupChild);
		if (!pGroupChild)
		{
			return;
		}

		QString strName = pGroupChild->m_strGroup;

		CQuoteItem *newItem = new CQuoteItem(strName);
		newItem->setData(strName, LASTVALUE);
		newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
		newItem->setData(SCADA_ITEM_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);

		m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
	}
}

void CScadaVariableModule::AddScadaItem(QModelIndex &index)
{
	auto pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	
	auto pGroup = reinterpret_cast<Config::CScadaGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	auto pScada = m_pScadaVirableConfig->AddNewScadaData(pGroup);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = tr("add sub scada item pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	CQuoteItem *pScadaItem = new CQuoteItem(pScada->m_szTagName);
	pScadaItem->setData(pScada->m_szTagName, LASTVALUE);
	pScadaItem->setData(SCADA_ITEM, Qt::UserRole);
	//pFesItem->setData(pFes->m_szTagName, TAGNAME);
	pScadaItem->setData(reinterpret_cast<long long>(pScada), POINTERDATA);
	pScadaItem->setIcon(QIcon(FES_PNG));
	pScadaItem->setData(pScada->m_strDescription, TREE_TOOL_TIP_ROLE);
	pScadaItem->setEditable(true);

	//变量节点
	CQuoteItem *pVariableItem = new CQuoteItem(tr("variable"));
	pVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE, Qt::UserRole);
	//pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScada->m_pScadaUserGroup);
	if (!pScada->m_pScadaUserGroup)
	{
		return;
	}
	//pVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaUserGroup), POINTERDATA);
	pVariableItem->setIcon(QIcon(FES_PNG));
	pVariableItem->setEditable(false);
	pScadaItem->appendRow(pVariableItem);

	//变量节点_系统变量
	CQuoteItem *pVariableSystemVariableItem = new CQuoteItem(tr("variable_systemvariable"));
	pVariableSystemVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_SYSTEM, Qt::UserRole);
	//pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pVariableSystemVariableItem->setData(reinterpret_cast<long long>(pScada), POINTERDATA);
	pVariableSystemVariableItem->setIcon(QIcon(FES_PNG));
	pVariableSystemVariableItem->setEditable(false);
	pVariableItem->appendRow(pVariableSystemVariableItem);

	//变量节点_用户变量
	CQuoteItem *pVariableUserVariableItem = new CQuoteItem(tr("variable_uservariable"));
	pVariableUserVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER, Qt::UserRole);
	pVariableUserVariableItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	Q_ASSERT(pScada->m_pScadaUserGroup);
	if (!pScada->m_pScadaUserGroup)
	{
		return;
	}
	pVariableUserVariableItem->setData(reinterpret_cast<long long>(pScada), POINTERDATA);
	pVariableUserVariableItem->setIcon(QIcon(FES_PNG));
	pVariableUserVariableItem->setEditable(false);
	pVariableItem->appendRow(pVariableUserVariableItem);

	//报警
	auto strAlarm = tr("alarm list");
	CQuoteItem *pItemAlarm = new CQuoteItem(strAlarm);
	pItemAlarm->setData(SCADAVARIABLE_ITEM_ALARM, Qt::UserRole);
	pItemAlarm->setIcon(QIcon(ALARM_PNG));
	pItemAlarm->setEditable(false);
	pScadaItem->appendRow(pItemAlarm);

	//模拟量报警
	CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("AnalogAlarm"));
	itemAnalogAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM, Qt::UserRole);
	itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScada->m_pScadaAnalogAlarmGroup);
	if (!pScada->m_pScadaAnalogAlarmGroup)
	{
		return;
	}
	itemAnalogAlarm->setData(reinterpret_cast<long long>(pScada->m_pScadaAnalogAlarmGroup), POINTERDATA);
	itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
	itemAnalogAlarm->setEditable(false);
	pItemAlarm->appendRow(itemAnalogAlarm);
	//InitAIAlarmGroup(itemAnalogAlarm, m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup);


	//开关量报警
	CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("DigtalAlarm"));
	itemDigtalAlarm->setData(SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM, Qt::UserRole);
	itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScada->m_pScadaDigitalAlarmGroup);
	if (!pScada->m_pScadaDigitalAlarmGroup)
	{
		return;
	}
	itemDigtalAlarm->setData(reinterpret_cast<long long>(pScada->m_pScadaDigitalAlarmGroup), POINTERDATA);
	itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
	itemDigtalAlarm->setEditable(false);
	pItemAlarm->appendRow(itemDigtalAlarm);
	Q_ASSERT(pScada->m_pScadaDigitalAlarmGroup);
	if (!pScada->m_pScadaDigitalAlarmGroup)
	{
		return;
	}
	//InitDIAlarmGroup(itemDigtalAlarm, m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup);


	///////////////////
	//[1]
	//转换
	CQuoteItem *itemScale = new CQuoteItem(tr("scale list"));
	itemScale->setData(SCADAVARIABLE_ITEM_SCALE, Qt::UserRole);
	itemScale->setIcon(QIcon(SCALE_PNG));
	itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemScale->setData(reinterpret_cast<long long>(pScada), POINTERDATA);
	itemScale->setEditable(false);
	pScadaItem->appendRow(itemScale);


	//线性转换 非线性转换
	auto pLinear = new CQuoteItem(tr("linear node"));
	pLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT, Qt::UserRole);
	pLinear->setIcon(QIcon(SCALE_PNG));
	pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScada->m_pScadaLinearGroup);
	if (!pScada->m_pScadaLinearGroup)
	{
		return;
	}
	pLinear->setData(reinterpret_cast<long long>(pScada->m_pScadaLinearGroup), POINTERDATA);
	pLinear->setEditable(false);
	//InitTransformGroup(pLinear, m_pScadaVirableConfig->m_pScadaLinearGroup);
	itemScale->appendRow(pLinear);
	auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	pNonLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT, Qt::UserRole);
	pNonLinear->setIcon(QIcon(SCALE_PNG));
	pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScada->m_pScadaNonLinearGroup);
	if (!pScada->m_pScadaNonLinearGroup)
	{
		return;
	}
	pNonLinear->setData(reinterpret_cast<long long>(pScada->m_pScadaNonLinearGroup), POINTERDATA);
	pNonLinear->setEditable(false);
	InitNonTransformGroup(pNonLinear, pScada->m_pScadaNonLinearGroup);
	itemScale->appendRow(pNonLinear);


	pItem->appendRow(pScadaItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

void CScadaVariableModule::DeleteScadaGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		auto pParentGroup = reinterpret_cast<Config::CScadaGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pParentGroup);
		if (!pParentGroup)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CScadaGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		QVector<QModelIndex> vec;
		vec.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADA_ITEM, vec);
		for each (auto indexScada in vec)
		{
			auto strScadaTagName = indexScada.data(Qt::EditRole).toString();
			if (!CheckTagNameIsValid(strScadaTagName, SCADAVARIABLE_DESC))
			{
				return;
			}

			if (!m_pScadaVirableConfig->DeleteScadaHahsMap(strScadaTagName))
			{
				auto strError = tr("Delete scada hash %1 fail!!!").arg(strScadaTagName);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			DeleteScadaModuleWidgetInTabWidget(indexScada);
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			return;
		}
		if (!pParentGroup->DeletGroup(pGroup, strTag))
		{
			auto strError = tr("delete group fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());

	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn void CScadaVariableModule::DeleteScadaItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteScadaItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年3月14日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteScadaItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the scada item, its childs will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		auto pGroupParent = reinterpret_cast<Config::CScadaGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pScada = reinterpret_cast<Config::CScadaData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}

		auto strScadaTagName = index.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strScadaTagName, SCADAVARIABLE_DESC))
		{
			return;
		}
		if (!m_pScadaVirableConfig->DeleteScadaHahsMap(strScadaTagName))
		{
			auto strError = tr("Delete scada fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		DeleteScadaModuleWidgetInTabWidget(index);

		//组内的scada项删除 删除scada内存
		if (!pGroupParent->DeletItem(pScada, strScadaTagName))
		{
			auto strError = tr("delete scada fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}


		///////////////scada修改，关联的模块也修改///////////////////
		std::vector<QString> vec;
		vec.clear();
		m_pCore->GetModulesName(vec);
		auto pNodeModule = m_pCore->GetModule("node_config");
		Q_ASSERT(pNodeModule);
		if (!pNodeModule)
		{
			return;
		}
		//Scada tagname删除,节点模块关联的scada tagname也删除
		QString strDelete = "";
		pNodeModule->ChangeTagNameNodify(strDelete, strScadaTagName, Module_ID::NODE_MODULE_SERVER);
		pNodeModule->ChangeTagNameNodify(strDelete, strScadaTagName, Module_ID::NODE_MODULE_WORKSTATION);

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn void CScadaVariableModule::DeleteFesModuleWidgetInTabWidget(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteFesModuleWidgetInTabWidget 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年3月14日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteScadaModuleWidgetInTabWidget(QModelIndex &index)
{
	auto strScadaTagName = index.data(Qt::EditRole).toString();
	auto bFlag = CheckTagNameIsValid(strScadaTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return;
	}

	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strScadaTagName, WIDGET_TYPE::SCADA_WIDGET_TYPE, strScadaTagName))
	{
		//删除打开的窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIbase);
		pIbase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	//系统变量窗口
	QVector<QModelIndex> vec;
	vec.clear();
	GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_VARIABLE_SYSTEM, vec);
	for each (auto system in vec)
	{
		auto strTag = system.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			auto strError = QObject::tr("Delete system widget fail, system tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_VARIABLE_SYSTEM_TYPE, strScadaTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//用户变量窗口
	vec.clear();
	GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP, vec);
	for each (auto user in vec)
	{
		auto indexTmp = user;
		auto strTag = GetGroupName(indexTmp);
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			auto strError = QObject::tr("Delete user widget fail, user tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE, strScadaTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//模拟量告警窗口
	vec.clear();
	GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD, vec);
	for each (auto analogAlarm in vec)
	{
		auto strTag = analogAlarm.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			auto strError = QObject::tr("Delete user widget fail, analogAlarm tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE, strScadaTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		QVector<QModelIndex> vecChild;
		vecChild.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), analogAlarm, SCADAVARIABLE_ITEM_ANALOG_ALARM_LIMIT_ITEM, vecChild);
		for each (auto limit in vecChild)
		{
			auto strTag = limit.data(Qt::EditRole).toString();
			if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
			{
				auto strError = QObject::tr("Delete analog alarm widget fail, analogAlarm limit tagname length is invalid!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE, strScadaTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}
	}

	//开关量告警窗口
	vec.clear();
	GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD, vec);
	for each (auto digitalAlarm in vec)
	{
		auto strTag = digitalAlarm.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			auto strError = QObject::tr("Delete user widget fail, digitalalarm tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE, strScadaTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		QVector<QModelIndex> vecChild;
		vecChild.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), digitalAlarm, SCADAVARIABLE_ITEM_DIGITAL_ALARM_LIMIT_ITEM, vecChild);
		for each (auto limit in vecChild)
		{
			auto strTag = limit.data(Qt::EditRole).toString();
			if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
			{
				auto strError = QObject::tr("Delete analog alarm widget fail, digitalalarm limit tagname length is invalid!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE, strScadaTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}
	}

	//线性转换
	vec.clear();
	GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM, vec);
	for each (auto linear in vec)
	{
		auto strTag = linear.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			auto strError = QObject::tr("Delete linear scale widget fail, linear scale tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_SCALE_LINEAR_TYPE, strScadaTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//非线性转换
	vec.clear();
	GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM, vec);
	for each (auto linear in vec)
	{
		auto strTag = linear.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			auto strError = QObject::tr("Delete nonlinear scale widget fail, nonlinear scale tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_SCALE_NONLINEAR_TYPE, strScadaTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}
}

/*! \fn void CScadaVariableModule::DoubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DoubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();
	
	if (nType == SCADA_ITEM)
	{
		Q_ASSERT(m_pCore);
		if (!m_pCore)
		{
			return;
		}

		auto pScadaData = reinterpret_cast<Config::CScadaData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pScadaData);
		if (!pScadaData)
		{
			return;
		}
		if (!CheckTagNameIsValid(pScadaData->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QObject::tr("scada tagname %1 is invalid!!!").arg(strTagName);
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断是否存在该widget
		bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_WIDGET_TYPE, pScadaData->m_szTagName);
		if (bFlag)
		{
			return;
		}

		auto pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		auto pDialog = new CScadaView(m_pCore, pScadaData, pItem, m_pScadaVirableConfig);

		TabKeyStruct info;
		info.BigTagName = pScadaData->m_szTagName;
		info.TagName = strTagName;
		info.Type = WIDGET_TYPE::SCADA_WIDGET_TYPE;

		auto &icon = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->icon();

		m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pDialog, strTagName, info, icon);
		m_pCore->GetUIMgr()->GetTabWidget()->setCurrentWidget(pDialog);
	}
	else if (nType == SCADAVARIABLE_ITEM_VARIABLE_SYSTEM)
	{
		Q_ASSERT(m_pCore);
		if (!m_pCore)
		{
			return;
		}

		auto pScada = reinterpret_cast<Config::CScadaData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return;
		}


		auto pConfig = m_pScadaVirableConfig;
		Q_ASSERT(pConfig);
		if (!pConfig)
		{
			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		//判断是否存在该widget
		auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_VARIABLE_SYSTEM_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			return;
		}

		auto &icon = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->icon();
		CScadaSystemVariableView *pView = new CScadaSystemVariableView(nullptr, m_pCore, pConfig, pScada);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pScada->m_szTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::SCADA_VARIABLE_SYSTEM_TYPE;
		m_pCore->GetUIMgr()->GetTabWidget()->AddTab((QWidget *)pView, strTagName, keyInfo, icon);
	}
	else if (nType == SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP)
	{
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return;
		}

		Q_ASSERT(m_pCore);
		if (!m_pCore)
		{
			return;
		}

		auto indexTmp = index;
		auto strGroupTmp = GetGroupName(indexTmp);

		auto pGroup = reinterpret_cast<Config::CScadaUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		auto strTagName = /*index.data(Qt::EditRole).toString()*/strGroupTmp;
		//判断是否存在该widget
		auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			return;
		}

		Q_ASSERT(m_pScadaVirableConfig);
		if (!m_pScadaVirableConfig)
		{
			return;
		}

		auto &icon = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->icon();
		//打开view
		CScadaUserVariableView *pView = new CScadaUserVariableView(nullptr, m_pCore, m_pScadaVirableConfig, pGroup, pScada);
		TabKeyStruct newKeyInfo;
		newKeyInfo.BigTagName = pScada->m_szTagName;
		newKeyInfo.TagName = strTagName;
		newKeyInfo.Type = WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE;
		m_pCore->GetUIMgr()->GetTabWidget()->AddTab((QWidget *)pView, strTagName, newKeyInfo, icon);
	}
	else if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD)
	{
		ShowAnalogAlarmView(index);
	}
	else if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_LIMIT_ITEM)
	{
		ShowAnalogAlarmLimitView(index);
	}
	else if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD)
	{
		ShowDigitalAlarmView(index);
	}
	else if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_LIMIT_ITEM)
	{
		ShowDigitalAlarmLimitView(index);
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM)
	{
		ShowScaleLinearView(index);
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM)
	{
		ShowScaleNonLinearView(index);
	}
}

/*! \fn void CScadaVariableModule::itemChanged(QStandardItem *item)
********************************************************************************************************* 
** \brief CScadaVariableModule::itemChanged 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::itemChanged(QStandardItem *item)
{
	int nType = item->data(Qt::UserRole).toInt();
	if (nType == SCADA_ITEM)
	{
		auto strTagName = item->data(Qt::EditRole).toString();
		auto strLastTagName = item->data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC) && CheckTagNameIsValid(strLastTagName, SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QObject::tr("Scada tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		Q_ASSERT(m_pCore);
		if (!m_pCore)
		{
			return;
		}

		//scada config判断修改的tagname是否存在
		if (m_pScadaVirableConfig->CheckModifyScadaTagNameExist(strTagName))
		{
			auto strError = tr("scada tag name %1 is exist!!!").arg(strTagName);
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		auto index = m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item);

		auto pGroup = reinterpret_cast<Config::CScadaGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}


		auto pScada = reinterpret_cast<Config::CScadaData *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pScada);
		if (pScada == nullptr)
		{
			auto strError = QObject::tr("scada tagname %1 is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}

		//scada单元内判断tagname是否存在
		if (pScada->CheckTagNameIsExist(strTagName))
		{
			auto strError = tr("scada tag name %1 is exist!!!").arg(strTagName);
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		//Scada config修改hash
		if (!m_pScadaVirableConfig->ModifyScadaHashMap(strTagName, strLastTagName))
		{
			auto strError = QObject::tr("Config scada hash modify fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//Scada修改hash
		if (!pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::SCADAID, strTagName, strLastTagName))
		{
			auto strError = QObject::tr("Scada hash modify fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断是否存在该widget
		bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::SCADA_WIDGET_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			TabKeyStruct newKeyInfo;
			TabKeyStruct lastKeyInfo;
			newKeyInfo.BigTagName = pScada->m_szTagName;
			newKeyInfo.TagName = strTagName;
			newKeyInfo.Type = WIDGET_TYPE::SCADA_WIDGET_TYPE;
			lastKeyInfo.BigTagName = pScada->m_szTagName;
			lastKeyInfo.TagName = strLastTagName;
			lastKeyInfo.Type = WIDGET_TYPE::SCADA_WIDGET_TYPE;

			m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

			IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}


		if (!pGroup->ModifyItem(strTagName, strLastTagName))
		{
			auto strError = QObject::tr("Group modify item fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}



		memset(pScada->m_szTagName, 0, sizeof(pScada->m_szTagName));
		int nSize = strTagName.size();
		strncpy(pScada->m_szTagName, strTagName.toStdString().c_str(), qMin(nSize, MAX_TAGNAME_LEN_SCADASTUDIO));
		item->setData(pScada->m_szTagName, LASTVALUE);

		///////////////scada修改，关联的模块也修改///////////////////
		//////////////////////////////////前置修改，和前置关联的模块也跟着修改/////////////////////////////////////////////////////
		std::vector<QString> vec;
		vec.clear();
		m_pCore->GetModulesName(vec);
		auto pNodeModule = m_pCore->GetModule("node_config");
		Q_ASSERT(pNodeModule);
		if (!pNodeModule)
		{
			return;
		}
		//Scada tagname修改,节点模块关联的scada tagname也修改
		pNodeModule->ChangeTagNameNodify(strTagName, strLastTagName, Module_ID::NODE_MODULE_SERVER);
		pNodeModule->ChangeTagNameNodify(strTagName, strLastTagName, Module_ID::NODE_MODULE_WORKSTATION);

		m_pCore->GetUIMgr()->GetTabWidget()->ModifyScadaVaraibleBigTagName(strTagName, strLastTagName);

		/////////////////////////////////
	}
	else if (nType == SCADA_ITEM_GROUP)
	{
		auto strLastValue = item->data(LASTVALUE).toString();
		auto strValue = item->data(Qt::EditRole).toString();
		if (strLastValue == strValue)
		{
			return;
		}

		auto bFlag = CheckTagNameIsValid(strLastValue, SCADAVARIABLE_DESC) && CheckTagNameIsValid(strValue, SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QObject::tr("Scada group length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pGroupParent = reinterpret_cast<Config::CScadaGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			auto strError = tr("group parent pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pGroup = reinterpret_cast<Config::CScadaGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			auto strError = tr("group pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (pGroupParent->CheckModifyGroupNameExist(pGroup, strValue))
		{
			auto strError = tr("group name %1 is exist!!!").arg(strValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastValue, Qt::EditRole);

			return;
		}

		if (!pGroupParent->ModifyGroup(strValue, strLastValue))
		{
			auto strError = tr("group name %1 is modify fail!!!").arg(strValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastValue, Qt::EditRole);

			return;
		}


		pGroup->m_strGroup = strValue;
		item->setData(strValue, LASTVALUE);
	}
	if (nType == SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP)
	{
		auto strTagName = item->data(Qt::EditRole).toString();
		auto strLastTagName = item->data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}
		
		auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC) && CheckTagNameIsValid(strLastTagName, SCADAVARIABLE_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QObject::tr("Scada user variable group tagname length is invalid!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		Q_ASSERT(m_pCore);
		if (!m_pCore)
		{
			return;
		}

		auto index = m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item);
		if (!index.isValid())
		{
			return;
		}

		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}

		Config::CScadaUserVariableGroup *pGroupParent = nullptr;

		auto nType = index.parent().data(Qt::UserRole).toInt();
		if (nType == SCADAVARIABLE_ITEM_VARIABLE_USER)
		{
			auto pConfig = reinterpret_cast<Config::CScadaData *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pConfig);
			if (!pConfig)
			{
				return;
			}

			pGroupParent = pConfig->m_pScadaUserGroup;
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}
		}
		else
		{
			pGroupParent = reinterpret_cast<Config::CScadaUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}
		}

		if (pGroupParent->CheckGroupNameExist(strTagName))
		{
			auto strError = tr("scada tagname %1 is exist!!!").arg(strTagName);
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		QVector<QModelIndex> vec;
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP, vec);
		vec.append(index);
		for each (auto var in vec)
		{
			//
			auto varTmp = var, varLastTmp = var;
			auto strGroupName = GetGroupName(var);
			auto strLastGroupTagName = GetLastGroupName(varLastTmp);

			//判断是否存在该widget 若存在,刷新
			auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastGroupTagName, WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE, pScada->m_szTagName);
			if (bFlag)
			{
				//判断是否存在该widget
				//strLastTagValue fes tagname
				TabKeyStruct newKeyInfo;
				TabKeyStruct lastKeyInfo;
				newKeyInfo.BigTagName = pScada->m_szTagName;
				newKeyInfo.TagName = strGroupName;
				newKeyInfo.Type = WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE;
				lastKeyInfo.BigTagName = pScada->m_szTagName;
				lastKeyInfo.TagName = strLastGroupTagName;
				lastKeyInfo.Type = WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE;
				
				m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}
		}

		item->setData(strTagName, LASTVALUE);

		auto pGroup = reinterpret_cast<Config::CScadaUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}
		pGroup->SetGroupName(strTagName);
	}
	else if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD)
	{
		ModifyAnalogAlarmItem(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM)
	{
		ModifyAnalogAlarmGroup(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM)
	{
		ModifyDigitalAlarmGroup(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD)
	{
		ModifyDigitalAlarmItem(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM)
	{
		ModifyScaleLinearItem(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_GROUP)
	{
		ModifyScaleLinearGroup(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM)
	{
		ModifyScaleNonLinearItem(item);
	}
	else if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP)
	{
		ModifyScaleNonLinearGroup(item);
	}

	if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM ||
		nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM)
	{
		auto nCount = m_pCore->GetUIMgr()->GetTabWidget()->count();
		for (int i = 0; i < nCount; i++)
		{
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->widget(i));
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				auto strError = QObject::tr("tab widget refresh fail!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
			}

			pIBase->Refresh();
		}
	}


}

/*! \fn void CScadaVariableModule::AddGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddGroup 
** \details 添加scada用户变量组
** \param index 
** \return void 
** \author gw
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddScadaVariableUserGroup(QModelIndex &index)
{
	auto indexTmp = index;

	//向上遍历查找SCADA
	auto bFlag = false;
	while (indexTmp.parent().isValid())
	{
		if (indexTmp.parent().data(Qt::UserRole) == SCADA_ITEM)
		{
			indexTmp = indexTmp.parent();
			bFlag = true;
			break;
		}

		indexTmp = indexTmp.parent();
	}

	auto pScada = reinterpret_cast<Config::CScadaData *>(indexTmp.data(POINTERDATA).toLongLong());
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	
	auto pGroupParent = pScada->m_pScadaUserGroup;
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	auto nDeepth = index.data(GROUP_NUMBER_ROLE).toInt();
	if (nDeepth < MAX_GROUP_NUM)
	{
		nDeepth++;

		auto pGroupChild = pGroupParent->CreateGroup();
		Q_ASSERT(pGroupChild);
		if (!pGroupChild)
		{
			return;
		}

		CQuoteItem *pNewItem = new CQuoteItem(pGroupChild->GetGroupName());
		pNewItem->setData(nDeepth, GROUP_NUMBER_ROLE);
		pNewItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP, Qt::UserRole);
		pNewItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
		pNewItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
		pNewItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);

		pNewItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(pNewItem);
		m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
	}
}

void CScadaVariableModule::AddScadaVariableUserGroup(QModelIndex &index, Config::CScadaUserVariableGroup *pGroup)
{
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	auto pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	auto nDeepth = index.data(GROUP_NUMBER_ROLE).toInt();
	if (nDeepth < MAX_GROUP_NUM)
	{
		nDeepth++;

		auto pGroupChild = pGroup->CreateGroup();
		Q_ASSERT(pGroupChild);
		if (!pGroupChild)
		{
			return;
		}

		CQuoteItem *pNewItem = new CQuoteItem(pGroupChild->GetGroupName());
		pNewItem->setData(nDeepth, GROUP_NUMBER_ROLE);
		pNewItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP, Qt::UserRole);
		pNewItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
		pNewItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
		pNewItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);

		pNewItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(pNewItem);
		m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
	}
}

/*! \fn void CScadaVariableModule::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
********************************************************************************************************* 
** \brief CScadaVariableModule::GetItemMap 
** \details 遍历指定类型项
** \param pModel 
** \param top 
** \param nType 
** \param indexVec 
** \return void 
** \author gw
** \date 2016年12月29日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
{
	QModelIndex Item;

	if (!top.isValid())
	{
		return;
	}

	for (int r = 0; r < pModel->rowCount(top); r++)
	{
		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);

			const QString strTmp = Item.data(Qt::EditRole).toString();

			if (nType == Item.data(Qt::UserRole).toInt())
			{
				indexVec.append(Item);

				//qDebug() << s;
			}

			//qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			GetItemMap(pModel, Item, nType, indexVec);
		}
	}
}

/*! \fn QString CScadaVariableModule::GetGroupName(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::GetGroupName 
** \details 获得index组名 例如group0.group1
** \param index 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2016年12月29日 
** \note 
********************************************************************************************************/
QString CScadaVariableModule::GetGroupName(QModelIndex &index)
{
	QStringList list;
	list.clear();

	QModelIndex indexTmp = index;

	auto strGroupTmp = indexTmp.data(Qt::EditRole).toString();
	list.append(strGroupTmp);
	while (indexTmp.parent().data(Qt::UserRole).toInt() == SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP)
	{
		indexTmp = indexTmp.parent();

		list.append(indexTmp.data(Qt::EditRole).toString());
	}

	auto it = list.rbegin();
	strGroupTmp.clear();
	for (; it != list.rend(); ++it)
	{
		strGroupTmp += *it + ".";
	}

	if (strGroupTmp.contains("."))
	{
		//删除最后一个.
		strGroupTmp.chop(1);
	}

	return strGroupTmp;
}

/*! \fn QString CScadaVariableModule::GetLastGroupName(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::GetLastGroupName 
** \details 获得lastvalue组名
** \param index 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2016年12月29日 
** \note 
********************************************************************************************************/
QString CScadaVariableModule::GetLastGroupName(QModelIndex &index)
{
	QStringList list;
	list.clear();
	auto strGroupTmp = index.data(LASTVALUE).toString();
	list.append(strGroupTmp);
	while (index.parent().data(Qt::UserRole).toInt() == SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP)
	{
		index = index.parent();

		list.append(index.data(LASTVALUE).toString());
	}

	auto it = list.rbegin();
	strGroupTmp.clear();
	for (; it != list.rend(); ++it)
	{
		strGroupTmp += *it + ".";
	}

	if (strGroupTmp.contains("."))
	{
		//删除最后一个.
		strGroupTmp.chop(1);
	}

	return strGroupTmp;
}

/*! \fn void CScadaVariableModule::DeleteGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月29日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteUserVariableGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its children will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			auto strError = QObject::tr("Scada pointer is null!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		int nSize = strlen(pScada->m_szTagName);
		auto bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("Scada user variable group tagname length is inviald!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto nType = index.parent().data(Qt::UserRole).toInt();
		Config::CScadaUserVariableGroup *pGroupParent = nullptr;
		if (nType == SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP)
		{
			pGroupParent = reinterpret_cast<Config::CScadaUserVariableGroup *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}
		}
		else
		{
			auto pConfig = reinterpret_cast<Config::CScadaData *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pConfig);
			if (!pConfig)
			{
				return;
			}

			pGroupParent = pConfig->m_pScadaUserGroup;
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}
		}


		auto pGroup = reinterpret_cast<Config::CScadaUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}
		m_pScadaVirableConfig;

		QVector<QModelIndex> vec;
		vec.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP, vec);
		vec.append(index);
		for each (auto var in vec)
		{
			//组名是拼接的 group0.group1
			auto strGroupName = GetGroupName(var);
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strGroupName, WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE, pScada->m_szTagName))
			{
				//删除打开的窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}

			//删除组下的item hash 不删除内存
			auto pGroup = reinterpret_cast<Config::CScadaUserVariableGroup *>(var.data(POINTERDATA).toLongLong());
			for each (auto pItem in pGroup->m_arrItem)
			{
				if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::USERID , pItem->m_szTagName))
				{
					auto strError = QObject::tr("Scada user variable item tagname %1 hash delete fail!!!").arg(pItem->m_szTagName);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				//删除用户变量的scale、alarm和sourcetagname关联的关系
				if (!pScada->DeleteUserVariableCombineRelation(pItem->m_szTagName, pItem))
				{
					auto strError = QObject::tr("Delete UserVariable CombineRelation %1 fail!!!").arg(pItem->m_szTagName);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

			}
		}


		auto strTag = index.data(Qt::EditRole).toString();
		//删除组以及组的子项(子组和子项)
		if (!pGroupParent->DeleteGroup(pGroup))
		{
			auto strError = tr("delete group fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		//while (pTop->rowCount())
		//{
		//	itemsToBeDeleted << pTop->takeRow(0);
		//	//if the row removed has children:
		//	if (itemsToBeDeleted.at(0)->hasChildren())
		//	{
		//		carryItems << pTop;                 //put on the stack the current loopItem
		//		pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
		//	}
		//	//if current loopItem has no more rows but carryItems list is not empty:
		//	if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

		//}
		//qDeleteAll(itemsToBeDeleted);

		pTop->parent()->removeRow(pTop->row());


		break;
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn void CScadaVariableModule::CreateUI()
********************************************************************************************************* 
** \brief CScadaVariableModule::CreateUI 
** \details 
** \return void 
** \author gw
** \date 2017年1月7日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::CreateNewUI()
{
	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}
	
	auto pTop = m_pCore->GetUIMgr()->GetTopItem();

	//scada变量根节点
	CQuoteItem *pNodeItem = new CQuoteItem(tr("ScadaVariableConfig"));
	pNodeItem->setData(SCADAVARIABLE_CONFIG, Qt::UserRole);
	pNodeItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pNodeItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pGroup), POINTERDATA);
	pNodeItem->setIcon(QIcon(FES_PNG));
	pNodeItem->setEditable(false);
	pTop->appendRow(pNodeItem);
	
	CreateScadaGroupUi(m_pScadaVirableConfig->m_pGroup, pNodeItem);

	////变量节点
	//CQuoteItem *pVariableItem = new CQuoteItem(tr("variable"));
	//pVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE, Qt::UserRole);
	////pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaUserGroup);
	//if (!m_pScadaVirableConfig->m_pScadaUserGroup)
	//{
	//	return;
	//}
	////pVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaUserGroup), POINTERDATA);
	//pVariableItem->setIcon(QIcon(FES_PNG));
	//pVariableItem->setEditable(false);
	//pNodeItem->appendRow(pVariableItem);

	////变量节点_系统变量
	//CQuoteItem *pVariableSystemVariableItem = new CQuoteItem(tr("variable_systemvariable"));
	//pVariableSystemVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_SYSTEM, Qt::UserRole);
	////pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//pVariableSystemVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig), POINTERDATA);
	//pVariableSystemVariableItem->setIcon(QIcon(FES_PNG));
	//pVariableSystemVariableItem->setEditable(false);
	//pVariableItem->appendRow(pVariableSystemVariableItem);

	////变量节点_用户变量
	//CQuoteItem *pVariableUserVariableItem = new CQuoteItem(tr("variable_uservariable"));
	//pVariableUserVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER, Qt::UserRole);
	//pVariableUserVariableItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig);
	//if (!m_pScadaVirableConfig)
	//{
	//	return;
	//}
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaUserGroup);
	//if (!m_pScadaVirableConfig->m_pScadaUserGroup)
	//{
	//	return;
	//}
	//pVariableUserVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig), POINTERDATA);
	//pVariableUserVariableItem->setIcon(QIcon(FES_PNG));
	//pVariableUserVariableItem->setEditable(false);
	//InitScadaUserGroup(pVariableUserVariableItem, m_pScadaVirableConfig->m_pScadaUserGroup);


	//pVariableItem->appendRow(pVariableUserVariableItem);


	////报警
	//auto strAlarm = tr("alarm list");
	//CQuoteItem *pItemAlarm = new CQuoteItem(strAlarm);
	//pItemAlarm->setData(SCADAVARIABLE_ITEM_ALARM, Qt::UserRole);
	//pItemAlarm->setIcon(QIcon(ALARM_PNG));
	//pItemAlarm->setEditable(false);
	//pNodeItem->appendRow(pItemAlarm);

	////模拟量报警
	//CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
	//itemAnalogAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM, Qt::UserRole);
	//itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup);
	//if (!m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup)
	//{
	//	return;
	//}
	//itemAnalogAlarm->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup), POINTERDATA);
	//itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
	//itemAnalogAlarm->setEditable(false);
	//pItemAlarm->appendRow(itemAnalogAlarm);

	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup);
	//if (!m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup)
	//{
	//	return;
	//}
	//InitAIAlarmGroup(itemAnalogAlarm, m_pScadaVirableConfig->m_pScadaAnalogAlarmGroup);


	////开关量报警
	//CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
	//itemDigtalAlarm->setData(SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM, Qt::UserRole);
	//itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup);
	//if (!m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup)
	//{
	//	return;
	//}
	//itemDigtalAlarm->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup), POINTERDATA);
	//itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
	//itemDigtalAlarm->setEditable(false);
	//pItemAlarm->appendRow(itemDigtalAlarm);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup);
	//if (!m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup)
	//{
	//	return;
	//}
	//InitDIAlarmGroup(itemDigtalAlarm, m_pScadaVirableConfig->m_pScadaDigitalAlarmGroup);


	/////////////////////
	////[1]
	////转换
	//CQuoteItem *itemScale = new CQuoteItem(tr("scale list"));
	//itemScale->setData(SCADAVARIABLE_ITEM_SCALE, Qt::UserRole);
	//itemScale->setIcon(QIcon(SCALE_PNG));
	//itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//itemScale->setData(reinterpret_cast<long long>(m_pScadaVirableConfig), POINTERDATA);
	//itemScale->setEditable(false);

	////线性转换 非线性转换
	//auto pLinear = new CQuoteItem(tr("linear node"));
	//pLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT, Qt::UserRole);
	//pLinear->setIcon(QIcon(SCALE_PNG));
	//pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaLinearGroup);
	//if (!m_pScadaVirableConfig->m_pScadaLinearGroup)
	//{
	//	return;
	//}
	//pLinear->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaLinearGroup), POINTERDATA);
	//pLinear->setEditable(false);
	//InitTransformGroup(pLinear, m_pScadaVirableConfig->m_pScadaLinearGroup);
	//itemScale->appendRow(pLinear);
	//auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	//pNonLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT, Qt::UserRole);
	//pNonLinear->setIcon(QIcon(SCALE_PNG));
	//pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//Q_ASSERT(m_pScadaVirableConfig->m_pScadaNonLinearGroup);
	//if (!m_pScadaVirableConfig->m_pScadaNonLinearGroup)
	//{
	//	return;
	//}
	//pNonLinear->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaNonLinearGroup), POINTERDATA);
	//pNonLinear->setEditable(false);
	//InitNonTransformGroup(pNonLinear, m_pScadaVirableConfig->m_pScadaNonLinearGroup);
	//itemScale->appendRow(pNonLinear);
	//pNodeItem->appendRow(itemScale);


	//m_pCore->GetUIMgr()->GetTopItem()->appendRow(pNodeItem);
}

void CScadaVariableModule::CreateScadaGroupUi(Config::CScadaGroup *pScadaGroup, CQuoteItem *pItem)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pScadaGroup->m_arrGroup.begin();
	for (; it != pScadaGroup->m_arrGroup.end(); it++)
	{
		auto strGroupName = it->second->m_strGroup;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(SCADA_ITEM_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(it->second), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		CreateScadaGroupUi(it->second, newItem);
	}

	auto ite = pScadaGroup->m_arrItem.begin();
	for (; ite != pScadaGroup->m_arrItem.end(); ++ite)
	{
		//添加前置节点
		auto pScada = ite->second;
		CQuoteItem *pScadaItem = new CQuoteItem(pScada->m_szTagName);
		pScadaItem->setData(pScada->m_szTagName, LASTVALUE);
		pScadaItem->setData(SCADA_ITEM, Qt::UserRole);
		//pScadaItem->setData(tmp->m_szTagName, TAGNAME);
		pScadaItem->setData((reinterpret_cast<std::size_t>(pScada)), POINTERDATA);
		pScadaItem->setData(pScada->m_strDescription, TREE_TOOL_TIP_ROLE);
		pScadaItem->setIcon(QIcon(FES_PNG));
		pScadaItem->setEditable(true);

		pItem->appendRow(pScadaItem);

		InitVariable(pScadaItem, pScada);

		InitAlarm(pScadaItem, pScada);

		InitTransfrom(pScadaItem, pScada);
	}
}

/*! \fn void CScadaVariableModule::InitVariable(CQuoteItem *pItem, Config::CScadaData *pCScadaData)
********************************************************************************************************* 
** \brief CScadaVariableModule::InitVariable 
** \details 
** \param pItem 
** \param pCScadaData 
** \return void 
** \author gw
** \date 2017年3月16日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::InitVariable(CQuoteItem *pItem, Config::CScadaData *pCScadaData)
{
	//变量节点
	CQuoteItem *pVariableItem = new CQuoteItem(tr("variable"));
	pVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE, Qt::UserRole);
	//pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	//pVariableItem->setData(reinterpret_cast<long long>(m_pScadaVirableConfig->m_pScadaUserGroup), POINTERDATA);
	pVariableItem->setIcon(QIcon(FES_PNG));
	pVariableItem->setEditable(false);
	pItem->appendRow(pVariableItem);

	//变量节点_系统变量
	CQuoteItem *pVariableSystemVariableItem = new CQuoteItem(tr("variable_systemvariable"));
	pVariableSystemVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_SYSTEM, Qt::UserRole);
	//pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pVariableSystemVariableItem->setData(reinterpret_cast<long long>(pCScadaData), POINTERDATA);
	pVariableSystemVariableItem->setIcon(QIcon(FES_PNG));
	pVariableSystemVariableItem->setEditable(false);
	pVariableItem->appendRow(pVariableSystemVariableItem);

	//变量节点_用户变量
	CQuoteItem *pVariableUserVariableItem = new CQuoteItem(tr("variable_uservariable"));
	pVariableUserVariableItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER, Qt::UserRole);
	pVariableUserVariableItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}
	Q_ASSERT(pCScadaData->m_pScadaUserGroup);
	if (!pCScadaData->m_pScadaUserGroup)
	{
		return;
	}
	pVariableUserVariableItem->setData(reinterpret_cast<long long>(pCScadaData), POINTERDATA);
	pVariableUserVariableItem->setIcon(QIcon(FES_PNG));
	pVariableUserVariableItem->setEditable(false);
	InitScadaUserGroup(pVariableUserVariableItem, pCScadaData->m_pScadaUserGroup);

	pVariableItem->appendRow(pVariableUserVariableItem);
}

/*! \fn bool CScadaVariableModule::InitChannelGroup(CQuoteItem *pItem, Config::CScadaUserVariableGroup *pGroup)
********************************************************************************************************* 
** \brief CScadaVariableModule::InitChannelGroup 
** \details 
** \param pItem 
** \param pGroup 
** \return bool 
** \author gw
** \date 2017年1月7日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::InitScadaUserGroup(CQuoteItem *pItem, Config::CScadaUserVariableGroup *pGroup)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pGroup->m_arrGroup.begin();
	for (; it != pGroup->m_arrGroup.end(); it++)
	{
		auto strGroupName = (*it)->GetGroupName();
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(*it), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitScadaUserGroup(newItem, *it);
	}

	return true;
}

void CScadaVariableModule::InitAlarm(CQuoteItem *pItem, Config::CScadaData *pScadaData)
{
	//报警
	auto strAlarm = tr("alarm list");
	CQuoteItem *pItemAlarm = new CQuoteItem(strAlarm);
	pItemAlarm->setData(SCADAVARIABLE_ITEM_ALARM, Qt::UserRole);
	pItemAlarm->setIcon(QIcon(ALARM_PNG));
	pItemAlarm->setEditable(false);
	pItem->appendRow(pItemAlarm);

	//模拟量报警
	CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
	itemAnalogAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM, Qt::UserRole);
	itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScadaData->m_pScadaAnalogAlarmGroup);
	if (!pScadaData->m_pScadaAnalogAlarmGroup)
	{
		return;
	}
	itemAnalogAlarm->setData(reinterpret_cast<long long>(pScadaData->m_pScadaAnalogAlarmGroup), POINTERDATA);
	itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
	itemAnalogAlarm->setEditable(false);
	pItemAlarm->appendRow(itemAnalogAlarm);

	Q_ASSERT(pScadaData->m_pScadaAnalogAlarmGroup);
	if (!pScadaData->m_pScadaAnalogAlarmGroup)
	{
		return;
	}
	InitAIAlarmGroup(itemAnalogAlarm, pScadaData->m_pScadaAnalogAlarmGroup);


	//开关量报警
	CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
	itemDigtalAlarm->setData(SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM, Qt::UserRole);
	itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScadaData->m_pScadaDigitalAlarmGroup);
	if (!pScadaData->m_pScadaDigitalAlarmGroup)
	{
		return;
	}
	itemDigtalAlarm->setData(reinterpret_cast<long long>(pScadaData->m_pScadaDigitalAlarmGroup), POINTERDATA);
	itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
	itemDigtalAlarm->setEditable(false);
	pItemAlarm->appendRow(itemDigtalAlarm);
	Q_ASSERT(pScadaData->m_pScadaDigitalAlarmGroup);
	if (!pScadaData->m_pScadaDigitalAlarmGroup)
	{
		return;
	}
	InitDIAlarmGroup(itemDigtalAlarm, pScadaData->m_pScadaDigitalAlarmGroup);
}

bool CScadaVariableModule::InitTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}



	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{

		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_GROUP, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);




		InitTransformGroup(newItem, var);
	}

	auto &arrLinearItem = pGroup->GetItem();
	for each (auto var in arrLinearItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *itemLinear = new CQuoteItem(var->m_pLinearScale->m_szTagName);
			itemLinear->setData(var->m_pLinearScale->m_szTagName, LASTVALUE);
			itemLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			itemLinear->setData(reinterpret_cast<long long>(var), POINTERDATA);
			itemLinear->setData(var->m_pLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);
			itemLinear->setIcon(QIcon(SCALE_PNG));
			itemLinear->setEditable(true);

			pItem->appendRow(itemLinear);
		}
	}

	return true;
}

bool CScadaVariableModule::InitNonTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}



	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{

		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitNonTransformGroup(newItem, var);
	}

	auto &arrNonLinearItem = pGroup->GetItem();
	for each (auto var in arrNonLinearItem)
	{
		{
			Q_ASSERT(var);
			Q_ASSERT(var->m_pNonLinearScale);
			//线性转换
			//名字
			CQuoteItem *itemLinear = new CQuoteItem(var->m_pNonLinearScale->m_szTagName);
			itemLinear->setData(var->m_pNonLinearScale->m_szTagName, LASTVALUE);
			itemLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			itemLinear->setData(reinterpret_cast<long long>(var), POINTERDATA);
			itemLinear->setData(var->m_pNonLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);
			itemLinear->setIcon(QIcon(SCALE_PNG));
			itemLinear->setEditable(true);

			pItem->appendRow(itemLinear);
		}
	}

	return true;
}

Config::CScadaData *CScadaVariableModule::GetScada(QModelIndex index)
{
	auto bFlag = false;
	while (index.parent().isValid())
	{
		if (index.parent().data(Qt::UserRole) == SCADA_ITEM)
		{
			index = index.parent();
			bFlag = true;
			break;
		}

		index = index.parent();
	}

	auto pScada = reinterpret_cast<Config::CScadaData *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("Scada pointer is null!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return nullptr;
	}
	int nSize = strlen(pScada->m_szTagName);
	bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		auto strError = QObject::tr("Scada user variable group tagname length is inviald!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return nullptr;
	}

	return pScada;
}

/*! \fn void CScadaVariableModule::AddAnalogAlarm(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddAnalogAlarm 
** \details 添加scada变量模拟量告警
** \param index 
** \return void 
** \author gw
** \date 2017年1月7日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddAnalogAlarm(QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("scada pointer is null!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	
	auto pAlarm = pScada->AddAnalogAlarm();
	Q_ASSERT(pAlarm);
	if (!pAlarm)
	{
		auto strError = QObject::tr("scada pointer is null!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return;
	}

	auto pAlarGroup = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAlarGroup);
	if (!pAlarGroup)
	{
		return;
	}

	//模拟量告警项添加到父组
	pAlarGroup->AddAnalogItem(pAlarm);
	//添加报警
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();
	QString strName = pAlarm->m_szTagName;

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD, Qt::UserRole);
	newItem->setData(strName, LASTVALUE);
	//newItem->setData(pTmp->m_szTagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pAlarm), POINTERDATA);
	newItem->setData(pAlarm->m_strDescription, TREE_TOOL_TIP_ROLE);
	newItem->setIcon(QIcon(ALARM_PNG));

	auto strLimitTagName = QString("%1_limit").arg(strName);
	auto strLimitDesc = QString(tr("%1_limit")).arg(strName);
	CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
	pLimitItem->setData(reinterpret_cast<long long>(pAlarm), POINTERDATA);
	pLimitItem->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_LIMIT_ITEM, Qt::UserRole);
	pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
	pLimitItem->setIcon(QIcon(ALARM_PNG));
	newItem->appendRow(pLimitItem);

	item->appendRow(newItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

/*! \fn void CScadaVariableModule::AddAnalogAlarmGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddAnalogAlarmGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月10日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddAnalogAlarmGroup(QModelIndex &index)
{
	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	auto nDeepth = pItem->data(GROUP_NUMBER_ROLE).toInt();
	if (nDeepth < MAX_GROUP_NUM)
	{
		auto pGroupParent = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroupChild = pGroupParent->CreateGroup();
		Q_ASSERT(pGroupChild);
		if (!pGroupChild)
		{
			return;
		}

		nDeepth++;

		CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
		newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
		newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(nDeepth, GROUP_NUMBER_ROLE);
		newItem->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaVariableModule::AddDigitalAlarm 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月10日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddDigitalAlarm(QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("scada pointer is null!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto pAlarm = pScada->AddDigitalAlarm();
	Q_ASSERT(pAlarm);
	if (!pAlarm)
	{
		return;
	}
	if (!CheckTagNameIsValid(pAlarm->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}


	auto pAlarGroup = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAlarGroup);
	if (!pAlarGroup)
	{
		return;
	}

	//模拟量告警项添加到父组
	pAlarGroup->AddDigitalItem(pAlarm);
	//添加报警
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();
	QString strName = pAlarm->m_szTagName;

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD, Qt::UserRole);
	newItem->setData(strName, LASTVALUE);
	//newItem->setData(pTmp->m_szTagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pAlarm), POINTERDATA);
	newItem->setData(pAlarm->m_strDescription, TREE_TOOL_TIP_ROLE);
	newItem->setIcon(QIcon(ALARM_PNG));

	auto strLimitTagName = QString("%1_limit").arg(strName);
	auto strLimitDesc = QString(tr("%1_limit")).arg(strName);
	CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
	pLimitItem->setData(reinterpret_cast<long long>(pAlarm), POINTERDATA);
	pLimitItem->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_LIMIT_ITEM, Qt::UserRole);
	pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
	pLimitItem->setIcon(QIcon(ALARM_PNG));
	newItem->appendRow(pLimitItem);

	item->appendRow(newItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaVariableModule::AddDigitalAlarmGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月10日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddDigitalAlarmGroup(QModelIndex &index)
{
	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	auto nDeepth = pItem->data(GROUP_NUMBER_ROLE).toInt();
	if (nDeepth < MAX_GROUP_NUM)
	{
		auto pGroupParent = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroupChild = pGroupParent->CreateGroup();
		Q_ASSERT(pGroupChild);
		if (!pGroupChild)
		{
			return;
		}

		nDeepth++;

		CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
		newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
		newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(nDeepth, GROUP_NUMBER_ROLE);
		newItem->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
	}
}

/*! \fn void CScadaVariableModule::ModifyAnalogAlarmItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyAnalogAlarmItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月10日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyAnalogAlarmItem(QStandardItem *item)
{
	auto strTagName = item->data(Qt::EditRole).toString();
	auto strLastTagName = item->data(LASTVALUE).toString();
	
	if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC) || !CheckTagNameIsValid(strLastTagName, SCADAVARIABLE_DESC))
	{
		auto strError = QObject::tr("Analog alarm length is longer %1").arg(MAX_TAGNAME_LEN_SCADASTUDIO);
		m_pCore->LogMsg(SCADASTUDIO_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return;
	}

	if (strTagName == strLastTagName)
	{
		return;
	}
	
	auto pScada = GetScada(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item));
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("scada pointer is null!!!");
		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADASTUDIO_DESC))
	{
		return;
	}

	if (pScada->CheckTagNameIsExist(strTagName))
	{
		auto strError = tr("analog alarm tag name %1 is exist!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strLastTagName, Qt::EditRole);

		return;
	}

	//整个hash表查找
	if (!pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMID, strTagName, strLastTagName))
	{
		auto strError = tr("ananlog alarm tagname %1 hash can not find!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto *pAnalog = reinterpret_cast<Config::CScadaAnalogAlarm *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pAnalog);
	if (!pAnalog)
	{
		auto strError = QObject::tr("Scada analog alarm pointer is null!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	memset(pAnalog->m_szTagName, 0, sizeof(pAnalog->m_szTagName));
	int nSize = strTagName.size();
	strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
	item->setData(pAnalog->m_szTagName, LASTVALUE);
	//item->setData(pAnalog->m_strDescription, TREE_TOOL_TIP_ROLE);

	if (item->hasChildren())
	{
		auto strTmp = QString("%1_limit").arg(pAnalog->m_szTagName);
		item->child(0, 0)->setData(strTmp, Qt::EditRole);
		item->child(0, 0)->setData(strTmp, TREE_TOOL_TIP_ROLE);
	}

	//判断是否存在该widget
	auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		TabKeyStruct keyInfo;
		TabKeyStruct lastKeyInfo;
		keyInfo.BigTagName = pScada->m_szTagName;
		keyInfo.TagName = pAnalog->m_szTagName;
		keyInfo.Type = WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE;
		lastKeyInfo.BigTagName = pScada->m_szTagName;
		lastKeyInfo.TagName = strLastTagName;
		lastKeyInfo.Type = WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE;
		
		m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(keyInfo, lastKeyInfo);

		IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pBase);
		if (!pBase)
		{
			return;
		}

		pBase->Refresh();
	}
	auto strLastTagNameTmp = strLastTagName + "_limit";
	auto strTagNameTmp = QString("%1%2").arg(pAnalog->m_szTagName).arg("_limit");
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagNameTmp, WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		TabKeyStruct keyInfo;
		TabKeyStruct lastkeyInfo;
		
		keyInfo.BigTagName = pScada->m_szTagName;
		keyInfo.TagName = strTagNameTmp;
		keyInfo.Type = WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE;
		lastkeyInfo.BigTagName = pScada->m_szTagName;
		lastkeyInfo.TagName = strLastTagNameTmp;
		lastkeyInfo.Type = WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE;

		m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(keyInfo, lastkeyInfo);

		IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pBase);
		if (!pBase)
		{
			return;
		}

		pBase->Refresh();
	}

	//模拟量告警修改，用户变量关联的这个告警也跟着修改
	if (!pScada->ModifyUserVariableAlarmArr(strTagName.toStdString(), strLastTagName.toStdString(), pAnalog->m_strDescription.toStdString(), AlarmType::AIALARM))
	{
		auto strError = QObject::tr("Modify UserVariable Alarm %1 failed!!!").arg(strLastTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
}

/*! \fn void CScadaVariableModule::ModifyAnalogAlarmGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyAnalogAlarmGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月10日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyAnalogAlarmGroup(QStandardItem *item)
{
	QString strAlarmTagName = item->data(Qt::EditRole).toString();
	auto strAlarmLastTagValue = item->data(LASTVALUE).toString();
	if (strAlarmTagName == strAlarmLastTagValue)
	{
		return;
	}

	auto pGroupParent = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(item->parent()->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto pGroup = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	auto strGroupName = item->data(Qt::EditRole).toString();

	if (pGroupParent->CheckModifyGroupNameExist(pGroup, strGroupName))
	{
		auto strError = tr("group name %1 is modify fail!!!").arg(strAlarmTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strAlarmLastTagValue, Qt::EditRole);

		return;
	}

	pGroup->SetGroupName(strAlarmTagName);
	item->setData(strAlarmTagName, LASTVALUE);
}

/*! \fn void CScadaVariableModule::ModifyDigitalAlarmGroup(QStandardItem *item)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyDigitalAlarmGroup 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyDigitalAlarmGroup(QStandardItem *item)
{
	QString strAlarmTagName = item->data(Qt::EditRole).toString();
	auto strAlarmLastTagValue = item->data(LASTVALUE).toString();
	if (strAlarmTagName == strAlarmLastTagValue)
	{
		return;
	}

	auto pGroupParent = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(item->parent()->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto pGroup = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	auto strGroupName = item->data(Qt::EditRole).toString();

	if (pGroupParent->CheckModifyGroupNameExist(pGroup, strGroupName))
	{
		auto strError = tr("group name %1 is modify fail!!!").arg(strAlarmTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strAlarmLastTagValue, Qt::EditRole);

		return;
	}

	pGroup->SetGroupName(strAlarmTagName);
	item->setData(strAlarmTagName, LASTVALUE);
}

/*! \fn void CScadaVariableModule::ModifyDigitalAlarmItem(QStandardItem *item)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyDigitalAlarmItem 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyDigitalAlarmItem(QStandardItem *item)
{
	auto index = m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item);
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("Scada pointer is null");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	auto bFlag = CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("Scada tagname %1 length is invalid").arg(pScada->m_szTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto strTagName = item->data(Qt::EditRole).toString();
	auto strLastTagName = item->data(LASTVALUE).toString();

	bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC) && CheckTagNameIsValid(strLastTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("Ditial alarm tagname length is longer %1").arg(MAX_TAGNAME_LEN_SCADASTUDIO);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	if (strTagName == strLastTagName)
	{
		return;
	}

	if (pScada->CheckTagNameIsExist(strTagName))
	{
		auto strError = tr("digital alarm tag name %1 is exist!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strLastTagName, Qt::EditRole);

		return;
	}

	//整个hash表查找
	if (!pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMID, strTagName, strLastTagName))
	{
		auto strError = tr("digital alarm tagname %1 hash can not find!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto pDigital = reinterpret_cast<Config::CScadaAnalogAlarm *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pDigital);
	if (!pDigital)
	{
		return;
	}

	memset(pDigital->m_szTagName, 0, sizeof(pDigital->m_szTagName));
	int nSize = strTagName.size();
	strncpy(pDigital->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
	item->setData(strTagName, LASTVALUE);
	if (item->hasChildren())
	{
		auto strTmp = QString("%1_limit").arg(pDigital->m_szTagName);
		item->child(0, 0)->setData(strTmp, Qt::EditRole);
	}

	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		TabKeyStruct keyInfo;
		TabKeyStruct lastKeyInfo;
		keyInfo.BigTagName = pScada->m_szTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE;
		lastKeyInfo.BigTagName = pScada->m_szTagName;
		lastKeyInfo.TagName = strLastTagName;
		lastKeyInfo.Type = WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE;

		m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(keyInfo, lastKeyInfo);

		IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pBase);
		if (!pBase)
		{
			return;
		}

		pBase->Refresh();
	}
	auto strLastTagNameTmp = strLastTagName + "_limit";
	auto strTagNameTmp = strTagName + "_limit";
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagNameTmp, WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		TabKeyStruct keyInfo;
		TabKeyStruct lastKeyInfo;
		keyInfo.BigTagName = pScada->m_szTagName;
		keyInfo.TagName = strTagNameTmp;
		keyInfo.Type = WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE;
		lastKeyInfo.BigTagName = pScada->m_szTagName;
		lastKeyInfo.TagName = strLastTagNameTmp;
		lastKeyInfo.Type = WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE;

		m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(keyInfo, lastKeyInfo);

		IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pBase);
		if (!pBase)
		{
			return;
		}

		pBase->Refresh();
	}

	//开关量告警修改,用户变量绑定关联的告警也修改
	pScada->ModifyUserVariableAlarmArr(strTagName.toStdString(), strLastTagName.toStdString(), pDigital->m_strDescription.toStdString(), AlarmType::DIALARM);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = tr("Modify UserVariable Related Alarm %1 fail!!!").arg(strLastTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
}

/*! \fn void CScadaVariableModule::DeleteAnalogAlarmGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteAnalogAlarmGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteAnalogAlarmGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
		case QMessageBox::Yes:
		{
			// Save was clicked
			auto pScada = GetScada(index);
			Q_ASSERT(pScada);
			if (!pScada)
			{
				return;
			}
			if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
			{
				return;
			}

			auto pGroupParent = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}

			//删除选中组
			auto pAnalogAlarmGroup = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pAnalogAlarmGroup);
			if (!pAnalogAlarmGroup)
			{
				return;
			}

			QVector<QModelIndex> vec;
			vec.clear();
			GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD, vec);
			for each (auto var in vec)
			{
				auto strTag = var.data(Qt::EditRole).toString();
				if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
				{
					return;
				}


				//判断是否存在该widget
				auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE, pScada->m_szTagName);
				if (bFlag)
				{
					//删除模拟量告警窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIbase);
					pIbase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
				}

				auto strTagNameTmp = strTag + "_limit";
				bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagNameTmp, WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE, pScada->m_szTagName);
				if (bFlag)
				{
					//删除模拟量告警窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIbase);
					pIbase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
				}

				//删除hash
				//删除告警hash
				if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMID, strTag))
				{
					auto strError = QObject::tr("Hash %1 delete failed!!!").arg(strTag);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				//删除告警限值hash
				auto pAlarm = reinterpret_cast<Config::CScadaAnalogAlarm *>(var.data(POINTERDATA).toLongLong());
				auto &arr = pAlarm->m_arrAlarmOfflimit;
				for each (auto var in arr)
				{
					auto strTmp = QString("%1").arg(var->m_szTagName);
					if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID, strTmp))
					{
						auto strError = QObject::tr("Hash %1 delete failed!!!").arg(strTmp);
						m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

						return;
					}
				}

				//开关量告警删除,用户变量关联绑定的告警也跟着删除
				bFlag = pScada->DeleteUserVariableAlarmArr(strTag.toStdString());
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					auto strError = tr("Delete UserVariable related Alarm %1!!!").arg(strTag);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}

			//删除组内的组
			if (!pGroupParent->DeleteGroup(pAnalogAlarmGroup))
			{
				auto strError = tr("delete group fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
			pTop->parent()->removeRow(pTop->row());
		}
		case QMessageBox::Cancel:
			// Cancel was clicked
			break;
		default:
			// should never be reached
			break;
	}
}

/*! \fn void CScadaVariableModule::ShowAnalogAlarmView(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowAnalogAlarmView 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowAnalogAlarmView(const QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("scada pointer is null!!!");
		m_pCore->LogMsg(SCADASTUDIO_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	Q_ASSERT(m_pCore);
	if (!m_pCore)
	{
		return;
	}
	
	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	auto pAlarm = reinterpret_cast<Config::CScadaAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAlarm);
	if (!pAlarm)
	{
		auto strError = tr("analog alarm pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	if (!CheckTagNameIsValid(pAlarm->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto strTagName = index.data(Qt::EditRole).toString();
	if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
	{
		return;
	}
	//判断是否存在该widget
	auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		return;
	}

	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	auto pView = new CScadaAnalogAlarmView(nullptr, m_pCore, pAlarm, pItem, m_pScadaVirableConfig, pScada);
	auto icon = pItem->icon();

	TabKeyStruct keyInfo;
	keyInfo.BigTagName = pScada->m_szTagName;
	keyInfo.TagName = strTagName;
	keyInfo.Type = WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE;
	m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
}

/*! \fn void CScadaVariableModule::ShowDigitalAlarmView(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowDigitalAlarmView 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowDigitalAlarmView(const QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = QObject::tr("scada digital alarm pointer is null!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto bFlag = CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("scada digital alarm %1 is invalid!!!").arg(pScada->m_szTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	Q_ASSERT(m_pCore);
	if (!m_pCore)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	auto pDigital = reinterpret_cast<Config::CScadaDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDigital);
	if (!pDigital)
	{
		auto strError = tr("digital alarm pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto strTagName = index.data(Qt::EditRole).toString();
	//判断tagname长度
	if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
	{
		auto strError = tr("digital alarm tagname %1 is invalid!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		return;
	}

	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	auto pView = new CScadaDigitalAlarmView(nullptr, m_pCore, pDigital, pItem, m_pScadaVirableConfig, pScada);
	auto icon = pItem->icon();
	TabKeyStruct keyInfo;
	keyInfo.BigTagName = pScada->m_szTagName;
	keyInfo.TagName = strTagName;
	keyInfo.Type = WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE;

	m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
}

/*! \fn void CScadaVariableModule::ShowDigitalAlarmLimitView(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowDigitalAlarmLimitView 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowDigitalAlarmLimitView(const QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	auto bFlag = CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = tr("digital alarm tagname length is invalid!!!").arg(pScada->m_szTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	
	Q_ASSERT(m_pCore);
	if (!m_pCore)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	auto pDigital = reinterpret_cast<Config::CScadaDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDigital);
	if (!pDigital)
	{
		auto strError = tr("digital alarm pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	if (!CheckTagNameIsValid(pDigital->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}


	auto strTagName = index.data(Qt::EditRole).toString();
	bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = tr("digital alarm tagname %1 length is invalid!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		return;
	}

	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	auto pView = new CScadaDigitalAlarmLimitView(nullptr, m_pCore, pDigital, pItem, m_pScadaVirableConfig, pScada);
	auto icon = pItem->icon();
	TabKeyStruct keyInfo;
	keyInfo.BigTagName = pScada->m_szTagName;
	keyInfo.TagName = strTagName;
	keyInfo.Type = WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE;
	m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
}

/*! \fn void CScadaVariableModule::ShowAnalogAlarmLimitView(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowAnalogAlarmLimitView 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowAnalogAlarmLimitView(const QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = tr("Scada pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}


	Q_ASSERT(m_pCore);
	if (!m_pCore)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	auto pAlarm = reinterpret_cast<Config::CScadaAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAlarm);
	if (!pAlarm)
	{
		auto strError = tr("analog alarm pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	if (!CheckTagNameIsValid(pAlarm->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto strTagName = index.data(Qt::EditRole).toString();
	if (!CheckTagNameIsValid(strTagName,SCADAVARIABLE_DESC))
	{
		return;
	}
	//判断是否存在该widget
	auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		return;
	}

	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	auto pView = new CScadaAnalogAlarmLimitView(nullptr, m_pCore, pAlarm, pItem, m_pScadaVirableConfig, pScada);
	auto icon = pItem->icon();
	TabKeyStruct keyInfo;
	keyInfo.BigTagName = pScada->m_szTagName;
	keyInfo.TagName = strTagName;
	keyInfo.Type = WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE;
	m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
}

/*! \fn void CScadaVariableModule::DeleteAnalogAlarmItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteAnalogAlarmItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteAnalogAlarmItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the analog alarm, the data will be deleted!!!"));
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}
		
		auto pGroupParent = reinterpret_cast<Config::CScadaAnalogAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pAlarmItem = reinterpret_cast<Config::CScadaAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAlarmItem);
		if (!pAlarmItem)
		{
			return;
		}

		Q_ASSERT(m_pScadaVirableConfig);
		if (!m_pScadaVirableConfig)
		{
			return;
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			return;
		}

		//判断是否存在该widget
		auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_ANALOG_ALARM_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			//删除模拟量告警窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		auto strTagNameTmp = strTag + "_limit";
		bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagNameTmp, WIDGET_TYPE::SCADA_ANALOG_ALARM_LIMIT_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			//删除模拟量告警窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		//删除hash
		//删除告警hash
		pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMID, strTag);
		//删除告警限值hash
		auto &arr = pAlarmItem->m_arrAlarmOfflimit;
		for each (auto var in arr)
		{
			auto strTmp = QString("%1").arg(var->m_szTagName);
			pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::ANALOGALARMLIMITID, strTmp);
		}



		if (!pGroupParent->DeleteItem(pAlarmItem))
		{
			auto strError = tr("delete analog alarm fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//删除关联 		//删除scada变量alarm关联的告警
		bFlag = pScada->DeleteUserVariableAlarmArr(strTag.toStdString());
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = tr("Delete UserVariable related Alarm %1!!!").arg(strTag);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());
	}

	}
}

/*! \fn void CScadaVariableModule::DeleteDigitalAlarmGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteDigitalAlarmGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月14日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteDigitalAlarmGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}

		auto pGroupParent = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		//删除选中组
		auto pDigitalAlarmGroup = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarmGroup);
		if (!pDigitalAlarmGroup)
		{
			return;
		}

		QVector<QModelIndex> vec;
		vec.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD, vec);
		for each (auto var in vec)
		{
			auto strTag = var.data(Qt::EditRole).toString();
			if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
			{
				return;
			}

			//判断是否存在该widget
			auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE, pScada->m_szTagName);
			if (bFlag)
			{
				//删除模拟量告警窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIbase);
				pIbase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
			else
			{
				//测试
				int a = 5;
			}

			auto strTagNameTmp = strTag + "_limit";
			bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagNameTmp, WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE, pScada->m_szTagName);
			if (bFlag)
			{
				//删除模拟量告警窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIbase);
				pIbase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
			else
			{
				//测试
				int a = 5;
			}

			//删除hash
			//删除告警hash
			if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMID, strTag))
			{
				auto strError = QObject::tr("Hash %1 delete failed!!!").arg(strTag);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
				
				return;
			}

			//开关量告警删除,用户变量绑定关联的开关量告警也跟着删除
			bFlag = pScada->DeleteUserVariableAlarmArr(strTag.toStdString());
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				auto strError = QObject::tr("Hash %1 delete failed!!!").arg(strTag);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			//删除告警限值hash
			auto pAlarm = reinterpret_cast<Config::CScadaDigitalAlarm *>(var.data(POINTERDATA).toLongLong());
			Q_ASSERT(pAlarm);
			if (!pAlarm)
			{
				return;
			}
			auto &arr = pAlarm->m_arrDigtallimit;
			for each (auto var in arr)
			{
				auto strTmp = QString("%1").arg(var->TagName);
				if (!CheckTagNameIsValid(strTmp, SCADAVARIABLE_DESC))
				{
					return;
				}

				if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID, strTmp))
				{
					auto strError = QObject::tr("Hash %1 delete failed!!!").arg(strTmp);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}
		}

		if (!pGroupParent->DeleteGroup(pDigitalAlarmGroup))
		{
			auto strError = tr("delete group fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn void CScadaVariableModule::DeleteDigitalAlarmItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteDigitalAlarmItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月14日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::DeleteDigitalAlarmItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the digital alarm, the data will be deleted!!!"));
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			auto strError = QObject::tr("Scada pointer is null!!!");
			m_pCore->LogMsg(SCADASTUDIO_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}


		auto pGroupParent = reinterpret_cast<Config::CScadaDigitalAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pAlarmItem = reinterpret_cast<Config::CScadaDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAlarmItem);
		if (!pAlarmItem)
		{
			return;
		}
		if (!CheckTagNameIsValid(pAlarmItem->m_szTagName, SCADAVARIABLE_DESC))
		{
			return;
		}

		Q_ASSERT(m_pScadaVirableConfig);
		if (!m_pScadaVirableConfig)
		{
			return;
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			return;
		}


		//判断是否存在该widget
		auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCADA_DIGITAL_ALARM_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			//删除模拟量告警窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		auto strTagNameTmp = strTag + "_limit";
		bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagNameTmp, WIDGET_TYPE::SCADA_DIGITAL_ALARM_LIMIT_TYPE, pScada->m_szTagName);
		if (bFlag)
		{
			//删除模拟量告警窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		//删除hash
		//删除告警hash
		pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMID, strTag);
		//删除告警限值hash
		auto &arr = pAlarmItem->m_arrDigtallimit;
		for each (auto var in arr)
		{
			auto strTmp = QString("%1").arg(var->TagName);
			if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::DIGITALALARMLIMITID, strTmp))
			{
				auto strError = QObject::tr("Hash %1 delete failed!!!").arg(strTmp);
				m_pCore->LogMsg(SCADASTUDIO_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

		}

		//删除组内的项
		if (!pGroupParent->DeleteItem(pAlarmItem))
		{
			auto strError = tr("delete digital alarm fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//删除关联的告警hash
		bFlag = pScada->DeleteUserVariableAlarmArr(strTag.toStdString());
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = tr("Delete UserVariable Related Alarm %1 fail!!!").arg(strTag);
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());
	}

	}
}

/*! \fn void CScadaVariableModule::CreateFesUi(Config::CScadaVariableConfigData *m_pConfig)
********************************************************************************************************* 
** \brief CScadaVariableModule::CreateFesUi 
** \details 
** \param m_pConfig 
** \return void 
** \author gw
** \date 2017年1月16日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::CreateFesUi(Config::CScadaVariableConfigData *pConfig)
{
	Q_UNUSED(pConfig);
}

/*! \fn bool CScadaVariableModule::InitAIAlarmGroup(CQuoteItem *pItem, Config::CScadaAnalogAlarmGroup *pGroup)
********************************************************************************************************* 
** \brief CScadaVariableModule::InitAIAlarmGroup 
** \details 创建模拟量告警UI
** \param pItem 
** \param pGroup 
** \return bool 
** \author gw
** \date 2017年1月16日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::InitAIAlarmGroup(CQuoteItem *pItem, Config::CScadaAnalogAlarmGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}



	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{

		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitAIAlarmGroup(newItem, var);
	}

	auto &arrItem = pGroup->GetItem();
	for each (auto var in arrItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *pItemAlarm = new CQuoteItem(var->m_szTagName);
			pItemAlarm->setData(var->m_szTagName, LASTVALUE);
			pItemAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			pItemAlarm->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pItemAlarm->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
			pItemAlarm->setIcon(QIcon(SCALE_PNG));
			pItemAlarm->setEditable(true);

			InitAIOfflimit(pItemAlarm, var);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaVariableModule::InitAIOfflimit 
** \details 创建模拟量告警限值UI
** \param pItem 
** \param m_arrAlarmOfflimit 
** \return void 
** \author gw
** \date 2017年1月16日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::InitAIOfflimit(CQuoteItem *pItem, Config::CScadaAnalogAlarm *pAlarm)
{
	Q_ASSERT(pItem);

	auto strName = pItem->data(Qt::EditRole).toString();

	auto strLimitTagName = QString("%1_limit").arg(strName);
	auto strLimitDesc = QString(tr("%1_limit")).arg(strName);
	CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
	pLimitItem->setData(reinterpret_cast<long long>(pAlarm), POINTERDATA);
	pLimitItem->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_LIMIT_ITEM, Qt::UserRole);
	pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
	pLimitItem->setIcon(QIcon(ALARM_PNG));
	pItem->appendRow(pLimitItem);
}

/*! \fn bool CScadaVariableModule::InitDIAlarmGroup(CQuoteItem *pItem, Config::CScadaDigitalAlarmGroup *pGroup)
********************************************************************************************************* 
** \brief CScadaVariableModule::InitDIAlarmGroup 
** \details 创建开关量告警UI
** \param pItem 
** \param pGroup 
** \return bool 
** \author gw
** \date 2017年1月17日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::InitDIAlarmGroup(CQuoteItem *pItem, Config::CScadaDigitalAlarmGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{
		//组
		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitDIAlarmGroup(newItem, var);
	}

	auto &arrItem = pGroup->GetItem();
	for each (auto var in arrItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *pItemAlarm = new CQuoteItem(var->m_szTagName);
			pItemAlarm->setData(var->m_szTagName, LASTVALUE);
			pItemAlarm->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			pItemAlarm->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pItemAlarm->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
			pItemAlarm->setIcon(QIcon(SCALE_PNG));
			pItemAlarm->setEditable(true);

			InitDIOfflimit(pItemAlarm, var);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

/*! \fn void CScadaVariableModule::InitDIOfflimit(CQuoteItem *pItem, Config::CScadaDigitalAlarm *pAlarm)
********************************************************************************************************* 
** \brief CScadaVariableModule::InitDIOfflimit 
** \details 
** \param pItem 
** \param pAlarm 
** \return void 
** \author gw
** \date 2017年1月17日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::InitDIOfflimit(CQuoteItem *pItem, Config::CScadaDigitalAlarm *pAlarm)
{
	Q_ASSERT(pItem);

	auto strName = pItem->data(Qt::EditRole).toString();

	auto strLimitTagName = QString("%1_limit").arg(strName);
	auto strLimitDesc = QString(tr("%1_limit")).arg(strName);
	CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
	pLimitItem->setData(reinterpret_cast<long long>(pAlarm), POINTERDATA);
	pLimitItem->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_LIMIT_ITEM, Qt::UserRole);
	pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
	pLimitItem->setIcon(QIcon(ALARM_PNG));
	pItem->appendRow(pLimitItem);
}

void CScadaVariableModule::InitTransfrom(CQuoteItem *pItem, Config::CScadaData *pScadaData)
{
	///////////////////
	//[1]
	//转换
	CQuoteItem *itemScale = new CQuoteItem(tr("scale list"));
	itemScale->setData(SCADAVARIABLE_ITEM_SCALE, Qt::UserRole);
	itemScale->setIcon(QIcon(SCALE_PNG));
	itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemScale->setData(reinterpret_cast<long long>(pScadaData), POINTERDATA);
	itemScale->setEditable(false);

	//线性转换 非线性转换
	auto pLinear = new CQuoteItem(tr("linear node"));
	pLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT, Qt::UserRole);
	pLinear->setIcon(QIcon(SCALE_PNG));
	pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScadaData->m_pScadaLinearGroup);
	if (!pScadaData->m_pScadaLinearGroup)
	{
		return;
	}
	pLinear->setData(reinterpret_cast<long long>(pScadaData->m_pScadaLinearGroup), POINTERDATA);
	pLinear->setEditable(false);
	InitTransformGroup(pLinear, pScadaData->m_pScadaLinearGroup);
	itemScale->appendRow(pLinear);
	auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	pNonLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT, Qt::UserRole);
	pNonLinear->setIcon(QIcon(SCALE_PNG));
	pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pScadaData->m_pScadaNonLinearGroup);
	if (!pScadaData->m_pScadaNonLinearGroup)
	{
		return;
	}
	pNonLinear->setData(reinterpret_cast<long long>(pScadaData->m_pScadaNonLinearGroup), POINTERDATA);
	pNonLinear->setEditable(false);
	InitNonTransformGroup(pNonLinear, pScadaData->m_pScadaNonLinearGroup);
	itemScale->appendRow(pNonLinear);
	pItem->appendRow(itemScale);
}

/*! \fn void CScadaVariableModule::AddScale(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddScale 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月17日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddLinearScale(QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(item);
	if (!item)
	{
		return;
	}
	
	auto pGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	auto pLinear = pScada->AddLinearTransform();
	Q_ASSERT(pLinear);
	if (!pLinear)
	{
		return;
	}

	Q_ASSERT(pLinear->m_pLinearScale);
	if (!pLinear->m_pLinearScale)
	{
		return;
	}


	//转换项添加到转换组里
	pGroupParent->AddTransformItem(pLinear);

	CQuoteItem *newItem = new CQuoteItem(pLinear->m_pLinearScale->m_szTagName);
	newItem->setData(pLinear->m_pLinearScale->m_szTagName, LASTVALUE);
	newItem->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM, Qt::UserRole);
	//newItem->setData(pTmp->m_szTagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pLinear), POINTERDATA);
	newItem->setData(pLinear->m_pLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);

	newItem->setIcon(QIcon(SCALE_PNG));

	item->appendRow(newItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

/*! \fn void CScadaVariableModule::AddScaleGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddScaleGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月17日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddLinearScaleGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg(SCALE_GROUP_NAME).arg(0);

	//auto nType = index.data(Qt::UserRole).toInt();
	//组深度
	auto nDeepth = index.data(GROUP_NUMBER_ROLE).toInt();
	//if (nType == SCADAVARIABLE_ITEM_SCALE_LINEAR)
	{
		//深度限制
		if (nDeepth < MAX_GROUP_NUM)
		{
			//线性组
			auto pLinearGroup = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pLinearGroup);
			if (!pLinearGroup)
			{
				return;
			}

			auto pGroupChild = pLinearGroup->CreateGroup();
			Q_ASSERT(pGroupChild);
			if (!pGroupChild)
			{
				return;
			}


			CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
			newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
			newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
			newItem->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_GROUP, Qt::UserRole);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
		}
	}
}

/*! \fn void CScadaVariableModule::AddNonLinearScale(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddNonLinearScale 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddNonLinearScale(QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(item);
	if (!item)
	{
		return;
	}

	auto pGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	auto pNonLinear = pScada->AddNonLinearTransform();
	Q_ASSERT(pNonLinear);
	if (!pNonLinear)
	{
		return;
	}

	//转换项添加到转换组里
	pGroupParent->AddTransformItem(pNonLinear);

	Q_ASSERT(pNonLinear->m_pNonLinearScale);
	if (!pNonLinear->m_pNonLinearScale)
	{
		return;
	}

	CQuoteItem *newItem = new CQuoteItem(pNonLinear->m_pNonLinearScale->m_szTagName);
	newItem->setData(pNonLinear->m_pNonLinearScale->m_szTagName, LASTVALUE);
	newItem->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM, Qt::UserRole);
	//newItem->setData(pTmp->m_szTagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pNonLinear), POINTERDATA);
	newItem->setData(pNonLinear->m_pNonLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);

	newItem->setIcon(QIcon(SCALE_PNG));

	item->appendRow(newItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

/*! \fn void CScadaVariableModule::AddNonLinearScaleGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::AddNonLinearScaleGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::AddNonLinearScaleGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg(SCALE_GROUP_NAME).arg(0);

	//auto nType = index.data(Qt::UserRole).toInt();
	//组深度
	auto nDeepth = index.data(GROUP_NUMBER_ROLE).toInt();
	//if (nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP)
	{
		//深度限制
		if (nDeepth < MAX_GROUP_NUM)
		{
			//线性组
			auto pNonLinearGroup = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pNonLinearGroup);
			if (!pNonLinearGroup)
			{
				return;
			}

			auto pGroupChild = pNonLinearGroup->CreateGroup();
			Q_ASSERT(pGroupChild);
			if (!pGroupChild)
			{
				return;
			}


			CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
			newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
			newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
			newItem->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP, Qt::UserRole);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
		}
	}
}

/*! \fn void CScadaVariableModule::ShowScaleLinearView(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowScaleLinearView 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowScaleLinearView(const QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto strTagName = index.data(Qt::EditRole).toString();
	auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("linear scale tagname %1 is invalid!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}


	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, SCADA_SCALE_LINEAR_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		return;
	}

	auto pTransform = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	if (!CheckTagNameIsValid(pTransform->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	CScadaLinearScaleView *pView = new CScadaLinearScaleView(nullptr, pTransform, (CQuoteItem *)pItem, this, m_pScadaVirableConfig, pScada);
	auto icon = pItem->icon();
	TabKeyStruct keyInfo;
	keyInfo.BigTagName = pScada->m_szTagName;
	keyInfo.TagName = strTagName;
	keyInfo.Type = WIDGET_TYPE::SCADA_SCALE_LINEAR_TYPE;
	m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo);
}


/*! \fn void CScadaVariableModule::ShowScaleNonLinearView(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::ShowScaleNonLinearView 
** \details 显示非先行转换view
** \param index 
** \return void 
** \author gw
** \date 2017年1月20日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ShowScaleNonLinearView(const QModelIndex &index)
{
	auto pScada = GetScada(index);
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}
	
	auto strTagName = index.data(Qt::EditRole).toString();
	auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("nonlinear scale tagname %1 is invalid!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, SCADA_SCALE_NONLINEAR_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		return;
	}

	auto pTransform = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	if (!CheckTagNameIsValid(pTransform->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(m_pScadaVirableConfig);
	if (!m_pScadaVirableConfig)
	{
		return;
	}

	CScadaNonlinearScaleView *pView = new CScadaNonlinearScaleView(nullptr, pTransform, (CQuoteItem *)pItem, this, m_pScadaVirableConfig);
	auto icon = pItem->icon();
	TabKeyStruct keyInfo;
	keyInfo.BigTagName = pScada->m_szTagName;
	keyInfo.TagName = strTagName;
	keyInfo.Type = WIDGET_TYPE::SCADA_SCALE_NONLINEAR_TYPE;

	m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
}

/*! \fn void CScadaVariableModule::ModifyScaleLinearItem(QStandardItem *item)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyScaleLinearItem 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2017年1月19日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyScaleLinearItem(QStandardItem *item)
{
	auto pScada = GetScada(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item));
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = tr("Scada item pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto strTagName = item->data(Qt::EditRole).toString();
	auto strLastTagName = item->data(LASTVALUE).toString();
	if (strTagName == strLastTagName)
	{	
		return;
	}

	auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC) && CheckTagNameIsValid(strLastTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = QObject::tr("Linear scale tagname length is invalid!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	if (pScada->CheckTagNameIsExist(strTagName))
	{
		auto strError = tr("linear scale tag name %1 is exist!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strLastTagName, Qt::EditRole);

		return;
	}

	auto *pTransform = reinterpret_cast<Config::CScadaTransform *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	if (!CheckTagNameIsValid(pTransform->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}
	

	auto pLinear = pTransform->GetLinear();
	Q_ASSERT(pLinear);
	if (!pLinear)
	{
		return;
	}
	if (!CheckTagNameIsValid(pLinear->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::SCADA_SCALE_LINEAR_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		TabKeyStruct newKeyInfo;
		TabKeyStruct lastKeyInfo;
		newKeyInfo.BigTagName = pScada->m_szTagName;
		newKeyInfo.TagName = strTagName;
		newKeyInfo.Type = WIDGET_TYPE::SCADA_SCALE_LINEAR_TYPE;
		lastKeyInfo.BigTagName = pScada->m_szTagName;
		lastKeyInfo.Type = WIDGET_TYPE::SCADA_SCALE_LINEAR_TYPE;
		lastKeyInfo.TagName = strLastTagName;

		m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

		IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pBase);
		if (!pBase)
		{
			return;
		}

		pBase->Refresh();
	}

	memset(pLinear->m_szTagName, 0, sizeof(pLinear->m_szTagName));
	int nSize = strTagName.size();
	strncpy(pLinear->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

	memset(pTransform->m_szTagName, 0, sizeof(pTransform->m_szTagName));
	strncpy(pTransform->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

	item->setData(strTagName, LASTVALUE);

	if (!pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID, strTagName, strLastTagName))
	{
		auto strError = QString("modify sacle fail!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//修改scada变量关联的转换 note
	bFlag = pScada->ModifyUserVariableScaleArr(strTagName.toStdString(), strLastTagName.toStdString(), pLinear->m_strDescription.toStdString(), ScaleType::LINEAR);
	if (!bFlag)
	{
		auto strError = QString("modify sacle relation fail!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//转换关联的用户变量表刷新
	if (!m_pCore->GetUIMgr()->GetTabWidget()->RefeshWidget(WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE))
	{
		QMessageBox box;
		box.setText(QObject::tr("widget refresh fail!!!"));

		return;
	}
}

/*! \fn void CScadaVariableModule::ModifyScaleNonLinearItem(QStandardItem *item)
*********************************************************************************************************
** \brief CScadaVariableModule::ModifyScaleNonLinearItem
** \details
** \param item
** \return void
** \author gw
** \date 2017年1月22日
** \note
********************************************************************************************************/
void CScadaVariableModule::ModifyScaleNonLinearItem(QStandardItem *item)
{
	auto strTagName = item->data(Qt::EditRole).toString();
	auto strLastTagName = item->data(LASTVALUE).toString();
	if (strTagName == strLastTagName)
	{
		return;
	}

	auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC) && CheckTagNameIsValid(strLastTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		auto strError = tr("Scada nonlinear tagname is invalid!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto pScada = GetScada(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item));
	Q_ASSERT(pScada);
	if (!pScada)
	{
		auto strError = tr("Scada item pointer is nullptr!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	if (pScada->CheckTagNameIsExist(strTagName))
	{
		auto strError = tr("nonlinear scale tag name %1 is exist!!!").arg(strTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strLastTagName, Qt::EditRole);

		return;
	}

	auto *pTransform = reinterpret_cast<Config::CScadaTransform *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return;
	}
	if (!CheckTagNameIsValid(pTransform->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	auto pNonLinear = pTransform->GetNonLinear();
	Q_ASSERT(pNonLinear);
	if (!pNonLinear)
	{
		return;
	}
	if (!CheckTagNameIsValid(pTransform->m_szTagName, SCADAVARIABLE_DESC))
	{
		return;
	}

	//判断是否存在该widget
	bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::SCADA_SCALE_NONLINEAR_TYPE, pScada->m_szTagName);
	if (bFlag)
	{
		TabKeyStruct keyInfo;
		TabKeyStruct lastKeyInfo;
		keyInfo.BigTagName = pScada->m_szTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::SCADA_SCALE_NONLINEAR_TYPE;
		lastKeyInfo.BigTagName = pScada->m_szTagName;
		lastKeyInfo.TagName = strLastTagName;
		lastKeyInfo.Type = WIDGET_TYPE::SCADA_SCALE_NONLINEAR_TYPE;

		m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(keyInfo, lastKeyInfo);

		IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pBase);
		if (!pBase)
		{
			return;
		}

		pBase->Refresh();
	}

	memset(pNonLinear->m_szTagName, 0, sizeof(pNonLinear->m_szTagName));
	int nSize = strTagName.size();
	strncpy(pNonLinear->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

	memset(pTransform->m_szTagName, 0, sizeof(pTransform->m_szTagName));
	strncpy(pTransform->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

	item->setData(strTagName, LASTVALUE);

	if (!pScada->ModifyHashMap(SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID, strTagName, strLastTagName))
	{
		auto strError = QString("modify sacle fail!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//非线性转换,scada用户变量关联的非线性转换转换也跟着修改 note
	bFlag = pScada->ModifyUserVariableScaleArr(strTagName.toStdString(), strLastTagName.toStdString(), pNonLinear->m_strDescription.toStdString(), ScaleType::NONLINEAR);
	if (!bFlag)
	{
		auto strError = QString("modify sacle relation fail!!!");
		m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	//转换关联的用户变量表刷新
	if (m_pCore->GetUIMgr()->GetTabWidget()->RefeshWidget(WIDGET_TYPE::SCADA_VARIABLE_USER_GROUP_TYPE))
	{
		QMessageBox box;
		box.setText(QObject::tr("widget refresh fail!!!"));
	}
}

/*! \fn void CScadaVariableModule::DeleteScaleLinearItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteScaleLinearItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年1月19日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::DeleteScaleLinearItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the scale, the scale data will be deleted. if scale tagname is related to AIs, AIs relation scale will be deleted.Do u comform to delete the item?");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return false;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return false;
		}

 		auto pGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return false;
		}

		auto pItem = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}
		if (!CheckTagNameIsValid(pItem->m_szTagName, SCADAVARIABLE_DESC))
		{
			return false;
		}


		Q_ASSERT(m_pScadaVirableConfig);
		if (!m_pScadaVirableConfig)
		{
			return false;
		}


		if (!DeleteLinearScaleWidgetInTabWidget(index, pScada))
		{
			auto strError = tr("delete linear scale widget fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC))
		{
			return false;
		}
		//组内的转换项删除 删除内存
		if (!pGroupParent->DeleteItem(pItem))
		{
			auto strError = tr("delete scale item fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//hash管理删除转换 不删除内存
		if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID, strTag))
		{
			auto strError = tr("delete scale hash %1 fail!!!").arg(strTag);

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//////////////////////////////关联删除/////////////////////////////////////
		//删除转换，如果scada变量的scale关联该转换，那么转换关系删除
		auto bFlag = pScada->DeleteUserVariableScaleArr(strTag.toStdString());
		if (!bFlag)
		{
			auto strError = QString("delete sacle relation fail!!!");
			m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());

		break;
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}

	return true;
}

/*! \fn bool CScadaVariableModule::DeleteScaleNonLinearItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteScaleNonLinearItem 
** \details 
** \param index 
** \return bool 
** \author gw
** \date 2017年1月22日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::DeleteScaleNonLinearItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the scale, the scale data will be deleted. if scale tagname is related to Others, relation scale will be deleted.Do u comform to delete the item?");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
		case QMessageBox::Yes:
		{
			auto pScada = GetScada(index);
			Q_ASSERT(pScada);
			if (!pScada)
			{
				return false;
			}
			if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
			{
				return false;
			}

			auto pGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return false;
			}

			auto pItem = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pItem);
			if (!pItem)
			{
				return false;
			}
			if (!CheckTagNameIsValid(pItem->m_szTagName, SCADAVARIABLE_DESC))
			{
				return false;
			}

			Q_ASSERT(m_pScadaVirableConfig);
			if (!m_pScadaVirableConfig)
			{
				return false;
			}

			if (!DeleteNonLinearScaleWidgetInTabWidget(index, pScada))
			{
				auto strError = tr("delete nonlinear scale widget fail!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			auto strTag = index.data(Qt::EditRole).toString();
			auto bFlag = CheckTagNameIsValid(strTag, SCADAVARIABLE_DESC);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return false;
			}
			//组内的通道项删除 删除通道内存
			if (!pGroupParent->DeleteItem(pItem))
			{
				auto strError = tr("delete nonscale item fail!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			//hash管理删除转换 不删除内存
			if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID, strTag))
			{
				auto strError = tr("delete nonscale hash fail!!!");

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			//////////////////////////////////关联///////////////////////////////////////////////
			//转换删除,用户变量绑定关联的转换也跟着删除
			bFlag = pScada->DeleteUserVariableScaleArr(strTag.toStdString());
			if (!bFlag)
			{
				auto strError = QString("delete sacle relation fail!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
			pTop->parent()->removeRow(pTop->row());

			break;
		}
		case QMessageBox::Cancel:
			// Cancel was clicked
			break;
		default:
			// should never be reached
			break;
	}

	return true;
}

bool CScadaVariableModule::DeleteLinearScaleWidgetInTabWidget(const QModelIndex &index, Config::CScadaData *pScada)
{
	auto pTransform = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return false;
	}

	auto pLinear = pTransform->GetLinear();
	Q_ASSERT(pLinear);
	if (!pLinear)
	{
		return false;
	}

	QString strTagName = pLinear->m_szTagName;
	auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return false;
	}

	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_SCALE_LINEAR_TYPE, pScada->m_szTagName))
	{
		//删除打开的前置窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIBase);
		if (!pIBase)
		{
			return false;
		}

		pIBase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	return true;
}

/*! \fn bool CScadaVariableModule::DeleteNonLinearScaleWidgetInTabWidget(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteNonLinearScaleWidgetInTabWidget 
** \details 删除tabwidget里的窗口
** \param index 
** \return bool 
** \author gw
** \date 2017年1月22日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::DeleteNonLinearScaleWidgetInTabWidget(const QModelIndex &index, Config::CScadaData *pScada)
{
	if (!index.isValid())
	{
		return false;
	}
	
	auto pTransform = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pTransform);
	if (!pTransform)
	{
		return false;
	}
	if (!CheckTagNameIsValid(pTransform->m_szTagName, SCADAVARIABLE_DESC))
	{
		return false;
	}

	auto pNonLinear = pTransform->GetNonLinear();
	Q_ASSERT(pNonLinear);
	if (!pNonLinear)
	{
		return false;
	}
	if (!CheckTagNameIsValid(pNonLinear->m_szTagName, SCADAVARIABLE_DESC))
	{
		return false;
	}

	QString strTagName = pNonLinear->m_szTagName;
	auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return false;
	}

	if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
	{
		return false;
	}
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCADA_SCALE_NONLINEAR_TYPE, pScada->m_szTagName))
	{
		//删除打开的前置窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIBase);
		if (!pIBase)
		{
			return false;
		}

		pIBase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	return true;
}

/*! \fn bool CScadaVariableModule::DeleteScaleLinearGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteScaleLinearGroup 
** \details 
** \param index 
** \return bool 
** \author gw
** \date 2017年1月19日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::DeleteScaleLinearGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
		case QMessageBox::Yes:
		{
			auto pScada = GetScada(index);
			Q_ASSERT(pScada);
			if (!pScada)
			{
				return false;
			}
			if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
			{
				return false;
			}

			//被删除组的父组
			auto pLinearGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pLinearGroupParent);
			if (!pLinearGroupParent)
			{
				return false;
			}

			//被删除组
			auto pLinearGroup = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pLinearGroup);
			if (!pLinearGroup)
			{
				return false;
			}

			QVector<QModelIndex> vec;
			vec.clear();
			GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM, vec);
			//删除前置hash内的线性转换
			for each (auto var in vec)
			{
				//关闭打开的窗口
				DeleteLinearScaleWidgetInTabWidget(var, pScada);

				auto strTagName = var.data(Qt::EditRole).toString();
				if (!CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC))
				{
					return false;
				}
				if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::LINEARTRANSFORMID, strTagName))
				{
					auto strError = tr("delete linear tagname %1 fail").arg(strTagName);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}

				//删除转换，如果scada变量的scale关联该转换，那么转换关系删除
				auto bFlag = pScada->DeleteUserVariableScaleArr(strTagName.toStdString());
				if (!bFlag)
				{
					auto strError = QString("delete sacle relation fail!!!");
					m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					return false;
				}

			}

			//删除内存结构 组和项
			if (!pLinearGroupParent->DeletGroup(pLinearGroup))
			{
				auto strError = tr("delete linear group %1 fail").arg(index.data(Qt::EditRole).toString());
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
			pTop->parent()->removeRow(pTop->row());
		}
		case QMessageBox::Cancel:
			// Cancel was clicked
			break;
		default:
			// should never be reached
			break;
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaVariableModule::DeleteScaleNonLinearGroup 
** \details 
** \param index 
** \return bool 
** \author gw
** \date 2017年1月22日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::DeleteScaleNonLinearGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		auto pScada = GetScada(index);
		Q_ASSERT(pScada);
		if (!pScada)
		{
			return false;
		}
		if (!CheckTagNameIsValid(pScada->m_szTagName, SCADAVARIABLE_DESC))
		{
			return false;
		}

		//被删除组的父组
		auto pLinearGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pLinearGroupParent);
		if (!pLinearGroupParent)
		{
			return false;
		}

		//被删除组
		auto pNonLinearGroup = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pNonLinearGroup);
		if (!pNonLinearGroup)
		{
			return false;
		}

		QVector<QModelIndex> vec;
		vec.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM, vec);
		//删除前置hash内的线性转换
		for each (auto var in vec)
		{
			//关闭打开的窗口
			DeleteNonLinearScaleWidgetInTabWidget(var, pScada);

			auto strTagName = var.data(Qt::EditRole).toString();
			auto bFlag = CheckTagNameIsValid(strTagName, SCADAVARIABLE_DESC);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				auto strError = tr("delete nonlinear tagname %1 length is invalid!!!").arg(strTagName);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			if (!pScada->DeleteTagNameHashMap(SCADA_VARIABLE_HASH_ID::NONLINEARTRANSFORMID, strTagName))
			{
				auto strError = tr("delete nonlinear hash %1 failed!!!").arg(strTagName);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			//转换删除，用户变量关联转换也删除
			bFlag = pScada->DeleteUserVariableScaleArr(strTagName.toStdString());
			if (!bFlag)
			{
				auto strError = QString("delete sacle relation fail!!!");
				m_pCore->LogMsg(SCADAVARIABLE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}

		//删除内存结构 组和项
		if (!pLinearGroupParent->DeletGroup(pNonLinearGroup))
		{
			auto strError = tr("delete nonlinear group %1 fail").arg(index.data(Qt::EditRole).toString());
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		auto pTop = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		pTop->parent()->removeRow(pTop->row());
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}

	return true;
}

/*! \fn void CScadaVariableModule::ModifyScaleLinearGroup(QStandardItem *item)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyScaleLinearGroup 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2017年1月19日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyScaleLinearGroup(QStandardItem *item)
{
	Q_ASSERT(item);
	if (!item)
	{
		return;
	}

	auto strScaleLastTagValue = item->data(LASTVALUE).toString();
	auto strScaleTagName = item->data(Qt::EditRole).toString();
	if (strScaleLastTagValue == strScaleTagName)
	{
		return;
	}

	auto index = m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item);
	if (!index.isValid())
	{
		return;
	}

	//父组
	auto pGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	//子组
	auto pGroupChild = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupChild);
	if (!pGroupChild)
	{
		return;
	}

	//检查父组下的子组是否存在该组名
	if (pGroupParent->CheckModifyGroupNameExist(pGroupChild, strScaleTagName))
	{
		auto strError = QString(tr("transform group name %1 exist!!!")).arg(strScaleTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strScaleLastTagValue, Qt::EditRole);

		return;
	}

	pGroupChild->SetGroupName(strScaleTagName);
	item->setData(strScaleTagName, LASTVALUE);
	item->setData(strScaleTagName, TREE_TOOL_TIP_ROLE);
}

/*! \fn void CScadaVariableModule::ModifyScaleNonLinearGroup(QStandardItem *item)
********************************************************************************************************* 
** \brief CScadaVariableModule::ModifyScaleNonLinearGroup 
** \details 
** \param item 
** \return void 
** \author gw
** \date 2017年1月22日 
** \note 
********************************************************************************************************/
void CScadaVariableModule::ModifyScaleNonLinearGroup(QStandardItem *item)
{
	Q_ASSERT(item);
	if (!item)
	{
		return;
	}

	auto strScaleLastTagValue = item->data(LASTVALUE).toString();
	auto strScaleTagName = item->data(Qt::EditRole).toString();
	if (strScaleLastTagValue == strScaleTagName)
	{
		return;
	}

	auto index = m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item);
	if (!index.isValid())
	{
		return;
	}

	//父组
	auto pGroupParent = reinterpret_cast<Config::CScadaTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	//子组
	auto pGroupChild = reinterpret_cast<Config::CScadaTransformGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupChild);
	if (!pGroupChild)
	{
		return;
	}

	//检查父组下的子组是否存在该组名
	if (pGroupParent->CheckModifyGroupNameExist(pGroupChild, strScaleTagName))
	{
		auto strError = QString(tr("transform group name %1 exist!!!")).arg(strScaleTagName);
		m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

		item->setData(strScaleLastTagValue, Qt::EditRole);

		return;
	}

	pGroupChild->SetGroupName(strScaleTagName);
	item->setData(strScaleTagName, LASTVALUE);
	item->setData(strScaleTagName, TREE_TOOL_TIP_ROLE);
}

/*! \fn bool CScadaVariableModule::ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
********************************************************************************************************* 
** \brief CScadaVariableModule::ChangeTagNameNodify 
** \details 前置DEVICE_AI DEVICE_DI DEVICE_AO DEVICE_DO 点tagname修改,通知绑定点的模块做出相应的修改 删除tagname为空 
** \param tagName 
** \param lastTagName 
** \param nType 
** \return bool 
** \author gw
** \date 2017年5月24日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
{
	//前置DEVICE_AI DEVICE_DI DEVICE_AO DEVICE_DO       
	Q_UNUSED(nType);
	m_pScadaVirableConfig->ChangeTagNameNodify(tagName, lastTagName, nType);

	return true;
}

/*! \fn bool CScadaVariableModule::GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
********************************************************************************************************* 
** \brief CScadaVariableModule::GetDesPoolArr 
** \details 字符串内存池arr
** \param  
** \param pHash 
** \param pStringPoolVec 
** \param pDescStringPoolOccNo 
** \return bool 
** \author gw
** \date 2017年3月14日 
** \note 
********************************************************************************************************/
bool CScadaVariableModule::GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
{
	Q_ASSERT(pHash);
	if (!pHash)
	{
		return false;
	}
	m_pHash = pHash;

	Q_ASSERT(pStringPoolVec);
	if (!pStringPoolVec)
	{
		return false;
	}
	m_pStringPoolVec = pStringPoolVec;

	Q_ASSERT(pDescStringPoolOccNo);
	if (!pDescStringPoolOccNo)
	{
		return false;
	}
	m_pDescStringPoolOccNo = pDescStringPoolOccNo;

	return true;
}
