#include "orderlistwgt.h"
#include "ordereditwgt.h"
#include "dync_event.h"

COrderListWgt::COrderListWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_AddNewOrder()));
	m_pOrderEditWgt = nullptr;

	m_pEventIntent = new CDyncEventData;
}

COrderListWgt::~COrderListWgt()
{
	if (m_pOrderEditWgt != nullptr)
	{
		m_pOrderEditWgt->deleteLater();
	}

	delete m_pEventIntent;
}

void COrderListWgt::Slot_AddNewOrder()
{
	if (m_pOrderEditWgt == nullptr)
	{
		m_pOrderEditWgt = new COrderEditWgt;
	}

	if (m_pOrderEditWgt->exec())
	{
		//МгдиЪ§Он
		
	}
	;
}
