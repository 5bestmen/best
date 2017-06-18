
#include "modeltool.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QFileDialog>
#include <QStyleFactory>
#include <QListWidgetItem>
#include <QTextEdit>

#include "modeldefine.h"

modeltool::modeltool()
{
    ui.setupUi(this);

    setWindowIcon(QIcon(QStringLiteral("resource/images/model_tool.png")));

    Init();

    m_pModelLib = new CModelLib();

    // ��ȡtreeԭʼ����
    if (!m_pModelLib->LoadTreeDataFromFile(""))
    {
        return;
    }

    // ��ȡviewԭʼ����
    m_pModelLib->LoadViewDataFromFile("");

    // �������ָ�� ���÷���Ϊˮƽ�ָ��  
    m_pMainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // ���Splitter
    m_pLeftSplitter = new QSplitter(Qt::Vertical, m_pMainSplitter);

    // ����Splitter
    m_pLeftTopSplitter = new QSplitter(Qt::Horizontal, m_pLeftSplitter);

    // �Ҳ�Splitter
    m_pRightSplitter = new QSplitter(Qt::Vertical, m_pMainSplitter);

    // ����Splitter
    m_pRightTopSplitter = new QSplitter(Qt::Horizontal, m_pRightSplitter);

    CreateLeftSplitter();

    CreateRightSplitter();

    m_pMainSplitter->setStretchFactor(1, 1);
    m_pMainSplitter->setWindowTitle(QObject::tr("�ָ��"));

    setCentralWidget(m_pMainSplitter);
}

modeltool::~modeltool()
{
    Fini();
}

void modeltool::Init()
{
    m_pModelLib = nullptr;
    m_nCurrentMode = -1;
    m_pCurrentEquipModel = nullptr;
    m_pComboxAnalogDelegate = nullptr;
    m_pComboxBinaryDelegate = nullptr;

    m_nCurrentModifyType = -1;

    // layout
    m_pLeftSplitter = nullptr;
    m_pLeftTopSplitter = nullptr;
    m_pRightSplitter = nullptr;
    m_pRightTopSplitter = nullptr;
    m_pMainSplitter = nullptr;

    // left
    m_pTreeView = nullptr;
    m_pFileBtn = nullptr;
    m_pAddBtn = nullptr;
    m_pDelBtn = nullptr;
    m_pTreeSaveBtn = nullptr;
    m_pTreeModel = nullptr;
    m_pItemRoot = nullptr;

    // right
    m_pAnalogModel = nullptr;
    m_pBinaryModel = nullptr;
    m_pAddPointButton = nullptr;
    m_pDelPointButton = nullptr;
    m_pSavePointButton = nullptr;
    m_pTabData = nullptr;
    m_pAnalogView = nullptr;
    m_pBinaryView = nullptr;
    m_nSelectDataType = -1;

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

void modeltool::Fini()
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

    if (nullptr != m_pAddBtn)
    {
        delete m_pAddBtn;
        m_pAddBtn = nullptr;
    }

    if (nullptr != m_pDelBtn)
    {
        delete m_pDelBtn;
        m_pDelBtn = nullptr;
    }

    if (nullptr != m_pTreeSaveBtn)
    {
        delete m_pTreeSaveBtn;
        m_pTreeSaveBtn = nullptr;
    }

    // ��ɾ��Item    
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

    // right    
    if (nullptr != m_pComboxAnalogDelegate)
    {
        delete m_pComboxAnalogDelegate;
        m_pComboxAnalogDelegate = nullptr;
    }

    if (nullptr != m_pComboxBinaryDelegate)
    {
        delete m_pComboxBinaryDelegate;
        m_pComboxBinaryDelegate = nullptr;
    }

    if (nullptr != m_pBinaryView)
    {
        delete m_pBinaryView;
        m_pBinaryView = nullptr;
    }

    if (nullptr != m_pAnalogView)
    {
        m_nSelectDataType = -1;
        delete m_pAnalogView;
        m_pAnalogView = nullptr;
    }

    if (nullptr != m_pAnalogModel)
    {
        delete m_pAnalogModel;
        m_pAnalogModel = nullptr;
    }

    if (nullptr != m_pBinaryModel)
    {
        delete m_pBinaryModel;
        m_pBinaryModel = nullptr;
    }

    if (nullptr != m_pAddPointButton)
    {
        delete m_pAddPointButton;
        m_pAddPointButton = nullptr;
    }

    if (nullptr != m_pDelPointButton)
    {
        delete m_pDelPointButton;
        m_pDelPointButton = nullptr;
    }

    if (nullptr != m_pSavePointButton)
    {
        delete m_pSavePointButton;
        m_pSavePointButton = nullptr;
    }

    if (nullptr != m_pTabData)
    {
        delete m_pTabData;
        m_pTabData = nullptr;
    }

    if (nullptr != m_pModelLib)
    {
        delete m_pModelLib;
        m_pModelLib = nullptr;
    }

    // layout
    if (nullptr != m_pLeftTopSplitter)
    {
        delete m_pLeftTopSplitter;
        m_pLeftTopSplitter = nullptr;
    }

    if (nullptr != m_pLeftSplitter)
    {
        delete m_pLeftSplitter;
        m_pLeftSplitter = nullptr;
    }

    if (nullptr != m_pRightTopSplitter)
    {
        delete m_pRightTopSplitter;
        m_pRightTopSplitter = nullptr;
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

void modeltool::CreateLeftSplitter()
{
    // �һ��¼������
    setContextMenuPolicy(Qt::CustomContextMenu);

    /********************************************* ���ϰ�ť *************************************************/
    //
    /*m_pFileBtn = new QPushButton(m_pLeftTopSplitter);
    m_pFileBtn->setFlat(true);
    m_pFileBtn->setIcon(QPixmap("resource/images/open.png"));
    connect(m_pFileBtn, SIGNAL(clicked()), this, SLOT(Slot_ShowFile()));

    // 
    m_pAddBtn = new QPushButton(m_pLeftTopSplitter);
    m_pAddBtn->setFlat(true);
    m_pAddBtn->setIcon(QPixmap("resource/images/add_model.png"));
    connect(m_pAddBtn, SIGNAL(clicked()), this, SLOT(Slot_AddItem()));

    //
    m_pDelBtn = new QPushButton(m_pLeftTopSplitter);
    m_pDelBtn->setFlat(true);
    m_pDelBtn->setIcon(QPixmap("resource/images/delete_model.png"));
    connect(m_pDelBtn, SIGNAL(clicked()), this, SLOT(Slot_DelItem()));*/
    m_pTreeSaveBtn = new QPushButton(m_pLeftTopSplitter);
    m_pTreeSaveBtn->setFlat(true);
    m_pTreeSaveBtn->setIcon(QPixmap("resource/images/m_save.png"));
    connect(m_pTreeSaveBtn, SIGNAL(clicked()), this, SLOT(Slot_TreeSave()));

    /********************************************* �������� *************************************************/
    //
    
    m_pTreeModel = new QStandardItemModel();
    m_pTreeView = new CTreeView(m_pTreeModel);
    m_pTreeView->setModel(m_pTreeModel);

    connect(m_pTreeView->header(), SIGNAL(sectionPressed(int)), this, SLOT(Slot_MousePressEvent(int)));
    
    // ģ��ͷ
    m_pTreeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("ģ������"));
    connect(m_pTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_Clicked(const QModelIndex &)));
    
    // ģ�ͳ���
    m_pItemRoot = new QStandardItem(QIcon(QStringLiteral("resource/images/model_tool.png")), QStringLiteral("����"));
    m_pItemRoot->setData(TYPE_ROOT, ROLE_ROOT); // ���������ڳ���
    m_pTreeModel->appendRow(m_pItemRoot);

    // չ����
    // SIGNAL(expanded(const QModelIndex&))
    connect(m_pTreeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(Slot_TreeTitleChanged(QStandardItem*)));

    // ����ԭʼ����
    LoadSourceData();

    // �����һ��¼�
    connect(m_pTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_RightClicked(const QPoint &)));

    m_pLeftSplitter->addWidget(m_pTreeView);

    CreateMenu();
}

