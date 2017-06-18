
#include "capturemsgwgt.h"
#include "dbgstudio/main_module.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QDateTime>
#include <QTextEdit>
#include <QScrollBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

CChooseInterfaceWgt::CChooseInterfaceWgt(QWidget *parent)
	:QWidget(parent),
	m_pTreeWgtChoose(new QTreeWidget(this)),
	m_pBtnStart(new QPushButton(tr("Start"),this)),
	m_pBtnStop(new QPushButton(tr("Stop"),this)),
	m_pBtnClose(new QPushButton(tr("Close"),this))
{
	setWindowTitle(tr("Capturing"));
	setFixedSize(600, 400);
	m_pTreeWgtChoose->setColumnCount(1);
	m_pTreeWgtChoose->setHeaderLabel(tr("Capturing Interfaces"));

	connect(m_pTreeWgtChoose, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_SelectTreeTopLevelItem(QTreeWidgetItem *, int)));
	connect(m_pBtnStart, SIGNAL(clicked()), this, SLOT(Slot_StartCapturing()));
	connect(m_pBtnStop, SIGNAL(clicked()), this, SLOT(Slot_StopCapturing()));
	connect(m_pBtnClose, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout *pBtnLayout = new QHBoxLayout;
	pBtnLayout->addStretch();
	pBtnLayout->addWidget(m_pBtnStart);
	pBtnLayout->addWidget(m_pBtnStop);
	pBtnLayout->addWidget(m_pBtnClose);

	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(m_pTreeWgtChoose);
	pWgtlayout->addLayout(pBtnLayout);
}


//增加选项
void CChooseInterfaceWgt::AddPluginSection(const QString &strPluginDeviceName)
{
	QTreeWidgetItem *pTcpTreeItem = new QTreeWidgetItem;
	pTcpTreeItem->setText(0, strPluginDeviceName);
	pTcpTreeItem->setCheckState(0,Qt::Unchecked);

	m_pTreeWgtChoose->addTopLevelItem(pTcpTreeItem);
}

//选中树父节点
void CChooseInterfaceWgt::Slot_SelectTreeTopLevelItem(QTreeWidgetItem *pItem, int iIndex)
{
	Q_UNUSED(iIndex);
	if (pItem->checkState(0) == Qt::Checked)
	{
		for (int i = 0; i < pItem->childCount(); i++)
		{
			pItem->child(i)->setCheckState(0,Qt::Checked);
		}
	}
	else
	{
		for (int j = 0; j < pItem->childCount(); j++)
		{
			pItem->child(j)->setCheckState(0, Qt::Unchecked);
		}
	}
}

//点击开始
void CChooseInterfaceWgt::Slot_StartCapturing()
{
	QStringList strlstCheck;
	for (int i = 0; i < m_pTreeWgtChoose->topLevelItemCount(); i++)
	{
		if (m_pTreeWgtChoose->topLevelItem(i)->checkState(0) == Qt::Checked)
		{
			strlstCheck.append(m_pTreeWgtChoose->topLevelItem(i)->text(0));
		}
	}

	emit Signal_ChoosingInterfaces(strlstCheck);

	this->close();
}
//点击停止
void CChooseInterfaceWgt::Slot_StopCapturing()
{
	emit Singal_CloseCapturing();
	this->close();
}

CChooseInterfaceWgt::~CChooseInterfaceWgt()
{
	m_pTreeWgtChoose->deleteLater();
}


CCaptureModel::CCaptureModel(QObject *parent, int rowCount, int colCount)
	: QAbstractTableModel(parent)
{
	Q_UNUSED(rowCount);
	this->m_nRowCount = 0;
	this->m_nColCount = colCount;

	m_lstHeader.append(tr("No."));
	m_lstHeader.append((tr("ModuleName")));
	m_lstHeader.append(tr("Date"));
	m_lstHeader.append(tr("Time"));
	m_lstHeader.append(tr("Source"));

	m_lstHeader.append(tr("Destination"));
	m_lstHeader.append(tr("Protocol"));
	m_lstHeader.append(tr("SubType"));
	m_lstHeader.append(tr("Length"));
	m_lstHeader.append(tr("Info"));

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
CCaptureModel::~CCaptureModel()
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
int	CCaptureModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return COLUMN_NINE+1;
}

