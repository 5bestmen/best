#ifndef TREE_OBJECT_H
#define TREE_OBJECT_H

#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QStandardItem>

#include "../include/scadastudio/icore.h"
#include "../include/scadastudio/treewidget.h"
#include "../include/scadastudio/imainwindow.h"

#include "datainfo.h"

class CTreeWidget;
class IMainWindow;



class CObjectTree : public QWidget
{
    Q_OBJECT

public:
    CObjectTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore);

public:
    CTreeWidget* GetTree() const
    {
        return m_pTreeWidget;
    }

    void InitTreeModel();



protected:
    
    // �˵�ʵ��
    void ShowRootMenu(QModelIndex &index);
    void ShowGroupMenu(QModelIndex &index);
    void ShowItemMenu(QModelIndex &index);

    // ���ɾ������
    void AddGroup(QModelIndex &index);
    void DelGroup(QModelIndex &index);
    void AddItem(QModelIndex &index);
    void DelItem(QModelIndex &index);
    
Q_SIGNALS:
    void Sig_AddGroup(QModelIndex& index);
    void Sig_AddItem(QModelIndex& index);

    void Sig_DelGroup(QModelIndex& index);
    void Sig_DelItem(QModelIndex& index);

    void Sig_LoadItem(const CObjectItem* pObjectItem, const QModelIndex& index);

protected slots:
    void Slot_ShowMouseRightButton(const QPoint& pos);

    void Slot_Clicked(const QModelIndex &index);

    void Slot_DoubleClicked(const  QModelIndex &index);

    void Slot_TreeItemChanged(QStandardItem* pItem);

private:
    // ���ļ��м��ض���ģ��
    bool LoadDataFromFile(const QModelIndex& index);

    bool ReadFile(const QString& strPathName, CObjectItem* pObjItem);

private:
    //���ؼ�
    CTreeWidget *m_pTreeWidget;
    //������
    IMainWindow *m_pUi;
    //�ں�
    IMainModuleInterface *m_pCore;
    //��TOP�ڵ�
    //CQuoteItem* m_pTopItem;
};

#endif // TREE_OBJECT_H