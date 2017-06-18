#ifndef ICOREUI_H
#define ICOREUI_H

#include <QMainWindow>

class CLeftDock;
class CRightDock;
class CSysOutDock;
class CTabWidget;
class QStandardItemModel;
class CTreeWidget;
class CQuoteItem;
class CAttriwindow;
struct MSG_LOG;

class IMainWindow : public QMainWindow
{
public:
	//ICoreui(QWidget *parent);
	virtual ~IMainWindow();

	//给外部模块接口
	//获得左边树指针
	virtual CTreeWidget *GetLeftTree()=0;

	//获得左边树的view的model指针
	virtual QStandardItemModel *GetLeftTreeModel()=0;

	//获得tab的指针
	virtual CTabWidget *GetTabWidget()=0;

	//获得顶级节点
	virtual CQuoteItem *GetTopItem()=0;
	virtual CQuoteItem *GetExtendServiceTopItem() = 0;
	virtual bool SetExtendServiceTopItem(CQuoteItem *pItem) = 0;
	//获得属性栏
	virtual CAttriwindow *GetAtrri ( )=0;

	//在model里创建工具栏
	virtual QToolBar *CreateToolBarInModel(QString strTitle)=0;

	virtual QAction *CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon = "")=0;

	//工具栏加入map
	virtual bool PutToolBarIntoMap(QString strName, QToolBar *pToolBar)=0;
	//工具栏从map里移除
	virtual bool RemoveToolBarFromMap(QString strName)=0;
	//查找工具栏
	virtual bool FindToolBar(QString strName)=0;

	//属性dock
	virtual CRightDock *GetRightDock() = 0;

	//Main menu bar
	virtual QMenuBar*GetMainMenuBar() = 0;

	//log
	virtual bool AddSysOutItem(MSG_LOG &msg, QColor color = QColor()) = 0;
private:
	
};

#endif // ICOREUI_H
