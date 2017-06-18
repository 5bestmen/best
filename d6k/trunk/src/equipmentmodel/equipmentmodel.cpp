
#include "equipmentmodel.h"
#include "modelinfo.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QStyleFactory>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QMessageBox>
#include <QToolBar>
#include <QMenuBar>


#include "modeldefine.h"

extern bool g_bModifyFlag;

equipmentmodel::equipmentmodel()
{
    //ui.setupUi(this);

    setWindowIcon(QIcon(QStringLiteral(":/images/model_tool.png")));
    setWindowTitle(QStringLiteral("模型工具"));
    setWindowModality(Qt::ApplicationModal);

    Init();

    m_pModelLib = new CModelLib();

    // 读取tree原始数据
    /*if (!m_pModelLib->LoadTreeDataFromFile(""))
    {
        return;
    }*/
    m_pModelLib->LoadTreeDataFromFile("");

    // 读取view原始数据
    m_pModelLib->LoadViewDataFromFile("");

    // 创建工具条
    CreateToolBar();

    // 创建主分割窗口 设置方向为水平分割窗口  
    m_pMainSplitter = new QSplitter(Qt::Horizontal, this);

    // 左侧Splitter
    m_pLeftSplitter = new QSplitter(Qt::Vertical, m_pMainSplitter);

    // 右侧Splitter
    m_pRightSplitter = new QSplitter(Qt::Vertical, m_pMainSplitter);

    CreateLeftSplitter();

    CreateRightSplitter();

    m_pMainSplitter->setStretchFactor(1, 1);
    m_pMainSplitter->setWindowTitle(QObject::tr("分割窗口"));

    setCentralWidget(m_pMainSplitter);

    resize(800, 600);
}

equipmentmodel::~equipmentmodel()
{
    Fini();
}

void equipmentmodel::Init()
{
    m_pModelLib = nullptr;
    m_nCurrentMode = -1;
    m_pCurrentEquipModel = nullptr;
    m_pComboxAnalogDelegate = nullptr;

    m_nCurrentModifyType = -1;
    m_nCurrentDeleteType = -1;
    m_bNeedSaveFlag = false;

    m_pSaveAct = nullptr;
    m_pAddAct = nullptr;
    m_pDelAct = nullptr;

    // layout
    m_pLeftSplitter = nullptr;
    m_pRightSplitter = nullptr;
    m_pMainSplitter = nullptr;

    // left
    m_pTreeView = nullptr;
    m_pFileBtn = nullptr;
    //m_pTreeSaveBtn = nullptr;
    m_pTreeModel = nullptr;
    m_pItemRoot = nullptr;

    // right
    m_pContentModel = nullptr;
    m_pEquipView = nullptr;

    // menu
    m_pMenuScene = nullptr;
    m_pMenuEquipment = nullptr;
    m_pMenuModelNo = nullptr;
    m_pMenuProject = nullptr;

    // QAction
    // Scene
    m_pCreateSceneAction = nullptr;
    m_pCreateGroupAction = nullptr;
    m_pCreateEquipAction = nullptr;
    m_pDeleteSceneAction = nullptr;
    m_pCreateModelNoAction = nullptr;
    m_pDeleteEquipmentAction = nullptr;
    m_pProjectCopyAction = nullptr;
    m_pProjectImportAction = nullptr;
    m_pProjectExportAction = nullptr;
    m_pProjectDeleteAction = nullptr;
}

void equipmentmodel::Fini()
{
    // menu
    if (nullptr != m_pMenuScene)
    {
        delete m_pMenuScene;
        m_pMenuScene = nullptr;
    }

    if (nullptr != m_pMenuEquipment)
    {
        delete m_pMenuEquipment;
        m_pMenuEquipment = nullptr;
    }

    if (nullptr != m_pMenuModelNo)
    {
        delete m_pMenuModelNo;
        m_pMenuModelNo = nullptr;
    }

    if (nullptr != m_pMenuProject)
    {
        delete m_pMenuProject;
        m_pMenuProject = nullptr;
    }

    // left
    if (nullptr != m_pFileBtn)
    {
        delete m_pFileBtn;
        m_pFileBtn = nullptr;
    }

    /*if (nullptr != m_pTreeSaveBtn)
    {
        delete m_pTreeSaveBtn;
        m_pTreeSaveBtn = nullptr;
    }*/

    // 先删除Item    
    if (nullptr != m_pTreeView)
    {
        int cnt = m_pTreeModel->rowCount();
        for (int i = 0; i < cnt; ++i)
        {
            QStandardItem* item = m_pTreeModel->item(i);
            if (nullptr != item)
            {
                int nFileItemCnt = item->rowCount();
                for (int j = 0; j < nFileItemCnt; ++j)
                {
                    item->removeRow(j);
                }
            }

            m_pTreeModel->removeRow(i);
        }

        if (nullptr != m_pTreeModel)
        {
            delete m_pTreeModel;
            m_pTreeModel = nullptr;
        }

        delete m_pTreeView;
        m_pTreeView = nullptr;
    }

    if (nullptr != m_pCreateSceneAction)
    {
        delete m_pCreateSceneAction;
        m_pCreateSceneAction = nullptr;
    }

    if (nullptr != m_pCreateGroupAction)
    {
        delete m_pCreateGroupAction;
        m_pCreateGroupAction = nullptr;
    }

    if (nullptr != m_pCreateEquipAction)
    {
        delete m_pCreateEquipAction;
        m_pCreateEquipAction = nullptr;
    }

    if (nullptr != m_pDeleteSceneAction)
    {
        delete m_pDeleteSceneAction;
        m_pDeleteSceneAction = nullptr;
    }

    if (nullptr != m_pCreateModelNoAction)
    {
        delete m_pCreateModelNoAction;
        m_pCreateModelNoAction = nullptr;
    }

    if (nullptr != m_pDeleteEquipmentAction)
    {
        delete m_pDeleteEquipmentAction;
        m_pDeleteEquipmentAction = nullptr;
    }

    if (nullptr != m_pProjectCopyAction)
    {
        delete m_pProjectCopyAction;
        m_pProjectCopyAction = nullptr;
    }

    if (nullptr != m_pProjectImportAction)
    {
        delete m_pProjectImportAction;
        m_pProjectImportAction = nullptr;
    }

    if (nullptr != m_pProjectExportAction)
    {
        delete m_pProjectExportAction;
        m_pProjectExportAction = nullptr;
    }

    if (nullptr != m_pProjectDeleteAction)
    {
        delete m_pProjectDeleteAction;
        m_pProjectDeleteAction = nullptr;
    }

    // right    
    if (nullptr != m_pComboxAnalogDelegate)
    {
        delete m_pComboxAnalogDelegate;
        m_pComboxAnalogDelegate = nullptr;
    }

    if (nullptr != m_pEquipView)
    {
        delete m_pEquipView;
        m_pEquipView = nullptr;
    }

    if (nullptr != m_pContentModel)
    {
        delete m_pContentModel;
        m_pContentModel = nullptr;
    }

    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }

    // action
    if (nullptr != m_pSaveAct)
    {
        delete m_pSaveAct;
        m_pSaveAct = nullptr;
    }
    if (nullptr != m_pAddAct)
    {
        delete m_pAddAct;
        m_pAddAct = nullptr;
    }
    if (nullptr != m_pDelAct)
    {
        delete m_pDelAct;
        m_pDelAct = nullptr;
    }

    // 
    if (nullptr != m_pLeftSplitter)
    {
        delete m_pLeftSplitter;
        m_pLeftSplitter = nullptr;
    }

    if (nullptr != m_pRightSplitter)
    {
        delete m_pRightSplitter;
        m_pRightSplitter = nullptr;
    }

    if (nullptr != m_pMainSplitter)
    {
        delete m_pMainSplitter;
        m_pMainSplitter = nullptr;
    }
}

