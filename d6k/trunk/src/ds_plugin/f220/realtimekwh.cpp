#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include "realtimekwh.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"


CRealTimeKwh::CRealTimeKwh(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt)
	: QWidget(parent),
	m_pCommunicate(NULL),
	m_pTableView(NULL),
	m_pTableModel(NULL)
{
	m_iCurrentPointsNum = 0;

	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}

	m_pPointInfo = pPointInfo;
	m_pCommunicate = pCommunicate;
    m_pConfgWgt = pConfgWgt;

	InitWidget();

	InitData();
}

void CRealTimeKwh::InitWidget()
{
	m_pTableView = new QTableView(this);
	m_pTableModel = new QStandardItemModel(this);
	m_pTableView->setModel(m_pTableModel);

	m_pTableView->verticalHeader()->setHidden(true);
	m_pTableView->setSortingEnabled(true);
	m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QStringList lstHorizonLables;
	lstHorizonLables << tr("Remote NUM") << tr("Device NUM") << tr("Destriber") << tr("Value") << tr("Unit") << tr("Time");
	m_pTableModel->setHorizontalHeaderLabels(lstHorizonLables);

	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(m_pTableView);

	connect(m_pCommunicate, SIGNAL(Signal_KwhNormal(int, int, int)), this, SLOT(Slot_AnalyseKwhData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_KwhDisNormal(int, int, int)), this, SLOT(Slot_AnalyseKwhData(int, int, int)));

	connect(m_pCommunicate, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)), this, SLOT(Slot_AnalyseShortTimeKwhData(int, int, float)));
	connect(m_pCommunicate, SIGNAL(Signal_KwhLongTimeNormal(int, int, float)), this, SLOT(Slot_AnalyseLongTimeKwhData(int, int, float)));
}

void CRealTimeKwh::AnalyseRealKwhData(int iDeviceID, int nPointID, float nValue, const QString &strType)
{
	Q_UNUSED(strType);
	Q_UNUSED(iDeviceID);
	//过滤点  去除不需要的点
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

    nPointID = nPointID - m_pConfgWgt->GetKwhStartAddr();

	if (m_pPointInfo->GetKwhMap().count() < m_iCurrentPointsNum)
	{
		m_mapIdItemObj.clear();
		//m_pTableModel->clear();
		m_pTableModel->setRowCount(0);

	}

    if (m_pPointInfo->GetKwhFlag())
    {
        m_mapIdItemObj.clear();
        //m_pTableModel->clear();
        m_pTableModel->setRowCount(0);

        m_pPointInfo->SetKwhFlag(false);
    }

	m_iCurrentPointsNum = m_pPointInfo->GetKwhMap().count();

	if (!m_pPointInfo->GetKwhMap().contains(nPointID))
	{
		//如果不是选中的测点 忽略此条信息
		return;
	}

	QMap<unsigned int, RPT> mapPoint = m_pPointInfo->GetKwhMap();;

	if (m_mapIdItemObj.contains(nPointID))
	{
		//更新
		int iUpdateRow = m_mapIdItemObj[nPointID]->row();
		//m_pTableModel->item(iUpdateRow, 2)->setText(QString::number(nPointID));
		m_pTableModel->item(iUpdateRow, 3)->setText(QString::number(nValue,'f',6));
		//m_pTableModel->item(iUpdateRow, 4)->setText(strType);
		m_pTableModel->item(iUpdateRow, 5)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
	}
	else
	{
		//插入
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
		//插入数据
		//
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(mapPoint[nPointID].NUM)));
		//
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem(QString::number(mapPoint[nPointID].NUM2)));
		//
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(mapPoint[nPointID].Destriber));
		//点值
        m_pTableModel->setItem(iRowNum, 3, new QStandardItem(QString::number(nValue, 'f', 6)));
		//遥信类型  单点  双点
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(mapPoint[nPointID].Unit));
		//当前时间
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
		//记录数据
		m_mapIdItemObj.insert(nPointID, m_pTableModel->item(iRowNum, 0));
	}

}

void CRealTimeKwh::Slot_AnalyseKwhData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealKwhData(iDeviceID, nPointID, nValue, tr("kwh"));
}

void CRealTimeKwh::Slot_AnalyseShortTimeKwhData(int iDeviceID, int nPointID, float nValue)
{
	AnalyseRealKwhData(iDeviceID, nPointID, nValue, tr("ShortTime Kwh"));
}

void CRealTimeKwh::Slot_AnalyseLongTimeKwhData(int iDeviceID, int nPointID, float nValue)
{
	AnalyseRealKwhData(iDeviceID, nPointID, nValue, tr("LongTime Kwh"));
}

CRealTimeKwh::~CRealTimeKwh()
{

}


void CRealTimeKwh::InitData()
{
	m_iCurrentPointsNum = m_pPointInfo->GetKwhMap().count();


	QMap<unsigned int, RPT> mapPoint = m_pPointInfo->GetKwhMap();

	QMap<unsigned int, RPT>::const_iterator i = mapPoint.constBegin();
	while (i != mapPoint.constEnd())
	{

		//插入
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
		//插入数据
		//插入数据
		//
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(i.value().NUM)));
		//
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem(QString::number(i.value().NUM2)));
		//
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(i.value().Destriber));
		//点值
		m_pTableModel->setItem(iRowNum, 3, new QStandardItem(""));
		//遥信类型  单点  双点
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(i.value().Unit));
		//当前时间
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(""));
		//记录数据
		m_mapIdItemObj.insert(i.key(), m_pTableModel->item(iRowNum, 0));


		++i;
	}


}

