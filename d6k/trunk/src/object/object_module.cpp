#if _MSC_VER >= 1900
//#ifdef DEBUG
#include <vld.h>
//#endif // DEBUG
#endif

#include "scadastudio/uimgr.h"
#include "scadastudio/icore.h"
#include "object_module.h"

#include "scadastudio/quoteItem.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"

#define MODULE_LIB
#include "scadastudio/module_export.h"
#include "stl_util-inl.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <QDomDocument> 
#include <QDir> 
#include <QTranslator> 
#include <QDebug>
#include <QQueue>
#include <QMessageBox>

#include "objectdefine.h"
#include "cchoosedialog.h"

static CObjectApi *s_ptrModule = nullptr;

/*! \fn extern "C" SCADA_EXPORT IModule *CreateModule()
********************************************************************************************************* 
** \brief CreateModule 
** \details 子模块入口函数
** \return extern " SCADA_EXPORT IModule * 
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
extern "C" SCADA_EXPORT IModule *CreateModule()
{
	s_ptrModule = new CObjectApi;
	return s_ptrModule;
//	return  s_ptrModule.get();
}

/*! \fn extern "C" SCADA_EXPORT void DestroyModule()
********************************************************************************************************* 
** \brief DestroyModule 
** \details 回收资源
** \return extern " SCADA_EXPORT void 
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
extern "C" SCADA_EXPORT void DestroyModule()
{
	if (s_ptrModule)
	{
		delete s_ptrModule;
		s_ptrModule = nullptr;
	}
}

CObjectApi *GetModuleApi()
{
	return s_ptrModule;
//	return  s_ptrModule.get();
}

IMainWindow *GetMainWindow()
{
//	return s_ptrModule->GetCoreUI();
	return s_ptrModule->GetMainWindow();
}


int CObjectApi::m_nTreeDepth = 0;

CObjectApi::CObjectApi()
{
	m_pTran = std::make_shared<QTranslator>();

	QString strRunPath = qApp->applicationDirPath();

	if (m_pTran->load(strRunPath + "/locales/object.qm"))
	{
		qApp->installTranslator(m_pTran.get());
	}
	else
	{
		qDebug() << QStringLiteral("加载object语言文件失败！");
	}

	m_pMainModuleInterface = nullptr;
    m_pUi = nullptr;
    m_pObjectTree = nullptr;
    m_pModelLib = nullptr;
}

CObjectApi::~CObjectApi()
{
    if (nullptr != m_pObjectTree)
    {
        delete m_pObjectTree;
        m_pObjectTree = nullptr;
    }

    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }
}

int CObjectApi::CallBackBeforeCloseTab(void *pData)
{
	qDebug() << "CallBackBeforeCloseTab";
	return 1;
}

void CObjectApi::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (pCore == nullptr)
		return;

	// these are defaults:
// 	qApp->setOrganizationName("scadastudio");
// 	qApp->setOrganizationDomain("scadastudio");
// 	qApp->setApplicationName("scadastudio");
//	qApp->setApplicationVersion(RSettings::getVersionString());
	m_pMainModuleInterface = pCore;

	m_pUi = pCore->GetUIMgr();

    // 加载模型配置
    LoadModelConfig();

    // 获取左侧树形控件
    CTreeWidget* pTree = m_pUi->GetLeftTree();
    Q_ASSERT (pTree);

    m_pObjectTree = new CObjectTree(pTree, m_pUi, m_pMainModuleInterface);

    m_pObjectTree->InitTreeModel();

    // 获取tabwidget
	CTabWidget *pTabWidget = m_pUi->GetTabWidget();
    Q_ASSERT (pTabWidget);

    m_pObjectWidget = new CObjectWidget(pTabWidget, m_pMainModuleInterface);

	m_pUi->GetTabWidget()->m_fnBeforeCloseTab = std::bind(&CObjectApi::CallBackBeforeCloseTab,this,std::placeholders::_1);

    CreateConnections();
}


bool CObjectApi::LoadModelConfig()
{
    m_pModelLib = new CModelLib();

    // 加载modeltool.xml
    m_pModelLib->LoadTreeDataFromFile("");

    // 读取模型原始数据
    m_pModelLib->LoadViewDataFromFile("");

    return true;
}

void CObjectApi::UnInit()
{

}
/*! \fn void  CGraphApi::InitGraphRootTreeItem()
********************************************************************************************************* 
** \brief CGraphApi::InitGraphRootTreeItem 
** \details 创建树叶（图形项），注册右键菜单
** \return void 
** \author LiJin
** \date 2016年8月18日 
** \note 
********************************************************************************************************/

