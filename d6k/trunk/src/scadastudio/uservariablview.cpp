#include "uservariablview.h"
#include "uservariabletable.h"
#include "uservariablemodle.h"
#include "sortfiltermodel.h"


#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

CUserVariablView::CUserVariablView(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData, Config::CUserVariableGroup *pVariable)
	: QMainWindow(parent), m_pTable(nullptr), m_pCore(nullptr), m_pVariable(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pVariable);

	if (!pCore)
	{
		return;
	}
	
	if (!pVariable)
	{
		return;
	}

	m_pCore = pCore;
	m_pVariable = pVariable;

	m_pTable = new CUserVariableTable(this, m_pCore, pFesData, pVariable);

	setupUi(this);

	connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(FilterChanged(QString)));
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterColumnChanged(int)));
}

CUserVariablView::~CUserVariablView()
{

}

void CUserVariablView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CUserVariablView::setupUi(QMainWindow *CUserVariablView)
{
	if (CUserVariablView->objectName().isEmpty())
		CUserVariablView->setObjectName(QStringLiteral("CUserVariablView"));
	CUserVariablView->resize(643, 622);
	centralWidget = new QWidget(CUserVariablView);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	gridLayout = new QGridLayout(centralWidget);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(11, 11, 11, 11);
	gridLayout->setObjectName(QStringLiteral("gridLayout"));
	label_2 = new QLabel(centralWidget);
	label_2->setText(tr("filter column"));
	label_2->setObjectName(QStringLiteral("label_2"));

	gridLayout->addWidget(label_2, 0, 0, 1, 1);

	comboBox = new QComboBox(centralWidget);
	comboBox->setObjectName(QStringLiteral("comboBox"));
	comboBox->addItems(m_pTable->GetModel()->GetColumnList());
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
	comboBox->setSizePolicy(sizePolicy);
	
	gridLayout->addWidget(comboBox, 0, 1, 1, 1);

	label = new QLabel(centralWidget);
	label->setText(tr("filter string"));
	label->setObjectName(QStringLiteral("label"));

	gridLayout->addWidget(label, 0, 2, 1, 1);

	lineEdit = new QLineEdit(centralWidget);
	lineEdit->setObjectName(QStringLiteral("lineEdit"));

	gridLayout->addWidget(lineEdit, 0, 3, 1, 1);

	gridLayout->addWidget(m_pTable, 1, 0, 1, 5);

	horizontalSpacer_2 = new QSpacerItem(145, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_2, 0, 4, 1, 1);

	CUserVariablView->setCentralWidget(centralWidget);
	menuBar = new QMenuBar(CUserVariablView);
	menuBar->setObjectName(QStringLiteral("menuBar"));
	menuBar->setGeometry(QRect(0, 0, 643, 23));
	CUserVariablView->setMenuBar(menuBar);
	mainToolBar = new QToolBar(CUserVariablView);
	mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
	CUserVariablView->addToolBar(Qt::TopToolBarArea, mainToolBar);
	//statusBar = new QStatusBar(CUserVariablView);
	//statusBar->setObjectName(QStringLiteral("statusBar"));
	//CUserVariablView->setStatusBar(statusBar);
}

/*! \fn void CUserVariablView::FilterChanged(QString text)
********************************************************************************************************* 
** \brief CUserVariablView::FilterChanged 
** \details 过滤字符串
** \param text 
** \return void 
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CUserVariablView::FilterChanged(QString text)
{
	QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax::FixedString;
	QRegExp regExp(text, Qt::CaseInsensitive, syntax);
	m_pTable->GetSortModel()->setFilterRegExp(regExp);
}

/*! \fn void CUserVariablView::filterColumnChanged(int column)
********************************************************************************************************* 
** \brief CUserVariablView::filterColumnChanged 
** \details 过滤列
** \param column 
** \return void 
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CUserVariablView::filterColumnChanged(int column)
{
	m_pTable->GetSortModel()->setFilterKeyColumn(column);
}