#include <QStandardItemModel>
#include <QModelIndex>
#include <QMessageBox>
#include <QTableView>
#include <QLibrary>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QMovie>
#include <QThread>
#include <QLabel>
#include <QMenu>
#include <QTranslator>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QProcess>
#include <QDockWidget>

#include "ftu_module.h"
#define MODULE_LIB
#include "devicestudio/module_export.h"
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "devicestudio/treewidget.h"
#include "../../../src/devicestudio/capturemsgwgt.h"
#include "realtimebinary.h"
#include "realtimeanalog.h"
#include "realtimesoe.h"
#include "realtimekwh.h"
#include  "commthread.h"
#include "icommplugin.h"
#include "choosepointswgt.h"
#include "remoteponittableanalyse.h"
#include "devicestudio/treeitem_def.h"
#include "devicestudio/tabwidget.h"
#include "infoconfigwgt.h"
#include "pointinfo.h"
#include "remotecontrolwgt.h"
#include "fixddeploy.h"
#include "recordwgt.h"
#include "maintecancewgt.h"
#include "checkremotewgt.h"
#include "historywgt.h"
#include "cmdhistorywgt.h"
#include "analyse104msg.h"
#include "sycsconfigdlg.h"
#include "lcdoperatorwgt.h"
#include "define_104.h"
#include "faulteventwgt.h"
#include "webdevicewgt.h"
#include "fixoperatewgt.h"
#include "filetranswgt.h"

#if _MSC_VER >= 1600
//#include <vld.h>
#endif
//#define COMMUNICATEPLUGINNAME "commplugin.dll"

static CFtuModule *s_ptrModule = nullptr;

extern "C" DS_EXPORT IPluginModule *CreateModule()
{


	s_ptrModule = new CFtuModule;
	return s_ptrModule;
//	return  s_ptrModule.get();
}

typedef ICommPlugin *(*FunCommModel)();

CFtuModule::CFtuModule()
{

	QTranslator *qtPlugini = new QTranslator(this);
	QString strRunPath = qApp->applicationDirPath();

	if (qtPlugini->load(strRunPath + "/locales/f220-zh-CN.qm"))
	{
		qApp->installTranslator(qtPlugini);
	}

	//m_pCommunicatePlugin = nullptr;
	m_iCurrentPluginIndex = 0;
	m_pRealTimeBinaryWgt = nullptr;
	m_pRealTimeAnalog = nullptr;
	m_pRealTimeSoe = nullptr;
	m_pRealTimeKwh = nullptr;
	m_pChoosePointWgt = nullptr;
	m_pReomteControl = nullptr;
	m_pFixDeploy = nullptr;
	m_pPluginTopItem = nullptr;
    m_pRecord = nullptr;
    m_pMaintencanceWgt = nullptr;
	m_pFileTransWgt = nullptr;
    m_pHistory = nullptr;
    m_pCmdHistory = nullptr;
    m_pPointInfo = nullptr;
    m_pCommProcess = nullptr;
    m_pAnalyse104 = nullptr;
    m_pSycsConfigWgt = nullptr;
    m_pLcdWgt = nullptr;
	m_pWebDevice = nullptr;
	m_pCommThread = new CCommThread(this);
	m_nFixAreaCode = 0;

}