// 创建工具条
void equipmentmodel::CreateToolBar()
{
    m_pSaveAct = new QAction(QStringLiteral("保存"), menuBar());
    m_pSaveAct->setIcon(QIcon(":/images/m_save.png"));
    m_pSaveAct->setEnabled(false);
    connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(Slot_SavePoint()));

    m_pAddAct = new QAction(QStringLiteral("新增"), menuBar());
    m_pAddAct->setIcon(QIcon(":/images/add_model.png"));
    m_pAddAct->setEnabled(false);
    connect(m_pAddAct, SIGNAL(triggered()), this, SLOT(Slot_AddPoint()));

    m_pDelAct = new QAction(QStringLiteral("删除"), menuBar());
    m_pDelAct->setIcon(QIcon(":/images/delete_model.png"));
    m_pDelAct->setEnabled(false);
    connect(m_pDelAct, SIGNAL(triggered()), this, SLOT(Slot_DelPoint()));

    QToolBar* pToolBar = addToolBar("ModelTool");
    pToolBar->addAction(m_pSaveAct);
    pToolBar->addAction(m_pAddAct);
    pToolBar->addAction(m_pDelAct);
}

void equipmentmodel::CreateLeftSplitter()
{
    // 右击事件需调用
    setContextMenuPolicy(Qt::CustomContextMenu);

    /********************************************* 左上按钮 *************************************************/
    //
    /*m_pFileBtn = new QPushButton(m_pLeftTopSplitter);
    m_pFileBtn->setFlat(true);
    m_pFileBtn->setIcon(QPixmap("resource/images/open.png"));
    connect(m_pFileBtn, SIGNAL(clicked()), this, SLOT(Slot_ShowFile()));*/

    /*m_pTreeSaveBtn = new QPushButton(m_pLeftTopSplitter);
    m_pTreeSaveBtn->setFlat(true);
    m_pTreeSaveBtn->setIcon(QPixmap("resource/images/m_save.png"));
    connect(m_pTreeSaveBtn, SIGNAL(clicked()), this, SLOT(Slot_TreeSave()));*/

    /********************************************* 左下树形 *************************************************/
    //

    m_pTreeModel = new QStandardItemModel();
    m_pTreeView = new CTreeView(m_pTreeModel);
    m_pTreeView->setModel(m_pTreeModel);

    connect(m_pTreeView->header(), SIGNAL(sectionPressed(int)), this, SLOT(Slot_MousePressEvent(int)));

    // 模型头
    m_pTreeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("模型配置"));
    connect(m_pTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_Clicked(const QModelIndex &)));

    // 模型场景
    m_pItemRoot = new QStandardItem(QIcon(QStringLiteral(":/images/model_tool.png")), M_APPLICATION);
    m_pItemRoot->setData(TYPE_ROOT, ROLE_ROOT); // 首先它属于场景
    m_pTreeModel->appendRow(m_pItemRoot);

    // 展开组
    // SIGNAL(expanded(const QModelIndex&))
    connect(m_pTreeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(Slot_TreeTitleChanged(QStandardItem*)));

    // 加载原始数据
    LoadSourceData();

    // 树型右击事件
    connect(m_pTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_RightClicked(const QPoint &)));

    m_pLeftSplitter->addWidget(m_pTreeView);

    CreateMenu();

    m_pTreeView->expandAll();
}

void equipmentmodel::LoadSourceData()
{
    if (m_pModelLib->m_mapScene.size() > 0)
    {
        int nSceneIndex = 0;
        for each (auto i in m_pModelLib->m_mapScene.keys())
        {
            m_pItemRoot = new QStandardItem(QIcon(QStringLiteral(":/images/m_scene.png")), i);

            m_pItemRoot->setData(TYPE_SCENE, ROLE_ROOT); // 首先它属于root
            m_pItemRoot->setData(TYPE_SCENE, ROLE_SCENE); //其次它是场景

            m_pTreeModel->item(0)->appendRow(m_pItemRoot);

            LoadEquip(i, nSceneIndex);

            ++nSceneIndex;
        }
    }
}

