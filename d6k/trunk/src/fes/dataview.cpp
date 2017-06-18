#include "dataview.h"
#include "db_svc.h"
#include "datamodel.h"
#include "alarmfes/alarmfes.h"
#include "log/log.h"
#include "doutoperdialog.h"
#include "aoutoperdialog.h"
#include <QFile>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLibrary>
#include <QModelIndex>

typedef QWidget *(*fes_alarm)(const QString&);
typedef int (*alarm_stop)();

CDataView::CDataView(QWidget *parent, std::shared_ptr<CDbSvc> &pMem, const QString& szName)
	: QDialog(parent), m_pMemDB(pMem), m_szRdbName (szName)
{
	ui.setupUi(this);
	m_szRdbName = QString::null;

	m_pTabWidget = std::make_shared<QTabWidget>();
	m_pTabWidget->setTabsClosable(true);
	m_pTabDataInfoWidget = std::make_shared<QTabWidget>();
	m_pTimer = std::make_shared<QTimer>();
	m_szRdbName = szName;
	m_pTimer->start(1000);

	InitView();
	setMinimumSize(QSize(800, 600));

	QObject::connect(m_pTree.get(), SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slot_on_dbClicked(QTreeWidgetItem*, int)));
	QObject::connect(m_pTabWidget.get(), SIGNAL(tabCloseRequested(int)), this, SLOT(slot_on_RemoveTab(int)));
	QObject::connect(m_pTimer.get(), SIGNAL(timeout()),this,SLOT(slot_on_Timer()));

	//窗口标记
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowCloseButtonHint;

	setWindowFlags(flags);

	LogMsg("dataview", QObject::tr("dataView module initalize").toStdString().c_str(), 0, nullptr);
}

CDataView::~CDataView()
{
	for (auto i : m_VecDataView)
	{
		delete i;
		i = Q_NULLPTR;
	}
	
	m_LibFes.unload();
}

