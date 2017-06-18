#include "thresholdchoosewgt.h"
#include "editcolorthreshold.h"
#include "dync_color.h"


CThresholdChooseWgt::CThresholdChooseWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setColumnWidth(0, 200);
	ui.tableWidget->setColumnWidth(1, 150);
	//SetDefaultVale();
	//setAttribute(Qt::WA_DeleteOnClose);
	ui.tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
	ui.tableWidget->verticalHeader()->setHidden(true);

	m_pEditColorWgt = new CEditColorThreshold;
	//增加
	connect(ui.pushButton_3,SIGNAL(clicked()),this,SLOT(Slot_AddNewItem()));
	//确定
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Slot_Conform()));
	//取消
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
	//delete
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(Slot_DeleteColor()));
	//edit
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(Slot_EditColor()));
	//defaule
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(Slot_SetDefault()));
	//
	connect(ui.tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(Slot_TableDoubleClick(QTableWidgetItem *)));
	//
	connect(m_pEditColorWgt,SIGNAL(Signal_Comform()),this,SLOT(Slot_UpdateEditData()));
	//
	ui.tableWidget->setColumnHidden(2, true);
}

CThresholdChooseWgt::~CThresholdChooseWgt()
{
	m_pEditColorWgt->deleteLater();
	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(ui.tableWidget->item(i, 0)->data(Qt::UserRole + 100).toLongLong());
		Q_ASSERT(pGrp);
		if (pGrp == nullptr)
		{
			return;
		}
		delete pGrp;
	}
}

void CThresholdChooseWgt::SetDefaultVale()
{
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);

	for (int i=0; i<7; i++)
	{
		ui.tableWidget->insertRow(0);

		CDyncClrValueInfo *pThresholdData = new CDyncClrValueInfo;

		QTableWidgetItem *item0 = new QTableWidgetItem;

		item0->setData(Qt::UserRole + 100, reinterpret_cast<long long>(pThresholdData));

		if (i == 0)
		{
			QColor tcol(170, 255, 0);
			item0->setBackground(QBrush(tcol));
			pThresholdData->m_dwColor = tcol;
		}
		else if (i == 1)
		{
			QColor tcol(255, 255, 0);
			item0->setBackground(QBrush(tcol));
			pThresholdData->m_dwColor = tcol;
		}
		else if (i == 2)
		{
			QColor tcol(85, 255, 127);
			item0->setBackground(QBrush(QColor(85, 255, 127)));
			pThresholdData->m_dwColor = tcol;
		}
		else if (i == 3)
		{
			QColor tcol(255, 170, 127);
			item0->setBackground(QBrush(QColor(255, 170, 127)));
			pThresholdData->m_dwColor = tcol;
		}
		else if (i == 4)
		{
			QColor tcol(85, 170, 0);
			item0->setBackground(QBrush(QColor(85, 170, 0)));
			pThresholdData->m_dwColor = tcol;
		}
		else if (i == 5)
		{
			QColor tcol(0, 85, 255);
			item0->setBackground(QBrush(QColor(0, 85, 255)));
			pThresholdData->m_dwColor = tcol;
		}
		else if (i == 6)
		{
			QColor tcol(255, 0, 0);
			item0->setBackground(QBrush(QColor(255, 0, 0)));
			pThresholdData->m_dwColor = tcol;
		}

		ui.tableWidget->setItem(0, 0, item0);

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(QString::number(i));
		ui.tableWidget->setItem(0, 1, item1);

		pThresholdData->m_dValue = i;

		QTableWidgetItem *item2 = new QTableWidgetItem;
		ui.tableWidget->setItem(0, 2, item2);

		pThresholdData->m_nBlinkInterval = 500;
		pThresholdData->m_nBlinkingType = CDyncClrValueInfo::BLINKING_NORMAL;
		pThresholdData->m_dwBlinkingColor = QColor(255,255,255);

	}

}