void  CObjectApi::InitObjectRootTreeItem()
{
	Q_ASSERT(m_pUi);
	if (m_pUi == nullptr)
		return;

	CQuoteItem *pItem = m_pUi->GetTopItem();


	QString strName = QStringLiteral("对象组态");
	CQuoteItem *pNewItem = new CQuoteItem(strName);
	pNewItem->setIcon(QIcon(":/images/equipment.png"));
    pNewItem->setData(OBJECT_ROOT_TYPE, OBJECT_ROOT_ROLE);


	pItem->appendRow(pNewItem);


}
/*! \fn bool CObjectApi::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CObjectApi::SaveProject 
** \details 保存图形工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author LiJin 
** \date 2016年8月27日 
** \note 
********************************************************************************************************/
bool CObjectApi::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_ASSERT(pXml);
    if (pXml == nullptr)
    {
        return false;
    }
		
	Q_ASSERT(szRoot.length() > 1);
    if (szRoot.length() <= 1)
    {
        return false;
    }

    // 保存工程信息
    QDomElement nRoot = pXml->documentElement();
    for (QDomNode dNode = nRoot.firstChild(); !dNode.isNull(); dNode = dNode.nextSibling())
    {
        QDomElement dElement = dNode.toElement();
        QString strRootName = dElement.tagName();
        if (strRootName.compare(OBJECT_NODE_NAME, Qt::CaseInsensitive) == 0)
        {
            // 删除之前节点
            while (dElement.hasChildNodes())
            {
                QDomNodeList nList = dElement.childNodes();

                for (int i = 0; i < nList.count(); ++i)
                {
                    QDomNode& tmpDomNode = nList.at(i);
                    dElement.removeChild(tmpDomNode);
                }
            }

            SaveObjectNode(pXml, &dElement);
        }
    }

    // 保存对象信息
    SaveObjectInfo(pXml, pRoot, szRoot);

	return true;  
}

// 保存object节点
bool CObjectApi::SaveObjectNode(QDomDocument* pDom, QDomElement* pElement)
{
    if (pDom == nullptr || pElement == nullptr)
    {
        return false;
    }

    // 写入组信息
    if (m_objMgr.m_pMgrGrps == nullptr)
    {
        return false;
    }

    CObjectGroup* pObjGrp = m_objMgr.m_pMgrGrps->m_mapGrps[OBJECT_ROOT_NAME];
    if (pObjGrp == nullptr)
    {
        return false;
    }

    if (!SaveChildNode(pDom, pElement, pObjGrp))
    {
        return false;
    }

    // 写入item信息
    for each (auto i in pObjGrp->m_mapItemInfo.keys())
    {
        QDomElement domChildItemElement = pDom->createElement("item");

        QDomAttr domNameAttr = pDom->createAttribute("name");

        domChildItemElement.setAttributeNode(domNameAttr);

        domChildItemElement.setAttribute("name", i);

        pElement->appendChild(domChildItemElement);
    }

    return true;
}

bool CObjectApi::SaveChildNode(QDomDocument* pDom, QDomElement* pElement, CObjectGroup* pObjGrp)
{
    if (pDom == nullptr || pElement == nullptr || pObjGrp == nullptr)
    {
        return false;
    }

    for each (auto i in pObjGrp->m_mapGrps.keys())
    {
        QDomElement domChildElement = pDom->createElement("group");

        QDomAttr domNameAttr = pDom->createAttribute("name");

        domChildElement.setAttributeNode(domNameAttr);

        domChildElement.setAttribute("name", i);

        pElement->appendChild(domChildElement);

        if (pObjGrp->m_mapGrps[i] != nullptr)
        {
            for each (auto j in (pObjGrp->m_mapGrps[i]->m_mapItemInfo.keys()))
            {
                QDomElement domChildItemElement = pDom->createElement("item");

                QDomAttr domNameAttr = pDom->createAttribute("name");

                domChildItemElement.setAttributeNode(domNameAttr);

                domChildItemElement.setAttribute("name", j);

                domChildElement.appendChild(domChildItemElement);
            }

            for each (auto j in pObjGrp->m_mapGrps[i]->m_mapGrps.keys())
            {
                SaveChildNode(pDom, &domChildElement, pObjGrp->m_mapGrps[i]);
            }
        }        

        
    }

    return true;
}


