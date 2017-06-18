#ifndef POWER_PREDICT_TREE_H
#define POWER_PREDICT_TREE_H

#include <QObject>
#include <QWidget>
#include <QModelIndex>
#include <QPoint>
#include <QString>
#include <QStandardItem>
#include <QMenu>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>


#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/treewidget.h"

#include "powerpredictconfig.h"
#include "powerpredictselectdialog.h"
#include "predictconfigwidget.h"
#include "predictdatainfo.h"
#include "predicttabwidget.h"
#include "predictaiview.h"
#include "predictparamview.h"


class CTreeWidget;
class IMainWindow;


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
	//显示装置菜单
	void ShowDeviceItemMenu(QModelIndex &index);

	//新增组
	void AddGroup(QModelIndex &index);
	//新增项
	void AddItem(QModelIndex &index);
	//删除组
	void DeleteGroup(QModelIndex &index);
	//删除项
	void DeleteItem(QModelIndex &index);
	//增加装置
	void AddDevice(QModelIndex &index);
	//删除装置
	void DeleteDevice(QModelIndex &index);

Q_SIGNALS:
	void Signl_AddGroup(QModelIndex &index);
	void Signl_AddItem(QModelIndex &indes);
	void Signl_DeleteGroup(QModelIndex &index);
	void Signl_DeleteItem(QModelIndex &index);
	void Signl_LoadItem(QModelIndex &index);
	void Signl_AddDevice(QModelIndex &index);
	void Signl_DeleteDevice(QModelIndex &index);
	void Signl_ItemChanged(QStandardItem *pItem);

protected slots:

	//右键显示
	void Slot_ShowMouseRightButton(const QPoint& pos);
	//单击显示
	void Slot_Clicked(const QModelIndex &index);
	//双击显示
	void Slot_DoubleClicked(const QModelIndex &index);
	//树项更改
	void Slot_TreeItemChanged(QStandardItem *pItem);

private:

	//从文件加载数据
	bool LoadDataFromFile(const QModelIndex &index);
	//读取文件
	bool ReadFile(const QString& strPathName, CPredictItem* pPdtItem);

private:
	IMainWindow *m_pUi;                 //主窗口指针
	IMainModuleInterface *m_pCore;      //主模块指针 
	CTreeWidget *m_pTreeWidget;         //树控件
};
#endif //POWER_PREDICT_TREE_H