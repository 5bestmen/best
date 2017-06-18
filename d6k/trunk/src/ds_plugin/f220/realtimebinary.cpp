#include <QMessageBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDateTime>
#include <QDebug>
#include "realtimebinary.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"

CRealTimeBinary::CRealTimeBinary(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt)
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

void CRealTimeBinary::InitWidget()
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
	lstHorizonLables << tr("Remote NUM") << tr("Device NUM") << tr("Destriber") << tr("Value") << tr("Binary Type") << tr("Time");
	m_pTableModel->setHorizontalHeaderLabels(lstHorizonLables);

	QVBoxLayout *pWgtlayout = new QVBoxLayout(this);
	pWgtlayout->addWidget(m_pTableView);

	connect(m_pCommunicate, SIGNAL(Signal_OnePointRemote(int, int, int)), this, SLOT(Slot_AnalyseoneBinaryData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_onePointDisRemote(int, int, int)), this, SLOT(Slot_AnalyseoneBinaryData(int, int, int)));

	connect(m_pCommunicate, SIGNAL(Signal_DoublePointRemote(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));

	connect(m_pCommunicate, SIGNAL(Signal_BinaryGroupPoint(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));
	connect(m_pCommunicate, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)), this, SLOT(Slot_AnalyseDoubleBinaryData(int, int, int)));


}

void CRealTimeBinary::InitData()
{
	m_iCurrentPointsNum = m_pPointInfo->GetBinaryMap().count();


	QMap<unsigned int, RPT> mapPoint = m_pPointInfo->GetBinaryMap();

	QMap<unsigned int, RPT>::const_iterator i = mapPoint.constBegin();
	while (i != mapPoint.constEnd())
	{

		//插入
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
		//插入数据
		// 远东序号
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(i.value().NUM)));
		//装置序号
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem(QString::number(i.value().NUM2)));
		//描述
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(i.value().Destriber));
		//点值
		m_pTableModel->setItem(iRowNum, 3, new QStandardItem(""));
		//遥信类型  单点  双点
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(""));
		//当前时间
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(""));
		//记录数据
		m_mapIdItemObj.insert(i.key(), m_pTableModel->item(iRowNum, 0));


		++i;
	}


}


CRealTimeBinary::~CRealTimeBinary()
{

}


void CRealTimeBinary::AnalyseRealBinaryData(int iDeviceID, int nPointID, int nValue, const QString &strType)
{
    //过滤点  去除不需要的点
	Q_UNUSED(iDeviceID);
	Q_ASSERT(m_pPointInfo);

	if (strType.toInt() == eSignal)
	{
		//单点
		nPointID = nPointID - m_pConfgWgt->GetBinaryStartAddr();
	}
	else
	{
		//双点
		nPointID = nPointID - m_pConfgWgt->GetDoubleBinaryStartAddr();
	}
    

	if (m_pPointInfo == NULL)
	{
		return;
	}
	
	if (m_pPointInfo->GetBinaryMap().count() < m_iCurrentPointsNum)
	{
		m_mapIdItemObj.clear();
		//m_pTableModel->clear();
		m_pTableModel->setRowCount(0);

	}
	
	/*
    if (m_pPointInfo->GetBinaryFlag())
    {
        m_mapIdItemObj.clear();
        //m_pTableModel->clear();
        m_pTableModel->setRowCount(0);

        m_pPointInfo->SetBinaryFlag(false);
    }
	*/

	m_iCurrentPointsNum = m_pPointInfo->GetBinaryMap().count();

	if (!m_pPointInfo->GetBinaryMap().contains(nPointID))
	{
		//如果不是选中的测点 忽略此条信息
		return;
	}

	QMap<unsigned int, RPT> mapPoint = m_pPointInfo->GetBinaryMap();

	if (m_mapIdItemObj.contains(nPointID))
	{
		//更新
		int iUpdateRow = m_mapIdItemObj[nPointID]->row();
		//m_pTableModel->item(iUpdateRow, 2)->setText(QString::number(nPointID));
		m_pTableModel->item(iUpdateRow, 5)->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

		if (strType.toInt() ==  eSignal)
		{
			QString strShow;
			if (nValue == 0)
			{
				strShow = tr("OFF");
			}
			else if (nValue == 1)
			{
				strShow = tr("ON");
			}
			m_pTableModel->item(iUpdateRow, 3)->setText(strShow);
			m_pTableModel->item(iUpdateRow, 4)->setText(tr("Single"));

		}
		else if (strType.toInt() == eDouble)
		{
			QString strShow;
			if (nValue == 0)
			{
				strShow = tr("Unknown");
			}
			else if (nValue == 1)
			{
				strShow = tr("OFF");
			}
			else if (nValue == 2)
			{
				strShow = tr("ON");
			}
			else if (nValue == 3)
			{
				strShow = tr("Invaild");
			}
			m_pTableModel->item(iUpdateRow, 3)->setText(strShow);
			m_pTableModel->item(iUpdateRow, 4)->setText(tr("Double"));

		}
		else if (strType.toInt() == eSignalGroup)
		{
			m_pTableModel->item(iUpdateRow, 3)->setText(QString::number(nValue));
			m_pTableModel->item(iUpdateRow, 4)->setText(tr("Group"));

		}
	}
	else
	{
		//插入
		//获取当前的行数
		int iRowNum = m_pTableModel->rowCount();
		//插入数据
		// 远东序号
		m_pTableModel->setItem(iRowNum, 0, new QStandardItem(QString::number(mapPoint[nPointID].NUM)));
		//装置序号
		m_pTableModel->setItem(iRowNum, 1, new QStandardItem(QString::number(mapPoint[nPointID].NUM2)));
		//描述
		m_pTableModel->setItem(iRowNum, 2, new QStandardItem(mapPoint[nPointID].Destriber));
		//点值
		m_pTableModel->setItem(iRowNum, 3, new QStandardItem(QString::number(nValue)));
		//遥信类型  单点  双点
		m_pTableModel->setItem(iRowNum, 4, new QStandardItem(strType));
		//当前时间
		m_pTableModel->setItem(iRowNum, 5, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
		//记录数据
		m_mapIdItemObj.insert(nPointID, m_pTableModel->item(iRowNum, 0));
	}
	
}

void CRealTimeBinary::Slot_AnalyseoneBinaryData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealBinaryData(iDeviceID, nPointID, nValue, QString::number(eSignal));
}

void CRealTimeBinary::Slot_AnalyseDoubleBinaryData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealBinaryData(iDeviceID, nPointID, nValue, QString::number(eDouble));
}

void CRealTimeBinary::Slot_AnalyseGroupBinaryData(int iDeviceID, int nPointID, int nValue)
{
	AnalyseRealBinaryData(iDeviceID, nPointID, nValue, QString::number(eSignalGroup));
}

