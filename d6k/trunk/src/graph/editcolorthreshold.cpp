
#include "editcolorthreshold.h"
#include "graph_module.h"
#include "scadastudio/icore.h"
#include "variant.h"

#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QCOmbobox>
#include <QSpinBox>
#include <QColorDialog>

EditCellWgt::EditCellWgt(QWidget *parent /*= 0*/)
{
	Q_UNUSED(parent);
	InitWgt();
}

void EditCellWgt::InitWgt()
{
	//颜色标识
	m_pLableColor = new QLabel(this);
	m_pLableColor->setFixedSize(28, 28);
	//颜色说明
	m_pLineEdColor = new QLineEdit(this);
	m_pLineEdColor->setReadOnly(true);
	m_pLineEdColor->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	m_pLineEdColor->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");

	//选择按键
	m_pPushCoose = new QPushButton(this);
	m_pPushCoose->setText("...");
	m_pPushCoose->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
	m_pPushCoose->setFixedSize(28, 28);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	layout->addWidget(m_pLableColor);
	layout->addWidget(m_pLineEdColor);
	layout->addWidget(m_pPushCoose);

	setAttribute(Qt::WA_InputMethodEnabled);

	connect(m_pPushCoose,SIGNAL(clicked()),this,SIGNAL(Signals_Comform()));

}


void EditCellWgt::SetLableHidden()
{
	m_pLableColor->setHidden(true);
}

void EditCellWgt::SetColor(const QColor &col)
{
	m_cloChoose = col;
	QString strCol = col.name();
	m_pLableColor->setStyleSheet("border:1px solid;border-color: rgb(0, 0, 0);background-color:" + strCol);

	m_pLineEdColor->setText(strCol);
}





void EditCellWgt::SetVarData(const QString &strVar)
{
	m_pLineEdColor->setText(strVar);
	m_strBindValue = strVar;
}

CEditColorThreshold::CEditColorThreshold(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->setHidden(true);
	setWindowTitle(tr("Color Threshold"));

	InitEditorWgt();
}

//初始化数据结构
void CEditColorThreshold::InitEditorWgt()
{
	
	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		ui.tableWidget->setRowHeight(i,30);
		ui.tableWidget->item(i, 0)->setFlags(ui.tableWidget->item(i, 0)->flags() & ~Qt::ItemIsEditable);
	}
	//值
	QTableWidgetItem *item0 = new QTableWidgetItem;

	item0->setText("0");
	ui.tableWidget->setItem(0, 1, item0);

	//变量
	m_pVarCellWgt = new EditCellWgt;
	m_pVarCellWgt->SetLableHidden();
	ui.tableWidget->setCellWidget(1, 1, m_pVarCellWgt);


	//颜色
	m_pColorCellWgt = new EditCellWgt;
	m_pColorCellWgt->SetColor(QColor(0,0,0));
	ui.tableWidget->setCellWidget(4,1, m_pColorCellWgt);

	//闪烁色
	m_pFlashingWgt = new EditCellWgt;
	m_pFlashingWgt->SetColor(QColor(255, 255, 255));
	ui.tableWidget->setCellWidget(5, 1, m_pFlashingWgt);

	//模式
	m_pComMode = new QComboBox;
	ui.tableWidget->setCellWidget(2, 1, m_pComMode);
	QStringList lstModes;
	lstModes << tr("Normal") << tr("Invisible") << tr("Slow Flashing")<<tr("Middle Flashing")<<tr("Fast Flashing")<<tr("Customize");

	m_pComMode->addItems(lstModes);

	//闪烁时间
	m_pFlashTime = new QSpinBox;
	m_pFlashTime->setMinimum(1);
	m_pFlashTime->setMaximum(999999);
	m_pFlashTime->setValue(500);
	ui.tableWidget->setCellWidget(3, 1, m_pFlashTime);

	connect(ui.tableWidget,SIGNAL(cellClicked(int, int)),this,SLOT(Slot_CellClicked(int,int)));

	connect(m_pColorCellWgt,SIGNAL(Signals_Comform()),this,SLOT(Slot_ColorChange()));

	connect(m_pFlashingWgt, SIGNAL(Signals_Comform()), this, SLOT(Slot_FlashColorChange()));
	//变量
	connect(m_pVarCellWgt, SIGNAL(Signals_Comform()), this, SLOT(Slot_VarBindData()));
	//确定
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
	//取消
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));

	ui.tableWidget->setRowHidden(1, true);
}

