
#include "treeobject.h"

#include <QQueue>
#include <QFile>
#include <QDomDocument>

#include "objectdefine.h"
#include "widgetobject.h"
#include "../include/scadastudio/quoteItem.h"

CObjectTree::CObjectTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore)
{
    Q_ASSERT(parent != nullptr);
    Q_ASSERT(pUi != nullptr);
    Q_ASSERT(pCore != nullptr);

    m_pTreeWidget = parent;
    m_pUi = pUi;
    m_pCore = pCore;
    //m_pTopItem = NULL;
}

void CObjectTree::InitTreeModel()
{
    QObject::connect(GetTree(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ShowMouseRightButton(const QPoint&)));
    QObject::connect(GetTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_Clicked(const QModelIndex &)));
    QObject::connect(GetTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Slot_DoubleClicked(const QModelIndex &)));
    QObject::connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(Slot_TreeItemChanged(QStandardItem *)));
}

void CObjectTree::Slot_ShowMouseRightButton(const QPoint& pos)
{
    QModelIndex index = m_pUi->GetLeftTree()->indexAt(pos);

    int nType = index.data(Qt::UserRole).toInt();
    switch (nType)
    {
        case OBJECT_ROOT_TYPE:
        {
            ShowRootMenu(index);
            break;
        }
        case OBJECT_GROUP_TYPE:
        {
            ShowGroupMenu(index);
            break;
        }
        case OBJECT_ITEM_TYPE:
        {
            ShowItemMenu(index);
            break;
        }
        default:
            break;
    }
}

void CObjectTree::ShowRootMenu(QModelIndex &index)
{
    QMenu* pMenu = new QMenu(this);

    QAction* pAddGroup = new QAction(ADD_OBJECT_GROUP, pMenu);
    pAddGroup->setIcon(QIcon(":/images/add_group.png"));


    QAction* pAddObjItem = new QAction(ADD_OBJECT_ITEM, pMenu);
    pAddObjItem->setIcon(QIcon(":/images/add_item.png"));

    pMenu->addAction(pAddGroup);
    pMenu->addAction(pAddObjItem);

    QAction *pSelectAction = pMenu->exec(QCursor::pos());
    if (pSelectAction == pAddGroup)
    {
        AddGroup(index);
    }
    else  if (pSelectAction == pAddObjItem)
    {
        AddItem(index);
    }


    pMenu->deleteLater();
}

void CObjectTree::ShowGroupMenu(QModelIndex &index)
{
    QMenu* pMenu = new QMenu(this);

    QAction* pAddGroup = new QAction(ADD_OBJECT_GROUP, pMenu);
    pAddGroup->setIcon(QIcon(":/images/add_group.png"));

    QAction* pDeleteObjGroup = new QAction(DELETE_OBJECT_GROUP, pMenu);
    pDeleteObjGroup->setIcon(QIcon(":/images/del_group.png"));

    QAction* pAddObjItem = new QAction(ADD_OBJECT_ITEM, pMenu);
    pAddObjItem->setIcon(QIcon(":/images/add_item.png"));

    QAction* pRenameAction = new QAction(RENAME_OBJECT_TREE_ITEM, pMenu);
    pRenameAction->setIcon(QIcon(":/images/rename.png"));

    pMenu->addAction(pAddGroup);
    pMenu->addAction(pDeleteObjGroup);
    pMenu->addAction(pAddObjItem);
    
    pMenu->addAction(pRenameAction);

    QAction *pSelectAction = pMenu->exec(QCursor::pos());
    if (pSelectAction == pAddGroup)
    {
        AddGroup(index);
    }
    else  if (pSelectAction == pDeleteObjGroup)
    {
        DelGroup(index);
    }
    else  if (pSelectAction == pAddObjItem)
    {
        AddItem(index);
    }
    else  if (pSelectAction == pRenameAction)
    {
        m_pTreeWidget->edit(index);
    }

    pMenu->deleteLater();
}

void CObjectTree::ShowItemMenu(QModelIndex &index)
{
    QMenu* pMenu = new QMenu(this);

    QAction* pDeleteObjItem = new QAction(DELETE_OBJECT_ITEM, pMenu);
    pDeleteObjItem->setIcon(QIcon(":/images/del_tem.png"));

    QAction* pRenameAction = new QAction(RENAME_OBJECT_TREE_ITEM, pMenu);
    pRenameAction->setIcon(QIcon(":/images/rename.png"));

    pMenu->addAction(pDeleteObjItem);
    pMenu->addAction(pRenameAction);

    QAction *pSelectAction = pMenu->exec(QCursor::pos());
    if (pSelectAction == pRenameAction)
    {
        m_pTreeWidget->edit(index);
    }
    else  if (pSelectAction == pDeleteObjItem)
    {
        DelItem(index);
    }

    pMenu->deleteLater();
}

