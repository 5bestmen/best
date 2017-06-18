
#include "binddatawgt.h"
#include "graph_module.h"
#include "scadastudio/icore.h"
#include "thresholdchoosewgt.h"
#include "variant.h"
#include "dync_color.h"
#include "dync_text.h"
#include "text_threshold.h"
#include "graph_module.h"
#include "orderlistwgt.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QFocusEvent>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>
#include <QDir>
#include <QFile>
#include <QDateTime>


// CBindDataWgt::CBindDataWgt(QWidget *parent)
// 	: QWidget(parent)
// {
// 	ui.setupUi(this);
// 	this->setFixedSize(100, 100);
// }
// 
// CBindDataWgt::~CBindDataWgt()
// {
// }





CBindDataWgt::CBindDataWgt(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	m_pLineEdit = new QLineEdit(this);
	m_pLineEdit->setReadOnly(true);
	//m_pLineEdit->setReadOnly(true);
	//m_pLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	QToolButton *button = new QToolButton(this);
	button->setText("...");
	//button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
	//button->setText(QLatin1String("..."));
	layout->addWidget(m_pLineEdit);

	m_pLable = new QLabel(this);
	m_pLable->setHidden(true);
	m_pLable->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	layout->addWidget(m_pLable);
	layout->addWidget(button);
	m_pLineEdit->setFocusPolicy(Qt::NoFocus);
	//setFocusProxy(m_pLineEdit);
	//setFocusPolicy(Qt::StrongFocus);
	//setAttribute(Qt::WA_InputMethodEnabled);
	//connect(theLineEdit, SIGNAL(textEdited(const QString &)),
	//         this, SIGNAL(filePathChanged(const QString &)));
	connect(button, SIGNAL(clicked()),this, SLOT(Slot_ButtonClicked()));

	m_pLineEdit->installEventFilter(this);
}


void CBindDataWgt::SetBindData(const QString &filePath)
{
// 	if (m_pLineEdit->text() != filePath)
// 	{
		m_pLineEdit->setText(filePath);
	//}
}


QString CBindDataWgt::SaveImageFile(const QString &strFilename)
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
			return "/graphimage/"  + strNewFileName;
		}
	}
}

void CBindDataWgt::Slot_ButtonClicked()
{
	if (m_nTypeFlag == ITEM_VAR)
	{
		std::string strNodeTagname;
		std::string strTagname;
		std::string strFilename;
		//
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
		m_bWriteFlag = true;
		//
		bool bSelectFlag = GetIMainInterface()->SelectTag(m_arrDataType, m_bWriteFlag, strNodeTagname, strTagname, strFilename);

		if (bSelectFlag)
		{
			QString strResult = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());

			m_pLineEdit->setText(strResult);

			emit Signal_ValueChanged(strResult);

		}
	}
	else if (m_nTypeFlag == ITEM_COLOR_RANGE)
	{
		CThresholdChooseWgt *pWgt = new CThresholdChooseWgt;

		//数据信息
		CBaseDyncData *pDynamicBrushInfo = reinterpret_cast<CBaseDyncData*>(m_lDynamicBrush);
		Q_ASSERT(pDynamicBrushInfo);
		if (pDynamicBrushInfo == nullptr)
		{
			return;
		}

		if (pDynamicBrushInfo->m_nDyncType == CBaseDyncData::DYNC_FILLING || pDynamicBrushInfo->m_nDyncType == CBaseDyncData::DYNC_PEN_COLOR ||
			pDynamicBrushInfo->m_nDyncType == CBaseDyncData::DYNC_TEXT_COLOR)
		{
			CDyncClrData *pFillingData = dynamic_cast<CDyncClrData*>(pDynamicBrushInfo);

			pWgt->UpdateAllData(pFillingData);

			if (pWgt->exec() == 1)
			{
				pFillingData->ClearAllClrData();

				for each (CDyncClrValueInfo * var in pWgt->GetAllBrushInfo())
				{
					CDyncClrValueInfo * tBrushThreshold = pFillingData->CreateClrData();

					Q_ASSERT(tBrushThreshold);

					if (tBrushThreshold != nullptr)
					{
						tBrushThreshold->m_dwColor = var->m_dwColor;
						tBrushThreshold->m_dwBlinkingColor = var->m_dwBlinkingColor;
						tBrushThreshold->m_nBlinkingType = var->m_nBlinkingType;
						tBrushThreshold->m_nBlinkInterval = var->m_nBlinkInterval;
						tBrushThreshold->m_dValue = var->m_dValue;
					}

				}

			}
		}

		pWgt->deleteLater();
	}
	else if (m_nTypeFlag == ITEM_TEXT_RANGE)
	{
		CTextThreshold *pWgt = new CTextThreshold;

		//数据信息
		CBaseDyncData *pDynamicTextInfo = reinterpret_cast<CBaseDyncData*>(m_lDynamicBrush);
		Q_ASSERT(pDynamicTextInfo);
		if (pDynamicTextInfo == nullptr)
		{
			return;
		}


		CDyncText *pTextData = dynamic_cast<CDyncText*>(pDynamicTextInfo);

		pWgt->UpdateAllData(pTextData);

		if (pWgt->exec() == 1)
		{

		}

		pWgt->deleteLater();
	}
	else if (m_nTypeFlag == ITEM_MAP_RANGE)
	{
		CTextThreshold *pWgt = new CTextThreshold;

		//数据信息
		CBaseDyncData *pDynamicTextInfo = reinterpret_cast<CBaseDyncData*>(m_lDynamicBrush);
		Q_ASSERT(pDynamicTextInfo);
		if (pDynamicTextInfo == nullptr)
		{
			return;
		}


		CDyncText *pTextData = dynamic_cast<CDyncText*>(pDynamicTextInfo);

		pWgt->UpdateMapData(pTextData);

		if (pWgt->exec() == 1)
		{

		}

		pWgt->deleteLater();
	}
	else if (m_nTypeFlag == ITEM_BACKGROUND_CHOOSE)
	{
		//选择
		QString strFilePath = QFileDialog::getOpenFileName(0, tr("Save File"), "./");

		if (!strFilePath.isEmpty())
		{
			QString strNewName = SaveImageFile(strFilePath);
			emit Signal_ValueChanged(strNewName);
		}
	}
	else if (m_nTypeFlag == ITEM_PUSHBUTTON_ORDER)
	{
		//按钮 命令
		qDebug() << "enter order widget";
		COrderListWgt *pOrderLstwgt = new COrderListWgt;
		pOrderLstwgt->exec();

		pOrderLstwgt->deleteLater();
	}

}

bool CBindDataWgt::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == m_pLineEdit)
	{
		if (event->type() == QEvent::FocusOut)
		{
			emit Signal_ValueChanged(m_pLineEdit->text());
		}

	}

	return QWidget::eventFilter(watched, event);

}

void CBindDataWgt::focusInEvent(QFocusEvent *e)
{
	m_pLineEdit->event(e);
	if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) 
	{
		m_pLineEdit->selectAll();
	}
	QWidget::focusInEvent(e);
}

void CBindDataWgt::focusOutEvent(QFocusEvent *e)
{
	m_pLineEdit->event(e);
	QWidget::focusOutEvent(e);

}

void CBindDataWgt::keyPressEvent(QKeyEvent *e)
{
	m_pLineEdit->event(e);

	emit Signal_ValueChanged(m_pLineEdit->text());
}

void CBindDataWgt::keyReleaseEvent(QKeyEvent *e)
{
	m_pLineEdit->event(e);
}

