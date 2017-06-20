#include "commthread.h"
#include "commplugin.h"
#include <QDateTime>

CCommThread::CCommThread(QObject *parent)
	: QThread(parent)
{
    m_pModule = qobject_cast<CFtuModule*>(parent);

    Q_ASSERT(m_pModule);
    if (m_pModule == nullptr)
    {
        return;
    }

	qRegisterMetaType<DEV_BASE>("DEV_BASE&");
	qRegisterMetaType<QMap<int, short>>("QMap<int,short>");
    qRegisterMetaType<QList<LB_DATA>>("QList<LB_DATA>&");
    qRegisterMetaType<ZOOM_BASE>("ZOOM_BASE&");
	qRegisterMetaType<FILE_CATALOG_REQUEST_1>("FILE_CATALOG_REQUEST_1&");
	qRegisterMetaType<FILE_ATTR_INFO>("FILE_ATTR_INFO&");
	qRegisterMetaType<QList<Catalog_Info>>("QList<Catalog_Info>&");
	qRegisterMetaType<ASDUGZ>("ASDUGZ");
}

CCommThread::~CCommThread()
{

}

//开启线程
void CCommThread::run()
{
    CCommPlugin commPlugin(m_pModule);
    commPlugin.SetGeneralTime(m_nGeneranTime, m_nSyncGeneralTime, m_nKwhCallTime);
	commPlugin.StartRun(m_strIP, m_iPort);

	//发出信号
	connect(&commPlugin, SIGNAL(Signal_SocketError(QString)), this, SIGNAL(Signal_SocketError(QString)));
	//连接成功
	connect(&commPlugin, SIGNAL(Signal_SocketConnectSuccess(QString)), this, SLOT(Slot_ConnectSuccess(QString)));
	//总召唤确认
	connect(&commPlugin, SIGNAL(Signal_AllCallRespond()), this, SIGNAL(Signal_AllCallRespond()));
	connect(&commPlugin, SIGNAL(Signal_onePointDisRemote(int, int, int)), this, SIGNAL(Signal_onePointDisRemote(int, int, int)));
	connect(&commPlugin, SIGNAL(Signal_OnePointRemote(int, int, int)), this, SIGNAL(Signal_OnePointRemote(int, int, int)));

	connect(&commPlugin, SIGNAL(Signal_DoublePointRemote(int, int, int)), this, SIGNAL(Signal_DoublePointRemote(int, int, int)));
	connect(&commPlugin, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), this, SIGNAL(Signal_DoublePointDisRemote(int, int, int)));

	connect(&commPlugin, SIGNAL(Signal_BinaryGroupPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupPoint(int, int, int)));
	connect(&commPlugin, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)));


	//SOE
    connect(&commPlugin, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)));
    connect(&commPlugin, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)), this, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)));
    connect(&commPlugin, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)));
    connect(&commPlugin, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)));

	//kwh
	connect(&commPlugin, SIGNAL(Signal_KwhNormal(int, int, int)), this, SIGNAL(Signal_KwhNormal(int, int, int)));
	connect(&commPlugin, SIGNAL(Signal_KwhDisNormal(int, int, int)), this, SIGNAL(Signal_KwhDisNormal(int, int, int)));

	connect(&commPlugin, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));
	connect(&commPlugin, SIGNAL(Signal_KwhLongTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));

	//遥测
	connect(&commPlugin, SIGNAL(Signal_AnalogNormal(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(&commPlugin, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));

	connect(&commPlugin, SIGNAL(Signal_AnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(&commPlugin, SIGNAL(Signal_DisAnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));
	connect(&commPlugin, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)), this, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)));
    //故障
    connect(&commPlugin, SIGNAL(Signal_MalFuction(ASDUGZ)), this, SIGNAL(Signal_MalFuction(ASDUGZ)));

	//发送和接收到的数据
	connect(&commPlugin, SIGNAL(Signal_Write16Data(QByteArray)), this, SIGNAL(Signal_Write16Data(QByteArray)));
    connect(&commPlugin, SIGNAL(Signal_recv16Data(QByteArray, int)), this, SIGNAL(Signal_recv16Data(QByteArray, int)));
	//遥控指令
	connect(this, SIGNAL(Signal_Control(int, int, int,int)), &commPlugin, SLOT(Slot_SetControlCommand(int, int, int,int)));
	//定值指令
	connect(this, SIGNAL(Signal_DevSendInfo(DEV_BASE &)), &commPlugin, SLOT(Slot_setDevOrder(DEV_BASE &)));
    //死区设置
    connect(this, SIGNAL(Signal_ZoomArea(ZOOM_BASE &)), &commPlugin, SLOT(Slot_SetZoomArea(ZOOM_BASE &)));

	//遥控反馈
	connect(&commPlugin, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)), this, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)));
	//定值
	connect(&commPlugin, SIGNAL(Signal_DevReadBack(QMap<int, short>)), this, SIGNAL(Signal_DevReadBack(QMap<int, short>)));
    //死区
    connect(&commPlugin, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)));
	//定值
	connect(&commPlugin, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)));

	//定值设定
	connect(&commPlugin, SIGNAL(Signal_devWriteBack(int, int, int)), this, SIGNAL(Signal_devWriteBack(int, int, int)));
    //录波文件获取
    connect(this, SIGNAL(Signal_RecordSendInfo(QList<LB_DATA> &)), &commPlugin, SLOT(Slot_SetRecordOrder(QList<LB_DATA> &)));
    //手动总召
    connect(this, SIGNAL(Signal_GeneralCall()), &commPlugin, SLOT(Slot_SendGeneralResquestMsg()));
    //电度召唤
    connect(this, SIGNAL(Signal_KwhCall()), &commPlugin, SLOT(Slot_SendKwhTimeRequestMsg()));
    //对时
    connect(this, SIGNAL(Signal_SycsTime(QDateTime)), &commPlugin, SLOT(Slot_SetSycsTime(QDateTime)));
    //复位进程
    connect(this, SIGNAL(Signal_ResetProcess()), &commPlugin, SLOT(Slot_ResetProcess()));

	//切换定制区
	connect(this,SIGNAL(Signal_SwitchFixArea(unsigned short)),&commPlugin,SLOT(Slot_SwitchFixArea(unsigned short)));
	//读取当前定值区号
	connect(this,SIGNAL(Signal_ReadFixArea()), &commPlugin, SLOT(Slot_ReadCurrentFixArea()));

	//目录召唤
	connect(this,SIGNAL(Signal_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &)),&commPlugin,SLOT(Slot_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &)));

	//写入激活
	connect(this,SIGNAL(SIGNAL_WriteActionRequest(FILE_ATTR_INFO &)),&commPlugin,SLOT(Slot_WriteAction(FILE_ATTR_INFO &)));
	//读取激活
	connect(this, SIGNAL(Signal_ReadActionRequest(FILE_ATTR_INFO &)), &commPlugin, SLOT(Slot_ReadAction(FILE_ATTR_INFO &)));
	//定值读取
	connect(&commPlugin,SIGNAL(Signal_ReadFixData(DEV_BASE &)),this,SIGNAL(Signal_ReadFixData(DEV_BASE &)));
	//文件目录
	connect(commPlugin.GetRecver(),SIGNAL(Signal_FIleCatalogINfo(QList<Catalog_Info>&)),this,SIGNAL(Signal_FIleCatalogINfo(QList<Catalog_Info>&)));

	this->exec();
}

