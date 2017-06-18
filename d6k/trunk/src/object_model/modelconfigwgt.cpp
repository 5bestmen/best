#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include "modelconfigwgt.h"
#include "model_container.h"
#include "xmldataanalyse.h"

CModelConfigWgt* GetModelToolDll()
{
    return new CModelConfigWgt();
}

CModelConfigWgt::CModelConfigWgt(QWidget *parent)
    : QWidget(parent),
    m_pTopLevelItem(nullptr),
    m_pDataModel(new CModelContainer),
    m_pXmlAnalyse(new CXmlDataAnalyse(m_pDataModel))
{
    ui.setupUi(this);
    m_UpdateFlag = false;

    ui.splitter->setStretchFactor(0, 1);
    ui.splitter->setStretchFactor(1, 3);
    //TODO
    m_strTypeMap.insert(0, tr("Bit"));
    m_strTypeMap.insert(1, tr("Signed Char"));
    m_strTypeMap.insert(2, tr("Unsigned Char"));
    m_strTypeMap.insert(3, tr("Signed Word"));
    m_strTypeMap.insert(4, tr("Unsigned Word"));
    m_strTypeMap.insert(5, tr("Signed Double Word"));
    m_strTypeMap.insert(6, tr("Unsigned Double Word"));
    m_strTypeMap.insert(7, tr("Float"));
    m_strTypeMap.insert(8, tr("Double"));
    m_strTypeMap.insert(9, tr("String"));
    //
    InitWgt();
    m_pTopLevelItem->setExpanded(true);
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CModelConfigWgt::InitWgt()
{
    if (ui.treeWidget->topLevelItemCount() == 1)
    {
        m_pTopLevelItem = ui.treeWidget->topLevelItem(0);
        m_pTopLevelItem->setData(0, Qt::UserRole, ITEM_APPLICATION);

    }

    if (m_pTopLevelItem == nullptr)
    {
        return;
    }

    QString strFile = qApp->applicationDirPath() + TREEXMLFILE;
    m_pXmlAnalyse->AnalyseTreeItems(strFile);
    //构造树
    InitOriginalData();
    //树
    ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_TreeContextMenuRequest(const QPoint &)));
    connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeItemClicked(QTreeWidgetItem *, int)));
    connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeItemDoubleClick(QTreeWidgetItem *, int)));
    connect(ui.treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(Slot_TreeItemDataChange(QTreeWidgetItem *, int)));
    //table右击
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_TableContextMenuRequest(const QPoint &)));
    connect(ui.tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(Slot_CellChange(int, int)));
    connect(ui.tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_ItemDoubleClick(QTableWidgetItem *)));

    //
}

void CModelConfigWgt::InitOriginalData()
{
    std::vector<std::shared_ptr<CModelScene>> pSceneInfo = m_pDataModel->GetModelScene();
    
    std::vector<std::shared_ptr<CModelScene>>::iterator it = pSceneInfo.begin();

    while (it != pSceneInfo.end())
    {
        //场景
        QString strScene = (*it)->GetName();

        QTreeWidgetItem *pSceneItem = new QTreeWidgetItem;
        pSceneItem->setData(0, Qt::UserRole, ITEM_SCENE);

        pSceneItem->setText(0,strScene);
        m_pTopLevelItem->addChild(pSceneItem);

        //添加映射
        m_mapScene.insert(pSceneItem, (*it));

        AddGroupItems((*it)->GetModelGrps(),pSceneItem);

        AddModelItems((*it)->GetModels(),pSceneItem);

        it++;
    }
}

//组下增加 
void CModelConfigWgt::AddGroupItems(std::vector<std::shared_ptr<CModelGroup>> groupModel, QTreeWidgetItem *pParentItem)
{
    std::vector<std::shared_ptr<CModelGroup>>::iterator it = groupModel.begin();

    while (it != groupModel.end())
    {
        // 组
        QString strGroup = (*it)->GetName();

        QTreeWidgetItem *pGroupItem = new QTreeWidgetItem;
        pGroupItem->setData(0,Qt::UserRole, ITEM_GROUP);
        pGroupItem->setText(0, strGroup);
        //添加映射
        m_mapGroup.insert(pGroupItem, (*it));

        pParentItem->addChild(pGroupItem);

        AddModelItems((*it)->GetModels(), pGroupItem);
        it++;
    }
}