/*! \fn bool CGridView::InitView()
*********************************************************************************************************
** \brief CGridView::InitView
** \details 初始化视图
** \return bool
** \author xingzhibing
** \date 2016年9月14日
** \note
********************************************************************************************************/
bool CDataView::InitView()
{
	//TREE
	m_pTree = std::make_shared<QTreeWidget>(this);

	m_pTree->setHeaderLabel(tr("FesParaInfo"));

	//工程基点
	QTreeWidgetItem* pProjItem = new QTreeWidgetItem(m_pTree.get());

	pProjItem->setText(0, tr("RootProj"));

	pProjItem->setData(0, DataType_ROLE, PROJECT_TYPE);

	m_pTree->addTopLevelItem(pProjItem);

	size_t nChannelCount = m_pMemDB->GetChannelMgr()->GetChannelCount();

	QString szChannelName, szDevName;

	for (int i = 0; i < nChannelCount; ++i)
	{
		CChannelPtr pChannel = m_pMemDB->GetChannelMgr()->GetChannel(i);

		if (pChannel)
		{
			QTreeWidgetItem* pChannelItem = new QTreeWidgetItem(pProjItem);

			size_t nDeviceCount = pChannel->GetDevCount();

			szChannelName = QString("ChannelNo:%1,DeviceNum:%2").arg(pChannel->GetChannelInfo()->OccNo).arg(pChannel->GetDevCount());

			pChannelItem->setText(0, szChannelName);

			pChannelItem->setData(0, DataDef_ROLE, reinterpret_cast<long long>(pChannel.get()));

			pChannelItem->setData(0, DataType_ROLE, CHANNEL_GERENAL_TYPE);

			pProjItem->addChild(pChannelItem);

			for (int j = 0; j < nDeviceCount; ++j)
			{
				CDevicePtr  pDevice = pChannel->GetDevice(j);
				if (pDevice)
				{
					QTreeWidgetItem* pDevItem = new QTreeWidgetItem(pChannelItem);

					szDevName = QString("DeviceNo:%1").arg(pDevice->GetDevicePtr()->OccNo);

					pDevItem->setText(0, szDevName);

					pDevItem->setData(0, DataDef_ROLE, reinterpret_cast<long long>(pDevice.get()));

					pDevItem->setData(0, DataType_ROLE, DEVICE_GERENAL_TYPE);

					pChannelItem->addChild(pDevItem);

					//AIN
					QTreeWidgetItem* pAinItem = new QTreeWidgetItem(pDevItem);
					pAinItem->setText(0, tr("AIN  :NUM(%1)").arg(pDevice->GetAinCount()));
					pAinItem->setData(0, DataType_ROLE, AIN_TYPE);
					pAinItem->setData(0, DataDevOcc_ROLE, QString::number(pDevice->GetOccNo()));

					//DIN
					QTreeWidgetItem* pDinItem = new QTreeWidgetItem(pDevItem);
					pDinItem->setText(0, tr("DIN  :NUM(%1)").arg(pDevice->GetDinCount()));
					pDinItem->setData(0, DataType_ROLE, DIN_TYPE);
					pDinItem->setData(0, DataDevOcc_ROLE, QString::number(pDevice->GetOccNo()));

					//AOUT
					QTreeWidgetItem* pAoutItem = new QTreeWidgetItem(pDevItem);
					pAoutItem->setText(0, tr("AOUT:NUM(%1)").arg(pDevice->GetAoutCount()));
					pAoutItem->setData(0, DataDevOcc_ROLE, QString::number(pDevice->GetOccNo()));
					pAoutItem->setData(0, DataType_ROLE, AOUT_TYPE);
					//DOUT
					QTreeWidgetItem* pDoutItem = new QTreeWidgetItem(pDevItem);
					pDoutItem->setText(0, tr("DOUT:NUM(%1)").arg(pDevice->GetDoutCount()));
					pDoutItem->setData(0, DataDevOcc_ROLE, QString::number(pDevice->GetOccNo()));
					pDoutItem->setData(0, DataType_ROLE, DOUT_TYPE);
				}
			}
		}
	}

	QSplitter* pSplitter = new QSplitter(Qt::Horizontal);
	pSplitter->addWidget(m_pTree.get());

	QSplitter* pRightSplitter = new QSplitter(Qt::Vertical);
	pRightSplitter->addWidget(m_pTabWidget.get());
	pRightSplitter->addWidget(m_pTabDataInfoWidget.get());



	const QString szModuleName = "alarmfes";

	m_LibFes.setFileName(szModuleName);

	if (m_LibFes.load())
	{
		fes_alarm  fnFesAlarm = (fes_alarm)m_LibFes.resolve("GetAlarmMainWindow");
		Q_ASSERT(fnFesAlarm);
		if (fnFesAlarm)
		{
			QWidget* pWidget = fnFesAlarm(m_szRdbName);

			pWidget->setParent(this);

			m_pTabDataInfoWidget->addTab((QWidget*)pWidget, "alarm");

		}
	}
	LogMsg("dataview", QObject::tr("alarmview  module started").toStdString().c_str(), 0, nullptr);
	
	pRightSplitter->setStretchFactor(0, 80);
	pRightSplitter->setStretchFactor(1, 1);

	pSplitter->addWidget(pRightSplitter);
	pSplitter->setStretchFactor(1, 60);


	QVBoxLayout* pLay = new QVBoxLayout;
	pLay->addWidget(pSplitter);
	setLayout(pLay);

	return true;
}

/*! \fn void CGridView::InitData()
*********************************************************************************************************
** \brief CGridView::InitData
** \details 初始化数据
** \return void
** \author xingzhibing
** \date 2016年9月14日
** \note
********************************************************************************************************/
void CDataView::InitData()
{

}

