#include "scdmainview.h"
#include "dbgstudio/imainwindow.h"
#include "dbgstudio/main_module.h"
#include "dbgstudio/treewidget.h"
#include "dbgstudio/tabwidget.h"
#include "fesapi/fesdatadef.h"
#include "fesdb.h"
#include "memdb.h"
#include <QDebug>
#include <QLabel>
#include <QTableView>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>
#include <QStandardItemModel>

#include "scadaapi/scdsvcapi.h"
static CScdMainView*  s_ptrModule = nullptr;

CScdMainView::CScdMainView()
{
//	m_pMem = std::make_shared<CViewMemDB>();
	m_pRootItem = new QStandardItem(QObject::tr("Scada Manger"));
	QObject::connect(this, SIGNAL(sig_DataView(const int&, const int32u&, const int32u&, const int32u&)), this, SLOT(slot_TabViewAdd(const int&, const int32u&, const int32u&, const int32u&)));
	m_pTimer = new QTimer();
	QObject::connect(m_pTimer, SIGNAL(timeout()),this,SLOT(slot_onTimer()));
	m_bInitalize = false;
	m_bUnInitalize = false;
	m_pCurrentView = nullptr;
	m_pLabel = new QLabel();
}

CScdMainView::~CScdMainView()
{
	CloseAllTabs();
}

void CScdMainView::Init(IMainModule *pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;

	IMainWindow* pMainWindow = m_pCore->GetMainWindow();

	Q_ASSERT(pMainWindow);

	CTreeWidget* pTree = pMainWindow->GetLeftTree();
	Q_ASSERT(pTree);
	if (pTree)
	{
		QStandardItemModel * pModel = dynamic_cast<QStandardItemModel*>(pTree->model());

		QStandardItem* pItem = pModel->invisibleRootItem();

		pItem->appendRow(m_pRootItem);

		QObject::connect(pTree, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slot_TreeDbClicked(const QModelIndex&)));
	}

	QTabWidget * pWidget = m_pCore->GetMainWindow()->GetTabWidget();
	Q_ASSERT(pWidget);
	if (pWidget)
	{
		QObject::connect(pWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(slot_CloseTabWidget(int)));
		QObject::connect(pWidget, SIGNAL(currentChanged(int )), this, SLOT(slot_TabChanged(int)));
	}

	m_pCore->GetMainWindow()->statusBar()->addWidget(m_pLabel);
}

void CScdMainView::UnInit()
{

}

void CScdMainView::SetLoadDeviceName(const QString &strDeviceName)
{
	Q_ASSERT(!strDeviceName.isEmpty() && strDeviceName.length() > 0);
	if (strDeviceName.isEmpty())
	{
		return;
	}
	m_szDeviceName = strDeviceName;

	QString pszDataPath = strDeviceName + "/scada";

	m_pDBAliveFlag = std::make_shared<ACE_Event>(1, 0, USYNC_PROCESS, pszDataPath.toStdString().c_str());

	if (!IsDBAlive(0))
	{
		QMessageBox::information(nullptr,tr("DB_ERROR"),tr("SCADA DB is not alive!!!"));
		m_pLabel->setText(tr("Scd DB is not alive!!!"));
	}
	else
	{
		m_bInitalize = true;
		//内存读取
		if (m_bInitalize)
		{
			LoadDBMem();
		}
	}	
	//定时器的作用就是一直刷新界面 以及一直判断scd 实时库到底是否存活
	m_pTimer->start(1000);

//	int32u nId;
//	GetOccNoByTagName(strDeviceName.toStdString().c_str(),&nId, &nId, &nId, &nId);
}

void CScdMainView::InitTreeView()
{
	//node
	QStandardItem* pItem;
	for (auto i : m_pMem->GetNodeArrs())
	{
		pItem = new QStandardItem(i->NodeTagName);
		pItem->setData(reinterpret_cast<long long>(i), Qt::UserRole + 1);
		pItem->setData(E_NODE, Qt::UserRole + 2);
		pItem->setIcon(QIcon(":/Resources/node.png"));
		InitNode(pItem, i->OccNo);
		m_pRootItem->appendRow(pItem);
	}
}