void equipmentmodel::LoadEquip(QString& strSceneKey, int nSceneIndex/* = 0*/)
{
    Q_ASSERT(m_pModelLib);

    QStandardItem* pEquipStandItem = m_pTreeView->GetEquipmentItem(nSceneIndex);
    if (nullptr == pEquipStandItem)
    {
        return;
    }

    // 加载
    if (m_pModelLib->m_mapScene[strSceneKey].mapEquipName.size() > 0)
    {
        int nIndex = 0;
        auto& mapEquip = m_pModelLib->m_mapScene[strSceneKey].mapEquipName;

        // 加载组
        for each (auto i in mapEquip.keys())
        {
            QStandardItem* pItemEquip = new QStandardItem(QIcon(QStringLiteral(":/images/m_group.png")), i);

            pItemEquip->setData(TYPE_EQUIPMENT, ROLE_ROOT);      // 首先它属于root
            pItemEquip->setData(TYPE_EQUIPMENT, ROLE_SCENE);     // 其次它属于场景
            pItemEquip->setData(TYPE_EQUIPMENT, ROLE_EQUIPMENT); // 其次它属于设备

            pEquipStandItem->appendRow(pItemEquip);

            QStandardItem* pModelStandItem = m_pTreeView->GetSceneModelItem(nSceneIndex, nIndex);
            if (nullptr == pModelStandItem)
            {
                return;
            }

            QMap<QString, QString>& mapModel = mapEquip[i];
            for each (auto j in mapModel.keys())
            {
                QStandardItem* pItemModel = new QStandardItem(QIcon(QStringLiteral(":/images/m_model.png")), j);

                pItemModel->setData(TYPE_MODEL_NO, ROLE_ROOT); // 首先它属于root
                pItemModel->setData(TYPE_MODEL_NO, ROLE_SCENE); //其次它属于场景
                pItemModel->setData(TYPE_MODEL_NO, ROLE_EQUIPMENT); //其次它属于设备组
                pItemModel->setData(TYPE_MODEL_NO, ROLE_MODEL_NO); //其次它属于设备模型

                pModelStandItem->appendRow(pItemModel);
            }

            nIndex++;
        }

        // 加载型号
        for each (auto j in m_pModelLib->m_mapScene[strSceneKey].mapModelName.keys())
        {
            QStandardItem* pItemEquip = new QStandardItem(QIcon(QStringLiteral(":/images/m_model.png")), j);

            pItemEquip->setData(TYPE_MODEL_NO, ROLE_ROOT);      // 首先它属于root
            pItemEquip->setData(TYPE_MODEL_NO, ROLE_SCENE);     // 其次它属于场景
            pItemEquip->setData(TYPE_MODEL_NO, ROLE_MODEL_NO);  // 其次它属于设备模型

            pEquipStandItem->appendRow(pItemEquip);
        }
    }
}

void equipmentmodel::CreateRightSplitter()
{
    m_pEquipView = CreateView();

    m_pRightSplitter->addWidget(m_pEquipView);
}

// 创建右击菜单
void equipmentmodel::CreateMenu()
{
    CreateSceneMenu();

    CreateEquipmentMenu();

    CreateModelNoMenu();

    CreateProjectMenu();
}

void equipmentmodel::CreateSceneMenu()
{
    m_pMenuScene = new QMenu(m_pTreeView);

    m_pCreateSceneAction = new QAction(QStringLiteral("创建场景"), this);
    connect(m_pCreateSceneAction, SIGNAL(triggered()), this, SLOT(Slot_CreateScene()));
    m_pMenuScene->addAction(m_pCreateSceneAction);
}

void equipmentmodel::Slot_CreateScene()
{
    const QModelIndex& index = m_pTreeView->currentIndex();

    QVariant var = index.data(ROLE_ROOT);
    int row = index.row();

    if (TYPE_ROOT == var.toInt())
    {
        // 获取有效文件名
        QString strName = m_pModelLib->CheckCreateSceneName(true);

        m_pItemRoot = new QStandardItem(QIcon(QStringLiteral(":/images/m_scene.png")), strName);

        m_pItemRoot->setData(TYPE_SCENE, ROLE_ROOT); // 首先它属于目录
        m_pItemRoot->setData(TYPE_SCENE, ROLE_SCENE); //其次它是目录
        m_pTreeModel->item(row)->appendRow(m_pItemRoot);

        emit Slot_TreeSave();
    }
}

void equipmentmodel::CreateEquipmentMenu()
{
    m_pMenuEquipment = new QMenu(m_pTreeView);

    m_pCreateGroupAction = new QAction(QStringLiteral("创建组"), this);
    connect(m_pCreateGroupAction, SIGNAL(triggered()), this, SLOT(Slot_CreateGroup()));

    m_pMenuEquipment->addAction(m_pCreateGroupAction);

    m_pCreateEquipAction = new QAction(QStringLiteral("创建设备型号"), this);
    connect(m_pCreateEquipAction, SIGNAL(triggered()), this, SLOT(Slot_CreateEquipModel()));

    m_pMenuEquipment->addAction(m_pCreateEquipAction);

    m_pDeleteSceneAction = new QAction(QStringLiteral("删除"), this);
    connect(m_pDeleteSceneAction, SIGNAL(triggered()), this, SLOT(Slot_DeleteModel()));

    m_pMenuEquipment->addAction(m_pDeleteSceneAction);
}

void equipmentmodel::Slot_CreateGroup()
{
    const QModelIndex& index = m_pTreeView->currentIndex();

    QVariant var = index.data(ROLE_ROOT);
    int row = index.row();

    if (TYPE_SCENE == var.toInt())
    {
        QStandardItem* pStandItem = m_pTreeView->GetEquipmentItem(index.row());
        if (nullptr == pStandItem)
        {
            return;
        }

        QString strScene = index.data().toString();

        // 获取有效文件名
        QString strName = m_pModelLib->CheckCreateGroupName(strScene, true);

        QStandardItem* pItem = new QStandardItem(QIcon(QStringLiteral(":/images/m_group.png")), strName);

        pItem->setData(TYPE_EQUIPMENT, ROLE_ROOT); // 首先它属于root
        pItem->setData(TYPE_EQUIPMENT, ROLE_SCENE); //其次它属于场景
        pItem->setData(TYPE_EQUIPMENT, ROLE_EQUIPMENT); //其次它属于设备

        pStandItem->appendRow(pItem);

        emit Slot_TreeSave();
    }
}

