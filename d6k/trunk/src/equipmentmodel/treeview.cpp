
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

QStandardItem* CTreeView::GetEquipmentItem(int nParent, int nRow/* = 0*/)
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

    int i = nParent;

    int cnt = project->rowCount();
	for (; i < project->rowCount(); ++i)
	{
        
		QStandardItem* child = project->child(i);
        QVariant var = child->data(ROLE_SCENE);
        if (!var.isValid())
        {
            continue;
        }

        if (TYPE_SCENE == var.value<int>())
        {
            return child;
        }
	}

	return nullptr;
}

QStandardItem* CTreeView::GetSceneModelItem(int nParentRow/* = 0*/, int nCurrentRow/* = 0*/)
{
    QStandardItem* project = GetEquipmentItem(nParentRow);
    if (nullptr == project)
    {
        return nullptr;
    }

    int cnt = project->rowCount();
    for (int i = nCurrentRow; i < cnt; ++i)
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

// 获取型号的父item（组）
QStandardItem* CTreeView::GetGroupModelItem(int nSceneRow/* = 0*/, int nGroupRow/* = 0*/, int nModelRow/* = 0*/)
{
    QStandardItem* project = GetEquipmentItem(nSceneRow);
    if (nullptr == project)
    {
        return nullptr;
    }

    int cnt = project->rowCount();
    for (int i = nGroupRow; i < cnt; ++i)
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
