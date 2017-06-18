#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QWebEngineView>
#include <QPushButton>
#include "webdevicewgt.h"

CWebDeviceWgt::CWebDeviceWgt(QWidget *parent)
	: QWidget(parent)
{
	InitWgt();
}

void CWebDeviceWgt::InitWgt()
{
	QLabel *plabelIp = new QLabel(this);
	plabelIp->setText(tr("IP:"));
	
	m_pLineIpAddr = new QLineEdit(this);
	m_pLineIpAddr->setText("172.16.42.111");

	QLabel *pLablePort = new QLabel(this);
	pLablePort->setText(tr("Port"));

	m_pLinePort = new QLineEdit(this);
	m_pLinePort->setText("8080");

	QPushButton *pConfBtn = new QPushButton(this);
	pConfBtn->setText(tr("Refresh"));

	QPushButton *pBackBtn = new QPushButton(this);
	pBackBtn->setText(tr("<-"));

	QPushButton *pForwordBtn = new QPushButton(this);
	pForwordBtn->setText(tr("->"));



	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(plabelIp);
	topLayout->addWidget(m_pLineIpAddr);
	topLayout->addWidget(pLablePort);
	topLayout->addWidget(m_pLinePort);
	topLayout->addWidget(pConfBtn);
	topLayout->addWidget(pBackBtn);
	topLayout->addWidget(pForwordBtn);

	m_pWebView = new QWebEngineView;
	//pWebView->load(QUrl("http://172.16.42.111:8080/"));

	QVBoxLayout *pTotalLayout = new QVBoxLayout(this);
	pTotalLayout->addLayout(topLayout);
	pTotalLayout->addWidget(m_pWebView);

	connect(pConfBtn,SIGNAL(clicked()),this,SLOT(Slot_Confrom()));
	connect(pBackBtn,SIGNAL(clicked()),this,SLOT(Slot_backPage()));
	connect(pForwordBtn, SIGNAL(clicked()), this, SLOT(Slot_Forword()));
}

CWebDeviceWgt::~CWebDeviceWgt()
{
	m_pWebView->stop();
	m_pWebView->close();
}

void CWebDeviceWgt::Slot_Confrom()
{
	QString strUrl = "http://" + m_pLineIpAddr->text() + ":" + m_pLinePort->text();
	m_pWebView->load(QUrl(strUrl));
}

void CWebDeviceWgt::Slot_backPage()
{
	m_pWebView->page()->triggerAction(QWebEnginePage::Back);
}

void CWebDeviceWgt::Slot_Forword()
{
	m_pWebView->page()->triggerAction(QWebEnginePage::Forward);
}
