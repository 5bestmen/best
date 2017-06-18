#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "fixoperatewgt.h"

CFixOperateWgt::CFixOperateWgt(QWidget *parent)
	: QDialog(parent)
{
	InitSetWgt();
}

CFixOperateWgt::~CFixOperateWgt()
{
}

void CFixOperateWgt::InitSetWgt()
{
	QLabel *pLabelArea = new QLabel(this);
	pLabelArea->setText(tr("Fix Area Code"));

	m_pLineEdArea = new QLineEdit(this);

	QPushButton *pConform = new QPushButton(this);
	pConform->setText(tr("Conform"));

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(pLabelArea);
	layout->addWidget(m_pLineEdArea);
	layout->addWidget(pConform);
	this->adjustSize();
	connect(pConform,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
}

QString  CFixOperateWgt::GetFixData()
{
	return m_pLineEdArea->text();
}

void CFixOperateWgt::Slot_Conform()
{
	accept();
}