void modeltool::LoadSourceData()
{
    if (m_pModelLib->m_mapScene.size() > 0)
    {
        int nSceneIndex = 0;
        for each (auto i in m_pModelLib->m_mapScene.keys())
        {
            m_pItemRoot = new QStandardItem(QIcon(QStringLiteral("resource/images/m_scene.png")), i);

            m_pItemRoot->setData(TYPE_SCENE, ROLE_ROOT); // ����������root
            m_pItemRoot->setData(TYPE_SCENE, ROLE_SCENE); //������ǳ���

            m_pTreeModel->item(0)->appendRow(m_pItemRoot);

            LoadEquip(i, nSceneIndex);

            ++nSceneIndex;
        }
    }
}

void modeltool::LoadEquip(QString& strSceneKey, int nSceneIndex/* = 0*/)
{
    Q_ASSERT(m_pModelLib);

    QStandardItem* pEquipStandItem = m_pTreeView->GetEquipmentItem(nSceneIndex);
    if (nullptr == pEquipStandItem)
    {
        return;
    }

    // ����
    if (m_pModelLib->m_mapScene[strSceneKey].mapEquipName.size() > 0)
    {
        int nIndex = 0;
        QMap<QString, QSet<QString> >& mapEquip = m_pModelLib->m_mapScene[strSceneKey].mapEquipName;

        // ������
        for each (auto i in mapEquip.keys())
        {
            QStandardItem* pItemEquip = new QStandardItem(QIcon(QStringLiteral("resource/images/m_group.png")), i);

            pItemEquip->setData(TYPE_EQUIPMENT, ROLE_ROOT);      // ����������root
            pItemEquip->setData(TYPE_EQUIPMENT, ROLE_SCENE);     // ��������ڳ���
            pItemEquip->setData(TYPE_EQUIPMENT, ROLE_EQUIPMENT); // ����������豸
            
            pEquipStandItem->appendRow(pItemEquip);

            QStandardItem* pModelStandItem = m_pTreeView->GetModelItem(nSceneIndex, nIndex);
            if (nullptr == pModelStandItem)
            {
                return;
            }

            QSet<QString>& setModel = mapEquip[i];
            for each (auto j in setModel)
            {
                QStandardItem* pItemModel = new QStandardItem(QIcon(QStringLiteral("resource/images/m_model.png")), j);

                pItemModel->setData(TYPE_MODEL_NO, ROLE_ROOT); // ����������root
                pItemModel->setData(TYPE_MODEL_NO, ROLE_SCENE); //��������ڳ���
                pItemModel->setData(TYPE_MODEL_NO, ROLE_EQUIPMENT); //����������豸��
                pItemModel->setData(TYPE_MODEL_NO, ROLE_MODEL_NO); //����������豸ģ��

                pModelStandItem->appendRow(pItemModel);
            }

            nIndex++;
        }

        // �����ͺ�
        for each (auto j in m_pModelLib->m_mapScene[strSceneKey].setModelName)
        {
            QStandardItem* pItemEquip = new QStandardItem(QIcon(QStringLiteral("resource/images/m_model.png")), j);

            pItemEquip->setData(TYPE_MODEL_NO, ROLE_ROOT);      // ����������root
            pItemEquip->setData(TYPE_MODEL_NO, ROLE_SCENE);     // ��������ڳ���
            pItemEquip->setData(TYPE_MODEL_NO, ROLE_MODEL_NO);  // ����������豸ģ��

            pEquipStandItem->appendRow(pItemEquip);
        }
    }
}

