
#include "treeview.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QList>

#include "modeldefine.h"


CTreeView::CTreeView(QStandardItemModel* pTreeModel) : m_pTreeModel(pTreeModel)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	//connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	//connect(this,SIGNAL(sendFesType(int)))
	setSortingEnabled(true);

	setDragEnabled(true);

	setEditTriggers(QAbstractItemView::SelectedClicked);

	/*m_pTreeModel = new QStandardItemModel(this);
	setModel(m_pTreeModel);

    connect(this->header(), SIGNAL(sectionPressed(int)), this, SLOT(Slot_MousePressEvent(int)));

	m_pTreeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("设备"));
    connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(clicked(const QModelIndex &)));
	/ *m_pItemRoot = new QStandardItem(QIcon(QStringLiteral("open.png")), QStringLiteral("1次设备"));
	m_pItemRoot->setData(MARK_PROJECT, ROLE_MARK_ITEM);//首先它是项目中目录
	m_pItemRoot->setData(MARK_FOLDER, ROLE_MARK_FOLDER);//其次它属于文件夹
	m_pTreeModel->appendRow(m_pItemRoot);* /

    connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_Clicked(const QModelIndex &)));

	m_pFileBtn = new QPushButton;
	m_pFileBtn->setFlat(true);
	m_pFileBtn->setIcon(QPixmap("open.png"));
    connect(m_pFileBtn, SIGNAL(clicked()), this, SLOT(Slot_ShowFile()));

	// 
	m_pAddBtn = new QPushButton;
	m_pAddBtn->setFlat(true);
	m_pAddBtn->setIcon(QPixmap("add_model.png"));
    connect(m_pAddBtn, SIGNAL(clicked()), this, SLOT(Slot_AddItem()));

	//
	m_pDelBtn = new QPushButton;
	m_pDelBtn->setFlat(true);
	m_pDelBtn->setIcon(QPixmap("delete_model.png"));
    connect(m_pDelBtn, SIGNAL(clicked()), this, SLOT(Slot_DelItem()));

    pLeftGridLayout->addWidget(m_pAddBtn, 0, 0);
    pLeftGridLayout->addWidget(m_pDelBtn, 0, 1);
    pLeftGridLayout->addWidget(m_pFileBtn, 0, 2);

    pLeftGridLayout->totalMaximumSize();
    pLeftGridLayout->addWidget(this, 1, 0, 1, 3);*/

}

CTreeView::~CTreeView()
{

}

QStandardItemModel* CTreeView::GetTreeModel()
{
	return m_pTreeModel;
}

QList<QStandardItem*> CTreeView::GetRoots()
{
	QList<QStandardItem*> roots;
	QStandardItemModel* model = GetTreeModel();
	for (int i = 0; i < model->rowCount(); ++i)
	{
		roots.append(model->item(i));
	}

	return roots;
}

QStandardItem* CTreeView::GetSceneItem()
{
	QList<QStandardItem*> roots = GetRoots();
	for (auto i = roots.begin(); i != roots.end(); ++i)
	{
        if ((*i)->data(ROLE_ROOT) == TYPE_ROOT)
		{
			return (*i);
		}
	}
	return nullptr;
}

QStandardItem* CTreeView::GetEquipmentItem(int nRow/* = 0*/)
{
    QStandardItem* project = GetSceneItem();
    if (nullptr == project)
    {
        return nullptr;
    }

    /*int i = 0;
    int nCurrRole = this->currentIndex().data(ROLE_ROOT).toInt();
    int nProRole = project->data(ROLE_ROOT).toInt();
    if (nCurrRole == nProRole)
    {
    i = 0;
    }
    else
    {
    i = this->currentIndex().row();
    }*/

    int i = nRow;

    int cnt = project->rowCount();
	for (; i < project->rowCount(); ++i)
	{
        
		QStandardItem* child = project->child(i);
        QVariant var = child->data(ROLE_SCENE);
        if (!var.isValid())
        {
            continue; // 说明不是ROLE_MARK_FOLDER，有可能是一些项目，对应项目结构树那个xxx.pro就是一个非文件夹条目
        }

        if (TYPE_SCENE == var.value<int>())
        {
            return child;
        }
	}

	return nullptr;
}

QStandardItem* CTreeView::GetModelItem(int nParentRow/* = 0*/, int nCurrentRow/* = 0*/)
{
    QStandardItem* project = GetEquipmentItem(nParentRow);
    if (nullptr == project)
    {
        return nullptr;
    }

    /*int i = 0;
    int nCurrRole = this->currentIndex().data(ROLE_ROOT).toInt();
    int nProRole = project->data(ROLE_ROOT).toInt();
    if (nCurrRole == nProRole)
    {
        i = 0;
    }
    else
    {
        i = this->currentIndex().parent().row();
    }*/

    //int i = this->currentIndex().row();
    for (int i = nCurrentRow; i < project->rowCount(); ++i)
    {
        QStandardItem* child = project->child(i);
        QVariant var = child->data(ROLE_EQUIPMENT);
        if (!var.isValid())
        {
            continue; // 说明不是ROLE_MARK_FOLDER，有可能是一些项目，对应项目结构树那个xxx.pro就是一个非文件夹条目
        }

        if (TYPE_EQUIPMENT == var.value<int>())
        {
            return child;
        }
    }

    return nullptr;
}
