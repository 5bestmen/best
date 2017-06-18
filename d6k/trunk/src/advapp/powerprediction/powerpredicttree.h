#ifndef POWER_PREDICT_TREE_H
#define POWER_PREDICT_TREE_H

#include <QObject>
#include <QWidget>
#include <QModelIndex>
#include <QPoint>
#include <QString>
#include <QStandardItem>
#include <QMenu>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>


#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/treewidget.h"

#include "powerpredictconfig.h"
#include "powerpredictselectdialog.h"
#include "predictconfigwidget.h"
#include "predictdatainfo.h"
#include "predicttabwidget.h"
#include "predictaiview.h"
#include "predictparamview.h"


class CTreeWidget;
class IMainWindow;


class CPredictTree : public QWidget
{
	Q_OBJECT

public:
	CPredictTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore);

	CTreeWidget* GetTree()
	{
		return m_pTreeWidget;
	}

	//��ʼ����
	void InitTreeModel();

protected:

	//��ʾ���˵�
	void ShowRootMenu(QModelIndex &index);
	//��ʾ��˵�
	void ShowGroupMenu(QModelIndex &index);
	//��ʾ��˵�
	void ShowItemMenu(QModelIndex &index);
	//��ʾװ�ò˵�
	void ShowDeviceItemMenu(QModelIndex &index);

	//������
	void AddGroup(QModelIndex &index);
	//������
	void AddItem(QModelIndex &index);
	//ɾ����
	void DeleteGroup(QModelIndex &index);
	//ɾ����
	void DeleteItem(QModelIndex &index);
	//����װ��
	void AddDevice(QModelIndex &index);
	//ɾ��װ��
	void DeleteDevice(QModelIndex &index);

Q_SIGNALS:
	void Signl_AddGroup(QModelIndex &index);
	void Signl_AddItem(QModelIndex &indes);
	void Signl_DeleteGroup(QModelIndex &index);
	void Signl_DeleteItem(QModelIndex &index);
	void Signl_LoadItem(QModelIndex &index);
	void Signl_AddDevice(QModelIndex &index);
	void Signl_DeleteDevice(QModelIndex &index);
	void Signl_ItemChanged(QStandardItem *pItem);

protected slots:

	//�Ҽ���ʾ
	void Slot_ShowMouseRightButton(const QPoint& pos);
	//������ʾ
	void Slot_Clicked(const QModelIndex &index);
	//˫����ʾ
	void Slot_DoubleClicked(const QModelIndex &index);
	//�������
	void Slot_TreeItemChanged(QStandardItem *pItem);

private:

	//���ļ���������
	bool LoadDataFromFile(const QModelIndex &index);
	//��ȡ�ļ�
	bool ReadFile(const QString& strPathName, CPredictItem* pPdtItem);

private:
	IMainWindow *m_pUi;                 //������ָ��
	IMainModuleInterface *m_pCore;      //��ģ��ָ�� 
	CTreeWidget *m_pTreeWidget;         //���ؼ�
};
#endif //POWER_PREDICT_TREE_H