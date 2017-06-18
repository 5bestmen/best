

#include "scadalog_view.h"
#include "scadalog_mq.h"
#include "scada_log.h"

#include "dbgstudio/main_module.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QDateTime>
#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>  
 
/*! \fn CScadaLogModel::CScadaLogModel(QObject *parent, int rowCount, int colCount)
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
CScadaLogModel::CScadaLogModel(QObject *parent, int rowCount, int colCount)
	: QAbstractTableModel(parent)
{
	Q_UNUSED(rowCount);
	this->m_nRowCount = 0;
	this->m_nColCount = colCount;

	m_lstHeader.append((tr("Index")));
	m_lstHeader.append(tr("Date"));
	m_lstHeader.append(tr("Time"));
	m_lstHeader.append(tr("Module Name"));

	m_lstHeader.append(tr("Log"));
	m_lstHeader.append(tr("Process ID"));
	m_lstHeader.append(tr("Thread ID"));


	m_lstHeader.append(tr("File Name"));
	m_lstHeader.append(tr("Line"));
	m_lstHeader.append(tr("Function Name")); 
	m_lstHeader.append(("Level"));
	m_lstHeader.append((" "));
}
/*! \fn CScadaLogModel::~CScadaLogModel()
*********************************************************************************************************
** \brief CScadaLogModel::~CScadaLogModel
** \details
** \return
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
CScadaLogModel::~CScadaLogModel()
{
	m_arrLogs.clear();
}
/*! \fn int	CScadaLogModel::columnCount(const QModelIndex &parent) const
*********************************************************************************************************
** \brief CScadaLogModel::columnCount
** \details
** \param parent
** \return int
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
int	CScadaLogModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return COL_MAX;
}

/*! \fn int CScadaLogModel::rowCount(const QModelIndex &parent) const
*********************************************************************************************************
** \brief CScadaLogModel::rowCount
** \details
** \param parent
** \return int
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
int CScadaLogModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

//	return 0;
	return  static_cast <int>  (m_arrLogs.size());
}
/*! \fn QVariant CScadaLogModel::data(const QModelIndex &index, int role) const
*********************************************************************************************************
** \brief CScadaLogModel::data
** \details
** \param index
** \param role
** \return QT_NAMESPACE::QVariant
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
QVariant CScadaLogModel::data(const QModelIndex &index, int role) const
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

		std::shared_ptr<CScadaLog> pLog = m_arrLogs[nRow];
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
			else if (nCol == COL_MODULE_NAME)
			{
				return QString::fromLocal8Bit(pLog->m_szModuleName.c_str());
			}
			else if (nCol == COL_LOG_TEXT)
			{
				return QString::fromLocal8Bit(pLog->m_szLogText.c_str());
			}
			else if (nCol == COL_PROCESS)
			{
				return pLog->m_nProcessId;
			}
			else if (nCol == COL_THREAD)
			{
				return pLog->m_nThreadId;
			}
			else if (nCol == COL_FILE_NAME)
			{
				return pLog->m_szFileName.c_str();
			}
			else if (nCol == COL_LINE)
			{
				return pLog->m_nLine;
			}
			else if (nCol == COL_FUNC_NAME)
			{
				return pLog->m_szFuncName.c_str();
			}
			else if (nCol == COL_LEVEL)
			{
				return pLog->m_nLevel;
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

		if (nRow >= m_arrLogs.size() || nRow < 0)
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

void  CScadaLogModel::ClearAll()
{
	beginResetModel();
	m_arrLogs.clear();
	endResetModel();
}

void  CScadaLogModel::AddItem(CScadaLog *pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;

	std::shared_ptr<CScadaLog>ptrLog = std::make_shared<CScadaLog>(pLog);
	 
	size_t nRow = m_arrLogs.size();

	beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);
	m_arrLogs.push_back(ptrLog);
	endInsertRows();

	size_t nCnt = m_arrLogs.size();
	if (nCnt >= MAX_LOG_NUM)
	{
		while (nCnt >= MAX_LOG_NUM)
		{
			m_arrLogs.pop_front();
			nCnt = m_arrLogs.size();
		}
	}
}

void  CScadaLogModel::DispLogString(std::deque<std::shared_ptr<CScadaLog>> &arrLogs)
{
	size_t nRow = m_arrLogs.size();

	beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);

	auto it = arrLogs.begin();
	for (; it != arrLogs.end(); ++it)
	{
		m_arrLogs.push_back(*it);
	}
	endInsertRows();

	size_t nCnt = this->rowCount();
	if (nCnt >= MAX_LOG_NUM)
	{
		beginResetModel();
		while (nCnt >= MAX_LOG_NUM)
		{
			m_arrLogs.pop_front();
			nCnt = this->rowCount();
		}
		endResetModel();
	}
}
/*! \fn QVariant CScadaLogModel::headerData(int section, Qt::Orientation orientation, int role) const
*********************************************************************************************************
** \brief CScadaLogModel::headerData
** \details
** \param section
** \param orientation
** \param role
** \return QT_NAMESPACE::QVariant
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
QVariant CScadaLogModel::headerData(int section, Qt::Orientation orientation, int role) const
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

CScadaLogView::CScadaLogView(QWidget *parent)
	: QWidget(parent)	
{
	ui.setupUi(this);
	m_pPullLogTask = nullptr;
	m_pTableModel = new CScadaLogModel(this);
	
	InitTask();
	InitView();
	m_nLogTimerId = 0;
	m_nLogTimerId = startTimer(1000);
}

CScadaLogView::~CScadaLogView()
{
	if (m_pPullLogTask)
	{
		m_pPullLogTask->StopModule();
	}
}

CScadaLogView * CScadaLogView::GetInstance()
{
	return Singleton<CScadaLogView, LeakySingletonTraits<CScadaLogView>>::instance();
}

void CScadaLogView::timerEvent(QTimerEvent *pEvent)
{
	Q_UNUSED(pEvent);

	if (pEvent->timerId() == m_nLogTimerId)
	{
		if (m_pPullLogTask == nullptr)
		{
			return;
		}
		DispLogString();
#if 0
		size_t nCnt = m_pPullLogTask->GetLogCount();
  		bool bRet = false;
  		CScadaLog log;
  
		if (nCnt)
		{
			while (nCnt)
			{
				bRet = m_pPullLogTask->GetLogString(&log);
				if (bRet)
				{
					m_pTableModel->AddItem(&log);
				}
				nCnt = m_pPullLogTask->GetLogCount();
			}
		}
#endif
	}
}

void CScadaLogView::DispLogString()
{	
	size_t nCnt = m_pPullLogTask->GetLogCount();
	bool bRet = false;

	if (nCnt>0)
	{
		std::deque<std::shared_ptr<CScadaLog>> arrLogs;
		
		bRet = m_pPullLogTask->GetLogs(arrLogs);
		if (bRet)
		{
			m_pTableModel->DispLogString(arrLogs);
		}
	}

	ui.LogGridCtrl->scrollToBottom();
}

void CScadaLogView::InitTask()
{
	if (m_pPullLogTask == nullptr)
	{
		m_pPullLogTask = std::make_shared<CScadaLogMQTask>();
		m_pPullLogTask->StartLogMQ();
	}
}

//初始化界面
void CScadaLogView::InitView()
{
	ui.LogGridCtrl->setModel(m_pTableModel);
	ui.LogGridCtrl->verticalHeader()->setHidden(true);
	ui.LogGridCtrl->setSortingEnabled(true);
	ui.LogGridCtrl->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.LogGridCtrl->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.LogGridCtrl->horizontalHeader()->setStretchLastSection(true);

	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_INDEX, 60);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_DATE, 110);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_TIME, 80);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_MODULE_NAME, 100);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_LOG_TEXT, 220);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_PROCESS, 110);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_THREAD, 80);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_FILE_NAME, 110);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_LINE, 80);

	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_FUNC_NAME, 120);
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_LEVEL, 80); 
	ui.LogGridCtrl->setColumnWidth(CScadaLogModel::COL_PAD, 80);
}

//筛选
void CScadaLogView::Slot_ClickFilter()
{
    //筛选界面
	QMessageBox::warning(this, tr("Warning"), tr("this function dose not develop"));
}


//读取表格中的文件写入到execl表格中
void CScadaLogView::ExportDataToFile(const QString &strFileName)
{
	if (strFileName.isEmpty())
	{
		return;
	}

    QFile file(strFileName);

	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug()<<file.errorString();
		return;
	}

	QTextStream out(&file);
//    out << m_pTableModel->getTotalData();
    file.close();
}

//导出
void CScadaLogView::Slot_ClickExport()
{
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() != QDialog::Accepted)
	{
		return;
	}

	return ExportDataToFile(dialog.selectedFiles().first());
}

