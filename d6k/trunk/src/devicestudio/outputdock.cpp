/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出dock窗口
文件名       :  sysoutdock.cpp
文件实现功能 :  系统输出dock窗口
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.15
*/

#include "outputdock.h"
#include "log_impl.h"

#include <QTabWidget>
#include <QTableView>
#include <QScrollBar>
#include <QHeaderView>
#include <QTimerEvent>
#include <QAction>
#include <QMenu>
#include <QClipboard>
#include <QMessageBox>
#include <QApplication>


/*! \fn COutputModel::COutputModel(QObject *parent, int rowCount, int colCount)
*********************************************************************************************************
** \brief COutputModel::COutputModel
** \details
** \param parent
** \param rowCount
** \param colCount
** \return
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
COutputModel::COutputModel(QObject *parent, int rowCount, int colCount)
	: QAbstractTableModel(parent)
{
	Q_UNUSED(rowCount);
	this->m_nRowCount = 0;
	this->m_nColCount = colCount;

	m_lstHeader.append(("!"));
	m_lstHeader.append((tr("Index")));
	m_lstHeader.append(tr("Date"));
	m_lstHeader.append(tr("Time"));
	m_lstHeader.append(tr("Module Name"));

	m_lstHeader.append(tr("Comment"));
	m_lstHeader.append(tr("Process ID"));
	m_lstHeader.append(tr("Thread ID"));
	
}
/*! \fn CSystemOutputModel::~CSystemOutputModel()
*********************************************************************************************************
** \brief CSystemOutputModel::~CSystemOutputModel
** \details
** \return
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
COutputModel::~COutputModel()
{
	m_arrLogs.clear();
}
/*! \fn int	CSystemOutputModel::columnCount(const QModelIndex &parent) const
*********************************************************************************************************
** \brief CSystemOutputModel::columnCount
** \details
** \param parent
** \return int
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
int	COutputModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return COL_MAX;
}

/*! \fn int COutputModel::rowCount(const QModelIndex &parent) const
*********************************************************************************************************
** \brief COutputModel::rowCount
** \details
** \param parent
** \return int
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
int COutputModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return  static_cast <int>  (m_arrLogs.size());
}
/*! \fn QVariant COutputModel::data(const QModelIndex &index, int role) const
*********************************************************************************************************
** \brief COutputModel::data
** \details
** \param index
** \param role
** \return QT_NAMESPACE::QVariant
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
QVariant COutputModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) 
	{
		return QVariant(); 
	}

// 	if (Qt::BackgroundColorRole == role)
// 	{
// 		//return  QVariant(QColor(Qt::black));
// 	}
	
	if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_arrLogs.size() || nRow < 0)
			return QVariant();

		std::shared_ptr<CMsgLog> pLog = m_arrLogs[nRow];
		Q_ASSERT(pLog != nullptr);
		if (pLog != nullptr)
		{
			if (nCol == COL_LEVEL)
			{
				return pLog->m_nLevel;
			}
			else if (nCol == COL_INDEX)
			{
				return pLog->m_nIndex;
			}
			else if (nCol == COL_DATE)
			{
				return pLog->m_tmLog.date().toString("yyyy-MM-dd");
			}
			else if (nCol == COL_TIME)
			{
				return pLog->m_tmLog.time().toString("hh:mm:ss.zzz");
			}
			else if (nCol == COL_MODULE)
			{
				return QString::fromLocal8Bit(pLog->m_szModuleName.c_str());
			}
			else if ( nCol == COL_COMMENT)
			{
				return QString::fromLocal8Bit(pLog->m_szLogText.c_str());
			}
			else if (nCol == COL_PROCESS)
			{
				return pLog->m_nProcessId;
			}
			else if (nCol ==COL_THREAD)
			{
				return pLog->m_nThreadId;
			}
			
			return QVariant();
		}
	}
	if (Qt::TextColorRole == role)
	{
// 		int nRow = index.row();
// 		int nCol = index.column();

// 		if (nRow >= m_arrLogs.size() || nRow<0)
// 			return QVariant();
// 
// 		CDataItem* pItem = m_items[nRow];
// 
// 		if (nCol == Date)
// 		{
// 			return tr(pItem->m_strDate.c_str());
// 		}
// 		else if (nCol == Time)
// 		{
// 			return tr(pItem->m_strTime.c_str());
// 		}
// 		else if (nCol == Tag)
// 		{
// 			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
// 			//return strMsg;
// 			return pItem->m_strTag.c_str();
// 		}
// 		else if (nCol == Type)
// 		{
// 			return pItem->m_strType.c_str();
// 		}

		return QVariant();
	}
#if 0
	if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_arrLogs.size() || nRow<0)
			return QVariant();

		CDataItem* pItem = m_items[nRow];

		if (nCol == Date)
		{
			return tr(pItem->m_strDate.c_str());
		}
		else if (nCol == Time)
		{
			return tr(pItem->m_strTime.c_str());
		}
		else if (nCol == Tag)
		{
			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
			//return strMsg;
			return pItem->m_strTag.c_str();
		}
		else if (nCol == Type)
		{
			return pItem->m_strType.c_str();
		}

		return QVariant();
	}
	else if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_items.size() || nRow < 0)
			return QVariant();

		CDataItem* pItem = m_items[nRow];

		if (nCol == Date)
		{
			return tr(pItem->m_strDate.c_str());
		}
		else if (nCol == Time)
		{
			return tr(pItem->m_strTime.c_str());
		}
		else if (nCol == Tag)
		{
			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
			//return strMsg;
			return pItem->m_strTag.c_str();
		}
		else if (nCol == Type)
		{
			return pItem->m_strType.c_str();
		}

		return QVariant();
	}

	return QVariant();
#endif

	return QVariant();
}
void  COutputModel::AddItem(CMsgLog *pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;
	
	std::shared_ptr<CMsgLog>ptrLog = std::make_shared<CMsgLog>(pLog);
	size_t nRow = m_arrLogs.size();

	beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);
	m_arrLogs.push_back(ptrLog);
	endInsertRows();
}

#if 0
/*! \fn int  COutputModel::AddItem(CMsgLog &log, QColor color)
*********************************************************************************************************
** \brief COutputModel::AddItem
** \details 添加项
** \param log
** \param color
** \return int
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
int  COutputModel::AddItem(CMsgLog &log, QColor &color)
{
	int nRow = m_arrLogs.size();

	//if (nRow >= m_rowCount)
	//ClearAll();


//	CDataItem* pItem = new CDataItem(log, color); 
	beginInsertRows(QModelIndex(), nRow, nRow);
//	m_arrLogs.push_back(&log);
	endInsertRows();

	//beginResetModel();

	//endResetModel();

	return static_cast <int> (m_arrLogs.size());
}
#endif
/*! \fn void CSystemOutputModel::ClearAll()
*********************************************************************************************************
** \brief CSystemOutputModel::ClearAll
** \details
** \return void
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
void COutputModel::ClearAll()
{
	beginResetModel();
	m_arrLogs.clear();
	endResetModel();
// 	std::vector <CDataItem*>::iterator iter = m_items.begin();
// 	for (; iter != m_items.end();)
// 	{
// 		CDataItem* pItem = *iter;
// 		if (pItem == NULL)
// 			continue;
// 
// 		delete pItem;
// 		pItem = NULL;
// 
// 		iter = m_items.erase(iter);
// 	}
}

/*! \fn QVariant CSystemOutputModel::headerData(int section, Qt::Orientation orientation, int role) const
*********************************************************************************************************
** \brief CSystemOutputModel::headerData
** \details
** \param section
** \param orientation
** \param role
** \return QT_NAMESPACE::QVariant
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
QVariant COutputModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_lstHeader.size() > section)
				return m_lstHeader[section];
			else
				return section + 1;
		}
		else
		{
			return section + 1;
		}
	}
	return QVariant();
}

//获取一行数据
QString COutputModel::CpyRowData(const QModelIndex &index)
{
	QString strLineData;
	int nRow = index.row();
	std::shared_ptr<CMsgLog> pLog = m_arrLogs[nRow];

	Q_ASSERT(pLog != nullptr);
	if (pLog != nullptr)
	{
		strLineData.append(QString::number(pLog->m_nIndex));
		strLineData.append("  ");

		strLineData.append(pLog->m_tmLog.date().toString("yyyy-MM-dd"));
		strLineData.append("  ");

		strLineData.append(pLog->m_tmLog.time().toString("hh:mm:ss.zzz"));
		strLineData.append("  ");

		strLineData.append(QString::fromLocal8Bit(pLog->m_szModuleName.c_str()));
		strLineData.append("  ");

		strLineData.append(QString::fromLocal8Bit(pLog->m_szLogText.c_str()));
		strLineData.append("  ");

		strLineData.append(QString::number(pLog->m_nProcessId));
		strLineData.append("  ");

		strLineData.append(QString::number(pLog->m_nThreadId));
		strLineData.append("  ");
		strLineData.append("\n");
	}
	return strLineData;
}

/*! \fn CSysOutDock::CSysOutDock(QWidget *parent)
********************************************************************************************************* 
** \brief CSysOutDock::CSysOutDock 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
COutputDock::COutputDock(QWidget *parent)
	: QDockWidget(parent)
{	
	this->setWindowTitle(tr("System Out"));
	b_m_srcollFlag = true;
	m_pTabWnd = new QTabWidget(this);
	m_pTabWnd->setTabPosition(QTabWidget::South);
	setWidget(m_pTabWnd);
 
	m_pLogView = new QTableView(this);
	m_pLogModel = new COutputModel(m_pLogView);

	m_pLogView->setModel(m_pLogModel);
	m_pLogView->setSelectionBehavior(QAbstractItemView::SelectRows);
	

	m_pTabWnd->addTab(m_pLogView, tr("Out"));

	connect(m_pLogModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(ModelRowsInserted(const QModelIndex &, int, int)));

	m_nLogTimerId = startTimer(1000);
	//table增加右击菜单
	m_pLogView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pLogView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
}

void COutputDock::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
	m_pLogView->setColumnWidth(0, m_pLogView->horizontalHeader()->width() / 30);
	m_pLogView->setColumnWidth(1, m_pLogView->horizontalHeader()->width() / 15);
	m_pLogView->setColumnWidth(2, m_pLogView->horizontalHeader()->width() / 15);
	m_pLogView->setColumnWidth(3, m_pLogView->horizontalHeader()->width() / 15);
	m_pLogView->setColumnWidth(4, m_pLogView->horizontalHeader()->width()*9 / 30);
	m_pLogView->setColumnWidth(5, m_pLogView->horizontalHeader()->width()*10 /30);
	m_pLogView->setColumnWidth(6, m_pLogView->horizontalHeader()->width() / 15);
	m_pLogView->setColumnWidth(7, m_pLogView->horizontalHeader()->width() / 15);
}

void  COutputDock::timerEvent(QTimerEvent *pEvent)
{
	if (pEvent->timerId() == m_nLogTimerId)
	{
		size_t nCnt = CLogImpl::GetInstance()->GetLogCount();
		bool bRet = false;
		CMsgLog log;

		if (nCnt)
		{
			while (nCnt)
			{
				bRet = CLogImpl::GetInstance()->GetLogString(&log);
				if (bRet)
				{
					m_pLogModel->AddItem(&log);
				}
				nCnt = CLogImpl::GetInstance()->GetLogCount();
			}
		}
	}
}


/*! \fn CSysOutDock::~CSysOutDock()
********************************************************************************************************* 
** \brief CSysOutDock::~CSysOutDock 
** \details 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
COutputDock::~COutputDock()
{
	if (m_nLogTimerId)
	{
		killTimer(m_nLogTimerId);
		m_nLogTimerId = 0;
	}
}

/*! \fn void CSysOutDock::createCommandView()
********************************************************************************************************* 
** \brief CSysOutDock::createCommandView 
** \details 
** \return void 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
void COutputDock::createCommandView()
{
// 	m_commandView = new CSystemOutputView(this);
// 
// 	m_commandModel = new CSystemOutputModel(this);
// 
// 	m_commandView->setModel(m_commandModel);
// 
// 	m_commandView->setSelectionBehavior(QAbstractItemView::SelectRows);
// 
// 	m_messsageTab->addTab(m_commandView, QIcon(":/images/new.png"), "CommandT");
}

/*! \fn void CSysOutDock::createLogCommandView()
********************************************************************************************************* 
** \brief CSysOutDock::createLogCommandView 
** \details 
** \return void 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
void COutputDock::createLogCommandView()
{
// 	m_logView = new CSystemOutputView(this);
// 
// 	m_logModel = new CSystemOutputModel(m_logView);
// 
// 	connect(m_logModel, SIGNAL(rowsInserted(const QModelIndex &,int,int)), this, SLOT(ModelRowsInserted(const QModelIndex &,int,int)));
// 
// 	QVector<QString> vec;
// 
// 	vec.push_back("test");
// 
// 	//m_logModel->AddItem(vec);
// 
// 	m_logView->setModel(m_logModel);
// 
// 	m_logView->setSelectionBehavior(QAbstractItemView::SelectRows);
// 
// 	m_messsageTab->addTab(m_logView, QIcon(":/images/new.png"), "LogT");
}

void COutputDock::ModelRowsInserted(const QModelIndex & parent, int start, int end)
{
	Q_UNUSED(end);
	Q_UNUSED(start);
	Q_UNUSED(parent);
	if (b_m_srcollFlag)
	{
		m_pLogView->scrollToBottom();
	}
}

//右击菜单
void COutputDock::Slot_ContextMenuRequest(const QPoint &cPoint)
{
	Q_UNUSED(cPoint);
	QMenu *pMenu = new QMenu(m_pLogView);
	
	QAction *pCpyAct = new QAction(tr("Copy"), m_pLogView);
	QAction *pClear = new QAction(tr("Clear"), m_pLogView);
	QAction *pStart = new QAction(tr("Run"),m_pLogView);
	QAction *pStop = new QAction(tr("Stop"), m_pLogView);
	QAction *pExportAct = new QAction(tr("Export"), m_pLogView);

	pMenu->addAction(pCpyAct);
	pMenu->addAction(pClear);
	pMenu->addAction(pStart);
	pMenu->addAction(pStop);
	pMenu->addSeparator();
	pMenu->addAction(pExportAct);

	connect(pCpyAct, SIGNAL(triggered()), this, SLOT(Slot_CpyAction()));
	connect(pClear, SIGNAL(triggered()), this, SLOT(Slot_ClearItemAction()));
	connect(pStart, SIGNAL(triggered()), this, SLOT(Slot_StartRunAction()));
	connect(pStop, SIGNAL(triggered()), this, SLOT(Slot_StopAction()));
	connect(pExportAct, SIGNAL(triggered()), this, SLOT(Slot_ExportAction()));


	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}

//copy
void COutputDock::Slot_CpyAction()
{
	QModelIndexList selected = m_pLogView->selectionModel()->selectedRows();
	//组装需要拷贝的数据
	QString strCpyData;
	//读取数据
	for each (QModelIndex mIndex in selected)
	{
		strCpyData.append(m_pLogModel->CpyRowData(mIndex));
	}

	QClipboard *board = QApplication::clipboard();

	board->setText(strCpyData);
}

//clear
void COutputDock::Slot_ClearItemAction()
{
	m_pLogModel->ClearAll();
}
//run
void COutputDock::Slot_StartRunAction()
{
	//开始滚动
	b_m_srcollFlag = true;
}
//stop
void COutputDock::Slot_StopAction()
{
	//停止滚动
	b_m_srcollFlag = false;
}
//Export
void COutputDock::Slot_ExportAction()
{
	//延后开发
	QMessageBox::warning(this,tr("warning"),tr("No Develop!"));
}

//获取tableview的指针
QTableView * COutputDock::GetTableView()
{
	return m_pLogView;
}