void CObjectTree::AddGroup(QModelIndex &index)
{
    emit Sig_AddGroup(index);
}

void CObjectTree::DelGroup(QModelIndex &index)
{
    emit Sig_DelGroup(index);
}

void CObjectTree::AddItem(QModelIndex &index)
{
    emit Sig_AddItem(index);
}

void CObjectTree::DelItem(QModelIndex &index)
{
    emit Sig_DelItem(index);
}

void CObjectTree::Slot_Clicked(const QModelIndex &index)
{
    QVariant varCurrent = index.data(OBJECT_ROOT_ROLE);
    if (!varCurrent.isValid())
    {
        return;
    }

    int nType = varCurrent.toInt();
    switch (nType)
    {
    case OBJECT_GROUP_TYPE:
    {
        
        break;
    }
    case OBJECT_ITEM_TYPE:
    {
        CObjectItem* pGrp = reinterpret_cast<CObjectItem*>(index.data(OBJECT_ITEM_ROLE).toLongLong());

        if (pGrp == nullptr)
        {
            if (!LoadDataFromFile(index))
            {
                return;
            }
        }

        int nCount = m_pUi->GetTabWidget()->count();

        for (int i = 0; i < nCount; ++i)
        {
            CObjectView* pObjView = dynamic_cast<CObjectView*>(m_pUi->GetTabWidget()->widget(i));
            if (pObjView)
            {
                CObjectItem* pItem = pObjView->GetObjItem();
                if (pItem && (pItem == pGrp))
                {
                    // 设置当前存在索引
                    m_pUi->GetTabWidget()->setCurrentIndex(i);
                    return;
                }
            }
        }        

        break;
    }
    default:
        break;
    }
}

// 从文件中加载对象模型
bool CObjectTree::LoadDataFromFile(const QModelIndex& index)
{
    CObjectItem* pObjItem = reinterpret_cast<CObjectItem*>(index.data(OBJECT_ITEM_ROLE).toLongLong());
    if (nullptr != pObjItem)
    {
        return true;
    }

    // 选中模型才从文件中读取
    QQueue<QString> queuePath;

    // 获取当前选中名字
    QString strCrtName = index.model()->data(index).toString();
    if (strCrtName.isEmpty())
    {
        return false;
    }

    // 获取对象root路径
    QVariant varObjPath = index.data(OBJECT_PATH_ROLE);
    if (!varObjPath.isValid())
    {
        return false;
    }

    QString strObjRootPath = varObjPath.toString();

    QModelIndex& parentIndex = index.parent();
    QVariant varParent = parentIndex.data(OBJECT_ROOT_ROLE);
    if (!varParent.isValid())
    {
        return false;
    }

    int nType = varParent.toInt();

    while (nType != OBJECT_ROOT_TYPE)
    {
        QString strParentName = parentIndex.model()->data(parentIndex).toString();
        if (strParentName.isEmpty())
        {
            return false;
        }

        queuePath.push_back(strParentName);

        parentIndex = parentIndex.parent();
        varParent = parentIndex.data(OBJECT_ROOT_ROLE);
        if (!varParent.isValid())
        {
            return false;
        }

        nType = varParent.toInt();
    }

    int cnt = queuePath.size();    

    QString strChoosePathName;
    if (cnt == 0)
    {
        strChoosePathName = strObjRootPath + strCrtName + OBJECT_FILE_TYPE;
    }
    else
    {        
        for each (auto i in queuePath)
        {
            strObjRootPath += i + "/";
        }
        strChoosePathName = strObjRootPath + strCrtName + OBJECT_FILE_TYPE;
    }

    pObjItem = new CObjectItem();

    if (!ReadFile(strChoosePathName, pObjItem))
    {
        delete pObjItem;
        pObjItem = nullptr;

        return false;
    }

    pObjItem->m_strObjectName = strCrtName;

    CQuoteItem* pQuoteItem = reinterpret_cast<CQuoteItem*>(index.data(QUOTE_ITEM_ROLE).toLongLong());
    if (pQuoteItem == nullptr)
    {
        delete pObjItem;
        pObjItem = nullptr;

        return false;
    }
    pQuoteItem->setData(reinterpret_cast<long long>(pObjItem), OBJECT_ITEM_ROLE);

    Sig_LoadItem(pObjItem, index);

    return true;
}

