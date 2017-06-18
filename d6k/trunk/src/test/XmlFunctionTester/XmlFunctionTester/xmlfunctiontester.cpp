#include "xmlfunctiontester.h"
#include <QDateTime>
XmlFunctionTester::XmlFunctionTester(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	InitView();
	setFixedSize(this->size());
}
/*! \fn void XmlFunctionTester::InitView()
********************************************************************************************************* 
** \brief XmlFunctionTester::InitView 
** \details 测试界面初始化
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void XmlFunctionTester::InitView()
{	
	m_AinNum = 0;
	m_DInNum = 0;
	m_DevNum = 0;

	m_pXmlWriter = new CXmlWriter();
	m_pXmlReader = new CXmlReader();
	m_pDevMgr = std::make_shared<CDeviceMgr>();
	
	QObject::connect(ui.m_btnCreDecModule, SIGNAL(clicked()), this, SLOT(slot_OnCreateDeviceModule()));
	QObject::connect(ui.m_btnCreFile, SIGNAL(clicked()), this, SLOT(slot_OnCreateFileInfo()));
	QObject::connect(ui.m_btnLoadFIle, SIGNAL(clicked()), this, SLOT(slot_OnLoadFileInfo()));
	QObject::connect(m_pXmlReader, SIGNAL(sig_ProcessInfo(const QString&)), this, SLOT(slot_OnLoadProcessView(const QString& )));
	QObject::connect(m_pXmlWriter, SIGNAL(sig_ProcessInfo(const QString&)), this, SLOT(slot_OnCreateProcessView(const QString&)));
}

XmlFunctionTester::~XmlFunctionTester()
{

}

/*! \fn void XmlFunctionTester::slot_OnCreateDeviceModule()
********************************************************************************************************* 
** \brief XmlFunctionTester::slot_OnCreateDeviceModule 
** \details 创建基本设备模型
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void XmlFunctionTester::slot_OnCreateDeviceModule()
{
	 m_AinNum = ui.m_editAinNum->text().toInt();
	 m_DInNum = ui.m_editDinNUm->text().toInt();
	 m_DevNum = ui.m_editDevNum->text().toInt();

	 int nPointNum = m_DevNum*(m_AinNum + m_DInNum);

	 ui.m_editPointNum->setText(QString::number(nPointNum));
}

/*! \fn void XmlFunctionTester::slot_OnCreateFileInfo()
********************************************************************************************************* 
** \brief XmlFunctionTester::slot_OnCreateFileInfo 
** \details 创建配置文件基本信息
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void XmlFunctionTester::slot_OnCreateFileInfo()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);

	CDevice* pDev = Q_NULLPTR;
	AIN* pAIN = Q_NULLPTR;
	DIN* pDIN = Q_NULLPTR;

	//内存文件
	static int nOccNum = 1;

	for (int i = 0; i < m_DevNum;++i)
	{
		pDev = new CDevice;

		pDev->m_OccNo = i;
		pDev->m_szDevName = tr("dev%1").arg(i);

		//AIN
		for (int j = 0; j < m_AinNum; ++j)
		{
			pAIN = new AIN;

			pAIN->m_OccNo = nOccNum;
			pAIN->m_Id = j;
			pAIN->m_ChannelOccNo = i;
			pAIN->m_BlockOccNo = i;
			pAIN->m_Address = i;
			pAIN->m_DataSource = j;
			pAIN->m_InitVal = 0.0;
			pAIN->m_InitQua = 0.0;
			pAIN->m_szTagName = "1####";
			pAIN->m_szDescription = "tester";
			pAIN->m_TransFromType = i;
			pAIN->m_szAlarmTagName = "testAlarm";
			pAIN->m_szAlarmDesc = "testAlarm";
			pAIN->m_RangeH = 100;
			pAIN->m_RangeL = 1.0;
			pAIN->m_HighQua = 1000;
			pAIN->m_LowQua = 1.0;
			pAIN->m_szUnitName = "testUint";
			pAIN->m_szExpress = "testExpress";
			pAIN->m_PinLanel = 1;
			pAIN->m_DataType = i;
			pAIN->m_SaveDisk = i;
			pAIN->m_SaveDiskPeriod = i;
			pAIN->m_SaveDb = j;
			pAIN->m_SaveDbPeriod = j;
			pAIN->m_Sensitivity = i*j;
			pAIN->m_ReferenceCount = i*i;

			nOccNum++;
			pDev->AddAInInfo(pAIN);
		}
		//DIN
		for (int k = 0; k < m_DInNum;++k)
		{
			pDIN = new DIN;
			pDIN->m_OccNo = nOccNum;
			pDIN->m_Id = k;
			pDIN->m_ChannelOccNo = i;
			pDIN->m_DeviceOccNo = i;
			pDIN->m_Address = k;
			pDIN->m_InitQua = k;
			pDIN->m_InitVal = k;
			pDIN->m_szTagName = "testdin";
			pDIN->m_szTransFormDesc = "testdesc";
			pDIN->m_AlarmType = k;
			pDIN->m_szAlarmTagName = "alarmtagname";
			pDIN->m_szAlarmDesc = "alarmdesc";
			pDIN->m_szExpress = "testExpress";
			pDIN->m_PinLabel = k;
			pDIN->m_bIsSBO = true;
			pDIN->m_TelCtrlSendTime = 1000;
			pDIN->m_TelCtrlWaitTime = 1000;
			pDIN->m_ReferenceCount = 100;
			
			nOccNum++;
			pDev->AddDINInfo(pDIN);
		}		

		m_pDevMgr->AddDevice(pDev);
	}

	//文件存储
	m_pXmlWriter->SetFileInfo(m_pDevMgr);

	int nLen = sizeof(m_pDevMgr);

	m_pXmlWriter->WriteFile("test.xml");

	QApplication::restoreOverrideCursor();
}

/*! \fn void XmlFunctionTester::slot_OnLoadFileInfo()
********************************************************************************************************* 
** \brief XmlFunctionTester::slot_OnLoadFileInfo 
** \details 读取配置文件基本信息
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void XmlFunctionTester::slot_OnLoadFileInfo()
{
	/*QApplication::setOverrideCursor(Qt::WaitCursor);
	if (m_pDevMgr)
	{
		m_pDevMgr.reset();
		m_pDevMgr = std::make_shared<CDeviceMgr>();
	}
	m_pXmlReader->SetFileInfo(m_pDevMgr);
	m_pXmlReader->ReadFileInfo("test.xml");*/


	slot_onRealAllInfo();