/*! \fn int CCaptureModel::rowCount(const QModelIndex &parent) const
*********************************************************************************************************
** \brief CCaptureModel::rowCount
** \details
** \param parent
** \return int
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
int CCaptureModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return  static_cast <int>  (m_arrLogs.size());
}
/*! \fn QVariant CCaptureModel::data(const QModelIndex &index, int role) const
*********************************************************************************************************
** \brief CCaptureModel::data
** \details
** \param index
** \param role
** \return QT_NAMESPACE::QVariant
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
QVariant CCaptureModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_arrLogs.size() || nRow < 0)
			return QVariant();

		std::shared_ptr<CCaptureMsg> pLog = m_arrLogs[nRow];
		
		Q_ASSERT(pLog != nullptr);
		if (pLog != nullptr)
		{
			if (nCol == COLUMN_ZERO)
			{
				return pLog->strNumber;
			}
			else if (nCol == CONUMN_ONE)
			{
				return pLog->strModuleName;
			}
			else if (nCol == COLUMN_TOW)
			{
				return pLog->tdata.toString("yyyy-MM-dd");
			}
			else if (nCol == COLUMN_THREE)
			{
				return pLog->tTime.toString("hh:mm:ss.zzz");
			}
			else if (nCol == COLUMN_FOUR)
			{
				return pLog->strSender;
			}
			else if (nCol == COLUMN_FIVE)
			{
				return pLog->strRecver;
			}
			else if (nCol == COLUMN_SIX)
			{
				return pLog->strProtrol;
			}
			else if (nCol == COLUMN_SEVEN)
			{
				return pLog->strType;
			}
			else if (nCol == COLUMN_EIGHT)
			{
				return pLog->nLength;
			}
			else if (nCol == COLUMN_NINE)
			{
				return pLog->strPartContext;
			}

			return QVariant();
		}
	}
	
	if (role == Qt::BackgroundRole)
	{
		int nRow = index.row();

		if (nRow >= m_arrLogs.size() || nRow < 0)
			return QVariant();

		std::shared_ptr<CCaptureMsg> pLog = m_arrLogs[nRow];

		Q_ASSERT(pLog != nullptr);

		if (pLog != nullptr)
		{
			return pLog->colorBackground;
		}

	}
	return QVariant();
}

QByteArray CCaptureModel::getDetailData(const QModelIndex &index)
{
	int nRow = index.row();

	if (nRow >= m_arrLogs.size() || nRow < 0)
	{
		return "";
	}

	std::shared_ptr<CCaptureMsg> pLog = m_arrLogs[nRow];

	if (pLog != nullptr)
	{
		return pLog->btDetailInfo;
	}
	return "";
}

QString CCaptureModel::GetColumnData(int nColumn, const QModelIndex &index)
{
	int nRow = index.row();
	int nCol = nColumn;

	if (nRow >= m_arrLogs.size() || nRow < 0)
		return "";

	std::shared_ptr<CCaptureMsg> pLog = m_arrLogs[nRow];

	if (pLog != nullptr)
	{
		if (nCol == COLUMN_ZERO)
		{
			return pLog->strNumber;
		}
		else if (nCol == CONUMN_ONE)
		{
			return pLog->strModuleName;
		}
		else if (nCol == COLUMN_TOW)
		{
			return pLog->tdata.toString("yyyy-MM-dd");
		}
		else if (nCol == COLUMN_THREE)
		{
			return pLog->tTime.toString("hh:mm:ss.zzz");
		}
		else if (nCol == COLUMN_FOUR)
		{
			return pLog->strSender;
		}
		else if (nCol == COLUMN_FIVE)
		{
			return pLog->strRecver;
		}
		else if (nCol == COLUMN_SIX)
		{
			return pLog->strProtrol;
		}
		else if (nCol == COLUMN_SEVEN)
		{
			return pLog->strType;
		}
		else if (nCol == COLUMN_EIGHT)
		{
			return QString::number(pLog->nLength);
		}
		else if (nCol == COLUMN_NINE)
		{
			return pLog->strPartContext;
		}
	}

	return "";
}

void CCaptureModel::removeFirstRow()
{
	beginResetModel();
	m_arrLogs.erase(m_arrLogs.begin());
	endResetModel();
}


QString CCaptureModel::getTotalData()
{

	QString strLineData;

	for (int i = 0; i < m_arrLogs.size(); i++)
	{
		strLineData.append(m_arrLogs.at(i)->strNumber + " " + m_arrLogs.at(i)->strModuleName + " " + m_arrLogs.at(i)->tdata.toString("yyyy-MM-dd") + " " +
			m_arrLogs.at(i)->tTime.toString("hh:mm:ss.zzz") + " " + m_arrLogs.at(i)->strSender + " " + m_arrLogs.at(i)->strRecver + " " +
			m_arrLogs.at(i)->strProtrol + " " + m_arrLogs.at(i)->strType + " " + QString::number(m_arrLogs.at(i)->nLength) + " " +
			m_arrLogs.at(i)->btDetailInfo.toHex() + "\n");
	}
	return strLineData;
}


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


void  CCaptureModel::AddItem(CCaptureMsg *pLog, QColor& color)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
	{
		return;
	}
		
	pLog->colorBackground = color;
	std::shared_ptr<CCaptureMsg>ptrLog = std::make_shared<CCaptureMsg>(*pLog);
	size_t nRow = m_arrLogs.size();

	beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);
	m_arrLogs.push_back(ptrLog);
	endInsertRows();

}

/*! \fn void CSystemOutputModel::ClearAll()
*********************************************************************************************************
** \brief CSystemOutputModel::ClearAll
** \details
** \return void
** \author gw
** \date 2016年4月12日
** \note
********************************************************************************************************/
void CCaptureModel::ClearAll()
{
	beginResetModel();
	m_arrLogs.clear();
	endResetModel();
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
QVariant CCaptureModel::headerData(int section, Qt::Orientation orientation, int role) const
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




CCaptureMsgWgt::CCaptureMsgWgt(QWidget *parent)
	: QWidget(parent),
	m_pTableModel(new CCaptureModel(this)),
	m_pChooseWgt(new CChooseInterfaceWgt)
{
	ui.setupUi(this);
	m_nIndexNum = 0;
	m_bCapturing = false;
	InitCapTureWgt();
	connect(ui.btnInterfaces, SIGNAL(clicked()), this, SLOT(Slot_ClickChooseInterfaces()));
	connect(m_pChooseWgt, SIGNAL(Signal_ChoosingInterfaces(QStringList)), this, SLOT(Slot_BeginCapturing(QStringList)));
	connect(m_pChooseWgt, SIGNAL(Singal_CloseCapturing()), this, SLOT(Slot_endCapturing()));
	connect(ui.tableViewCurrentData, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_ClickShowDetailInfo(const QModelIndex &)));
	//停止检测
	connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(Slot_endCapturing()));
	//重新检测
	connect(ui.btnRestart, SIGNAL(clicked()), this, SLOT(Slot_RestartCapturing()));
	//导出
	connect(ui.btnExport, SIGNAL(clicked()), this, SLOT(Slot_ClickExport()));
	//筛选
	connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(Slot_ClickFilter()));
	//
	ui.frame_3->setHidden(true);
	this->startTimer(100);

	ui.splitter->setStretchFactor(0, 9);
	ui.splitter->setStretchFactor(1, 1);
	ui.splitter->setStretchFactor(2, 1);

}