// 保存object信息
bool CObjectApi::SaveObjectInfo(QDomDocument* pDom, QDomElement* pElement, QString szRoot)
{
    if (pDom == nullptr || pElement == nullptr || szRoot.isEmpty())
    {
        return false;
    }

    QString strObjPath = szRoot + "/" + OBJECT_SAVE_FOLDER_NAME + "/";

    QDir dir;
    if (!dir.exists(szRoot))
    {
        return false;
    }

    if (!dir.exists(strObjPath))
    {
        bool bRes = dir.mkpath(strObjPath);
        if (!bRes)
        {
            return false;
        }
    }

    if (m_objMgr.m_pMgrGrps == nullptr)
    {
        return false;
    }

    for each (auto i in m_objMgr.m_pMgrGrps->m_mapGrps.keys())
    {
        SaveGroupInfo(pDom, pElement, strObjPath, m_objMgr.m_pMgrGrps->m_mapGrps[i]);
    }

    return true;
}

bool CObjectApi::SaveGroupInfo(QDomDocument* pDom, QDomElement* pElement, const QString strPath, CObjectGroup* pObjGrp)
{
    if (pDom == nullptr || pElement == nullptr || strPath.isEmpty() || pObjGrp == nullptr)
    {
        return false;
    }

    for each (auto i in pObjGrp->m_mapGrps.keys())
    {
        if (pObjGrp->m_mapGrps[i] != nullptr)
        {
            const QString path = strPath + i + "/";
            QDir dir;
            // 模型目录创建
            if (!dir.exists(path))
            {
                bool bRes = dir.mkpath(path);
                if (!bRes)
                {
                    return false;
                }
            }

            SaveGroupInfo(pDom, pElement, path, pObjGrp->m_mapGrps[i]);
        }
    }

    for each (auto i in pObjGrp->m_mapItemInfo.keys())
    {
        if (pObjGrp->m_mapItemInfo[i] != nullptr)
        {
            const QString strPathName = strPath + i + OBJECT_FILE_TYPE;
            QFile file(strPathName);

            // todo:新增修改标志
            // 模型目录创建
            if (!file.exists())
            {
                bool bRes = file.open(QIODevice::WriteOnly);
                if (!bRes)
                {
                    return false;
                }

                QXmlStreamWriter writer(&file);
                writer.setAutoFormatting(true);
                writer.writeStartDocument();

                writer.writeStartElement("object");

                // 写入item信息
                SaveItemInfo(writer, strPath, pObjGrp->m_mapItemInfo[i]);

                writer.writeEndElement();

                writer.writeEndDocument();

                file.close();
            }
        }
    }

    return true;
}

// 保存item信息
bool CObjectApi::SaveItemInfo(QXmlStreamWriter& writer, const QString strPath, CObjectItem* pObjItem)
{
    if (strPath.isEmpty() || pObjItem == nullptr)
    {
        return false;
    }

    int nIndex = 0;
    for each (auto i in pObjItem->m_vecTableInfo)
    {
        writer.writeStartElement("m");

        writer.writeAttribute("ID", QString("%1").arg(nIndex));

        writer.writeAttribute("name", i.m_strName);

        writer.writeAttribute("desc", i.m_strDiscribe);

        writer.writeAttribute("type", QString("%1").arg(i.m_nType));

        writer.writeAttribute("rw", QString("%1").arg(i.m_bRWAttr));

        writer.writeAttribute("targetname", QString("%1").arg(i.m_strRelationPoint));

        writer.writeEndElement();

        nIndex++;
    }

    return true;
}

bool CObjectApi::LoadProject(QDomDocument* pXml, QDomElement* pRoot, const QString& szRoot)
{
	Q_ASSERT(pXml);
    if (pXml == nullptr || szRoot.isEmpty())
    {
        return false;
    }

    SetObjectPath(szRoot);

    QDir dir;
    // 目录不存在创建对象根目录
    if (!dir.exists(m_strObjectPath))
    {
        bool bRes = dir.mkdir(m_strObjectPath);
        if (!bRes)
        {
            return false;
        }
    }

    QDomElement root = pXml->documentElement();

    if (root.tagName() != PROJECT_ROOT_NODE_NAME)
    {
        return false;
    }

    QDomNode nChild = pRoot->firstChild();
    //QString name = nChild.toElement().tagName();
    for (; !nChild.isNull(); nChild = nChild.nextSibling())
    {
        QDomElement domElm = nChild.toElement();
        if (domElm.nodeName() == OBJECT_NODE_NAME)
        {
            // 新建对象root节点
            CQuoteItem* pObjTop = m_pUi->GetTopItem();
            if (pObjTop == nullptr)
            {
                return false;
            }

            CQuoteItem* pTopItem = new CQuoteItem(OBJECT_ROOT_NAME);
            pTopItem->setData(OBJECT_ROOT_TYPE, OBJECT_ROOT_ROLE);
            pTopItem->setIcon(QIcon(":/images/equipment.png"));

            pObjTop->appendRow(pTopItem);

            m_objMgr.m_pMgrGrps = new CObjectGroup();

            //CObjectGroup* pObjGrp = m_objMgr.m_pMgrGrps->CreateGroup(OBJECT_ROOT_NAME);
            // 加载组
            LoadObjectNode(domElm, pTopItem, m_objMgr.m_pMgrGrps);

            // 加载item
            //CObjectItem* pObjItem = m_objMgr.m_pMgrGrps->m_mapItemInfo[OBJECT_ROOT_NAME];

            break;
        }
    }

	return true;
}

