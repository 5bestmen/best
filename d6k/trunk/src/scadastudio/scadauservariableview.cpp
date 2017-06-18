#include "scadauservariableview.h"
#include "scadauservariabletable.h"
#include "scadauservariablemodel.h"
#include "sortfiltermodel.h"

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

CScadaUserVariableView::CScadaUserVariableView(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pScadaVirableConfig
	, Config::CScadaUserVariableGroup *pGroup, Config::CScadaData *pScada)
	: QMainWindow(parent), m_pTable(nullptr), m_pCore(nullptr), m_pScadaVirableConfig(nullptr), m_pGroup(nullptr), m_pScada(nullptr)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	Q_ASSERT(pScadaVirableConfig);
	if (!pScadaVirableConfig)
	{
		return;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}

	m_pCore = pCore;
	m_pScadaVirableConfig = pScadaVirableConfig;
	m_pGroup = pGroup;
	m_pScada = pScada;

	setupUi(this);
	connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(FilterChanged(QString)));
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterColumnChanged(int)));
}

CScadaUserVariableView::~CScadaUserVariableView()
{
	if (m_pTable)
	{
		delete m_pTable;
		m_pTable = nullptr;
	}
}

/*! \fn void CScadaUserVariableView::Delete()
********************************************************************************************************* 
** \brief CScadaUserVariableView::Delete 
** \details 
** \return void 
** \author gw
** \date 2016年12月28日 
** \note 
********************************************************************************************************/
void CScadaUserVariableView::Delete()
{
	//m_pTable->GetSortModel()->setSourceModel(nullptr);
	m_pTable->setModel(nullptr);
	//m_pTable->setParent(nullptr);
}

/*! \fn void CScadaUserVariableView::setupUi(QMainWindow *CUserVariablView)
********************************************************************************************************* 
** \brief CScadaUserVariableView::setupUi 
** \details ui文件格局代码
** \param CUserVariablView 
** \return void 
** \author gw
** \date 2016年12月28日 
** \note 
********************************************************************************************************/
void CScadaUserVariableView::setupUi(QMainWindow *CScadaUserVariableView)
{
	if (CScadaUserVariableView->objectName().isEmpty())
		CScadaUserVariableView->setObjectName(QStringLiteral("CScadaUserVariableView"));
	CScadaUserVariableView->resize(1218, 874);
	centralWidget = new QWidget(CScadaUserVariableView);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	gridLayout = new QGridLayout(centralWidget);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(11, 11, 11, 11);
	gridLayout->setObjectName(QStringLiteral("gridLayout"));
	label = new QLabel(centralWidget);
	label->setObjectName(QStringLiteral("label"));

	gridLayout->addWidget(label, 0, 0, 1, 1);

	m_pTable = new CScadaUserVariableTable(this, m_pCore, m_pScadaVirableConfig, m_pGroup, m_pScada);

	comboBox = new QComboBox(centralWidget);
	comboBox->setObjectName(QStringLiteral("comboBox"));
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
	comboBox->setSizePolicy(sizePolicy);
	comboBox->addItems(m_pTable->GetModel()->GetColumnList());

	gridLayout->addWidget(comboBox, 0, 1, 1, 1);

	label_2 = new QLabel(centralWidget);
	label_2->setObjectName(QStringLiteral("label_2"));

	gridLayout->addWidget(label_2, 0, 2, 1, 1);

	lineEdit = new QLineEdit(centralWidget);
	lineEdit->setObjectName(QStringLiteral("lineEdit"));

	gridLayout->addWidget(lineEdit, 0, 3, 1, 1);

	horizontalSpacer_2 = new QSpacerItem(455, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_2, 0, 4, 1, 1);

	gridLayout->addWidget(m_pTable, 1, 0, 1, 5);

	CScadaUserVariableView->setCentralWidget(centralWidget);
	menuBar = new QMenuBar(CScadaUserVariableView);
	menuBar->setObjectName(QStringLiteral("menuBar"));
	menuBar->setGeometry(QRect(0, 0, 1218, 23));
	CScadaUserVariableView->setMenuBar(menuBar);
	mainToolBar = new QToolBar(CScadaUserVariableView);
	mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
	CScadaUserVariableView->addToolBar(Qt::TopToolBarArea, mainToolBar);
	statusBar = new QStatusBar(CScadaUserVariableView);
	statusBar->setObjectName(QStringLiteral("statusBar"));
	CScadaUserVariableView->setStatusBar(statusBar);
}

void CScadaUserVariableView::filterColumnChanged(int column)
{
	m_pTable->GetSortModel()->setFilterKeyColumn(column);
}

void CScadaUserVariableView::FilterChanged(QString text)
{
	QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax::FixedString;
	QRegExp regExp(text, Qt::CaseInsensitive, syntax);
	m_pTable->GetSortModel()->setFilterRegExp(regExp);
}

void CScadaUserVariableView::Refresh()
{
	m_pTable->Refresh();
}