//获取发送方信息
QString CCommThread::GetOwnNetInfo()
{
	return m_LocalInfo;
}
//获取对方信息
QString CCommThread::GetOtherInfo()
{
	return m_strIP + ":" + QString::number(m_iPort);
}

void CCommThread::Slot_ConnectSuccess(QString strLocalInfo)
{
	m_LocalInfo = strLocalInfo;
	emit Signal_SocketConnectSuccess();
}



//发送遥控命令接口
void CCommThread::SendRemoteControl(int iControlType, int iPointNum, int iOpType,int iMessType)
{
	emit Signal_Control(iControlType, iPointNum, iOpType,iMessType);
}

//发送定值指令
void CCommThread::SendDevOrder(DEV_BASE &dveBaseInfo)
{
	emit Signal_DevSendInfo(dveBaseInfo);
}

//死区设置
void CCommThread::SetZoomArea(ZOOM_BASE &zoomBaseInfo)
{
    emit Signal_ZoomArea(zoomBaseInfo);
}


void CCommThread::SetFixAreaCall(unsigned short uFixArea)
{
	emit Signal_SwitchFixArea(uFixArea);
}

void CCommThread::ReadFixArea()
{
	emit Signal_ReadFixArea();
}

void CCommThread::SendCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest)
{
	emit Signal_ReadCatalogRequest(catalogRequest);
}

//文件写入激活
void CCommThread::SendWriteActionRequest(FILE_ATTR_INFO &writeAction)
{
	emit SIGNAL_WriteActionRequest(writeAction);;
}

void CCommThread::SendReadActionRequest(FILE_ATTR_INFO &ReadAction)
{
	emit Signal_ReadActionRequest(ReadAction);
}

//开始运行
void CCommThread::StartRun(const char* strIP, int iPort)
{
	if (this->isRunning())
	{
		this->quit();
		this->wait();
	}
	m_strIP = strIP;
	m_iPort = iPort;
	this->start();
	
}
//

void CCommThread::SendRecordOrder(QList<LB_DATA> &lbInfo)
{
    emit Signal_RecordSendInfo(lbInfo);
}

void CCommThread::SetGeneralTimes(int nGeneralTime, int nSyncTime, int nKwhTime)
{
    m_nGeneranTime = nGeneralTime;
    m_nSyncGeneralTime = nSyncTime;
    m_nKwhCallTime = nKwhTime;
}

void CCommThread::SetGeneralCall()
{
    emit Signal_GeneralCall();
}

//手动电度召唤
void CCommThread::SetKwhCall()
{
    emit Signal_KwhCall();
}