void CEditColorThreshold::ResetWgtData()
{
	ui.tableWidget->item(0, 1)->setText("0");
	m_pVarCellWgt->SetVarData("");
	m_pComMode->setCurrentIndex(0);
	m_pFlashTime->setValue(500);
	m_pColorCellWgt->SetColor(QColor(0,0,0));
	m_pFlashingWgt->SetColor(QColor(255,255,255));
}

void CEditColorThreshold::SetMode(int nIndex)
{
	m_pComMode->setCurrentIndex(nIndex);
}

void CEditColorThreshold::Slot_CellClicked(int row, int column)
{
	Q_UNUSED(row);
	Q_UNUSED(column);
// 	m_pVarCellWgt->setVisible(false);
// 	m_pColorCellWgt->setVisible(false);
// 	m_pFlashingWgt->setVisible(false);
// 	m_pComMode->setVisible(false);
// 	m_pFlashTime->setVisible(false);
// 
// 	if (column == 1 && row == 1)
// 	{
// 		m_pVarCellWgt->setHidden(false);
// 	}
// 	else if (column == 1 && row == 2)
// 	{
// 		m_pComMode->setHidden(false);
// 	}
// 	else if (column == 1 && row == 3)
// 	{
// 		m_pFlashTime->setHidden(false);
// 	}
// 	else if (column == 1 && row == 4)
// 	{
// 		m_pColorCellWgt->setHidden(false);
// 	}
// 	else if (column == 1 && row == 5)
// 	{
// 		m_pFlashingWgt->setHidden(false);
// 	}
}

void CEditColorThreshold::Slot_ColorChange()
{
	QColor color = QColorDialog::getColor(m_pColorCellWgt->GetColor(), this, tr("Color"), QColorDialog::ShowAlphaChannel);
	if (color.isValid())
	{
		m_pColorCellWgt->SetColor(color);
	}
}

void CEditColorThreshold::Slot_FlashColorChange()
{
	QColor color = QColorDialog::getColor(m_pFlashingWgt->GetColor(), this, tr("Color"), QColorDialog::ShowAlphaChannel);
	if (color.isValid())
	{
		m_pFlashingWgt->SetColor(color);
	}
	

}

void CEditColorThreshold::Slot_VarChoose()
{

}

void CEditColorThreshold::Slot_DoubleClickTable(QTableWidgetItem *item)
{
	//双击table
	Q_UNUSED(item);
}

void CEditColorThreshold::Slot_VarBindData()
{
	std::string strNodeTagname;
	std::string strTagname;
	std::string strFilename;
	//
	std::vector<int> arrDataType;
	arrDataType.push_back(DT_NULL);
	arrDataType.push_back(DT_BOOLEAN);
	arrDataType.push_back(DT_CHAR);
	arrDataType.push_back(DT_BYTE);
	arrDataType.push_back(DT_SHORT);
	arrDataType.push_back(DT_WORD);
	arrDataType.push_back(DT_INT);
	arrDataType.push_back(DT_DWORD);
	arrDataType.push_back(DT_LLONG);
	arrDataType.push_back(DT_LWORD);
	arrDataType.push_back(DT_FLOAT);
	arrDataType.push_back(DT_DOUBLE);
	arrDataType.push_back(DT_STRING);
	arrDataType.push_back(DT_BLOCK);
	arrDataType.push_back(DT_MAX);
	//
	bool bSelectFlag = GetIMainInterface()->SelectTag(arrDataType, true, strNodeTagname, strTagname, strFilename);

	if (bSelectFlag)
	{
		m_strBindData = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());
		m_pVarCellWgt->SetVarData(m_strBindData);
	}
	else
	{
		m_pVarCellWgt->SetVarData("");
	}
}

void CEditColorThreshold::Slot_Conform()
{
	m_nValue = ui.tableWidget->item(0, 1)->text().toInt();
	m_strBindData = m_pVarCellWgt->GetVarData();
	m_nMode = m_pComMode->currentIndex();
	m_nFlashingTime = m_pFlashTime->value();
	m_Color = m_pColorCellWgt->GetColor();
	m_FlashingColr = m_pFlashingWgt->GetColor();
	accept();

	emit Signal_Comform();
	this->close();
}

CEditColorThreshold::~CEditColorThreshold()
{
}