// 创建设备型号
void equipmentmodel::Slot_CreateEquipModel()
{
    const QModelIndex& index = m_pTreeView->currentIndex();

    QVariant var = index.data(ROLE_ROOT);
    int row = index.row();

    if (TYPE_EQUIPMENT == var.toInt())
    {
        QStandardItem* pStandItem = m_pTreeView->GetSceneModelItem(index.parent().row(), index.row());
        if (nullptr == pStandItem)
        {
            return;
        }

        QString strScene = index.parent().data().toString();
        QString strGroup = index.data().toString();

        // 获取有效文件名
        QString strName = m_pModelLib->CheckCreateModelNoName(strScene, strGroup, true);

        QStandardItem* pItem = new QStandardItem(QIcon(QStringLiteral(":/images/m_model.png")), strName);

        pItem->setData(TYPE_MODEL_NO, ROLE_ROOT); // 首先它属于root
        pItem->setData(TYPE_MODEL_NO, ROLE_SCENE); //其次它属于场景
        pItem->setData(TYPE_MODEL_NO, ROLE_EQUIPMENT); //其次它属于场景
        pItem->setData(TYPE_MODEL_NO, ROLE_MODEL_NO); //其次它属于设备模型

        pStandItem->appendRow(pItem);

        emit Slot_TreeSave();
    }
    else if (TYPE_SCENE == var.toInt()) // 场景下面创建设备型号
    {
        QStandardItem* pStandItem = m_pTreeView->GetEquipmentItem(index.row());
        if (nullptr == pStandItem)
        {
            return;
        }

        QString strScene = index.data().toString();

        // 获取有效文件名
        QString strName = m_pModelLib->CheckCreateModelNoName(strScene, true);

        QStandardItem* pItem = new QStandardItem(QIcon(QStringLiteral(":/images/m_model.png")), strName);

        pItem->setData(TYPE_MODEL_NO, ROLE_ROOT); // 首先它属于root
        pItem->setData(TYPE_MODEL_NO, ROLE_SCENE); //其次它属于场景
        pItem->setData(TYPE_MODEL_NO, ROLE_MODEL_NO); //其次它属于设备模型

        pStandItem->appendRow(pItem);

        emit Slot_TreeSave();
    }
}

void equipmentmodel::Slot_DeleteModel()
{
    QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("确定删除？？？"));
    warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    warning.setButtonText(QMessageBox::Ok, QStringLiteral("确 定"));
    warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取 消"));

    if (warning.exec() == QMessageBox::Cancel)
    {
        return;
    }

    QModelIndexList indexes = m_pTreeView->selectionModel()->selection().indexes();
    for each (QModelIndex index in indexes)
    {
        QVariant varCurrent = index.data(ROLE_ROOT);
        if (!varCurrent.isValid())
        {
            continue;
        }

        int nCurrntType = varCurrent.toInt();

        switch (nCurrntType)
        {
        case TYPE_SCENE:
        {
            //int nFileCnt = m_pTreeModel->item(index.row())->rowCount();
            // 释放treeview内存
            DeleteSceneTreeItem(index.row());

            QString strSceneName = index.model()->data(index, Qt::DisplayRole).toString();
            if (m_pTreeModel->removeRow(index.row(), index.parent()))
            { // 树形目录删除成功

                // 删除目录
                m_pModelLib->DelSceneItem(strSceneName);
            }

            emit Slot_ClearView();

            break;
        }
        case TYPE_EQUIPMENT:
        {
            //int nFileCnt = m_pTreeModel->item(index.row())->rowCount();

            QString strSceneName = index.parent().data().toString();
            QString strGroupName = index.data().toString();

            // 释放treeview内存
            DeleteGroupTreeItem(index.parent().row(), index.row());

            int nRow = index.row();
            if (m_pTreeModel->removeRow(nRow, index.parent()))
            { // 树形结构删除成功
                m_pModelLib->DelSceneGroupItem(strSceneName, strGroupName);
            }

            emit Slot_ClearView();

            break;
        }
        case TYPE_MODEL_NO:
        {
            QVariant varParent = index.parent().data(ROLE_ROOT);
            if (!varParent.isValid())
            {
                continue;
            }

            int nParentType = varParent.toInt();
            if (TYPE_SCENE == nParentType)
            {
                QString strSceneName = index.parent().data().toString();
                QString strModelName = index.data().toString();

                // 释放场景下型号item内存
                DeleteSceneModelTreeItem(index.parent().row(), index.row());

                int nRow = index.row();
                if (m_pTreeModel->removeRow(nRow, index.parent()))
                {
                    m_pModelLib->DelSceneModelNoItem(strSceneName, strModelName);
                }

                emit Slot_ClearView();
            }
            else if (TYPE_EQUIPMENT == nParentType)
            {
                QString strSceneName = index.parent().parent().data().toString();
                QString strGroupName = index.parent().data().toString();
                QString strModelName = index.data().toString();

                // 释放组下型号item内存
                DeleteGroupModelTreeItem(index.parent().parent().row(), index.parent().row(), index.row());

                int nRow = index.row();
                if (m_pTreeModel->removeRow(nRow, index.parent()))
                {
                    m_pModelLib->DelGroupModelNoItem(strSceneName, strGroupName, strModelName);
                }

                emit Slot_ClearView();
            }
            else
            {
                continue;
            }

            break;
        }
        default:
        {
            break;
        }
        }
    }

    emit Slot_TreeSave();
}

// 释放场景item内存
void equipmentmodel::DeleteSceneTreeItem(int nSceneRow)
{
    // 释放treeview内存
    QStandardItem* pRootItem = m_pTreeView->GetSceneItem();
    if (nullptr != pRootItem)
    {
        // 释放场景内存
        QStandardItem* pSceneItem = m_pTreeView->GetEquipmentItem(nSceneRow);
        if (nullptr != pSceneItem)
        {
            int nGroupRow = 0;
            while (true)
            {                
                QStandardItem* pGroupItem = pSceneItem->child(nGroupRow);
                // 释放组内存
                if (nullptr != pGroupItem)
                {
                    QString strGroupName = pGroupItem->data(Qt::EditRole).toString();
                    int nModelRow = 0;
                    while (true)
                    {
                        QStandardItem* pModelItem = pGroupItem->child(nModelRow);
                        if (nullptr != pModelItem)
                        {
                            QString strModelName = pModelItem->data(Qt::EditRole).toString();
                            delete pModelItem;
                            pModelItem = nullptr;
                        }
                        else
                        {
                            break;
                        }

                        ++nModelRow;
                    }

                    delete pGroupItem;
                    pGroupItem = nullptr;
                }
                else
                {
                    break;
                }

                ++nGroupRow;
            }

            delete pSceneItem;
            pSceneItem = nullptr;
        }
    }
}

