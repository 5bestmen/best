#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include "realtimesoe.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"

CRealTimeSOE::CRealTimeSOE(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt)
	: QWidget(parent)
{
	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}

    m_pPointInfo = pPointInfo;

	m_pConfgWgt = pConfgWgt;

	m_pCommunicate = pCommunicate;

	InitWidget();
}

void CRealTimeSOE::InitWidget()
{
	m_pTableView = new QTableView(this);
	m_pTableModel = new QStandardItemModel(this);
	m_pTableView->setModel(m_pTableModel);

	m_pTableView->verticalHeader()->setHidden(true);
	m_pTableView->setSortingEnabled(true);
	m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu);

	QStringList lstHorizonLables;
	lstHorizonLables << tr("NO") << tr("Destriber") << tr("Address") << tr("Value") << tr("Destriber") << tr("Time");
	m_pTableModel->setHorizontalHeaderLabels(lstHorizonLables);
	m_pTableView->setColumnHidden(4,true);

	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(m_pTableView);
	
   // connect(m_pCommunicate, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)), this, SLOT(Slot_AnalyseSeoBinaryData(int, int, int, int, QString)));
    connect(m_pCommunicate, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)), this, SLOT(Slot_AnalyseSeoBinaryData(int, int, int, int, QString)));


	connect(m_pCommunicate, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)), this, SLOT(Slot_AnalyseSeoBinaryData(int, int, int, int, QString)));
    //connect(m_pCommunicate, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)), this, SLOT(Slot_AnalyseSeoBinaryData(int, int, int, int, QString)));
    //connect(m_pCommunicate, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)), this, SLOT(Slot_AnalyseSeoBinaryData(int, int, int, int, QString)));

	connect(m_pTableView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
}

void CRealTimeSOE::AnalyseRealSeoBinaryData(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime)
{
    Q_UNUSED(iDeviceID);

	if (nQuality == 0)
	{
		//单点
		nPointID = nPointID - m_pConfgWgt->GetBinaryStartAddr();
	}
	else
	{
		//双点
		nPointID = nPointID - m_pConfgWgt->GetDoubleBinaryStartAddr();
	}

	//插入
    QMap<unsigned int, RPT> mapPoint = m_pPointInfo->GetBinaryMap();
	//获取当前的行数
	int iRowNum = m_pTableModel->rowCount();
	//插入数据
	//序号
	m_pTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(iRowNum + 1)));
	//站点ID
    m_pTableModel->setItem(iRowNum, 1, new QStandardItem(mapPoint[nPointID-1].Destriber));
	//地址
	m_pTableModel->setItem(iRowNum, 2, new QStandardItem(QString::number(nPointID-1)));
	//点值
	m_pTableModel->setItem(iRowNum, 3, new QStandardItem(QString::number(nValue)));
	//遥信类型  单点  双点
	m_pTableModel->setItem(iRowNum, 4, new QStandardItem(QString::number(nQuality)));
	//当前时间
	m_pTableModel->setItem(iRowNum, 5, new QStandardItem(pDateTime));
}

void CRealTimeSOE::Slot_AnalyseSeoBinaryData(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime)
{
	AnalyseRealSeoBinaryData(iDeviceID, nPointID, nValue, nQuality, pDateTime);
}

//清空操作
void CRealTimeSOE::Slot_ContextMenuRequest(const QPoint &point)
{
	Q_UNUSED(point);
	QMenu *pMenu = new QMenu(m_pTableView);
	QAction *pClearAct = new QAction(tr("Clear All"),m_pTableView);
	pMenu->addAction(pClearAct);
	connect(pClearAct, SIGNAL(triggered()), this, SLOT(Slot_clearAction()));
	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
	pClearAct->deleteLater();
}

//清空act
void CRealTimeSOE::Slot_clearAction()
{
	m_pTableModel->removeRows(0, m_pTableModel->rowCount());
}


CRealTimeSOE::~CRealTimeSOE()
{

}