void CFtuModule::Init(IMainModule *pMainModule)
{
	Q_ASSERT(pMainModule);
	if (!pMainModule)
	{
		return;
	}

	m_pMainModule = pMainModule;
	//获取主界面对象
	m_pMainWindow = pMainModule->GetMainWindow();
    
	Q_ASSERT(m_pMainWindow);
	if (!m_pMainWindow)
	{
		return;
	}
    m_pCommProcess = new QProcess(this);

	//在首次调用时会动态加载动态库 需要时间
// 	QMimeData *mimeData = new QMimeData;
// 
// 	QDrag *drag2 = new QDrag(this);
// 	drag2->setMimeData(mimeData);
// 	drag2->exec();
	//end

	m_pConfigWgt = new CInfoConfigWgt;
	QString strIP;
	int iPort = 0;
	QString strPointTableName;
	if (m_pConfigWgt->exec() == QDialog::Accepted)
    {
		strIP = m_pConfigWgt->GetIpAddress();
		iPort = m_pConfigWgt->GetPort();
		strPointTableName = m_pConfigWgt->GetPointTable();
        m_pConfigWgt->setCursor(Qt::WaitCursor);

        InitCallZt();
    }
	else
	{
		strIP = m_pConfigWgt->GetIpAddress();
		iPort = m_pConfigWgt->GetPort();
		strPointTableName = m_pConfigWgt->GetPointTable();
		//return;
        
        return;
	}

	if (strPointTableName.isEmpty())
	{
		qApp->exit();
	}
	
	CreateTreeItem();
	connect(m_pMainWindow->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_ClickLeftTreeItem(const QModelIndex &)));
	connect(m_pMainWindow->GetTabWidget(), SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_CloseOneTabWidget(int)));

    m_pCommThread->SetGeneralTimes(m_pConfigWgt->GetGereralTime(),m_pConfigWgt->GetSyncGenertalTime(),m_pConfigWgt->GetKwhCallTime());

	m_pCommThread->StartRun(strIP.toStdString().c_str(), iPort);
	connect(m_pCommThread, SIGNAL(Signal_SocketError(QString)), this, SLOT(Slot_SocketError(QString)));
	connect(m_pCommThread, SIGNAL(Signal_SocketConnectSuccess()), this, SLOT(Slot_SocketSuccess()));
	connect(m_pCommThread, SIGNAL(Signal_AllCallRespond()), this, SLOT(Slot_TotalCallResponseSuccess()));
	//发送和接收 信息槽
	connect(m_pCommThread, SIGNAL(Signal_Write16Data(QByteArray)), this, SLOT(Slot_Send16Data(QByteArray)));
	connect(m_pCommThread, SIGNAL(Signal_recv16Data(QByteArray, int)), this, SLOT(Slot_Recv16Data(QByteArray, int)));
	//遥控反馈
	connect(m_pCommThread, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)), this, SLOT(Slot_ControlFeedback(int, int, int, QString)));
	//加载点表数据
	m_pPointTableAnalyse = new CRemotePonitTableAnalyse(this);

	QString strRunPath = qApp->applicationDirPath();

	if (!m_pPointTableAnalyse->OpenPointTableFile(strRunPath + POINTTABLEPATH + strPointTableName + ".txt"))
	{
		return;
	}

	m_pPointInfo = new CPointInfo;
	m_pChoosePointWgt = new CChoosePointsWgt(m_pPointInfo);
	m_pChoosePointWgt->InitWgt(m_pPointTableAnalyse);

    m_pChoosePointWgt->SetProjectPath(strRunPath + PROJECTPATH + m_pConfigWgt->GetProjectName());

	m_pChoosePointWgt->Slot_UpdateAllChoosePoints(strRunPath + REMOTETABLE + "iec104sList.ini");

	m_pChoosePointWgt->Slot_UpdateBinaryPointData();
	m_pChoosePointWgt->Slot_UpdateKwhPointData();
	m_pChoosePointWgt->Slot_UpdateAnalogPointData();
	//遥信实时数据
    m_pRealTimeBinaryWgt = new CRealTimeBinary(0, m_pCommThread, m_pPointInfo, m_pConfigWgt);
	//遥测实时数据
    m_pRealTimeAnalog = new CRealTimeAnalog(0, m_pCommThread, m_pPointInfo, m_pPointTableAnalyse, m_pConfigWgt);
	//SOE数据
    m_pRealTimeSoe = new CRealTimeSOE(0, m_pCommThread, m_pPointInfo, m_pConfigWgt);
	//kwh数据
    m_pRealTimeKwh = new CRealTimeKwh(0, m_pCommThread, m_pPointInfo, m_pConfigWgt);
	//遥控
    m_pReomteControl = new CRemoteControlWgt(m_pCommThread,0, m_pConfigWgt);
    //故障事件
    m_FaultEventWgt = new CFaultEventWgt(m_pCommThread, m_pPointInfo, m_pConfigWgt);
    

    m_pFixDeploy = new CFixdDeploy(m_pCommThread, pMainModule, m_strDeviceName.toLocal8Bit().data());
    //录波
    m_pRecord = new CRecordWgt(m_pCommThread, pMainModule, m_strDeviceName.toLocal8Bit().data());
    m_pRecord->SetRecordPath(strRunPath + PROJECTPATH + m_pConfigWgt->GetProjectName() + "/COMTRADE");

    m_pRecord->AnalyseRecordFile(strRunPath + RECORDPATH + strPointTableName + ".xml");
    m_pRecord->SetRecoredConifgName(strRunPath + RECORDPATH + strPointTableName + ".xml");
    //模拟液晶
    m_pLcdWgt = new CLcdoperatorWgt;

    //维护调试功能
    m_pMaintencanceWgt = new CMaintecanceWgt(m_pCommThread,m_pConfigWgt, m_pLcdWgt, strRunPath + REMOTETABLE + "iec104sList.ini");
    m_pMaintencanceWgt->SetRecordConifgName(strRunPath + RECORDPATH + strPointTableName + ".xml");

	//文件传输
	m_pFileTransWgt = new CFileTransWgt(m_pCommThread);

    m_pFixDeploy->SetPorjectName(strRunPath + PROJECTPATH + m_pConfigWgt->GetProjectName());

    m_pFixDeploy->InitWgt(strRunPath + DEVCONFIGPATH + strPointTableName + ".conf");

    QDir tDir(strRunPath + REMOTETABLE);
    QStringList lstFiles = tDir.entryList(QDir::Files);

    
    
    //历史数据
    m_pHistory = new CHistoryWgt(m_pChoosePointWgt);
    //操作日志
    m_pCmdHistory = new CCmdHistoryWgt;
	//web
	m_pWebDevice = new CWebDeviceWgt;

	//树结构增加右击菜单
	m_pMainWindow->GetLeftTree()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pMainWindow->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
    //更新测点
    connect(m_pMaintencanceWgt->GetftpPointWgt(), SIGNAL(Signal_FileDownLoadSuccess(const QString &)), m_pChoosePointWgt, SLOT(Slot_UpdateAllChoosePoints(const QString &)));
    //更新录波配置文件
    connect(m_pMaintencanceWgt, SIGNAL(Signal_UpdateRecordFile()), m_pRecord, SLOT(Slot_UpdateFileAct()));
    m_pConfigWgt->setCursor(Qt::ArrowCursor);

    //m_pRealTimeAnalog->AnalyseRealAnalogData(0, 16385 + 93, 3413, 1, "ggg");

    //报文数据部分
    m_pAnalyse104 = new CAnalyse104Msg(pMainModule);
    connect(pMainModule->GetCaptureWidget(), SIGNAL(Signal_CurrentClickBtData(QByteArray &)), m_pAnalyse104, SLOT(Slot_ClickOneCaptureInfo(QByteArray &)));
    //tab分离
    connect(m_pMainWindow->GetTabWidget(), SIGNAL(tabBarDoubleClicked(int)), this, SLOT(Slot_DoubleClickTabBar(int)));

    m_pMonitorDockWgt = new QDockWidget(m_pMainWindow);
    m_pMonitorDockWgt->setWindowTitle("Monitor");
    //时间同步
    m_pSycsConfigWgt = new CSycsConfigDlg;
    connect(m_pSycsConfigWgt, SIGNAL(Signal_SycsTime(QDateTime)), m_pCommThread, SIGNAL(Signal_SycsTime(QDateTime)));

    m_pReomteControl->SetRemoteTableInfo(m_pPointInfo);


}

