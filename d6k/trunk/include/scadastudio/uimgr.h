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
	//���ⲿģ��ӿ�
	//��������ָ��
	CTreeWidget *GetLeftTree();

	//����������view��modelָ��
	QStandardItemModel *GetLeftTreeModel();

	//���tab��ָ��
	CTabWidget *GetTabWidget();

	//��ö����ڵ�
	CQuoteItem *GetTopItem();
	virtual CQuoteItem *GetExtendServiceTopItem();
	virtual bool SetExtendServiceTopItem(CQuoteItem *pItem);
	//���������
	CAttriwindow *GetAtrri();
	// ��ȡ ���˵�
	QMenuBar*GetMainMenuBar();

	//��model�ﴴ��������
	QToolBar *CreateToolBarInModel(QString strTitle);

	QAction *CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon = "");

	//����������map
	bool PutToolBarIntoMap(QString strName, QToolBar *pToolBar);
	//��������map���Ƴ�
	bool RemoveToolBarFromMap(QString strName);
	//���ҹ�����
	bool FindToolBar(QString strName);

	//��������ģ��
	void CreateModel();

	//���������
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
	//�����
	void CreateLeftTree();

	void CreateDock();

	//�ұ�������
	void CreateRightAttri();

	void CreateMenus();

	void CreateToolBar();

	//ǰ����̬��ʼ��
	void InitFesUi();
private:
	//�������dock
	CLeftDock *m_pLeftTreeDock;
	CRightDock *m_pRightDock;
	CSysOutDock *m_pSysOutDock;

	CTabWidget *m_pTabWidget;

	//����֧
	CTreeWidget *m_pTreeWidget;
	QStandardItemModel *m_pTreeModel;

	CQuoteItem *m_pTopProjectItem;
	CQuoteItem *m_pExtendServiceTopItem;
	CAttriwindow *m_pAttri;

	QMenu *m_pFileMenu;

	QMenu *m_pDatabaseMenu;

	QToolBar *m_pFileToolBar;

	QAction *m_pSaveAct;

	//������map
	QMap<QString, QToolBar *> m_toolBarMap;

	//ǰ��ģ��
	CFesModule *m_pFesModel;
};

#endif // COREUI_H
