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

class CHMIConfig : public QMainWindow
{
	Q_OBJECT

public:
	CHMIConfig(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CHMIConfig();

	//��������ָ��
	CTreeWidget *GetLeftTree();

	//����������view��modelָ��
	QStandardItemModel *GetLeftTreeModel();

	//���tab��ָ��
	CTabWidget *GetTabWidget();

private:
	void CreateMenus();
		
	void CreateToolBar();

	void Init();

	void CreateDock();

	//�����
	void CreateLeftTree();

	//��������ģ��
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
	
	//�������dock
	CLeftDock *m_pLeftTreeDock;
	CRightDock *m_pRightDock;
	CSysOutDock *m_pSysOutDock;

	CTabWidget *m_pTabWidget;

	//����֧
	CTreeWidget *m_pTreeWidget;
	QStandardItemModel *m_pTreeModel;

	CQuoteItem *m_pTopProjectItem;

	//ǰ��ģ��
	CFesModel *m_pFesModel;

	CEquipmentModel *m_pEquiModel;

	CGraphModel *m_pGraphModel;

	CReportModel *m_pReportModel;

	CUserModel *m_pUserModel;

	CSysModel *m_pSysModel;

	//�ڵ�
	CNodeParaModel *m_pNodeParaModel;

	//˳����ű�
	CSeqscriptModel *m_pSeqScriptModel;
};

#endif // HMICONFIG_H