CFtuModule::~CFtuModule()
{
	m_pCommThread->quit();
	m_pCommThread->wait();
	m_pCommThread->deleteLater();
	m_pChoosePointWgt->deleteLater();
	m_pRealTimeBinaryWgt->deleteLater();
	m_pRealTimeAnalog->deleteLater();
	m_pRealTimeSoe->deleteLater();
	m_pRealTimeKwh->deleteLater();
	m_pConfigWgt->deleteLater();
    m_pCommProcess->deleteLater();
    m_pAnalyse104->deleteLater();
    m_pSycsConfigWgt->deleteLater();
    m_pLcdWgt->deleteLater();
    //故障事件
    m_FaultEventWgt->deleteLater();

    if (m_pPointInfo != nullptr)
    {
        delete m_pPointInfo;
    }
	
    m_pReomteControl->deleteLater();
	m_pFixDeploy->deleteLater();
    m_pRecord->deleteLater();

    if (m_pMaintencanceWgt != nullptr)
    {
        delete m_pMaintencanceWgt;
    }
    
    m_pHistory->deleteLater();
    m_pCmdHistory->deleteLater();
	m_pFileTransWgt->deleteLater();
// 	delete m_pWebDevice;
}

bool CFtuModule::CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc)
{
	return m_pMainModule->CommMsgLog(pszModuleName, pMsg, nMsgLen, nMsgType, pMsgSubType, pSource, pDesc);
}

//socket连接成功
void CFtuModule::Slot_SocketSuccess()
{
    m_pFixDeploy->SetDeviceName(m_strDeviceName);
	QByteArray byDestr = tr("Connect To Socket Server Success").toLocal8Bit();
	m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), byDestr.data(), 1);
}
//收到总召响应
void CFtuModule::Slot_TotalCallResponseSuccess()
{
	QByteArray byDestr = tr("Receive All Call Response").toLocal8Bit();
	m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), byDestr.data(), 1);
}

//网络socke错误
void CFtuModule::Slot_SocketError(QString strError)
{
	m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), strError.toStdString().c_str(), 1);
}

//收到的信息
void CFtuModule::Slot_Recv16Data(QByteArray buff, int nLenght)
{
    QString strType;
    if (buff.length() > 6)
    {
        int nType = unsigned char(buff[6]);
        strType = QString::number(nType);

        if (nType == 103)
        {
            AnalyseSycsTime(buff,nLenght,2);
        }
        
    }

    CommMsgLog(m_strDeviceName.toStdString().c_str(), buff, nLenght, IMainModule::CMSG_TCP_RECV, strType.toStdString().c_str(), m_pCommThread->GetOwnNetInfo().toStdString().c_str(), m_pCommThread->GetOtherInfo().toStdString().c_str());
}

