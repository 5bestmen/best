#ifndef COREUI_H
#define COREUI_H

#include "imainwindow.h"

#include <QMainwindow>
#include <QMap>
//#include <functional>
//#include "coreui_global.h"

class CLeftDock;
class CRightDock;
class CSysOutDock;
class CTabWidget;
class QStandardItemModel;
class CTreeWidget;
class CQuoteItem;
class CAttriwindow;
class CFesModule;

class  CUIMgr : public IMainWindow
{
	Q_OBJECT
public:
	CUIMgr();
	~CUIMgr();
public:
	//给外部模块接口
	//获得左边树指针
	CTreeWidget *GetLeftTree();

	//获得左边树的view的model指针
	QStandardItemModel *GetLeftTreeModel();

	//获得tab的指针
	CTabWidget *GetTabWidget();

	//获得顶级节点
	CQuoteItem *GetTopItem();
	virtual CQuoteItem *GetExtendServiceTopItem();
	virtual bool SetExtendServiceTopItem(CQuoteItem *pItem);
	//获得属性栏
	CAttriwindow *GetAtrri();
	// 获取 主菜单
	QMenuBar*GetMainMenuBar();

	//在model里创建工具栏
	QToolBar *CreateToolBarInModel(QString strTitle);

	QAction *CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon = "");

	//工具栏加入map
	bool PutToolBarIntoMap(QString strName, QToolBar *pToolBar);
	//工具栏从map里移除
	bool RemoveToolBarFromMap(QString strName);
	//查找工具栏
	bool FindToolBar(QString strName);

	//创建各个模块
	void CreateModel();

	//添加属性栏
	//void AddAtrri(CAtrribute *pAttri);

	void closeEvent(QCloseEvent * event)
	{
		Q_UNUSED(event);
		
		emit DeleteCore();
		
		this->deleteLater();
	}

	virtual CRightDock *GetRightDock();

	virtual bool AddSysOutItem(MSG_LOG &msg, QColor color = QColor());

signals:
	void DeleteCore();

private:
	//左边树
	void CreateLeftTree();

	void CreateDock();

	//右边属性栏
	void CreateRightAttri();

	void CreateMenus();

	void CreateToolBar();

	//前置组态初始化
	void InitFesUi();
private:
	//左边树形dock
	CLeftDock *m_pLeftTreeDock;
	CRightDock *m_pRightDock;
	CSysOutDock *m_pSysOutDock;

	CTabWidget *m_pTabWidget;

	//树分支
	CTreeWidget *m_pTreeWidget;
	QStandardItemModel *m_pTreeModel;

	CQuoteItem *m_pTopProjectItem;
	CQuoteItem *m_pExtendServiceTopItem;
	CAttriwindow *m_pAttri;

	QMenu *m_pFileMenu;

	QMenu *m_pDatabaseMenu;

	QToolBar *m_pFileToolBar;

	QAction *m_pSaveAct;

	//工具栏map
	QMap<QString, QToolBar *> m_toolBarMap;

	//前置模块
	CFesModule *m_pFesModel;
};

#endif // COREUI_H
