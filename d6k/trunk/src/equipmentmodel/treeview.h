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

    // ��ȡ����Item
	QStandardItem* GetSceneItem();

    // ��ȡ��ĸ�item��������
    QStandardItem* GetEquipmentItem(int nParent, int nRow = 0);
	
    // ��ȡ�ͺŵĸ�item�����������飩
    QStandardItem* GetSceneModelItem(int nParentRow = 0, int nCurrentRow = 0);

    // ��ȡ�ͺŵĸ�item���飩
    QStandardItem* GetGroupModelItem(int nSceneRow = 0, int nGroupRow = 0, int nModelRow = 0);

private:

    QStandardItemModel* m_pTreeModel;
};

#endif // _TREE_VIEW_H_