void CScdMainView::InitNode(QStandardItem* pItem, int32u nNodeOccNo)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	CFesDBR *pFes = m_pMem->GetFesByOccNo(nNodeOccNo);
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}

	QStandardItem * pChannelItem = nullptr;

	for (auto i : pFes->GetChannelArrs())
	{
		Q_ASSERT(i);
		if (i)
		{
			if (strlen(i->ChannelName) == 0)
			{
				pChannelItem = new QStandardItem(QString("node:%1_channel:%2").arg(nNodeOccNo).arg(i->OccNo));
				pChannelItem->setData(reinterpret_cast<long long>(i), Qt::UserRole + 1);
				pChannelItem->setData(E_CHANNEL, Qt::UserRole + 2);
				pChannelItem->setIcon(QIcon(":/Resources/channel.png"));
				InitChannel(pChannelItem, pFes, i->OccNo);
				pItem->appendRow(pChannelItem);
			}
			else
			{
				pChannelItem = new QStandardItem(QString(i->ChannelName) + QString(i->DriverName));
				pChannelItem->setData(reinterpret_cast<long long>(i), Qt::UserRole + 1);
				InitChannel(pChannelItem, pFes, i->OccNo);
				pChannelItem->setIcon(QIcon(":/Resources/channel.png"));
				pChannelItem->setData(E_CHANNEL, Qt::UserRole + 2);
				pItem->appendRow(pChannelItem);
			}
		}
	}
}

void CScdMainView::InitChannel(QStandardItem* pItem, CFesDBR *pFes, int32u nChannelOccNo)
{
	Q_ASSERT(pItem && pFes);
	if (!pItem || !pFes)
	{
		return;
	}
	QStandardItem * pDev = nullptr;
	std::vector<CScdDevice*>  arrDevices;
	bool bRet = pFes->GetScdMemRel()->GetDevArrs(nChannelOccNo, arrDevices);
	if (bRet == false)
		return;

	for (auto i : arrDevices)
	{
		if (i)
		{
			pDev = new QStandardItem(QString("node:%1_channel:%2_dev:%3").arg(i->m_pDevice->NodeOccNo).arg(i->m_pDevice->ChannelOccNo).arg(i->m_pDevice->OccNo));
			pDev->setData(reinterpret_cast<long long>(i->m_pDevice), Qt::UserRole + 1);
			pDev->setData(E_DEVICE, Qt::UserRole + 2);
			pDev->setIcon(QIcon(":/Resources/device.png"));
			InitDevice(pDev, pFes, nChannelOccNo, i->m_pDevice->OccNo);
			pItem->appendRow(pDev);
		}
	}
}

void CScdMainView::InitDevice(QStandardItem* pItem, CFesDBR *pFes, int32u nChannelOccNo, int32u nDevOccNo)
{
	Q_ASSERT(pItem  && pFes);
	if (!pItem || !pFes)
	{
		return;
	}
	QStandardItem * pChild = nullptr;
	CScdDevice* pDev = pFes->GetScdMemRel()->GetDevice(nChannelOccNo, nDevOccNo);

	pChild = new QStandardItem(QObject::tr("AIN[%1]").arg(pDev->GetAinsSize()));
	pChild->setData(reinterpret_cast<long long>(pDev->m_pDevice), Qt::UserRole + 1);
	pChild->setIcon(QIcon(":/Resources/aio.png"));
	pChild->setData(E_AIN, Qt::UserRole + 2);
	pItem->appendRow(pChild);

	pChild = new QStandardItem(QObject::tr("DIN[%1]").arg(pDev->GetDinSize()));
	pChild->setData(reinterpret_cast<long long>(pDev->m_pDevice), Qt::UserRole + 1);
	pChild->setIcon(QIcon(":/Resources/aio.png"));
	pChild->setData(E_DIN, Qt::UserRole + 2);
	pItem->appendRow(pChild);

	pChild = new QStandardItem(QObject::tr("AQ_[%1]").arg(pDev->GetAoutSize()));
	pChild->setData(reinterpret_cast<long long>(pDev->m_pDevice), Qt::UserRole + 1);
	pChild->setIcon(QIcon(":/Resources/aio.png"));
	pChild->setData(E_AOUT, Qt::UserRole + 2);
	pItem->appendRow(pChild);

	pChild = new QStandardItem(QObject::tr("DQ_[%1]").arg(pDev->GetDoutSize()));
	pChild->setData(reinterpret_cast<long long>(pDev->m_pDevice), Qt::UserRole + 1);
	pChild->setIcon(QIcon(":/Resources/aio.png"));
	pChild->setData(E_DOUT, Qt::UserRole + 2);
	pItem->appendRow(pChild);


}