bool CObjectApi::LoadObjectNode(QDomElement& domElement, CQuoteItem* pItem, CObjectGroup* pObjGrp)
{
    if (domElement.isNull() || pItem == nullptr || pObjGrp == nullptr)
    {
        return false;
    }
    CObjectGroup* pNewObjGrp = pObjGrp->CreateGroup(OBJECT_ROOT_NAME);
    pObjGrp->m_mapGrps[OBJECT_ROOT_NAME] = pNewObjGrp;

    /*CObjectItem* pNewObjItem = pObjGrp->CreateItem(OBJECT_ROOT_NAME);
    pObjGrp->m_mapItemInfo[OBJECT_ROOT_NAME] = pNewObjItem;*/

    QDomNode& nNode = domElement.firstChild();
    QDomElement elmData;
    QString tagName;

    for (; !nNode.isNull(); nNode = nNode.nextSibling())
    {
        elmData = nNode.toElement();
        tagName = elmData.tagName();
        if (tagName == OBJECT_GROUP_NODE_NAME)
        {
            // 树的深度初始为0
            m_nTreeDepth = 0;
            LoadGroupNode(elmData, pItem, pNewObjGrp);
        }
        else if (tagName == OBJECT_ITEM_NODE_NAME)
        {
            //LoadItemNode(elmData, pItem, pNewObjItem);

            //CObjectItem* pNewObjItem = pNewObjGrp->CreateItem(OBJECT_ROOT_NAME);
            

            QString strItemName = elmData.attribute("name");
            
            //pNewObjGrp->m_mapItemInfo[strItemName] = pNewObjItem;
            pNewObjGrp->m_mapItemInfo[strItemName] = nullptr;

            CQuoteItem* pNewItem = new CQuoteItem(strItemName);
            
            pNewItem->setData(OBJECT_ITEM_TYPE, OBJECT_ROOT_ROLE);
            pNewItem->setData(strItemName, OBJECT_OLD_NAME_ROLE);
            pNewItem->setData(reinterpret_cast<long long >(pObjGrp), OBJECT_GROUP_ROLE);
            pNewItem->setData(reinterpret_cast<long long >(pNewObjGrp->m_mapItemInfo[strItemName]), OBJECT_ITEM_ROLE);
            pNewItem->setData(reinterpret_cast<long long >(pNewItem), QUOTE_ITEM_ROLE);
            pNewItem->setData(m_strObjectPath, OBJECT_PATH_ROLE);
            pNewItem->setIcon(QIcon(":/images/m_model.png"));

            pItem->appendRow(pNewItem);

            
        }
    }

    /*for each (auto i in pObjGrp->m_mapGrps.keys())
    {

    CQuoteItem* m_pNewItem = new CQuoteItem(OBJECT_ROOT_NAME);
    m_pNewItem->setData(OBJECT_ROOT_TYPE, Qt::UserRole);
    m_pNewItem->setIcon(QIcon(":/images/add_group.png"));

    pItem->appendRow(m_pNewItem);

    for each (auto j in pObjGrp->m_mapGrps[i]->m_mapGrps.keys())
    {
    LoadGroup(pXml, pRoot, m_pNewItem, pObjGrp->m_mapGrps[i]);
    }
    }*/

    return true;
}