CCaptureMsgWgt * CCaptureMsgWgt::GetInstance()
{
	return Singleton<CCaptureMsgWgt, LeakySingletonTraits<CCaptureMsgWgt>>::instance();
}

void CCaptureMsgWgt::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);
	//加锁
	QMutexLocker locker(&m_mutexLock);//加互斥锁
	QList<RECVOUTMSG> lstRecvMsg = m_lstRecvMsg;
	m_lstRecvMsg.clear();
	locker.unlock();

	if (lstRecvMsg.count() == 0)
	{
		return;
	}

	if (m_bCapturing)
	{
		for (int i = 0; i < lstRecvMsg.count(); i++)
		{
			if (!m_strlstCapturing.contains(lstRecvMsg.at(i).moduleName))
			{
				return;
			}

			QString strData = "";
			QByteArray btArray(lstRecvMsg.at(i).msgContext, lstRecvMsg.at(i).msgLegth);

			if (lstRecvMsg.at(i).msgLegth > 6)
			{
				strData = btArray.mid(0, 19).toHex();
			}
			else
			{
				strData = btArray.toHex();
			}

			QString newstrData = strData.toUpper();
			QString strShowData;
			for (int m = 0; m < newstrData.count(); m++)
			{
				if (m % 2 == 0 && m != 0)
				{
					strShowData.append(" ");
				}

				strShowData.append(newstrData.at(m));
			}
			if (strShowData.count() > 20)
			{
				strShowData.append("...");
			}

			UpdateDataToTable(lstRecvMsg.at(i), strShowData);
		}
	}
}

