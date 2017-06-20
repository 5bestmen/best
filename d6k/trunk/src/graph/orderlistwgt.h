#pragma once

#include <QDialog>
#include "ui_orderlistwgt.h"

class COrderEditWgt;
class CDyncEventData;
class COrderListWgt : public QDialog
{
	Q_OBJECT

public:
	COrderListWgt(QWidget *parent = Q_NULLPTR);
	~COrderListWgt();
public slots:
    void Slot_AddNewOrder();

private:
	Ui::COrderListWgt ui;
	//ÃüÁî²Ù×÷
	COrderEditWgt *m_pOrderEditWgt;
	//ÃüÁî
	CDyncEventData* m_pEventIntent;

};
