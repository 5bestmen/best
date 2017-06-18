/*! @file ctree.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  ctree.h
�ļ�ʵ�ֹ��� :  ������ؼ�����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ������ؼ�����
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
	//�˵�ʵ�ֺ���
	void ShowMenu(QModelIndex &index);
	void ShowRootMenu(QModelIndex &index);
	void ShowGroupMenu(QModelIndex &index);
	void ShowItemMenu(QModelIndex &index);
	//�ڲ�ʵ�ֺ���
	void AddGroup(QModelIndex &index);
	void DelGroup(QModelIndex &index);
	
public:
	void AddGroupChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon);
	//�ⲿʹ��
	void AddChildItem(CTableInfo info, CQuoteItem* item, int Role, QIcon Icon);
	//���ӱ����ļ�
	void AddReportFile(QModelIndex& index);
	//ɾ�������ļ�
	void DelReportFile(QModelIndex& index);
	//�޸��ļ�����
	void ModReportName(QModelIndex& index);
	//�޸��ļ�����
	void ModReportType(QModelIndex& index);
	//���뱨���ļ�
	void ImportReportFile(QModelIndex& index);
	//���ӿؼ������κ��޸�
	
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
	//���ؼ�
	CTreeWidget *m_pTreeWidget;
	//������
	IMainWindow *m_pUi;
	//�ں�
	IMainModuleInterface *m_pCore;
	//��TOP�ڵ�
	CQuoteItem* m_pTopItem;
};

