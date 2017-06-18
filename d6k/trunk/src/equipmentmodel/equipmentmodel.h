#ifndef EQUIPMENTMODEL_H
#define EQUIPMENTMODEL_H

#include <QStandardItemModel>
#include <QPushButton>
#include <QMainWindow>
#include <QTableView>
#include <QSplitter>
#include <QResizeEvent>
#include <QMenu>
#include <QMutex>

#include "equipmentmodel.h"
#include "treeview.h"

#include "contentmodel.h"

#include "delegate.h"

#ifdef MODEL_TOOL_DLL
#define MOTEL_TOOL_EXPORT Q_DECL_EXPORT
#else
#define MOTEL_TOOL_EXPORT Q_DECL_IMPORT
#endif

class MOTEL_TOOL_EXPORT equipmentmodel : public QMainWindow
{
    Q_OBJECT

public:
    equipmentmodel();
    ~equipmentmodel();

private:

    // 模型工具窗口初始化
    void Init();

    // 回收资源
    void Fini();    

    // 加载原始数据
    void LoadSourceData();

    // 场景key
    void LoadEquip(QString& strSceneKey, int nSceneIndex = 0);

    // 创建工具条
    void CreateToolBar();

    // 创建左侧splitter
    void CreateLeftSplitter();

    // 创建右侧splitter
    void CreateRightSplitter();

    // 创建右击菜单
    void CreateMenu();

    void CreateSceneMenu();

    void CreateEquipmentMenu();

    void CreateModelNoMenu();

    void CreateProjectMenu();

public slots:
    void Slot_ShowFile();

    void Slot_TreeSave();

    void Slot_MousePressEvent(int index);

    void Slot_Clicked(const QModelIndex &index);

    // 修改tree名
    void Slot_TreeTitleChanged(QStandardItem*);

    // 右击树型结构
    void Slot_RightClicked(const QPoint& pos);

public slots:
    void Slot_AddPoint();
    void Slot_DelPoint();
    void Slot_SavePoint();

    void Slot_ShowPoint();

    // 创建场景
    void Slot_CreateScene();

    // 创建组
    void Slot_CreateGroup();
    // 创建设备型号
    void Slot_CreateEquipModel();
    // 删除树形结构
    void Slot_DeleteModel();

    // clear view
    void Slot_ClearView();

public: // 右侧

    bool CheckModelFolder();

    // 获取GUID
    const QString GetGuid();

    // 释放场景item内存
    void DeleteSceneTreeItem(int nSceneRow);

    // 释放组item内存
    void DeleteGroupTreeItem(int nSceneRow, int nGroupRow);

    // 释放场景下型号item内存
    void DeleteSceneModelTreeItem(int nSceneRow, int nModelRow);

    // 释放组下型号item内存
    void DeleteGroupModelTreeItem(int nSceneRow, int nGroupRow, int nModelRow);

private:

    // 当前模式 (场景 or 组)
    int m_nCurrentMode;

    // 当前场景名
    QString m_strSceneName;

    // 当前组名
    QString m_strCurrentGroupName;

    // 保存当前选中的文件名
    QString m_strCurrentModelName;

    CEquipmentModel* m_pCurrentEquipModel;

    // 模型库
    CModelLib* m_pModelLib;

private: // 修改tree item名
    enum
    {
        M_MODIFY_SCENE = 10,            // 修改场景
        M_MODIFY_SCENE_GROUP,           // 修改场景下面的组
        M_MODIFY_SCENE_MODEL_NO,        // 修改场景下面的型号
        M_MODIFY_GROUP_MODEL_NO,        // 修改组下面的型号
    };
    // 当前修改类型
    int m_nCurrentModifyType;
    // 当前old场景名
    QString m_strOldSceneName;
    // 当前old组名
    QString m_strCurrentOldGroupName;
    // 当前old型号名
    QString m_strCurrentOldModelName;

private: // 删除tree item
    enum
    {
        M_DELETE_SCENE = 20,            // 修改场景
        M_DELETE_SCENE_GROUP,           // 修改场景下面的组
        M_DELETE_SCENE_MODEL_NO,        // 修改场景下面的型号
        M_DELETE_GROUP_MODEL_NO,        // 修改组下面的型号
    };

    // 当前删除类型
    int m_nCurrentDeleteType;

private:

    // 是否需要
    bool m_bNeedSaveFlag;
    QMutex m_mutex;

    void SetSaveFlag(bool bFlag)
    {
        QMutexLocker locker(&m_mutex);

        m_bNeedSaveFlag = bFlag;
    }
private:

    // 树形结构
    CTreeView* m_pTreeView;

    // 内容
    CModelToolModel* m_pContentModel;

    QAction* m_pSaveAct;
    QAction* m_pAddAct;
    QAction* m_pDelAct;

    // 左侧Splitter
    QSplitter* m_pLeftSplitter;

    // 右侧Splitter
    QSplitter* m_pRightSplitter;

    // 整体Splitter
    QSplitter* m_pMainSplitter;


private:
    /************************************  LEFT  ***************************************************/
    QPushButton* m_pFileBtn;        // 打开file按钮
    //QPushButton* m_pTreeSaveBtn;    // 保存树按钮

    QStandardItemModel* m_pTreeModel;
    QStandardItem* m_pItemRoot;

    /************************************  RIGHT  ***************************************************/
    QTableView* m_pEquipView;

    // 数据类型combo代理
    CComboxDelegate* m_pComboxAnalogDelegate;
    QVector<ID_DESC> m_vecAnalogCombo;
    QVector<ID_DESC> m_vecBinaryCombo;
    void InitDataTypeComboData();

    QTableView* CreateView();

    // 点击文件显示数据
    void ShowData(bool bAdd = false);

    // 创建数据
    void CreateData();

    // 释放内存
    void ReleaseData();

    // 设置头
    void SetGridHead();

    // 加载数据
    void SetLoadGridData(bool bAdd = false);

    // 加载遥测
    int LoadAnalog(bool bAdd = false);

    // 加载遥信
    int LoadBinary(bool bAdd = false);

private:

    // Scene
    QAction* m_pCreateSceneAction;          // 创建场景 

    // Equipment
    QAction* m_pCreateGroupAction;          // 创建组
    QAction* m_pCreateEquipAction;          // 创建设备型号
    QAction* m_pDeleteSceneAction;          // 删除场景

    // ModelNo
    QAction* m_pCreateModelNoAction;        // 创建设备模型
    QAction* m_pDeleteEquipmentAction;      // 删除组或者设备

    // Project
    QAction* m_pProjectCopyAction;          // 复制
    QAction* m_pProjectImportAction;        // 导入
    QAction* m_pProjectExportAction;        // 导出
    QAction* m_pProjectDeleteAction;        // 删除设备

    // 右击菜单
    QMenu* m_pMenuScene;
    QMenu* m_pMenuEquipment;
    QMenu* m_pMenuModelNo;
    QMenu* m_pMenuProject;
};

extern "C" MOTEL_TOOL_EXPORT equipmentmodel* GetModelToolDll();

#endif // EQUIPMENTMODEL_H
