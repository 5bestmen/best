/*! @file
<PRE>
********************************************************************************
模块名       :  标准对话框
文件名       :  channelview.cpp
文件实现功能 :  标准对话框
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 通道 装置
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/ 

#include "mainfrm.h"
#include "graph_file.h"
#include "realtime_scene.h"
#include "realtime_view.h"
#include "realtime_graph.h"
#include "colour_define.h"
#include "scadaapi/scdsvcapi.h"
#include "log/log.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTabWidget>
#include <QSplitter>
#include <QDockWidget>

CMainFrame::CMainFrame(QWidget *pParent) :  QMainWindow(pParent)
{ 
	m_pTabWidget = new QTabWidget(nullptr);
	
	m_pDockWidget = new QDockWidget(nullptr);
	m_pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea |Qt::BottomDockWidgetArea);
	
	m_pTreeWidget = new QTreeWidget(m_pDockWidget);

	m_pDockWidget->setWidget(m_pTreeWidget);

	InitMainWindow();


	QObject::connect(m_pTreeWidget,&QTreeWidget::itemDoubleClicked,this,&CMainFrame::slot_OnTreeDbClicked);
}
/*! \fn CGraphTabView::~CGraphTabView()
********************************************************************************************************* 
** \brief CGraphTabView::~CGraphTabView
** \details 
** \return  
** \author gw
** \date 2015年11月18日 
** \note 
********************************************************************************************************/
CMainFrame::~CMainFrame()
{ 	
//	this->deleteLater();
} 
/*! \fn void CGraphTabView::closeEvent( QCloseEvent * event )
********************************************************************************************************* 
** \brief CGraphTabView::closeEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月20日 
** \note 
********************************************************************************************************/
void CMainFrame::closeEvent(QCloseEvent * event)
{	
	Q_UNUSED(event);
	for (auto iter:m_arrTasks)
	{
		iter->Shutdown();
		delete iter;
		iter = nullptr;
	}
	ShutDown();
}

bool CMainFrame::Initalize(std::vector < SIM_GRAPH_DEF >& arrFiles)
{
	Q_ASSERT(!arrFiles.empty());

	::ConnectScada(GetProjPath().toStdString().c_str(), "");

	::CreateLog("RT_GRAPH");

	m_arrFiles = arrFiles;

	if (arrFiles.empty())
	{
		return false;
	}
	else
	{
		InitTreeWidget();
	}
	return true;
}

/*! \fn void CGraphTabView::OnDelete()
********************************************************************************************************* 
** \brief CGraphTabView::OnDelete 
** \details 删除
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CMainFrame::OnDelete()
{

}


void CMainFrame::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
//	m_pGraphScene->setSceneRect(0, 0, this->width(), this->height());
}
/*! \fn void  CMainFrame::AddView(CGraphFile *pGraphFile)
********************************************************************************************************* 
** \brief CMainFrame::AddView 
** \details 打开视图
** \param pGraphFile 
** \return void 
** \author LiJin 
** \date 2017年2月9日 
** \note 
********************************************************************************************************/
void  CMainFrame::AddView(std::shared_ptr<CGraphFile> pGraphFile)
{
	Q_ASSERT(pGraphFile);
	if (pGraphFile == nullptr)
		return;

	CRealTimeView *pView = new CRealTimeView(pGraphFile->GetScene(), nullptr);

	pView->setScene(pGraphFile->GetScene());

	CRealTimeTask * pTask = new CRealTimeTask(pGraphFile);

	m_arrTasks.push_back(pTask);

	pTask->Initialize(pGraphFile, 0);

	pTask->Run();

	pGraphFile->GetScene()->setSceneRect(pGraphFile->GetBackgroundRect());

	m_pTabWidget->addTab(pView, "test1");

}

void CMainFrame::InitTreeWidget()
{
	m_pTreeWidget->setColumnCount(1);
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem(m_pTreeWidget);
	pRootItem->setText(0, "root");
	m_pTreeWidget->addTopLevelItem(pRootItem);

	for (auto iter:m_arrFiles)
	{
		auto pItem = new QTreeWidgetItem(pRootItem);
		pItem->setText(0,QString::fromStdString(iter->szFileName));
		pItem->setData(0, USER_FILE_ROLE, QString::fromStdString(iter->szFilePath));
		pRootItem->addChild(pItem);
	}
}


void CMainFrame::Run()
{

}

void CMainFrame::ShutDown()
{
	::DisconnectScada("","");
	::DestroyLog("RT_GRAPH");
}


void CMainFrame::slot_OnTreeDbClicked(QTreeWidgetItem *item, int column)
{
	Q_ASSERT(item && column >= 0);
	if (!item || column < 0 )
	{
		return;
	}
	auto pFile = std::make_shared<GraphFileDataUnit>();
	pFile->szFileName = item->text(0).toStdString();
	pFile->szFilePath = item->data(0, USER_FILE_ROLE).toString().toStdString();
	 
	SetCurrentFile(QString::fromStdString(pFile->szFilePath));
}

void CMainFrame::InitMainWindow()
{
	QSplitter* pSplitter = new QSplitter(Qt::Horizontal);
	//pSplitter->addWidget(m_pDockWidget);
	pSplitter->addWidget(m_pTabWidget);

	addDockWidget(Qt::LeftDockWidgetArea,m_pDockWidget);

	setCentralWidget(pSplitter);
}

void CMainFrame::SetCurrentFile(const QString& szName)
{
	Q_ASSERT(!szName.isEmpty() && szName.length()>0);
	if (szName.isEmpty() || szName.length()==0)
	{
		return;
	}

	auto pGraphFile = std::make_shared<CGraphFile>();

	pGraphFile->LoadFile(szName);

	AddView(pGraphFile);
}