//插入数据到表格
void CCaptureMsgWgt::UpdateDataToTable(const RECVOUTMSG &sMsgdata, const QString &strShowData)
{
	//更新表格数据
	m_nIndexNum++;
	ui.tableViewCurrentData->scrollToBottom();

	CCaptureMsg tcapMsg;
	tcapMsg.strNumber = QString::number(m_nIndexNum);
	tcapMsg.strModuleName = sMsgdata.moduleName;
	tcapMsg.tdata = QDateTime::currentDateTime().date();
	tcapMsg.tTime = QDateTime::currentDateTime().time();
	tcapMsg.strSender = sMsgdata.dataSoruce;
	tcapMsg.strRecver = sMsgdata.dataDesc;
	tcapMsg.strType = sMsgdata.subType;
	tcapMsg.nLength = sMsgdata.msgLegth;
	tcapMsg.strPartContext = strShowData;
	tcapMsg.btDetailInfo.append(sMsgdata.msgContext, sMsgdata.msgLegth);

	if (sMsgdata.msgType == IMainModule::CMSG_TCP_SEND)
	{
		tcapMsg.strProtrol = tr("TCP");
		m_pTableModel->AddItem(&tcapMsg, QColor(170, 255, 127));
	}
	else if (sMsgdata.msgType == IMainModule::CMSG_TCP_RECV)
	{
		tcapMsg.strProtrol = tr("TCP");
		m_pTableModel->AddItem(&tcapMsg, QColor(170, 255, 255));
	}
	else if (sMsgdata.msgType == IMainModule::CMSG_UDP_SEND)
	{
		tcapMsg.strProtrol = tr("UDP");
		m_pTableModel->AddItem(&tcapMsg, QColor(255, 255, 127));
	}
	else if (sMsgdata.msgType == IMainModule::CMSG_UDP_RECV)
	{
		tcapMsg.strProtrol = tr("UDP");
		m_pTableModel->AddItem(&tcapMsg, QColor(131, 208, 255));
	}
	else if (sMsgdata.msgType == IMainModule::CMSG_SERIALPORT_SEND)
	{
		tcapMsg.strProtrol = tr("Send");
		m_pTableModel->AddItem(&tcapMsg, QColor(189, 240, 255));
	}
	else if (sMsgdata.msgType == IMainModule::CMSG_SERALPORT_RECV)
	{
		tcapMsg.strProtrol = tr("Serial Recv");
		m_pTableModel->AddItem(&tcapMsg, QColor(219, 206, 255));
	}
	else
	{
		tcapMsg.strProtrol = QString::number(sMsgdata.msgType);
		m_pTableModel->AddItem(&tcapMsg, QColor(255, 0, 0));
	}

	while (m_pTableModel->rowCount() >= MAXCAPTURENUMS)
	{
		m_pTableModel->removeFirstRow();
	}

	if (m_nIndexNum >= MAXCAPTURENUMS)
	{
		m_nIndexNum = 0;
	}
}

