#ifndef GRAPH_H
#define GRAPH_H

#include <memory>
#include <array>

#include <QObject>
#include <QActionGroup>

#include "graph_global.h"
#include "scadastudio/imodule.h"

class IMainWindow;

class CGraphView;
class CGraphScene;
class CGraphTabView;
class CGraphContainer;
class CGraphUi;
class CGraphFile;
class QStandardItem;
class QTableWidget;
class QDockWidget;

class CGraphView;
class CGraphScene;
class CGraphLayerDockWidget;
class QTranslator;
class CGraphToolsWgt;
//class QTableWidgetItem;
//class QXmlStreamReader;
//class QXmlStreamWriter;

class QDomDocument;

enum MODULE_ICON
{
	ICON_NULL ,
	ICON_EYE_ON = 1,
	ICON_EYE_OFF,
	ICON_LEFT,
	ICON_LAYER,

	ICON_END,
	ICON_MAX = ICON_END 
};

class CGraphApi;

QIcon GetModuleIcon(unsigned int nIconIdx);
CGraphApi *GetModuleApi();
IMainWindow *GetMainWindow();
IMainModuleInterface *GetIMainInterface();

class CGraphApi : public QObject,public IModule
{
	Q_OBJECT
public:
	CGraphApi();
	virtual ~CGraphApi();

	void Init(IMainModuleInterface *pUi);

	void UnInit();

	void ClearPropertyBrowser();

public:
	QTableWidget *GetGraphLayerList()
	{
		return m_pLayerList;
	}
	IMainWindow *GetMainWindow()
	{
		return m_pUi;
	}

	IMainModuleInterface *GetIMainInterface()
	{
		return m_pMainModuleInterface;
	}

	CGraphToolsWgt *GetToolWgt()
	{
		return m_pToolWgt;
	}

//	virtual bool SaveProject(QXmlStreamReader *pXml);
//	virtual bool LoadProject(QXmlStreamWriter *pXml);

	virtual bool SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);
	virtual bool LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot);

	//点tagname修改,通知绑定点的模块做出相应的修改
	virtual bool ChangeTagNameNodify(QString &tagName, QString &lastTagName);


	// 关闭工程文件
	virtual void CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot , bool bSaveFlag);
	// 新建一个空的工程文件
	virtual void CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem);

	virtual bool GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_UNUSED(pHash);
		Q_UNUSED(pStringPoolVec);
		Q_UNUSED(pDescStringPoolOccNo);
		return true; 
	};

	virtual bool ChangeTagNameNodify(QString &, QString &, int)
	{
		return true;
	}

	virtual bool GetModifyFlag()const
	{
		return true;
	}
	virtual bool Check()
	{
		return true;
	}
	void InitGraphRootTreeItem();
	bool LogMsg(const char *szLogTxt, int nLevel) ;
	//根据item判断路径
	QString GetRelativePath(CQuoteItem *pItem);
	//遍历group下文件
	void AnalyseGroupFile(CQuoteItem *pParent,QDomElement*pGroupNode, QString strPath);
	//删除所有的子item
	void DeleteAllChildItems(CQuoteItem *pParent);
	//遍历所有子item
	void RenameAllChildItems(QStandardItem *pParent);
	//获取graph目录
	const QString &GetGraphicPath() const
	{
		return m_strGraphcsPath;
	}

protected:
	void ShowRootMenu(QModelIndex &index);
	void ShowGraphChildMenu(QModelIndex &index);
	void ShowGraphGrpChildMenu(QModelIndex &index);

	void ShowGraphMenu();

	void ShowMenu(QModelIndex &index);
	void AddGraph(QModelIndex &index);
	void AddGraphGroup(QModelIndex &index);

	void DeleteGraph(QModelIndex &index);
	void DeleteGraphFolder(QModelIndex &index);

	void CreateMenu();
	
	void CreateEditMenu();
	void CreateViewMenu();
	void CreateOptionMenu();
	void CreateWindowMenu();
	void CreateHelpMenu();

	void CreateDockWidgets(IMainWindow *pMainWindow);

	//rename
	void RenameTreeItem();
	//
	bool CheckRenameLegailty(QStandardItem *pParent, QStandardItem *pCurrent, const QString strName);
	//加载模板数据
	bool LoadTemplateInfo();

private slots:
//	void showMouseRightButton(const QPoint &point);
//	void itemChanged(QStandardItem *pItem);
	void slot_doubleClicked(const QModelIndex &index);
	void slot_itemChanged(QStandardItem *item);
	void showMouseRightButton(const QPoint &point);
	void slot_currentChanged(int nIdx);

	void SelectDrawTool();
	void Simulation();

	// 编辑
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnCut();
	void OnWidgetsGroup();
	void OnWidgetsBreak();

	void OnAutoAlign();
	void OnLeftAlign();
	void OnRightAlign();
	void OnCenterHorzAlign();
	void OnTopAlign();
	void OnCenterVertAlign();
	void OnButtomAlign();
	//关闭tab
	void Slot_CloseTabWidget(QWidget*);
//	void slotItemClicked(QTableWidgetItem*);
	//
	void Slot_TreeItemChange(QStandardItem *);

private:
	IMainWindow *m_pUi;
	IMainModuleInterface * m_pMainModuleInterface;

	QTableWidget *m_pLayerList;
	CGraphLayerDockWidget *m_pDockWidget;
    //工具箱
    CGraphLayerDockWidget *m_pToolBoxDockWidget;

	int CallBackBeforeCloseTab(void *pData);
	void AddNewTabView(CGraphFile *pFile);
	void DeleteTabView(CGraphFile *pFile);

	enum 
	{
		USER_DATA_ROLE = Qt::UserRole+100,
		USER_PATH_ROLE = Qt::UserRole+101,    //对应的目录
	};
public:
	std::array<QIcon, ICON_MAX> m_arrIconList;
private:
	QMenu *m_pFileMenu;
	QMenu *m_pEditMenu;
	QMenu *m_pElementMenu;
	QMenu *m_pDebugMenu;
	QMenu *m_pArrangeMenu;

	QActionGroup *m_pDrawToolActGrp;

	QAction *m_pDrawToolSel;
	QAction *m_pDrawToolLine;
	QAction *m_pDrawToolRect;
	QAction *m_pDrawToolRound;
	QAction *m_pDrawToolRoundRect;

	std::shared_ptr<CGraphContainer> m_pGraphData; //! 图形文件

	std::shared_ptr<QTranslator> m_pTran;
	//图形根节点
	CQuoteItem *m_pGraphicsRoot;
	//graphics path
	QString m_strGraphcsPath;
	//tree 改名前name
	QString m_strOldName;
	//工具箱
	CGraphToolsWgt *m_pToolWgt;
};



#endif // GRAPH_H