//增加模型
void CModelConfigWgt::AddModelItems(std::vector<std::shared_ptr<CObjectModelInfo>> tModel, QTreeWidgetItem *pParentItem)
{
    std::vector<std::shared_ptr<CObjectModelInfo>>::iterator it = tModel.begin();

    while (it != tModel.end())
    {
        //模型
        QString strModel = (*it)->GetName();

        QTreeWidgetItem *pModelItem = new QTreeWidgetItem;
        pModelItem->setData(0, Qt::UserRole, ITEM_MODEL);
        pModelItem->setText(0, strModel);

        //添加映射
        m_mapModel.insert(pModelItem, (*it));

        pParentItem->addChild(pModelItem);

        AddModelChildItems(pModelItem);

        it++;
    }

}

//增加子item
void CModelConfigWgt::AddModelChildItems(QTreeWidgetItem *pParentItem)
{
    QTreeWidgetItem *itemStatic = new QTreeWidgetItem;
    itemStatic->setText(0, tr("Static"));
    itemStatic->setData(0, Qt::UserRole, ITEM_STATIC);

    QTreeWidgetItem *itemDynamic = new QTreeWidgetItem;
    itemDynamic->setText(0, tr("Dynamic"));
    itemDynamic->setData(0, Qt::UserRole, ITEM_DYNAMIC);

    pParentItem->addChild(itemStatic);
    pParentItem->addChild(itemDynamic);
}


CModelConfigWgt::~CModelConfigWgt()
{
    delete m_pDataModel;
}

void CModelConfigWgt::Slot_TreeContextMenuRequest(const QPoint &tPoint)
{
    QTreeWidgetItem *pItem = ui.treeWidget->itemAt(tPoint);

    if (pItem != nullptr)
    {
        if (pItem->data(0, Qt::UserRole) == ITEM_APPLICATION)
        {
            //根节点
            QMenu *pMenu = new QMenu(ui.treeWidget);

            QAction *pCreateScene = new QAction(tr("CreateScene"), ui.treeWidget);

            pMenu->addAction(pCreateScene);

            connect(pCreateScene, SIGNAL(triggered()), this, SLOT(Slot_CreateScene()));

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();
        }
        else if (pItem->data(0, Qt::UserRole) == ITEM_SCENE)
        {
            //根节点
            QMenu *pMenu = new QMenu(ui.treeWidget);

            QAction *pCreateGroup = new QAction(tr("CreateGroup"), ui.treeWidget);
            QAction *pCreateModel = new QAction(tr("CreateModel"), ui.treeWidget);
            QAction *pDelete = new QAction(tr("Delete"), ui.treeWidget);

            pMenu->addAction(pCreateGroup);
            pMenu->addAction(pCreateModel);
            pMenu->addAction(pDelete);

            connect(pCreateGroup, SIGNAL(triggered()), this, SLOT(Slot_CreateGroup()));
            connect(pCreateModel, SIGNAL(triggered()), this, SLOT(Slot_CreateModel()));
            connect(pDelete, SIGNAL(triggered()), this, SLOT(Slot_DeleteItem()));

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();
        }
        else if (pItem->data(0, Qt::UserRole) == ITEM_GROUP)
        {
            QMenu *pMenu = new QMenu(ui.treeWidget);

            QAction *pCreateModel = new QAction(tr("CreateModel"), ui.treeWidget);
            QAction *pDelete = new QAction(tr("Delete"), ui.treeWidget);

            pMenu->addAction(pCreateModel);
            pMenu->addAction(pDelete);

            connect(pCreateModel, SIGNAL(triggered()), this, SLOT(Slot_CreateModel()));
            connect(pDelete, SIGNAL(triggered()), this, SLOT(Slot_DeleteItem()));

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();
        }
        else if (pItem->data(0, Qt::UserRole) == ITEM_MODEL)
        {
            //模型自动生成  不做处理
            QMenu *pMenu = new QMenu(ui.treeWidget);

            QAction *pDeleteAct = new QAction(tr("Delete"), ui.treeWidget);

            pMenu->addAction(pDeleteAct);

            connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteModel()));

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();

        }
    }
    else
    {
        QMenu *pMenu = new QMenu(ui.treeWidget);

        QAction *pSave = new QAction(tr("Save"), ui.treeWidget);

        pMenu->addAction(pSave);

        connect(pSave, SIGNAL(triggered()), this, SLOT(Slot_SaveProject()));

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
}