//	QApplication::restoreOverrideCursor();
}


/*! \fn void XmlFunctionTester::slot_OnCreateProcessView(const QString& str)
********************************************************************************************************* 
** \brief XmlFunctionTester::slot_OnCreateProcessView 
** \details 创建文件过程中信息显示到界面
** \param str 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void XmlFunctionTester::slot_OnCreateProcessView(const QString& str)
{
	ui.textBrowser->append(str);
}

/*! \fn void XmlFunctionTester::slot_OnLoadProcessView(const QString& str)
********************************************************************************************************* 
** \brief XmlFunctionTester::slot_OnLoadProcessView 
** \details 加载进程信息显示到界面
** \param str 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void XmlFunctionTester::slot_OnLoadProcessView(const QString& str)
{
	ui.textBrowser_2->append(str);
}

void XmlFunctionTester::slot_onRealAllInfo()
{
	uint beginTime = QDateTime::currentDateTime().toTime_t();
	ui.textBrowser_2->append(QString("Begin:%1").arg(beginTime));

	
	int nCount = m_pDevMgr->GetDevNum();
	for (int i = 0; i < nCount;++i)
	{
		CDevice *pDev = m_pDevMgr->GetDev(i);

		int nAinCount = pDev->GetAINCount();
		int nDinCount = pDev->GetDINCount();
		for (int j = 0; j < nAinCount;++j)
		{
			AIN* pAin = pDev->GetAin(i);
			pAin->m_InitVal = 1.0;
		}

		for (int k = 0; k < nDinCount;++k)
		{
			DIN* pDin = pDev->GetDin(k);
			pDin->m_InitVal = 0;
		}
		uint endTime = QDateTime::currentDateTime().toTime_t();
		ui.textBrowser_2->append(QString("End:%1").arg(endTime));
	}


	char* pData = new char[1000*1000];
	

	for (unsigned int i = 0; i < 1000 * 1000;i++)
	{
		pData[i] = i;
	}
	uint endTime = QDateTime::currentDateTime().toTime_t();
	ui.textBrowser_2->append(QString("End1:%1").arg(endTime));

	uint nSends = endTime - beginTime;
	ui.textBrowser_2->append(QString("all time:%1").arg(nSends));
}
