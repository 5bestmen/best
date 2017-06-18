
#include "alarminfomodel.h"
#include "alarmwindow.h"
#include "delegate.h"
#include "recvinfo.h"
#include "analysemsg.h"
#include "mail/mail.h"
#include "log/log.h"
#include "fesapi/fesapi.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTimer>
#include <QDebug>
#include <QLabel>
#include <QTranslator>

CAlarmWindow::CAlarmWindow(QWidget *parent, QString szProjName)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	QTranslator *qtPlugini = new QTranslator();

	QString strRunPath = qApp->applicationDirPath();

	if (qtPlugini->load(strRunPath + "/locales/alarmwin_zh.qm"))
	{
		qApp->installTranslator(qtPlugini);
	}

	OpenIO(szProjName.toStdString().c_str(),0,0);

	LogMsg("alarm", QObject::tr("alarmview::open fesDB succeed ").toStdString().c_str(), 0, nullptr);

	OpenPostOffice("FES");
	int nMailID = 0;
	OpenMailBox("FES", "ALARM_WIN", &nMailID);
	//实现邮件订阅
	SubscribeMail("FES", "ALARM_WIN", MSG_EVT_ALL);

	LogMsg("alarm", QObject::tr("alarmview::open fes mail system OK").toStdString().c_str(), 0, nullptr);

	//创建共享队列
	m_pAlarmDataQueue = std::make_shared<CAlarmInfoQueue>();

	m_pAlarmViewQueue = std::make_shared<CAlarmInfoQueue>();

	//界面初始化
	InitView();

	//启动接收线程
	m_pRecvThread = new CRecvInfoTask(this,m_pAlarmDataQueue);

	m_pRecvThread->SetThreadState(true);

	m_pRecvThread->start();

	QObject::connect(m_pRecvThread, SIGNAL(sig_FesInfo(const QString &)), this, SLOT(slot_OnFesStatus(const QString&)));

	//启动处理线程
	m_pHandleThread = new CAnalyseTask(this, m_pAlarmDataQueue,m_pAlarmViewQueue);

	m_pHandleThread->SetThreadState(true);

	m_pHandleThread->start();

	//QObject::connect(m_pHandleThread, SIGNAL(sig_ParOneMsgOK(void *)), this, SLOT(slot_OnRecvMsg(void* )));

	m_pTimer = new QTimer(this);

	QObject::connect(m_pTimer, SIGNAL(timeout()),this,SLOT(slot_OnTimer()));

	m_pTimer->start(1000);

	m_pRandTimer = new QTimer(this);

//	QObject::connect(m_pRandTimer, SIGNAL(timeout()), this, SLOT(slot_onTimerAdd()));

	QObject::connect(m_pRandTimer, SIGNAL(timeout()), this, SLOT(slot_onTimerDel()));

	m_pRandTimer->start(1000);

	m_pLabel = new QLabel(this);

	this->statusBar()->addWidget(m_pLabel);
}

CAlarmWindow::~CAlarmWindow()
{
	m_pRecvThread->SetThreadState(false);
	m_pRecvThread->wait();

	m_pHandleThread->SetThreadState(false);
	m_pHandleThread->wait();

	LogMsg("alarm", QObject::tr("alarmview::module stopped").toStdString().c_str(), 0, nullptr);
//	CloseMailBox("FES", "ALARM_WIN");

//	ClosePostOffice("FES");
}

/*! \fn void CAlarmWindow::InitView()
********************************************************************************************************* 
** \brief CAlarmWindow::InitView 
** \details 初始化视图界面
** \return void 
** \author xingzhibing
** \date 2016年10月9日 
** \note 
********************************************************************************************************/
void CAlarmWindow::InitView()
{

	m_pMainView = new QTableView(this);

	m_pMainView->setShowGrid(false);
	//代理
	//CStarDelegate  * pDelegate = new CStarDelegate(m_pMainView);

	//m_pMainView->setItemDelegateForColumn(0, pDelegate);

	m_pMainView->horizontalHeader()->setStretchLastSection(true);

	m_pDataModel = new CAlarmInfoModel(m_pMainView, m_pAlarmViewQueue);

	m_pMainView->setModel(m_pDataModel);

	SetHeaderList();

	SetGridData();

	QSplitter *pLeftSplitter = new QSplitter(Qt::Vertical);
	pLeftSplitter->addWidget(m_pMainView);
	setCentralWidget(pLeftSplitter);

	m_pMainView->setAlternatingRowColors(true);

}