void CModelConfigWgt::Slot_TableContextMenuRequest(const QPoint &tPoint)
{
    Q_UNUSED(tPoint);

    QTreeWidgetItem *pItem = ui.treeWidget->currentItem();

    if (pItem != nullptr)
    {
        if (pItem->data(0, Qt::UserRole) == ITEM_STATIC)
        {
            //根节点
            QMenu *pMenu = new QMenu(ui.tableWidget);

            QAction *pAddItem = new QAction(tr("Add"), ui.tableWidget);
            pMenu->addAction(pAddItem);
            
            QItemSelectionModel *selectionModel = ui.tableWidget->selectionModel();
            QModelIndexList indexes = selectionModel->selectedRows();

            if (indexes.count() > 0)
            {
                QAction *pDeleteAct = new QAction(tr("Delete"), ui.tableWidget);
                pMenu->addAction(pDeleteAct);
                connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteStaticTableItem()));

            }

            connect(pAddItem, SIGNAL(triggered()), this, SLOT(Slot_AddStaticTableItem()));


            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();
        }
        else if (pItem->data(0, Qt::UserRole) == ITEM_DYNAMIC)
        {
            //根节点
            QMenu *pMenu = new QMenu(ui.tableWidget);

            QAction *pAddItem = new QAction(tr("Add"), ui.tableWidget);
            pMenu->addAction(pAddItem);

            QItemSelectionModel *selectionModel = ui.tableWidget->selectionModel();
            QModelIndexList indexes = selectionModel->selectedRows();

            if (indexes.count() > 0)
            {
                QAction *pDeleteAct = new QAction(tr("Delete"), ui.tableWidget);
                pMenu->addAction(pDeleteAct);
                connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteStaticTableItem()));

            }

            connect(pAddItem, SIGNAL(triggered()), this, SLOT(Slot_AddDynamicTableItem()));


            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();
        }
    }
}


void CModelConfigWgt::Slot_CreateScene()
{
    QTreeWidgetItem *pSceneItem = new QTreeWidgetItem;
    int nSceneCount = m_pTopLevelItem->childCount();
    QString strSceneName = tr("Scene%1").arg(nSceneCount);

    pSceneItem->setText(0, strSceneName);
    pSceneItem->setData(0, Qt::UserRole, ITEM_SCENE);
    m_pTopLevelItem->addChild(pSceneItem);
    //更新场景模型
    std::shared_ptr<CModelScene> tSceneModel = m_pDataModel->CreateModelScene(strSceneName);
    m_mapScene.insert(pSceneItem, tSceneModel);

}

void CModelConfigWgt::Slot_CreateGroup()
{
    QTreeWidgetItem *pItem = ui.treeWidget->currentItem();

    if (pItem != nullptr)
    {
        QTreeWidgetItem *pGroupItem = new QTreeWidgetItem;
        int nSceneCount = 0;
        for (int i = 0; i<pItem->childCount(); i++)
        {
            if (pItem->child(i)->data(0, Qt::UserRole) == ITEM_GROUP)
            {
                nSceneCount++;
            }
        }

        QString strGroupName = tr("Group%1").arg(nSceneCount);
        pGroupItem->setText(0, strGroupName);
        pGroupItem->setData(0, Qt::UserRole, ITEM_GROUP);
        pItem->addChild(pGroupItem);
        //更新组
        if (m_mapScene.contains(pItem))
        {
            std::shared_ptr<CModelScene> tSceneModel = m_mapScene[pItem];
            std::shared_ptr<CModelGroup> tGroup = tSceneModel->CreateModelGroup(strGroupName);
            m_mapGroup.insert(pGroupItem, tGroup);
        }
    }
}

