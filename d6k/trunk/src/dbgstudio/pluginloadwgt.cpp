

#include "pluginloadwgt.h"

#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QApplication>
#include <QDir>

CPluginLoadWgt::CPluginLoadWgt(QWidget *parent)
	: QWidget(parent),
	m_pTableWgt(new QTableWidget(this))
{
	setWindowTitle(tr("LoadingDevice"));
	QPushButton *pBtnConform = new QPushButton(tr("Conform"),this);
	QPushButton *pBtnCancel = new QPushButton(tr("Cancel"),this);

	QVBoxLayout *pWgtLayout = new QVBoxLayout(this);
	pWgtLayout->addWidget(m_pTableWgt);
	
	QHBoxLayout *pBtnLayout = new QHBoxLayout;
	pBtnLayout->addStretch();
	pBtnLayout->addWidget(pBtnConform);
	pBtnLayout->addWidget(pBtnCancel);
	pWgtLayout->addLayout(pBtnLayout);
	resize(600, 400);

	m_pTableWgt->setColumnCount(COL_MAX);
	QStringList lstHorHeader;
	lstHorHeader << tr("NO") << tr("Name") << tr("Version") << tr("Remark") << tr("Dll");
	m_pTableWgt->setHorizontalHeaderLabels(lstHorHeader);
	m_pTableWgt->horizontalHeader()->setStretchLastSection(true);
	m_pTableWgt->setColumnHidden(COL_DLL, true);
	m_pTableWgt->setSortingEnabled(true);
	m_pTableWgt->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTableWgt->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(pBtnConform, SIGNAL(clicked()), this, SLOT(Slot_ClickConform()));
	connect(pBtnCancel,SIGNAL(clicked()),this,SLOT(close()));
	connect(m_pTableWgt, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_ClickConform()));
	//获取可执行文件路径
	QString strRunPath = qApp->applicationDirPath();
	//
	OpenPluginFile(strRunPath + PLUGIN_DLL_FILE);
}

CPluginLoadWgt::~CPluginLoadWgt()
{

}

//打开文件
void CPluginLoadWgt::OpenPluginFile(const QString &strFilename)
{
	if (strFilename.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("read Config file is null,please check!"));
		return;
	}

	QFile fPluginFile(strFilename);
	if (fPluginFile.open(QFile::ReadOnly | QFile::Text))
	{
		AnalyseXmlData(&fPluginFile);
		fPluginFile.close();
	}
	else
	{
		QMessageBox::warning(this, tr("warning"), tr("open file %1 error!").arg(strFilename));
		return;
	}

}

//解析文件
void CPluginLoadWgt::AnalyseXmlData(QIODevice *pDevice)
{
	QXmlStreamReader xmlReader(pDevice);
	 
	while (!xmlReader.atEnd() && (!xmlReader.hasError()))
	{
		//遍历所有配置
		xmlReader.readNext();
		if (xmlReader.isStartElement())
		{
			//qDebug() << "xmlReader:"<<xmlReader.name().toString();
			if (xmlReader.name().toString() == "deviceitem")
			{
				//解析数据 写入表格中
				QString strNum = xmlReader.attributes().value("number").toString();
				QString strName = xmlReader.attributes().value("devicename").toString();
				QString strCommon = xmlReader.attributes().value("common").toString();
				QString strVersion = xmlReader.attributes().value("version").toString();
				QString strDll = xmlReader.attributes().value("dllname").toString();
				InsertDataToTable(strNum,strName,strCommon,strVersion,strDll);
			}
		}
	}
	
}

//往表格中插入数据
void CPluginLoadWgt::InsertDataToTable(const QString &strNum, const QString &strName, const QString &strCommon, const QString &strVersion, const QString &strDll)
{
	//编号
	QTableWidgetItem *tPluginNum = new QTableWidgetItem(strNum);
	//名称
	QTableWidgetItem *tPluginName = new QTableWidgetItem(strName);
	//备注
	QTableWidgetItem *tPluginCommon = new QTableWidgetItem(strCommon);
	//版本号
	QTableWidgetItem *tPluginVersion = new QTableWidgetItem(strVersion);
	//动态库
	QTableWidgetItem *tPluginDll = new QTableWidgetItem(strDll);

	int iCurrentRows = m_pTableWgt->rowCount();
	m_pTableWgt->insertRow(iCurrentRows);
	m_pTableWgt->setItem(iCurrentRows, COL_NUMBER, tPluginNum);
	m_pTableWgt->setItem(iCurrentRows, COL_NAME, tPluginName);
	m_pTableWgt->setItem(iCurrentRows, COL_VER, tPluginVersion);
	m_pTableWgt->setItem(iCurrentRows, COL_COMMENT, tPluginCommon);
	m_pTableWgt->setItem(iCurrentRows, COL_DLL, tPluginDll);
}

void CPluginLoadWgt::Slot_ClickConform()
{
	this->setCursor(Qt::WaitCursor);
	//选择确定
	if (m_pTableWgt->currentRow() < 0)
	{
		QMessageBox::warning(this, tr("warning"), tr("Please Choose One Row!"));
		return;
	}
	this->close();

	//发出需要加载动态库名称的信号
	emit Signal_NeedLoadInfo(m_pTableWgt->item(m_pTableWgt->currentRow(), COL_DLL)->text(), m_pTableWgt->item(m_pTableWgt->currentRow(), COL_NAME)->text());
	this->setCursor(Qt::ArrowCursor);
}