//发送的信息
void CFtuModule::Slot_Send16Data(QByteArray buff)
{
    QString strType;
    if (buff.length() > 6)
    {
        int nType = (unsigned char)buff[6];
        strType = QString::number(nType);

        if (nType == 103)
        {
            AnalyseSycsTime(buff, buff.length(), 1);
        }
    }

    CommMsgLog(m_strDeviceName.toStdString().c_str(), buff.data(), buff.length(), IMainModule::CMSG_TCP_SEND, strType.toStdString().c_str(), m_pCommThread->GetOtherInfo().toStdString().c_str(), m_pCommThread->GetOwnNetInfo().toStdString().c_str());
}
//遥控反馈
void CFtuModule::Slot_ControlFeedback(int iControlType, int iPointNum, int iOperateType, QString strStatus)
{
	m_pReomteControl->InsertDataFromFeedBack(iControlType, iPointNum, iOperateType, strStatus);
}

//树右击菜单
void CFtuModule::Slot_ContextMenuRequest(const QPoint &cPoint)
{
	Q_UNUSED(cPoint);

	QModelIndex index = m_pMainWindow->GetLeftTree()->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	QStandardItemModel *itemModel = qobject_cast<QStandardItemModel*>(m_pMainWindow->GetLeftTree()->model());
	if (itemModel == nullptr)
	{
		return;
	}
	if (itemModel->itemFromIndex(index) != m_pPluginTopItem)
	{
		return;
	}

	QMenu *pLeftMenu = new QMenu(m_pMainWindow->GetLeftTree());

	QAction *pProgramConfigAct = new QAction(tr("Program Config"), m_pMainWindow->GetLeftTree());

	connect(pProgramConfigAct, SIGNAL(triggered()), this, SLOT(Slot_SetConfigAct()));

	pLeftMenu->addAction(pProgramConfigAct);


	pLeftMenu->exec(QCursor::pos());
	pLeftMenu->deleteLater();
}

//配置action
void CFtuModule::Slot_SetConfigAct()
{
	if (m_pConfigWgt->exec() == QDialog::Accepted)
	{
		QString strIP = m_pConfigWgt->GetIpAddress();
		int iPort = m_pConfigWgt->GetPort();

		QString strPointTableName = m_pConfigWgt->GetPointTable();

        m_pCommThread->SetGeneralTimes(m_pConfigWgt->GetGereralTime(), m_pConfigWgt->GetSyncGenertalTime(), m_pConfigWgt->GetKwhCallTime());
		m_pCommThread->StartRun(strIP.toStdString().c_str(), iPort);

		QString strRunPath = qApp->applicationDirPath();

		if (m_pPointTableAnalyse->OpenPointTableFile(strRunPath + POINTTABLEPATH + strPointTableName + ".txt"))
		{
			m_pChoosePointWgt->InitWgt(m_pPointTableAnalyse);
            m_pChoosePointWgt->SetProjectPath(strRunPath + PROJECTPATH + m_pConfigWgt->GetProjectName());

            m_pReomteControl->SetRemoteTableInfo(m_pPointInfo);

			QString strRunPath = qApp->applicationDirPath();
			m_pFixDeploy->InitWgt(strRunPath + DEVCONFIGPATH + strPointTableName + ".conf");

			return;
		}
	}
	else
	{
		return;
	}
}


void CFtuModule::UnInit()
{

}