//初始化界面
void CCaptureMsgWgt::InitCapTureWgt()
{
	ui.tableViewCurrentData->setModel(m_pTableModel);
	ui.tableViewCurrentData->verticalHeader()->setHidden(true);
	ui.tableViewCurrentData->setSortingEnabled(true);
	ui.tableViewCurrentData->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableViewCurrentData->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableViewCurrentData->horizontalHeader()->setStretchLastSection(true);

	ui.treeWidgetDetail->header()->setHidden(true);
	ui.treeWidgetDetail->setColumnCount(2);
	ui.treeWidgetDetail->setColumnWidth(0, 300);

	ui.tableWidgetNum->setColumnCount(1);
	ui.tableWidgetNum->horizontalHeader()->setHidden(true);
	ui.tableWidgetNum->verticalHeader()->setHidden(true);
	ui.tableWidgetNum->setColumnWidth(0, 50);
	ui.tableWidgetNum->setFixedWidth(50);
	ui.tableWidgetNum->setStyleSheet("border-style: solid;gridline-color: rgb(255, 255, 255);");
	ui.tableWidgetNum->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidgetNum->setFocusPolicy(Qt::NoFocus);

	ui.tableWidget16Info->setFixedWidth(425);
	ui.tableWidget16Info->setColumnCount(17);
	ui.tableWidget16Info->horizontalHeader()->setHidden(true);
	ui.tableWidget16Info->verticalHeader()->setHidden(true);
	ui.tableWidget16Info->setStyleSheet("border-style: solid;gridline-color: rgb(255, 255, 255);");
	ui.tableWidget16Info->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget16Info->setFocusPolicy(Qt::NoFocus);

	for (int i = 0; i < 17; i++)
	{
		ui.tableWidget16Info->setColumnWidth(i,25);
	}


	ui.tableWidgetSymbal->setFixedWidth(255);
	ui.tableWidgetSymbal->setColumnCount(17);
	ui.tableWidgetSymbal->horizontalHeader()->setHidden(true);
	ui.tableWidgetSymbal->verticalHeader()->setHidden(true);
	ui.tableWidgetSymbal->setStyleSheet("border-style: solid;gridline-color: rgb(255, 255, 255);");
	ui.tableWidgetSymbal->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidgetSymbal->setFocusPolicy(Qt::NoFocus);

	for (int i = 0; i < 17; i++)
	{
		ui.tableWidgetSymbal->setColumnWidth(i, 15);
	}

	ui.tableViewCurrentData->setColumnWidth(0, 60);
	ui.tableViewCurrentData->setColumnWidth(1, 110);
	ui.tableViewCurrentData->setColumnWidth(2, 80);
	ui.tableViewCurrentData->setColumnWidth(3, 100);
	ui.tableViewCurrentData->setColumnWidth(4, 110);
	ui.tableViewCurrentData->setColumnWidth(5, 110);
	ui.tableViewCurrentData->setColumnWidth(6, 80);
	ui.tableViewCurrentData->setColumnWidth(7, 80);
	ui.tableViewCurrentData->setColumnWidth(8, 80);
}

//点击选择接口
void CCaptureMsgWgt::Slot_ClickChooseInterfaces()
{
	m_pChooseWgt->show();
}

//筛选
void CCaptureMsgWgt::Slot_ClickFilter()
{
    //筛选界面
	QMessageBox::warning(this, tr("Warning"), tr("this function dose not develop"));
}


//读取表格中的文件写入到execl表格中
void CCaptureMsgWgt::ExportDataToFile(const QString &strFileName)
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
    out << m_pTableModel->getTotalData();
}

//导出
void CCaptureMsgWgt::Slot_ClickExport()
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


//开始检测
void CCaptureMsgWgt::Slot_BeginCapturing(const QStringList &strlstCode)
{
	m_strlstCapturing.clear();
	m_strlstCapturing = strlstCode;
	m_bCapturing = true;
	//清空表格
	
	m_pTableModel->ClearAll();
	ui.treeWidgetDetail->clear();
	ui.tableWidgetNum->clearContents();
	ui.tableWidgetNum->setRowCount(0);
	ui.tableWidget16Info->setRowCount(0);
	ui.tableWidgetSymbal->setRowCount(0);
}

//停止检测
void CCaptureMsgWgt::Slot_endCapturing()
{
	m_bCapturing = false;
	//暂时不保存文件
}

