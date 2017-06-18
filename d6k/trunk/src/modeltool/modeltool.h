#ifndef MODELTOOL_H
#define MODELTOOL_H

#include <QStandardItemModel>
#include <QPushButton>
#include <QMainWindow>
#include <QSplitter>
#include <QResizeEvent>
#include <QMenu>

#include "ui_modeltool.h"

#include "equipmentmodel.h"
#include "treeview.h"

#include "analogmodel.h"
#include "binarymodel.h"

#include "delegate.h"

class modeltool : public QMainWindow
{
	Q_OBJECT

public:
	modeltool();
	~modeltool();

private:
	Ui::modeltoolClass ui;

private:

    // 模型工具窗口初始化
    void Init();

    // 回收资源
    void Fini();

    void CreateLeftSplitter();

    // 加载原始数据
    void LoadSourceData();

    // 场景key
    void LoadEquip(QString& strSceneKey, int nSceneIndex = 0);

    void CreateRightSplitter();

    // 创建右击菜单
    void CreateMenu();

    void CreateSceneMenu();

    void CreateEquipmentMenu();

    void CreateModelNoMenu();

    void CreateProjectMenu();

public slots:
    void Slot_ShowFile();

    void Slot_AddItem();

    void Slot_DelItem();

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

    void Slot_SelectTabChanged();

    // 创建场景
    void Slot_CreateScene();

    // 创建组
    void Slot_CreateGroup();

    // 创建设备型号
    void Slot_CreateEquipModel();

public: // 右侧

    bool CheckModelFolder();

private:

    // 当前模式 (场景 or 组)
    int m_nCurrentMode;

    // 当前场景名
    QString m_strSceneName;

    // 当前组名
    QString m_strCurrentGroupName;

    // 保存当前选中的文件名
    QString m_strCurrentModelName;

    typedef QMap<int, QMap<int, CContentInfo> > M_COTENT_ATTRIBUTE;

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

private:

	// 树形结构
	CTreeView* m_pTreeView;

	// 内容
    CAnalogModel* m_pAnalogModel;
    CBinaryModel* m_pBinaryModel;

	// 左侧Splitter
    QSplitter* m_pLeftTopSplitter;

    // 左上Splitter
    QSplitter* m_pLeftSplitter;

	// 右侧Splitter
    QSplitter* m_pRightSplitter;

    // 右上Splitter
    QSplitter* m_pRightTopSplitter;

	// 整体Splitter
    QSplitter* m_pMainSplitter;


private:
    /************************************  LEFT  ***************************************************/
    QPushButton* m_pFileBtn;        // 打开file按钮
    QPushButton* m_pAddBtn;	        // 新增
    QPushButton* m_pDelBtn;         // 删除
    QPushButton* m_pTreeSaveBtn;    // 保存树按钮

    QStandardItemModel* m_pTreeModel;
    QStandardItem* m_pItemRoot;

    /************************************  RIGHT  ***************************************************/
    QPushButton* m_pAddPointButton;
    QPushButton* m_pDelPointButton;
    QPushButton* m_pSavePointButton;

    QTabWidget* m_pTabData;

    QTableView* m_pAnalogView;
    QTableView* m_pBinaryView;

    // 数据类型combo代理
    CComboxDelegate* m_pComboxAnalogDelegate;
    CComboxDelegate* m_pComboxBinaryDelegate;
    QVector<IDDESC> m_vecAnalogCombo;
    QVector<IDDESC> m_vecBinaryCombo;
    void InitDataTypeComboData();

    int m_nSelectDataType;

    QTableView* CreateView();

    // 点击文件显示数据
    void ShowData(bool bAdd = false);

    // 创建数据
    void CreateData();

    // 释放内存
    void ReleaseData();

    // 设置头
    void SetGridHead(int nDataType = -1);

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


#endif // MODELTOOL_H