void CScdMainView::slot_TreeDbClicked(const QModelIndex& index)
{
	Q_ASSERT(index.isValid());

	if (!index.isValid())
	{
		return;
	}

	QStandardItemModel * pModel = dynamic_cast<QStandardItemModel*>(m_pCore->GetMainWindow()->GetLeftTree()->model());

	if (pModel)
	{
		QStandardItem* pItem = pModel->itemFromIndex(index);
		int nType = pItem->data(Qt::UserRole + 2).toInt();

		switch (nType)
		{
		case E_ROOT:
			break;
		case E_NODE:
			break;
		case E_CHANNEL:
		{
			CHANNEL* pChannel = reinterpret_cast<CHANNEL *>(pItem->data(Qt::UserRole + 1).toLongLong());

			if (pChannel)
			{
				emit sig_DataView(nType, pChannel->NodeOccNo, pChannel->OccNo, 1);
			}
			break;
		}
		case E_DEVICE:
		case E_AIN:
		case E_DIN:
		case E_AOUT:
		case E_DOUT:
		{
			DEVICE* pDev = reinterpret_cast<DEVICE *>(pItem->data(Qt::UserRole + 1).toLongLong());

			if (pDev)
			{
				emit sig_DataView(nType, pDev->NodeOccNo, pDev->ChannelOccNo, pDev->OccNo);
			}
			break;
		}
		default:
			break;
		}
	}

}

void CScdMainView::slot_TabViewAdd(const int& nType, const int32u& nNodeOccNo, const int32u& nChannelOccNo, const int32u& nDevOccNo)
{
	Q_ASSERT(nType != 0);
	if (nType == 0)
	{
		return;
	}
	QString szName;
	FormatNameString(nType, nNodeOccNo, nChannelOccNo, nDevOccNo, szName);

	bool bRet = IsFileAdded(szName);
	if (bRet)
	{
		return;
	}
	QTableView* pView = new QTableView;
	
	CScdDataModel* pModel = new CScdDataModel(m_pMem, pView);
	pModel->Initalize(nType, nNodeOccNo, nChannelOccNo, nDevOccNo);

	SetHeaderList(pModel, nType);
	SetModelData(pModel, nType,nNodeOccNo,nChannelOccNo,nDevOccNo);
	
	
	pView->setModel(pModel);
	pView->setAlternatingRowColors(true);
	
	m_pCore->GetMainWindow()->GetTabWidget()->AddTab(pView, szName, szName);
	
	m_pCore->GetMainWindow()->GetTabWidget()->setCurrentWidget(pView);
	
	m_pCurrentView = pView;

	m_mapView.insert(std::make_pair(szName,pView));
}