//重新检测
void CCaptureMsgWgt::Slot_RestartCapturing()
{
	//暂时不保存文件

	m_bCapturing = true;
	m_pTableModel->ClearAll();
	ui.treeWidgetDetail->clear();
	ui.tableWidgetNum->clearContents();
	ui.tableWidgetNum->setRowCount(0);
	ui.tableWidget16Info->setRowCount(0);
	ui.tableWidgetSymbal->setRowCount(0);
	m_nIndexNum = 0;
}

//点击显示详细信息
void CCaptureMsgWgt::Slot_ClickShowDetailInfo(const QModelIndex &modelIndex)
{

	//清空树形结构
	ui.treeWidgetDetail->clear();
	//查找对应的而数据
	QByteArray byData = m_pTableModel->getDetailData(modelIndex);

	//往树形结构中插入数据
	QTreeWidgetItem *pTopBaseInfoItem = new QTreeWidgetItem;
	pTopBaseInfoItem->setText(0,tr("Data Base Info:"));
	
	QTreeWidgetItem *pDataFromModule = new QTreeWidgetItem;
	pDataFromModule->setText(0, tr("Data From the Module:"));
	pDataFromModule->setText(1, m_pTableModel->GetColumnData(1,modelIndex));

	QTreeWidgetItem *pRecvTimeItem = new QTreeWidgetItem;
	pRecvTimeItem->setText(0, tr("Receive Message Time:"));
	pRecvTimeItem->setText(1, m_pTableModel->GetColumnData(2, modelIndex) + " " + m_pTableModel->GetColumnData(3, modelIndex));

	QTreeWidgetItem *PProtrolItem = new QTreeWidgetItem;
	PProtrolItem->setText(0, tr("Source Address:"));
	PProtrolItem->setText(1, m_pTableModel->GetColumnData(4, modelIndex).split(" ").at(0));

	QTreeWidgetItem *pElecProtrolItem = new QTreeWidgetItem;
	pElecProtrolItem->setText(0, tr("Destination Address:"));
	pElecProtrolItem->setText(1, m_pTableModel->GetColumnData(5, modelIndex));

	QTreeWidgetItem *pBagLength = new QTreeWidgetItem;
	pBagLength->setText(0, tr("Protocol:"));
	pBagLength->setText(1, m_pTableModel->GetColumnData(6, modelIndex));

	pTopBaseInfoItem->addChild(pDataFromModule);
	pTopBaseInfoItem->addChild(pRecvTimeItem);
	pTopBaseInfoItem->addChild(PProtrolItem);
	pTopBaseInfoItem->addChild(pElecProtrolItem);
	pTopBaseInfoItem->addChild(pBagLength);

	ui.treeWidgetDetail->addTopLevelItem(pTopBaseInfoItem);
	pTopBaseInfoItem->setExpanded(true);
	
	AnalyseData(byData);
}

//解析数据
void  CCaptureMsgWgt::AnalyseData(const QByteArray &btBuff)
{
	//数据
	QTreeWidgetItem *pTopDataInfoItem = new QTreeWidgetItem;
	pTopDataInfoItem->setText(0, tr("Data Detail Infomation:"));

	QTreeWidgetItem *pStartNumItem = new QTreeWidgetItem;
	pStartNumItem->setText(0, tr("Start Number:"));
	pStartNumItem->setText(1, btBuff.mid(0,1).toHex().toUpper());

	QTreeWidgetItem *pDetailItem = new QTreeWidgetItem;
	pDetailItem->setText(0, tr("Detail Infomation:"));
	//pDetailItem->setText(1, btBuff.toHex().toUpper());

	QTextEdit *pTxtDetail = new QTextEdit;
	pTxtDetail->setStyleSheet("border-style: solid;font: 9pt Arial;");
	QString strHex = btBuff.toHex().toUpper();
	QString strNewHex;
	//pTxtDetail->setText(btBuff.toHex().toUpper());

	for (int i = 0; i < strHex.count(); i++)
	{
		if (i % 2 == 0 && i != 0)
		{
			strNewHex.append(" ");
		}

		strNewHex.append(strHex.at(i));
	}

	pTxtDetail->setText(strNewHex);

	pTxtDetail->setFixedSize(650, 100);
	
	pTopDataInfoItem->addChild(pStartNumItem);
	pTopDataInfoItem->addChild(pDetailItem);

	ui.treeWidgetDetail->addTopLevelItem(pTopDataInfoItem);

	ui.treeWidgetDetail->setItemWidget(pDetailItem, 1, pTxtDetail);

	pTopDataInfoItem->setExpanded(true);

	InsertDataToTables(strNewHex,btBuff);
}