/*! \fn void CDataView::slot_on_tabChanged(int index)
*********************************************************************************************************
** \brief CDataView::slot_on_tabChanged
** \details
** \param index
** \return void
** \author xingzhibing
** \date 2016年9月19日
** \note
********************************************************************************************************/
void CDataView::slot_on_tabChanged(int index)
{
	Q_UNUSED(index);
}

/*! \fn void CDataView::slot_on_dbClicked(QTreeWidgetItem* item, int col)
*********************************************************************************************************
** \brief CDataView::slot_on_dbClicked
** \details 双击进行数据切换
** \param item
** \param col
** \return void
** \author xingzhibing
** \date 2016年9月19日
** \note
********************************************************************************************************/
void CDataView::slot_on_dbClicked(QTreeWidgetItem* item, int col)
{
	Q_ASSERT(item);

	//先判断是否存在
	for (int i = 0; i < m_pTabWidget->count(); ++i)
	{
		QString szName = m_pTabWidget->tabText(i);
	}
	QString szName;

	int nType = item->data(0, DataType_ROLE).toInt();

	switch (nType)
	{
	case PROJECT_TYPE:
	{
		szName = tr("ProjOverView");

		if (IsFileOpen(szName))
		{
			return;
		}

		m_nDataType = PROJECT_TYPE;

		QTableView* pView = new QTableView(this);

		pView->setAlternatingRowColors(true);

		CDataModel* projModel = new CDataModel(m_pMemDB, pView);

		m_VecDataModel.push_back(projModel);

		m_VecDataView.push_back(pView);

		ShowData(projModel, 0);

		int nIndex = m_pTabWidget->addTab(pView, szName);

		m_pTabWidget->setCurrentIndex(nIndex);

		pView->setModel(projModel);
	}
	break;
	case CHANNEL_GERENAL_TYPE:
	{
		m_nDataType = CHANNEL_GERENAL_TYPE;

		CChannel* pChannel = reinterpret_cast<CChannel*>(item->data(0, DataDef_ROLE).toLongLong());
		if (pChannel)
		{

			int nOccNo = pChannel->GetOccNo();

			szName = tr("ChannelN0:%1").arg(nOccNo);

			if (IsFileOpen(szName))
			{
				return;
			}
			QTableView* pView = new QTableView(this);

			pView->setAlternatingRowColors(true);

			CDataModel* projModel = new CDataModel(m_pMemDB,pView);

			m_VecDataModel.push_back(projModel);

			m_VecDataView.push_back(pView);

			projModel->SetChannelOccNo(nOccNo);

			ShowData(projModel, nOccNo);

			int nIndex = m_pTabWidget->addTab(pView, szName);

			m_pTabWidget->setCurrentIndex(nIndex);

			pView->setModel(projModel);
		}
	}
	break;
	case DEVICE_GERENAL_TYPE:
	{
		m_nDataType = DEVICE_GERENAL_TYPE;

		CDevice *pDev = reinterpret_cast<CDevice*>(item->data(0, DataDef_ROLE).toLongLong());
		if (pDev)
		{
			int nOccNo = pDev->GetOccNo();

			szName = tr("DeviceNo:%1").arg(nOccNo);

			if (IsFileOpen(szName))
			{
				return;
			}

			QTableView* pView = new QTableView(this);

			pView->setAlternatingRowColors(true);

			CDataModel* projModel = new CDataModel(m_pMemDB, pView);

			m_VecDataModel.push_back(projModel);

			m_VecDataView.push_back(pView);

			projModel->SetDeviceOccNo(nOccNo);

			ShowData(projModel, nOccNo);

			int nIndex = m_pTabWidget->addTab(pView, szName);

			m_pTabWidget->setCurrentIndex(nIndex);

			pView->setModel(projModel);

		}
	}
	break;
	case AIN_TYPE:
	{
		m_nDataType = AIN_TYPE;

		QString szName;

		int32u nDevice = item->data(0, DataDevOcc_ROLE).toUInt();

		szName = tr("AIN,DeviceNo:%1").arg(nDevice);

		if (IsFileOpen(szName))
		{
			return;
		}

		QTableView* pView = new QTableView(this);

		pView->setAlternatingRowColors(true);

		CDataModel* projModel = new CDataModel(m_pMemDB, pView);

		m_VecDataModel.push_back(projModel);

		m_VecDataView.push_back(pView);

		projModel->SetDeviceOccNo(nDevice);

		projModel->SetDataType(AIN_TYPE);

		ShowData(projModel, nDevice);

		int nIndex = m_pTabWidget->addTab(pView, szName);

		m_pTabWidget->setCurrentIndex(nIndex);

		pView->setModel(projModel);

		break;
	}
	case DIN_TYPE:
	{
		m_nDataType = DIN_TYPE;

		QString szName;

		int32u nDevice = item->data(0, DataDevOcc_ROLE).toUInt();

		szName = tr("DIN,DeviceNo:%1").arg(nDevice);

		if (IsFileOpen(szName))
		{
			return;
		}
		QTableView* pView = new QTableView(this);

		pView->setAlternatingRowColors(true);

		CDataModel* projModel = new CDataModel(m_pMemDB, pView);

		m_VecDataModel.push_back(projModel);

		m_VecDataView.push_back(pView);

		projModel->SetDeviceOccNo(nDevice);

		projModel->SetDataType(DIN_TYPE);

		ShowData(projModel, nDevice);

		int nIndex = m_pTabWidget->addTab(pView, szName);

		m_pTabWidget->setCurrentIndex(nIndex);

		pView->setModel(projModel);

		break;
	}
	case AOUT_TYPE:
	{
		m_nDataType = AOUT_TYPE;

		QString szName;

		int32u nDevice = item->data(0, DataDevOcc_ROLE).toUInt();

		szName = tr("AOUT,DeviceNo:%1").arg(nDevice);

		if (IsFileOpen(szName))
		{
			return;
		}
		QTableView* pView = new QTableView(this);

		QObject::connect(pView, &QTableView::doubleClicked, this, &CDataView::slot_OnAODbClicked);

		pView->setAlternatingRowColors(true);

		CDataModel* projModel = new CDataModel(m_pMemDB, pView);

		m_VecDataModel.push_back(projModel);

		m_VecDataView.push_back(pView);

		projModel->SetDeviceOccNo(nDevice);

		projModel->SetDataType(AOUT_TYPE);

		ShowData(projModel, nDevice);

		int nIndex = m_pTabWidget->addTab(pView, szName);

		m_pTabWidget->setCurrentIndex(nIndex);

		pView->setModel(projModel);

		break;
	}
	case DOUT_TYPE:
	{
		m_nDataType = DOUT_TYPE;

		QString szName;

		int32u nDevice = item->data(0, DataDevOcc_ROLE).toUInt();

		szName = tr("DOUT,DeviceNo:%1").arg(nDevice);

		if (IsFileOpen(szName))
		{
			return;
		}
		QTableView* pView = new QTableView(this);

		QObject::connect(pView,&QTableView::doubleClicked,this,&CDataView::slot_on_tabDBClieked);

		CDataModel* projModel = new CDataModel(m_pMemDB, pView);

		pView->setAlternatingRowColors(true);

		m_VecDataModel.push_back(projModel);

		m_VecDataView.push_back(pView);

		projModel->SetDeviceOccNo(nDevice);

		projModel->SetDataType(DOUT_TYPE);

		ShowData(projModel, nDevice);

		int nIndex = m_pTabWidget->addTab(pView, szName);

		m_pTabWidget->setCurrentIndex(nIndex);

		pView->setModel(projModel);

		break;
	}
	default:
		break;
	}
}

