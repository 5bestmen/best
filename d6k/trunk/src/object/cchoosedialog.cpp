
#include "cchoosedialog.h"

#include <QMessageBox>

#include "objectdefine.h"
#include "../include/scadastudio/quoteItem.h"

CChooseDialog::CChooseDialog(QMap<QString, SGroupAndModel>& mapScene, QMap<QString, CEquipmentGroupAndModel>& arrModels, QWidget *parent)
    : m_mapScene(mapScene), m_arrModels(arrModels), QDialog(parent)
{
    ui.setupUi(this);

    setWindowIcon(QIcon(":/images/choose.ico"));
    setWindowTitle(QStringLiteral("模型选择"));
    setWindowModality(Qt::ApplicationModal);

    connect(ui.appComboBox, SIGNAL(activated(int) ()), this, SLOT(slot_AppComboBoxChanged(int)));
    connect(ui.groupComboBox, SIGNAL(activated(int)), this, SLOT(slot_GroupComboBoxChanged(int)));

    this->setFixedSize(this->width(), this->height());

    InitChooseInfo();
}

CChooseDialog::~CChooseDialog()
{

}

void CChooseDialog::InitChooseInfo()
{
    int nIndexApp = 0;
    for each (auto& i in m_arrModels.keys())
    {
        ui.appComboBox->insertItem(nIndexApp, i);

        ++nIndexApp;
    }

    nIndexApp = 0;
    QString strAppKey = ui.appComboBox->currentText();

    ui.groupComboBox->insertItem(nIndexApp, "");
    nIndexApp += 1;

    for each (auto& i in m_arrModels[strAppKey].m_mapEquipGroup.keys())
    {
        ui.groupComboBox->insertItem(nIndexApp, i);

        ++nIndexApp;
    }

    nIndexApp = 0;
    if (m_arrModels[strAppKey].m_mapEquipGroup.size() <= 0)
    {
        for each (auto& i in m_arrModels[strAppKey].m_mapEquipModel.keys())
        {
            ui.modelComboBox->insertItem(nIndexApp, i);

            ++nIndexApp;
        }
    }
    else
    {
        /*QString strGroupKey = ui.groupComboBox->currentText();
        for each (auto& i in m_arrModels[strAppKey].m_mapEquipGroup[strGroupKey].keys())
        {
            ui.modelComboBox->insertItem(nIndexApp, i);

            ++nIndexApp;
        }*/

        for each (auto& i in m_arrModels[strAppKey].m_mapEquipModel.keys())
        {
            ui.modelComboBox->insertItem(nIndexApp, i);

            ++nIndexApp;
        }
    }
    
}

void CChooseDialog::slot_AppComboBoxChanged(int)
{
    int nIndexApp = 0;
    QString strAppKey = ui.appComboBox->currentText();

    ui.modelComboBox->clear();

    if (m_arrModels[strAppKey].m_mapEquipGroup.size() <= 0)
    {
        for each (auto& i in m_arrModels[strAppKey].m_mapEquipModel.keys())
        {
            ui.modelComboBox->insertItem(nIndexApp, i);

            ++nIndexApp;
        }
    }
    else
    {
        QString strGroupKey = ui.groupComboBox->currentText();
        for each (auto& i in m_arrModels[strAppKey].m_mapEquipGroup[strGroupKey].keys())
        {
            ui.modelComboBox->insertItem(nIndexApp, i);

            ++nIndexApp;
        }
    }
}

void CChooseDialog::slot_GroupComboBoxChanged(int)
{
    int nIndex = 0;
    QString strAppKey = ui.appComboBox->currentText();
    QString strGroupKey = ui.groupComboBox->currentText();

    ui.modelComboBox->clear();

    if (strGroupKey.isEmpty())
    {
        for each (auto& i in m_arrModels[strAppKey].m_mapEquipModel.keys())
        {
            ui.modelComboBox->insertItem(nIndex, i);

            ++nIndex;
        }
    }
    else
    {
        for each (auto& i in m_arrModels[strAppKey].m_mapEquipGroup[strGroupKey].keys())
        {
            ui.modelComboBox->insertItem(nIndex, i);

            ++nIndex;
        }
    }
}

void CChooseDialog::slot_clickSure()
{
    QString strCrtAppName = ui.appComboBox->currentText();
    QString strCrtGroupName = ui.groupComboBox->currentText();
    QString strCrtModelName = ui.modelComboBox->currentText();
    if (strCrtModelName.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("模型名为空"), QStringLiteral("请重新选择模型！！！"));
        return;
    }

    m_strAppName = ui.appComboBox->currentText();
    m_strGroupName = ui.groupComboBox->currentText();
    m_strItemName = strCrtModelName;

    QDialog::accept();
    //close();
}