// 释放组item内存
void equipmentmodel::DeleteGroupTreeItem(int nSceneRow, int nGroupRow)
{
    // 释放treeview内存
    QStandardItem* pParentStandItem = m_pTreeView->GetEquipmentItem(nSceneRow);
    if (nullptr != pParentStandItem)
    {
        //QString name = pParentStandItem->child(nGroupRow)->data(Qt::EditRole).toString();
        QStandardItem* pCurrentItem = pParentStandItem->child(nGroupRow);
        if (nullptr != pCurrentItem)
        {
            int nRow = 0;
            while (true)
            {
                QStandardItem* pChildItem = pCurrentItem->child(nRow);
                if (nullptr != pChildItem)
                {
                    //QString name1 = pChildItem->data(Qt::EditRole).toString();

                    delete pChildItem;
                    pChildItem = nullptr;
                }
                else
                {
                    break;;
                }
                ++nRow;
            }

            delete pCurrentItem;
            pCurrentItem = nullptr;
        }
    }
}

// 释放场景下型号item内存
void equipmentmodel::DeleteSceneModelTreeItem(int nSceneRow, int nModelRow)
{
    if (nSceneRow < 0 || nModelRow < 0)
    {
        return;
    }

    // 释放treeview内存
    QStandardItem* pParentStandItem = m_pTreeView->GetEquipmentItem(nSceneRow);
    if (nullptr != pParentStandItem)
    {
        //QString name = pParentStandItem->child(nModelRow)->data(Qt::EditRole).toString();
        QStandardItem* pCurrentItem = pParentStandItem->child(nModelRow);
        if (nullptr != pCurrentItem)
        {
            delete pCurrentItem;
            pCurrentItem = nullptr;
        }
    }
}

// 释放组下型号item内存
void equipmentmodel::DeleteGroupModelTreeItem(int nSceneRow, int nGroupRow, int nModelRow)
{
    if (nSceneRow < 0 || nGroupRow < 0 || nModelRow < 0)
    {
        return;
    }

    // 释放treeview内存
    QStandardItem* pParentStandItem = m_pTreeView->GetGroupModelItem(nSceneRow, nGroupRow, nModelRow);
    if (nullptr != pParentStandItem)
    {
        //QString name = pParentStandItem->child(nModelRow)->data(Qt::EditRole).toString();
        QStandardItem* pCurrentItem = pParentStandItem->child(nModelRow);
        if (nullptr != pCurrentItem)
        {
            delete pCurrentItem;
            pCurrentItem = nullptr;
        }
    }
}

// clear view
void equipmentmodel::Slot_ClearView()
{
    if (nullptr != m_pEquipView)
    {
        m_pEquipView->setModel(nullptr);
    }
}

void equipmentmodel::CreateModelNoMenu()
{
    m_pMenuModelNo = new QMenu(m_pTreeView);

    if (nullptr != m_pCreateEquipAction)
    {
        m_pMenuModelNo->addAction(m_pCreateEquipAction);
    }

    m_pDeleteEquipmentAction = new QAction(QStringLiteral("删除"), this);
    connect(m_pDeleteEquipmentAction, SIGNAL(triggered()), this, SLOT(Slot_DeleteModel()));

    m_pMenuModelNo->addAction(m_pDeleteEquipmentAction);
}

void equipmentmodel::CreateProjectMenu()
{
    m_pMenuProject = new QMenu(m_pTreeView);

    m_pProjectCopyAction = new QAction(QStringLiteral("复制"), this);

    m_pMenuProject->addAction(m_pProjectCopyAction);

    m_pProjectImportAction = new QAction(QStringLiteral("导入"), this);

    m_pMenuProject->addAction(m_pProjectImportAction);

    m_pProjectExportAction = new QAction(QStringLiteral("导出"), this);

    m_pMenuProject->addAction(m_pProjectExportAction);

    m_pProjectDeleteAction = new QAction(QStringLiteral("删除"), this);
    connect(m_pProjectDeleteAction, SIGNAL(triggered()), this, SLOT(Slot_DeleteModel()));

    m_pMenuProject->addAction(m_pProjectDeleteAction);
}

QTableView* equipmentmodel::CreateView()
{
    QTableView* pView = new QTableView;
    pView->setObjectName(QString::fromUtf8("analogview"));

    pView->setAlternatingRowColors(true);

    // 按行选择，删除时默认整行删除.
    pView->setSelectionBehavior(QAbstractItemView::SelectRows);//按行选择

    pView->setStyleSheet("QTableView{background-color: rgb(250, 250, 250);"
        "alternate-background-color: rgb(141, 163, 215);}");

    pView->verticalHeader()->setVisible(false);
    //m_pEquipView->horizontalHeader()->setStretchLastSection(true);  

    return pView;
}

/*********************************************************** RIGHT ********************************************************************/
void equipmentmodel::ShowData(bool bAdd/* = false*/)
{
    Q_ASSERT(m_pModelLib);

    // 设置投头
    SetGridHead();

    // 设置数据
    SetLoadGridData(bAdd);

    m_pContentModel->RefrushModel();
}

void equipmentmodel::InitDataTypeComboData()
{
    m_vecAnalogCombo.clear();

    ID_DESC tmp;
    tmp.ID = "0";
    tmp.desc = M_BIT;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "1";
    tmp.desc = M_SIGNED_CHAR;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "2";
    tmp.desc = M_UNSIGNED_CHAR;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "3";
    tmp.desc = M_SIGNED_WORD;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "4";
    tmp.desc = M_UNSIGNED_WORD;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "5";
    tmp.desc = M_SIGNED_DOUBLE_WORD;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "6";
    tmp.desc = M_UNSIGNED_DOUBLE_WORD;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "7";
    tmp.desc = M_FLOAT;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "8";
    tmp.desc = M_DOUBLE;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "9";
    tmp.desc = M_STRING;
    m_vecAnalogCombo.append(tmp);

    tmp.ID = "10";
    tmp.desc = M_ARRIBUTE;
    m_vecAnalogCombo.append(tmp);
}

