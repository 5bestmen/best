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
    ITEM_APPLICATION,   //应用
    ITEM_SCENE,         //场景
    ITEM_GROUP,         //组
    ITEM_MODEL,          //模型
    ITEM_STATIC,         //静态
    ITEM_DYNAMIC,        //动态
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
    //初始化
    void InitWgt();
    //创建树
    void InitOriginalData();

    //创建static
    void CreateStaticModel(QTreeWidgetItem *pItem);
    //创建动态
    void CreateDynamicModel(QTreeWidgetItem *pItem);

    //组下增加 
    void AddGroupItems(std::vector<std::shared_ptr<CModelGroup>> groupModel, QTreeWidgetItem *pParentItem);
    //增加模型
    void AddModelItems(std::vector<std::shared_ptr<CObjectModelInfo>> tModel, QTreeWidgetItem *pParentItem);
    //增加子item
    void AddModelChildItems(QTreeWidgetItem *pParentItem);
    //分析item model数据
    //CObjectModelInfo AnalyseCurrentModelItem(QTreeWidgetItem *pModelItem);
    //创建header
    void CreateTableHeader(QStringList lstHeader);
    //创建table
    void CreateTableData(QStringList lstData);
    //创建动态属性table
    void CreateDynamicTable(const QString &strName, const QString &strDestr, bool ReadWrite, int DataType);
    //
    void DeleteTableItems();
    //更新模型数据
    void UpdaetModeItemData(QTreeWidgetItem *pModelItem);
    //更新静态数据
    void UpdateStaticItemData(QTreeWidgetItem *pStaticItem);
    //更新动态数据
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
    //增加场景
    void Slot_CreateScene();
    //增加组
    void Slot_CreateGroup();
    //创建模型
    void Slot_CreateModel();
    //删除
    void Slot_DeleteItem();
    //点击treeitem
    void Slot_TreeItemClicked(QTreeWidgetItem *item, int column);
    //
    void Slot_TreeItemDoubleClick(QTreeWidgetItem *item, int column);
    //增加item
    void Slot_AddStaticTableItem();
    //删除item
    void Slot_DeleteStaticTableItem();
    //增加动态item
    void Slot_AddDynamicTableItem();
    //删除动态item
    void Slot_DeleteDynamicTableItem();
    //cell 改变
    void Slot_CellChange(int row, int column);
    //
    void Slot_ItemDoubleClick(QTableWidgetItem *item);
    //checkbox
    void Slot_CheckBoxChange(int state);
    //combobox
    void Slot_ComboboxChange(int index);
    //
    void Slot_TreeItemDataChange(QTreeWidgetItem *pItem, int nColumn);
    //删除模型
    void Slot_DeleteModel();
    //保存
    void Slot_SaveProject();
private:
    Ui::CModelConfigWgt ui;
    //跟节点
    QTreeWidgetItem *m_pTopLevelItem;
    //数据模型
    CModelContainer *m_pDataModel;
    //
    CXmlDataAnalyse *m_pXmlAnalyse;
    //场景 数据
    QMap<QTreeWidgetItem*, std::shared_ptr<CModelScene>> m_mapScene;
    //组 数据
    QMap<QTreeWidgetItem*, std::shared_ptr<CModelGroup>> m_mapGroup;
    //模型数据
    QMap<QTreeWidgetItem*, std::shared_ptr<CObjectModelInfo>> m_mapModel;
    //数据类型映射
    QMap<int, QString> m_strTypeMap;
    //是否更新数据标志
    bool m_UpdateFlag;
    //双击时记录名称
    QString m_strDoubleTreeName;
    //
    QString m_strTableItemName;
};

extern "C" MOTEL_TOOL_EXPORT CModelConfigWgt* GetModelToolDll();


#endif // MODELCONFIGWGT_H