void modeltool::CreateRightSplitter()
{
    m_pAddPointButton = new QPushButton(m_pRightTopSplitter);
    m_pAddPointButton->setIcon(QPixmap("resource/images/add_model.png"));
    m_pAddPointButton->setFlat(true);
    m_pAddPointButton->setEnabled(false);
    connect(m_pAddPointButton, SIGNAL(clicked()), this, SLOT(Slot_AddPoint()));

    m_pDelPointButton = new QPushButton(m_pRightTopSplitter);
    m_pDelPointButton->setIcon(QPixmap("resource/images/delete_model.png"));
    m_pDelPointButton->setFlat(true);
    m_pDelPointButton->setEnabled(false);
    connect(m_pDelPointButton, SIGNAL(clicked()), this, SLOT(Slot_DelPoint()));

    m_pSavePointButton = new QPushButton(m_pRightTopSplitter);
    m_pSavePointButton->setIcon(QPixmap("resource/images/save.png"));
    m_pSavePointButton->setFlat(true);
    m_pSavePointButton->setEnabled(false);
    connect(m_pSavePointButton, SIGNAL(clicked()), this, SLOT(Slot_SavePoint()));

    m_pTabData = new QTabWidget;    

    m_pAnalogView = CreateView();
    m_pBinaryView = CreateView();

    m_pTabData->addTab(m_pAnalogView, QIcon("./resource/analog.png"), tr("Analog"));
    m_pTabData->addTab(m_pBinaryView, QIcon("./resource/analog.png"), tr("Binary"));

    

    m_pRightSplitter->addWidget(m_pTabData);
}

// �����һ��˵�
void modeltool::CreateMenu()
{
    CreateSceneMenu();
    
    CreateEquipmentMenu();

    CreateModelNoMenu();

    CreateProjectMenu();
}

void modeltool::CreateSceneMenu()
{
    m_pMenuScene = new QMenu(m_pTreeView);

    m_pCreateSceneAction = new QAction(QStringLiteral("��������"), this);
    connect(m_pCreateSceneAction, SIGNAL(triggered()), this, SLOT(Slot_CreateScene()));
    m_pMenuScene->addAction(m_pCreateSceneAction);

}

void modeltool::Slot_CreateScene()
{
    const QModelIndex& index = m_pTreeView->currentIndex();

    QVariant var = index.data(ROLE_ROOT);
    int row = index.row();

    if (TYPE_ROOT == var.toInt())
    {
        // ��ȡ��Ч�ļ���
        QString strName = m_pModelLib->CheckCreateSceneName(true);

        m_pItemRoot = new QStandardItem(QIcon(QStringLiteral("resource/images/m_scene.png")), strName);

        m_pItemRoot->setData(TYPE_SCENE, ROLE_ROOT); // ����������Ŀ¼
        m_pItemRoot->setData(TYPE_SCENE, ROLE_SCENE); //�������Ŀ¼
        m_pTreeModel->item(row)->appendRow(m_pItemRoot);
    }
}

void modeltool::CreateEquipmentMenu()
{
    m_pMenuEquipment = new QMenu(m_pTreeView);

    m_pCreateGroupAction = new QAction(QStringLiteral("������"), this);
    connect(m_pCreateGroupAction, SIGNAL(triggered()), this, SLOT(Slot_CreateGroup()));

    m_pMenuEquipment->addAction(m_pCreateGroupAction);

    m_pCreateEquipAction = new QAction(QStringLiteral("�����豸�ͺ�"), this);
    connect(m_pCreateEquipAction, SIGNAL(triggered()), this, SLOT(Slot_CreateEquipModel()));

    m_pMenuEquipment->addAction(m_pCreateEquipAction);

    m_pDeleteSceneAction = new QAction(QStringLiteral("ɾ��"), this);
    //connect(m_pDeleteSceneAction, SIGNAL(triggered()), this, SLOT(Slot_AddPoint2()));

    m_pMenuEquipment->addAction(m_pDeleteSceneAction);
}

void modeltool::Slot_CreateGroup()
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

        // ��ȡ��Ч�ļ���
        QString strName = m_pModelLib->CheckCreateGroupName(strScene, true);

        QStandardItem* pItem = new QStandardItem(QIcon(QStringLiteral("resource/images/m_group.png")), strName);

        pItem->setData(TYPE_EQUIPMENT, ROLE_ROOT); // ����������root
        pItem->setData(TYPE_EQUIPMENT, ROLE_SCENE); //��������ڳ���
        pItem->setData(TYPE_EQUIPMENT, ROLE_EQUIPMENT); //����������豸
        
        pStandItem->appendRow(pItem);
    }
}