void CModelConfigWgt::Slot_CreateModel()
{
    QTreeWidgetItem *pItem = ui.treeWidget->currentItem();

    if (pItem != nullptr)
    {
        QTreeWidgetItem *pModelItem = new QTreeWidgetItem;
        int nModelCount = 0;
        for (int i = 0; i < pItem->childCount(); i++)
        {
            if (pItem->child(i)->data(0, Qt::UserRole) == ITEM_MODEL)
            {
                nModelCount++;
            }
        }

        QString strModelName = tr("Model%1").arg(nModelCount);
        pModelItem->setText(0, strModelName);
        pModelItem->setData(0, Qt::UserRole, ITEM_MODEL);

        pItem->addChild(pModelItem);

        CreateStaticModel(pModelItem);
        CreateDynamicModel(pModelItem);

        //更新组

        if (pItem->data(0, Qt::UserRole) == ITEM_SCENE)
        {
            if (m_mapScene.contains(pItem))
            {
                std::shared_ptr<CModelScene> tSceneModel = m_mapScene[pItem];
                std::shared_ptr<CObjectModelInfo> tModel = tSceneModel->CreateObjectModel(strModelName);
                m_mapModel.insert(pModelItem, tModel);

            }
        }
        else if (pItem->data(0, Qt::UserRole) == ITEM_GROUP)
        {
            if (m_mapGroup.contains(pItem))
            {
                std::shared_ptr<CModelGroup> tGroupModel = m_mapGroup[pItem];
                std::shared_ptr<CObjectModelInfo> tModel = tGroupModel->CreateObjectModel(strModelName);
                m_mapModel.insert(pModelItem, tModel);

            }
        }
    }
    
}

void CModelConfigWgt::Slot_DeleteItem()
{
    QTreeWidgetItem *pItem = ui.treeWidget->currentItem();
    DeleteItemsData(pItem);
    delete pItem;
}

void CModelConfigWgt::Slot_TreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    m_UpdateFlag = false;

    ui.tableWidget->clear();
    ui.tableWidget->setRowCount(0);
    ui.tableWidget->setColumnCount(0);
    //
    if (item->data(0, Qt::UserRole) == ITEM_MODEL)
    {
        //加载item数据
        if (m_mapModel.contains(item))
        {
            std::shared_ptr<CObjectModelInfo> tModel = m_mapModel[item];
            QString strTagName = tModel->GetTagName();

            CreateTableHeader(QStringList(tr("TagName")));
            CreateTableData(QStringList(strTagName));
        }
    }
    else if (item->data(0, Qt::UserRole) == ITEM_STATIC)
    {
        //加载静态数据
        //获取树节点
        QTreeWidgetItem *pPraentModel = item->parent();
        if (pPraentModel != nullptr)
        {
            if (m_mapModel.contains(pPraentModel))
            {
                std::shared_ptr<CObjectModelInfo> tModel = m_mapModel[pPraentModel];

                QStringList lstHeader;
                lstHeader << tr("Name") << tr("Destriber") << tr("Value");
                CreateTableHeader(QStringList(lstHeader));


                std::vector<std::shared_ptr<CStaticProperty>> tStatic = tModel->GetStaticProperty();
                //便利static
                std::vector<std::shared_ptr<CStaticProperty>>::iterator it = tStatic.begin();
                
                while (it != tStatic.end())
                {
                    QStringList lstRowValues;
                    lstRowValues.append((*it)->GetTagName());
                    lstRowValues.append((*it)->GetDescription());
                    lstRowValues.append((*it)->GetSzValue());

                    CreateTableData(QStringList(lstRowValues));
                    it++;
                }

               
            }
        }
    }
    else if (item->data(0, Qt::UserRole) == ITEM_DYNAMIC)
    {
        //加载动态数据

        //获取树节点
        QTreeWidgetItem *pPraentModel = item->parent();
        if (pPraentModel != nullptr)
        {
            if (m_mapModel.contains(pPraentModel))
            {
                std::shared_ptr<CObjectModelInfo> tModel = m_mapModel[pPraentModel];

                QStringList lstHeader;
                lstHeader << tr("Name") << tr("Destriber") << tr("ReadWrite") << tr("DataType");
                CreateTableHeader(QStringList(lstHeader));


                std::vector<std::shared_ptr<CDynamicProperty>> tStatic = tModel->GetDynamicProperty();
                //便利static
                std::vector<std::shared_ptr<CDynamicProperty>>::iterator it = tStatic.begin();

                while (it != tStatic.end())
                {
                    CreateDynamicTable((*it)->GetTagName(),(*it)->GetDescription(),(*it)->GetReadOnlyFlag(),(*it)->GetDataType());
                    it++;
                }

            }
        }
    }
    else
    {
        ui.tableWidget->clear();
    }
    

}

