
/*
#if _MSC_VER >= 1600
//#define ENABLE_VLD
#ifdef ENABLE_VLD
#include <vld.h>
#endif
#endif*/
#include <QStandardItem>
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QTableWidget>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QToolBar>
#include <QFontDatabase>
#include <QFontComboBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextBrowser>
#include <QPrintPreviewDialog>
#include <QDesktopServices>
#include <QTranslator>
#include "report.h"
#include "cwidget.h"
#include "scadastudio/uimgr.h"
#include "scadastudio/iprojmgr.h"
#include "scadastudio/icore.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"
#include "reportdefine.h"
#include "createreport.h"
#include "cwidget.h"
#include "ctree.h"
#include "aqp/aqp.hpp"
#include "public/dcolorbutton.h"
#include "public/dcolorpanel.h"
#include "calformula.h"
#include "configinfo.h"
#include "ctrlparadialog.h"

static CReportAPI *s_ptrModule = nullptr;

#define  DEFAULT_PATH  "../project/scadastudio/report/"

extern "C" SCADA_EXPORT  IModule *CreateModule()
{
	s_ptrModule = new CReportAPI;
	return s_ptrModule;
}

/*! \fn extern "C" SCADA_EXPORT void DestroyModule()
*********************************************************************************************************
** \brief DestroyModule
** \details ������Դ
** \return extern " SCADA_EXPORT void
** \author LiJin
** \date 2016��8��18��
** \note
********************************************************************************************************/
extern "C" SCADA_EXPORT void DestroyModule()
{
	if (s_ptrModule)
	{
 		delete s_ptrModule;
 		s_ptrModule = nullptr;
	}
}


CReportAPI *GetModuleApi()
{
	return s_ptrModule;
}

IMainWindow *GetMainWindow()
{
	return s_ptrModule->GetCoreUI();
}

/*! \fn  CReportAPI::CReportAPI()
*********************************************************************************************************
** \brief CReportAPI::CReportAPI
** \details ���캯��
** \return
** \author xingzhibing
** \date 2016��4��11��
** \note
********************************************************************************************************/
CReportAPI::CReportAPI()
{
	m_pCore = NULL;
	m_pTopItem = NULL;
	m_bDirtyFlag = false;
	m_bIsInitView = false;
	m_szProjectPath = QString::null;

	m_pRptData = std::make_shared<CRptFileMgr>();

	QTranslator *qtPlugini = new QTranslator();
	QString strRunPath = qApp->applicationDirPath();

	if (qtPlugini->load(strRunPath + "/locales/report_zh_CN.qm"))
	{
		qApp->installTranslator(qtPlugini);
	}
}
/*! \fn CReportAPI::~CReportAPI()
*********************************************************************************************************
** \brief CReportAPI::~CReportAPI
** \details ��������
** \return
** \author xingzhibing
** \date 2016��4��11��
** \note
********************************************************************************************************/
CReportAPI::~CReportAPI()
{
	if (m_pTree)
	{
		m_pTree->deleteLater();
	}
	if (m_pTabWidget)
	{
		m_pTabWidget->deleteLater();
	}
}
/*! \fn void CReportAPI::Init(IMainModuleInterface * pUi)
*********************************************************************************************************
** \brief CReportAPI::Init
** \details ��ʼ��������
** \param pUi
** \return void
** \author xingzhibing
** \date 2016��4��11��
** \note
********************************************************************************************************/

void CReportAPI::Init(IMainModuleInterface * pCore)
{
	Q_ASSERT(pCore);
	if (pCore == nullptr)
		return;
	m_pCore = pCore;


	m_pUi = pCore->GetUIMgr();

	//��ʼ�����ؼ�
	CTreeWidget *pTree = m_pUi->GetLeftTree();

	m_pTree = new CTree(pTree, m_pUi, m_pCore);

	m_pTree->InitTreeModel();

	//��ʼ��TAB�ؼ�
	CTabWidget *m_Table = m_pUi->GetTabWidget();

	m_pTabWidget = new CWidget(m_Table, m_pCore);

	InitMenuBar();

	InitToolBar();

	CreateConnections();

	slot_IfInit(false);
}
/*! \fn void CReportAPI::UnInit()
*********************************************************************************************************
** \brief CReportAPI::UnInit
** \details
** \return void
** \author xingzhibing
** \date 2016��4��11��
** \note
********************************************************************************************************/
void CReportAPI::UnInit()
{

}
/*! \fn bool CReportAPI::SaveProject(QDomDocument *pXml)
*********************************************************************************************************
** \brief CReportAPI::SaveProject
** \details ���ݱ������ļ�
** \param pXml
** \return bool
** \author xingzhibing
** \date 2016��4��11��
** \note
********************************************************************************************************/
bool CReportAPI::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);

	QDomDocument *pDom = m_pCore->GetProjMgr()->GetDocument();

	if (!pDom)
	{
		m_pCore->LogMsg(REPORT_DES, (char *)QStringLiteral("���ļ�ʧ��!!!").toStdString().c_str(), 1);

		return false;
	}

	QDomElement nRoot = pDom->documentElement();

	//�ҵ�report�ڵ㣬�Դ˽ڵ���б������
	for (QDomNode nNode = nRoot.firstChild(); !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement nElement = nNode.toElement();

		if (nElement.tagName().compare("report", Qt::CaseInsensitive) == 0)
		{
			//����ǰ���֮ǰ�ļ�
			while (nElement.hasChildNodes())
			{
				QDomNodeList nList = nElement.childNodes();

				for (int i = 0; i < nList.count(); ++i)
				{
					QDomNode nNode = nList.at(i);
					nElement.removeChild(nNode);
				}
			}	
			CRptFileGrp *pRptGrp = m_pRptData->GetRptGrp(tr("report"));
			if (pRptGrp==Q_NULLPTR)
			{
				return false;
			}
			SaveRptGrpsToProject(pDom, nElement, pRptGrp,true);
		}
	}
	bool bRet = false;
	// ���������ļ�
	if (m_pRptData)
	{
		bRet |=  m_pRptData->SaveFiles();
	}


	return bRet;
}

/*! \fn bool CReportAPI::LoadCursorFromFile(QDomDocument *pXml)
*********************************************************************************************************
** \brief CReportAPI::LoadCursorFromFile
** \details ���ļ���������
** \param pXml
** \return bool
** \author xingzhibing
** \date 2016��4��11��
** \note
********************************************************************************************************/
bool CReportAPI::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_ASSERT(pXml);

	SetProjectPath(szRoot);

	QDomElement root = pXml->documentElement();

	if (root.tagName() != "Project")
	{
		return false;
	}

	QDomNode nChild = root.firstChild();
	QString szRptRootPath = szRoot + "/report/";

	for (; !nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement n = nChild.toElement();
		if (n.nodeName() == "report")
		{
			//�½�����ڵ�
			CQuoteItem* pTop = m_pUi->GetTopItem();
			QString strName = tr("report");
			m_pTopItem = new CQuoteItem(strName);
			m_pTopItem->setData(REPORT_ROOT, Qt::UserRole);
			m_pTopItem->setIcon(QIcon(":/images/report.ico"));

			pTop->appendRow(m_pTopItem);

			//����ROOT�ļ���
			auto* pGrp=m_pRptData->CreateRptGrp(strName);
			
			long long lRet = reinterpret_cast<long long>(pGrp);

			m_pTopItem->setData(lRet, REPORT_USER_DATA_ROLE);
			//���ر���ڵ�
			LoadReportNode(n, m_pTopItem, pGrp, szRptRootPath);
		}
	}
	return true;
}
// �رչ����ļ�
void CReportAPI::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	// �ȹر� �Լ�ģ�����������н���
	int nCount = m_pUi->GetTabWidget()->count();
	CReportEditView* pChildWidget = Q_NULLPTR;
	for (int i = 0; i < nCount;++i)
	{		
		QWidget* pWidget = m_pUi->GetTabWidget()->widget(i);
		pChildWidget = dynamic_cast<CReportEditView*>(pWidget);
		if (pWidget)
		{
			m_pUi->GetTabWidget()->removeTab(i);
		}
	}
	// �����ڴ�
	if (m_pRptData)
	{
		m_pRptData->Close();
	}	
}
// �½�һ���յĹ����ļ�
void CReportAPI::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{

	QDir dir;
	bool ok = dir.mkdir(szRoot + "/" + "report");
	if (!ok)
	{
		auto strError = QString(tr("%1/report folder mkdir failed!!!")).arg(szRoot);

		m_pCore->LogMsg(REPORT_DES, (char *)strError.toStdString().c_str(), LEVEL_1);
	}

	if (m_pRptData)
	{
		m_pRptData.reset();
		m_pRptData = std::make_shared<CRptFileMgr>();
	}

	// ��ʼ��
	QDomElement root = pXml->createElement("report");
	pRoot->appendChild(root);

	CQuoteItem *pRootItem = new CQuoteItem(tr("report"));
	pRootItem->setData(REPORT_ROOT, Qt::UserRole);
	pRootItem->setIcon(QIcon(":/images/report.ico"));
	pRootItem->setEditable(false);

	CRptFileGrp* pRootTag = m_pRptData->CreateRptGrp(tr("report"));

	long long lRet = reinterpret_cast<long long>(pRootTag);

	pRootItem->setData(lRet, REPORT_USER_DATA_ROLE);

	pTopItem->appendRow(pRootItem);


	SetProjectPath(szRoot);

}

