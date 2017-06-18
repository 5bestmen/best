#include "predict_inverter_group_table_widget.h"
#include "predict_module.h"

#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

CInverterGrpTableWidget::CInverterGrpTableWidget(CInverterGroup* inverterGrpItem)
	:m_pInverterGrpItem(inverterGrpItem)
{
	QStringList headStringList;

	headStringList << "ID" << "NAME" << "SelectPoint";

	this->setRowCount(2);
	this->setColumnCount(3);
	this->setHorizontalHeaderLabels(headStringList);

	QTableWidgetItem *newItem = new QTableWidgetItem(tr("1"));
	newItem->setTextAlignment(Qt::AlignCenter);
	this->setItem(0, 0, newItem);

	QTableWidgetItem *newItem_1 = new QTableWidgetItem(tr("2"));
	newItem_1->setTextAlignment(Qt::AlignCenter);
	this->setItem(1, 0, newItem_1);

	QTableWidgetItem *newItem_2 = new QTableWidgetItem(tr("ActivePower"));
	newItem_2->setTextAlignment(Qt::AlignCenter);
	this->setItem(0, 1, newItem_2);

	QTableWidgetItem *newItem_3 = new QTableWidgetItem(tr("ReActivePower"));
	newItem_3->setTextAlignment(Qt::AlignCenter);
	this->setItem(1, 1, newItem_3);

	QPushButton *pushButton = new QPushButton("");
	pushButton->setFlat(true);
	this->setCellWidget(0, 2, pushButton);
	connect(pushButton, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));


	QPushButton *pushButton_1 = new QPushButton("");
	pushButton_1->setFlat(true);
	this->setCellWidget(1, 2, pushButton_1);
	connect(pushButton_1, SIGNAL(clicked()), this, SLOT(Slot_SetBindValue()));


}

QString CInverterGrpTableWidget::GetBindValue()
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

void CInverterGrpTableWidget::Slot_SetBindValue()
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
}



CInverterGrpTableWidget::~CInverterGrpTableWidget()
{


}


CInverterGroup* CInverterGrpTableWidget::GetInverterGrpItem() const
{
	Q_ASSERT(m_pInverterGrpItem);
	if (m_pInverterGrpItem)
	{
		return m_pInverterGrpItem;
	}
	else
	{
		return Q_NULLPTR;
	}

}


void CInverterGrpTableWidget::Save()
{


}


void CInverterGrpTableWidget::Refresh()
{


}


void CInverterGrpTableWidget::SetModifyFlag(bool bFlag)
{
	Q_UNUSED(bFlag);

}

bool CInverterGrpTableWidget::GetModifyFlag()
{
	return true;
}


CPredictInverterGrpWidget::CPredictInverterGrpWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTableWidget = pTableWidget;

	m_pCore = pCore;

}




CPredictInverterGrpWidget::~CPredictInverterGrpWidget()
{


}



void CPredictInverterGrpWidget::AddToTableCtrl(CInverterGroup* pInverterGrpItem)
{
	int nCount = m_pTableWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CInverterGrpTableWidget* pTableWdgt = dynamic_cast<CInverterGrpTableWidget*>(m_pTableWidget->widget(i));
		if (pTableWdgt)
		{
			CInverterGroup* pItem = pTableWdgt->GetInverterGrpItem();
			if (pItem && (pItem == pInverterGrpItem))
			{
				m_pTableWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CInverterGrpTableWidget* pTableWdgt = new CInverterGrpTableWidget(pInverterGrpItem);

	m_pTableWidget->AddTab(pTableWdgt, "Plant Data", 1, "");
}


void CPredictInverterGrpWidget::ReleaseData()
{



}


void CPredictInverterGrpWidget::SetTabViewHead()
{




}

