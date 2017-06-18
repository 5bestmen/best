#include "predictselectdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

CPredictSelectDialog::CPredictSelectDialog(QWidget* parent/* = 0 */)
	:QDialog(parent)
{
	label_1 = new QLabel("Project:");
	label_2 = new QLabel("Group:");
	label_3 = new QLabel("Item:");

	comboBox_1 = new QComboBox();
/*
	comboBox_1->addItem("proj1");
	comboBox_1->addItem("proj2");
	comboBox_1->addItem("proj3");*/

	comboBox_1->insertItem(0, QIcon(":/images/powerpredictitem.png"), "proj1");
	comboBox_1->insertItem(1, QIcon(":/images/device.png"), "proj2");
	comboBox_1->insertItem(2, QIcon(":/images/powerpredict.png"), "proj3");

	comboBox_2 = new QComboBox();
	comboBox_2->addItem("group1");
	comboBox_2->addItem("group2");
	comboBox_2->addItem("group3");

	comboBox_3 = new QComboBox();
	comboBox_3->addItem("item1");
	comboBox_3->addItem("item2");
	comboBox_3->addItem("item3");

	okButton = new QPushButton("OK");
	cancelButton = new QPushButton("Cancel");

	groupBox_1 = new QGroupBox();
	groupBox_2 = new QGroupBox();
	groupBox_3 = new QGroupBox();

	

	QHBoxLayout* layout_1 = new QHBoxLayout();
	layout_1->addWidget(okButton);
	layout_1->addWidget(cancelButton);

	QVBoxLayout* layout_2 = new QVBoxLayout();
	layout_2->addWidget(label_1);
	layout_2->addWidget(comboBox_1);
	groupBox_1->setLayout(layout_2);

	QVBoxLayout* layout_3 = new QVBoxLayout();
	layout_3->addWidget(label_2);
	layout_3->addWidget(comboBox_2);
	groupBox_2->setLayout(layout_3);

	QVBoxLayout* layout_4 = new QVBoxLayout();
	layout_4->addWidget(label_3);
	layout_4->addWidget(comboBox_3);
	groupBox_3->setLayout(layout_4);

	QVBoxLayout* layout_5 = new QVBoxLayout();
	layout_5->addWidget(groupBox_1);
	layout_5->addWidget(groupBox_2);
	layout_5->addWidget(groupBox_3);
	layout_5->addStretch();
	layout_5->addLayout(layout_1);
	layout_5->addStretch();

	this->setLayout(layout_5);

}


CPredictSelectDialog::~CPredictSelectDialog()
{


}