/*! \fn void CReportAPI::InitMenuBar()
*********************************************************************************************************
** \brief CReportAPI::InitMenuBar
** \details ��ʼ���˵���
** \return void
** \author xingzhibing
** \date 2016��7��20��
** \note
********************************************************************************************************/
void CReportAPI::InitMenuBar()
{
//!�ļ��˵�
	QMenuBar* m_MenuBar = m_pUi->GetMainMenuBar();
	//m_MenuBar->clear();

	m_pFileMenu = new QMenu(tr("&File"), m_pUi);

	m_pFileNew = new QAction(tr("N&ew"), m_pFileMenu);
	m_pFileNew->setShortcut(QKeySequence::New);
	m_pFileNew->setIcon(QIcon(":/images/new.png"));
	connect(m_pFileNew, SIGNAL(triggered()), this, SLOT(slot_OnNewFile()));

	m_pFileOpen = new QAction(tr("O&pen"), m_pFileMenu);
	m_pFileOpen->setShortcut(QKeySequence::Open);
	m_pFileOpen->setIcon(QIcon(":/images/open.png"));
	QObject::connect(m_pFileOpen, SIGNAL(triggered()), this, SLOT(slot_OnOpenFile()));

	m_pFileSave = new QAction(tr("S&ave"), m_pFileMenu);
	m_pFileSave->setShortcut(QKeySequence::Save);
	m_pFileSave->setIcon(QIcon(":/images/save.png"));
	QObject::connect(m_pFileSave, SIGNAL(triggered()), this, SLOT(slot_OnSaveFile()));

	m_pFileSaveAs = new QAction(tr("S&aveAs"), m_pFileMenu);
	m_pFileSaveAs->setShortcut(QKeySequence::SaveAs);
	m_pFileSaveAs->setIcon(QIcon(":/images/save_as.png"));
	QObject::connect(m_pFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_OnSaveAsFile()));

	m_pFileExport = new QAction(tr("E&xport"), m_pFileMenu);
	QObject::connect(m_pFileExport, SIGNAL(triggered()), this, SLOT(slot_OnExport()));

	m_pFilePdf = new QAction(tr("P&dfExport"), m_pFileMenu);
	m_pFilePdf->setIcon(QIcon(":/images/exportpdf.png"));
	QObject::connect(m_pFilePdf, SIGNAL(triggered()), this, SLOT(slot_OnPdfExport()));

	m_pFilePrint = new QAction(tr("Pr&int"), m_pFileMenu);
	m_pFilePrint->setShortcut(QKeySequence::Print);
	m_pFilePrint->setIcon(QIcon(":/images/print.png"));
	QObject::connect(m_pFilePrint, SIGNAL(triggered()), this, SLOT(slot_OnPrintFile()));

	m_pFilePrintOverView = new QAction(tr("PreV&iew"), m_pFileMenu);
	m_pFilePrintOverView->setIcon(QIcon(":/images/preview.png"));
	QObject::connect(m_pFilePrintOverView, SIGNAL(triggered()), this, SLOT(slot_OnPreViewFile()));

	m_pFileClose = new QAction(tr("C&lose"), m_pFileMenu);
	m_pFileClose->setShortcut(QKeySequence::Close);
	m_pFileClose->setIcon(QIcon(":/images/no.png"));
	QObject::connect(m_pFileClose, SIGNAL(triggered()), this, SLOT(slot_OnCloseFile()));

	m_pFileMenu->addAction(m_pFileNew);
	m_pFileMenu->addAction(m_pFileOpen);
	m_pFileMenu->addAction(m_pFileSave);
	m_pFileMenu->addAction(m_pFileSaveAs);
	m_pFileMenu->addAction(m_pFileExport);
	m_pFileMenu->addAction(m_pFilePdf);
	m_pFileMenu->addAction(m_pFilePrint);
	m_pFileMenu->addAction(m_pFilePrintOverView);
	m_pFileMenu->addAction(m_pFileClose);

	m_MenuBar->addMenu(m_pFileMenu);
//���༭�˵�
	m_pEdit = new QMenu(tr("&Edit"), m_pUi);


	m_CutAction = new QAction(tr("&Cut"), m_pEdit);
	m_CutAction->setShortcut(QKeySequence::Cut);
	m_CutAction->setIcon(QIcon(":/images/cut.png"));

	m_CopyAction = new QAction(tr("&Copy"), m_pEdit);
	m_CopyAction->setShortcut(QKeySequence::Copy);
	m_CopyAction->setIcon(QIcon(":/images/copy.png"));

	m_PasteAction = new QAction(tr("&Paste"), m_pEdit);
	m_PasteAction->setShortcut(QKeySequence::Paste);
	m_PasteAction->setIcon(QIcon(":/images/paste.png"));

	m_DeleteAction = new QAction(tr("&Delete"), m_pEdit);
	m_DeleteAction->setShortcut(QKeySequence::Delete);
	m_DeleteAction->setIcon(QIcon(":/images/delete_report_file.ico"));

	m_SelectSubMenu = new QMenu(tr("&Select"), m_pUi);
	m_SelectSubMenu->setIcon(QIcon(":/images/report_pie.ico"));
	m_SelectAllAction = new QAction(tr("&SelectAll"), m_SelectSubMenu);
	m_SelectRowAction = new QAction(tr("&SelectRow"), m_SelectSubMenu);
	m_SelectColumnAction = new QAction(tr("&SelectColumn"), m_SelectSubMenu);

	m_SelectSubMenu->addAction(m_SelectAllAction);
	m_SelectSubMenu->addAction(m_SelectRowAction);
	m_SelectSubMenu->addAction(m_SelectColumnAction);

	m_pEdit->addAction(m_CutAction);
	m_pEdit->addAction(m_CopyAction);
	m_pEdit->addAction(m_PasteAction);
	m_pEdit->addAction(m_DeleteAction);
	m_pEdit->addMenu(m_SelectSubMenu);

	m_MenuBar->addMenu(m_pEdit);

//!�����˵�
	m_OperateMenu = new QMenu(tr("&Operate"), m_pUi);
	m_MergeAction = new QAction(tr("&Merge"), m_OperateMenu);
	m_MergeAction->setIcon(QIcon(":/images/span_cell.png"));

	m_UnMergeAction = new QAction(tr("&UnMerge"), m_OperateMenu);
	m_UnMergeAction->setIcon(QIcon(":/images/split_cell.png"));

	m_SetFont = new QAction(tr("&Font"), m_OperateMenu);
	m_SetFont->setIcon(QIcon(":/images/font.png"));

	m_FontColor = new QAction(tr("&FontColor"), m_OperateMenu);
	m_FontColor->setIcon(QIcon(":/images/cell_print.png"));

	m_PlaceSubMenu = new QMenu(tr("Align"), m_pUi);
	m_PlaceSubMenu->setIcon(QIcon(":/images/report_custom.ico"));

	m_LeftAlign = new QAction(tr("&Left"), m_PlaceSubMenu);
	m_LeftAlign->setIcon(QIcon(":/images/ali_left.png"));

	m_MiddleAlign = new QAction(tr("&Middle"), m_PlaceSubMenu);
	m_MiddleAlign->setIcon(QIcon(":/images/ali_center.png"));

	m_RightAlign = new QAction(tr("&Right"), m_PlaceSubMenu);
	m_RightAlign->setIcon(QIcon(":/images/ali_right.png"));

	m_SetFont->setIcon(QIcon(":/images/font.png"));
	m_FontColor->setIcon(QIcon(":/images/brush_color.png"));

	m_PlaceSubMenu->addAction(m_LeftAlign);
	m_PlaceSubMenu->addAction(m_MiddleAlign);
	m_PlaceSubMenu->addAction(m_RightAlign);

	m_OperateMenu->addAction(m_MergeAction);
	m_OperateMenu->addAction(m_UnMergeAction);
	m_OperateMenu->addAction(m_SetFont);
	m_OperateMenu->addAction(m_FontColor);
	m_OperateMenu->addMenu(m_PlaceSubMenu);

	m_MenuBar->addMenu(m_OperateMenu);
//!ѡ��ѡ��
	m_pOptions = new QMenu(tr("&Options"), m_pUi);
	m_SelectPoints = new QAction(tr("&MultiPoints"), m_pOptions);
	m_SelectPoints->setIcon(QIcon(":/images/moredata.png"));

	m_SelectSignalPoint = new QAction(tr("&SignalPoint"), m_pOptions);
	m_SelectSignalPoint->setIcon(QIcon(":/images/singledata.png"));

	m_SetMyFormula = new QAction(tr("&Formula"), m_pOptions);
	m_SetMyFormula->setIcon(QIcon(":/images/function.png"));

	m_pOptions->addAction(m_SelectPoints);
	m_pOptions->addAction(m_SelectSignalPoint);
	m_pOptions->addAction(m_SetMyFormula);

	m_MenuBar->addMenu(m_pOptions);