/*! \fn void CAlarmWindow::SetHeaderList()
********************************************************************************************************* 
** \brief CAlarmWindow::SetHeaderList 
** \details 
** \return void 
** \author xingzhibing
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CAlarmWindow::SetHeaderList()
{
	QStringList szNameList;
	szNameList << tr("ALARM_TYPE")
		       << tr("ALARM_TIME")
			   << tr("ALARM_VALUE")
			   << tr("ALARM_STATE")
			   << tr("ALARM_DESC");

	m_pDataModel->SetColCount(szNameList.count());

	m_pDataModel->SetRowCount(m_pAlarmViewQueue->size());

	m_pDataModel->SetHeaderData(szNameList);
}

/*! \fn void CAlarmWindow::SetGridData()
********************************************************************************************************* 
** \brief CAlarmWindow::SetGridData 
** \details 
** \return void 
** \author xingzhibing
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CAlarmWindow::SetGridData()
{
	/*std::shared_ptr < CAlarmDataConfig > pMsg;

	for (int i = 0; i < 10000; i++)
	{
		pMsg = std::make_shared<CAlarmDataConfig>();
		pMsg->m_nAlarmID = i;
		pMsg->m_szTimeTag = QDateTime::currentDateTime();
		pMsg->m_nAlarmState = alarm_unconfim;
		strncpy(pMsg->m_szContent, "FES1.CHANNEL1.AIN001 Break ON/OFF", 512);
		m_pAlarmDataQueue->push_back(pMsg);
	}*/
	//m_pDataModel->SetDataModel(m_pAlarmViewQueue);
}

/*! \fn void CAlarmWindow::slot_OnTimer()
********************************************************************************************************* 
** \brief CAlarmWindow::slot_OnTimer 
** \details 定时刷新界面
** \return void 
** \author xingzhibing
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CAlarmWindow::slot_OnTimer()
{
	if (m_pMainView)
	{
		m_pMainView->reset();
	}

	SetHeaderList();

//	SetGridData();

//	m_pMainView->setModel(m_pDataModel);

	m_pDataModel->RefeshModel();

	m_pMainView->update();

	/*if (m_pDataModel)
	{
		delete m_pDataModel;
		m_pDataModel = NULL;
	}
	m_pDataModel = new CAlarmInfoModel(m_pMainView,m_pAlarmViewQueue);

	SetHeaderList();

	m_pDataModel->SetRowCount(m_pAlarmViewQueue->size());

	m_pMainView->setModel(m_pDataModel);*/

	if (IsFesAlive())
	{
		m_pLabel->setText("good");
	}
	else
	{
		m_pLabel->setText("bad");
	}
		
}

/*! \fn void CAlarmWindow::slot_OnRecvMsg()
********************************************************************************************************* 
** \brief CAlarmWindow::slot_OnRecvMsg 
** \details 接收到客户端发来邮件,当前接收到的邮件
** \return void 
** \author xingzhibing
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CAlarmWindow::slot_OnRecvMsg(void* pMsg)
{	
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
	{
		//当前邮件无效
		return;
	}
	else
	{
		AlarmInfo_PTR pInfo = std::shared_ptr<CAlarmDataConfig>((CAlarmDataConfig*)pMsg);

		m_pAlarmViewQueue->push_back(pInfo);
	}

	//
}

//定时每隔1s 增加随机告警
void CAlarmWindow::slot_onTimerAdd()
{
	int nCount = qrand() % 1000;

	std::shared_ptr < CAlarmDataConfig > pMsg;

	for (int i = 0; i < nCount; i++)
	{
		pMsg = std::make_shared<CAlarmDataConfig>();
		pMsg->m_nAlarmID = i;
		//pMsg->m_szTimeTag = QDateTime::currentDateTime();
		pMsg->m_nAlarmState = ((i % 2)==1) ? alarm_confim : alarm_unconfim;
		strncpy(pMsg->m_szContent, "FES1.CHANNEL1.AIN001 Break ON/OFF", 512);
		m_pAlarmDataQueue->push_back(pMsg);
	}
}
//定时 每次删除 只保留10000条信息
void CAlarmWindow::slot_onTimerDel()
{
	if (m_pAlarmViewQueue->empty())
	{
		return;
	}

	m_pAlarmViewQueue->erase(50000);
#if 0
	int nSize = m_pAlarmViewQueue->size();	
	int offset = nSize - 500000;

	if (offset<0)
	{
		return;
	}
	CAlarmInfoQueue::ITERATOR it = m_pAlarmViewQueue->begin();
	m_pAlarmViewQueue->erase(it, it + offset);
#endif
}

/*! \fn void CAlarmWindow::closeEvent(QCloseEvent *event)
********************************************************************************************************* 
** \brief CAlarmWindow::closeEvent 
** \details 处理界面关闭效果
** \param event 
** \return void 
** \author xingzhibing
** \date 2016年10月13日 
** \note 
********************************************************************************************************/
void CAlarmWindow::closeEvent(QCloseEvent *event)
{
	m_pRecvThread->SetThreadState(false);
	m_pRecvThread->wait();

	m_pHandleThread->SetThreadState(false);
	m_pHandleThread->wait();

	QMainWindow::closeEvent(event);
}

void CAlarmWindow::slot_OnFesStatus(const QString& str)
{
	//this->statusBar();
	m_pLabel->setText(str);
}
