/*! @file ctree.h
<PRE>
********************************************************************************
模块名       :
文件名       :  ctree.h
文件实现功能 :  左侧树控件定义
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   左侧树控件定义
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#pragma once
#include <QWidget>
#include "report_global.h"
class CTreeWidget;
class CQuoteItem;
class QPoint;
class QModelIndex;
class IMainWindow;
class QIcon;
class QStandardItem;

#include "cwidget.h"
class CTree:public QWidget
{
	Q_OBJECT
public:
	CTree(CTreeWidget* parent,IMainWindow* pUi,IMainModuleInterface* pCore);
	~CTree();
public:
	void InitTreeModel();
protected:
	//菜单实现函数
	void ShowMenu(QModelIndex &index);
	void ShowRootMenu(QModelIndex &index);
	void ShowGroupMenu(QModelIndex &index);
	void ShowItemMenu(QModelIndex &index);
	//内部实现函数
	void AddGroup(QModelIndex &index);
	void DelGroup(QModelIndex &index);
	
public:
	void AddGroupChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon);
	//外部使用
	void AddChildItem(CTableInfo info, CQuoteItem* item, int Role, QIcon Icon);
	//增加报表文件
	void AddReportFile(QModelIndex& index);
	//删除报表文件
	void DelReportFile(QModelIndex& index);
	//修改文件名称
	void ModReportName(QModelIndex& index);
	//修改文件类型
	void ModReportType(QModelIndex& index);
	//导入报表文件
	void ImportReportFile(QModelIndex& index);
	//树子控件发生任何修改
	
Q_SIGNALS:
	void sig_IfReportClicked(bool bIsCicked);
	void sig_AddReportFile(QModelIndex& index);
	void sig_DelReportFile(QModelIndex& index);
	void sig_AddReportGroup(QModelIndex& index);
	void sig_ModReportName(QModelIndex& index);
	void sig_ModReportType(QModelIndex& index);
	void sig_OpenReportFile(QModelIndex& index);
	void sig_ImpReportFile(QModelIndex& index);
	void sig_ItemChanged(QStandardItem* item);
protected Q_SLOTS:
	void slot_on_RightMouseClicked(const QPoint&);
	void slot_on_DBClicked(const  QModelIndex &index);
	void slot_on_TreeItemChanged(QStandardItem* item);
public:
	CTreeWidget* GetTree()
	{ 
		return m_pTreeWidget; 
	}
private:
	//树控件
	CTreeWidget *m_pTreeWidget;
	//主窗口
	IMainWindow *m_pUi;
	//内核
	IMainModuleInterface *m_pCore;
	//树TOP节点
	CQuoteItem* m_pTopItem;
};

