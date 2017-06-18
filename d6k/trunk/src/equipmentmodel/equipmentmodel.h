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

    // ģ�͹��ߴ��ڳ�ʼ��
    void Init();

    // ������Դ
    void Fini();    

    // ����ԭʼ����
    void LoadSourceData();

    // ����key
    void LoadEquip(QString& strSceneKey, int nSceneIndex = 0);

    // ����������
    void CreateToolBar();

    // �������splitter
    void CreateLeftSplitter();

    // �����Ҳ�splitter
    void CreateRightSplitter();

    // �����һ��˵�
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

    // �޸�tree��
    void Slot_TreeTitleChanged(QStandardItem*);

    // �һ����ͽṹ
    void Slot_RightClicked(const QPoint& pos);

public slots:
    void Slot_AddPoint();
    void Slot_DelPoint();
    void Slot_SavePoint();

    void Slot_ShowPoint();

    // ��������
    void Slot_CreateScene();

    // ������
    void Slot_CreateGroup();
    // �����豸�ͺ�
    void Slot_CreateEquipModel();
    // ɾ�����νṹ
    void Slot_DeleteModel();

    // clear view
    void Slot_ClearView();

public: // �Ҳ�

    bool CheckModelFolder();

    // ��ȡGUID
    const QString GetGuid();

    // �ͷų���item�ڴ�
    void DeleteSceneTreeItem(int nSceneRow);

    // �ͷ���item�ڴ�
    void DeleteGroupTreeItem(int nSceneRow, int nGroupRow);

    // �ͷų������ͺ�item�ڴ�
    void DeleteSceneModelTreeItem(int nSceneRow, int nModelRow);

    // �ͷ������ͺ�item�ڴ�
    void DeleteGroupModelTreeItem(int nSceneRow, int nGroupRow, int nModelRow);

private:

    // ��ǰģʽ (���� or ��)
    int m_nCurrentMode;

    // ��ǰ������
    QString m_strSceneName;

    // ��ǰ����
    QString m_strCurrentGroupName;

    // ���浱ǰѡ�е��ļ���
    QString m_strCurrentModelName;

    CEquipmentModel* m_pCurrentEquipModel;

    // ģ�Ϳ�
    CModelLib* m_pModelLib;

private: // �޸�tree item��
    enum
    {
        M_MODIFY_SCENE = 10,            // �޸ĳ���
        M_MODIFY_SCENE_GROUP,           // �޸ĳ����������
        M_MODIFY_SCENE_MODEL_NO,        // �޸ĳ���������ͺ�
        M_MODIFY_GROUP_MODEL_NO,        // �޸���������ͺ�
    };
    // ��ǰ�޸�����
    int m_nCurrentModifyType;
    // ��ǰold������
    QString m_strOldSceneName;
    // ��ǰold����
    QString m_strCurrentOldGroupName;
    // ��ǰold�ͺ���
    QString m_strCurrentOldModelName;

private: // ɾ��tree item
    enum
    {
        M_DELETE_SCENE = 20,            // �޸ĳ���
        M_DELETE_SCENE_GROUP,           // �޸ĳ����������
        M_DELETE_SCENE_MODEL_NO,        // �޸ĳ���������ͺ�
        M_DELETE_GROUP_MODEL_NO,        // �޸���������ͺ�
    };

    // ��ǰɾ������
    int m_nCurrentDeleteType;

private:

    // �Ƿ���Ҫ
    bool m_bNeedSaveFlag;
    QMutex m_mutex;

    void SetSaveFlag(bool bFlag)
    {
        QMutexLocker locker(&m_mutex);

        m_bNeedSaveFlag = bFlag;
    }
private:

    // ���νṹ
    CTreeView* m_pTreeView;

    // ����
    CModelToolModel* m_pContentModel;

    QAction* m_pSaveAct;
    QAction* m_pAddAct;
    QAction* m_pDelAct;

    // ���Splitter
    QSplitter* m_pLeftSplitter;

    // �Ҳ�Splitter
    QSplitter* m_pRightSplitter;

    // ����Splitter
    QSplitter* m_pMainSplitter;


private:
    /************************************  LEFT  ***************************************************/
    QPushButton* m_pFileBtn;        // ��file��ť
    //QPushButton* m_pTreeSaveBtn;    // ��������ť

    QStandardItemModel* m_pTreeModel;
    QStandardItem* m_pItemRoot;

    /************************************  RIGHT  ***************************************************/
    QTableView* m_pEquipView;

    // ��������combo����
    CComboxDelegate* m_pComboxAnalogDelegate;
    QVector<ID_DESC> m_vecAnalogCombo;
    QVector<ID_DESC> m_vecBinaryCombo;
    void InitDataTypeComboData();

    QTableView* CreateView();

    // ����ļ���ʾ����
    void ShowData(bool bAdd = false);

    // ��������
    void CreateData();

    // �ͷ��ڴ�
    void ReleaseData();

    // ����ͷ
    void SetGridHead();

    // ��������
    void SetLoadGridData(bool bAdd = false);

    // ����ң��
    int LoadAnalog(bool bAdd = false);

    // ����ң��
    int LoadBinary(bool bAdd = false);

private:

    // Scene
    QAction* m_pCreateSceneAction;          // �������� 

    // Equipment
    QAction* m_pCreateGroupAction;          // ������
    QAction* m_pCreateEquipAction;          // �����豸�ͺ�
    QAction* m_pDeleteSceneAction;          // ɾ������

    // ModelNo
    QAction* m_pCreateModelNoAction;        // �����豸ģ��
    QAction* m_pDeleteEquipmentAction;      // ɾ��������豸

    // Project
    QAction* m_pProjectCopyAction;          // ����
    QAction* m_pProjectImportAction;        // ����
    QAction* m_pProjectExportAction;        // ����
    QAction* m_pProjectDeleteAction;        // ɾ���豸

    // �һ��˵�
    QMenu* m_pMenuScene;
    QMenu* m_pMenuEquipment;
    QMenu* m_pMenuModelNo;
    QMenu* m_pMenuProject;
};

extern "C" MOTEL_TOOL_EXPORT equipmentmodel* GetModelToolDll();

#endif // EQUIPMENTMODEL_H