//设置加载的插件的名称
void CFtuModule::SetLoadDeviceName(const QString &strDeviceName)
{
	m_strDeviceName = strDeviceName;
}
void CFtuModule::CreateTreeItem()
{
	//FTU
	QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pMainWindow->GetLeftTree()->model();
	QStandardItem *pTopItem = (QStandardItem*)pTreeModel->item(0, 0);

	int iDevice_num = 1;
	for (int i = 0; i < pTopItem->rowCount(); i++)
	{
		if (pTopItem->child(i, 0)->text().contains("FTU", Qt::CaseInsensitive))
		{
			iDevice_num++;
		}
	}

	QString strDeviceName = "FTU_" + QString::number(iDevice_num);
	
	QStandardItem *pFtuItem = new QStandardItem(strDeviceName);
	m_pPluginTopItem = pFtuItem;
	pFtuItem->setData(TREE_FTU_ITEM, Qt::UserRole);
	//pFtuItem->setIcon(QIcon(":/icons/ftu.png"));
	pTopItem->appendRow(pFtuItem);

	m_iCurrentPluginIndex = pTopItem->rowCount()*1000;

	//测点选择
	QStandardItem *pSelectPoint = new QStandardItem(tr("ChoosePoints"));
	pSelectPoint->setData(TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex, Qt::UserRole);
	//pSelectPoint->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pSelectPoint);

	//遥信
	QStandardItem *pRemoteSignal = new QStandardItem(tr("Binary"));
	pRemoteSignal->setData(TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pRemoteSignal->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pRemoteSignal);

	//遥测
	QStandardItem *pRemoteMeasure = new QStandardItem(tr("Analog"));
	pRemoteMeasure->setData(TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pRemoteMeasure->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pRemoteMeasure);

	//soe
	QStandardItem *pRemotControl = new QStandardItem(tr("SOE"));
	pRemotControl->setData(TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pRemotControl->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pRemotControl);

	//kwh
	QStandardItem *pRemotKwh = new QStandardItem(tr("Kwh"));
	pRemotKwh->setData(TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pRemotKwh->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pRemotKwh);
	//control
	QStandardItem *pRemotRemoteCot = new QStandardItem(tr("Control"));
	pRemotRemoteCot->setData(TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pRemotRemoteCot->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pRemotRemoteCot);
	//fix
	QStandardItem *pFixValueSet = new QStandardItem(tr("devConfig"));
	pFixValueSet->setData(TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex, Qt::UserRole);
	//pFixValueSet->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pFixValueSet);
    //录波  Recorder
    QStandardItem *pRecord = new QStandardItem(tr("Recorder"));
    pRecord->setData(TREE_RECORD_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
    //pRecord->setIcon(QIcon(":/icons/analog.png"));
    pFtuItem->appendRow(pRecord);
    //调试维护
    QStandardItem *pMaintecance = new QStandardItem(tr("Maintecance"));
    pMaintecance->setData(TREE_DEBUG_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
    //pMaintecance->setIcon(QIcon(":/icons/analog.png"));
    pFtuItem->appendRow(pMaintecance);
    //历史数据文件
    QStandardItem *pHistory = new QStandardItem(tr("History"));
    pHistory->setData(TREE_HISTORY_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
    //pHistory->setIcon(QIcon(":/icons/analog.png"));
    pFtuItem->appendRow(pHistory);
    //操作数据文件
    QStandardItem *pCmdHistory = new QStandardItem(tr("CmdHistory"));
    pCmdHistory->setData(TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
    //pHistory->setIcon(QIcon(":/icons/analog.png"));
    pFtuItem->appendRow(pCmdHistory);
    //模拟液晶
    QStandardItem *pLed = new QStandardItem(tr("LED"));
    pLed->setData(TREE_LED_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
    //pHistory->setIcon(QIcon(":/icons/analog.png"));
    pFtuItem->appendRow(pLed);
    //故障事件
    QStandardItem *pMal = new QStandardItem(tr("MalFunction"));
    pMal->setData(TREE_MAL_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
    //pHistory->setIcon(QIcon(":/icons/analog.png"));
    pFtuItem->appendRow(pMal);
	//web
	QStandardItem *pWeb = new QStandardItem(tr("WebDevice"));
	pWeb->setData(TREE_WEB_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pHistory->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pWeb);
	//file
	QStandardItem *pFile = new QStandardItem(tr("File"));
	pFile->setData(TREE_FILE_ITEM + m_iCurrentPluginIndex, Qt::UserRole);
	//pHistory->setIcon(QIcon(":/icons/analog.png"));
	pFtuItem->appendRow(pFile);


}

void CFtuModule::Slot_ClickLeftTreeItem(const QModelIndex &qIndex)
{
	int nType = qIndex.data(Qt::UserRole).toInt();

    m_pMaintencanceWgt->StopLcdTimer();

	if (nType == TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex)
	{
		//选择测点
		if (m_MIndexTab.contains(TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pChoosePointWgt, tr("ChoosePoints"), tr("ChoosePoints"));
			m_MIndexTab.insert(TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex, m_pChoosePointWgt);
			m_MTabIndex.insert(m_pChoosePointWgt, TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_CHOOSE_OBSERVE_POINTS + m_iCurrentPluginIndex]);

		}
	}
	else if (nType == TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pRealTimeBinaryWgt, tr("Binary"), tr("yxsssj"));
			m_MIndexTab.insert(TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex, m_pRealTimeBinaryWgt);
			m_MTabIndex.insert(m_pRealTimeBinaryWgt, TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_SIGNALITEM + m_iCurrentPluginIndex]);
		}
	}
	else if (nType == TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pRealTimeAnalog, tr("Analog"), tr("ycsssj"));
			m_MIndexTab.insert(TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex, m_pRealTimeAnalog);
			m_MTabIndex.insert(m_pRealTimeAnalog, TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_MEASUREITEM + m_iCurrentPluginIndex]);
		}
	}
	else if (nType == TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pRealTimeSoe, tr("SOE"), tr("realSoe"));
			m_MIndexTab.insert(TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex, m_pRealTimeSoe);
			m_MTabIndex.insert(m_pRealTimeSoe, TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_SOE_ITEM + m_iCurrentPluginIndex]);
		}
	}
	else if (nType == TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pRealTimeKwh, tr("Kwh"), tr("realKwh"));
			m_MIndexTab.insert(TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex, m_pRealTimeKwh);
			m_MTabIndex.insert(m_pRealTimeKwh, TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_KWH_ITEM + m_iCurrentPluginIndex]);
		}
	}
	else if (nType == TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex)
	{
		//遥控
		if (m_MIndexTab.contains(TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pReomteControl, tr("Control"), tr("RemoteControl"));
			m_MIndexTab.insert(TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex, m_pReomteControl);
			m_MTabIndex.insert(m_pReomteControl, TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_CONTROLITEM + m_iCurrentPluginIndex]);
		}
	}
	else if (nType == TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex)
	{
		//遥控
		if (m_MIndexTab.contains(TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pFixDeploy, tr("DevConfig"), tr("DevConfig"));
			m_MIndexTab.insert(TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex, m_pFixDeploy);
			m_MTabIndex.insert(m_pFixDeploy, TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_FIX_DEV_CONFIG + m_iCurrentPluginIndex]);
		}
	}
    else if (nType == TREE_RECORD_ITEM + m_iCurrentPluginIndex)
    {
        //录波   m_pRecord
        //遥控
        if (m_MIndexTab.contains(TREE_RECORD_ITEM + m_iCurrentPluginIndex))
        {
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_RECORD_ITEM + m_iCurrentPluginIndex]);
        }
        else
        {
            m_pMainWindow->GetTabWidget()->AddTab(m_pRecord, tr("Record"), tr("Record"));
            m_MIndexTab.insert(TREE_RECORD_ITEM + m_iCurrentPluginIndex, m_pRecord);
            m_MTabIndex.insert(m_pRecord, TREE_RECORD_ITEM + m_iCurrentPluginIndex);
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_RECORD_ITEM + m_iCurrentPluginIndex]);
        }
    }
    else if (nType == TREE_DEBUG_ITEM + m_iCurrentPluginIndex)
    {
        
        //维护调试功能
        if (m_MIndexTab.contains(TREE_DEBUG_ITEM + m_iCurrentPluginIndex))
        {
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_DEBUG_ITEM + m_iCurrentPluginIndex]);
        }
        else
        {
            m_pMainWindow->GetTabWidget()->AddTab(m_pMaintencanceWgt, tr("Maintecance"), tr("Maintecance"));
            m_MIndexTab.insert(TREE_DEBUG_ITEM + m_iCurrentPluginIndex, m_pMaintencanceWgt);
            m_MTabIndex.insert(m_pMaintencanceWgt, TREE_DEBUG_ITEM + m_iCurrentPluginIndex);
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_DEBUG_ITEM + m_iCurrentPluginIndex]);
        }
        
    }
    else if (nType == TREE_HISTORY_ITEM + m_iCurrentPluginIndex)
    {
        //维护调试功能
        if (m_MIndexTab.contains(TREE_HISTORY_ITEM + m_iCurrentPluginIndex))
        {
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_HISTORY_ITEM + m_iCurrentPluginIndex]);
        }
        else
        {
            m_pMainWindow->GetTabWidget()->AddTab(m_pHistory, tr("History"), tr("History"));
            m_MIndexTab.insert(TREE_HISTORY_ITEM + m_iCurrentPluginIndex, m_pHistory);
            m_MTabIndex.insert(m_pHistory, TREE_HISTORY_ITEM + m_iCurrentPluginIndex);
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_HISTORY_ITEM + m_iCurrentPluginIndex]);
        }
    }
    else if (nType == TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex)
    {
        //操作历史数据
        if (m_MIndexTab.contains(TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex))
        {
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex]);
        }
        else
        {
            m_pMainWindow->GetTabWidget()->AddTab(m_pCmdHistory, tr("CmdHistory"), tr("CmdHistory"));
            m_MIndexTab.insert(TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex, m_pCmdHistory);
            m_MTabIndex.insert(m_pCmdHistory, TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex);
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_CMD_HISTORY_ITEM + m_iCurrentPluginIndex]);
        }
    }
    else if (nType == TREE_LED_ITEM + m_iCurrentPluginIndex)
    {
        m_pMaintencanceWgt->StartLcdTimer();
        //
        if (m_MIndexTab.contains(TREE_LED_ITEM + m_iCurrentPluginIndex))
        {
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_LED_ITEM + m_iCurrentPluginIndex]);
        }
        else
        {
            m_pMainWindow->GetTabWidget()->AddTab(m_pLcdWgt, tr("LED"), tr("LED"));
            m_MIndexTab.insert(TREE_LED_ITEM + m_iCurrentPluginIndex, m_pLcdWgt);
            m_MTabIndex.insert(m_pLcdWgt, TREE_LED_ITEM + m_iCurrentPluginIndex);
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_LED_ITEM + m_iCurrentPluginIndex]);
        }
    }
    else if (nType == TREE_MAL_ITEM + m_iCurrentPluginIndex)
    {
        //
        if (m_MIndexTab.contains(TREE_MAL_ITEM + m_iCurrentPluginIndex))
        {
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_MAL_ITEM + m_iCurrentPluginIndex]);
        }
        else
        {
            m_pMainWindow->GetTabWidget()->AddTab(m_FaultEventWgt, tr("Malfunction"), "Malfunction1");
            m_MIndexTab.insert(TREE_MAL_ITEM + m_iCurrentPluginIndex, m_FaultEventWgt);
            m_MTabIndex.insert(m_FaultEventWgt, TREE_MAL_ITEM + m_iCurrentPluginIndex);
            m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_MAL_ITEM + m_iCurrentPluginIndex]);
        }
    }
	else if (nType == TREE_WEB_ITEM + m_iCurrentPluginIndex)
	{
		//
		if (m_MIndexTab.contains(TREE_WEB_ITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_WEB_ITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pWebDevice, tr("WebDevice"), "WebDevice");
			m_MIndexTab.insert(TREE_WEB_ITEM + m_iCurrentPluginIndex, m_pWebDevice);
			m_MTabIndex.insert(m_pWebDevice, TREE_WEB_ITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_WEB_ITEM + m_iCurrentPluginIndex]);
		}
	}
	else if (nType == TREE_FILE_ITEM + m_iCurrentPluginIndex)
	{
		//
		if (m_MIndexTab.contains(TREE_FILE_ITEM + m_iCurrentPluginIndex))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_FILE_ITEM + m_iCurrentPluginIndex]);
		}
		else
		{
			m_pMainWindow->GetTabWidget()->AddTab(m_pFileTransWgt, tr("File"), "File");
			m_MIndexTab.insert(TREE_FILE_ITEM + m_iCurrentPluginIndex, m_pFileTransWgt);
			m_MTabIndex.insert(m_pFileTransWgt, TREE_FILE_ITEM + m_iCurrentPluginIndex);
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_FILE_ITEM + m_iCurrentPluginIndex]);
		}
	}
}