void CScdMainView::SetHeaderList(CScdDataModel* pModel,int nType)
{
	Q_ASSERT(nType != INVALID_TYPE && nType < MAX_TYPE);

	if (nType == INVALID_TYPE || nType > MAX_TYPE)
	{
		return;
	}

	Q_ASSERT(pModel);
	if (!pModel)
	{
		return;
	}
	m_szHeaderList.clear();
	switch (nType)
	{
		case E_ROOT:
		case E_NODE:
			break;
		case E_CHANNEL:
		{
			m_szHeaderList.push_back(tr("Channel_OccNo"));
			m_szHeaderList.push_back(tr("Channel_NameOccNo"));
			m_szHeaderList.push_back(tr("Channel_IsDefined"));
			m_szHeaderList.push_back(tr("Channel_ScanEnable"));
			m_szHeaderList.push_back(tr("Channel_Init"));
			m_szHeaderList.push_back(tr("Chanel_Quality"));
			m_szHeaderList.push_back(tr("Channel_PeerQuality"));
			m_szHeaderList.push_back(tr("Channel_Heartbeat"));
			m_szHeaderList.push_back(tr("Channel_PrintOn"));
			m_szHeaderList.push_back(tr("Channel_WorkNet"));

			m_szHeaderList.push_back(tr("Channel_MasterComPort"));
			m_szHeaderList.push_back(tr("Channel_SlaveComPort"));
			m_szHeaderList.push_back(tr("Channel_BaudRate"));
			m_szHeaderList.push_back(tr("Channel_Parity"));
			m_szHeaderList.push_back(tr("Channel_DataBit"));
			m_szHeaderList.push_back(tr("Channel_StopBit"));
			m_szHeaderList.push_back(tr("Channel_Pad"));

			m_szHeaderList.push_back(tr("Channel_MasterIP"));
			m_szHeaderList.push_back(tr("Channel_SlaveIP"));
			m_szHeaderList.push_back(tr("Channel_MasterPort"));
			m_szHeaderList.push_back(tr("Channel_SlavePort"));

			m_szHeaderList.push_back(tr("Channel_State"));
			m_szHeaderList.push_back(tr("Channel_Prority"));
			m_szHeaderList.push_back(tr("Channel_SetTimeAttr"));
			m_szHeaderList.push_back(tr("Channel_DeviceCount"));
			m_szHeaderList.push_back(tr("Channel_NodeOccNo"));
			m_szHeaderList.push_back(tr("Channel_ChannelName"));
			m_szHeaderList.push_back(tr("Channel_DriverName"));
			m_szHeaderList.push_back(tr("Channel_DriverType"));
			m_szHeaderList.push_back(tr("Channel_StateCore"));
			m_szHeaderList.push_back(tr("Channel_StateDriver"));
			break;
		}
		case E_DEVICE:
		{
			m_szHeaderList.push_back(tr("DEVICE_OccNo"));
			m_szHeaderList.push_back(tr("DEVICE_NameOccNo"));
			m_szHeaderList.push_back(tr("DEVICE_NodeOccNo"));
			m_szHeaderList.push_back(tr("DEVICE_ChannelOccNo"));
			m_szHeaderList.push_back(tr("DEVICE_ModelOccNo"));
			m_szHeaderList.push_back(tr("DEVICE_Type"));
			m_szHeaderList.push_back(tr("DEVICE_Pad1"));
			m_szHeaderList.push_back(tr("DEVICE_Pad2"));
			m_szHeaderList.push_back(tr("DEVICE_ADDRESS"));
			m_szHeaderList.push_back(tr("DEVICE_Extension"));
			m_szHeaderList.push_back(tr("DEVICE_IsDefined"));
			m_szHeaderList.push_back(tr("DEVICE_ScanEnable"));
			m_szHeaderList.push_back(tr("DEVICE_Init"));
			m_szHeaderList.push_back(tr("DEVICE_Quality"));
			break;
		}
		case E_AIN:
		{
			m_szHeaderList.push_back(tr("AIN_OccNo"));
			m_szHeaderList.push_back(tr("AIN_BlockNo"));
			m_szHeaderList.push_back(tr("AIN_NameOccNo"));
			m_szHeaderList.push_back(tr("IsSpare"));
			//	m_szHeaderList.push_back(tr("AIN_Pad1"));
			m_szHeaderList.push_back(tr("AIN_Pad2"));
			m_szHeaderList.push_back(tr("AIN_Address"));
			m_szHeaderList.push_back(tr("AIN_State"));
			m_szHeaderList.push_back(tr("AIN_IsDefined"));
			m_szHeaderList.push_back(tr("AIN_ScanEnable"));
			m_szHeaderList.push_back(tr("AIN_Init"));
			m_szHeaderList.push_back(tr("AIN_Quality"));
			m_szHeaderList.push_back(tr("AIN_ManSet"));

			//m_szHeaderList.push_back(tr("AIN_AlarmState"));
			m_szHeaderList.push_back(tr("AIN_SignalValue"));
			m_szHeaderList.push_back(tr("AIN_RawValue"));
			m_szHeaderList.push_back(tr("AIN_Value"));
			m_szHeaderList.push_back(tr("AIN_OldValue"));
			m_szHeaderList.push_back(tr("AIN_InitValue"));
			m_szHeaderList.push_back(tr("AIN_DataSource"));
			m_szHeaderList.push_back(tr("AIN_RangeL"));
			m_szHeaderList.push_back(tr("AIN_RangeH"));

			m_szHeaderList.push_back(tr("AIN_HighQua"));
			m_szHeaderList.push_back(tr("AIN_LowQua"));
			m_szHeaderList.push_back(tr("AIN_NodeOccNo"));
			m_szHeaderList.push_back(tr("AIN_ChannelOccNo"));
			m_szHeaderList.push_back(tr("AIN_DeviceOccNo"));

			m_szHeaderList.push_back(tr("AIN_AlarmOccNo"));
			m_szHeaderList.push_back(tr("AIN_ExpressOccNo"));
			m_szHeaderList.push_back(tr("AIN_TransformType"));
			m_szHeaderList.push_back(tr("AIN_TransformOccNo"));
			m_szHeaderList.push_back(tr("AIN_PinLabelOccNo"));
			m_szHeaderList.push_back(tr("AIN_UnitNameOccNo"));
			m_szHeaderList.push_back(tr("AIN_LastUpdateTime"));

			break;
		}
		case E_DIN:
		{
			m_szHeaderList.push_back(tr("DIN_OccNo"));
			m_szHeaderList.push_back(tr("DIN_BlockNo"));
			m_szHeaderList.push_back(tr("DIN_NameOccNo"));
			m_szHeaderList.push_back(tr("DIN_NeedTranmitFlag"));
			m_szHeaderList.push_back(tr("DIN_Pad2"));
			m_szHeaderList.push_back(tr("DIN_IsSpare"));
			m_szHeaderList.push_back(tr("DIN_Address"));

			m_szHeaderList.push_back(tr("DIN_InitValEnable"));
			m_szHeaderList.push_back(tr("DIN_InitValue"));
			m_szHeaderList.push_back(tr("DIN_InitQua"));

			m_szHeaderList.push_back(tr("DIN_SlaveOccNo"));
			m_szHeaderList.push_back(tr("DIN_DDIFilter"));
			m_szHeaderList.push_back(tr("DIN_DDIFlag"));
			m_szHeaderList.push_back(tr("DIN_DDIInUse"));

			m_szHeaderList.push_back(tr("DIN_State"));



			m_szHeaderList.push_back(tr("DIN_IsDefined"));
			m_szHeaderList.push_back(tr("DIN_ScanEnable"));
			m_szHeaderList.push_back(tr("DIN_Init"));
			m_szHeaderList.push_back(tr("DIN_Quality"));
			m_szHeaderList.push_back(tr("DIN_ManSet"));



			m_szHeaderList.push_back(tr("DIN_Value"));
			m_szHeaderList.push_back(tr("DIN_NegValue"));
			m_szHeaderList.push_back(tr("DIN_RawValue"));


			m_szHeaderList.push_back(tr("DIN_CtrlByte"));
			m_szHeaderList.push_back(tr("DIN_IsSOE"));

			m_szHeaderList.push_back(tr("DIN_StartCtrl"));
			m_szHeaderList.push_back(tr("DIN_SignalType"));

			m_szHeaderList.push_back(tr("DIN_NodeOccNo"));
			m_szHeaderList.push_back(tr("DIN_ChannelOccNo"));
			m_szHeaderList.push_back(tr("DIN_DeviceOccNo"));
			m_szHeaderList.push_back(tr("DIN_AlarmOccNo"));
			m_szHeaderList.push_back(tr("DIN_DataSource"));

			m_szHeaderList.push_back(tr("DIN_ExpressOccNo"));
			m_szHeaderList.push_back(tr("DIN_Desc0_OccNo"));
			m_szHeaderList.push_back(tr("DIN_Desc1_OccNo"));
			m_szHeaderList.push_back(tr("DIN_PinLabelOccNo"));
			m_szHeaderList.push_back(tr("DIN_LastUpdateTime"));


			break;
		}
		case E_AOUT:
		{
			m_szHeaderList.push_back(tr("AOUT_OccNo"));
			m_szHeaderList.push_back(tr("AOUT_BlockNo"));
			m_szHeaderList.push_back(tr("AOUT_NameOccNo"));
			m_szHeaderList.push_back(tr("AOUT_State"));
			m_szHeaderList.push_back(tr("AOUT_IsDefined"));
			m_szHeaderList.push_back(tr("AOUT_ScanEnable"));
			m_szHeaderList.push_back(tr("AOUT_Init"));
			m_szHeaderList.push_back(tr("AOUT_Quality"));
			m_szHeaderList.push_back(tr("AOUT_Output"));
			m_szHeaderList.push_back(tr("AOUT_RawData"));
			m_szHeaderList.push_back(tr("AOUT_DataSource"));
			m_szHeaderList.push_back(tr("AOUT_NodeOccNo"));
			m_szHeaderList.push_back(tr("AOUT_ChannelOccNo"));
			m_szHeaderList.push_back(tr("AOUT_DeviceOccNo"));
			m_szHeaderList.push_back(tr("AOUT_AlarmOccNo"));
			m_szHeaderList.push_back(tr("AOUT_ExpressOccNo"));
			m_szHeaderList.push_back(tr("AOUT_TransformType"));
			m_szHeaderList.push_back(tr("AOUT_TransformOccNo"));
			m_szHeaderList.push_back(tr("AOUT_PinLabelOccNo"));
			m_szHeaderList.push_back(tr("AOUT_UnitNameOccNo"));
			m_szHeaderList.push_back(tr("AOUT_LastUpdateTime"));

			break;
		}
		case E_DOUT:
		{
			m_szHeaderList.push_back(tr("DOUT_OccNo"));
			m_szHeaderList.push_back(tr("DOUT_BlockNo"));
			m_szHeaderList.push_back(tr("DOUT_NameOccNo"));
			m_szHeaderList.push_back(tr("DOUT_State"));
			m_szHeaderList.push_back(tr("DOUT_IsDefined"));
			m_szHeaderList.push_back(tr("DOUT_ScanEna"));
			m_szHeaderList.push_back(tr("DOUT_Init"));
			m_szHeaderList.push_back(tr("DOUT_Quality"));
			m_szHeaderList.push_back(tr("DOUT_Output"));
			m_szHeaderList.push_back(tr("DOUT_RelayState"));
			m_szHeaderList.push_back(tr("DOUT_DataSource"));
			m_szHeaderList.push_back(tr("DOUT_CtrlType"));
			m_szHeaderList.push_back(tr("DOUT_Filter"));
			m_szHeaderList.push_back(tr("DOUT_NodeOccNo"));
			m_szHeaderList.push_back(tr("DOUT_ChannelOccNo"));
			m_szHeaderList.push_back(tr("DOUT_DeviceOccNo"));
			m_szHeaderList.push_back(tr("DOUT_AlarmOccNo"));
			m_szHeaderList.push_back(tr("DOUT_ExpressionOccNo"));
			m_szHeaderList.push_back(tr("DOUT_PinLabelOccNo"));
			m_szHeaderList.push_back(tr("DOUT_LastUpdateTime"));

			break;
		}
		default:
		{
			m_szHeaderList.clear();
			break;
		}
	}
	pModel->SetHeaderList(m_szHeaderList);
}