bool CObjectApi::LoadGroupNode(QDomElement& domElement, CQuoteItem* pItem, CObjectGroup* pObjGrp)
{
    if (domElement.isNull() || pItem == nullptr || pObjGrp == nullptr)
    {
        return false;
    }

    
    QString strGrpName = domElement.attribute("name");

    CObjectGroup* pNewObjGrp = pObjGrp->CreateGroup(OBJECT_ROOT_NAME);
    pObjGrp->m_mapGrps[strGrpName] = pNewObjGrp;
    
    CQuoteItem* pNewItem = new CQuoteItem(strGrpName);
    // 树的深度
    m_nTreeDepth++;
    pNewItem->setData(m_nTreeDepth, Qt::WhatsThisRole);
    pNewItem->setData(reinterpret_cast<long long >(pObjGrp), OBJECT_GROUP_ROLE);
    pNewItem->setData(strGrpName, OBJECT_OLD_NAME_ROLE);
    pNewItem->setData(OBJECT_GROUP_TYPE, OBJECT_ROOT_ROLE);
    pNewItem->setData(reinterpret_cast<long long >(pNewItem), QUOTE_ITEM_ROLE);
    pNewItem->setData(m_strObjectPath, OBJECT_PATH_ROLE);
    pNewItem->setIcon(QIcon(":/images/m_group.png"));

    pItem->appendRow(pNewItem);

    
    QDomNode pNode;
    for (pNode = domElement.firstChild(); !pNode.isNull(); pNode = pNode.nextSibling())
    {
        QDomElement childDomElm = pNode.toElement();
        QString& name = childDomElm.tagName();
        if (name.isEmpty())
        {
            return false;
        }

        if (name == OBJECT_GROUP_NODE_NAME)
        {
            LoadGroupNode(childDomElm, pNewItem, pNewObjGrp);
        }
        else if (name == OBJECT_ITEM_NODE_NAME)
        {
            LoadItemNode(childDomElm, pNewItem, pNewObjGrp);
        }
    }

    return true;
}

bool CObjectApi::LoadItemNode(QDomElement& domElement, CQuoteItem* pItem, CObjectGroup* pObjGrp)
{
    if (domElement.isNull() || pItem == nullptr || pObjGrp == nullptr)
    {
        return false;
    }

    //CObjectItem* pNewObjItem = pObjGrp->CreateItem(OBJECT_ROOT_NAME);

    QString strItemName = domElement.attribute("name");
    CQuoteItem* pNewItem = new CQuoteItem(strItemName);
    pNewItem->setData(OBJECT_ITEM_TYPE, OBJECT_ROOT_ROLE);
    pNewItem->setData(strItemName, OBJECT_OLD_NAME_ROLE);
    pNewItem->setData(reinterpret_cast<long long >(pObjGrp), OBJECT_GROUP_ROLE);
    pNewItem->setData(reinterpret_cast<long long >(pObjGrp->m_mapItemInfo[strItemName]), OBJECT_ITEM_ROLE);
    pNewItem->setData(reinterpret_cast<long long >(pNewItem), QUOTE_ITEM_ROLE);
    pNewItem->setData(m_strObjectPath, OBJECT_PATH_ROLE);
    pNewItem->setIcon(QIcon(":/images/m_model.png"));

    pItem->appendRow(pNewItem);

    //pObjGrp->m_mapItemInfo[strItemName] = pNewObjItem;
    pObjGrp->m_mapItemInfo[strItemName] = nullptr;


    return true;
}

void CObjectApi::SetObjectPath(const QString& path)
{
    m_strObjectPath = path + '/' + OBJECT_SAVE_FOLDER_NAME + "/";
}

// 关闭工程文件
void CObjectApi::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	// 先关闭 自己模块所属的所有界面

	// 回收内存

