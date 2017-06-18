#ifndef _TREE_VIEW_H_
#define _TREE_VIEW_H_

#include <QTreeView>
#include <QGridLayout>
#include <QPushButton>
#include <QStandardItemModel>

class CTreeView : public QTreeView
{
	Q_OBJECT

public:
    CTreeView(QStandardItemModel* pTreeModel);
	~CTreeView();

public:

	QList<QStandardItem*> GetRoots();

	QStandardItemModel* GetTreeModel();

    // 获取场景Item
	QStandardItem* GetSceneItem();

    // 获取组的父item（场景）
    QStandardItem* GetEquipmentItem(int nParent, int nRow = 0);
	
    // 获取型号的父item（场景或者组）
    QStandardItem* GetSceneModelItem(int nParentRow = 0, int nCurrentRow = 0);

    // 获取型号的父item（组）
    QStandardItem* GetGroupModelItem(int nSceneRow = 0, int nGroupRow = 0, int nModelRow = 0);

private:

    QStandardItemModel* m_pTreeModel;
};

#endif // _TREE_VIEW_H_
