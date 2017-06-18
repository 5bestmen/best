#ifndef PREDICT_TREE_H
#define PREDICT_TREE_H

#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"

#include "predict_datainfo.h"

#include <QObject>
#include <QWidget>

class QStandardItem;
class QModelIndex;


class CPredictTree : public QWidget
{
	Q_OBJECT

public:

	CPredictTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore);

	CTreeWidget* GetTree()
	{
		return m_pTreeWidget;
	}

	//初始化树
	void InitTreeModel();

protected:

	//显示根菜单
	void ShowRootMenu(QModelIndex &index);
	//显示组菜单
	void ShowGroupMenu(QModelIndex &index);
	//显示项菜单
	void ShowItemMenu(QModelIndex &index);
	//显示逆变器组菜单
	void ShowInvterGrpMenu(QModelIndex &index);
	//显示逆变器菜单
	void ShowInvterMenu(QModelIndex &index);

	//新增组
	void AddGroup(QModelIndex &index);
	//删除组
	void DeleteGroup(QModelIndex &index);

	//新增项
	void AddItem(QModelIndex &index);
	//删除项
	void DeleteItem(QModelIndex &index);

	//新增逆变器
	void AddInverter(QModelIndex &index);
	//删除逆变器
	void DeleteInverter(QModelIndex &index);

Q_SIGNALS:
	void Signl_AddGroup(QModelIndex &index);
	void Signl_DeleteGroup(QModelIndex &index);

	void Signl_AddItem(QModelIndex &indes);
	void Signl_DeleteItem(QModelIndex &index);

	void Signl_LoadItem(QModelIndex &index);

	void Signl_AddInterver(QModelIndex &index);
	void Signl_DeleteInverter(QModelIndex &index);

	//void Signl_ItemChanged(QStandardItem *pItem);


protected slots:
	
	//右键显示
	void Slot_ShowMouseRightButton(const QPoint& pos);
	//单击显示
	void Slot_Clicked(const QModelIndex &index);
	//双击显示
	void Slot_DoubleClicked(const QModelIndex &index);
	//树项更改
	//void Slot_TreeItemChanged(QStandardItem *pItem);


private:

	//从文件加载数据
	bool LoadDataFromFile(const QModelIndex &index);
	//读取文件
	bool ReadFile(const QString& strPathName, CStationData* pPdtItem);


private:

	IMainWindow *m_pUi;                 //主窗口指针
	IMainModuleInterface *m_pCore;      //主模块指针 
	CTreeWidget *m_pTreeWidget;         //树控件

};
#endif  //PREDICT_TREE_H