void CScdMainView::SetModelData(CScdDataModel* pModel, const int& nType, const int32u& nNodeOccNo, const int32u& nChannelOccNo, const int32u& nDevOccNo)
{
	Q_ASSERT(nType != INVALID_TYPE && nType < MAX_TYPE);

	if (nType == INVALID_TYPE || nType > MAX_TYPE)
	{
		return;
	}

	Q_ASSERT(pModel);
	if (!pModel)
	{
		return;
	}


	switch (nType)
	{
	case E_ROOT:
	case E_NODE:
		break;
	case E_CHANNEL:
	{
		pModel->SetRowCount(m_szHeaderList.size());
		pModel->SetColCount(1);
		break;
	}
	case E_DEVICE:
	{
		pModel->SetRowCount(m_szHeaderList.size());
		pModel->SetColCount(1);
		break;
	}
	case E_AIN:
	{
		pModel->SetColCount(m_szHeaderList.size());
		int nSize=m_pMem->GetFesByOccNo(nNodeOccNo)->GetScdMemRel()->GetChannelByOccNo(nChannelOccNo)->GetDeviceByOccNo(nDevOccNo)->GetAinsSize();
		pModel->SetRowCount(nSize);
		break;
	}
	case E_DIN:
	{
		pModel->SetColCount(m_szHeaderList.size());
		int nSize = m_pMem->GetFesByOccNo(nNodeOccNo)->GetScdMemRel()->GetChannelByOccNo(nChannelOccNo)->GetDeviceByOccNo(nDevOccNo)->GetDinSize();
		pModel->SetRowCount(nSize);
		break;
	}
	case E_AOUT:
	{
		pModel->SetColCount(m_szHeaderList.size());
		int nSize = m_pMem->GetFesByOccNo(nNodeOccNo)->GetScdMemRel()->GetChannelByOccNo(nChannelOccNo)->GetDeviceByOccNo(nDevOccNo)->GetAoutSize();
		pModel->SetRowCount(nSize);
		break;
	}
	case E_DOUT:
	{
		pModel->SetColCount(m_szHeaderList.size());
		int nSize = m_pMem->GetFesByOccNo(nNodeOccNo)->GetScdMemRel()->GetChannelByOccNo(nChannelOccNo)->GetDeviceByOccNo(nDevOccNo)->GetDoutSize();
		pModel->SetRowCount(nSize);
		break;
	}
	default:
		break;
	}
}