void CThresholdChooseWgt::InitEditWgt(CDyncClrValueInfo *pBrushData)
{
	Q_ASSERT(pBrushData);
	if (pBrushData == nullptr)
	{
		return;
	}

	//阈值
	m_pEditColorWgt->SetValue(pBrushData->m_dValue);
	//绑定值
	//m_pEditColorWgt->SetVarable(pBrushData->GetBindData());
	//mode
	m_pEditColorWgt->SetMode(pBrushData->m_nBlinkingType);
	//flashtime
	m_pEditColorWgt->SetFlashingTime(pBrushData->m_nBlinkInterval);
	//颜色
	m_pEditColorWgt->SetColor(pBrushData->m_dwColor);
	//闪烁色
	m_pEditColorWgt->SetFlahingColor(pBrushData->m_dwBlinkingColor);

	m_pEditColorWgt->SetTypeFlag(CEditColorThreshold::ENUM_EDIT);
	m_pEditColorWgt->exec();
}

void CThresholdChooseWgt::UpdateEditData(CDyncClrValueInfo *pBrushData)
{
	Q_ASSERT(pBrushData);
	
	if (pBrushData == nullptr)
	{
		return;
	}
}

const QList<CDyncClrValueInfo *>& CThresholdChooseWgt::GetAllBrushInfo()
{
	m_lstBrushInfo.clear();

	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(ui.tableWidget->item(i,0)->data(Qt::UserRole + 100).toLongLong());

		Q_ASSERT(pGrp);
		if (pGrp == nullptr)
		{
			break;
		}

		m_lstBrushInfo.append(pGrp);

	}
	return m_lstBrushInfo;
}

void CThresholdChooseWgt::UpdateAllData(CDyncClrData *pDyBrush)
{
	Q_ASSERT(pDyBrush);
	if (pDyBrush == nullptr)
	{
		return;
	}

// 	if (pDyBrush->GetBrushThresholdData().size() == 0)
// 	{
// 		//默认值
// 		SetDefaultVale();
// 	}
// 	else
// 	{
	for (auto it : pDyBrush->m_arrColors)
	{
		int nRowCount = ui.tableWidget->rowCount();

		ui.tableWidget->insertRow(nRowCount);
		QTableWidgetItem *item0 = new QTableWidgetItem;
		ui.tableWidget->setItem(nRowCount, 0, item0);

		QColor tcol(it->m_dwColor);
		item0->setBackground(QBrush(tcol));

		QTableWidgetItem *item1 = new QTableWidgetItem;
		ui.tableWidget->setItem(nRowCount, 1, item1);
		item1->setText(QString::number(it->m_dValue));


		QTableWidgetItem *item2 = new QTableWidgetItem;
		ui.tableWidget->setItem(nRowCount, 2, item2);
		//item2->setText(it->GetBindData());

		CDyncClrValueInfo *pThresholdData = new CDyncClrValueInfo;

		pThresholdData->m_dValue = it->m_dValue;
		pThresholdData->m_nBlinkingType = it->m_nBlinkingType;
		pThresholdData->m_nBlinkInterval = it->m_nBlinkInterval;
		pThresholdData->m_dwColor = it->m_dwColor;
		pThresholdData->m_dwBlinkingColor = it->m_dwBlinkingColor;

		//pThresholdData->SetBindData(it->GetBindData());
		item0->setData(Qt::UserRole + 100, reinterpret_cast<long long>(pThresholdData));

	}

	/*}*/
}

void CThresholdChooseWgt::Slot_AddNewItem()
{
	m_pEditColorWgt->ResetWgtData();
	m_pEditColorWgt->SetTypeFlag(CEditColorThreshold::ENUM_CREATE);

	m_pEditColorWgt->exec();
	
}

void CThresholdChooseWgt::Slot_Conform()
{
	this->accept();
	this->close();
}