//�Ҽ��˵���
	m_pCustomMenu = new QMenu( m_pUi);
	m_ConnForGround = new QAction(tr("ConData"),m_pCustomMenu);
	m_delForGround = new QAction(tr("DelData"),m_pCustomMenu);

	m_delRow = new QAction(tr("DelRow"),m_pCustomMenu);
	m_delRow->setIcon(QIcon(":/images/del_row.png"));

	m_delCol = new QAction(tr("DelCol"),m_pCustomMenu);
	m_delCol->setIcon(QIcon(":/images/del_col.png"));

	m_AddRow = new QAction(tr("AddRow"),m_pCustomMenu);
	m_AddRow->setIcon(QIcon(":/images/add_row.png"));

	m_AddCol = new QAction(tr("AddCol"),m_pCustomMenu);
	m_AddCol->setIcon(QIcon(":/images/add_col.png"));

	m_SetReportTime = new QAction(tr("SetTime"),m_pCustomMenu);
	m_TimeCol = new QAction(tr("TimeCol"),m_pCustomMenu);

	m_pCustomMenu->addAction(m_ConnForGround);
	m_pCustomMenu->addAction(m_delForGround);
	m_pCustomMenu->addAction(m_delRow);
	m_pCustomMenu->addAction(m_delCol);
	m_pCustomMenu->addAction(m_AddRow);
	m_pCustomMenu->addAction(m_AddCol);
	m_pCustomMenu->addAction(m_SetReportTime);
	m_pCustomMenu->addAction(m_TimeCol);

	m_Color = new DColorButton(m_pUi);
	m_ColorPanel = new DColorPanel(m_pUi);
	m_Color->setIcon(QIcon(":/images/font.png"));
	m_Color->setPopupMode(QToolButton::MenuButtonPopup);
	m_Color->setMenu(m_ColorPanel);

	m_Brush = new DColorButton(m_pUi);
	m_BrushPanel = new DColorPanel(m_pUi);
	m_Brush->setIcon(QIcon(":/images/brush_color.png"));
	m_Brush->setPopupMode(QToolButton::MenuButtonPopup);
	m_Brush->setMenu(m_BrushPanel);

	m_Bold = new QAction(tr("Bold"), m_pUi);
	m_Bold->setIcon(QIcon(":/images/font_bold.png"));

	m_Itilic = new QAction(tr("Itilic"), m_pUi);
	m_Itilic->setIcon(QIcon(":/images/font_italic.png"));

	m_Underline = new QAction(tr("UnderLine"), m_pUi);
	m_Underline->setIcon(QIcon(":/images/font_underline.png"));

	m_Fontsize = new QComboBox(m_pUi);
	foreach(int size, QFontDatabase::standardSizes())
	{
		m_Fontsize->addItem(QVariant(size).toString());
	}
	m_FontCombo = new QFontComboBox(m_pUi);
	m_FontCombo->setFontFilters(QFontComboBox::ScalableFonts);
	m_FontCombo->setFixedWidth(100);

	m_BarAction = new QAction(tr("Bar"));
	m_BarAction->setIcon(QIcon(":/images/report_bar.ico"));

	m_PieAction = new QAction(tr("Pie"));
	m_PieAction->setIcon(QIcon(":/images/report_pie.ico"));

	m_CurveAction = new QAction(tr("Chart"));
	m_CurveAction->setIcon(QIcon(":/images/report_curve.ico"));

}

