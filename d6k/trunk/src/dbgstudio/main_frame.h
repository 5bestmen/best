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
	//���ⲿģ��ӿ�
	//��������ָ��
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
	//��Ҫ���صĲ��
	void Signal_NeedLoadInfo(const QString &strDllName, const QString &strDeviceName);

private:
	void CreateTab();
	void CreateLeftTree();
	void CreateDock(); 
	void CreateMenus();
public slots:
    void Slot_ClickAboutThisTool();
	//�򿪲���б���Ϣ
	void Slot_ClickOpenDllWgt();
	//���ز��
	void Slot_LoadPlugin(const QString &strPluginName, const QString &strDeviceName);
	//�鿴ʵʱ����
	void Slot_ShowMsgMonitorWgt();

	void SlotViewLog();

private:
	// 
	CTabWidget *m_pTabWidget;

	//����֧
	CTreeWidget *m_pTreeWidget;
	QStandardItemModel *m_pTreeModel;

	QStandardItem *m_pTopTreeItem;

	QDockWidget * m_pLeftTreeDock;

//	CRightDock *m_pRightDock;
	COutputDock *m_pOutDock;
	//����װ�ý���
	CPluginLoadWgt *m_pPluginWgt;
	//���������
	//IMainModule *m_pModuleManager;
};

#endif // COREUI_H
