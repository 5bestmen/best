#include "ordereditwgt.h"
#include "editcolorthreshold.h"
#include <QComboBox>
#include <QSpinBox>
#include "scadastudio/icore.h"
#include "variant.h"
#include "graph_module.h"
#include "browsescreenwgt.h"
#include "graph_module.h"
#include "dync_var_op_event.h"
#include "dync_file_op_event.h"



COrderEditWgt::COrderEditWgt(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_7->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_8->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_9->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_10->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_11->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//初始化变量数据结构
	InitVarTabInfo();
	//初始化window
	InitWindowTabInfo();
	//
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_OrderConform()));
	//取消
	connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
}

COrderEditWgt::~COrderEditWgt()
{
}

void COrderEditWgt::InitVarTabInfo()
{
	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		ui.tableWidget->item(i, 0)->setFlags(ui.tableWidget->item(i, 0)->flags() ^ Qt::ItemIsEditable);;
	}
	//变量
	m_pVarCellWgt = new EditCellWgt;
	m_pVarCellWgt->SetLableHidden();
	ui.tableWidget->setCellWidget(0, 1, m_pVarCellWgt);
	connect(m_pVarCellWgt,SIGNAL(Signals_Comform()),this,SLOT(Slot_VarBindData()));

	//action
	m_pVarActionCombox = new QComboBox;
	QStringList lstActions;
	lstActions << tr("set") << tr("reset") << tr("toggle") << tr("strobe") << tr("decrease") << tr("alphanumeric pad") << tr("numeric pad")
		<< tr("Append Value") << tr("Remove Value") << tr("Swap Plus-Minus") << tr("Append Decimal Mode ON-OFF") << tr("Move Value") 
		<< tr("Reset Statistic") << tr("Move Min.Value") << tr("Move Max. Value") << tr("Move Average value") << tr("Set da ID di Stringa");
	m_pVarActionCombox->addItems(lstActions);
	ui.tableWidget->setCellWidget(1, 1, m_pVarActionCombox);


	//传递到变量
	m_pVarTransforVarCellWgt = new EditCellWgt;
	m_pVarTransforVarCellWgt->SetLableHidden();
	ui.tableWidget->setCellWidget(2, 1, m_pVarTransforVarCellWgt);
	connect(m_pVarTransforVarCellWgt, SIGNAL(Signals_Comform()), this, SLOT(Slot_VarBindData()));

	//选通时间
	m_pVarxtTime = new QSpinBox;
	ui.tableWidget->setCellWidget(4, 1, m_pVarxtTime);
	m_pVarxtTime->setMinimum(0);

	//最大字符数
	m_pVarMaxStringLegth = new QSpinBox;
	ui.tableWidget->setCellWidget(7, 1, m_pVarMaxStringLegth);
	m_pVarMaxStringLegth->setMinimum(0);

}

void COrderEditWgt::InitWindowTabInfo()
{
	for (int i = 0; i < ui.tableWidget_2->rowCount(); i++)
	{
		ui.tableWidget_2->item(i, 0)->setFlags(ui.tableWidget_2->item(i, 0)->flags() ^ Qt::ItemIsEditable);;
	}
	//变量
	m_pWindowCellWgt = new EditCellWgt;
	m_pWindowCellWgt->SetLableHidden();
	ui.tableWidget_2->setCellWidget(0, 1, m_pWindowCellWgt);
	connect(m_pWindowCellWgt, SIGNAL(Signals_Comform()), this, SLOT(Slot_ChooseWindow()));

	//action
	m_pWindowActionCombobox = new QComboBox;
	QStringList lstActions;
	lstActions << tr("Open Normal (scree change)") << tr("Open Modal (pop-up screen)") << tr("Open Frame (multi-monitor)") 
		<< tr("Open in other process (Safe Mode)") << tr("Print") << tr("Close and Return Back") << tr("Execute Synapses")
		<< tr("Open Next (open as normal next ID screen)") << tr("Open Prev. (open as normal prev. ID screen)") 
		<< tr("Capture and Print") << tr("Capture and Save");

	m_pWindowActionCombobox->addItems(lstActions);
	ui.tableWidget_2->setCellWidget(1, 1, m_pWindowActionCombobox);
}

void COrderEditWgt::Slot_VarBindData()
{

	EditCellWgt *pCellwgt = qobject_cast<EditCellWgt*>(sender());
	Q_ASSERT(pCellwgt);
	if (pCellwgt == nullptr)
	{
		return;
	}

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
		QString strBindData = QString(strNodeTagname.c_str()) + "." + QString(strTagname.c_str()) + "." + QString(strFilename.c_str());
		pCellwgt->SetVarData(strBindData);
	}
	else
	{
		pCellwgt->SetVarData("");
	}
}

void COrderEditWgt::Slot_ChooseWindow()
{
	CBrowseScreenWgt *pBrowseWgt = new CBrowseScreenWgt;
	QString strFilename = GetModuleApi()->GetGraphicPath();

	strFilename.remove(strFilename.length() - 1, 1);
	strFilename.remove( "/" + strFilename.split("/").last());

	strFilename = strFilename + "/" + strFilename.split("/").last() + ".proj";


	pBrowseWgt->LoadWindowData(strFilename);
	if (pBrowseWgt->exec())
	{
		//TODO
	}
	pBrowseWgt->deleteLater();
}

void COrderEditWgt::Slot_OrderConform()
{
	//确认

	//判断当前页面
	if (ui.tabWidget->currentIndex() == 0)
	{
		//基础
		if (ui.tabWidget_2->currentIndex() == 0)
		{
			//变量
			m_pEventItem = new CDyncVarOpEventItem;
		}
		else if (ui.tabWidget_2->currentIndex() == 1)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 2)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 3)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 4)
		{
		}
		else if (ui.tabWidget_2->currentIndex() == 5)
		{
		}
	}
	else if (ui.tabWidget->currentIndex() == 1)
	{
		//操作
		if (ui.tabWidget_3->currentIndex() == 0)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
		else if (ui.tabWidget_3->currentIndex() == 1)
		{

		}
	}
	accept();
}
