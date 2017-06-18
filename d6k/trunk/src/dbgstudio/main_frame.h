#ifndef COREUI_H
#define COREUI_H


#include "dbgstudio/imainwindow.h" 

#include <QMainWindow>
#include <QMap>
#include <functional>
 


class CTabWidget;
class CTreeWidget;
class QDockWidget;
class QStandardItemModel;
class QStandardItem;
class CPluginLoadWgt;
class CModuleMgr;
class IMainModule;
class COutputDock;

class CMainFrm : public IMainWindow
{
	Q_OBJECT
public:
	CMainFrm();
	~CMainFrm();
public:	
	//给外部模块接口
	//获得左边树指针
	CTreeWidget *GetLeftTree();
	CTabWidget  *GetTabWidget();
	QTableView * GetOutputTableView();
public:
	void closeEvent(QCloseEvent * event)
	{
		emit DeleteMainModule();
		this->deleteLater();
		Q_UNUSED(event);
	} 
signals:
	void DeleteMainModule();
	//需要加载的插件
	void Signal_NeedLoadInfo(const QString &strDllName, const QString &strDeviceName);

private:
	void CreateTab();
	void CreateLeftTree();
	void CreateDock(); 
	void CreateMenus();
public slots:
    void Slot_ClickAboutThisTool();
	//打开插件列表信息
	void Slot_ClickOpenDllWgt();
	//加载插件
	void Slot_LoadPlugin(const QString &strPluginName, const QString &strDeviceName);
	//查看实时报文
	void Slot_ShowMsgMonitorWgt();

	void SlotViewLog();

private:
	// 
	CTabWidget *m_pTabWidget;

	//树分支
	CTreeWidget *m_pTreeWidget;
	QStandardItemModel *m_pTreeModel;

	QStandardItem *m_pTopTreeItem;

	QDockWidget * m_pLeftTreeDock;

//	CRightDock *m_pRightDock;
	COutputDock *m_pOutDock;
	//加载装置界面
	CPluginLoadWgt *m_pPluginWgt;
	//插件管理类
	//IMainModule *m_pModuleManager;
};

#endif // COREUI_H