void equipmentmodel::CreateData()
{
    CEquipmentModel* pEquipModel = nullptr;
    if (TYPE_SCENE == m_nCurrentMode)
    {
        pEquipModel = m_pModelLib->GetEquipModel(m_strSceneName, m_strCurrentModelName);
        if (nullptr == pEquipModel)
        {
            m_pCurrentEquipModel = &(m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipModel[m_strCurrentModelName]);
        }
        else
        {
            m_pCurrentEquipModel = pEquipModel;
        }
    }
    else if (TYPE_EQUIPMENT == m_nCurrentMode)
    {
        pEquipModel = m_pModelLib->GetEquipModel(m_strSceneName, m_strCurrentGroupName, m_strCurrentModelName);
        if (nullptr == pEquipModel)
        {
            m_pCurrentEquipModel = &(m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipGroup[m_strCurrentGroupName][m_strCurrentModelName]);
        }
        else
        {
            m_pCurrentEquipModel = pEquipModel;
        }
    }
    else
    {
        return;
    }

    m_pContentModel = new CModelToolModel(m_pCurrentEquipModel->m_arrAttrs);
    SetGridHead();

    m_pEquipView->setModel(m_pContentModel);

    if (nullptr == m_pComboxAnalogDelegate)
    {
        InitDataTypeComboData();

        m_pComboxAnalogDelegate = new CComboxDelegate(m_vecAnalogCombo, m_pContentModel);

        m_pEquipView->setItemDelegateForColumn(3, m_pComboxAnalogDelegate);
    }
}

// 加载数据
void equipmentmodel::SetLoadGridData(bool bAdd)
{
    LoadAnalog(bAdd);
}

// 加载遥测
int equipmentmodel::LoadAnalog(bool bAdd/* = false*/)
{
    Q_ASSERT(m_pModelLib);
    Q_ASSERT(m_pCurrentEquipModel);

    if (bAdd)
    {
        int nRow = 0;

        nRow = m_pCurrentEquipModel->GetAttrSize();

        CContentInfo contInfo;        
        contInfo.m_bReadWrite = true;
        contInfo.m_nDataType = 0;
        contInfo.m_strName = M_NEW_NAME.toStdString();
        contInfo.m_strDescribe = M_NEW_DESCRIBE.toStdString();

        // 保存新增的数据
        m_pCurrentEquipModel->AddAttr(nRow, contInfo);

        m_pContentModel->m_modelInfo.m_mapContentInfo[nRow].m_bReadWrite = contInfo.m_bReadWrite;
        m_pContentModel->m_modelInfo.m_mapContentInfo[nRow].m_nDataType = contInfo.m_nDataType;
        m_pContentModel->m_modelInfo.m_mapContentInfo[nRow].m_strName = contInfo.m_strName;
        m_pContentModel->m_modelInfo.m_mapContentInfo[nRow].m_strDescribe = contInfo.m_strDescribe;
    }

    return 1;
}

void equipmentmodel::ReleaseData()
{
    if (nullptr != m_pComboxAnalogDelegate)
    {
        delete m_pComboxAnalogDelegate;
        m_pComboxAnalogDelegate = nullptr;
    }

    if (nullptr != m_pContentModel)
    {
        delete m_pContentModel;
        m_pContentModel = nullptr;
    }
}

void equipmentmodel::SetGridHead()
{
    QStringList headerList;

    headerList << ("NO")
        << QStringLiteral("名称")
        << QStringLiteral("描述")
        << QStringLiteral("类型")
        << QStringLiteral("读写属性"); // 默认只读

    m_pContentModel->SetHeadTitles(headerList);

}

/*********************************************************** LEFT SLOTS ***************************************************************/
void equipmentmodel::Slot_MousePressEvent(int index)
{
    QModelIndexList indexes = m_pTreeView->selectionModel()->selection().indexes();
    foreach(QModelIndex index, indexes)
    {
        m_pTreeView->selectionModel()->clear();
    }
}

void equipmentmodel::Slot_ShowFile()
{
    QString str = QFileDialog::getOpenFileName(this, QStringLiteral("设备配置"), "/",
        "C++ files(equipmentmodel.xml)");
}

void equipmentmodel::Slot_TreeSave()
{
    if (nullptr != m_pModelLib)
    {
        m_pModelLib->SaveToFile("");
    }

    m_bNeedSaveFlag = false;
    //m_pTreeSaveBtn->setEnabled(false);
}

void equipmentmodel::Slot_Clicked(const QModelIndex &index)
{
    Q_ASSERT(m_pModelLib);

    QVariant varCurrent = index.data(ROLE_ROOT);
    QVariant varParent = index.parent().data(ROLE_ROOT);
    if (!varCurrent.isValid() || !varParent.isValid())
    {
        m_pAddAct->setEnabled(false);

        return;
    }

    int nCurrentType = varCurrent.toInt();
    switch (nCurrentType)
    {
    case TYPE_MODEL_NO:
    {
        if (TYPE_EQUIPMENT == varParent.toInt())
        {
            m_pAddAct->setEnabled(true);
            m_pSaveAct->setEnabled(true);

            // 重复点击同一模型不触发tabchanged事件
            // 点击组下面的型号
            if (m_strCurrentModelName != index.data().toString() ||
                m_strSceneName != index.parent().data().toString() ||
                m_strCurrentGroupName != index.parent().parent().data().toString())
            {
                m_nCurrentMode = varParent.toInt();

                m_strCurrentModelName = index.data().toString();
                m_strCurrentGroupName = index.parent().data().toString();
                m_strSceneName = index.parent().parent().data().toString();

                // 当前修改类型
                m_nCurrentModifyType = M_MODIFY_GROUP_MODEL_NO;

                ReleaseData();
                CreateData();

                emit Slot_ShowPoint();
            }
        }
        else if (TYPE_SCENE == varParent.toInt())
        {
            m_pAddAct->setEnabled(true);
            m_pSaveAct->setEnabled(true);

            // 重复点击同一模型不触发tabchanged事件
            // 点击场景下面的型号
            if (m_strCurrentModelName != index.data().toString() ||
                m_strSceneName != index.parent().data().toString())
            {
                m_nCurrentMode = varParent.toInt();

                m_strCurrentModelName = index.data().toString();
                m_strSceneName = index.parent().data().toString();
                m_strCurrentGroupName = "";

                // 当前修改类型
                m_nCurrentModifyType = M_MODIFY_SCENE_MODEL_NO;

                ReleaseData();
                CreateData();

                emit Slot_ShowPoint();
            }
        }
        break;
    }
    case TYPE_EQUIPMENT:
    {
        // 当前修改类型
        m_nCurrentModifyType = M_MODIFY_SCENE_GROUP;

        m_strSceneName = index.parent().data().toString();
        m_strCurrentGroupName = index.data().toString();
        m_strCurrentModelName = "";

        m_pAddAct->setEnabled(false);
        m_pDelAct->setEnabled(false);

        break;
    }
    case TYPE_SCENE:
    {
        // 当前修改类型
        m_nCurrentModifyType = M_MODIFY_SCENE;

        m_nCurrentMode = varParent.toInt();

        m_strSceneName = index.data().toString();
        m_strCurrentGroupName = "";
        m_strCurrentModelName = "";

        m_pAddAct->setEnabled(false);
        m_pDelAct->setEnabled(false);

        break;
    }
    default:
        break;
    }
}