// �����豸�ͺ�
void modeltool::Slot_CreateEquipModel()
{
    const QModelIndex& index = m_pTreeView->currentIndex();

    QVariant var = index.data(ROLE_ROOT);
    int row = index.row();

    if (TYPE_EQUIPMENT == var.toInt())
    {
        QStandardItem* pStandItem = m_pTreeView->GetModelItem(index.parent().row(), index.row());
        if (nullptr == pStandItem)
        {
            return;
        }

        QString strScene = index.parent().data().toString();
        QString strGroup = index.data().toString();

        // ��ȡ��Ч�ļ���
        QString strName = m_pModelLib->CheckCreateModelNoName(strScene, strGroup, true);

        QStandardItem* pItem = new QStandardItem(QIcon(QStringLiteral("resource/images/m_model.png")), strName);

        pItem->setData(TYPE_MODEL_NO, ROLE_ROOT); // ����������root
        pItem->setData(TYPE_MODEL_NO, ROLE_SCENE); //��������ڳ���
        pItem->setData(TYPE_MODEL_NO, ROLE_EQUIPMENT); //��������ڳ���
        pItem->setData(TYPE_MODEL_NO, ROLE_MODEL_NO); //����������豸ģ��

        pStandItem->appendRow(pItem);
    }
    else if (TYPE_SCENE == var.toInt()) // �������洴���豸�ͺ�
    {
        QStandardItem* pStandItem = m_pTreeView->GetEquipmentItem(index.row());
        if (nullptr == pStandItem)
        {
            return;
        }

        QString strScene = index.data().toString();

        // ��ȡ��Ч�ļ���
        QString strName = m_pModelLib->CheckCreateModelNoName(strScene, true);

        QStandardItem* pItem = new QStandardItem(QIcon(QStringLiteral("resource/images/m_model.png")), strName);

        pItem->setData(TYPE_MODEL_NO, ROLE_ROOT); // ����������root
        pItem->setData(TYPE_MODEL_NO, ROLE_SCENE); //��������ڳ���
        pItem->setData(TYPE_MODEL_NO, ROLE_MODEL_NO); //����������豸ģ��

        pStandItem->appendRow(pItem);
    }
}

void modeltool::CreateModelNoMenu()
{
    m_pMenuModelNo = new QMenu(m_pTreeView);

    if (nullptr != m_pCreateEquipAction)
    {
        m_pMenuModelNo->addAction(m_pCreateEquipAction);
    }

    m_pDeleteEquipmentAction = new QAction(QStringLiteral("ɾ��"), this);
    //connect(m_pDeleteEquipmentAction, SIGNAL(triggered()), this, SLOT(Slot_AddPoint2()));

    m_pMenuModelNo->addAction(m_pDeleteEquipmentAction);
}

void modeltool::CreateProjectMenu()
{
    m_pMenuProject = new QMenu(m_pTreeView);

    m_pProjectCopyAction = new QAction(QStringLiteral("����"), this);

    m_pMenuProject->addAction(m_pProjectCopyAction);

    m_pProjectImportAction = new QAction(QStringLiteral("����"), this);

    m_pMenuProject->addAction(m_pProjectImportAction);

    m_pProjectExportAction = new QAction(QStringLiteral("����"), this);

    m_pMenuProject->addAction(m_pProjectExportAction);

    m_pProjectDeleteAction = new QAction(QStringLiteral("ɾ��"), this);

    m_pMenuProject->addAction(m_pProjectDeleteAction);
}

QTableView* modeltool::CreateView()
{
    QTableView* pView = new QTableView;
    pView->setObjectName(QString::fromUtf8("analogview"));

    pView->setAlternatingRowColors(true);

    // ����ѡ��ɾ��ʱĬ������ɾ��.
    pView->setSelectionBehavior(QAbstractItemView::SelectRows);//����ѡ��

    pView->setStyleSheet("QTableView{background-color: rgb(250, 250, 250);"
        "alternate-background-color: rgb(141, 163, 215);}");

    pView->verticalHeader()->setVisible(false);
    //m_pAnalogView->horizontalHeader()->setStretchLastSection(true);  

    return pView;
}

/*********************************************************** RIGHT ********************************************************************/
void modeltool::ShowData(bool bAdd/* = false*/)
{
    Q_ASSERT(m_pModelLib);    

    /*if (DATATYPE_ANALOG == m_nSelectDataType)
    {
        if (nullptr != m_pAnalogView)
        {
            m_pAnalogView->setModel(m_pContent);
            //CComboxDelegate tem();
            CComboxDelegate* comboxDelegate = new CComboxDelegate(m_pTabData);
            m_pAnalogView->setItemDelegateForColumn(2, comboxDelegate);
        }
    }
    else if (DATATYPE_BINARY == m_nSelectDataType)
    {
        if (nullptr != m_pBinaryView)
        {
            m_pBinaryView->setModel(m_pContent);
        }
    }*/

    // ����Ͷͷ
    SetGridHead();

    // ��������
    SetLoadGridData(bAdd);

    m_pAnalogModel->RefrushModel();
    m_pBinaryModel->RefrushModel();
}

