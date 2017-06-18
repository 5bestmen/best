#ifndef PREDICT_TREE_H
#define PREDICT_TREE_H

#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"

#include "predict_datainfo.h"

#include <QObject>
#include <QWidget>

class QStandardItem;
class QModelIndex;


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
	//��ʾ�������˵�
	void ShowInvterGrpMenu(QModelIndex &index);
	//��ʾ������˵�
	void ShowInvterMenu(QModelIndex &index);

	//������
	void AddGroup(QModelIndex &index);
	//ɾ����
	void DeleteGroup(QModelIndex &index);

	//������
	void AddItem(QModelIndex &index);
	//ɾ����
	void DeleteItem(QModelIndex &index);

	//���������
	void AddInverter(QModelIndex &index);
	//ɾ�������
	void DeleteInverter(QModelIndex &index);

Q_SIGNALS:
	void Signl_AddGroup(QModelIndex &index);
	void Signl_DeleteGroup(QModelIndex &index);

	void Signl_AddItem(QModelIndex &indes);
	void Signl_DeleteItem(QModelIndex &index);

	void Signl_LoadItem(QModelIndex &index);

	void Signl_AddInterver(QModelIndex &index);
	void Signl_DeleteInverter(QModelIndex &index);

	//void Signl_ItemChanged(QStandardItem *pItem);


protected slots:
	
	//�Ҽ���ʾ
	void Slot_ShowMouseRightButton(const QPoint& pos);
	//������ʾ
	void Slot_Clicked(const QModelIndex &index);
	//˫����ʾ
	void Slot_DoubleClicked(const QModelIndex &index);
	//�������
	//void Slot_TreeItemChanged(QStandardItem *pItem);


private:

	//���ļ���������
	bool LoadDataFromFile(const QModelIndex &index);
	//��ȡ�ļ�
	bool ReadFile(const QString& strPathName, CStationData* pPdtItem);


private:

	IMainWindow *m_pUi;                 //������ָ��
	IMainModuleInterface *m_pCore;      //��ģ��ָ�� 
	CTreeWidget *m_pTreeWidget;         //���ؼ�

};
#endif  //PREDICT_TREE_H
