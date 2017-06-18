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

    // ģ�͹��ߴ��ڳ�ʼ��
    void Init();

    // ������Դ
    void Fini();

    void CreateLeftSplitter();

    // ����ԭʼ����
    void LoadSourceData();

    // ����key
    void LoadEquip(QString& strSceneKey, int nSceneIndex = 0);

    void CreateRightSplitter();

    // �����һ��˵�
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

    // �޸�tree��
    void Slot_TreeTitleChanged(QStandardItem*);

    // �һ����ͽṹ
    void Slot_RightClicked(const QPoint& pos);

public slots:
    void Slot_AddPoint();
    void Slot_DelPoint();
    void Slot_SavePoint();

    void Slot_ShowPoint();

    void Slot_SelectTabChanged();

    // ��������
    void Slot_CreateScene();

    // ������
    void Slot_CreateGroup();

    // �����豸�ͺ�
    void Slot_CreateEquipModel();

public: // �Ҳ�

    bool CheckModelFolder();

private:

    // ��ǰģʽ (���� or ��)
    int m_nCurrentMode;

    // ��ǰ������
    QString m_strSceneName;

    // ��ǰ����
    QString m_strCurrentGroupName;

    // ���浱ǰѡ�е��ļ���
    QString m_strCurrentModelName;

    typedef QMap<int, QMap<int, CContentInfo> > M_COTENT_ATTRIBUTE;

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

private:

	// ���νṹ
	CTreeView* m_pTreeView;

	// ����
    CAnalogModel* m_pAnalogModel;
    CBinaryModel* m_pBinaryModel;

	// ���Splitter
    QSplitter* m_pLeftTopSplitter;

    // ����Splitter
    QSplitter* m_pLeftSplitter;

	// �Ҳ�Splitter
    QSplitter* m_pRightSplitter;

    // ����Splitter
    QSplitter* m_pRightTopSplitter;

	// ����Splitter
    QSplitter* m_pMainSplitter;


private:
    /************************************  LEFT  ***************************************************/
    QPushButton* m_pFileBtn;        // ��file��ť
    QPushButton* m_pAddBtn;	        // ����
    QPushButton* m_pDelBtn;         // ɾ��
    QPushButton* m_pTreeSaveBtn;    // ��������ť

    QStandardItemModel* m_pTreeModel;
    QStandardItem* m_pItemRoot;

    /************************************  RIGHT  ***************************************************/
    QPushButton* m_pAddPointButton;
    QPushButton* m_pDelPointButton;
    QPushButton* m_pSavePointButton;

    QTabWidget* m_pTabData;

    QTableView* m_pAnalogView;
    QTableView* m_pBinaryView;

    // ��������combo����
    CComboxDelegate* m_pComboxAnalogDelegate;
    CComboxDelegate* m_pComboxBinaryDelegate;
    QVector<IDDESC> m_vecAnalogCombo;
    QVector<IDDESC> m_vecBinaryCombo;
    void InitDataTypeComboData();

    int m_nSelectDataType;

    QTableView* CreateView();

    // ����ļ���ʾ����
    void ShowData(bool bAdd = false);

    // ��������
    void CreateData();

    // �ͷ��ڴ�
    void ReleaseData();

    // ����ͷ
    void SetGridHead(int nDataType = -1);

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


#endif // MODELTOOL_H
