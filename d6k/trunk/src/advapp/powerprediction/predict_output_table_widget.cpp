#include "predict_output_table_widget.h"
#include "predict_module.h"

#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

COutputTableWidget::COutputTableWidget(CPredictData* outputItem)
	:m_pOutputItem(outputItem)
{
	QStringList headStringList;

	headStringList << "ID" << "NAME" << "Type" << "SelectPoint";

	this->setRowCount(2);
	this->setColumnCount(4);
	this->setHorizontalHeaderLabels(headStringList);

	QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_4Hour.m_nID));
	newItem->setTextAlignment(Qt::AlignCenter);
	newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 0, newItem);

	QTableWidgetItem *newItem_1 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_72Hour.m_nID));
	newItem_1->setTextAlignment(Qt::AlignCenter);
	newItem_1->setFlags(newItem_1->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 0, newItem_1);

	QTableWidgetItem *newItem_2 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_4Hour.m_szName));
	newItem_2->setTextAlignment(Qt::LeftToRight);
	newItem_2->setFlags(newItem_2->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 1, newItem_2);

	QTableWidgetItem *newItem_3 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_72Hour.m_szName));
	newItem_3->setTextAlignment(Qt::LeftToRight);
	newItem_3->setFlags(newItem_3->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 1, newItem_3);

	QTableWidgetItem *newItem_4 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_4Hour.m_nType));
	newItem_4->setTextAlignment(Qt::AlignCenter);
	newItem_4->setFlags(newItem_4->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 2, newItem_4);

	QTableWidgetItem *newItem_5 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_72Hour.m_nType));
	newItem_5->setTextAlignment(Qt::AlignCenter);
	newItem_5->setFlags(newItem_5->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 2, newItem_5);

	QTableWidgetItem *newItem_6 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_4Hour.m_szLinkedTagName));
	newItem_6->setTextAlignment(Qt::AlignCenter);
	newItem_6->setFlags(newItem_6->flags() & (~Qt::ItemIsEditable));
	this->setItem(0, 3, newItem_6);

	QTableWidgetItem *newItem_7 = new QTableWidgetItem(QString("%1").arg(m_pOutputItem->m_72Hour.m_szLinkedTagName));
	newItem_7->setTextAlignment(Qt::AlignCenter);
	newItem_7->setFlags(newItem_7->flags() & (~Qt::ItemIsEditable));
	this->setItem(1, 3, newItem_7);

	QPushButton *pushButton = new QPushButton("");
	pushButton->setFlat(true);
	this->setCellWidget(0, 3, pushButton);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	QPushButton *pushButton_1 = new QPushButton("");
	pushButton_1->setFlat(true);
	this->setCellWidget(1, 3, pushButton_1);
	connect(pushButton_1, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));

	setColumnWidth(0, 100);
	setColumnWidth(1, 200);
	setColumnWidth(2, 100);
	setColumnWidth(3, 300);
}



COutputTableWidget::~COutputTableWidget()
{


}

QString COutputTableWidget::GetBindValue()
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

void COutputTableWidget::Slot_SetBindValue()
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
			m_pOutputItem->m_vecTableInfo[i].m_szLinkedTagName = strBindValue;
		}
	}
}


CPredictData* COutputTableWidget::GetOutputItem() const
{
	Q_ASSERT(m_pOutputItem);
	if (m_pOutputItem)
	{
		return m_pOutputItem;
	}
	else
	{
		return Q_NULLPTR;
	}

}


void COutputTableWidget::Save()
{


}


void COutputTableWidget::Refresh()
{


}


void COutputTableWidget::SetModifyFlag(bool bFlag)
{
	Q_UNUSED(bFlag);

}

bool COutputTableWidget::GetModifyFlag()
{
	return true;
}


CPredictOutputWidget::CPredictOutputWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}


CPredictOutputWidget::~CPredictOutputWidget()
{


}



void CPredictOutputWidget::AddToTableCtrl(CPredictData* pOutputItem, QString staName)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		COutputTableWidget* pTableWdgt = dynamic_cast<COutputTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CPredictData* pItem = pTableWdgt->GetOutputItem();
			if (pItem && (pItem == pOutputItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	COutputTableWidget* pTableWdgt = new COutputTableWidget(pOutputItem);

	QString szName = staName + "_" + "Output Data";

	m_pTableWidget->AddTab(pTableWdgt, szName, 1, "");
}


void CPredictOutputWidget::ReleaseData()
{



}


void CPredictOutputWidget::SetTabViewHead()
{




}