/*! \fn void CDataView::ShowData()
*********************************************************************************************************
** \brief CDataView::ShowData
** \details 显示数据
** \return void
** \author xingzhibing
** \date 2016年9月20日
** \note
********************************************************************************************************/
void CDataView::ShowData(DataModel_PTR ptrData, int nOccNo)
{
	Q_ASSERT(ptrData);
	//设置表头
	SetHeaderData(ptrData);
	//设置数据
	SetGridData(ptrData, nOccNo);
	//设置类型
	ptrData->SetDataType(m_nDataType);

}

/*! \fn void CDataView::SetHeaderData()
*********************************************************************************************************
** \brief CDataModel::SetHeaderData
** \details 按照结构设置表格标题数据显示
** \return void
** \author xingzhibing
** \date 2016年9月20日
** \note
********************************************************************************************************/
void CDataView::SetHeaderData(DataModel_PTR ptrData)
{
	Q_ASSERT(ptrData);

	m_szHeaderList.clear();

	switch (m_nDataType)
	{
	case PROJECT_TYPE:
	case CHANNEL_GERENAL_TYPE:
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
	case DEVICE_GERENAL_TYPE:
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
	case AIN_TYPE:
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
	case DIN_TYPE:
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
	case AOUT_TYPE:
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
	case DOUT_TYPE:
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
		break;
	}

	ptrData->SetHeaderList(m_szHeaderList);
}