void CScdMainView::slot_onTimer()
{
	if (m_pCurrentView)
	{
		m_pCurrentView->reset();
	}
	//定时判断scd是否存活
	bool nRet = IsDBAlive(0);

	if (!nRet)
	{
		//若不存活，则程序界面退出
		if (m_bInitalize)
		{
			UnLoadDBMem();
			m_bInitalize = !m_bInitalize;
		}		
		m_pLabel->setText(tr("SCADA DB is not alive!!!"));
	}
	else
	{
		if (!m_bInitalize)
		{
			LoadDBMem();
			m_bInitalize = !m_bInitalize;
		}
		m_pLabel->setText(tr("SCADA DB is alive!!!"));
	}	
}

void CScdMainView::slot_CloseTabWidget(int nIndex)
{
	auto pWidget = m_pCore->GetMainWindow()->GetTabWidget()->widget(nIndex);
	Q_ASSERT(pWidget);
	if (!pWidget)
	{
		return;
	}
	QString szName = m_pCore->GetMainWindow()->GetTabWidget()->tabText(nIndex);
	Q_ASSERT(!szName.isEmpty());
	if (szName.isEmpty())
	{
		return;
	}
	m_pCore->GetMainWindow()->GetTabWidget()->removeTab(nIndex);

	auto it = m_mapView.find(szName);
	if (it==m_mapView.end())
	{
		return;
	}
	else
	{			
		it->second->deleteLater();
		m_mapView.erase(szName);		
	}
}

