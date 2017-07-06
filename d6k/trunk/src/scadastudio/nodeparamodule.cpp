/*! @file
<PRE>
********************************************************************************
模块名       :  节点参数模块
文件名       :  nodeparamodel.cpp
文件实现功能 :  节点配置模块
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
*  @date    2015.11.23
*/

#include <QDir>
#include <QDomDocument>

#include "nodeparamodule.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "scadastudio/icore.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/iprojmgr.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/tabwidget.h"
#include "nodeserverview.h"
#include "config_data.h"
#include "nodefesgroupview.h"
#include "nodeworkstationview.h"
#include "nodeworkstationgroupview.h"
#include "nodefesview.h"
#include "global.h"

#include <QMenu>
#include <QMessageBox>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CNodeParaModel::CNodeParaModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CNodeParaModel::CNodeParaModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
CNodeParaModule::CNodeParaModule()
	:m_pCore(nullptr), m_pUi(nullptr), m_pNodeData(nullptr), m_pHash(nullptr), m_pStringPoolVec(nullptr), m_pDescStringPoolOccNo(nullptr)
{
	
}

/*! \fn CNodeParaModel::~CNodeParaModel()
********************************************************************************************************* 
** \brief CNodeParaModel::~CNodeParaModel 
** \details 
** \return  
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
CNodeParaModule::~CNodeParaModule()
{
	if (m_pNodeData)
	{
		delete m_pNodeData;
		m_pNodeData = nullptr;
	}
}

void CNodeParaModule::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	m_pUi = pCore->GetUIMgr();

	//connect(m_pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));
	//connect(m_pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
	connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(ItemChanged(QStandardItem *)));

	//CQuoteItem *pTop = m_pUi->GetTopItem();
	//
	////对象节点
	//CQuoteItem *pItem = new CQuoteItem(QIcon(NODE_PNG), tr("node config"));

	//pItem->setData(NODE_ITEM, Qt::UserRole);

	//pItem->setEditable(false);

	//pTop->appendRow(pItem);

	m_pNodeData = new Config::CNodeConfigData();
}

/*! \fn bool CNodeParaModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CNodeParaModule::SaveProject 
** \details 保存工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CNodeParaModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	QDomDocument *pDom = m_pCore->GetProjMgr()->GetDocument();

	if (!pDom)
	{
		m_pCore->LogMsg(FES_DESC, (char *)QStringLiteral("打开文件失败!!!").toStdString().c_str(), 1);

		return false;
	}

	QDomNode n = pRoot->firstChild();

	while (!n.isNull())
	{
		QDomElement e = n.toElement();
		if (!e.isNull())
		{
			QString strTmp = e.tagName();
			if (strTmp.compare("node") == 0)
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

				m_pNodeData->SaveNodeFiles(pXml, e, szRoot, m_pHash, m_pStringPoolVec, m_pDescStringPoolOccNo);
			}
		}

		n = n.nextSibling();
	}
	
	return true;
}

/*! \fn bool CNodeParaModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CNodeParaModule::LoadProject 
** \details 加载工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CNodeParaModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	
	QString strTmp = m_pCore->GetProjMgr()->GetDocument()->documentElement().tagName();

	QDomNode n = m_pCore->GetProjMgr()->GetDocument()->documentElement().firstChild();

	//QDomNode pTmp = pRoot->firstChild();

	QString strPath = "";

	while (!n.isNull())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if (!e.isNull())
		{
			QString strTmp = e.tagName();// the node really is an element.

			if (strTmp.compare("node") == 0)
			{
				QDomNode n = e.firstChild();

				while (!n.isNull())
				{
					QDomElement e = n.toElement(); // try to convert the node to an element.

					strTmp = e.tagName();
					if (strTmp.compare("n") == 0)
					{
						QString strFile = e.attribute("file");

						strPath = strFile;
					}

					n = n.nextSibling();
				}
			}
		}

		n = n.nextSibling();
	}

	if (strPath.isEmpty())
	{
		//auto strError = tr("node load error!!!");
		//m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

		m_pNodeData->Clear();
		m_pNodeData->Create();

		CreateUi();

		return false;
	}

	m_pNodeData->Clear();
	m_pNodeData->Create();
	if (m_pNodeData->LoadNodeFile(strPath) && m_pNodeData->LoadNodeService())
	{
		CreateUi();
	}
	else
	{
		auto strError = tr("node load error!!!");
		m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	//CreateFesUi(vec);
	
	return true;
}

// 关闭工程文件
/*! \fn void CNodeParaModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
********************************************************************************************************* 
** \brief CNodeParaModule::CloseProject 
** \details 关闭工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param bSaveFlag 
** \return void 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
void CNodeParaModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(bSaveFlag);
}

// 新建一个空的工程文件
/*! \fn void CNodeParaModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
********************************************************************************************************* 
** \brief CNodeParaModule::CreateNewProject 
** \details 
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param pTopItem 
** \return void 
** \author gw
** \date 2016年8月30日 
** \note 
********************************************************************************************************/
void CNodeParaModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	QDir dir;
	bool ok = dir.mkdir(szRoot + "/" + "node");
	if (!ok)
	{
		auto strError = QString(tr("%1/node folder mkdir failed!!!")).arg(szRoot);

		m_pCore->LogMsg(NODE_CONFIG_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
	}

	m_pNodeData->Clear();
	m_pNodeData->Create();

	if (!m_pNodeData->LoadNodeService())
	{
		auto strError = QObject::tr("Load node service file fail!!!");
		m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto strTmp = tr("node");
	QDomElement root = pXml->createElement(strTmp);
	pRoot->appendChild(root);

	CQuoteItem *pNodeItem = new CQuoteItem(strTmp);
	pNodeItem->setData(NODE_ITEM, Qt::UserRole);
	pNodeItem->setData(reinterpret_cast<long long>(m_pNodeData), POINTERDATA);
	pNodeItem->setIcon(QIcon(NODE_PNG));
	pNodeItem->setEditable(false);

	//server
	auto strNodeServer = tr("server");
	//auto pServer = new Config::CNodeServer(m_pCore);
	//m_pNodeData->AddNodeData(pServer);

	CQuoteItem *pNodeServerItem = new CQuoteItem(strNodeServer);
	pNodeServerItem->setData(reinterpret_cast<long long>(m_pNodeData->m_pNodesServer), POINTERDATA);
	pNodeServerItem->setData(NODE_ITEM_SERVER, Qt::UserRole);
	pNodeServerItem->setIcon(QIcon(NODE_PNG));
	pNodeServerItem->setEditable(false);

	//fes
	auto strNodeFes = tr("fes");
	CQuoteItem *pNodeFesItem = new CQuoteItem(strNodeFes);
	pNodeFesItem->setData(NODE_ITEM_FES, Qt::UserRole);
	Q_ASSERT(m_pNodeData->m_pFesGroup);
	if (!m_pNodeData->m_pFesGroup)
	{
		return;
	}
	pNodeFesItem->setData(reinterpret_cast<long long>(m_pNodeData->m_pFesGroup), POINTERDATA);
	pNodeFesItem->setIcon(QIcon(NODE_PNG));
	pNodeFesItem->setEditable(false);

	//workstation
	auto strNodeWorkstatio = tr("workstation");
	CQuoteItem *pNodeWorkstationItem = new CQuoteItem(strNodeWorkstatio);
	pNodeWorkstationItem->setData(NODE_ITEM_WORKSTATION, Qt::UserRole);
	Q_ASSERT(m_pNodeData->m_pWorkstationGroup);
	if (!m_pNodeData->m_pWorkstationGroup)
	{
		return;
	}
	pNodeWorkstationItem->setData(reinterpret_cast<long long>(m_pNodeData->m_pWorkstationGroup), POINTERDATA);
	pNodeWorkstationItem->setIcon(QIcon(NODE_PNG));
	pNodeWorkstationItem->setEditable(false);

	pNodeItem->appendRow(pNodeServerItem);
	pNodeItem->appendRow(pNodeFesItem);
	pNodeItem->appendRow(pNodeWorkstationItem);

	pTopItem->appendRow(pNodeItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(pTopItem));
}

// 由子模块实现，由主模块调用
// 主程序判断，只要有任何一个子模块的“脏”标志存在，就推出对话框《是、否、取消》，如果是，则主模块调用所有子模块的保存接口
bool CNodeParaModule::GetModifyFlag() const
{
	return false;
}

/*! \fn bool CNodeParaModule::ChangeTagNameNodify(QString &tagName, QString &lastTagName)
********************************************************************************************************* 
** \brief CNodeParaModule::ChangeTagNameNodify 
** \details 节点模块 前置tagname修改，节点关联的前置tagname也跟着修改
** \param tagName 
** \param lastTagName 
** \return bool 
** \author gw
** \date 2017年4月23日 
** \note 
********************************************************************************************************/
bool CNodeParaModule::ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
{
	if (Module_ID::NODE_MODULE_FES == nType)
	{
		auto pFesGroup = m_pNodeData->m_pFesGroup;
		Q_ASSERT(pFesGroup);
		if (!pFesGroup)
		{
			return false;
		}

		auto bFlag = ModifyConfig(tagName, lastTagName, pFesGroup);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
	}
	else if (Module_ID::NODE_MODULE_SERVER == nType)
	{
		auto &arr = m_pNodeData->m_pNodesServer->m_arrNodes;

		for each (auto var in arr)
		{
			if (lastTagName.compare(var->m_strConfig) == 0)
			{
				var->m_strConfig = tagName;
			}
		}
	}
	else if (Module_ID::NODE_MODULE_WORKSTATION == nType)
	{
		auto pGroup = m_pNodeData->m_pWorkstationGroup;
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto bFlag = ModifyConfig(tagName, lastTagName, pGroup);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}
	}
	


	return true;
}