void CThresholdChooseWgt::Slot_EditColor()
{
	//
	if (ui.tableWidget->currentItem() == nullptr)
	{
		return;
	}

	QTableWidgetItem *pitem = ui.tableWidget->item(ui.tableWidget->currentItem()->row(), 0);

	CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(pitem->data(Qt::UserRole+100).toLongLong());
	InitEditWgt(pGrp);

}

void CThresholdChooseWgt::Slot_DeleteColor()
{
	if (ui.tableWidget->currentRow() < 0)
	{
		return;
	}

	if (ui.tableWidget->currentItem()->row() == -1)
	{
		return;
	}

	QTableWidgetItem *pitem = ui.tableWidget->item(ui.tableWidget->currentItem()->row(), 0);

	CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(pitem->data(Qt::UserRole + 100).toLongLong());
	Q_ASSERT(pGrp);
	if (pGrp == nullptr)
	{
		return;
	}
	delete pGrp;

	ui.tableWidget->removeRow(ui.tableWidget->currentRow());

}

void CThresholdChooseWgt::Slot_SetDefault()
{
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(ui.tableWidget->item(i, 0)->data(Qt::UserRole + 100).toLongLong());
		Q_ASSERT(pGrp);
		if (pGrp == nullptr)
		{
			return;
		}
		delete pGrp;
	}

	SetDefaultVale();
}

void CThresholdChooseWgt::Slot_TableDoubleClick(QTableWidgetItem *item)
{
	//
	QTableWidgetItem *pitem = ui.tableWidget->item(item->row(), 0);
	CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(pitem->data(Qt::UserRole + 100).toLongLong());
	InitEditWgt(pGrp);
}

void CThresholdChooseWgt::Slot_UpdateEditData()
{
	int nRow = 0;

	if (m_pEditColorWgt->GetTypeFlay() == CEditColorThreshold::ENUM_CREATE)
	{
		ui.tableWidget->insertRow(0);
		QTableWidgetItem *item0 = new QTableWidgetItem;
		ui.tableWidget->setItem(0, 0, item0);

		QTableWidgetItem *item1 = new QTableWidgetItem;
		ui.tableWidget->setItem(0, 1, item1);


		QTableWidgetItem *item2 = new QTableWidgetItem;
		ui.tableWidget->setItem(0, 2, item2);

		CDyncClrValueInfo *pThresholdData = new CDyncClrValueInfo;

		item0->setData(Qt::UserRole + 100, reinterpret_cast<long long>(pThresholdData));
		nRow = 0;

	}
	else
	{
		nRow = ui.tableWidget->currentItem()->row();
	}

	//第一列
	QTableWidgetItem *pCurrentItem = ui.tableWidget->item(nRow,0);

	CDyncClrValueInfo* pGrp = reinterpret_cast<CDyncClrValueInfo*>(pCurrentItem->data(Qt::UserRole + 100).toLongLong());
	Q_ASSERT(pGrp);
	if (pGrp == nullptr)
	{
		return;
	}

	//第一列显示内容
	pCurrentItem->setBackground(QBrush(m_pEditColorWgt->GetColor()));

	//第二列显示内容
	ui.tableWidget->item(nRow, 1)->setText(QString::number(m_pEditColorWgt->GetValue()));
	//第三列显示内容
	ui.tableWidget->item(nRow, 2)->setText(m_pEditColorWgt->GetVarable());
	

	//属性   阈值
	pGrp->m_dValue = m_pEditColorWgt->GetValue();
	//属性  颜色
	pGrp->m_dwColor = m_pEditColorWgt->GetColor();
	//属性   显示颜色
	pGrp->m_dwBlinkingColor = m_pEditColorWgt->GetFlashingColor();
	//显示 模式
	pGrp->m_nBlinkingType = CDyncClrValueInfo::BLINKING_TYPE(m_pEditColorWgt->GetMode());
	//闪烁时间
	pGrp->m_nBlinkInterval = m_pEditColorWgt->GetFlashingTime();

}