/*! \fn void CDataView::SetGridData()
*********************************************************************************************************
** \brief CDataView::SetGridData
** \details 表格数据内容
** \return void
** \author xingzhibing
** \date 2016年9月20日
** \note
********************************************************************************************************/
void CDataView::SetGridData(DataModel_PTR ptrData, int nOccNo)
{
	Q_ASSERT(ptrData);

	switch (m_nDataType)
	{
	case PROJECT_TYPE:              //工程信息 显示所有通道信息
	{
		ptrData->SetRowCount(m_pMemDB->GetChannelMgr()->GetChannelCount());
		ptrData->SetColumnCount(m_szHeaderList.size());
	}
	break;
	case CHANNEL_GERENAL_TYPE:      //通道信息显示当下装置和点信息
	{
		ptrData->SetRowCount(m_szHeaderList.size());
		ptrData->SetColumnCount(1);
		break;
	}
	case DEVICE_GERENAL_TYPE:       //装置信息页显示当下所有点信息
	{
		ptrData->SetRowCount(m_szHeaderList.size());
		ptrData->SetColumnCount(1);
		break;
	}
	case AIN_TYPE:
	{
		CDevicePtr pDev = m_pMemDB->GetChannelMgr()->GetDevice(nOccNo - 1);
		Q_ASSERT(pDev);
		if (pDev)
		{
			ptrData->SetRowCount(pDev->GetAinCount());
		}
		ptrData->SetColumnCount(m_szHeaderList.size());
		break;
	}
	case DIN_TYPE:
	{
		CDevicePtr pDev = m_pMemDB->GetChannelMgr()->GetDevice(nOccNo - 1);
		Q_ASSERT(pDev);
		if (pDev)
		{
			ptrData->SetRowCount(pDev->GetDinCount());
		}
		ptrData->SetColumnCount(m_szHeaderList.size());
		break;
	}
	case AOUT_TYPE:
	{
		CDevicePtr pDev = m_pMemDB->GetChannelMgr()->GetDevice(nOccNo - 1);
		Q_ASSERT(pDev);
		if (pDev)
		{
			ptrData->SetRowCount(pDev->GetAoutCount());
		}
		ptrData->SetColumnCount(m_szHeaderList.size());
		break;
	}
	case DOUT_TYPE:
	{
		CDevicePtr pDev = m_pMemDB->GetChannelMgr()->GetDevice(nOccNo - 1);
		Q_ASSERT(pDev);
		if (pDev)
		{
			ptrData->SetRowCount(pDev->GetDoutCount());
		}
		ptrData->SetColumnCount(m_szHeaderList.size());
		break;
	}
	default:
		break;
	}
}

