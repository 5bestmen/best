#ifndef HMICONFIG_H
#define HMICONFIG_H

#include <QtWidgets\qmainwindow.h>

class CSplitter;
class CLeftDock;
class CRightDock;
class CSysOutDock;
class CTabWidget;
class QStandardItemModel;
class CTreeWidget;
class CQuoteItem;
class CFesModel;
class CEquipmentModel;
class CGraphModel;
class CReportModel;
class CUserModel;
class CSysModel;
class CNodeParaModel;
class CSeqscriptModel;
class CAtrribute;

class CScadastudio : public QMainWindow
{
	Q_OBJECT

public:
	CScadastudio(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CScadastudio();

	//获得左边树指针
	CTreeWidget *GetLeftTree();

	//获得左边树的view的model指针
	QStandardItemModel *GetLeftTreeModel();

	//获得tab的指针
	CTabWidget *GetTabWidget();

	//在model里创建工具栏
	QToolBar *CreateToolBarInModel(QString strTitle);

	QAction *CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon = "");

	CAtrribute *GetAtrribute();
private:
	void CreateMenus();
		
	void CreateToolBar();

	void Init();

	void CreateDock();

	//左边树
	void CreateLeftTree();

	//右边属性栏
	void CreateRightAttri();

	//创建各个模块
	void CreateModel();

	void CreateConnect();

private slots:
	void Save();

private:
	QMenu *m_pFileMenu;

	QMenu *m_pDatabaseMenu;

	QToolBar *m_pFileToolBar;

	QAction *m_pSaveAct;

	CSplitter *m_pSplitter;
	
	//左边树形dock
	CLeftDock *m_pLeftTreeDock;
	CRightDock *m_pRightDock;
	CSysOutDock *m_pSysOutDock;

	CTabWidget *m_pTabWidget;

	//树分支
	CTreeWidget *m_pTreeWidget;
	QStandardItemModel *m_pTreeModel;

	CQuoteItem *m_pTopProjectItem;

	//前置模块
	CFesModel *m_pFesModel;

	CEquipmentModel *m_pEquiModel;

	CGraphModel *m_pGraphModel;

	CReportModel *m_pReportModel;

	CUserModel *m_pUserModel;

	CSysModel *m_pSysModel;

	//节点
	CNodeParaModel *m_pNodeParaModel;

	//顺控与脚本
	CSeqscriptModel *m_pSeqScriptModel;

	CAtrribute *m_pAttri;
};

#endif // HMICONFIG_H
