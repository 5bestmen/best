#include "orderlistwgt.h"
#include "ordereditwgt.h"

COrderListWgt::COrderListWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_AddNewOrder()));
	m_pOrderEditWgt = nullptr;
}

COrderListWgt::~COrderListWgt()
{
	if (m_pOrderEditWgt != nullptr)
	{
		m_pOrderEditWgt->deleteLater();
	}
}

void COrderListWgt::Slot_AddNewOrder()
{
	if (m_pOrderEditWgt == nullptr)
	{
		m_pOrderEditWgt = new COrderEditWgt;
	}

	m_pOrderEditWgt->exec();
}
