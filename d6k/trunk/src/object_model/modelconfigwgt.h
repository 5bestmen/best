#ifndef MODELCONFIGWGT_H
#define MODELCONFIGWGT_H

#include <QWidget>
#include <vector>
#include <memory>
#include "ui_modelconfigwgt.h"
#include "model_container.h"

#ifdef MODEL_TOOL_DLL
#define MOTEL_TOOL_EXPORT Q_DECL_EXPORT
#else
#define MOTEL_TOOL_EXPORT Q_DECL_IMPORT
#endif


enum 
{
    ITEM_APPLICATION,   //Ӧ��
    ITEM_SCENE,         //����
    ITEM_GROUP,         //��
    ITEM_MODEL,          //ģ��
    ITEM_STATIC,         //��̬
    ITEM_DYNAMIC,        //��̬
};

#define  TREEXMLFILE "/modeltool.xml"

class QTreeWidgetItem;
class CModelContainer;
class CXmlDataAnalyse;
class MOTEL_TOOL_EXPORT CModelConfigWgt : public QWidget
{
    Q_OBJECT

public:
    CModelConfigWgt(QWidget *parent = 0);
    ~CModelConfigWgt();
    //��ʼ��
    void InitWgt();
    //������
    void InitOriginalData();

    //����static
    void CreateStaticModel(QTreeWidgetItem *pItem);
    //������̬
    void CreateDynamicModel(QTreeWidgetItem *pItem);

    //�������� 
    void AddGroupItems(std::vector<std::shared_ptr<CModelGroup>> groupModel, QTreeWidgetItem *pParentItem);
    //����ģ��
    void AddModelItems(std::vector<std::shared_ptr<CObjectModelInfo>> tModel, QTreeWidgetItem *pParentItem);
    //������item
    void AddModelChildItems(QTreeWidgetItem *pParentItem);
    //����item model����
    //CObjectModelInfo AnalyseCurrentModelItem(QTreeWidgetItem *pModelItem);
    //����header
    void CreateTableHeader(QStringList lstHeader);
    //����table
    void CreateTableData(QStringList lstData);
    //������̬����table
    void CreateDynamicTable(const QString &strName, const QString &strDestr, bool ReadWrite, int DataType);
    //
    void DeleteTableItems();
    //����ģ������
    void UpdaetModeItemData(QTreeWidgetItem *pModelItem);
    //���¾�̬����
    void UpdateStaticItemData(QTreeWidgetItem *pStaticItem);
    //���¶�̬����
    void UpdateDynamicItemData(QTreeWidgetItem *pDynamicItem);
    //
    void DeleteItemsData(QTreeWidgetItem *pItem);
    //
    bool CheckTreeRight(QTreeWidgetItem *pItem);
    //
    void UpdateTableData();

public slots:
    void Slot_TreeContextMenuRequest(const QPoint &tPoint);
    //table
    void Slot_TableContextMenuRequest(const QPoint &tPoint);
    //���ӳ���
    void Slot_CreateScene();
    //������
    void Slot_CreateGroup();
    //����ģ��
    void Slot_CreateModel();
    //ɾ��
    void Slot_DeleteItem();
    //���treeitem
    void Slot_TreeItemClicked(QTreeWidgetItem *item, int column);
    //
    void Slot_TreeItemDoubleClick(QTreeWidgetItem *item, int column);
    //����item
    void Slot_AddStaticTableItem();
    //ɾ��item
    void Slot_DeleteStaticTableItem();
    //���Ӷ�̬item
    void Slot_AddDynamicTableItem();
    //ɾ����̬item
    void Slot_DeleteDynamicTableItem();
    //cell �ı�
    void Slot_CellChange(int row, int column);
    //
    void Slot_ItemDoubleClick(QTableWidgetItem *item);
    //checkbox
    void Slot_CheckBoxChange(int state);
    //combobox
    void Slot_ComboboxChange(int index);
    //
    void Slot_TreeItemDataChange(QTreeWidgetItem *pItem, int nColumn);
    //ɾ��ģ��
    void Slot_DeleteModel();
    //����
    void Slot_SaveProject();
private:
    Ui::CModelConfigWgt ui;
    //���ڵ�
    QTreeWidgetItem *m_pTopLevelItem;
    //����ģ��
    CModelContainer *m_pDataModel;
    //
    CXmlDataAnalyse *m_pXmlAnalyse;
    //���� ����
    QMap<QTreeWidgetItem*, std::shared_ptr<CModelScene>> m_mapScene;
    //�� ����
    QMap<QTreeWidgetItem*, std::shared_ptr<CModelGroup>> m_mapGroup;
    //ģ������
    QMap<QTreeWidgetItem*, std::shared_ptr<CObjectModelInfo>> m_mapModel;
    //��������ӳ��
    QMap<int, QString> m_strTypeMap;
    //�Ƿ�������ݱ�־
    bool m_UpdateFlag;
    //˫��ʱ��¼����
    QString m_strDoubleTreeName;
    //
    QString m_strTableItemName;
};

extern "C" MOTEL_TOOL_EXPORT CModelConfigWgt* GetModelToolDll();


#endif // MODELCONFIGWGT_H