void CFtuModule::Slot_CloseOneTabWidget(int iIndex)
{
	//
	QWidget *pCloseWgt = m_pMainWindow->GetTabWidget()->widget(iIndex);
	
	if (pCloseWgt != NULL)
	{
		if (pCloseWgt->objectName() == "Monitor")
		{
			m_pMainWindow->GetTabWidget()->removeTab(iIndex);
			return;
		}
		int iIndex2 = m_MTabIndex[pCloseWgt];
		m_MIndexTab.remove(iIndex2);
		m_MTabIndex.remove(pCloseWgt);
		m_pMainWindow->GetTabWidget()->removeTab(iIndex);
		
		//删除需要关闭的wgt
// 		if (qobject_cast<CChoosePointsWgt*>(pCloseWgt) == NULL)
// 		{
// 			//点表选择不删
// 			pCloseWgt->deleteLater();
// 		}

	}
}

IMainModule * CFtuModule::GetMainModule()
{
    return m_pMainModule;
}

QString CFtuModule::GetDeviceName()
{
    return m_strDeviceName;
}

int CFtuModule::GetDeviceAddr()
{
    return m_pConfigWgt->GetDeviceAddr();
}

int CFtuModule::GetBinaryStartAddr()
{
    return m_pConfigWgt->GetBinaryStartAddr();
}