void modeltool::InitDataTypeComboData()
{
    m_vecAnalogCombo.clear();

    IDDESC tmp;
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

void modeltool::CreateData()
{
    int nIndex = m_pTabData->currentIndex();
    switch (nIndex)
    {
        case 0: // ң��
        {
            m_nSelectDataType = DATATYPE_ANALOG;

            break;
        }
        case 1: // ң��
        {
            m_nSelectDataType = DATATYPE_BINARY;

            break;
        }

        default:
        {
            return;
        }
    }

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

    m_pAnalogModel = new CAnalogModel(m_pCurrentEquipModel->m_arrAttrs[DATATYPE_ANALOG]);
    SetGridHead(DATATYPE_ANALOG);

    m_pBinaryModel = new CBinaryModel(m_pCurrentEquipModel->m_arrAttrs[DATATYPE_BINARY]);
    SetGridHead(DATATYPE_BINARY);

    m_pAnalogView->setModel(m_pAnalogModel);

    m_pBinaryView->setModel(m_pBinaryModel);

    if (nullptr == m_pComboxAnalogDelegate)
    {
        InitDataTypeComboData();

        m_pComboxAnalogDelegate = new CComboxDelegate(m_vecAnalogCombo, m_pAnalogModel);

        m_pAnalogView->setItemDelegateForColumn(2, m_pComboxAnalogDelegate);
    }

    if (nullptr == m_pComboxBinaryDelegate)
    {
        m_vecBinaryCombo.clear();
        IDDESC iddesc;
        iddesc.ID = "0";
        iddesc.desc = "bool";

        m_vecBinaryCombo.push_back(iddesc);

        m_pComboxBinaryDelegate = new CComboxDelegate(m_vecBinaryCombo, m_pBinaryModel);

        m_pBinaryView->setItemDelegateForColumn(2, m_pComboxBinaryDelegate);
    }
}

// ��������
void modeltool::SetLoadGridData(bool bAdd)
{
    int nSize = 0;
    if (DATATYPE_ANALOG == m_nSelectDataType)
    {
        nSize = LoadAnalog(bAdd);
    }
    else if (DATATYPE_BINARY == m_nSelectDataType)
    {
        nSize = LoadBinary(bAdd);
    }
}

// ����ң��
int modeltool::LoadAnalog(bool bAdd/* = false*/)
{
    Q_ASSERT(m_pModelLib);
    Q_ASSERT(m_pCurrentEquipModel);

    if (bAdd)
    {
        int nRow = 0;

        nRow = m_pCurrentEquipModel->GetAttrSize(DATATYPE_ANALOG);

        CContentInfo contInfo;
        contInfo.s_nDataType = 0;
        contInfo.s_bReadWrite = true;
        contInfo.s_strDescribe = QStringLiteral("����").toStdString();

        // ��������������
        m_pCurrentEquipModel->AddAttr(DATATYPE_ANALOG, nRow, contInfo);
        
        m_pAnalogModel->m_mapAnalogInfo[nRow].s_nDataType = contInfo.s_nDataType;
        m_pAnalogModel->m_mapAnalogInfo[nRow].s_bReadWrite = contInfo.s_bReadWrite;
        m_pAnalogModel->m_mapAnalogInfo[nRow].s_strDescribe = contInfo.s_strDescribe;
    }

    return 1;
}

// ����ң��
int modeltool::LoadBinary(bool bAdd/* = false*/)
{
    Q_ASSERT(m_pModelLib);
    Q_ASSERT(m_pCurrentEquipModel);

    if (bAdd)
    {
        int nRow = 0;

        nRow = m_pCurrentEquipModel->GetAttrSize(DATATYPE_BINARY);

        CContentInfo contInfo;
        contInfo.s_nDataType = 0;
        contInfo.s_bReadWrite = true;
        contInfo.s_strDescribe = QStringLiteral("����").toStdString();

        // ��������������
        m_pCurrentEquipModel->AddAttr(DATATYPE_BINARY, nRow, contInfo);
        
        m_pBinaryModel->m_mapBinaryInfo[nRow].s_nDataType = contInfo.s_nDataType;
        m_pBinaryModel->m_mapBinaryInfo[nRow].s_bReadWrite = contInfo.s_bReadWrite;
        m_pBinaryModel->m_mapBinaryInfo[nRow].s_strDescribe = contInfo.s_strDescribe;
    }

    return 1;
}

void modeltool::ReleaseData()
{
    /*if (DATATYPE_ANALOG == m_nSelectDataType)
    {
        if (nullptr != m_pAnalogView)
        {
            if (nullptr != m_pAnalogView->model())
            {
                m_pAnalogView->setModel(nullptr);
            }
        }
    }
    else if (DATATYPE_BINARY == m_nSelectDataType)
    {
        if (nullptr != m_pBinaryView)
        {
            if (nullptr != m_pBinaryView->model())
            {
                m_pBinaryView->setModel(nullptr);
            }            
        }
    }*/

    if (nullptr != m_pComboxAnalogDelegate)
    {
        delete m_pComboxAnalogDelegate;
        m_pComboxAnalogDelegate = nullptr;
    }

    if (nullptr != m_pComboxBinaryDelegate)
    {
        delete m_pComboxBinaryDelegate;
        m_pComboxBinaryDelegate = nullptr;
    }

    if (nullptr != m_pAnalogModel)
    {
        delete m_pAnalogModel;
        m_pAnalogModel = nullptr;
    }

    if (nullptr != m_pBinaryModel)
    {
        delete m_pBinaryModel;
        m_pBinaryModel = nullptr;
    }
}

void modeltool::SetGridHead(int nDataType/* = -1*/)
{
    QStringList headerList;
    switch (nDataType)
    {
    case DATATYPE_ANALOG:
    {
        headerList << ("NO")
                   << QStringLiteral("����")
                   << QStringLiteral("��������")
                   << QStringLiteral("��д����"); // Ĭ��ֻ��

        m_pAnalogModel->SetHeadTitles(headerList);

        break;
    }
    case DATATYPE_BINARY:
    {
        headerList << ("NO")
                   << QStringLiteral("����")
                   << QStringLiteral("��������")
                   << QStringLiteral("��д����"); // Ĭ��ֻ��

        m_pBinaryModel->SetHeadTitles(headerList);

        break;
    }
    default:
        break;
    }

    
}

/*********************************************************** LEFT SLOTS ***************************************************************/
void modeltool::Slot_DelItem()
{
    Q_ASSERT(m_pModelLib);

    // todo: ɾ����ʱ������Ŀ¼�����ļ���ѡ���ļ�ʱ����ɾ��Ŀ¼��
    QModelIndexList indexes = m_pTreeView->selectionModel()->selection().indexes();
    for each (QModelIndex index in indexes)
    {
        QVariant var = index.data(ROLE_SCENE);
        if (var.isValid())
        {
            if (MARK_FOLDER == var.toInt())
            { // ɾ��Ŀ¼
                int nFileCnt = m_pTreeModel->item(index.row())->rowCount();

                QString strFolderName = index.model()->data(index, Qt::DisplayRole).toString();
                if (m_pTreeModel->removeRow(index.row()))
                { // ����Ŀ¼ɾ���ɹ�

                    // ɾ��Ŀ¼
                    int res = m_pModelLib->DelFolderItem(strFolderName);
                    if (1 == res)
                    { // ����˵��Ŀ¼�������ļ�
                        emit Slot_SelectTabChanged();
                    }
                }
            }
            else if (MARK_ITEM == var.toInt())
            { // ɾ���ļ�
                //m_pAddPointButton->setEnabled(true);

                QString strFileName = index.model()->data(index, Qt::DisplayRole).toString();

                //int nP = index.parent().row();                
                //int nNO = index.row();
                
                // ɾ�������ļ�
                m_pTreeModel->item(index.parent().row())->removeRow(index.row());

                QString strFolderName = index.parent().data().toString();

                // ɾ��Ŀ¼���ļ��Ķ�Ӧ��ϵ
                m_pModelLib->DelFileItem(strFolderName, strFileName);

                emit Slot_SelectTabChanged();
            }
        }
    }
}

void modeltool::Slot_MousePressEvent(int index)
{
    QModelIndexList indexes = m_pTreeView->selectionModel()->selection().indexes();
    foreach(QModelIndex index, indexes)
    {
        m_pTreeView->selectionModel()->clear();
    }
}

void modeltool::Slot_ShowFile()
{
    QString str = QFileDialog::getOpenFileName(this, QStringLiteral("�豸����"), "/",
        "C++ files(modeltool.xml)");
}

void modeltool::Slot_TreeSave()
{
    if (nullptr != m_pModelLib)
    {
        m_pModelLib->SaveToFile("");
    }
}

void modeltool::Slot_Clicked(const QModelIndex &index)
{
    Q_ASSERT(m_pModelLib);

    QVariant varCurrent = index.data(ROLE_ROOT);
    QVariant varParent = index.parent().data(ROLE_ROOT);
    if (!varCurrent.isValid() || !varParent.isValid())
    {
        return;
    }

    int nCurrentType = varCurrent.toInt();
    switch (nCurrentType)
    {
        case TYPE_MODEL_NO:
        {
            if (TYPE_EQUIPMENT == varParent.toInt())
            {
                m_pAddPointButton->setEnabled(true);

                // �ظ����ͬһģ�Ͳ�����tabchanged�¼�
                // �����������ͺ�
                if (m_strCurrentModelName != index.data().toString() ||
                    m_strSceneName != index.parent().data().toString() ||
                    m_strCurrentGroupName != index.parent().parent().data().toString())
                {
                    m_nCurrentMode = varParent.toInt();

                    m_strCurrentModelName = index.data().toString();
                    m_strCurrentGroupName = index.parent().data().toString();
                    m_strSceneName = index.parent().parent().data().toString();

                    // ��ǰ�޸�����
                    m_nCurrentModifyType = M_MODIFY_GROUP_MODEL_NO;

                    ReleaseData();
                    CreateData();
                    connect(m_pTabData, SIGNAL(currentChanged(int)), this, SLOT(Slot_SelectTabChanged()));

                    emit Slot_ShowPoint();
                }
            }
            else if (TYPE_SCENE == varParent.toInt())
            {
                m_pAddPointButton->setEnabled(true);

                // �ظ����ͬһģ�Ͳ�����tabchanged�¼�
                // �������������ͺ�
                if (m_strCurrentModelName != index.data().toString() ||
                    m_strSceneName != index.parent().data().toString())
                {
                    m_nCurrentMode = varParent.toInt();

                    m_strCurrentModelName = index.data().toString();
                    m_strSceneName = index.parent().data().toString();
                    m_strCurrentGroupName = "";

                    // ��ǰ�޸�����
                    m_nCurrentModifyType = M_MODIFY_SCENE_MODEL_NO;

                    ReleaseData();
                    CreateData();
                    connect(m_pTabData, SIGNAL(currentChanged(int)), this, SLOT(Slot_SelectTabChanged()));

                    emit Slot_ShowPoint();
                }
            }
            break;
        }
        case TYPE_EQUIPMENT:
        {
            // ��ǰ�޸�����
            m_nCurrentModifyType = M_MODIFY_SCENE_GROUP;

            m_strSceneName = index.parent().data().toString();
            m_strCurrentGroupName = index.data().toString();
            m_strCurrentModelName = "";

            break;
        }
        case TYPE_SCENE:
        {
            // ��ǰ�޸�����
            m_nCurrentModifyType = M_MODIFY_SCENE;

            m_nCurrentMode = varParent.toInt();

            m_strSceneName = index.data().toString();
            m_strCurrentGroupName = "";
            m_strCurrentModelName = "";

            break;
        }
    default:
        break;
    }
}

void modeltool::Slot_TreeTitleChanged(QStandardItem* item)
{
    auto strNewValue = item->data(Qt::EditRole).toString();

    switch (m_nCurrentModifyType)
    {
        case M_MODIFY_SCENE:// �޸ĳ���
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
        case M_MODIFY_SCENE_GROUP:// �޸���
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
        case M_MODIFY_SCENE_MODEL_NO:// �޸ĳ���������ͺ�
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
        case M_MODIFY_GROUP_MODEL_NO:// �޸���������ͺ�
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

// �һ����νṹ�¼�
void modeltool::Slot_RightClicked(const QPoint& pos)
{
    //QModelIndex index = m_pTreeView->currentIndex();

    //int nType = index.data(ROLE_ROOT).toInt();

    QModelIndex index = m_pTreeView->indexAt(pos);

    QVariant var = index.data(ROLE_ROOT);

    if (var.isValid())
    {
        int nType = var.toInt();
        if (TYPE_ROOT == nType)
        {
            m_pMenuScene->exec(QCursor::pos());//�����Ҽ��˵����˵�λ��Ϊ���λ��
        }
        else if (TYPE_SCENE == var.toInt())
        {
            m_pMenuEquipment->exec(QCursor::pos());
        }
        else if (TYPE_EQUIPMENT == var.toInt())
        {
            m_pMenuModelNo->exec(QCursor::pos());
        }
        else if (TYPE_MODEL_NO == var.toInt())
        {
            m_pMenuProject->exec(QCursor::pos());
        }
        else
        {
            return;
        }
    }
}

// left
void modeltool::Slot_AddItem()
{
    //QFile file(M_MODEL_CONFIG_NAME_XML);

    /*Q_ASSERT(m_pModelLib);

    QModelIndexList indexes = m_pTreeView->selectionModel()->selection().indexes();

    // �ж�ѡ������豸������Ŀ¼.

    int nSize = indexes.size();

    // ѡ�г���һ����������
    if (nSize > 1)
    {
        return;
    }
    else if (1 == nSize) // ѡ��һ��Ŀ¼�����ļ�
    { // 
        const QModelIndex& index = indexes.at(0);        
        //indexes
        static int nCount = 1;

        QString strFileName = QStringLiteral("%1.h").arg(nCount);
        QStandardItem* item = new QStandardItem(QIcon(":/icon/icon/i.png"), strFileName);

        item->setData(MARK_ITEM, ROLE_SCENE); // ����������Ŀ¼
        item->setData(MARK_ITEM_FILE, ROLE_MARK_ITEM);// ��������ļ�

        item->setData(QColor(255, 0, 0, 255), Qt::BackgroundRole);        

        // ����ѡ��Ŀ¼�����ļ�
        QString strFolderName("");
        QVariant var = index.data(ROLE_SCENE);
        if (var.isValid())
        {
            if (MARK_FOLDER == var.toInt())
            {
                strFolderName = index.data().toString();
                m_pTreeModel->item(index.row())->appendRow(item);
            }
            else if(MARK_ITEM == var.toInt())
            {
                strFolderName  = index.parent().data().toString();
                m_pTreeModel->item(index.parent().row())->appendRow(item);
            }
            else
            {
                return;
            }
        }
        
        // ���Ŀ¼
        m_pModelLib->AddFileItem(strFolderName, strFileName);

        ++nCount;
    }
    else // Ŀ¼����
    {
        // ��ȡ��Ч�ļ���
        QString strName = m_pModelLib->FindFolderName();

        //QListWidgetItem* rightList = new QListWidgetItem(QIcon("./resource/data.png"), strName);

        m_pItemRoot = new QStandardItem(QIcon(QStringLiteral("open.png")), strName);

        m_pItemRoot->setData(MARK_FOLDER, ROLE_SCENE); // ����������Ŀ¼
        m_pItemRoot->setData(MARK_ITEM_FOLDER, ROLE_MARK_FOLDER); //�������Ŀ¼

        m_pTreeModel->appendRow(m_pItemRoot);
    }*/

}

/*********************************************************** RIGHT SLOTS ***************************************************************/
void modeltool::Slot_AddPoint()
{
    //ReleaseData();

    int nIndex = m_pTabData->currentIndex();
    switch (nIndex)
    {
        case 0: // ң��
        {
            m_nSelectDataType = DATATYPE_ANALOG;
            break;
        }
        case 1: // ң��
        {
            m_nSelectDataType = DATATYPE_BINARY;
            break;
        }

        default:
        {
            return;
        }
    }

    ShowData(true);

    m_pDelPointButton->setEnabled(true);
    m_pSavePointButton->setEnabled(true);
}

void modeltool::Slot_ShowPoint()
{
    int nIndex = m_pTabData->currentIndex();
    switch (nIndex)
    {
    case 0: // ң��
    {
        m_nSelectDataType = DATATYPE_ANALOG;
        break;
    }
    case 1: // ң��
    {
        m_nSelectDataType = DATATYPE_BINARY;
        break;
    }

    default:
    {
        return;
    }
    }

    ShowData();

    m_pDelPointButton->setEnabled(true);
    m_pSavePointButton->setEnabled(true);
}

void modeltool::Slot_DelPoint()
{
    Q_ASSERT(m_pAnalogModel);
    Q_ASSERT(m_pBinaryModel);

    if (DATATYPE_ANALOG == m_nSelectDataType)
    {
        if (nullptr != m_pAnalogView)
        {
            QModelIndexList indexes = m_pAnalogView->selectionModel()->selection().indexes();
            
            int nCnt = m_pAnalogView->selectionModel()->selection().count();
            if (nCnt == 0)
            {
                return;
            }

            int nSize = indexes.count(); // �� ���� ��  ������
           
            int nHeadCnt = 4;// nSize / nCnt;
            for (int i = 0; i < nSize;)
            {
                QModelIndex index = indexes.at(i);
                //QString str = index.data().toString();
                int row = index.row();

                m_pAnalogModel->removeRow(row);

                i += nHeadCnt;

                QMap<int, QMap<int, CContentInfo> >::iterator iterType = m_pCurrentEquipModel->m_arrAttrs.find(DATATYPE_ANALOG);
                QMap<int, CContentInfo>::iterator iterInfo = iterType->find(row);
                if (iterInfo != iterType->end())
                {
                    //iterFile->erase(iterInfo);
                    m_pCurrentEquipModel->m_arrAttrs[DATATYPE_ANALOG].erase(iterInfo);
                }
            }

            // ȫ�����ݺ���������ͬ��
            QMap<int, CContentInfo> mapInfo = m_pCurrentEquipModel->m_arrAttrs[DATATYPE_ANALOG];
            
            if (TYPE_SCENE == m_nCurrentMode)
            {
                M_COTENT_ATTRIBUTE& arrAttrs = m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipModel[m_strCurrentModelName].m_arrAttrs;
                arrAttrs[DATATYPE_ANALOG].clear();

                int j = 0;
                for each (auto i in mapInfo)
                {
                    arrAttrs[DATATYPE_ANALOG][j] = i;
                    j++;
                }
            }
            else if (TYPE_EQUIPMENT == m_nCurrentMode)
            {
                M_COTENT_ATTRIBUTE& arrAttrs = m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipGroup[m_strCurrentGroupName][m_strCurrentModelName].m_arrAttrs;
                arrAttrs[DATATYPE_ANALOG].clear();

                int j = 0;
                for each (auto i in mapInfo)
                {
                    arrAttrs[DATATYPE_ANALOG][j] = i;
                    j++;
                }
            }
            
            emit Slot_SelectTabChanged();
            //m_pContent->removeRow()
        }
    }
    else if (DATATYPE_BINARY == m_nSelectDataType)
    {
        if (nullptr != m_pBinaryView)
        {
            QModelIndexList indexes = m_pBinaryView->selectionModel()->selection().indexes();

            int nCnt = m_pBinaryView->selectionModel()->selection().count();
            int nSize = indexes.count(); // �� ���� ��  ������
            if (nCnt == 0)
            {
                return;
            }

            int nHeadCnt = nSize / nCnt;
            for (int i = 0; i < nSize;)
            {
                QModelIndex index = indexes.at(i);
                //QString str = index.data().toString();
                int row = index.row();

                m_pBinaryModel->removeRow(row);

                i += nHeadCnt;

                QMap<int, QMap<int, CContentInfo> >::iterator iterType = m_pCurrentEquipModel->m_arrAttrs.find(DATATYPE_BINARY);
                QMap<int, CContentInfo>::iterator iterInfo = iterType->find(row);
                if (iterInfo != iterType->end())
                {
                    //iterFile->erase(iterInfo);
                    m_pCurrentEquipModel->m_arrAttrs[DATATYPE_BINARY].erase(iterInfo);
                }
            }

            // ȫ�����ݺ���������ͬ��
            QMap<int, CContentInfo> mapInfo = m_pCurrentEquipModel->m_arrAttrs[DATATYPE_BINARY];

            if (TYPE_SCENE == m_nCurrentMode)
            {
                M_COTENT_ATTRIBUTE& arrAttrs = m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipModel[m_strCurrentModelName].m_arrAttrs;
                arrAttrs[DATATYPE_BINARY].clear();

                int j = 0;
                for each (auto i in mapInfo)
                {
                    arrAttrs[DATATYPE_BINARY][j] = i;
                    j++;
                }
            }
            else if (TYPE_EQUIPMENT == m_nCurrentMode)
            {
                M_COTENT_ATTRIBUTE& arrAttrs = m_pModelLib->m_arrModels[m_strSceneName].m_mapEquipGroup[m_strCurrentGroupName][m_strCurrentModelName].m_arrAttrs;
                arrAttrs[DATATYPE_BINARY].clear();

                int j = 0;
                for each (auto i in mapInfo)
                {
                    arrAttrs[DATATYPE_BINARY][j] = i;
                    j++;
                }
            }

            emit Slot_SelectTabChanged();
        }
    }
}

void modeltool::Slot_SavePoint()
{
    // todo:�����޸Ĳ��ܱ���

    // ���Ŀ¼�ṹ
    // todo:������ǰѡ���ļ�check����������ȫ��.
    /*if (!CheckModelFolder())
    {
        return;
    }*/

    QString strCurrentPath = "";
    if (TYPE_SCENE == m_nCurrentMode)
    { // ����ģʽ
        CEquipmentModel* pEquipModel = m_pModelLib->GetEquipModel(m_strSceneName, m_strCurrentModelName);

        strCurrentPath = M_SAVE_MODEL_FOLDER_NAME + "//" + m_strSceneName;

        pEquipModel->SaveToFile(strCurrentPath, m_strCurrentModelName);
    }
    else if (TYPE_EQUIPMENT == m_nCurrentMode)
    { // ��ģʽ
        CEquipmentModel* pEquipModel = m_pModelLib->GetEquipModel(m_strSceneName, m_strCurrentGroupName, m_strCurrentModelName);

        strCurrentPath = M_SAVE_MODEL_FOLDER_NAME + "//" + m_strSceneName + "//" + m_strCurrentGroupName;

        pEquipModel->SaveToFile(strCurrentPath, m_strCurrentModelName);
    }
}

bool modeltool::CheckModelFolder()
{
    QDir dir;
    // ģ��Ŀ¼����
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

    // ������������Ŀ¼
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

        // ��
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

            // �����ļ�
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

        // �ͺ�
        for each (auto j in m_pModelLib->m_mapScene[i].setModelName)
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

void modeltool::Slot_SelectTabChanged()
{
    if (nullptr == m_pTabData)
    {
        return;
    }

    int nIndex = m_pTabData->currentIndex();
    switch (nIndex)
    {
        case 0: // ң��
        {
            m_nSelectDataType = DATATYPE_ANALOG; 
            break;
        }
        case 1: // ң��
        {
            m_nSelectDataType = DATATYPE_BINARY;
            break;
        }
        default:
        {
            return;
        }
    }
    // ����Ͷͷ
    //SetGridHead();
    //ShowData();
    m_pAnalogModel->RefrushModel();
    m_pBinaryModel->RefrushModel();

    m_pDelPointButton->setEnabled(true);
    m_pSavePointButton->setEnabled(true);
}
