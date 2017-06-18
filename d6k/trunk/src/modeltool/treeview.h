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
	//左边树形添加通道通道组
	//void SendFesType(int nType, QModelIndex &indexSelect, int nMmouseClickType);
private:
	//void CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType);

	
public slots :


public:
	QList<QStandardItem*> GetRoots();
	QStandardItemModel* GetTreeModel();

    // 获取场景Item
	QStandardItem* GetSceneItem();

    // 获取设备Item
    QStandardItem* GetEquipmentItem(int nRow = 0);
	
    // 获取型号Item
    QStandardItem* GetModelItem(int nParentRow = 0, int nCurrentRow = 0);

private:
    QStandardItemModel* m_pTreeModel;
	
};

#endif // _TREE_VIEW_H_