int CFtuModule::GetAnalogStartAddr()
{
    return m_pConfigWgt->GetAnalogStartAddr();
}

int CFtuModule::GetKwhStartAddr()
{
    return m_pConfigWgt->GetKwhStartAddr();
}

int CFtuModule::GetControlStartAddr()
{
    return m_pConfigWgt->GetControlStart();
}

void CFtuModule::InitCallZt()
{
    QMenu *pPlcMenu = m_pMainWindow->menuBar()->addMenu(tr("Control(C)"));

    //调用commConfig接口
    QAction *pCommAct = new QAction(tr("CommConfig(G)"),pPlcMenu);
    pPlcMenu->addAction(pCommAct);

    //调用总召
    QAction *pGeneralAct = new QAction(tr("General(L)"),pPlcMenu);
    pPlcMenu->addAction(pGeneralAct);

    //调用对时
    QAction *pSycsAct = new QAction(tr("Sycs Time(T)"),pPlcMenu);
    pPlcMenu->addAction(pSycsAct);

    //调用电度召唤
    QAction *pKwhAct = new QAction(tr("Kwh Call(K)"),pPlcMenu);
    pPlcMenu->addAction(pKwhAct);

    //复位进程
    QAction *pResetProcessAct = new QAction(tr("Reset Process(P)"),pPlcMenu);
    pPlcMenu->addAction(pResetProcessAct);

    //保存测点
    QAction *pSavePoint = new QAction(tr("Save Point"),pPlcMenu);
    pPlcMenu->addAction(pSavePoint);

	//切换定值区号
	QAction *pFixSwitch = new QAction(tr("Switch Fix Area"),pPlcMenu);
	pPlcMenu->addAction(pFixSwitch);

	//读取定值区号
	QAction *pFixReadArea = new QAction(tr("Read Fix Area"),pPlcMenu);
	pPlcMenu->addAction(pFixReadArea);

    connect(pCommAct, SIGNAL(triggered()), this, SLOT(Slot_CallCommConfig()));
    connect(pGeneralAct, SIGNAL(triggered()), this, SLOT(Slot_GeneralCall()));
    connect(pSycsAct, SIGNAL(triggered()), this, SLOT(Slot_SycsTimeAct()));
    connect(pKwhAct, SIGNAL(triggered()), this, SLOT(Slot_KwhCallTimeAct()));

    connect(pResetProcessAct, SIGNAL(triggered()), m_pCommThread, SIGNAL(Signal_ResetProcess()));
    connect(pSavePoint, SIGNAL(triggered()), this, SLOT(Slot_SavePoints()));
	//切换定值区号
	connect(pFixSwitch,SIGNAL(triggered()),this,SLOT(Slot_SwitchFixArea()));
	//读取定值区号
	connect(pFixReadArea, SIGNAL(triggered()), this, SLOT(Slot_ReadFixArea()));
}