/*! \fn void CReportAPI::CreateConnections()
*********************************************************************************************************
** \brief CReportAPI::CreateConnections
** \details �����źŲ�����
** \return void
** \author xingzhibing
** \date 2016��7��20��
** \note
********************************************************************************************************/
void CReportAPI::CreateConnections()
{
//tree
	connect(m_pTree, SIGNAL(sig_AddReportFile(QModelIndex&)), this, SLOT(slot_OnAddReportFile(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_DelReportFile(QModelIndex&)), this, SLOT(slot_OnDelReportFile(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_ModReportName(QModelIndex&)), this, SLOT(slot_OnModReportName(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_ModReportType(QModelIndex&)), this, SLOT(slot_OnModReportType(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_ImpReportFile(QModelIndex&)), this, SLOT(slot_OnImpReportFile(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_OpenReportFile(QModelIndex&)), this, SLOT(slot_OnOpenReportFile(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_AddReportGroup(QModelIndex&)), this, SLOT(slot_OnAddReporGrp(QModelIndex&)));
	connect(m_pTree, SIGNAL(sig_ItemChanged(QModelIndex&)), this , SLOT(slot_OnRptItemChanged(QModelIndex& )));
	connect(m_pTree, SIGNAL(sig_IfReportClicked(bool)), this, SLOT(slot_IfInit(bool)));
//menu
	connect(m_FontCombo, SIGNAL(currentIndexChanged(int)),this,SLOT(slot_OnSetFontFamily(int)));
	connect(m_Fontsize, SIGNAL(currentIndexChanged(int)),this,SLOT(slot_OnSetFontSize(int)));
	QObject::connect(m_ColorPanel, SIGNAL(colorChanged(QColor)), m_Color, SLOT(setColor(QColor)));
	QObject::connect(m_ColorPanel, SIGNAL(colorChanged(QColor)), this, SLOT(slot_OnSetColor(const QColor&)));
	QObject::connect(m_BrushPanel, SIGNAL(colorChanged(QColor)), m_Brush, SLOT(setColor(QColor)));
	QObject::connect(m_BrushPanel, SIGNAL(colorChanged(QColor)), this, SLOT(slot_OnSetBgColor(const QColor&)));

	connect(m_Bold, SIGNAL(triggered()), this, SLOT(slot_OnSetFontTextBold()));
	connect(m_Itilic, SIGNAL(triggered()), this, SLOT(slot_OnSetFontTextItalic()));
	connect(m_Underline, SIGNAL(triggered()), this, SLOT(slot_OnSetFontUnderLine()));
//operation	
	connect(m_CopyAction, SIGNAL(triggered()), this,SLOT(slot_OnCopy()));
	connect(m_CutAction, SIGNAL(triggered()), this, SLOT(slot_OnCut()));
	connect(m_PasteAction, SIGNAL(triggered()), this, SLOT(slot_OnPaste()));
	connect(m_MergeAction, SIGNAL(triggered()), this, SLOT(slot_OnMerge()));
	connect(m_UnMergeAction, SIGNAL(triggered()), this, SLOT(slot_OnUnMerge()));
	connect(m_LeftAlign, SIGNAL(triggered()), this, SLOT(slot_OnAlignLeft()));
	connect(m_RightAlign, SIGNAL(triggered()), this, SLOT(slot_OnAlignRight()));
	connect(m_MiddleAlign, SIGNAL(triggered()), this, SLOT(slot_OnAlignCenter()));

	connect(m_SetMyFormula, SIGNAL(triggered()), this, SLOT(slot_OnSetFormula()));

	connect(m_pUi->GetLeftTree()->model(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_OnDataChanged(QStandardItem *)));

	connect(m_BarAction, SIGNAL(triggered()),this,SLOT(slot_OnBar()));
	connect(m_PieAction, SIGNAL(triggered()), this, SLOT(slot_OnPie()));
	connect(m_CurveAction, SIGNAL(triggered()), this, SLOT(slot_OnCurve()));

	connect(m_pTabWidget, SIGNAL(sig_TabClicked(bool )), this, SLOT(slot_IfInit(bool)));
}

/*! \fn void CReportAPI::slot_NewFile()
*********************************************************************************************************
** \brief CReportAPI::slot_NewFile
** \details ���ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnNewFile()
{
	QCreateReport createreport;

	if (createreport.exec() == QDialog::Rejected)
	{
		return;
	}

	CreateTableInfo tempInfo = createreport.GetTableInfo();

}

/*! \fn  void CReportAPI::slot_OpenFile()
*********************************************************************************************************
** \brief CReportAPI::slot_OpenFile
** \details ���ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnOpenFile()
{
	QString nName = QFileDialog::getOpenFileName(NULL, \
		tr("Open Report File"), \
		".", \
		tr("Report files (*.xml)"));
	if (!nName.isEmpty())
	{
		if (!LoadFile(nName))
		{
			m_pCore->LogMsg(REPORT_DES, (char*)tr("load file failed!!!").toStdString().c_str(), LEVEL_1);
		}
	}
}

/*! \fn void CReportAPI::slot_SaveFile()
*********************************************************************************************************
** \brief CReportAPI::slot_SaveFile
** \details �����ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
bool CReportAPI::slot_OnSaveFile()
{
	CReportEditView* pWidget = GetCurrentReportView();
	if (pWidget)
	{
		CRptFile* pFile = pWidget->GetRptFile();
		//�ĵ����Ϊ���޸�
		if (pFile->GetModifyFlag()==true)
		{
			pWidget->GetCellData();
			pFile->WriteXmlFile();
			return true;
		}
		else
		{
			return false;
		}
		/*if (pWidget->GetModifyFlag())
		{
			

			bool nRet = false;

			if (pFile)
			{
				if (pFile->GetModifyFlag()==true)
				{
					pFile->GetData();
				}
				nRet = pFile->WriteXmlFile();
			}
		/ *	int r = QMessageBox::warning(NULL, tr("report"), tr("Document has been Modified\n"" Save ?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (r == QMessageBox::Yes)
			{
				pWidget->Save();
				return true;
			}
			else
			{
				return false;
			}* /
		}*/
	}
	return false;
}

/*! \fn void CReportAPI::slot_SaveAsFile()
*********************************************************************************************************
** \brief CReportAPI::slot_SaveAsFile
** \details ���Ϊ�ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnSaveAsFile()
{
	QString fileName = QFileDialog::getSaveFileName(nullptr,
		tr("Save Spreadsheet"), ".",
		tr("Spreadsheet files (*.*)"));

	if (fileName.isEmpty())
		return ;
	//return saveFile(fileName);
}

/*! \fn void CReportAPI::slot_PrintFile()
*********************************************************************************************************
** \brief CReportAPI::slot_PrintFile
** \details ��ӡ�ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnPrintFile()
{
	CReportEditView* pWidget = GetCurrentReportView();
	if (pWidget)
	{
		QString nHtml = pWidget->ProduceHtml();

		QPrinter printer(QPrinter::HighResolution);
		QPrintDialog printDialog(&printer, Q_NULLPTR);
		if (printDialog.exec()) {
			QTextBrowser *pBrowser = new QTextBrowser;
			pBrowser->setText(nHtml);
			pBrowser->print(&printer);
		}
	}
}

/*! \fn void CReportAPI::slot_PreViewFile()
*********************************************************************************************************
** \brief CReportAPI::slot_PreViewFile
** \details Ԥ���ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnPreViewFile()
{
	QPrinter printer(QPrinter::ScreenResolution);
	QPrintPreviewDialog preview(&printer, Q_NULLPTR);
	preview.setMinimumSize(1200, 1000);
	connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(slot_OnPreViewReport(QPrinter *)));
	preview.exec();
}

/*! \fn void CReportAPI::slot_OnPreViewReport(QPrinter* printer)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnPreViewReport 
** \details Ԥ���ļ�
** \param printer 
** \return void 
** \author xingzhibing
** \date 2016��8��10�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnPreViewReport(QPrinter* printer)
{
	Q_ASSERT(printer);
	CReportEditView* pWidget = GetCurrentReportView();
	if (pWidget)
	{
		QString nHtml = pWidget->ProduceHtml();
		QTextBrowser *pBrowser = new QTextBrowser;
		pBrowser->setText(nHtml);
		pBrowser->print(printer);	
	}
}

/*! \fn void CReportAPI::slot_Export()
*********************************************************************************************************
** \brief CReportAPI::slot_Export
** \details �����ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnExport()
{
	CReportEditView* pWidget = GetCurrentReportView();
	if (pWidget)
	{
		QString nHtml = pWidget->ProduceHtml();
		pWidget->SavePageOrExcel(nHtml);
	}
}

/*! \fn void CReportAPI::slot_PdfExport()
*********************************************************************************************************
** \brief CReportAPI::slot_PdfExport
** \details ����PDF�ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��;
** \note
********************************************************************************************************/
void CReportAPI::slot_OnPdfExport()
{
	CReportEditView* pWidget = GetCurrentReportView();
	if (pWidget)
	{
		QString nHtml = pWidget->ProduceHtml();

		QString filename = DEFAULT_PATH ;
		filename = QFileDialog::getSaveFileName(Q_NULLPTR, "Save File", filename, "Adobe PDF Files (*.pdf)");

		QPrinter  printer;
		printer.setPageSize(QPrinter::A4);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(filename);
	}
}

/*! \fn void CReportAPI::slot_CloseFile()
*********************************************************************************************************
** \brief CReportAPI::slot_CloseFile
** \details �ر��ļ�
** \return void
** \author xingzhibing
** \date 2016��7��21��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnCloseFile()
{

}

/*! \fn void CReportAPI::setWindowModiy()
*********************************************************************************************************
** \brief CReportAPI::setWindowModiy
** \details �ļ��޸�
** \return void
** \author xingzhibing
** \date 2016��7��22��
** \note
********************************************************************************************************/
void CReportAPI::setWindowModiy()
{

}

/*! \fn void CReportAPI::slot_OnAddReportFile(QModelIndex& index)
*********************************************************************************************************
** \brief CReportAPI::slot_OnAddReportFile
** \details ���ӱ����ļ�
** \param index
** \return void
** \author xingzhibing
** \date 2016��7��22��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnAddReportFile(QModelIndex& index)
{
	//��ȡ�������ƺͻ�������
	QCreateReport createreport;
	if (createreport.exec() == QDialog::Rejected)
	{
		return;
	}
	CreateTableInfo tempInfo = createreport.GetTableInfo();

	CTableInfo* pInfo=new CTableInfo;
	pInfo->m_szTableName = tempInfo.tableName;
	pInfo->m_szTablePath = GetProjectPath()+'/' + tempInfo.tableName;
	pInfo->m_nTableType = tempInfo.tableType;
	pInfo->m_RowCount = tempInfo.tableRow;
	pInfo->m_ColCount = tempInfo.tableColumn;

	//��Mgrͳһ����
	auto pCrptFile = m_pRptData->CreateRptFile(pInfo);

	if (pCrptFile == Q_NULLPTR)
	{
		m_pCore->LogMsg(REPORT_DES, (char*)tr("file add failed!!!").toStdString().c_str(), LEVEL_1);
		return;
	}
	pCrptFile->SetModifyFlag(true);
	
	//�½��ļ�
	m_pTabWidget->AddTabCtrl(pCrptFile);

	//���ؼ����
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	
	QIcon nICon = QIcon(":/images/report_file.ico");
	//m_pTree->AddChildItem(nInfo, item, REPORT_ITEM, nICon);
	QString szParentName = item->text();

	CRptFileGrp* pRptGrp = m_pRptData->GetRptGrp(szParentName);

	if (pRptGrp==Q_NULLPTR)
	{
		//log error
		LogMsg(tr("parent node is null!!!").toStdString().c_str(),LEVEL_1);
	}
	else
	{
		pRptGrp->AddRptFile(pCrptFile);
	}

	if (item->data(Qt::WhatsThisRole).toInt() <= MAX_REPORT_GROUP_COUNT)
	{
		QString szName = QFileInfo(pCrptFile->GetRptFileName()).baseName();

		CQuoteItem *newItem = new CQuoteItem(szName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		//Name
		newItem->setData(QVariant(pInfo->m_szTableName), Qt::UserRole + 1);
		//Path
		newItem->setData(QVariant(pInfo->m_szTablePath), Qt::UserRole + 2);

		newItem->setData(reinterpret_cast<long long >(pCrptFile),REPORT_USER_DATA_ROLE);

		newItem->setData(REPORT_ITEM, Qt::UserRole);

		newItem->setIcon(nICon);

		item->appendRow(newItem);
	}
	m_pTree->GetTree()->expanded(index);
}

/*! \fn void CReportAPI::slot_OnDelReportFile(QModelIndex& index)
*********************************************************************************************************
** \brief CReportAPI::slot_OnDelReportFile
** \details ɾ�������ļ�
** \param index
** \return void
** \author xingzhibing
** \date 2016��7��22��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnDelReportFile(QModelIndex& index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//�ڴ��ļ�ɾ��
	CRptFile *pFile = reinterpret_cast < CRptFile* >(item->data(REPORT_USER_DATA_ROLE).toLongLong());

	Q_ASSERT(pFile);
	if (pFile == nullptr)
		return;

	m_pRptData->DelRptFile(pFile);
	
	int nType = item->data(Qt::UserRole).toInt();
	
	//�ļ��б����ļ�ɾ��
	if (nType == REPORT_ITEM)
	{
		QString nPath = item->data(Qt::UserRole + 2).toString();
		QFile nFile(nPath);
		if (nFile.exists())
		{
			bool nRet = QFile::remove(nPath);
			if (!nRet)
			{
				m_pCore->LogMsg(REPORT_DES, (char*)tr("can not remove file!!!").toStdString().c_str(), LEVEL_1);
			}
		}
	
	}
	//������ɾ��
	CQuoteItem *pItem = dynamic_cast<CQuoteItem*>(item->parent());
	if (pItem)
	{
		pItem->removeRow(item->row());
	}
	//Tableɾ��
}

/*! \fn void CReportAPI::slot_OnModReportName(QModelIndex& index)
*********************************************************************************************************
** \brief CReportAPI::slot_OnModReportName
** \details �޸ı�������
** \param index
** \return void
** \author xingzhibing
** \date 2016��7��22��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnModReportName(QModelIndex& index)
{
	Q_UNUSED(index);
}

/*! \fn void CReportAPI::slot_OnModReportType(QModelIndex& index)
*********************************************************************************************************
** \brief CReportAPI::slot_OnModReportType
** \details �޸ı�������
** \param index
** \return void
** \author xingzhibing
** \date 2016��7��22��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnModReportType(QModelIndex& index)
{
	Q_UNUSED(index);
}


/*! \fn bool CReportAPI::SaveFile(QString& fileName)
*********************************************************************************************************
** \brief CReportAPI::SaveFile
** \details
** \param fileName
** \return bool
** \author xingzhibing
** \date 2016��7��26��
** \note
********************************************************************************************************/
bool CReportAPI::SaveFile(QString& fileName)
{
	Q_UNUSED(fileName);
	CReportEditView* pView = GetCurrentReportView();

	Q_ASSERT(pView);

	if (pView)
	{
		CRptFile *pFile = pView->GetRptFile();
	}	
	return false;
}

/*! \fn void CReportAPI::SetCurrentFile(const QString& fileName)
*********************************************************************************************************
** \brief CReportAPI::SetCurrentFile
** \details
** \param fileName
** \return void
** \author xingzhibing
** \date 2016��7��26��
** \note
********************************************************************************************************/
void CReportAPI::SetCurrentFile(const QString& fileName)
{
	Q_UNUSED(fileName);
}

/*! \fn  CReportEditView* CReportAPI::GetCurrentReport()
*********************************************************************************************************
** \brief CReportAPI::GetCurrentReport
** \details ��ȡ��ǰ����
** \return CReportEditView*
** \author xingzhibing
** \date 2016��7��26��
** \note
********************************************************************************************************/
CReportEditView* CReportAPI::GetCurrentReportView()
{
	return m_pTabWidget->GetCurrerntTable();
}

/*! \fn bool CReportAPI::OkToContinue()
*********************************************************************************************************
** \brief CReportAPI::OkToContinue
** \details �ж��ļ��Ƿ��޸�
** \return bool
** \author xingzhibing
** \date 2016��7��27��
** \note
********************************************************************************************************/
bool CReportAPI::OkToContinue()
{
	if (m_pTabWidget->GetCurrerntTable()->isWindowModified())
	{
		int r = QMessageBox::warning(NULL, tr("report"), tr("The Document has been Modified\n""Save?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
		{
			return slot_OnSaveFile();
		}
		else if (r == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

/*! \fn void CReportAPI::SetItemData(int nRow, int nCol, QString nContent)
*********************************************************************************************************
** \brief CReportAPI::SetItemData
** \details ��������ֵ
** \param nRow
** \param nCol
** \param nContent
** \return void
** \author xingzhibing
** \date 2016��7��27��
** \note
********************************************************************************************************/
void CReportAPI::SetItemData(int nRow, int nCol, QString nContent)
{

}

/*! \fn bool CReportAPI::LoadFile(const QString& nName)
*********************************************************************************************************
** \brief CReportAPI::LoadFile
** \details �ļ��ڲ�����
** \param nName
** \return bool
** \author xingzhibing
** \date 2016��7��27��
** \note
********************************************************************************************************/
bool CReportAPI::LoadFile(const QString& nName)
{
	QFileInfo nFileInfo(nName);
	if (!nFileInfo.exists())
	{
		m_pCore->LogMsg(REPORT_DES, (char*)tr("file not exist!!!").toStdString().c_str(), LEVEL_1);
		return false;
	}

	CRptFile *pRptFile = m_pRptData->GetRptFile(nName);

	m_pTabWidget->AddTabCtrl(pRptFile);

	return true;
}

/*! \fn bool CReportAPI::LoadReportNode(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp,const QString & szRootPath)
********************************************************************************************************* 
** \brief CReportAPI::LoadReportNode 
** \details  �ӹ����ļ�����������ã�
** \param pElement 
** \param pItem 
** \param pRptGrp 
** \return bool 
** \author LiJin 
** \date 2016��8��18�� 
** \note Ϊʡ�ڴ濼�ǣ����ѱ����ļ���ȫ����Ϣ���ؽ������������������͵Ȼ�����Ϣ
********************************************************************************************************/
bool CReportAPI::LoadReportNode(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath)
{
	Q_ASSERT(pElement.isNull()==false);	
	Q_ASSERT(pItem);
	Q_ASSERT(pRptGrp);

	QDomNode nNode;
	QDomElement elmData;

	for (nNode = pElement.firstChild(); !nNode.isNull(); nNode = nNode.nextSibling())
	{
		elmData = nNode.toElement();
		qDebug() << elmData.tagName();
		if (elmData.tagName() == "group")
		{
			//�������Ա
			XmlParaseReportGroup(elmData, pItem, pRptGrp,szRootPath);
		}
		if (elmData.tagName() == "file")
		{
			//���������ļ�
			XmlParaseReportFile(elmData, pItem, pRptGrp,szRootPath);
		}
	}
	return true;
}

/*! \fn bool CReportAPI::XmlParaseReportGroup(QDomElement pElement, CQuoteItem* pItem)
*********************************************************************************************************
** \brief CReportAPI::XmlParaseReportGroup
** \details �����������ļ���Ϣ����������
** \param index
** \return bool
** \author xingzhibing
** \date 2016��7��29��
** \note
********************************************************************************************************/
bool CReportAPI::XmlParaseReportGroup(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath)
{
	Q_ASSERT(pElement.isNull() == false);
	Q_ASSERT(pItem);
	Q_ASSERT(pRptGrp);

	CQuoteItem* pNewItem = new CQuoteItem();

	CRptFileGrp *pChildGrp = Q_NULLPTR;

	//���з�����Ϣ ������ʵ�����Ա
	if (!pElement.hasChildNodes())
	{
		QString nGroupName = pElement.attribute("GroupName");
		//����
		pChildGrp = m_pRptData->CreateRptGrp(nGroupName);
		if (pChildGrp == Q_NULLPTR)
		{
			return false;
		}
		pRptGrp->AddRptGrp(pChildGrp);
		//����
		QIcon nIcon(QIcon(":/images/report_day.ico"));
		pNewItem->setIcon(nIcon);
		pNewItem->setText(nGroupName);
		pNewItem->setData(pNewItem->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);
		pNewItem->setData(REPORT_GROUP_ITEM, Qt::UserRole);
		pNewItem->setData(reinterpret_cast<long long>(pRptGrp), REPORT_USER_DATA_ROLE);
		pItem->appendRow(pNewItem);
		
		return true;
	}
	//��ʵ�ʳ�Ա����������
	else
	{
		QString nGroupName = pElement.attribute("GroupName");
		//����
		pChildGrp = m_pRptData->CreateRptGrp(nGroupName);
		if (pChildGrp==Q_NULLPTR)
		{
			return false;
		}
		pRptGrp->AddRptGrp(pChildGrp);
		//����
		QIcon nIcon(QIcon(":/images/report_day.ico"));
		pNewItem->setIcon(nIcon);
		pNewItem->setText(nGroupName);
		pNewItem->setData(pNewItem->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);
		pNewItem->setData(REPORT_GROUP_ITEM, Qt::UserRole);
		pNewItem->setData(reinterpret_cast<long long>(pRptGrp), REPORT_USER_DATA_ROLE);
		pItem->appendRow(pNewItem);
	}

	//�����˵�

	QDomNode pNode;

	for (pNode = pElement.firstChild(); !pNode.isNull(); pNode = pNode.nextSibling())
	{
		QDomElement pEls = pNode.toElement();
		if (pEls.tagName().compare("group") == 0)
		{
			XmlParaseReportGroup(pEls, pNewItem, pChildGrp, szRootPath);
		}
		if (pEls.tagName().compare("file") == 0)
		{
			XmlParaseReportFile(pEls, pNewItem, pChildGrp,szRootPath);
		}
	}
	return true;
}

/*! \fn bool CReportAPI::XmlParaseReportFile(QDomElement pElement, CQuoteItem* pItem)
*********************************************************************************************************
** \brief CReportAPI::XmlParaseReportFile
** \details ���������ļ����ں��ļ����ƣ�·��������
** \param index
** \return bool
** \author xingzhibing
** \date 2016��7��29��
** \note
********************************************************************************************************/
bool CReportAPI::XmlParaseReportFile(QDomElement pElement, CQuoteItem* pItem, CRptFileGrp* pRptGrp, const QString & szRootPath)
{
	Q_ASSERT(pElement.isNull() == false);
	Q_ASSERT(pItem);
	Q_ASSERT(pRptGrp);

	if (pElement.isNull() && pItem && pRptGrp)
	{
		return false;
	}

	//��ȡ�����ļ�����
	QString szFileName;
	int nType;
	szFileName = pElement.attribute("file_name");
	//szPath = pElement.attribute("nfilePath");
	nType = pElement.attribute("report_type").toInt();

	CTableInfo*  info=new CTableInfo;
	info->m_szTableName = szFileName;
	info->m_szTablePath = szRootPath;
	info->m_nTableType = nType;

	//����
	CRptFile *pRptFile = m_pRptData->CreateRptFile(info);
	if (pRptFile)
	{
		pRptGrp->AddRptFile(pRptFile);
	}
	else
	{
		Q_ASSERT(pRptFile);
		return false;
	}

	//����
	QIcon nICon = QIcon(":/images/report_file.ico");

	QString szName = QFileInfo(szFileName).baseName();

	CQuoteItem *pFileItem = new CQuoteItem(szName);
	pFileItem->setIcon(nICon);
	pFileItem->setData(REPORT_ITEM, Qt::UserRole);
	//Name
	pFileItem->setData(QVariant(info->m_szTableName), Qt::UserRole + 1);
	//Path
	pFileItem->setData(QVariant(info->m_szTablePath), Qt::UserRole + 2);
	//Type
	pFileItem->setData(QVariant(info->m_nTableType), Qt::UserRole + 3);
	//�������ý�data
	pFileItem->setData(reinterpret_cast<long long>(pRptFile), REPORT_USER_DATA_ROLE);

	pItem->appendRow(pFileItem);

	return true;
}

/*! \fn void CReportAPI::AddChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon)
*********************************************************************************************************
** \brief CReportAPI::AddChildItem
** \details
** \param str    �ؼ�����
** \param item   ���ؼ���
** \param Role   ������ɫ
** \param Icon   ͼ�����
** \return void
** \author xingzhibing
** \date 2016��8��2��
** \note
********************************************************************************************************/
void CReportAPI::AddChildItem(QString str, CQuoteItem* item, int Role, QIcon Icon)
{
	Q_ASSERT(item);

	QString strName = str;

	if (item->data(Qt::WhatsThisRole).toInt() < MAX_REPORT_GROUP_COUNT)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(Role, Qt::UserRole);

		newItem->setIcon(Icon);

		item->appendRow(newItem);
	}
}

/*! \fn void CReportAPI::CreateFileGroupNode(CQuoteItem* pItem��QDomDocument *pDom )
*********************************************************************************************************
** \brief CReportAPI::CreateFileGroupNode
** \details
** \param pItem  ����ڵ�Ϊ���ڵ���ĵ��ڵ�
** \return void
** \author xingzhibing
** \date 2016��8��3��
** \note
********************************************************************************************************/
void CReportAPI::CreateFileGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement)
{
	Q_ASSERT(pItem &&  pDom);
	//�����ӽڵ�
	QDomElement nGroup = pDom->createElement("group");
	if (!pItem->hasChildren())
	{
		QDomAttr    nGroupName = pDom->createAttribute("GroupName");
		nGroup.setAttributeNode(nGroupName);
		nGroup.setAttribute("nGroupName", pItem->text());
	}
	else
	{
		QDomAttr    nGroupName = pDom->createAttribute("GroupName");
		nGroup.setAttributeNode(nGroupName);
		nGroup.setAttribute("GroupName", pItem->text());
		CreateGroupNode(pItem, pDom, nGroup);
	}
	pElement.appendChild(nGroup);
}

/*! \fn void CReportAPI::CreateFileNode(CQuoteItem* pItem��QDomDocument *pDom )
*********************************************************************************************************
** \brief CReportAPI::CreateFileNode
** \details
** \param pItem ����ڵ�Ϊ���ڵ���ĵ��ڵ�
** \return void
** \author xingzhibing
** \date 2016��8��3��
** \note
********************************************************************************************************/
void CReportAPI::CreateFileNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement)
{
	Q_ASSERT(pItem &&  pDom);
	//GetData
	QString nReportName = pItem->data(Qt::UserRole + 1).toString();
	QString nReportPath = pItem->data(Qt::UserRole + 2).toString();
	int     nReportType = pItem->data(Qt::UserRole + 3).toInt();
	//DomAttr
	QDomAttr nName = pDom->createAttribute("nfileName");
	QDomAttr nPath = pDom->createAttribute("nfilePath");
	QDomAttr nType = pDom->createAttribute("nfileType");
	//SetAttr
	pElement.setAttributeNode(nName);
	pElement.setAttributeNode(nPath);
	pElement.setAttributeNode(nType);

	pElement.setAttribute("nfileName", nReportName);
	pElement.setAttribute("nfilePath", nReportPath);
	pElement.setAttribute("nfileType", nReportType);

}

/*! \fn void CReportAPI::CreateGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement)
*********************************************************************************************************
** \brief CReportAPI::CreateGroupNode
** \details
** \param pItem
** \param pDom
** \param pElement
** \return void
** \author xingzhibing
** \date 2016��8��3��
** \note
********************************************************************************************************/
void CReportAPI::CreateGroupNode(CQuoteItem* pItem, QDomDocument *pDom, QDomElement pElement)
{
	Q_ASSERT(pItem && pDom);

	if (!pItem->hasChildren())
	{
		QDomElement pGroup = pDom->createElement("group");
		QDomAttr nName = pDom->createAttribute("GroupName");
		pGroup.setAttributeNode(nName);
		pGroup.setAttribute("GroupName", pItem->text());
		pElement.appendChild(pGroup);
		return;
	}

	int nCount = pItem->rowCount();

	for (int i = 0; i < nCount; ++i)
	{
		CQuoteItem *ppItem = static_cast<CQuoteItem*>(pItem->child(i));

		QDomElement nChildElement;

		int nType = ppItem->data(Qt::UserRole).toInt();

		if (nType == REPORT_GROUP_ITEM)
		{
			nChildElement = pDom->createElement("group");
			if (!ppItem->hasChildren())
			{
				QDomAttr nName = pDom->createAttribute("GroupName");
				nChildElement.setAttributeNode(nName);
				nChildElement.setAttribute("GroupName", ppItem->text());
			}
			else
			{
				QDomAttr nName = pDom->createAttribute("GroupName");
				nChildElement.setAttributeNode(nName);
				nChildElement.setAttribute("GroupName", ppItem->text());
				CreateGroupNode(ppItem, pDom, nChildElement);
			}
		}
		else  if (nType == REPORT_ITEM)
		{
			nChildElement = pDom->createElement("file");
			CreateFileNode(ppItem, pDom, nChildElement);
		}
		pElement.appendChild(nChildElement);
	}
}

/*! \fn void CReportAPI::slot_OnImpReportFile(QModelIndex& index)
*********************************************************************************************************
** \brief CReportAPI::slot_OnImpReportFile
** \details �����ѱ༭�ñ���
** \param index
** \return void
** \author xingzhibing
** \date 2016��8��4��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnImpReportFile(QModelIndex& index)
{
	QStringList _fileNames = QFileDialog::getOpenFileNames(nullptr,
		tr("ImportReport"), ".",
		tr("ReportFiles(*.xml )"));

	if (_fileNames.isEmpty())
		return;

	//��ȡ���ڵ�
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	int nType = item->data(Qt::UserRole).toInt();

	if (nType != REPORT_GROUP_ITEM)
	{
		return;
	}
	//Name + Path
	QString nName = item->text();

	CRptFileGrp* pRpt = m_pRptData->GetRptGrp(nName);

	QIcon  nIcon(":/images/report_file.ico");

	for each (QString var in _fileNames)
	{
		QFileInfo nInfo(var);

		CTableInfo *pInfo=new CTableInfo ;

		pInfo->m_szTableName = nInfo.baseName();

		pInfo->m_szTablePath = nInfo.filePath();

		pInfo->m_nTableType = UNKNOWN_REPORT;

		//���ӽ�Mgr����
		CRptFile* pFile = m_pRptData->CreateRptFile(pInfo);

		if (pFile==Q_NULLPTR)
		{
			m_pCore->LogMsg(REPORT_DES, (char*)tr("[RptFile]:file has been added !!!").toStdString().c_str(), LEVEL_1);
			continue;
		}
		pRpt->AddRptFile(pFile);

		//m_pTree->AddChildItem(pInfo, item, REPORT_ITEM, nIcon);
		QString szName = QFileInfo(pInfo->m_szTableName).baseName();
		//�ֶ��������
		CQuoteItem *newItem = new CQuoteItem(szName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		//Name
		newItem->setData(QVariant(pInfo->m_szTableName), Qt::UserRole + 1);
		//Path
		newItem->setData(QVariant(pInfo->m_szTablePath), Qt::UserRole + 2);

		newItem->setData(reinterpret_cast<long long >(pFile),REPORT_USER_DATA_ROLE);

		newItem->setData(REPORT_ITEM, Qt::UserRole);

		newItem->setIcon(nIcon);

		item->appendRow(newItem);
	}
}

/*! \fn void CReportAPI::slot_OnOpenReportFile(QModelIndex& index)
*********************************************************************************************************
** \brief CReportAPI::slot_OnOpenReportFile
** \details ˫�����ļ�
** \param index
** \return void
** \author xingzhibing
** \date 2016��8��5��
** \note
********************************************************************************************************/
void CReportAPI::slot_OnOpenReportFile(QModelIndex& index)
{
	Q_ASSERT(index.isValid());

	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
#if 0
	//�ж��ڴ��ļ����Ƿ��Ѿ�����
	if (item)
	{
		CRptFile* pFile = reinterpret_cast< CRptFile* >(item->data(REPORT_USER_DATA_ROLE).toLongLong());
		
		//ͨ���ж��ļ��б��ԪΪ0���жϴ��ļ�δ�������ڴ�
		if (pFile)
		{
			if (pFile->GetRptCellCount()==0)
			{
				//�ж��ڴ���û�У����ж��Ƿ��ܴ��ļ��м���
				if (item)
				{
					QString nPath = item->data(Qt::UserRole + 2).toString();
					QFileInfo nInfo(nPath);
					if (nInfo.isFile() && nInfo.exists())
					{
						LoadFile(nPath);
					}
					else
					{
						m_pCore->LogMsg(REPORT_DES, (char*)tr("file not exist!!!").toStdString().c_str(), LEVEL_1);
					}
				}
			}
			//����ļ��Ѿ����ڣ�����ڴ��м���
			else
			{
				//���ж��ļ��Ƿ��Ѿ���
				int nCount = m_pUi->GetTabWidget()->count();
				QWidget* pWidget = Q_NULLPTR;
				CReportEditView* pBaseWidget = Q_NULLPTR;
				for (int i = 0; i < nCount; ++i)
				{
					pWidget = m_pUi->GetTabWidget()->widget(i);
					pBaseWidget = dynamic_cast<CReportEditView*>(pWidget);
					if (pBaseWidget)
					{
						if (pBaseWidget->GetRptFile() == pFile)
						{
							m_pUi->GetTabWidget()->removeTab(i);
						}
					}
				}
			}
		}		
	}
#endif
	if (item)
	{
		CRptFile* pFile = reinterpret_cast< CRptFile* >(item->data(REPORT_USER_DATA_ROLE).toLongLong());
		Q_ASSERT(pFile);
		if (pFile == nullptr)
			return;

		// ˫����item->��tab��ʾ����
		if (pFile->m_bLoaded == true)
		{// �ļ��򿪹�,���ڴ����

		}
		else
		{    
			// ��Ӳ���ϼ��ر����ļ�,���سɹ��Ļ������ü��ر��λΪ��
			if (pFile->ReadXmlFile())
			{
				pFile->m_bLoaded=true;
			}
		}

		m_pTabWidget->AddTabCtrl(pFile);

#if 0
		if (pFile)
		{
			//�ļ�ֻ���½������ڴ��д���
			if (pFile->GetRptStatus()==FileOperInfo::FILE_SHM)
			{
				//�ļ������ڣ����ڽ������¼���
				if (!IsFileInTabWidget(pFile))
				{
					m_pTabWidget->AddTabCtrl(pFile);
				}
			}
			//�ļ����ڣ���û���ص��ڴ���
			else if (pFile->GetRptStatus()==FileOperInfo::FILE_XML)
			{
				//������������ڴ��ļ�
				QString nPath = item->data(Qt::UserRole + 2).toString();
				QFileInfo nInfo(nPath);
				if (nInfo.isFile() && nInfo.exists())
				{
					LoadFile(nPath);
				}
				else
				{
					m_pCore->LogMsg(REPORT_DES, (char*)tr("file not exist!!!").toStdString().c_str(), LEVEL_1);
				}
			}
			//�ļ��Ѿ����ص��ڴ���
			else if (pFile->GetRptStatus()==FileOperInfo::FILE_ALL)
			{
				//�Ѿ������ˣ�����ڴ��ж�ȡ
				QString nPath = item->data(Qt::UserRole + 2).toString();
				QFileInfo nInfo(nPath);
				if (nInfo.isFile() && nInfo.exists())
				{
					LoadFile(nPath);
				}
				else
				{
					m_pCore->LogMsg(REPORT_DES, (char*)tr("file not exist!!!").toStdString().c_str(), LEVEL_1);
				}
			}
		}
#endif
	}

}

/*! \fn bool CReportAPI::IsFileInTabWidget(CRptFile* pRptFile)
********************************************************************************************************* 
** \brief CReportAPI::IsFileInTabWidget 
** \details �ж��ļ��Ƿ����Tab��
** \param pRptFile �����򷵻�true,�������򷵻�false
** \return bool 
** \author xingzhibing
** \date 2016��8��18�� 
** \note 
********************************************************************************************************/
bool CReportAPI::IsFileInTabWidget(CRptFile* pRptFile)
{
	int nCount = m_pUi->GetTabWidget()->count();

	if (nCount==0)
	{
		return false;
	}

	QWidget* pWidget = Q_NULLPTR;
	CReportEditView* pBaseWidget = Q_NULLPTR;
	for (int i = 0; i < nCount; ++i)
	{
		pWidget = m_pUi->GetTabWidget()->widget(i);
		Q_ASSERT(pWidget);

		pBaseWidget = dynamic_cast<CReportEditView*>(pWidget);
		Q_ASSERT(pBaseWidget);
		if (pBaseWidget)
		{
			if (pBaseWidget->GetRptFile() == pRptFile)
			{
				return true;
			}
		}
	}
	return false;
}

/*! \fn void CReportAPI::slot_OnCustomContextMenu(const QPoint& pos)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnCustomContextMenu 
** \details �ؼ��Ҽ��˵�ʵ�ֲ���
** \param pos 
** \return void 
** \author xingzhibing
** \date 2016��8��5�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnCustomContextMenu(const QPoint& pos)
{
	Q_UNUSED(pos);
}

/*! \fn void CReportAPI::InitToolBar()
********************************************************************************************************* 
** \brief CReportAPI::InitToolBar 
** \details ��ʼ���˵���
** \return void 
** \author xingzhibing
** \date 2016��8��9�� 
** \note 
********************************************************************************************************/
void CReportAPI::InitToolBar()
{
//File
	m_pToolBar = m_pUi->addToolBar("Bar1");
	m_pToolBar->addAction(m_pFileNew);
	m_pToolBar->addAction(m_pFileOpen);
	m_pToolBar->addAction(m_pFileSave);
	m_pToolBar->addAction(m_pFileSaveAs);
	m_pToolBar->addAction(m_pFilePdf);
	m_pToolBar->addAction(m_pFilePrint);
	m_pToolBar->addAction(m_pFilePrintOverView);
	m_pToolBar->addSeparator();
//font
	//QToolBar *pBar5 = m_pUi->addToolBar("Bar5");
	m_pToolBar->addWidget(m_FontCombo);
	m_pToolBar->addWidget(m_Fontsize);
	m_pToolBar->addWidget(m_Color);
	m_pToolBar->addWidget(m_Brush);
	m_pToolBar->addSeparator();
//fontStyle
	//QToolBar* pBar6 = m_pUi->addToolBar("Bar6");
	m_pToolBar->addAction(m_Bold);
	m_pToolBar->addAction(m_Itilic);
	m_pToolBar->addAction(m_Underline);
	m_pToolBar->addSeparator();
//Action
	//QToolBar *pBar2 = m_pUi->addToolBar("Bar2");
	m_pToolBar->addAction(m_CutAction);
	m_pToolBar->addAction(m_CopyAction);
	m_pToolBar->addAction(m_PasteAction);
	m_pToolBar->addSeparator();
//Span
	//QToolBar *pBar3 = m_pUi->addToolBar("Bar3");
	m_pToolBar->addAction(m_MergeAction);
	m_pToolBar->addAction(m_UnMergeAction);
//Align
	//QToolBar *pBar4 = m_pUi->addToolBar("Bar4");
	m_pToolBar->addAction(m_LeftAlign);
	m_pToolBar->addAction(m_MiddleAlign);
	m_pToolBar->addAction(m_RightAlign);
	m_pToolBar->addSeparator();

//Chart
	//QToolBar* pBar7 = m_pUi->addToolBar("Bar5");
	m_pToolBar->addAction(m_BarAction);
	m_pToolBar->addAction(m_PieAction);
	m_pToolBar->addAction(m_CurveAction);
	m_pToolBar->addSeparator();
}

/*! \fn void CReportAPI::slot_OnSetColor(const QColor&)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetColor 
** \details ����������ɫ
** \param & 
** \return void 
** \author xingzhibing
** \date 2016��8��9�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetColor(const QColor& color)
{
	CReportEditView* pWidget = GetCurrentReportView();

	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					pItem->setTextColor(color);
				}
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnSetBgColor(const QColor&)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetBgColor 
** \details ���ñ�����ɫ
** \param & 
** \return void 
** \author xingzhibing
** \date 2016��8��9�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetBgColor(const QColor& color)
{
	CReportEditView* pWidget = GetCurrentReportView();

	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					pItem->setBackgroundColor(color);
				}
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnSetFontTextBold()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetFontTextBold 
** \details �����������
** \return void 
** \author xingzhibing
** \date 2016��8��9�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetFontTextBold()
{
	CReportEditView* pWidget = GetCurrentReportView();

	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					QFont curFont = pItem->font();

					if (pItem->font().bold())
					{
						curFont.setBold(false);
					}
					else
					{
						curFont.setBold(true);
					}
					pItem->setFont(curFont);
				}
			}
		}
	}	
}

/*! \fn void CReportAPI::slot_OnSetFontTextItalic()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetFontTextItalic 
** \details ��������б��
** \return void 
** \author xingzhibing
** \date 2016��8��9�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetFontTextItalic()
{
	CReportEditView* pWidget = GetCurrentReportView();

	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					QFont curFont = pItem->font();

					if (pItem->font().italic())
					{
						curFont.setItalic(false);
					}
					else
					{
						curFont.setItalic(true);
					}
					pItem->setFont(curFont);
				}
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnSetFontUnderLine()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetFontUnderLine 
** \details ���������»���
** \return void 
** \author xingzhibing
** \date 2016��8��9�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetFontUnderLine()
{
	CReportEditView* pWidget = GetCurrentReportView();

	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					QFont curFont = pItem->font();

					if (pItem->font().underline())
					{
						curFont.setUnderline(false);
					}
					else
					{
						curFont.setUnderline(true);
					}
					pItem->setFont(curFont);
				}
			}
		}
	}
}

/*! \fn bool CReportAPI::IsFileExisted(const QString& fileName)
********************************************************************************************************* 
** \brief CReportAPI::IsFileExisted 
** \details �ж��ļ��Ƿ����
** \param fileName 
** \return bool 
** \author xingzhibing
** \date 2016��8��10�� 
** \note 
********************************************************************************************************/
bool CReportAPI::IsFileExisted(const QString& fileName)
{
	QFileInfo nInfo(fileName);

	QDir nDir(DEFAULT_PATH);

	if (!nDir.exists())
	{
		//�ļ�·��������
		m_pCore->LogMsg(REPORT_DES, (char*)tr("file path error,please check!!!").toStdString().c_str(), LEVEL_1);
	}
	nDir.setFilter(QDir::Files|QDir::NoSymLinks);
	
	QFileInfoList nList = nDir.entryInfoList();

	int nCount = nList.count();

	//�ļ������ļ�������Լ���
	if (nCount<=0)
	{
		return true;
	}

	QStringList nFileList;

	for each (QFileInfo  var in nList)
	{
		if (QString::compare(var.suffix(), QString("xml"), Qt::CaseInsensitive) == 0)
		{
			qDebug() << var.baseName();
			if (var.fileName() == fileName)
			{
				return false;
			}
		}
	}
	return true;
}

/*! \fn void CReportAPI::slot_OnSetFontFamily(int)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetFontFamily 
** \details ��������
** \param  
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetFontFamily(int nIndex)
{
	CReportEditView* pWidget = GetCurrentReportView();

	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					QFont curFont = pItem->font();
					curFont.setFamily(m_FontCombo->currentFont().family());
					pItem->setFont(curFont);
				}
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnSetFontSize(int)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetFontSize 
** \details ���������С
** \param  
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetFontSize(int nIndex)
{
	CReportEditView* pWidget = GetCurrentReportView();
	
	Q_ASSERT(pWidget);

	if (pWidget)
	{
		QList<QTableWidgetItem *> items = pWidget->selectedItems();
		if (!items.isEmpty())
		{
			foreach(QTableWidgetItem *pItem, items)
			{
				Q_ASSERT(pItem);

				if (pItem)
				{
					QFont curFont = pItem->font();
					curFont.setPointSize(m_Fontsize->currentText().toInt());
					pItem->setFont(curFont);
				}
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnCopy()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnCopy 
** \details ���Ʋ���
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnCopy()
{
	CReportEditView* pView = GetCurrentReportView();
//	Q_ASSERT(pWidget);
	if (pView)
	{
		pView->copy();
	}
}
/*! \fn void CReportAPI::slot_OnPaste()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnPaste 
** \details ճ������
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnPaste()
{
	CReportEditView* pView = GetCurrentReportView();
//	Q_ASSERT(pWidget);
	if (pView)
	{
		pView->paste();
	}
}
/*! \fn void CReportAPI::slot_OnCut()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnCut 
** \details ���в���
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnCut()
{
	CReportEditView* pView = GetCurrentReportView();

	Q_ASSERT(pView);

	if (pView)
	{
		pView->cut();
	}
}
/*! \fn void CReportAPI::slot_OnSpan()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSpan 
** \details �ϲ�����
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnMerge()
{
	CReportEditView* pView = GetCurrentReportView();

	Q_ASSERT(pView);

	if (pView)
	{
		pView->SetMerge();
	}
}
/*! \fn void CReportAPI::slot_OnUnSpan()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnUnSpan 
** \details ��ֵ�Ԫ��
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnUnMerge()
{
	CReportEditView* pView = GetCurrentReportView();

	Q_ASSERT(pView);

	if (pView)
	{
		pView->SetUnMerge();
	}
}
/*! \fn void CReportAPI::slot_OnAlignLeft()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnAlignLeft 
** \details �����
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnAlignLeft()
{
	CReportEditView* pView = GetCurrentReportView();

	Q_ASSERT(pView);

	if (pView)
	{
		pView->SetFontLeft();
	}
}
/*! \fn void CReportAPI::slot_OnAlignRight()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnAlignRight 
** \details �Ҷ���
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnAlignRight()
{
	CReportEditView* pView = GetCurrentReportView();

	Q_ASSERT(pView);

	if (pView)
	{
		pView->SetFontRight();
	}
}
/*! \fn void CReportAPI::slot_OnAlignCenter()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnAlignCenter 
** \details ����
** \return void 
** \author xingzhibing
** \date 2016��8��11�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnAlignCenter()
{
	CReportEditView* pView = GetCurrentReportView();
	if (pView)
	{
		pView->SetFontMiddle();
	}
}
/*! \fn void CReportAPI::slot_OnSetSigPointData()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetSigPointData 
** \details ���õ�������
** \return void 
** \author xingzhibing
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetSigPointData()
{
	//TODO ���ѡ��ģ��
}

/*! \fn void CReportAPI::slot_OnSetMultiPointData()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetMultiPointData 
** \details ���ö��������
** \return void 
** \author xingzhibing
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetMultiPointData()
{
	//TODO ���ѡ��ģ��
}

/*! \fn void CReportAPI::slot_OnSetFormula()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnSetFormula 
** \details ���ü��㹫ʽ
** \return void 
** \author xingzhibing
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnSetFormula()
{
	CReportEditView* pView = GetCurrentReportView();
	if (pView)
	{
		int nRow = pView->currentRow();
		int nCol = pView->currentColumn();
		QCalFormula *pCal = Q_NULLPTR;
		QString nStr="";
		if (!pCal)
		{
			pCal = new QCalFormula();
			connect(pView, SIGNAL(cellSelected(const QString&)), pCal, SLOT(slot_on_dealData(const QString&)));
		}
		if (pCal->exec()==QDialog::Accepted)
		{
			nStr = pCal->GetResult();
		}
		else
		{
			return;
		}
		CCellData nData;
		nData.m_szDataContent = nStr;
		pView->SetCellFormat(nRow, nCol, nData); 
		pCal->deleteLater();
	}
}

/*! \fn void CReportAPI::slot_OnBar()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnBar 
** \details ���ݰ�״ͼ��ʾ
** \return void 
** \author xingzhibing
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnBar()
{
	CReportEditView* pWidget = GetCurrentReportView();
	Q_ASSERT(pWidget);
	if (pWidget)
	{
		CRptFile* pFile = pWidget->GetRptFile();

		Q_ASSERT(pFile);

		if (pFile)
		{
			QString szName = pFile->GetRecommendCtrlName();

			CCtrlData* pData = pFile->CreateCtrlData(szName);

			Q_ASSERT(pData);

			if (!pData)
			{
				//TODO Log����ʧ�� 
			}

			pData->SetCtrlType(CtrlType::BarType);

			if (!pWidget->SetBarView(pData))
			{
				pFile->DelCtrlData(pData);
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnPie()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnPie 
** \details ��״ͼ��ʾ
** \return void 
** \author xingzhibing
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnPie()
{
	CReportEditView* pView = GetCurrentReportView();
	if (pView)
	{
		CRptFile* pFile = pView->GetRptFile();

		if (pFile)
		{
			QString szName = pFile->GetRecommendCtrlName();

			CCtrlData* pData=pFile->CreateCtrlData(szName);

			Q_ASSERT(pData);

			if (!pData)
			{
				//TODO Log����ʧ�� 
			}

			pData->SetCtrlType(CtrlType::PieType);

			if (!pView->SetPieView(pData))
			{
				pFile->DelCtrlData(pData);
			}
		}		
	}
}

/*! \fn void CReportAPI::slot_OnCurve()
********************************************************************************************************* 
** \brief CReportAPI::slot_OnCurve 
** \details ������ʾ
** \return void 
** \author xingzhibing
** \date 2016��8��12�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnCurve()
{
	CReportEditView* pView = GetCurrentReportView();
	if (pView)
	{
		CRptFile* pFile = pView->GetRptFile();

		Q_ASSERT(pFile);

		if (pFile)
		{
			QString szName = pFile->GetRecommendCtrlName();

			CCtrlData* pData = pFile->CreateCtrlData(szName);

			Q_ASSERT(pData);

			if (!pData)
			{
				//TODO Log����ʧ�� 
			}

			pData->SetCtrlType(CtrlType::ChartType);

			if (!pView->SetChartView(pData))
			{
				pFile->DelCtrlData(pData);
			}
		}
	}
}

/*! \fn void CReportAPI::slot_OnAddReportRpt(QModelIndex& index)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnAddReportRpt 
** \details ���ӱ������ļ�
** \param index 
** \return void 
** \author xingzhibing
** \date 2016��8��15�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnAddReporGrp(QModelIndex& index)
{
	//��ȡ���ڵ�,����ȡ��ǰ���ļ���
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString nParentName = pItem->text();

	//auto pParentGrp = m_pRptData->GetRptGrp(nParentName);

	//��ȡ���ڵ�
	CRptFileGrp* pParentGrp = reinterpret_cast<CRptFileGrp*>(pItem->data(REPORT_USER_DATA_ROLE).toLongLong());


	//���ݲ���
	QString nszName = m_pRptData->GetRecommandRptGrpName();

	auto pRptGrp = m_pRptData->CreateRptGrp(nszName);

	

	if (pRptGrp==Q_NULLPTR)
	{
		m_pCore->LogMsg(REPORT_DES, (char*)tr("create report group  failed!!!").toStdString().c_str(), LEVEL_1);
		return;
	}
	//�������ӹ�ϵ
	pParentGrp->AddRptGrp(pRptGrp);

	//�������
	QIcon nIcon(QIcon(":/images/report_day.ico"));

	QString strName = nszName;

	if (pItem->data(Qt::WhatsThisRole).toInt() < MAX_REPORT_GROUP_COUNT)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(pItem->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(REPORT_GROUP_ITEM, Qt::UserRole);

		newItem->setIcon(nIcon);

		pItem->appendRow(newItem);

		newItem->setData(reinterpret_cast<long long>(pRptGrp), REPORT_USER_DATA_ROLE);

	}
	
	

}

/*! \fn void CReportAPI::slot_OnRptItemChanged(QModelIndex& index)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnRptItemChanged 
** \details ���ؼ������ݷ����仯����ʱ����Mgr����
** \param index 
** \return void 
** \author xingzhibing
** \date 2016��8��15�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnRptItemChanged(QModelIndex& index)
{

}

/*! \fn void CReportAPI::SaveToProject(QDomElement nElement, CRptFileGrp* pRptGrp)
********************************************************************************************************* 
** \brief CReportAPI::SaveToProject 
** \details ���ݱ������ļ�
** \param nElement  �洢�ڵ�
** \param pRptGrp   ��Ŀ¼
** \return void 
** \author xingzhibing
** \date 2016��8��16�� 
** \note 
********************************************************************************************************/
void CReportAPI::SaveRptGrpsToProject(QDomDocument *pXml, QDomElement nElement, CRptFileGrp* pRptGrp, bool bIsAppended)
{
	Q_ASSERT(pRptGrp);
	Q_ASSERT(nElement.isNull()==false);
	Q_ASSERT(pXml);
	if (!pXml)
	{
		return;
	}
	if (!pRptGrp)
	{
		return;
	}
	QDomElement domElement;
	if (bIsAppended)
	{
		domElement = nElement;
	}
	else
	{
		domElement = pXml->createElement("group");
		QDomAttr domAttr = pXml->createAttribute("GroupName");
		QString nName = pRptGrp->GetGrpName();
		domElement.setAttribute("GroupName", nName);
		nElement.appendChild(domElement);
	}

	int nRptGrpCount = pRptGrp->GetRptGrpCount();

	if (nRptGrpCount>0)
	{
		QVector<CRptFileGrp*> vecRptGrp;
		pRptGrp->GetArryGrps(vecRptGrp);
		//�����
		for (auto i : vecRptGrp)
		{
			if (i)
			{
				SaveRptGrpsToProject(pXml, domElement, i,false);
			}
		}
	}
	
	//�ļ�����
	QVector<CRptFile*> vecRptFile;
	pRptGrp->GetArryFiles(vecRptFile);

	for (auto var : vecRptFile)
	{
		Q_ASSERT(var);
		if (var)
		{
			QDomElement domChildElement = pXml->createElement("file");

			QDomAttr domNameAttr = pXml->createAttribute("file_name");
			QDomAttr domTypeAttr = pXml->createAttribute("report_type");

			domChildElement.setAttributeNode(domNameAttr);
			domChildElement.setAttributeNode(domTypeAttr);

			domChildElement.setAttribute("file_name", var->GetRptFileName());
			domChildElement.setAttribute("report_type", var->GetRptInfo()->m_nTableType);

			domElement.appendChild(domChildElement);
		}
	}	
}

/*! \fn bool CReportAPI::LogMsg(const char*szLogText, int nLevel)
********************************************************************************************************* 
** \brief CReportAPI::LogMsg 
** \details 
** \param szLogText   log�ı���Ϣ
** \param nLevel      log��Ϣ�ȼ�
** \return bool 
** \author xingzhibing
** \date 2016��8��18�� 
** \note 
********************************************************************************************************/
bool CReportAPI::LogMsg(const char*szLogText, int nLevel)
{
	Q_ASSERT(szLogText && strlen(szLogText)>0);
	if (szLogText==Q_NULLPTR || strlen(szLogText)==0)
	{
		return false;
	}
	Q_ASSERT(m_pCore);

	return m_pCore->LogMsg("report",szLogText,nLevel);
}

/*! \fn void CReportAPI::slot_OnDataChanged(QStandardItem* pItem)
********************************************************************************************************* 
** \brief CReportAPI::slot_OnDataChanged 
** \details ���ؼ��н��������޸ģ����Ӧ�����ݲ��޸�
** \param pItem 
** \return void 
** \author xingzhibing
** \date 2016��8��22�� 
** \note 
********************************************************************************************************/
void CReportAPI::slot_OnDataChanged(QStandardItem* pItem)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}
	//��ȡ������
	QString szName = pItem->text();

	//ת��ΪModule����
	CQuoteItem* ppItem = dynamic_cast<CQuoteItem*>(pItem);

	//��ȡ��������
	int nType = ppItem->data(Qt::UserRole).toInt();

	switch (nType)
	{
	case REPORT_GROUP_ITEM:
		{
			CRptFileGrp* pGrp = reinterpret_cast<CRptFileGrp*>(ppItem->data(REPORT_USER_DATA_ROLE).toLongLong());
			Q_ASSERT(pGrp);
			if (pGrp)
			{
				pGrp->SetGrpName(szName);
			}
			break;
		}		
	case REPORT_ITEM:
		{
			CRptFile* pFile = reinterpret_cast<CRptFile*>(ppItem->data(REPORT_USER_DATA_ROLE).toLongLong());
			Q_ASSERT(pFile);
			if (pFile)
			{
				pFile->SetRptFileName(szName);
			}
			break;
		}		
	default:
		break;
	}	
}

void CReportAPI::slot_IfInit(bool bRet)
{
	if (bRet)
	{
		//show
		m_pFileMenu->menuAction()->setVisible(true);
		m_pEdit->menuAction()->setVisible(true);
		m_OperateMenu->menuAction()->setVisible(true);
		m_pOptions->menuAction()->setVisible(true);
		m_pToolBar->setVisible(true);
	}
	else
	{
		//hide
		m_pFileMenu->menuAction()->setVisible(false);
		m_pEdit->menuAction()->setVisible(false);
		m_OperateMenu->menuAction()->setVisible(false);
		m_pOptions->menuAction()->setVisible(false);
		m_pToolBar->setVisible(false);
	}
}

