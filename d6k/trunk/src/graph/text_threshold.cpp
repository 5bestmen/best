#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include "text_threshold.h"
#include "dync_text.h"
#include "graph_module.h"





CMapCellWgt::CMapCellWgt(QWidget *parent /*= 0*/)
{
	Q_UNUSED(parent);
	InitWgt();
}

void CMapCellWgt::InitWgt()
{
	//颜色标识
	m_pLableMap = new QLabel(this);
	m_pLableMap->setFixedSize(28, 28);
	//颜色说明
	m_pLineEdMap = new QLineEdit(this);
	m_pLineEdMap->setReadOnly(true);
	m_pLineEdMap->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	m_pLineEdMap->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");

	//选择按键
	m_pPushCoose = new QPushButton(this);
	m_pPushCoose->setText("...");
	m_pPushCoose->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
	m_pPushCoose->setFixedSize(28, 28);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	layout->addWidget(m_pLableMap);
	layout->addWidget(m_pLineEdMap);
	layout->addWidget(m_pPushCoose);

	setAttribute(Qt::WA_InputMethodEnabled);

	connect(m_pPushCoose, SIGNAL(clicked()), this, SLOT(Slot_Confomrm()));

}

//
void CMapCellWgt::Slot_Confomrm()
{
	QString strFilePath = QFileDialog::getOpenFileName(0, tr("Open File"), "./");

	if (strFilePath.isEmpty() == false)
	{
		QString strnewPath = SaveImageFile(strFilePath);
		SetMapPath(strnewPath);
	}
}


void CMapCellWgt::SetMapPath(const QString &strPath)
{
	m_strMapPath = strPath;
	m_pLineEdMap->setText(strPath.split("/").last());

	m_pLableMap->setPixmap(QPixmap(GetModuleApi()->GetGraphicPath() + strPath));
}




QString CMapCellWgt::SaveImageFile(const QString &strFilename)
{
	//图形目录
	QString strImagePath = GetModuleApi()->GetGraphicPath() + "/graphimage/";
	QDir tDir(strImagePath);
	if (!tDir.exists())
	{
		tDir.mkdir(strImagePath);
	}

	//是否相同名称的文件
	if (QFile::exists(strImagePath + strFilename.split("/").last()))
	{
		//是否是同一个文件
		qDebug() << tDir.absoluteFilePath(strFilename);
		qDebug() << strImagePath + strFilename.split("/").last();
		if (strFilename.contains("/graphimage/" + strFilename.split("/").last()))
		{
			return "/graphimage/" + strFilename.split("/").last();
		}
		else
		{
			QString strNewFileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "." + strFilename.split(".").last();
			strNewFileName.remove(" ");

			//不存在  创建
			if (!QFile::copy(strFilename, strImagePath + strNewFileName))
			{
				return "";
			}
			else
			{
				//返回的是相对路径
				return "/graphimage/" + strNewFileName;
			}


		}

	}
	else
	{
		QString strNewFileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "." + strFilename.split(".").last();
		strNewFileName.remove(" ");
		//不存在
		if (!QFile::copy(strFilename, strImagePath + strNewFileName))
		{
			return "";
		}
		else
		{
			//返回的是相对路径
			return "/graphimage/" + strNewFileName;
		}
	}
}

CTextThreshold::CTextThreshold(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_pDyncText = nullptr;
	connect(ui.pushButton_3,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
	connect(ui.pushButton_4,SIGNAL(clicked()),this,SLOT(close()));
	//add
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_AddItems()));
	//delete
	connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(Slot_DeleteItems()));
}

CTextThreshold::~CTextThreshold()
{
}

void CTextThreshold::UpdateAllData(CDyncText* pDyncText)
{
	m_pDyncText = pDyncText;
	m_nType = TEXT_ITEM;

	InsertDataToWgt(m_pDyncText);
}

