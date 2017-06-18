#include "predict_inverter_table_widget.h"
#include "predict_module.h"

#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

CInverterTableWidget::CInverterTableWidget(CInverterInfo* inverterItem)
	:m_pInverterItem(inverterItem)
{
	QStringList headStringList;

	headStringList << "ID" << "NAME" << "Type" <<"SelectPoint";

	this->setRowCount(3);
	this->setColumnCount(4);
	this->setHorizontalHeaderLabels(headStringList);

	QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ActPower.m_nID));
	newItem->setTextAlignment(Qt::AlignCenter);
	newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 0, newItem);

	QTableWidgetItem *newItem_1 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ReactPower.m_nID));
	newItem_1->setTextAlignment(Qt::AlignCenter);
	newItem_1->setFlags(newItem_1->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 0, newItem_1);

	QTableWidgetItem *newItem_2 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_OpenState.m_nID));
	newItem_2->setTextAlignment(Qt::AlignCenter);
	newItem_2->setFlags(newItem_2->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 0, newItem_2);

	QTableWidgetItem *newItem_10 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ActPower.m_szName));
	newItem_10->setTextAlignment(Qt::LeftToRight);
	newItem_10->setFlags(newItem_10->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 1, newItem_10);

	QTableWidgetItem *newItem_11 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ReactPower.m_szName));
	newItem_11->setTextAlignment(Qt::LeftToRight);
	newItem_11->setFlags(newItem_11->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 1, newItem_11);

	QTableWidgetItem *newItem_12 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_OpenState.m_szName));
	newItem_12->setTextAlignment(Qt::LeftToRight);
	newItem_12->setFlags(newItem_12->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 1, newItem_12);

	QTableWidgetItem *newItem_20 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ActPower.m_nType));
	newItem_20->setTextAlignment(Qt::AlignCenter);
	newItem_20->setFlags(newItem_20->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 2, newItem_20);

	QTableWidgetItem *newItem_21 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ReactPower.m_nType));
	newItem_21->setTextAlignment(Qt::AlignCenter);
	newItem_21->setFlags(newItem_21->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 2, newItem_21);

	QTableWidgetItem *newItem_22 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_OpenState.m_nType));
	newItem_22->setTextAlignment(Qt::AlignCenter);
	newItem_22->setFlags(newItem_22->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 2, newItem_22);

	QTableWidgetItem *newItem_30 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ActPower.m_szLinkedTagName));
	newItem_30->setTextAlignment(Qt::AlignCenter);
	newItem_30->setFlags(newItem_30->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 3, newItem_30);

	QTableWidgetItem *newItem_31 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_ReactPower.m_szLinkedTagName));
	newItem_31->setTextAlignment(Qt::AlignCenter);
	newItem_31->setFlags(newItem_31->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 3, newItem_31);

	QTableWidgetItem *newItem_32 = new QTableWidgetItem(QString("%1").arg(m_pInverterItem->m_OpenState.m_szLinkedTagName));
	newItem_32->setTextAlignment(Qt::AlignCenter);
	newItem_32->setFlags(newItem_32->flags() & (~Qt::ItemIsEditable));
	this->setItem(2, 3, newItem_32);

	QPushButton *pushButton = new QPushButton("");
	pushButton->setFlat(true);
	this->setCellWidget(0, 3, pushButton);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_1 = new QPushButton("");
	pushButton_1->setFlat(true);
	this->setCellWidget(1, 3, pushButton_1);
	connect(pushButton_1, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_2 = new QPushButton("");
	pushButton_2->setFlat(true);
	this->setCellWidget(2, 3, pushButton_2);
	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	setColumnWidth(0, 100);
	setColumnWidth(1, 200);
	setColumnWidth(2, 100);
	setColumnWidth(3, 300);

}



CInverterTableWidget::~CInverterTableWidget()
{


}

QString CInverterTableWidget::GetBindValue()
{
	std::string strNodeTagname;
	std::string strTagname;
	std::string strFilename;
	//
	std::vector<int> m_arrDataType;
	m_arrDataType.push_back(DT_NULL);
	m_arrDataType.push_back(DT_BOOLEAN);
	m_arrDataType.push_back(DT_CHAR);
	m_arrDataType.push_back(DT_BYTE);
	m_arrDataType.push_back(DT_SHORT);
	m_arrDataType.push_back(DT_WORD);
	m_arrDataType.push_back(DT_INT);
	m_arrDataType.push_back(DT_DWORD);
	m_arrDataType.push_back(DT_LLONG);
	m_arrDataType.push_back(DT_LWORD);
	m_arrDataType.push_back(DT_FLOAT);
	m_arrDataType.push_back(DT_DOUBLE);
	m_arrDataType.push_back(DT_STRING);
	m_arrDataType.push_back(DT_BLOCK);
	m_arrDataType.push_back(DT_MAX);
	//
	bool bSelectFlag = GetPreIMainInterface()->SelectTag(m_arrDataType, true, strNodeTagname, strTagname, strFilename);

	if (bSelectFlag)
	{
		QString strResult = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());
		return strResult;
	}

	return "";
}

void CInverterTableWidget::Slot_SetBindValue()
{
	QString strBindValue = GetBindValue();
	if (strBindValue.isEmpty())
	{
		return;
	}

	QPushButton *pushBtn = qobject_cast<QPushButton*>(sender());

	Q_ASSERT(pushBtn);
	if (pushBtn == nullptr)
	{
		return;
	}

	pushBtn->setText(strBindValue);

	for (int i = 0; i < rowCount(); i++)
	{
		if (this->cellWidget(i, 3) == pushBtn)
		{
			m_pInverterItem->m_vecTableInfo[i].m_szLinkedTagName = strBindValue;
		}
	}
}

CInverterInfo* CInverterTableWidget::GetInverterItem() const
{
	Q_ASSERT(m_pInverterItem);
	if (m_pInverterItem)
	{
		return m_pInverterItem;
	}
	else
	{
		return Q_NULLPTR;
	}

}


void CInverterTableWidget::Save()
{


}


void CInverterTableWidget::Refresh()
{


}


void CInverterTableWidget::SetModifyFlag(bool bFlag)
{
	Q_UNUSED(bFlag);

}

bool CInverterTableWidget::GetModifyFlag()
{
	return true;
}


CPredictInverterWidget::CPredictInverterWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CPredictInverterWidget::~CPredictInverterWidget()
{


}



void CPredictInverterWidget::AddToTableCtrl(CInverterInfo* pInverterItem, QString strName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CInverterTableWidget* pTableWdgt = dynamic_cast<CInverterTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CInverterInfo* pItem = pTableWdgt->GetInverterItem();
			if (pItem && (pItem == pInverterItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CInverterTableWidget* pTableWdgt = new CInverterTableWidget(pInverterItem);

	QString szName = strName + "_" + pInverterItem->m_szName;

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CPredictInverterWidget::ReleaseData()
{



}


void CPredictInverterWidget::SetTabViewHead()
{




}

