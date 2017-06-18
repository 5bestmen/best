#include "predictconfigwidget.h"

CConfigWidget::CConfigWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitSlots();
	
	InitTable();

	Load();
}

CConfigWidget::~CConfigWidget()
{

}

void CConfigWidget::InitSlots()
{
	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(onAdd()));
	connect(ui.actionRemove, SIGNAL(triggered()), this, SLOT(OnRemove()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(OnSave()));
	connect(ui.actionSetting, SIGNAL(triggered()), this, SLOT(OnSetting()));

}

void CConfigWidget::InitTable()
{

}

void CConfigWidget::Load()
{

}

void CConfigWidget::OnAdd()
{

}

void CConfigWidget::OnRemove() 
{

}

void CConfigWidget::OnSave()
{

}

void CConfigWidget::OnSetting()
{

}