void CTextThreshold::UpdateMapData(CDyncText* pDyncText)
{
	m_pDyncText = pDyncText;

	Q_ASSERT(m_pDyncText);
	if (m_pDyncText == nullptr)
	{
		return;
	}

	m_nType = MAP_ITEM;

	for each (CDyncTextValueInfo* var in m_pDyncText->m_arrTexts)
	{
		int nRowCount = ui.tableWidget->rowCount();

		ui.tableWidget->insertRow(nRowCount);

		CMapCellWgt *pCellWgt = new CMapCellWgt;
		pCellWgt->SetMapPath(var->GetDescData());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(var->GetorigData());

		ui.tableWidget->setCellWidget(nRowCount, 0, pCellWgt);
		ui.tableWidget->setItem(nRowCount, 1, item1);

	}

	ui.tableWidget->setColumnWidth(0, 200);

}

void CTextThreshold::InsertDataToWgt(CDyncText* pDyncText)
{
	m_pDyncText = pDyncText;

	Q_ASSERT(m_pDyncText);
	if (m_pDyncText == nullptr)
	{
		return;
	}

	m_nType = TEXT_ITEM;

	for each (CDyncTextValueInfo* var in m_pDyncText->m_arrTexts)
	{
		int nRowCount = ui.tableWidget->rowCount();

		ui.tableWidget->insertRow(nRowCount);
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(var->GetorigData());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(var->GetDescData());

		ui.tableWidget->setItem(nRowCount, 0, item0);
		ui.tableWidget->setItem(nRowCount, 1, item1);

	}

}

void CTextThreshold::Slot_Conform()
{
	Q_ASSERT(m_pDyncText);
	if (m_pDyncText == nullptr)
	{
		return;
	}

	m_pDyncText->ClearAllTextData();

	if (m_nType == TEXT_ITEM)
	{


		for (int i = 0; i < ui.tableWidget->rowCount(); i++)
		{
			CDyncTextValueInfo *pTextInfo = m_pDyncText->CreateTextData();

			Q_ASSERT(pTextInfo);
			if (pTextInfo == nullptr)
			{
				return;
			}

			pTextInfo->SetOrigData(ui.tableWidget->item(i, 0)->text());

			pTextInfo->SetDescData(ui.tableWidget->item(i, 1)->text());
		}
	}
	else if (m_nType == MAP_ITEM)
	{
		for (int i = 0; i < ui.tableWidget->rowCount(); i++)
		{
			CDyncTextValueInfo *pTextInfo = m_pDyncText->CreateTextData();

			Q_ASSERT(pTextInfo);
			if (pTextInfo == nullptr)
			{
				return;
			}

			CMapCellWgt *pCellWgt = dynamic_cast<CMapCellWgt*>(ui.tableWidget->cellWidget(i, 0));

			pTextInfo->SetOrigData(ui.tableWidget->item(i, 1)->text());
			pTextInfo->SetDescData(pCellWgt->GetMapPath());
		}
	}


	this->close();
}

void CTextThreshold::Slot_AddItems()
{
	if (m_nType == TEXT_ITEM)
	{
		ui.tableWidget->insertRow(0);
		QTableWidgetItem *item0 = new QTableWidgetItem;
		QTableWidgetItem *item1 = new QTableWidgetItem;
		ui.tableWidget->setItem(0, 0, item0);
		ui.tableWidget->setItem(0, 1, item1);
	}
	else if (m_nType == MAP_ITEM)
	{
		ui.tableWidget->insertRow(0);
		CMapCellWgt *item0 = new CMapCellWgt;

		QTableWidgetItem *item1 = new QTableWidgetItem;
		ui.tableWidget->setCellWidget(0, 0, item0);
		ui.tableWidget->setItem(0, 1, item1);

	}
}

void CTextThreshold::Slot_DeleteItems()
{
	int nRow = ui.tableWidget->currentRow();

	if (nRow != -1)
	{
		ui.tableWidget->removeRow(nRow);
	}
}