/*! \fn void CDataView::slot_on_RemoveTab(int nIndex)
*********************************************************************************************************
** \brief CDataView::slot_on_RemoveTab
** \details 移除子标签
** \param nIndex
** \return void
** \author xingzhibing
** \date 2016年9月21日
** \note
********************************************************************************************************/
void CDataView::slot_on_RemoveTab(int nIndex)
{
	if (nIndex < m_pTabWidget->count())
	{
		QTableView* pView = reinterpret_cast<QTableView*>( m_pTabWidget->widget(nIndex));
		if (pView)
		{
			std::vector<DataView_PTR>::iterator it = m_VecDataView.begin();
		}

		m_pTabWidget->removeTab(nIndex);
	}
}

/*! \fn bool CDataView::IsFileOpen(const QString& szName)
*********************************************************************************************************
** \brief CDataView::IsFileOpen
** \details 判断文件是否打开
** \param szName
** \return bool 返回false 则次文件未打开, 反之 则文件已经打开
** \author xingzhibing
** \date 2016年9月21日
** \note
********************************************************************************************************/
bool CDataView::IsFileOpen(const QString& szName)
{
	if (m_pTabWidget->count() <= 0)
	{
		return false;
	}
	for (int i = 0; i < m_pTabWidget->count(); ++i)
	{
		if (m_pTabWidget->tabText(i) == szName)
		{
			m_pTabWidget->setCurrentIndex(i);
			return true;
		}
	}
	return false;
}

bool CDataView::IsDataViewOpen(const QString& szName)
{
	if (m_pTabDataInfoWidget->count() <= 0)
	{
		return false;
	}
	for (int i = 0; i < m_pTabDataInfoWidget->count(); ++i)
	{
		if (m_pTabDataInfoWidget->tabText(i) == szName)
		{
			return true;
		}
	}
	return false;
}

/*! \fn void CDataView::slot_on_Timer()
********************************************************************************************************* 
** \brief CDataView::slot_on_Timer 
** \details 定时更新当前页面数据
** \return void 
** \author xingzhibing
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
void CDataView::slot_on_Timer()
{
	QTableView* pView = reinterpret_cast<QTableView*>(m_pTabWidget->currentWidget());

	if (pView)
	{
		pView->reset();
	}
}

void CDataView::slot_on_tabDBClieked(const QModelIndex& index)
{
	Q_ASSERT(index.isValid());

	if (!index.isValid())
	{
		return;
	}

	
	QTableView* pView = reinterpret_cast<QTableView*>(m_pTabWidget->currentWidget());

	QModelIndex nIndex = pView->model()->index(index.row(),0);

	QVariant data = pView->model()->data(nIndex);

	CDoutOperDialog *m_dialog=new CDoutOperDialog(NULL,data.toUInt(),m_pMemDB);

	m_dialog->setModal(false);

	m_dialog->exec();
}

void CDataView::slot_OnAODbClicked(const QModelIndex& index)
{
	Q_ASSERT(index.isValid());

	if (!index.isValid())
	{
		return;
	}

	QTableView* pView = reinterpret_cast<QTableView*>(m_pTabWidget->currentWidget());

	QModelIndex nIndex = pView->model()->index(index.row(), 0);

	QVariant data = pView->model()->data(nIndex);

	CAoutOperDialog *m_dialog = new CAoutOperDialog(NULL, data.toUInt(), m_pMemDB);

	m_dialog->setModal(false);

	m_dialog->exec();
}

void CDataView::closeEvent(QCloseEvent *e)
{
	alarm_stop  fnFesAlarm = (alarm_stop)m_LibFes.resolve("StopAlarmWin");
	if (fnFesAlarm)
	{
		fnFesAlarm();
	}

	LogMsg("dataview", QObject::tr("data module stopped").toStdString().c_str(), 0, nullptr);

	QDialog::closeEvent(e);
}