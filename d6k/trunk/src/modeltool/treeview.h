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
	enum{ LEFTCLICK, LEFTDOUBLECLICK, RIGHTCLICK };

public:
    CTreeView(QStandardItemModel* pTreeModel);
	~CTreeView();

protected:
	//void mouseDoubleClickEvent(QMouseEvent *event);
	//void mousePressEvent(QMouseEvent *event);

signals:
	//����������ͨ��ͨ����
	//void SendFesType(int nType, QModelIndex &indexSelect, int nMmouseClickType);
private:
	//void CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType);

	
public slots :


public:
	QList<QStandardItem*> GetRoots();
	QStandardItemModel* GetTreeModel();

    // ��ȡ����Item
	QStandardItem* GetSceneItem();

    // ��ȡ�豸Item
    QStandardItem* GetEquipmentItem(int nRow = 0);
	
    // ��ȡ�ͺ�Item
    QStandardItem* GetModelItem(int nParentRow = 0, int nCurrentRow = 0);

private:
    QStandardItemModel* m_pTreeModel;
	
};

#endif // _TREE_VIEW_H_