void CModelConfigWgt::Slot_TreeItemDoubleClick(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    m_strDoubleTreeName = item->text(0);
    //
    if (item->data(0, Qt::UserRole) != ITEM_STATIC && item->data(0, Qt::UserRole) != ITEM_DYNAMIC)
    {
        item->setFlags(m_pTopLevelItem->flags() | Qt::ItemIsEditable);
    }
}

void CModelConfigWgt::CreateStaticModel(QTreeWidgetItem *pItem)
{
    if (pItem != nullptr)
    {
        QTreeWidgetItem *pModelItem = new QTreeWidgetItem;

        pModelItem->setText(0, tr("Static"));
        pModelItem->setData(0, Qt::UserRole, ITEM_STATIC);

        pItem->addChild(pModelItem);
    }
}

void CModelConfigWgt::CreateDynamicModel(QTreeWidgetItem *pItem)
{
    if (pItem != nullptr)
    {
        QTreeWidgetItem *pModelItem = new QTreeWidgetItem;

        pModelItem->setText(0, tr("Dynamic"));
        pModelItem->setData(0, Qt::UserRole, ITEM_DYNAMIC);

        pItem->addChild(pModelItem);
    }
}

void CModelConfigWgt::CreateTableHeader(QStringList lstHeader)
{
    int nColumnCount = lstHeader.count();

    ui.tableWidget->setColumnCount(nColumnCount);
    ui.tableWidget->setHorizontalHeaderLabels(lstHeader);
}

void CModelConfigWgt::CreateTableData(QStringList lstData)
{
    int nRowCount = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRowCount);

    for (int i = 0; i < lstData.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(lstData.at(i));

        ui.tableWidget->setItem(nRowCount, i, item);
    }
}

void CModelConfigWgt::CreateDynamicTable(const QString &strName, const QString &strDestr, bool ReadWrite, int DataType)
{
    int nRowCount = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRowCount);


    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(strName);
    ui.tableWidget->setItem(nRowCount, 0, item0);
    
    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(strDestr);
    ui.tableWidget->setItem(nRowCount, 1, item1);

    QCheckBox *ReadCheck = new QCheckBox;
    ReadCheck->setText(tr("ReadOnly"));
    if (ReadWrite)
    {
        ReadCheck->setCheckState(Qt::Checked);
    }
    else
    {
        ReadCheck->setCheckState(Qt::Unchecked);
    }
    connect(ReadCheck, SIGNAL(stateChanged(int)), this, SLOT(Slot_CheckBoxChange(int)));
    
    ui.tableWidget->setCellWidget(nRowCount, 2, ReadCheck);

    QComboBox *pComboType = new QComboBox;
    connect(pComboType, SIGNAL(currentIndexChanged(int)),this,SLOT(Slot_ComboboxChange(int)));
    pComboType->addItems(m_strTypeMap.values());

    if (m_strTypeMap.contains(DataType))
    {
        pComboType->setCurrentText(m_strTypeMap[DataType]);
    }
    
    ui.tableWidget->setCellWidget(nRowCount, 3, pComboType);
}