bool CScdMainView::IsFileAdded(QString szName)
{
	Q_ASSERT(!szName.isEmpty());

	if (szName.isEmpty())
	{
		return false;
	}

	auto it = m_mapView.find(szName);

	if (it==m_mapView.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CScdMainView::FormatNameString(int nType, const int32u& nNodeOccNo, const int32u& nChannelOccNo, const int32u& nDevOccNo, QString& szName)
{
	switch (nType)
	{
	case E_ROOT:
	case E_NODE:
		break;
	case E_CHANNEL:
	{
		szName = QString("node:%1_channel:%2").arg(nNodeOccNo).arg(nChannelOccNo);
		break;
	}
	case E_DEVICE:
	{
		szName = QString("node:%1_channel:%2_device:%3").arg(nNodeOccNo).arg(nChannelOccNo).arg(nDevOccNo);
		break;
	}		
	case E_AIN:
	{
		szName = QString("node:%1_channel:%2_device:%3_ain").arg(nNodeOccNo).arg(nChannelOccNo).arg(nDevOccNo);
		break;
	}
	case E_DIN:
	{
		szName = QString("node:%1_channel:%2_device:%3_din").arg(nNodeOccNo).arg(nChannelOccNo).arg(nDevOccNo);
		break;
	}
	case E_AOUT:
	{
		szName = QString("node:%1_channel:%2_device:%3_aout").arg(nNodeOccNo).arg(nChannelOccNo).arg(nDevOccNo);
		break;
	}
	case E_DOUT:
	{
		szName = QString("node:%1_channel:%2_device:%3_dout").arg(nNodeOccNo).arg(nChannelOccNo).arg(nDevOccNo);
		break;
	}
	default:
		break;
	}
}

bool CScdMainView::IsDBAlive(int nTimeOut)
{
	if (m_pDBAliveFlag == nullptr)
		return false;

	ACE_Time_Value tm_val;

	int nRet = m_pDBAliveFlag->wait(&tm_val, 0);
	// 注意 返回值
	return (nRet == -1) ? false : true;
}

bool CScdMainView::LoadDBMem()
{
	if (!m_pMem)
	{
		m_pMem = std::make_shared<CViewMemDB>();
	}	
	Q_ASSERT(m_pMem);

	if (m_pMem->Initialize(m_szDeviceName.toStdString().c_str(), 0))
	{
		InitTreeView();
	}
	else
	{
		m_pLabel->setText(tr("SCADA DB initalize error,please check"));
	}

	return true;
}

bool CScdMainView::UnLoadDBMem()
{
	CloseAllTabs();

	m_pRootItem->removeRows(0, m_pRootItem->rowCount());

	m_pMem.reset();

	return true;
}

void CScdMainView::CloseAllTabs()
{

	for (int i = 0; i < m_pCore->GetMainWindow()->GetTabWidget()->count(); )
	{
		QString szName = m_pCore->GetMainWindow()->GetTabWidget()->tabText(i);

		bool bRet = IsFileAdded(szName);

		if (bRet)
		{
			m_pCore->GetMainWindow()->GetTabWidget()->removeTab(i);
			auto it = m_mapView.find(szName);
			if (it == m_mapView.end())
			{
				continue;
			}
			else
			{
				m_mapView.erase(it->first);
			}			
		}
		else
		{
			i++;
		}
	}

}

void CScdMainView::slot_TabChanged(int nIndex)
{
	if (nIndex>=0)
	{
		m_pCurrentView = dynamic_cast<QTableView*>(m_pCore->GetMainWindow()->GetTabWidget()->widget(nIndex));
	}
	else
	{
		m_pCurrentView = nullptr;
	}
}


extern SCDVIEW_EXPORT IPluginModule* CreateModule()
{
	s_ptrModule = new CScdMainView;
	return s_ptrModule;
}

extern SCDVIEW_EXPORT void DestroyModule()
{
	if (s_ptrModule)
	{
		delete s_ptrModule;
		s_ptrModule = nullptr;
	}
}