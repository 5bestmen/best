#include "attrwgt.h"

CAttrWgt::CAttrWgt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.radioButton->setChecked(true);
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
}

CAttrWgt::~CAttrWgt()
{

}

void CAttrWgt::SetMastName()
{
    ui.comboBox->setCurrentIndex(0);
    ui.comboBox->setEnabled(false);
}

void CAttrWgt::SetFastName()
{
    ui.comboBox->setCurrentIndex(1);
    ui.comboBox->setEnabled(false);
}

void CAttrWgt::SetAuxName()
{
    ui.comboBox->setCurrentIndex(2);
    ui.comboBox->setEnabled(false);
}

void CAttrWgt::Slot_Conform()
{

}