bool CNodeParaModule::ModifyConfig(QString &tagName, QString &lastTagName, Config::CNodeGroup *pGroup)
{
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}
	
	for each (auto var in pGroup->m_arrGroup)
	{
		if (!ModifyConfig(tagName, lastTagName, var))
		{
			return false;
		}
	}

	for each (auto var in pGroup->m_arrItem)
	{
		if (lastTagName.compare(var->m_strConfig) == 0)
		{
			var->m_strConfig = tagName;
		}
	}

	return true;
}

/*! \fn std::string & CNodeParaModule::GetFesNodeNameByFesConfigName(std::string &szFesCongfigName)
********************************************************************************************************* 
** \brief CNodeParaModule::GetFesNodeNameByFesConfigName 
** \details 前置tagname转换网络节点名
** \param szFesCongfigName 
** \return std::string & 
** \author gw
** \date 2017年4月27日 
** \note 
********************************************************************************************************/
std::string CNodeParaModule::GetNodeNameByFesConfigName(std::string &szFesCongfigName)
{
	QString strNodeTagName = "";
	m_pNodeData->m_pFesGroup->GetNodeTagNameByFesConfigTagName(szFesCongfigName.c_str(), strNodeTagName);


	return strNodeTagName.toStdString();
}

std::string CNodeParaModule::GetFesConfigNameByNodeName(std::string &szNodeName)
{
	QString strFesTagName = "";
	m_pNodeData->m_pFesGroup->GetFesConfigTagNameByNodeTagName(szNodeName.c_str(), strFesTagName);


	return strFesTagName.toStdString();
}

