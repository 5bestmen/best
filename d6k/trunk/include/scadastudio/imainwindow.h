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

	//���ⲿģ��ӿ�
	//��������ָ��
	virtual CTreeWidget *GetLeftTree()=0;

	//����������view��modelָ��
	virtual QStandardItemModel *GetLeftTreeModel()=0;

	//���tab��ָ��
	virtual CTabWidget *GetTabWidget()=0;

	//��ö����ڵ�
	virtual CQuoteItem *GetTopItem()=0;
	virtual CQuoteItem *GetExtendServiceTopItem() = 0;
	virtual bool SetExtendServiceTopItem(CQuoteItem *pItem) = 0;
	//���������
	virtual CAttriwindow *GetAtrri ( )=0;

	//��model�ﴴ��������
	virtual QToolBar *CreateToolBarInModel(QString strTitle)=0;

	virtual QAction *CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon = "")=0;

	//����������map
	virtual bool PutToolBarIntoMap(QString strName, QToolBar *pToolBar)=0;
	//��������map���Ƴ�
	virtual bool RemoveToolBarFromMap(QString strName)=0;
	//���ҹ�����
	virtual bool FindToolBar(QString strName)=0;

	//����dock
	virtual CRightDock *GetRightDock() = 0;

	//Main menu bar
	virtual QMenuBar*GetMainMenuBar() = 0;

	//log
	virtual bool AddSysOutItem(MSG_LOG &msg, QColor color = QColor()) = 0;
private:
	
};

#endif // ICOREUI_H