//更新监控数据
//模块名称 数据内容  数据长度  数据类型（tcp udp serial） 数据标识（总召、遥信、遥控等）
void CCaptureMsgWgt::UpdateCapturingData(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType,const char *pSource,const char *pDesc)
{
	RECVOUTMSG sRecvMsg;
	sRecvMsg.moduleName = pszModuleName;
	sRecvMsg.dataDesc = pDesc;
	sRecvMsg.dataSoruce = pSource;
	sRecvMsg.msgContext.append(pMsg,nMsgLen);
	sRecvMsg.msgLegth = nMsgLen;
	sRecvMsg.msgType = nMsgType;
	sRecvMsg.subType = pMsgSubType;

	//多线程调用
	//加锁
	QMutexLocker locker(&m_mutexLock);//加互斥锁
	m_lstRecvMsg.append(sRecvMsg);
}

//往表格中插入数据
void CCaptureMsgWgt::InsertDataToTables(const QString &byBuff, const QByteArray &btBuff)
{
	//清空所有数据
	ui.tableWidgetNum->clearContents();
	ui.tableWidgetNum->setRowCount(0);

	ui.tableWidget16Info->clearContents();
	ui.tableWidget16Info->setRowCount(0);

	ui.tableWidgetSymbal->clearContents();
	ui.tableWidgetSymbal->setRowCount(0);

	QStringList lstCode = byBuff.split(" ");
	if (lstCode.length() != btBuff.length())
	{
		return;
	}
	int iColumn = 0;
	for (int i = 0; i < lstCode.count(); i++)
	{
		if ( (i%15==0) && (i!=lstCode.count()-1))
		{
			//增加一行
			int iCurrentRow = ui.tableWidgetNum->rowCount();
			QString strNum = "0000" + QString::number(iCurrentRow * 10);
			strNum = strNum.mid(strNum.count() - 4, 4);

			ui.tableWidgetNum->insertRow(iCurrentRow);
			//设置ROw高度
			ui.tableWidgetNum->setRowHeight(iCurrentRow,20);
			QTableWidgetItem *pNumItem = new QTableWidgetItem;
			pNumItem->setText(strNum);
			ui.tableWidgetNum->setItem(iCurrentRow, 0, pNumItem);
		}

		iColumn = i % 16 + i % 16 / 8;
		int iRow = i / 16;

		if (ui.tableWidget16Info->rowCount() < iRow+1)
		{
			ui.tableWidget16Info->insertRow(iRow);
			ui.tableWidget16Info->setRowHeight(iRow, 20);
		}
		if (ui.tableWidgetSymbal->rowCount() < iRow + 1)
		{
			ui.tableWidgetSymbal->insertRow(iRow);
			ui.tableWidgetSymbal->setRowHeight(iRow, 20);
		}
	    QTableWidgetItem *pSixItem = new QTableWidgetItem;
		pSixItem->setText(lstCode.at(i));
	    ui.tableWidget16Info->setItem(iRow, iColumn,pSixItem);

		QTableWidgetItem *pAssItem = new QTableWidgetItem;
		//char pdata = lstCode.at(i).toInt();
		//QString strData = pdata + "";
		if (btBuff.at(i)>32 && btBuff.at(i)<126)
		{
			pAssItem->setText(QString(btBuff.at(i)));
		}
		else
		{
			pAssItem->setText(".");
		}
		
		ui.tableWidgetSymbal->setItem(iRow, iColumn, pAssItem);
	}
}

//增加选项
void CCaptureMsgWgt::AddPluginSection(const QString &strPluginDeviceName)
{
	m_pChooseWgt->AddPluginSection(strPluginDeviceName);
}

CCaptureMsgWgt::~CCaptureMsgWgt()
{
	if (m_pChooseWgt != NULL)
	{
		delete m_pChooseWgt;
	}
	m_pChooseWgt = nullptr;
}
