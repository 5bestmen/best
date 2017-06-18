#include "totalselectdialog.h"
#include "scadastudiodefine.h"
#include "stl_util-inl.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"

#include <QSplitter>
#include <QMainWindow>
#include <QBoxLayout>

extern IMainModuleInterface *s_pGlobleCore;

CTotalSelectDialog::CTotalSelectDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.listWidget->resize(100, 200);

	m_arrWidgets.clear();

	//QHBoxLayout *pLayout = new QHBoxLayout(this);

	//m_pMain = new QMainWindow(this);

	//QSplitter *pSplitter = new QSplitter(this);

	//pSplitter->addWidget(ui.listWidget);
	//pSplitter->addWidget(m_pMain);

	//pLayout->addWidget(pSplitter);

	//setLayout(pLayout);

	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(SelectModule(QListWidgetItem *)));
}

CTotalSelectDialog::~CTotalSelectDialog()
{
	STLDeleteElements(&m_arrWidgets);
}

void CTotalSelectDialog::closeEvent(QCloseEvent *event)
{	
	this->deleteLater();
	QDialog::closeEvent(event);
}

/*! \fn void CTotalSelectDialog::AddWidget(QWidget *pWidget, QString strModuleName)
********************************************************************************************************* 
** \brief CTotalSelectDialog::AddWidget 
** \details 模块获得dialog,添加到选点对话框
** \param pWidget 
** \param strModuleName 
** \return void 
** \author gw
** \date 2016年9月30日 
** \note 
********************************************************************************************************/
void CTotalSelectDialog::AddWidget(QWidget *pWidget, QString strModuleName)
{
	ui.listWidget->addItem(new QListWidgetItem(QIcon(FES_PNG), strModuleName));
	ui.listWidget->addItem(new QListWidgetItem(QIcon(QObject::tr("images/hk.png")), QObject::tr("Hong Kong")));
	ui.listWidget->addItem(new QListWidgetItem(QIcon(QObject::tr("images/macau.png")), QObject::tr("Macau")));

	ui.stackedWidget->addWidget(pWidget);
	ui.stackedWidget->setCurrentWidget(pWidget);
	auto pTest = new QWidget;
	ui.stackedWidget->addWidget(pTest);
	//pWidget->show();
	//ui.stackedWidget->show();


	m_arrWidgets.insert(strModuleName, pWidget);
	m_arrWidgets.insert("tt", pTest);
}

/*! \fn void CTotalSelectDialog::SelectModule(QListWidgetItem *item)
********************************************************************************************************* 
** \brief CTotalSelectDialog::SelectModule 
** \details 选择模块
** \param item 
** \return void 
** \author gw
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
void CTotalSelectDialog::SelectModule(QListWidgetItem *item)
{
	auto strModule = item->text();
	if (m_arrWidgets.contains(strModule))
	{
		ui.stackedWidget->setCurrentWidget(m_arrWidgets[strModule]);
	}
	else
	{
		auto strError = QObject::tr("item is not exist!!!");
		s_pGlobleCore->LogMsg(QObject::tr("Select Point Module").toStdString().c_str(),
			strError.toStdString().c_str(), LEVEL_1);

		ui.stackedWidget->setCurrentWidget(m_arrWidgets["tt"]);

		return;
	}
}