void CModelConfigWgt::Slot_AddStaticTableItem()
{
    int nRowCount = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRowCount);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(tr("Static%1").arg(nRowCount));
    ui.tableWidget->setItem(nRowCount, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    ui.tableWidget->setItem(nRowCount, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem;
    ui.tableWidget->setItem(nRowCount, 2, item2);

    m_UpdateFlag = true;
    UpdateTableData();
    m_UpdateFlag = false;
}

//增加动态item
void CModelConfigWgt::Slot_AddDynamicTableItem()
{
    int nRowCount = ui.tableWidget->rowCount();

    ui.tableWidget->insertRow(nRowCount);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(tr("Dynamic%1").arg(nRowCount));
    ui.tableWidget->setItem(nRowCount, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    ui.tableWidget->setItem(nRowCount, 1, item1);

    QCheckBox *pCheck = new QCheckBox;
    pCheck->setText(tr("ReadOnly"));
    ui.tableWidget->setCellWidget(nRowCount, 2, pCheck);

    QComboBox *pCombo = new QComboBox;
    pCombo->addItems(m_strTypeMap.values());
    ui.tableWidget->setCellWidget(nRowCount, 3, pCombo);

    m_UpdateFlag = true;
    UpdateTableData();
    m_UpdateFlag = false;
}


//删除item
void CModelConfigWgt::Slot_DeleteStaticTableItem()
{
    DeleteTableItems();
}

void CModelConfigWgt::Slot_DeleteDynamicTableItem()
{
    DeleteTableItems();
}

void CModelConfigWgt::DeleteTableItems()
{
    QItemSelectionModel *selectionModel = ui.tableWidget->selectionModel();
    QModelIndexList indexes = selectionModel->selectedRows();

    int nBeginRow = -1;
    int nEndRow = -1;

    for (int i = 0; i < indexes.count(); i++)
    {
        if (i == 0)
        {
            nBeginRow = indexes.at(0).row();
            nEndRow = indexes.at(0).row();
        }

        if (nBeginRow > indexes.at(i).row())
        {
            nBeginRow = indexes.at(i).row();
        }

        if (nEndRow < indexes.at(i).row())
        {
            nEndRow = indexes.at(i).row();
        }
    }

    for (int m = nEndRow; m > nBeginRow - 1; m--)
    {
        ui.tableWidget->removeRow(m);
    }

    m_UpdateFlag = true;
    UpdateTableData();
    m_UpdateFlag = false;
}

void CModelConfigWgt::Slot_CellChange(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    if (m_UpdateFlag && column == 0)
    {
        for (int i = 0; i < ui.tableWidget->rowCount(); i++)
        {
            if ((i != row) && (ui.tableWidget->item(i, 0)->text() == ui.tableWidget->item(row, column)->text()))
            {
                ui.tableWidget->item(row, column)->setText(m_strTableItemName);
                QMessageBox::warning(this, tr("Warning"), tr("Name repetition! "));
            }
        }
    }

    UpdateTableData();
}

void CModelConfigWgt::UpdateTableData()
{
    if (m_UpdateFlag)
    {
        if (ui.treeWidget->currentItem()->data(0, Qt::UserRole) == ITEM_MODEL)
        {
            //更新数据
            UpdaetModeItemData(ui.treeWidget->currentItem());
        }
        else if (ui.treeWidget->currentItem()->data(0, Qt::UserRole) == ITEM_STATIC)
        {
            //静态
            UpdateStaticItemData(ui.treeWidget->currentItem());
        }
        else if (ui.treeWidget->currentItem()->data(0, Qt::UserRole) == ITEM_DYNAMIC)
        {
            //动态
            UpdateDynamicItemData(ui.treeWidget->currentItem());
        }
    }
    else
    {
        //qDebug() << "222222222222222222";
    }

    m_UpdateFlag = false;
}


void CModelConfigWgt::Slot_ItemDoubleClick(QTableWidgetItem *item)
{
    m_UpdateFlag = true;
    m_strTableItemName = item->text();
}

void CModelConfigWgt::UpdaetModeItemData(QTreeWidgetItem *pModelItem)
{
    std::shared_ptr<CObjectModelInfo> tModle = m_mapModel[pModelItem];
    QString strTagname;
    if (ui.tableWidget->rowCount() > 0)
    {
        strTagname = ui.tableWidget->item(0, 0)->text();
    }

    if (tModle != nullptr)
    {
        tModle->SetTagName(strTagname);
    }
}

void CModelConfigWgt::UpdateStaticItemData(QTreeWidgetItem *pStaticItem)
{
    QTreeWidgetItem *pModelItem = pStaticItem->parent();

    if (pModelItem != nullptr)
    {
        std::shared_ptr<CObjectModelInfo> tModle = m_mapModel[pModelItem];

        if (tModle != nullptr)
        {
            tModle->CearStatic();
            
            //遍历更新数据
            for (int i = 0; i < ui.tableWidget->rowCount();i++)
            {
                std::shared_ptr<CStaticProperty> tStatic = tModle->CreateStaticProperty(ui.tableWidget->item(i, 0)->text());
                tStatic->SetDescription(ui.tableWidget->item(i, 1)->text());
                tStatic->SetSzValue(ui.tableWidget->item(i, 2)->text());
            }
        }
    }
}

void CModelConfigWgt::UpdateDynamicItemData(QTreeWidgetItem *pDynamicItem)
{
    QTreeWidgetItem *pModelItem = pDynamicItem->parent();

    if (pModelItem != nullptr)
    {
        std::shared_ptr<CObjectModelInfo> tModle = m_mapModel[pModelItem];

        if (tModle != nullptr)
        {
            tModle->ClearDynamic();
            //遍历更新数据
            for (int i = 0; i < ui.tableWidget->rowCount(); i++)
            {
                std::shared_ptr<CDynamicProperty> tDynamic = tModle->CreateDynamicProperty(ui.tableWidget->item(i, 0)->text());

                tDynamic->SetDescription(ui.tableWidget->item(i, 1)->text());

                QCheckBox *CheckRead = qobject_cast<QCheckBox *>(ui.tableWidget->cellWidget(i,2));

                if (CheckRead != nullptr)
                {
                    if (CheckRead->isChecked())
                    {
                        tDynamic->SetReadOnlyFlag(true);
                    }
                    else
                    {
                        tDynamic->SetReadOnlyFlag(false);
                    }
                }


                QComboBox *pCombobox = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(i,3));

                if (pCombobox != nullptr)
                {
                    tDynamic->SetDataType(pCombobox->currentIndex());
                }
            }
        }
    }
}

void CModelConfigWgt::Slot_CheckBoxChange(int state)
{
    Q_UNUSED(state);
    m_UpdateFlag = true;
    UpdateTableData();
    m_UpdateFlag = false;
}

void CModelConfigWgt::Slot_ComboboxChange(int index)
{
    Q_UNUSED(index);
    m_UpdateFlag = true;
    UpdateTableData();
    m_UpdateFlag = false;
}

void CModelConfigWgt::Slot_TreeItemDataChange(QTreeWidgetItem *pItem, int nColumn)
{
    Q_UNUSED(nColumn);
    //重复性检查
    if (!CheckTreeRight(pItem))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Name repetition,please check!"));
        pItem->setText(0, m_strDoubleTreeName);
        return;
    }

    if (pItem->data(0, Qt::UserRole) == ITEM_SCENE)
    {
        //场景
        if (m_mapScene.contains(pItem))
        {
            std::shared_ptr<CModelScene> tScene = m_mapScene[pItem];
            tScene->SetName(pItem->text(0));
        }
    }
    else if (pItem->data(0, Qt::UserRole) == ITEM_GROUP)
    {
        //组
        if (m_mapGroup.contains(pItem))
        {
            std::shared_ptr<CModelGroup> tGroup = m_mapGroup[pItem];
            tGroup->SetName(pItem->text(0));
        }
    }
    else if (pItem->data(0, Qt::UserRole) == ITEM_MODEL)
    {
        //模型
        if (m_mapModel.contains(pItem))
        {
            std::shared_ptr < CObjectModelInfo > tModel = m_mapModel[pItem];
            tModel->SetName(pItem->text(0));
        }
    }
}