bool CObjectTree::ReadFile(const QString& strPathName, CObjectItem* pObjItem)
{
    if (strPathName.isEmpty() || pObjItem == nullptr)
    {
        return false;
    }

    QFile file(strPathName);
    if (!file.exists())
    {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument  document;

    // 设置文件，这时会将流设置为初始状态
    if (!document.setContent(&file))
    {
        file.close();
        return false;
    }

    QDomElement root = document.documentElement();
    if (root.tagName() != "object")
    {
        file.close();
        return false;
    }

    QDomNode node = root.firstChild();
    for (; !node.isNull(); node = node.nextSibling())
    {
        if (node.nodeName() == "o")
        {
            QDomElement domElm = node.toElement();
            if (domElm.isNull())
            {
                continue;
            }

            CItemInfo itemInfo;

            //int nID = domElm.attribute("ID").toInt();
            itemInfo.m_strName = domElm.attribute("name");
            itemInfo.m_strDiscribe = domElm.attribute("desc");
            itemInfo.m_nType = domElm.attribute("type").toInt();
            itemInfo.m_bRWAttr = QVariant(domElm.attribute("rw")).toBool();
            itemInfo.m_strRelationPoint = domElm.attribute("targetname");
            
            pObjItem->m_vecTableInfo.push_back(itemInfo);
        }
    }

    file.close();
    return true;
}

void CObjectTree::Slot_DoubleClicked(const QModelIndex &index)
{
    QModelIndex nIndex = index;

    int nType = nIndex.data(OBJECT_ROOT_ROLE).toInt();

    if (nType != OBJECT_ITEM_TYPE)
    {
        GetTree()->expanded(nIndex);
    }
}

void CObjectTree::Slot_TreeItemChanged(QStandardItem* pItem)
{
    if (pItem == nullptr)
    {
        return;
    }

    // todo:判断new name是否存在

    auto strOldValue = pItem->data(OBJECT_OLD_NAME_ROLE).toString();
    auto strNewValue = pItem->data(Qt::EditRole).toString();

    if (strNewValue != strOldValue)
    {
        if (strNewValue.isEmpty())
        {
            pItem->setData(strOldValue, Qt::EditRole);

            return;
        }
    }
    else
    {
        return;
    }

    QModelIndex index = pItem->index();;
    QVariant varCurrent = index.data(OBJECT_ROOT_ROLE);
    if (!varCurrent.isValid())
    {
        return;
    }

    int nType = varCurrent.toInt();
    switch (nType)
    {
        case OBJECT_GROUP_TYPE:
        {
            CObjectGroup* pGrp = reinterpret_cast<CObjectGroup*>(index.data(OBJECT_GROUP_ROLE).toLongLong());
            CQuoteItem* pQuoteItem = reinterpret_cast<CQuoteItem*>(index.data(QUOTE_ITEM_ROLE).toLongLong());
            if (pGrp == nullptr || pQuoteItem == nullptr)
            {
                return;
            }

            auto itemIter = pGrp->m_mapGrps.find(strNewValue);
            if (itemIter != pGrp->m_mapGrps.end())
            {
                pItem->setData(strOldValue, Qt::EditRole);
                return;
            }

            auto value = pGrp->m_mapGrps[strOldValue];

            pGrp->m_mapGrps.erase(itemIter);

            pGrp->m_mapGrps[strNewValue] = value;

            pQuoteItem->setData(strNewValue, OBJECT_OLD_NAME_ROLE);

            break;
        }
        case OBJECT_ITEM_TYPE:
        {        
            CObjectGroup* pGrp = reinterpret_cast<CObjectGroup*>(index.data(OBJECT_GROUP_ROLE).toLongLong());
            CQuoteItem* pQuoteItem = reinterpret_cast<CQuoteItem*>(index.data(QUOTE_ITEM_ROLE).toLongLong());
            if (pGrp == nullptr || pQuoteItem == nullptr)
            {
                return;
            }

            auto itemIter = pGrp->m_mapItemInfo.find(strNewValue);
            if (itemIter != pGrp->m_mapItemInfo.end())
            {
                pItem->setData(strOldValue, Qt::EditRole);
                return;
            }
            
            auto value = pGrp->m_mapItemInfo[strOldValue];
            if (value != nullptr)
            {
                value->m_strObjectName = strNewValue;
            }

            pGrp->m_mapItemInfo.erase(itemIter);

            pGrp->m_mapItemInfo[strNewValue] = value;

            pQuoteItem->setData(strNewValue, OBJECT_OLD_NAME_ROLE);

            break;
        }
    }
}