//	m_pGraphData.reset();
}
// 新建一个空的工程文件
void CObjectApi::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	Q_ASSERT(pXml && pRoot);
	if (pXml == nullptr || pRoot == nullptr)
		return;
	// 初始化
 	QDomElement root = pXml->createElement("object");
 	pRoot->appendChild(root);
  

	InitObjectRootTreeItem();
	// 创建目录 
	QDir dir;
	bool bRet = dir.mkdir(szRoot + "/" + "object");
	if (!bRet)
	{
		QString strError = QString(tr("%1/object folder mkdir failed!!!")).arg(szRoot);
		LogMsg(strError.toStdString().c_str(), 0);
	}
	 
}
/*! \fn bool  CObjectApi::LogMsg(const char *szLogTxt, int nLevel)
********************************************************************************************************* 
** \brief CObjectApi::LogMsg 
** \details log日志
** \param szLogTxt log的内容
** \param nLevel   log等级
** \return bool    是否成功标志
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
bool  CObjectApi::LogMsg(const char *szLogTxt, int nLevel)
{
	Q_ASSERT(szLogTxt && strlen(szLogTxt) > 0);
	if ( szLogTxt == nullptr || strlen(szLogTxt) == 0 ) 
		return false;

	Q_ASSERT(m_pMainModuleInterface);
	if (m_pMainModuleInterface == nullptr)
		return false;

	return m_pMainModuleInterface->LogMsg(tr("object").toStdString().c_str(), szLogTxt, nLevel);
}

void CObjectApi::CreateConnections()
{
    connect(m_pObjectTree, SIGNAL(Sig_AddGroup(QModelIndex&)), this, SLOT(Slot_OnAddGroup(QModelIndex&)));
    connect(m_pObjectTree, SIGNAL(Sig_AddItem(QModelIndex&)), this, SLOT(Slot_OnAddItem(QModelIndex&)));

    connect(m_pObjectTree, SIGNAL(Sig_DelGroup(QModelIndex&)), this, SLOT(Slot_OnDelGroup(QModelIndex&)));
    connect(m_pObjectTree, SIGNAL(Sig_DelItem(QModelIndex&)), this, SLOT(Slot_OnDelItem(QModelIndex&)));

    connect(m_pObjectTree, SIGNAL(Sig_LoadItem(const CObjectItem*, const QModelIndex&)), this, SLOT(Slot_LoadItem(const CObjectItem*, const QModelIndex&)));
}

void CObjectApi::Slot_OnAddGroup(QModelIndex& index)
{
    // 获取树节点, 即获取当前主文件夹
    CQuoteItem* pGroupItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

    int nCnt = pGroupItem->data(Qt::WhatsThisRole).toInt();
    if (nCnt >= MAX_OBJECT_GROUP_COUNT)
    {
        return;
    }

    QString nParentName = pGroupItem->text();

    CObjectGroup* pSelectGrp = GetGroupInfo(index);
    if (pSelectGrp == nullptr)
    {
        return;
    }

    QString& strNewGrpName = pSelectGrp->GetGroupName(CREATE_OBJECT_GROUP_NAME);

    auto pObjGroup = new CObjectGroup();

    if (!pSelectGrp->AddObjectGroup(strNewGrpName, pObjGroup))
    {
        return;
    }

    CQuoteItem* newItem = new CQuoteItem(strNewGrpName);

    newItem->setData(pGroupItem->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

    newItem->setData(reinterpret_cast<long long >(pSelectGrp), OBJECT_GROUP_ROLE);
    newItem->setData(reinterpret_cast<long long >(newItem), QUOTE_ITEM_ROLE);
    newItem->setIcon(QIcon(":/images/m_group.png"));

    newItem->setData(OBJECT_GROUP_TYPE, OBJECT_ROOT_ROLE);

    newItem->setData(strNewGrpName, OBJECT_OLD_NAME_ROLE);

    pGroupItem->appendRow(newItem);

    m_pObjectTree->GetTree()->expand(index);    
}

void CObjectApi::Slot_OnAddItem(QModelIndex& index)
{
    Q_ASSERT(m_pModelLib);

    CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
    int nCnt = item->data(Qt::WhatsThisRole).toInt();
    if (nCnt > MAX_OBJECT_GROUP_COUNT)
    {
        return;
    }

    CChooseDialog chooseDlg(m_pModelLib->m_mapScene, m_pModelLib->m_arrModels);

    int nExecRes = chooseDlg.exec();
    if (nExecRes == QDialog::Rejected)
    {
        return;
    }

    CModelInfo* pModelInfo = chooseDlg.GetModelInfo();
    if (pModelInfo == nullptr)
    {
        return;
    }

    auto pObjItem = new CObjectItem();
    CObjectGroup* pSelectGrp = GetGroupInfo(index);
    if (pSelectGrp == nullptr)
    {
        return;
    }    

    // 加载数据
    LoadData(pObjItem, *pModelInfo);

    const QString& strCrtModelName = chooseDlg.GetModelName();

    const QString& strNewItemName = pSelectGrp->GetItemName(strCrtModelName);
    if (strNewItemName.isEmpty())
    {
        return;
    }

    pObjItem->m_strObjectName = strNewItemName;

    if (!pSelectGrp->AddObjectItem(strNewItemName, pObjItem))
    {
        return;
    }

    //树控件添加
    CQuoteItem* newItem = new CQuoteItem(strNewItemName);

    newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

    newItem->setData(reinterpret_cast<long long >(pSelectGrp), OBJECT_GROUP_ROLE);
    newItem->setData(reinterpret_cast<long long >(pObjItem), OBJECT_ITEM_ROLE);

    newItem->setData(reinterpret_cast<long long >(newItem), QUOTE_ITEM_ROLE);

    newItem->setData(OBJECT_ITEM_TYPE, OBJECT_ROOT_ROLE);

    newItem->setData(m_strObjectPath, OBJECT_PATH_ROLE);

    newItem->setData(strNewItemName, OBJECT_OLD_NAME_ROLE);

    newItem->setIcon(QIcon(":/images/m_model.png"));
        
    item->appendRow(newItem);

    m_pObjectTree->GetTree()->expand(index);

    m_pObjectWidget->AddToTableCtrl(pObjItem);
}

void CObjectApi::Slot_OnDelGroup(QModelIndex& index)
{
    QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("确定删除？？？"));
    warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    warning.setButtonText(QMessageBox::Ok, QStringLiteral("确 定"));
    warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取 消"));

    if (warning.exec() == QMessageBox::Cancel)
    {
        return;
    }

    CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
    if (item == nullptr)
    {
        return;
    }

    QString strName = index.data().toString();
    CObjectGroup* pSelectGrp = GetGroupInfo(index.parent());
    if (pSelectGrp == nullptr)
    {
        return;
    }

    int nType = item->data(OBJECT_ROOT_ROLE).toInt();
    //组类型判断
    if (nType == OBJECT_GROUP_TYPE)
    {
        if (item->hasChildren())//若存成子成员，则需要等子成员删除后再删组级
        {
            QMessageBox::information(0, QString(QStringLiteral("删除异常")), QString(QStringLiteral("请先逐个删除子项!!!")));
            return;
        }
        else
        {
            CQuoteItem *pItem = dynamic_cast<CQuoteItem*>(item->parent());
            if (pItem)
            {
                pItem->removeRow(item->row());

                auto iter = pSelectGrp->m_mapGrps.find(strName);
                if (iter != pSelectGrp->m_mapGrps.end())
                {
                    delete pSelectGrp->m_mapGrps[strName];
                    pSelectGrp->m_mapGrps[strName] = nullptr;

                    pSelectGrp->m_mapGrps.erase(iter);
                }
                
            }
        }
    }
}

void CObjectApi::Slot_OnDelItem(QModelIndex& index)
{
    QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("确定删除？？？"));
    warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    warning.setButtonText(QMessageBox::Ok, QStringLiteral("确 定"));
    warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取 消"));

    if (warning.exec() == QMessageBox::Cancel)
    {
        return;
    }

    CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
    if (item == nullptr)
    {
        return;
    }

    QString strName = index.data().toString();
    CObjectGroup* pSelectGrp = GetGroupInfo(index.parent());
    if (pSelectGrp == nullptr)
    {
        return;
    }

    int nType = item->data(OBJECT_ROOT_ROLE).toInt();
    //组类型判断
    if (nType == OBJECT_ITEM_TYPE)
    {
        CQuoteItem *pItem = dynamic_cast<CQuoteItem*>(item->parent());
        if (pItem)
        {
            pItem->removeRow(item->row());

            auto iter = pSelectGrp->m_mapItemInfo.find(strName);
            if (iter != pSelectGrp->m_mapItemInfo.end())
            {
                delete pSelectGrp->m_mapItemInfo[strName];
                pSelectGrp->m_mapItemInfo[strName] = nullptr;

                pSelectGrp->m_mapItemInfo.erase(iter);
            }

        }

    }
}

void CObjectApi::Slot_LoadItem(const CObjectItem* pObjectItem, const QModelIndex& index)
{
    if (nullptr == pObjectItem)
    {
        return;
    }

    CObjectGroup* pSelectGrp = GetGroupInfo(index.parent());
    if (pSelectGrp == nullptr)
    {
        return;
    }

    if (!pSelectGrp->AddObjectItem(pObjectItem->m_strObjectName, pObjectItem))
    {
        return;
    }

    m_pObjectWidget->AddToTableCtrl(const_cast<CObjectItem*>(pObjectItem));

}

QStandardItem* CObjectApi::GetObjectRootNode() const
{
    QList<QStandardItem*> roots;
    QStandardItemModel* root = m_pUi->GetLeftTreeModel();
    for (int i = 0; i < root->rowCount(); ++i)
    {
        auto* itemModel = root->item(i);
        if (itemModel->data(OBJECT_ROOT_ROLE) == OBJECT_ROOT_TYPE)
        {
            return itemModel;
        }
    }

    return nullptr;
}

CObjectGroup* CObjectApi::GetGroupInfo(const QModelIndex &index)
{
    QQueue<QString> queueKey;

    QString strCrtName = index.model()->data(index).toString();
    if (strCrtName.isEmpty())
    {
        return nullptr;
    }

    QVariant varCurrent = index.data(OBJECT_ROOT_ROLE);
    if (!varCurrent.isValid())
    {
        return nullptr;
    }

    if (varCurrent.toInt() == OBJECT_ROOT_TYPE)
    {
        if (m_objMgr.m_pMgrGrps == nullptr)
        {
            m_objMgr.m_pMgrGrps = new CObjectGroup();
        }
        auto iter = m_objMgr.m_pMgrGrps->m_mapGrps.find(strCrtName);
        if (iter == m_objMgr.m_pMgrGrps->m_mapGrps.end())
        {
            auto pObjGroup = new CObjectGroup();
            m_objMgr.m_pMgrGrps->m_mapGrps[strCrtName] = pObjGroup;
        }
        
        return m_objMgr.m_pMgrGrps->m_mapGrps[strCrtName];
    }

    queueKey.push_back(strCrtName);

    QModelIndex& parent = index.parent();
    QVariant varParent = parent.data(OBJECT_ROOT_ROLE);
    if (!varParent.isValid())
    {
        return nullptr;
    }

    int nType = varParent.toInt();

    while (nType != OBJECT_ROOT_TYPE)
    {
        QString strParentName = parent.model()->data(parent).toString();
        
        if (strParentName.isEmpty())
        {
            return nullptr;
        }

        queueKey.push_back(strParentName);

        parent = parent.parent();
        QVariant varParent = parent.data(OBJECT_ROOT_ROLE);
        if (!varParent.isValid())
        {
            return nullptr;
        }

        nType = varParent.toInt();
    }

    QString strParentName = parent.model()->data(parent).toString();
    queueKey.push_back(strParentName);

    QString& key = queueKey.back();
    queueKey.pop_back();
    CObjectGroup* pObjGroup = m_objMgr.m_pMgrGrps->m_mapGrps[key];
    if (pObjGroup == nullptr)
    {
        return nullptr;
    }

    while (!queueKey.isEmpty())
    {
        key = queueKey.back();
        queueKey.pop_back();

        pObjGroup = pObjGroup->m_mapGrps[key];
    }

    return pObjGroup;
}

CObjectGroup* CObjectApi::GetGroupInfoFromItem(const QModelIndex &index)
{
    QQueue<QString> queueKey;

    QString strCrtName = index.model()->data(index).toString();
    if (strCrtName.isEmpty())
    {
        return nullptr;
    }

    QVariant varCurrent = index.data(OBJECT_ROOT_ROLE);
    if (!varCurrent.isValid())
    {
        return nullptr;
    }

    if (varCurrent.toInt() == OBJECT_ROOT_TYPE)
    {
        if (m_objMgr.m_pMgrGrps == nullptr)
        {
            m_objMgr.m_pMgrGrps = new CObjectGroup();
        }
        auto iter = m_objMgr.m_pMgrGrps->m_mapGrps.find(strCrtName);
        if (iter == m_objMgr.m_pMgrGrps->m_mapGrps.end())
        {
            auto pObjGroup = new CObjectGroup();
            m_objMgr.m_pMgrGrps->m_mapGrps[strCrtName] = pObjGroup;
        }

        return m_objMgr.m_pMgrGrps->m_mapGrps[strCrtName];
    }

    queueKey.push_back(strCrtName);

    QModelIndex& parent = index.parent();
    QVariant varParent = parent.data(OBJECT_ROOT_ROLE);
    if (!varParent.isValid())
    {
        return nullptr;
    }

    int nType = varParent.toInt();

    while (nType != OBJECT_ROOT_TYPE)
    {
        QString strParentName = parent.model()->data(parent).toString();

        if (strParentName.isEmpty())
        {
            return nullptr;
        }

        queueKey.push_back(strParentName);

        parent = parent.parent();
        QVariant varParent = parent.data(OBJECT_ROOT_ROLE);
        if (!varParent.isValid())
        {
            return nullptr;
        }

        nType = varParent.toInt();
    }

    QString strParentName = parent.model()->data(parent).toString();
    queueKey.push_back(strParentName);

    QString& key = queueKey.back();
    queueKey.pop_back();
    CObjectGroup* pObjGroup = m_objMgr.m_pMgrGrps->m_mapGrps[key];
    if (pObjGroup == nullptr)
    {
        return nullptr;
    }

    while (!queueKey.isEmpty())
    {
        key = queueKey.back();
        queueKey.pop_back();

        pObjGroup = pObjGroup->m_mapGrps[key];
    }

    return pObjGroup;
}