void CModelConfigWgt::Slot_DeleteModel()
{
    QTreeWidgetItem *pItem = ui.treeWidget->currentItem();
    DeleteItemsData(pItem);

    delete pItem;
}

void CModelConfigWgt::DeleteItemsData(QTreeWidgetItem *pItem)
{
    //删除item以及item下所有数据
    if (pItem->data(0, Qt::UserRole) == ITEM_SCENE)
    {
        if (m_mapScene.contains(pItem))
        {
            m_pDataModel->RemoveModelScene(pItem->text(0));
            m_mapScene.remove(pItem);
            //删除子节点
            for (int i = 0; i < pItem->childCount(); i++)
            {
                if (pItem->child(i)->data(0, Qt::UserRole) == ITEM_GROUP)
                {
                    for (int j = 0; j < pItem->child(i)->childCount(); j++)
                    {
                        m_mapModel.remove(pItem->child(i)->child(j));
                    }

                    m_mapGroup.remove(pItem->child(i));
                }
                else if (pItem->child(i)->data(0, Qt::UserRole) == ITEM_MODEL)
                {
                    m_mapModel.remove(pItem->child(i));
                }
            }

        }
    }
    else if (pItem->data(0, Qt::UserRole) == ITEM_GROUP)
    {
        if (m_mapGroup.contains(pItem))
        {
            if ((pItem->parent() != nullptr) && (m_mapScene.contains(pItem->parent())))
            {
                std::shared_ptr<CModelScene> tScene = m_mapScene[pItem->parent()];
                tScene->RemoveModelGroup(pItem->text(0));

                //删除子节点
                for (int i = 0; i < pItem->childCount(); i++)
                {
                    if (pItem->child(i)->data(0, Qt::UserRole) == ITEM_MODEL)
                    {
                        m_mapModel.remove(pItem->child(i));
                    }
                }
            }
        }
    }
    else if (pItem->data(0, Qt::UserRole) == ITEM_MODEL)
    {
        if (m_mapModel.contains(pItem))
        {
            if ((pItem->parent() != nullptr))
            {
                if (pItem->parent()->data(0, Qt::UserRole) == ITEM_GROUP)
                {
                    if (m_mapGroup.contains(pItem->parent()))
                    {
                        std::shared_ptr<CModelGroup> tGroup = m_mapGroup[pItem->parent()];
                        tGroup->RemoveObjectModel(pItem->text(0));

                    }
                }
                else if (pItem->parent()->data(0, Qt::UserRole) == ITEM_SCENE)
                {
                    if (m_mapScene.contains(pItem->parent()))
                    {
                        std::shared_ptr<CModelScene> tScene = m_mapScene[pItem->parent()];
                        tScene->RemoveObjectModel(pItem->text(0));
                    }
                }

                m_mapModel.remove(pItem);
            }
        }
    }

}

void CModelConfigWgt::Slot_SaveProject()
{
    m_pXmlAnalyse->SaveDataToFile();
}

bool CModelConfigWgt::CheckTreeRight(QTreeWidgetItem *pItem)
{
    //查询统计重复
    QTreeWidgetItem *pParent = pItem->parent();
    
    if (pParent != nullptr)
    {
        for (int i = 0; i < pParent->childCount(); i++)
        {
             if (pParent->child(i) != pItem)
             {
                 if (pParent->child(i)->text(0) == pItem->text(0))
                 {
                     return false;
                 }
             }
        }
    }

    return true;
}