void equipmentmodel::Slot_TreeTitleChanged(QStandardItem* item)
{
    auto strNewValue = item->data(Qt::EditRole).toString();

    switch (m_nCurrentModifyType)
    {
    case M_MODIFY_SCENE:// 修改场景
    {
        if (strNewValue != m_strSceneName)
        {
            if (strNewValue.isEmpty())
            {
                item->setData(m_strSceneName, Qt::EditRole);
            }
            else
            {
                if (!m_pModelLib->ModifyScene(m_strSceneName, strNewValue))
                {
                    item->setData(m_strSceneName, Qt::EditRole);
                }
            }
        }

        break;
    }
    case M_MODIFY_SCENE_GROUP:// 修改组
    {
        if (strNewValue != m_strCurrentGroupName)
        {
            if (strNewValue.isEmpty())
            {
                item->setData(m_strSceneName, Qt::EditRole);
            }
            else
            {
                if (!m_pModelLib->ModifySceneGroup(m_strSceneName, m_strCurrentGroupName, strNewValue))
                {
                    item->setData(m_strCurrentGroupName, Qt::EditRole);
                }
            }
        }
        break;
    }
    case M_MODIFY_SCENE_MODEL_NO:// 修改场景下面的型号
    {
        if (strNewValue != m_strCurrentModelName)
        {
            if (strNewValue.isEmpty())
            {
                item->setData(m_strSceneName, Qt::EditRole);
            }
            else
            {
                if (!m_pModelLib->ModifySceneModelNo(m_strSceneName, m_strCurrentModelName, strNewValue))
                {
                    item->setData(m_strCurrentModelName, Qt::EditRole);
                }
            }
        }
        break;
    }
    case M_MODIFY_GROUP_MODEL_NO:// 修改组下面的型号
    {
        if (strNewValue != m_strCurrentModelName)
        {
            if (strNewValue.isEmpty())
            {
                item->setData(m_strSceneName, Qt::EditRole);
            }
            else
            {
                if (!m_pModelLib->ModifyGroupModelNo(m_strSceneName, m_strCurrentGroupName, m_strCurrentModelName, strNewValue))
                {
                    item->setData(m_strCurrentModelName, Qt::EditRole);
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

// 右击树形结构事件
void equipmentmodel::Slot_RightClicked(const QPoint& pos)
{
    QModelIndex index = m_pTreeView->indexAt(pos);

    QVariant varCurrent = index.data(ROLE_ROOT);
    QVariant varParent = index.parent().data(ROLE_ROOT);
    if (!varCurrent.isValid())
    {
        return;
    }

    int nCurrntType = varCurrent.toInt();
    switch (nCurrntType)
    {
    case TYPE_ROOT:
    {
        m_pMenuScene->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
        break;
    }
    case TYPE_SCENE:
    {
        m_nCurrentDeleteType = M_DELETE_SCENE;

        m_pMenuEquipment->exec(QCursor::pos());
        break;
    }
    case TYPE_EQUIPMENT:
    {
        m_nCurrentDeleteType = M_DELETE_SCENE_GROUP;

        m_pMenuModelNo->exec(QCursor::pos());
        break;
    }
    case TYPE_MODEL_NO:
    {
        int nParentType = varParent.toInt();
        if (TYPE_SCENE == nParentType)
        {
            m_nCurrentDeleteType = M_DELETE_SCENE_MODEL_NO;
        }
        else if (TYPE_EQUIPMENT == nParentType)
        {
            m_nCurrentDeleteType = M_DELETE_GROUP_MODEL_NO;
        }
        else
        {
            m_nCurrentDeleteType = -1;
        }

        m_pMenuProject->exec(QCursor::pos());
        break;
    }
    default:
    {
        break;
    }
    }
}

// left


/*********************************************************** RIGHT SLOTS ***************************************************************/
void equipmentmodel::Slot_AddPoint()
{
    ShowData(true);

    m_pSaveAct->setEnabled(true);
    m_pAddAct->setEnabled(true);
    m_pDelAct->setEnabled(true);

    // 设置修改标志
    SetSaveFlag(true);
}

void equipmentmodel::Slot_ShowPoint()
{
    ShowData();

    m_pAddAct->setEnabled(true);
    m_pDelAct->setEnabled(true);
}

void equipmentmodel::Slot_DelPoint()
{
    Q_ASSERT(m_pContentModel);

    if (nullptr != m_pEquipView)
    {
        QModelIndexList indexes = m_pEquipView->selectionModel()->selection().indexes();

        int nCnt = m_pEquipView->selectionModel()->selection().count();
        if (nCnt == 0)
        {
            return;
        }

        int nSize = indexes.count(); // 行 乘以 列  的数量

        int nHeadCnt = 4;// nSize / nCnt;
        for (int i = 0; i < nSize;)
        {
            QModelIndex index = indexes.at(i);
            //QString str = index.data().toString();
            int row = index.row();

            m_pContentModel->removeRow(row);

            i += nHeadCnt;

            auto iterInfo = m_pCurrentEquipModel->m_arrAttrs.m_mapContentInfo.find(row);
            if (iterInfo != m_pCurrentEquipModel->m_arrAttrs.m_mapContentInfo.end())
            {
                //iterFile->erase(iterInfo);
                m_pCurrentEquipModel->m_arrAttrs.m_mapContentInfo.erase(iterInfo);
            }
        }

        // 全局数据和内容数据同步
        auto& mapInfo = m_pCurrentEquipModel->m_arrAttrs;

        if (TYPE_SCENE == m_nCurrentMode)
        {
            CModelInfo& arrAttrs = m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipModel[m_strCurrentModelName].m_arrAttrs;
            arrAttrs.m_mapContentInfo.clear();

            int j = 0;
            for each (auto i in mapInfo.m_mapContentInfo)
            {
                arrAttrs.m_mapContentInfo[j] = i;
                j++;
            }
        }
        else if (TYPE_EQUIPMENT == m_nCurrentMode)
        {
            CModelInfo& arrAttrs = m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipGroup[m_strCurrentGroupName][m_strCurrentModelName].m_arrAttrs;
            arrAttrs.m_mapContentInfo.clear();

            int j = 0;
            for each (auto i in mapInfo.m_mapContentInfo)
            {
                arrAttrs.m_mapContentInfo[j] = i;
                j++;
            }
        }

        emit Slot_ShowPoint();
    }

    // 设置修改标志
    SetSaveFlag(true);
}

void equipmentmodel::Slot_SavePoint()
{
    QMutexLocker locker(&m_mutex);
    // 做过修改才能保存
    if (!m_bNeedSaveFlag)
    {
        if (!g_bModifyFlag)
        {
            return;
        }
    }
    

    // 检查目录结构
    // todo:新增当前选中文件check，新增保存全部.
    /*if (!CheckModelFolder())
    {
    return;
    }*/

    QString strCurrentPath = "";
    if (TYPE_SCENE == m_nCurrentMode)
    { // 场景模式
        CEquipmentModel* pEquipModel = m_pModelLib->GetEquipModel(m_strSceneName, m_strCurrentModelName);

        strCurrentPath = M_SAVE_MODEL_FOLDER_NAME + "//" + m_strSceneName;

        const QString strGuid = GetGuid();
        if (!strGuid.isEmpty())
        {
            pEquipModel->SaveToFile(strCurrentPath, m_strCurrentModelName, strGuid);
        }
    }
    else if (TYPE_EQUIPMENT == m_nCurrentMode)
    { // 组模式
        CEquipmentModel* pEquipModel = m_pModelLib->GetEquipModel(m_strSceneName, m_strCurrentGroupName, m_strCurrentModelName);

        strCurrentPath = M_SAVE_MODEL_FOLDER_NAME + "//" + m_strSceneName + "//" + m_strCurrentGroupName;

        const QString strGuid = GetGuid();
        if (!strGuid.isEmpty())
        {
            pEquipModel->SaveToFile(strCurrentPath, m_strCurrentModelName, strGuid);
        }
    }

    m_bNeedSaveFlag = false;
    g_bModifyFlag = false;
    //m_pSaveAct->setEnabled(false);
}

bool equipmentmodel::CheckModelFolder()
{
    QDir dir;
    // 模型目录创建
    if (!dir.exists(M_SAVE_MODEL_FOLDER_NAME))
    {
        bool bRes = dir.mkdir(M_SAVE_MODEL_FOLDER_NAME);
        if (!bRes)
        {
            return false;
        }
    }

    QString strSceneName = "";
    QString strGroupName = "";

    QString strFileName = "";

    QString strCreatePath = "";
    QString strCreatePathName = "";

    // 创建场景、组目录
    for each (auto i in m_pModelLib->m_mapScene.keys())
    {
        strSceneName = i;
        strCreatePath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName;
        if (!dir.exists(strCreatePath))
        {
            bool bRes = dir.mkpath(strCreatePath);
            if (!bRes)
            {
                return false;
            }
        }

        // 组
        for each (auto j in m_pModelLib->m_mapScene[i].mapEquipName.keys())
        {
            strGroupName = j;
            strCreatePath = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strGroupName;
            if (!dir.exists(strCreatePath))
            {
                bool bRes = dir.mkpath(strCreatePath);
                if (!bRes)
                {
                    return false;
                }
            }

            // 创建文件
            for each (auto k in m_pModelLib->m_mapScene[i].mapEquipName[j])
            {
                strFileName = k;

                strCreatePathName = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strGroupName + "//" + strFileName;
                QFile file(strCreatePathName);
                if (!file.exists())
                {
                    bool b = file.open(QIODevice::WriteOnly);
                    if (!b)
                    {
                        return false;
                    }

                    file.close();
                }
            }
        }

        // 型号
        for each (auto j in m_pModelLib->m_mapScene[i].mapModelName)
        {
            strFileName = j;

            strCreatePathName = M_SAVE_MODEL_FOLDER_NAME + "//" + strSceneName + "//" + strFileName;
            QFile file(strCreatePathName);
            if (!file.exists())
            {
                bool b = file.open(QIODevice::WriteOnly);
                if (!b)
                {
                    return false;
                }

                file.close();
            }
        }
    }

    return true;
}

const QString equipmentmodel::GetGuid()
{
    if (TYPE_SCENE == m_nCurrentMode)
    {
        auto iterScene = m_pModelLib->m_mapScene.find(m_strSceneName);
        if (iterScene != m_pModelLib->m_mapScene.end())
        {
            auto iterModel = iterScene->mapModelName.find(m_strCurrentModelName);
            if (iterModel != iterScene->mapModelName.end())
            {
                return iterScene->mapModelName[m_strCurrentModelName];
            }
        }
    }
    else if (TYPE_EQUIPMENT == m_nCurrentMode)
    {
        auto iterScene = m_pModelLib->m_mapScene.find(m_strSceneName);
        if (iterScene != m_pModelLib->m_mapScene.end())
        {
            auto iterGroup = iterScene->mapEquipName.find(m_strCurrentGroupName);
            if (iterGroup != iterScene->mapEquipName.end())
            {
                auto iterModel = iterGroup->find(m_strCurrentModelName);
                if (iterModel != iterGroup->end())
                {

                    return iterScene->mapEquipName[m_strCurrentGroupName][m_strCurrentModelName];
                }
            }
        }
    }

    return "";
}

equipmentmodel* GetModelToolDll()
{
    return new equipmentmodel();
}