void CNodeParaModule::TestGetFesConfigName()
{
	if (m_pCore == nullptr)
		return;

	IModule *pFesModule = m_pCore->GetModule("FES");
	if(pFesModule)
	{
		std::vector<std::string > arrNames;
		pFesModule->GetFesConfigName(arrNames);
	}
}

/*! \fn void CNodeParaModule::DoubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::DoubleClicked 
** \details 节点模块双击槽函数
** \param index 
** \return void 
** \author gw
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
void CNodeParaModule::DoubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == NODE_ITEM_SERVER)
	{
		auto strNodeServerTagName = index.data(Qt::EditRole).toString();
		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strNodeServerTagName, WIDGET_TYPE::NODE_SERVER_TYPE);
		if (bFlag)
		{
			return;
		}
	
		auto pNode = reinterpret_cast<Config::CNodeServer *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pNode);
		if (!pNode)
		{
			return;
		}
		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		CNodeServerView *pView = new CNodeServerView(nullptr, m_pCore, pNode, m_pNodeData);
		m_pUi->GetTabWidget()->AddTab(pView, strNodeServerTagName, NODE_SERVER_TYPE, strNodeServerTagName);
	}
	else if (nType == NODE_ITEM_FES_CHILD_GROUP)
	{
		auto strNodeFesGroupTagName = index.data(Qt::EditRole).toString();
		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strNodeFesGroupTagName, WIDGET_TYPE::NODE_FES_GROUP_TYPE);
		if (bFlag)
		{
			return;
		}

		auto pNodeFesGroup = reinterpret_cast<Config::CNodeGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pNodeFesGroup);
		if (!pNodeFesGroup)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		CNodeFesGroupView *pView = new CNodeFesGroupView(nullptr, m_pCore, pNodeFesGroup, m_pNodeData);
		m_pUi->GetTabWidget()->AddTab(pView, strNodeFesGroupTagName, NODE_FES_GROUP_TYPE, strNodeFesGroupTagName);
	}
	else if (nType == NODE_ITEM_FES_CHILD_ITEM)
	{
		auto strNodeFesTagName = index.data(Qt::EditRole).toString();
		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strNodeFesTagName, WIDGET_TYPE::NODE_FES_TYPE);
		if (bFlag)
		{
			return;
		}

		auto pNode = reinterpret_cast<Config::CNode *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pNode);
		if (!pNode)
		{
			return;
		}

		CQuoteItem *pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		CNodeFesView *pView = new CNodeFesView(nullptr, m_pCore, pNode, pItem, m_pNodeData);
		m_pUi->GetTabWidget()->AddTab(pView, strNodeFesTagName, NODE_FES_TYPE, strNodeFesTagName);
	}
	else if (nType == NODE_ITEM_WORKSTATION_CHILD_ITEM)
	{
		auto strTagName = index.data(Qt::EditRole).toString();
		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::NODE_WORKSTATION_TYPE);
		if (bFlag)
		{
			return;
		}

		auto pData = reinterpret_cast<Config::CNode *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pData);
		if (!pData)
		{
			auto strError = tr("data is null!!!");
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		CQuoteItem *pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		CNodeWorkstationView *pView = new CNodeWorkstationView(nullptr, m_pCore, pData, pItem, m_pNodeData);
		m_pUi->GetTabWidget()->AddTab(pView, strTagName, NODE_WORKSTATION_TYPE, strTagName);
	}
	else if (nType == NODE_ITEM_WORKSTATION_CHILD_GROUP)
	{
		//节点模块 工作站组
		auto strTagName = index.data(Qt::EditRole).toString();
		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::NODE_WORKSTATION_GROUP_TYPE);
		if (bFlag)
		{
			return;
		}

		auto pData = reinterpret_cast<Config::CNodeGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pData);
		if (!pData)
		{
			auto strError = tr("data is null!!!");
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		CNodeWorkstationGroupView *pView = new CNodeWorkstationGroupView(nullptr, m_pCore, pData, m_pNodeData);
		m_pUi->GetTabWidget()->AddTab(pView, strTagName, NODE_WORKSTATION_GROUP_TYPE, strTagName);
	}
}

/*! \fn void CNodeParaModule::ShowMouseRightButton(const QPoint &point)
********************************************************************************************************* 
** \brief CNodeParaModule::ShowMouseRightButton 
** \details 右键菜单
** \param point 
** \return void 
** \author gw
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
#include "variant.h"
void CNodeParaModule::ShowMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	QMenu *pMenu = new QMenu(NULL);

	int nType = indexSelect.data(Qt::UserRole).toInt();
	if (nType == NODE_ITEM)
	{
		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());

			std::string strTmp;
			//m_pCore->SelectPoint(strTmp);

			std::vector<int> vec;
			vec.clear();
			vec.push_back(DT_DOUBLE);
			vec.push_back(DT_BOOLEAN);

			std::string strNodeName;
			std::string strTagName;
			std::string strAttrName;
			m_pCore->SelectTag(vec, false, strNodeName, strTagName, strAttrName);

		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (nType == NODE_ITEM_FES)
	{
		QAction *pNodeFesGroupAction = new QAction(ADD_NODE_FES_GROUP, pMenu);
		pNodeFesGroupAction->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pNodeFesGroupAction);

		QAction *pNodeFesItemAction = new QAction(ADD_NODE_FES_ITEM, pMenu);
		pNodeFesItemAction->setIcon(QIcon(NODE_PNG));
		pMenu->addAction(pNodeFesItemAction);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);
		pMenu->addAction(pActionCollapse);


		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pNodeFesGroupAction)
		{
			AddNodeFesGroup(indexSelect);
		}
		else if (selectedItem == pNodeFesItemAction)
		{
			AddNodeFesItem(indexSelect);
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
	//else if (nType == NODE_ITEM_WORKSTATION)
	//{
	//	QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
	//	QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
	//	pMenu->addAction(pActionExpand);
	//	pMenu->addAction(pActionCollapse);

	//	QAction *pActionMofidy = new QAction(RENAME_ITEM, pMenu);
	//	pMenu->addAction(pActionMofidy);
	//	pActionMofidy->setIcon(QIcon(CLOSE_GROUP_PNG));
	//	pMenu->addAction(pActionMofidy);

	//	QAction* selectedItem = pMenu->exec(QCursor::pos());

	//	if (selectedItem == pActionExpand)
	//	{
	//		ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
	//	}
	//	else if (selectedItem == pActionCollapse)
	//	{
	//		CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
	//	}
	//	else if (selectedItem == pActionMofidy)
	//	{
	//		m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
	//	}
	//}
	else if (nType == NODE_ITEM_FES_CHILD_GROUP ||
		nType == NODE_ITEM_FES_CHILD_ITEM)
	{
		QAction *pActionMofidy = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionMofidy);
		pActionMofidy->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionMofidy);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		
		if (selectedItem == pActionMofidy)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete)
		{
			if (nType == NODE_ITEM_FES_CHILD_GROUP)
			{
				DeleteFesNodeGroup(indexSelect);
			}
			else if (nType == NODE_ITEM_FES_CHILD_ITEM)
			{
				DeleteFesNodeItem(indexSelect);
			}
		}
	}
	else if (nType == NODE_ITEM_WORKSTATION)
	{
		QAction *pNodeWorkstationGroupAction = new QAction(ADD_NODE_WORKSTATION_GROUP, pMenu);
		pNodeWorkstationGroupAction->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pNodeWorkstationGroupAction);

		QAction *pNodeWorkstationAction = new QAction(ADD_NODE_WORKSTATION, pMenu);
		pNodeWorkstationAction->setIcon(QIcon(NODE_PNG));
		pMenu->addAction(pNodeWorkstationAction);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pNodeWorkstationGroupAction)
		{
			AddNodeWorkstationGroup(indexSelect);
		}
		else if (selectedItem == pNodeWorkstationAction)
		{
			AddNodeWorkstationItem(indexSelect);
		}
	}
	else if (nType == NODE_ITEM_WORKSTATION_CHILD_ITEM)
	{
		QAction *pActionMofidy = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionMofidy);
		pActionMofidy->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionMofidy);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionMofidy)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete)
		{
			DeleteWorkstationNodeItem(indexSelect);
		}
	}
	else if (nType == NODE_ITEM_WORKSTATION_CHILD_GROUP)
	{
		QAction *pActionMofidy = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionMofidy);
		pActionMofidy->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionMofidy);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionMofidy)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(indexSelect);
		}
		else if (selectedItem == pActionDelete)
		{
			DeleteWorkstationNodeGroup(indexSelect);
		}
	}

	pMenu->deleteLater();
}

/*! \fn void CNodeParaModule::AddNodeFesGroup(const QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::AddNodeFesGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
void CNodeParaModule::AddNodeFesGroup(const QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();


	auto pGroupParent = reinterpret_cast<Config::CNodeGroup *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto pGroupChild = m_pNodeData->AddNodeGroup(pGroupParent);
	CQuoteItem *newItem = new CQuoteItem(pGroupChild->m_strGroup);
	newItem->setData(pGroupChild->m_strGroup, LASTVALUE);
	newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
	newItem->setData(NODE_ITEM_FES_CHILD_GROUP, Qt::UserRole);
	newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
	newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
	newItem->setEditable(true);

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CNodeParaModule::AddNodeFesItem(const QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::AddNodeFesItem 
** \details 添加前置节点
** \param index 
** \return void 
** \author gw
** \date 2016年12月7日 
** \note 
********************************************************************************************************/
void CNodeParaModule::AddNodeFesItem(const QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	auto pGroupParent = reinterpret_cast<Config::CNodeGroup *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	Q_ASSERT(m_pNodeData);
	if (!m_pNodeData)
	{
		return;
	}

	auto pDataItem = m_pNodeData->AddNodeItem(pGroupParent);
	Q_ASSERT(pDataItem);
	if (!pDataItem)
	{
		return;
	}
	pDataItem->m_nNodeType = NODETYPE::FEP;

	auto pNodeFesItem = new CQuoteItem(pDataItem->m_szTagName);
	pNodeFesItem->setData(NODE_ITEM_FES_CHILD_ITEM, Qt::UserRole);
	pNodeFesItem->setData(pDataItem->m_szTagName, LASTVALUE);
	pNodeFesItem->setData(reinterpret_cast<long long>(pDataItem), POINTERDATA);
	pNodeFesItem->setData(pDataItem->m_strHostName, TREE_TOOL_TIP_ROLE);
	pNodeFesItem->setIcon(QIcon(NODE_PNG));
	item->appendRow(pNodeFesItem);

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CNodeParaModule::AddNodeWorkstationGroup(const QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::AddNodeWorkstationGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
void CNodeParaModule::AddNodeWorkstationGroup(const QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	Q_ASSERT(m_pNodeData);
	if (!m_pNodeData)
	{
		return;
	}

	auto pGroupParent = reinterpret_cast<Config::CNodeGroup *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto pGroupChild = m_pNodeData->AddNodeGroup(pGroupParent);
	Q_ASSERT(pGroupParent);
	if (!pGroupChild)
	{
		return;
	}

	CQuoteItem *newItem = new CQuoteItem(pGroupChild->m_strGroup);
	newItem->setData(pGroupChild->m_strGroup, LASTVALUE);
	//newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
	newItem->setData(NODE_ITEM_WORKSTATION_CHILD_GROUP, Qt::UserRole);
	newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
	newItem->setData(pGroupChild->m_strGroup, TREE_TOOL_TIP_ROLE);
	newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
	newItem->setEditable(true);

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CNodeParaModule::AddNodeWorkstationItem(const QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::AddNodeWorkstationItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月8日 
** \note 
********************************************************************************************************/
void CNodeParaModule::AddNodeWorkstationItem(const QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	Q_ASSERT(m_pNodeData);
	if (!m_pNodeData)
	{
		return;
	}

	auto pGroupParent = reinterpret_cast<Config::CNodeGroup *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto pDataItem = m_pNodeData->AddNodeItem(pGroupParent);
	pDataItem->m_strService = QObject::tr(ServiceConfig);
	//工作站节点默认service
	for each (auto var in m_pNodeData->m_arrStandTemplateService)
	{
		if (var->m_strName.compare(NORMAL_WORKSTATION, Qt::CaseInsensitive) == 0)
		{
			pDataItem->m_arrNodeServiceRole = var->m_arrServiceRole;
		}
	}

	pDataItem->m_nNodeType = NODETYPE::WORKSTAIOTN;
	Q_ASSERT(pDataItem);
	if (!pDataItem)
	{
		return;
	}

	auto pNodeWorkstationItem = new CQuoteItem(pDataItem->m_szTagName);
	pNodeWorkstationItem->setData(NODE_ITEM_WORKSTATION_CHILD_ITEM, Qt::UserRole);
	pNodeWorkstationItem->setData(pDataItem->m_szTagName, LASTVALUE);
	pNodeWorkstationItem->setData(reinterpret_cast<long long>(pDataItem), POINTERDATA);
	pNodeWorkstationItem->setData(pDataItem->m_strHostName, TREE_TOOL_TIP_ROLE);
	pNodeWorkstationItem->setIcon(QIcon(NODE_PNG));
	item->appendRow(pNodeWorkstationItem);

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CNodeParaModule::CreateUi(QString &strPath)
********************************************************************************************************* 
** \brief CNodeParaModule::CreateUi 
** \details 创建ui
** \param strPath 
** \return void 
** \author gw
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
void CNodeParaModule::CreateUi()
{
	Q_ASSERT(m_pNodeData);
	if (!m_pNodeData)
	{
		return;
	}

	auto strTmp = tr("node");
	CQuoteItem *pNodeItem = new CQuoteItem(strTmp);
	pNodeItem->setData(NODE_ITEM, Qt::UserRole);
	pNodeItem->setData(reinterpret_cast<long long>(m_pNodeData), POINTERDATA);
	pNodeItem->setIcon(QIcon(NODE_PNG));
	pNodeItem->setEditable(false);

	//server
	auto strNodeServer = tr("server");
	CQuoteItem *pNodeServerItem = new CQuoteItem(strNodeServer);
	pNodeServerItem->setData(NODE_ITEM_SERVER, Qt::UserRole);
	pNodeServerItem->setData(reinterpret_cast<long long>(m_pNodeData->m_pNodesServer), POINTERDATA);
	pNodeServerItem->setIcon(QIcon(NODE_PNG));
	pNodeServerItem->setEditable(false);


	//fes
	auto strNodeFes = tr("fes");
	CQuoteItem *pNodeFesItem = new CQuoteItem(strNodeFes);
	pNodeFesItem->setData(NODE_ITEM_FES, Qt::UserRole);
	Q_ASSERT(m_pNodeData->m_pFesGroup);
	if (!m_pNodeData->m_pFesGroup)
	{
		return;
	}
	pNodeFesItem->setData(reinterpret_cast<long long>(m_pNodeData->m_pFesGroup), POINTERDATA);
	pNodeFesItem->setIcon(QIcon(NODE_PNG));
	pNodeFesItem->setEditable(false);

	CreateFesNode(pNodeFesItem, m_pNodeData->m_pFesGroup);

	//workstation
	auto strNodeWorkstatio = tr("workstation");
	CQuoteItem *pNodeWorkstationItem = new CQuoteItem(strNodeWorkstatio);
	pNodeWorkstationItem->setData(NODE_ITEM_WORKSTATION, Qt::UserRole);
	Q_ASSERT(m_pNodeData->m_pWorkstationGroup);
	if (!m_pNodeData->m_pWorkstationGroup)
	{
		return;
	}
	pNodeWorkstationItem->setData(reinterpret_cast<long long>(m_pNodeData->m_pWorkstationGroup), POINTERDATA);
	pNodeWorkstationItem->setIcon(QIcon(NODE_PNG));
	pNodeWorkstationItem->setEditable(false);
	CreateWorkstationNode(pNodeWorkstationItem, m_pNodeData->m_pWorkstationGroup);

	pNodeItem->appendRow(pNodeServerItem);
	pNodeItem->appendRow(pNodeFesItem);
	pNodeItem->appendRow(pNodeWorkstationItem);

	m_pUi->GetTopItem()->appendRow(pNodeItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(m_pUi->GetTopItem()));
}

/*! \fn void CNodeParaModule::CreateFesNode(CQuoteItem *pItem, Config::CNodeGroup *pGroup)
********************************************************************************************************* 
** \brief CNodeParaModule::CreateFesNode 
** \details 创建前置节点
** \param pItem 
** \param pGroup 
** \return void 
** \author gw
** \date 2016年12月10日 
** \note 
********************************************************************************************************/
void CNodeParaModule::CreateFesNode(CQuoteItem *pItem, Config::CNodeGroup *pGroup)
{
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}
	
	for each (auto var in pGroup->m_arrGroup)
	{
		CQuoteItem *newItem = new CQuoteItem(var->m_strGroup);
		newItem->setData(var->m_strGroup, LASTVALUE);

		newItem->setData(NODE_ITEM_FES_CHILD_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(var->m_strGroup, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(true);
		pItem->appendRow(newItem);
	}

	for each (auto var in pGroup->m_arrItem)
	{
		CQuoteItem *newItem = new CQuoteItem(var->m_szTagName);
		newItem->setData(var->m_szTagName, LASTVALUE);

		newItem->setData(NODE_ITEM_FES_CHILD_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(var->m_strHostName, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(NODE_PNG));
		newItem->setEditable(true);
		pItem->appendRow(newItem);
	}
}

/*! \fn void CNodeParaModule::CreateWorkstationNode(CQuoteItem *pItem, Config::CNodeGroup *pGroup)
********************************************************************************************************* 
** \brief CNodeParaModule::CreateWorkstationNode 
** \details 创建工作站节点
** \param pItem 
** \param pGroup 
** \return void 
** \author gw
** \date 2016年12月10日 
** \note 
********************************************************************************************************/
void CNodeParaModule::CreateWorkstationNode(CQuoteItem *pItem, Config::CNodeGroup *pGroup)
{
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	for each (auto var in pGroup->m_arrGroup)
	{
		CQuoteItem *newItem = new CQuoteItem(var->m_strGroup);
		newItem->setData(var->m_strGroup, LASTVALUE);

		newItem->setData(NODE_ITEM_WORKSTATION_CHILD_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(var->m_strGroup, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(true);
		pItem->appendRow(newItem);
	}

	for each (auto var in pGroup->m_arrItem)
	{
		CQuoteItem *newItem = new CQuoteItem(var->m_szTagName);
		newItem->setData(var->m_szTagName, LASTVALUE);

		newItem->setData(NODE_ITEM_WORKSTATION_CHILD_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(var->m_strHostName, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(NODE_PNG));
		newItem->setEditable(true);
		pItem->appendRow(newItem);
	}
}

/*! \fn void CNodeParaModule::DeleteTreeItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::DeleteTreeItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月14日 
** \note 
********************************************************************************************************/
void CNodeParaModule::DeleteFesNodeGroup(QModelIndex &index)
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

		auto pGroupParent = reinterpret_cast<Config::CNodeGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CNodeGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::NODE_FES_GROUP_TYPE))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		//删除group hash管理 不删除实际前置内存
		auto bFlag = m_pNodeData->DeleteHashGroup(strTagName);
		Q_ASSERT(m_pNodeData);
		if (!bFlag)
		{
			auto strError = tr("delete node fes group fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		
		//Note 节点组内有节点项 在view里 节点项的hash关系也得删除
		for each (auto var in pGroup->m_arrItem)
		{
			bFlag = m_pNodeData->DeleteHashItem(var->m_szTagName);
			Q_ASSERT(m_pNodeData);
			if (!bFlag)
			{
				auto strError = tr("delete node fes item %1 fail!!!").arg(var->m_szTagName);

				m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			//网络节点tagname修改，
			//如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
			auto pFesModule = m_pCore->GetModule("fes");
			Q_ASSERT(pFesModule);
			if (!pFesModule)
			{
				return;
			}
			bool bFlag = pFesModule->ChangeTagNameNodify(QString(""), QString(var->m_szTagName), Module_ID::NODE_MODULE_FES);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				auto strError = tr("Fes Module ChangeTagNameNodify %1 fail!!!").arg(var->m_szTagName);
				m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
			}
		}


		if (!pGroupParent->DeletGroup(pGroup))
		{
			auto strError = tr("delete group fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
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

/*! \fn void CNodeParaModule::DeleteFesNodeItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::DeleteFesNodeItem 
** \details 删除前置项
** \param index 
** \return void 
** \author gw
** \date 2016年12月14日 
** \note 
********************************************************************************************************/
void CNodeParaModule::DeleteFesNodeItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the item, the item will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		auto pParentGroup = reinterpret_cast<Config::CNodeGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pParentGroup);
		if (!pParentGroup)
		{
			return;
		}

		auto pItem = reinterpret_cast<Config::CNode *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::NODE_FES_TYPE))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}


		//网络节点tagname修改，
		//如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
		auto pFesModule = m_pCore->GetModule("fes");
		Q_ASSERT(pFesModule);
		if (!pFesModule)
		{
			return;
		}
		bool bFlag = pFesModule->ChangeTagNameNodify(QString(""), strTagName, Module_ID::NODE_MODULE_FES);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = tr("Fes Module ChangeTagNameNodify %1 fail!!!").arg(strTagName);
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
		}

		//组内的通道项删除 删除通道内存
		if (!pParentGroup->DeleteItem(pItem))
		{
			auto strError = tr("delete fes node item fail!!!");
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//hash管理删除通道 不删除通道内存
		if (!m_pNodeData->DeleteHashItem(strTagName))
		{
			auto strError = tr("delete fes node item fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}


		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
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

		//QVector<QModelIndex> vec;
		//vec.clear();
		//GetItemMap(m_pModel, index, FES_ITEM, vec);
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

/*! \fn void CNodeParaModule::DeleteWorkstationNodeGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::DeleteWorkstationNodeGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月14日 
** \note 
********************************************************************************************************/
void CNodeParaModule::DeleteWorkstationNodeGroup(QModelIndex &index)
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

		auto pGroupParent = reinterpret_cast<Config::CNodeGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CNodeGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::NODE_WORKSTATION_GROUP_TYPE))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		//删除group hash管理 不删除实际前置内存
		auto bFlag = m_pNodeData->DeleteHashGroup(strTagName);
		if (!bFlag)
		{
			auto strError = tr("delete node fes group fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//删除item hash管理
		for each (auto var in pGroup->m_arrItem)
		{
			auto bFlag = m_pNodeData->DeleteHashItem(var->m_szTagName);
			if (!bFlag)
			{
				auto strError = tr("delete node fes item %1 hash fail!!!").arg(var->m_szTagName);

				m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}


		if (!pGroupParent->DeletGroup(pGroup))
		{
			auto strError = tr("delete group fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
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

/*! \fn void CNodeParaModule::DeleteWorkstationNodeItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CNodeParaModule::DeleteWorkstationNodeItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月14日 
** \note 
********************************************************************************************************/
void CNodeParaModule::DeleteWorkstationNodeItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the item, the item will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		auto pParentGroup = reinterpret_cast<Config::CNodeGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pParentGroup);
		if (!pParentGroup)
		{
			return;
		}

		auto pItem = reinterpret_cast<Config::CNode *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::NODE_WORKSTATION_TYPE))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		//组内的通道项删除 删除通道内存
		if (!pParentGroup->DeleteItem(pItem))
		{
			auto strError = tr("delete fes node item fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//hash管理删除通道 不删除通道内存
		if (!m_pNodeData->DeleteHashItem(strTagName))
		{
			auto strError = tr("delete fes node item fail!!!");

			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}


		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
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

		//QVector<QModelIndex> vec;
		//vec.clear();
		//GetItemMap(m_pModel, index, FES_ITEM, vec);
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

void CNodeParaModule::ItemChanged(QStandardItem *item)
{
	int nType = item->data(Qt::UserRole).toInt();

	if (nType == NODE_ITEM_FES_CHILD_GROUP)
	{
		auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		auto strTagName = index.data(Qt::EditRole).toString();
		auto strLastTagName = index.data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		auto pNodeGroup = reinterpret_cast<Config::CNodeGroup *>(index.data(POINTERDATA).toLongLong());
		if (!pNodeGroup)
		{
			auto strError = tr("node fes group pointer is nullptr!!!");
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			Q_ASSERT(m_pNodeData);
			if (!m_pNodeData)
			{
				return;
			}
			
			//判断修改的组名是否存在
			if (m_pNodeData->CheckGroupTagNameIsExist(strTagName))
			{
				item->setData(strLastTagName, Qt::EditRole);
				return;
			}
			
			auto pNodeGroupParent = reinterpret_cast<Config::CNodeGroup *>(item->parent()->data(POINTERDATA).toLongLong());
			Q_ASSERT(pNodeGroupParent);
			if (!pNodeGroupParent)
			{
				return;
			}

			//修改组内的组
			if (!pNodeGroupParent->ModifyGroup(strTagName, pNodeGroup))
			{
				return;
			}

			if (!m_pNodeData->ModifyGroupHashMap(strTagName, strLastTagName))
			{
				return;
			}

			//pNodeGroup->m_strGroup = strTagName;
			item->setData(strTagName, LASTVALUE);
			item->setData(strTagName, TREE_TOOL_TIP_ROLE);

			//判断是否存在该widget
			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::NODE_FES_GROUP_TYPE
				);
			if (bFlag)
			{
				m_pUi->GetTabWidget()->SetTabName(strTagName, WIDGET_TYPE::NODE_FES_GROUP_TYPE, strLastTagName);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}
		}
	}
	else if (nType == NODE_ITEM_FES_CHILD_ITEM)
	{
		auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		auto strTagName = index.data(Qt::EditRole).toString();
		auto strLastTagName = index.data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		if (!CheckTagNameIsValid(strTagName, NODE_CONFIG_DESC) || !CheckTagNameIsValid(strLastTagName, NODE_CONFIG_DESC))
		{
			item->setData(strLastTagName, Qt::EditRole);
			
			return;
		}

		auto pNode = reinterpret_cast<Config::CNode *>(index.data(POINTERDATA).toLongLong());
		if (!pNode)
		{
			return;
		}

		auto pNodeParent = reinterpret_cast<Config::CNodeGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pNodeParent);
		if (!pNodeParent)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		if (m_pNodeData->CheckTagNameIsExist(strTagName))
		{
			item->setData(strLastTagName, Qt::EditRole);
			
			return;
		}


		//网络节点tagname修改，
		//如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
		auto pFesModule = m_pCore->GetModule("fes");
		Q_ASSERT(pFesModule);
		if (!pFesModule)
		{
			return;
		}
		pFesModule->ChangeTagNameNodify(strTagName, strLastTagName , Module_ID::NODE_MODULE_FES);

		if (!pNodeParent->ModifyItem(strTagName, pNode))
		{
			return;
		}

		if (!m_pNodeData->ModifyItemHashMap(strTagName, strLastTagName))
		{
			return;
		}

		item->setData(strTagName, LASTVALUE);

		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::NODE_FES_TYPE);
		if (bFlag)
		{
			m_pUi->GetTabWidget()->SetTabName(strTagName, WIDGET_TYPE::NODE_FES_TYPE, strLastTagName);

			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}

	}
	else if (nType == NODE_ITEM_WORKSTATION_CHILD_GROUP)
	{
		auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		auto strTagName = index.data(Qt::EditRole).toString();
		auto strLastTagName = index.data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		if (!CheckTagNameIsValid(strTagName, NODE_CONFIG_DESC) || !CheckTagNameIsValid(strLastTagName, NODE_CONFIG_DESC))
		{
			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		auto pNodeGroup = reinterpret_cast<Config::CNodeGroup *>(index.data(POINTERDATA).toLongLong());
		if (!pNodeGroup)
		{
			auto strError = tr("node workstation group pointer is nullptr!!!");
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		
		auto pNodeGroupParent = reinterpret_cast<Config::CNodeGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pNodeGroupParent);
		if (!pNodeGroupParent)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		if (m_pNodeData->CheckGroupTagNameIsExist(strTagName))
		{
			return;
		}

		if (!pNodeGroupParent->ModifyGroup(strTagName, pNodeGroup))
		{
			return;
		}

		if (!m_pNodeData->ModifyGroupHashMap(strTagName, strLastTagName))
		{
			return;
		}

		item->setData(strTagName, LASTVALUE);
		item->setData(strTagName, TREE_TOOL_TIP_ROLE);

		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::NODE_WORKSTATION_GROUP_TYPE);
		if (bFlag)
		{
			m_pUi->GetTabWidget()->SetTabName(strTagName, WIDGET_TYPE::NODE_WORKSTATION_GROUP_TYPE, strLastTagName);

			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}

	}
	else if (nType == NODE_ITEM_WORKSTATION_CHILD_ITEM)
	{
		auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		auto strTagName = index.data(Qt::EditRole).toString();
		auto strLastTagName = index.data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		if (!CheckTagNameIsValid(strTagName, NODE_CONFIG_DESC) || !CheckTagNameIsValid(strLastTagName, NODE_CONFIG_DESC))
		{
			return;
		}

		auto pNode = reinterpret_cast<Config::CNode *>(index.data(POINTERDATA).toLongLong());
		if (!pNode)
		{
			auto strError = tr("node workstation pointer is nullptr!!!");
			m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pNodeParent = reinterpret_cast<Config::CNodeGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pNodeParent);
		if (!pNodeParent)
		{
			return;
		}

		Q_ASSERT(m_pNodeData);
		if (!m_pNodeData)
		{
			return;
		}

		if (m_pNodeData->CheckTagNameIsExist(strTagName))
		{
			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		if (!pNodeParent->ModifyItem(strTagName, pNode))
		{
			return;
		}

		if (!m_pNodeData->ModifyItemHashMap(strTagName, strLastTagName))
		{
			return;
		}

		item->setData(strTagName, LASTVALUE);

		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::NODE_WORKSTATION_TYPE
			);
		if (bFlag)
		{
			m_pUi->GetTabWidget()->SetTabName(strTagName, WIDGET_TYPE::NODE_WORKSTATION_TYPE, strLastTagName);

			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}
	}
}