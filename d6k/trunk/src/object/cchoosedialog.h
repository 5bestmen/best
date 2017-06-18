#ifndef CCHOOSEDIALOG_H
#define CCHOOSEDIALOG_H

#include <QDialog>
#include "ui_cchoosedialog.h"

#include "../include/scadastudio/imainwindow.h"
#include "../src/equipmentmodel/modelinfo.h"

#include "treeobject.h"

class CChooseDialog : public QDialog
{
    Q_OBJECT

public:

    CChooseDialog(QMap<QString, SGroupAndModel>& mapScene, QMap<QString, CEquipmentGroupAndModel>& arrModels, QWidget *parent = 0);

    ~CChooseDialog();


private:
    Ui::CChooseDialog ui;

private slots:
    //void slot_on_ok();
    //void slot_on_cancel();

    void slot_AppComboBoxChanged(int);
    void slot_GroupComboBoxChanged(int);

    void slot_clickSure();
private:

    void InitChooseInfo();

public:
    CModelInfo* GetModelInfo()
    {
        if (m_strAppName.isEmpty() || m_strItemName.isEmpty())
        {
            return nullptr;
        }

        auto appIter = m_arrModels.find(m_strAppName);
        if (appIter == m_arrModels.end())
        {
            return nullptr;
        }

        if (m_strGroupName.isEmpty())
        {
            auto itemIter = appIter->m_mapEquipModel.find(m_strItemName);
            if (itemIter == appIter->m_mapEquipModel.end())
            {
                return nullptr;
            }

            return &(itemIter->m_arrAttrs);
        }
        else
        {
            auto groupIter = appIter->m_mapEquipGroup.find(m_strGroupName);
            if (groupIter == appIter->m_mapEquipGroup.end())
            {
                return nullptr;
            }

            auto itemIter = groupIter->find(m_strItemName);
            if (itemIter == groupIter->end())
            {
                return nullptr;
            }

            return &(itemIter->m_arrAttrs);
        }
    }

public:
    const QString& GetModelName() const
    {
        return m_strItemName;
    }

public:
    QString m_strAppName;         // 应用
    QString m_strGroupName;       // 组名
    QString m_strItemName;        // 模型名

    // key:场景名  value:目录和文件的对应关系(设备和型号对应关系)
    QMap<QString, SGroupAndModel>& m_mapScene;

    // key:场景
    QMap<QString, CEquipmentGroupAndModel>& m_arrModels;

    // 模型名
    QSet<QString> m_strModelName;
};

#endif // CCHOOSEDIALOG_H