void CFtuModule::Slot_CallCommConfig()
{
    QString strRun = qApp->applicationDirPath()+ QString("/DA-C2XX组态工具包/DA-C2XX组态工具包/bin/CommConfig.exe");

    QFile tRunFile(strRun);
    if (!tRunFile.exists())
    {
        QMessageBox::warning(0, tr("Warning"), tr("can not find :%1").arg(strRun));
        return;
    }

    m_pCommProcess->setWorkingDirectory(qApp->applicationDirPath() + QString("/DA-C2XX组态工具包/DA-C2XX组态工具包/bin/"));
    m_pCommProcess->start(strRun);
}


void CFtuModule::Slot_DoubleClickTabBar(int nIndex)
{
    QWidget *pCloseWgt = m_pMainWindow->GetTabWidget()->widget(nIndex);

    if (pCloseWgt != NULL)
    {
        if (pCloseWgt->objectName() == "Monitor")
        {
            m_pMainWindow->GetTabWidget()->removeTab(nIndex);

            m_pMonitorDockWgt->setWidget(pCloseWgt);

            m_pMonitorDockWgt->show();

            m_pMainWindow->addDockWidget(Qt::BottomDockWidgetArea, m_pMonitorDockWgt);

        }
    }
}

void CFtuModule::Slot_GeneralCall()
{
    //总召
    m_pCommThread->SetGeneralCall();
}

//切换定值区号
void CFtuModule::Slot_SwitchFixArea()
{
	CFixOperateWgt *pFixOperaorWgt = new CFixOperateWgt;

	if (pFixOperaorWgt->exec())
	{
		m_nFixAreaCode = pFixOperaorWgt->GetFixData().toInt();

		m_pCommThread->SetFixAreaCall(pFixOperaorWgt->GetFixData().toUShort());
	}	

	pFixOperaorWgt->deleteLater();
}

//读取定值区号
void CFtuModule::Slot_ReadFixArea()
{
	m_pCommThread->ReadFixArea();
}


void CFtuModule::Slot_SycsTimeAct()
{
    m_pSycsConfigWgt->show();
}

//调用电度
void CFtuModule::Slot_KwhCallTimeAct()
{
    m_pCommThread->SetKwhCall();
}

void CFtuModule::AnalyseSycsTime(const QByteArray &btData, int nLength, int nDFlag)
{
    if (nLength <= 0)
    {
        return;
    }
    //组织ASDU101
    ASDU103* pAsdu103 = (ASDU103*)(btData.data() + +sizeof(APCI));

    if (nDFlag == 1)
    {
        //发送
        QByteArray byDestr = tr("Send synchronous time: %1").arg(pAsdu103->m_time.Dump()).toLocal8Bit();

        m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), byDestr.data(), 1);
    }
    else if (nDFlag == 2)
    {
         //接收
        QByteArray byDestr = tr("Recv synchronous time: %1").arg(pAsdu103->m_time.Dump()).toLocal8Bit();

        m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), byDestr.data(), 1);
    }
}

void CFtuModule::Slot_SavePoints()
{
    QString strRunPath = qApp->applicationDirPath();
    m_pChoosePointWgt->SaveChooseBinFile(strRunPath + REMOTETABLE + "iec104sList.ini");
}
