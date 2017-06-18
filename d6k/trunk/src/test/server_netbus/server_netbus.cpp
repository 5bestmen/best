#include "server_netbus.h"

#include <QMessageBox>


#include "netbus/nbdef.h"
#include "fesapi/fescnst.h"

#include <windows.h>

typedef bool (*StartNetBus)(NET_CONFIG * pConf, const char *pszProjPath, const char * pszPredicateName, int32u nNodeType, int32u nStartMode);

typedef void (*StopNetBus)(const char * pszPredicateName);

typedef int (*SendData)(const char * pszPredicateName, const int8u * pMsg, int32u nLen);

typedef bool(*RecvData)(const int8u * pMsg, int32u nLen, int32u *pCopySize, int32u ulTimeout);

server_netbus::server_netbus(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pNetbus = nullptr;
	m_bStartFlag = false;

	ui.serverRadioButton->setChecked(true);
	ui.normalRadioButton->setChecked(true);
	ui.msgSizeLineEdit->setText("1");
	ui.numLoopLineEdit->setText("1");
	//ui.recvTextEdit->setEnabled(false);
	ui.sendTextEdit->setFocus();
	ui.titleLabel->setText(ui.serverRadioButton->text());

	m_pNBRecv = new CNBRecvThread();
	m_pNBRecv->start(QThread::HighPriority);

	connect(m_pNBRecv, SIGNAL(sigDataRecv(QString)), this, SLOT(SLOT_DataReceive(QString)));

	connect(ui.startNetBus, SIGNAL(clicked()), this, SLOT(SLOT_StartNetBus()));
	connect(ui.stopNetBus, SIGNAL(clicked()), this, SLOT(SLOT_StopNetBus()));
	connect(ui.sendPushButton, SIGNAL(clicked()), this, SLOT(SLOT_SendData()));

	connect(ui.msgSizeLineEdit, SIGNAL(selectionChanged()), this, SLOT(SLOT_MsgSizeRadio()));
	connect(ui.serverRadioButton, SIGNAL(clicked()), this, SLOT(SLOT_ServerRadio()));
	connect(ui.fesRadioButton, SIGNAL(clicked()), this, SLOT(SLOT_FesRadio()));
	connect(ui.clientRadioButton, SIGNAL(clicked()), this, SLOT(SLOT_ClientRadio()));
	//connect(ui.startNetBus, SIGNAL(clicked()), this, SLOT(SLOT_StartNetBus()));

	ui.stopNetBus->setEnabled(false);
	ui.sendPushButton->setEnabled(false);

	if (!InitNetBus())
	{
		QMessageBox::information(NULL, "Warning", "InitNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
}

server_netbus::~server_netbus()
{
	if (m_pNBRecv != nullptr)
	{
		m_pNBRecv->quit();

		delete m_pNBRecv;
		m_pNBRecv = nullptr;
	}

	if (!FiniNetBus())
	{
		QMessageBox::information(NULL, "Warning", "FiniNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

bool server_netbus::InitNetBus()
{
	if (m_pNetbus != nullptr)
	{
		delete m_pNetbus;
		m_pNetbus = nullptr;
	}

#ifdef _DEBUG
	m_pNetbus = new QLibrary("netbus");
#else
	m_pNetbus = new QLibrary("netbus");
#endif

	if (!m_pNetbus->load())
	{
		QMessageBox::information(NULL, "Warning", "Load fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}

	return true;
}

bool server_netbus::FiniNetBus()
{
	Q_ASSERT(m_pNetbus);
	if (m_pNetbus == nullptr)
	{
		return false;
	}

	if (m_pNetbus->isLoaded())
	{
		if (m_bStartFlag)
		{
			StopNetBus stop = (StopNetBus)m_pNetbus->resolve("StopNetBus");
			if (stop == nullptr)
			{
				QMessageBox::information(NULL, "Warning", "Resolve StopNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return false;
			}

			stop("222");
		}

		if (m_pNetbus->unload())
		{
			delete m_pNetbus;
			m_pNetbus = nullptr;
		}
	}

	return true;
}

bool server_netbus::StartNetbus()
{
	Q_ASSERT(m_pNetbus);
	if (m_pNetbus == nullptr)
	{
		return false;
	}
	StartNetBus startup = (StartNetBus)m_pNetbus->resolve("StartNetBus");
	if (startup == nullptr)
	{
		QMessageBox::information(NULL, "Warning", "Resolve StartNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}

	NET_CONFIG* pTest = new NET_CONFIG();
	memset(pTest, 0, sizeof(NET_CONFIG));
	pTest->MyNodeOccNo = 1;
	pTest->IsDoubleNet = false;
	pTest->NodeCount = 2;
	pTest->pNodeConfig = new CNodeConfig[pTest->NodeCount];
	memset(pTest->pNodeConfig, 0, sizeof(CNodeConfig)*pTest->NodeCount);
	
	pTest->pNodeConfig[0].OccNo = 1;
	pTest->pNodeConfig[0].NodeType = NODE_FES;
	strncpy(pTest->pNodeConfig[0].chIP[0], "172.16.38.2", strlen("172.16.38.2"));
	strncpy(pTest->pNodeConfig[0].HostName, "zyj-PC", strlen("zyj-PC"));

	pTest->pNodeConfig[1].OccNo = 2;
	pTest->pNodeConfig[1].NodeType = NODE_SVR;
	//strncpy(pTest->pNodeConfig[1].chIP[0], "172.16.38.131", strlen("172.16.38.131"));
	strncpy(pTest->pNodeConfig[1].chIP[0], "172.16.38.2", strlen("172.16.38.2"));
	strncpy(pTest->pNodeConfig[1].HostName, "zyj-PC", strlen("zyj-PC"));

	bool bRes = false;
	if (ui.serverRadioButton->isChecked())
	{
		bRes = startup(pTest, "111", "222", NODE_SVR, 1);
	}
	else if (ui.fesRadioButton->isChecked())
	{
		bRes = startup(pTest, "111", "222", NODE_FES, 1);
	}
	else if (ui.clientRadioButton->isChecked())
	{
		bRes = startup(pTest, "111", "222", NODE_CLIENT, 1);
	}
	else
	{
		return false;
	}

	if (bRes)
	{
		m_bStartFlag = true;
		QMessageBox::information(NULL, "Debug", "Start NetBus succ.", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	else
	{
		m_bStartFlag = false;
		QMessageBox::information(NULL, "Warning", "Start NetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}

	return bRes;
}

bool server_netbus::StopNetbus()
{
	Q_ASSERT(m_pNetbus);
	if (m_pNetbus == nullptr)
	{
		return false;
	}

	if (m_pNetbus->isLoaded())
	{
		StopNetBus stop = (StopNetBus)m_pNetbus->resolve("StopNetBus");
		if (stop == nullptr)
		{
			QMessageBox::information(NULL, "Warning", "Resolve StopNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}

		stop("222");
	}

	return true;
}

void server_netbus::SLOT_StartNetBus()
{
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	ui.startNetBus->setEnabled(false);
	ui.stopNetBus->setEnabled(true);

	// 启动总线
	if (!StartNetbus())
	{
		QMessageBox::information(NULL, "Warning", "StartNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	ui.sendPushButton->setEnabled(true);

	// 启动总线成功后的操作
	if (ui.serverRadioButton->isChecked())
	{
		ui.fesRadioButton->setEnabled(false);
		ui.clientRadioButton->setEnabled(false);
	}
	else if (ui.fesRadioButton->isChecked())
	{
		ui.serverRadioButton->setEnabled(false);
		ui.clientRadioButton->setEnabled(false);
	}
	else if (ui.clientRadioButton->isChecked())
	{
		ui.serverRadioButton->setEnabled(false);
		ui.fesRadioButton->setEnabled(false);
	}

	ui.sendPushButton->setEnabled(true);
}

void server_netbus::SLOT_StopNetBus()
{
	StopNetbus();

	m_bStartFlag = false;

	ui.startNetBus->setEnabled(true);
	ui.stopNetBus->setEnabled(false);
	ui.sendPushButton->setEnabled(false);

	ui.serverRadioButton->setEnabled(true);
	ui.fesRadioButton->setEnabled(true);
	ui.clientRadioButton->setEnabled(true);
}

void server_netbus::SLOT_SendData()
{
	Q_ASSERT(m_pNetbus);
	if (m_pNetbus == nullptr)
	{
		QMessageBox::information(NULL, "Warning", "m_pNetbus == nullptr", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		return;
	}

	if (!m_bStartFlag)
	{
		QMessageBox::information(NULL, "Warning", "Don't start netbus !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		return;
	}

	QString strData = "";
	std::string data = "";
	const char* pData = nullptr;
	const int8u* p = nullptr;

	size_t len = 0;

	if (ui.normalRadioButton->isChecked())
	{ // 普通发送
		strData = ui.sendTextEdit->toPlainText();
		data = strData.toStdString();
		pData = data.c_str();

		p = (const int8u*)pData;
		len = strData.length();
	}
	else
	{ // 指定 N 个kb大小
		QString strBaseData = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
123456789012345678901234"; // 1024

		unsigned int nCnt = ui.msgSizeLineEdit->text().toInt();
		if (nCnt == 0)
		{
			nCnt = 1;
		}
		else if (nCnt > (MAX_EMSG_L/1024) - 1)
		{
			nCnt = (MAX_EMSG_L / 1024) - 1;
		}

		for (unsigned int i = 1; i <= nCnt; ++i)
		{
			strData += strBaseData;
		}

		data = strData.toStdString();
		pData = data.c_str();

		p = (const int8u*)pData;
		len = strData.length();
	}
	

	/*EMSG_BUF msgBuf;
	msgBuf.MsgPath = TO_SERVER;
	msgBuf.MsgType = 1;
	msgBuf.MsgDataSize = len;*/
	//msgBuf.data = (void*)(new char[msgBuf.MsgDataSize]);
	//msgBuf.buffData = new char[msgBuf.MsgDataSize];
	EMSG_BUF* pTest = (EMSG_BUF*) malloc(sizeof(EMSG_BUF) + len);
	pTest->MsgPath = TO_SERVER;
	pTest->MsgType = 1;
	pTest->MsgDataSize = len + EMSG_BUF_SIZE;

	memcpy(pTest->buffData, p, len);

	if (strData.isEmpty())
	{
		QMessageBox::information(NULL, "Warning", "Send content can not be empty !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	SendData send = (SendData)m_pNetbus->resolve("NBSendData");
	if (send == nullptr)
	{
		QMessageBox::information(NULL, "Warning", "Resolve StopNetBus fail !!!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	len += sizeof(EMSG_BUF);

	int num = ui.numLoopLineEdit->text().toInt();
	if (num <= 0)
	{
		num = 1;
		ui.numLoopLineEdit->setText("1");
	}
	else if (num > 1000000)
	{
		num = 1000000;
		ui.numLoopLineEdit->setText("1000000");
	}

	for (int i = 0; i < num; ++i)
	{
		if (send("222", (int8u*)(pTest), len) <= 0)
		{
			break;
		}
	}

	free(pTest);
	//delete[] msgBuf.data;
}

void server_netbus::SLOT_MsgSizeRadio()
{
	ui.normalRadioButton->setChecked(false);
	ui.editRadioButton->setChecked(true);
}

void server_netbus::SLOT_ServerRadio()
{
	ui.titleLabel->setText(ui.serverRadioButton->text());
}

void server_netbus::SLOT_FesRadio()
{
	ui.titleLabel->setText(ui.fesRadioButton->text());
}

void server_netbus::SLOT_ClientRadio()
{
	ui.titleLabel->setText(ui.clientRadioButton->text());
}

void server_netbus::SLOT_DataReceive(QString data)
{
	ui.recvTextEdit->append(data);
}

CNBRecvThread::CNBRecvThread()
{
#ifdef _DEBUG
	m_pNetbus = new QLibrary("netbus");
#else
	m_pNetbus = new QLibrary("netbus");
#endif
}

CNBRecvThread::~CNBRecvThread()
{
	if (m_pNetbus != nullptr)
	{
		delete m_pNetbus;
		m_pNetbus = nullptr;
	}
}

void CNBRecvThread::run()
{
	RecvData recv = (RecvData)m_pNetbus->resolve("NBRecvData");
	while (1)
	{
		int8u cMsg[MAX_EMSG_L + 1] = { 0 };
		int32u nCopySize = 0;
		// 队列
		if (!recv(cMsg, MAX_EMSG_L, &nCopySize, 0))
		{
			Sleep(10);
			continue;
		}

		EMSG_BUF* msg = (EMSG_BUF*)cMsg;

		emit sigDataRecv((char*)msg->buffData);
	}
}
