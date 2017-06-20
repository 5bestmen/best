#include <QTimer>
#include <QDebug>
#include "commplugin.h"
#include "socketthread.h"
#include "asdu_info.h"
#include "define_104.h"
#include "apdusender.h"
#include "apdurecver.h"

// static CCommPlugin *s_ptrCommModule = nullptr;
// 
// extern "C" COMMPLUGINSHARED_EXPORT ICommPlugin *CreateModule()
// {
//     s_ptrCommModule = new CCommPlugin;
//     return s_ptrCommModule;
// }

/*********************************************************************************************************
** \brief CCommPlugin()
** \details 发送类构造函数
** \param argl   父类指针
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
CCommPlugin::CCommPlugin(CFtuModule *pModule)
{
	//qDebug() << "communicate Thread ID:" << QThread::currentThreadId();
    m_pModule = pModule;

	m_pTimerOut0 = new QTimer(this);
	m_pTimerOut1 = new QTimer(this);
	m_pTimerOut2 = new QTimer(this);
	m_pTimerOut3 = new QTimer(this);
	m_pTimeGeneralSendMsg = new QTimer(this);
    m_pTimerSyncTimeMsg = new QTimer(this);
    m_pTimerKwhMsg = new QTimer(this);

    m_k  = 0;
    m_w = 0;
    m_bIsRunning = false;
    m_nRecvNum = 0;
    m_nSendNum = 0;

    m_pApduSender = new CApduSender(this);
    m_pApduRecver = new CApduRecver(this);


    m_pSocketThread =new CSocketThread;
	m_pSocketThread->setApduRecver(m_pApduRecver);

	//m_pSocketThread->start();
    //m_pSocketThread->moveToThread(&m_thrdSocket);
    //定时获取数据 进行处理

    //connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Slot_OnRecvData()));

    connect(m_pSocketThread,SIGNAL(Signal_SocketError(QString)),this,SLOT(Slot_SocketError(QString)));
    //连接成功
	connect(m_pSocketThread, SIGNAL(Signal_ConnectSuccess(QString)), this, SLOT(Slot_sockeConnectSuccess(QString)));
    //总召唤确认
    connect(m_pApduRecver,SIGNAL(Signal_AllCallRespond()),this,SIGNAL(Signal_AllCallRespond()));
    connect(m_pApduRecver,SIGNAL(Signal_onePointDisRemote(int,int,int)),this,SIGNAL(Signal_onePointDisRemote(int,int,int)));
    connect(m_pApduRecver,SIGNAL(Signal_OnePointRemote(int,int,int)),this,SIGNAL(Signal_OnePointRemote(int,int,int)));

	connect(m_pApduRecver, SIGNAL(Signal_DoublePointRemote(int, int, int)), this, SIGNAL(Signal_DoublePointRemote(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), this, SIGNAL(Signal_DoublePointDisRemote(int, int, int)));

	connect(m_pApduRecver, SIGNAL(Signal_BinaryGroupPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupPoint(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)), this, SIGNAL(Signal_BinaryGroupDisPoint(int, int, int)));


	//SOE
    connect(m_pApduRecver, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinarySignalPointShortTime(int, int, int, int, QString)));
    connect(m_pApduRecver, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePointShortTime(int, int, int, int, QString)));
    connect(m_pApduRecver, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)), this, SIGNAL(Signal_SoeBinarySinglePointTime(int, int, int, int, QString)));
    connect(m_pApduRecver, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)), this, SIGNAL(Signal_BinaryDoublePoint(int, int, int, int, QString)));

	//kwh
	connect(m_pApduRecver, SIGNAL(Signal_KwhNormal(int, int, int)), this, SIGNAL(Signal_KwhNormal(int, int, int)));
	connect(m_pApduRecver, SIGNAL(Signal_KwhDisNormal(int, int, int)), this, SIGNAL(Signal_KwhDisNormal(int, int, int)));

	connect(m_pApduRecver, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));
	connect(m_pApduRecver, SIGNAL(Signal_KwhLongTimeNormal(int, int, float)), this, SIGNAL(Signal_KwhShortTimeNormal(int, int, float)));

	//遥测
	connect(m_pApduRecver, SIGNAL(Signal_AnalogNormal(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(m_pApduRecver, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));

	connect(m_pApduRecver, SIGNAL(Signal_AnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_AnalogNormal(int, int, float, int)));
	connect(m_pApduRecver, SIGNAL(Signal_DisAnalogShortFloat(int, int, float, int)), this, SIGNAL(Signal_ScaledAnalogNormal(int, int, float, int)));

	connect(m_pApduRecver, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)), this, SIGNAL(Signal_AnalogNormalShortTime(int, int, float, int)));
	//遥控
	connect(m_pApduRecver, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)), this, SIGNAL(Signal_ControlFeedBack(int, int, int, QString)));

	//发送的数据
	connect(m_pSocketThread, SIGNAL(Signal_Write16Data(QByteArray)), this, SIGNAL(Signal_Write16Data(QByteArray)));
	//接收的数据
    connect(m_pApduRecver, SIGNAL(Signal_recv16Data(QByteArray, int)), this, SIGNAL(Signal_recv16Data(QByteArray, int)));
	//定值
	connect(m_pApduRecver, SIGNAL(Signal_DevReadBack(QMap<int, short>)), this, SIGNAL(Signal_DevReadBack(QMap<int, short>)));
	//定值设定
	connect(m_pApduRecver, SIGNAL(Signal_devWriteBack(int, int, int)), this, SIGNAL(Signal_devWriteBack(int, int, int)));
    //故障
    connect(m_pApduRecver, SIGNAL(Signal_MalFuction(ASDUGZ)), this, SIGNAL(Signal_MalFuction(ASDUGZ)));
    //死区
    connect(m_pApduRecver, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)), this, SIGNAL(Signal_ZoneFeedBack(int, float, int, int)));
	//定值读取
	connect(m_pApduRecver,SIGNAL(Signal_ReadFixData(DEV_BASE &)),this,SIGNAL(Signal_ReadFixData(DEV_BASE &)));
	//文件目录

	//定时获取数据
	//m_pTimer->setInterval(1);
	//tcp断连后不断重连
	m_pTimerOut0->setInterval(2000);

    //m_thrdSocket.start();
	//三个超时设置
	//t0超时
	connect(m_pTimerOut0, SIGNAL(timeout()), this, SLOT(Slot_TimeOutT0()));
	//t1超时
	connect(m_pTimerOut1, SIGNAL(timeout()), this, SLOT(Slot_TimeOutT1()));
	//t2超时
	connect(m_pTimerOut2, SIGNAL(timeout()), this, SLOT(Slot_TimeOutT2()));
	//t3超时
	connect(m_pTimerOut3, SIGNAL(timeout()), this, SLOT(Slot_timeOutT3()));
	//定时发送总召和时间同步
	connect(m_pTimeGeneralSendMsg, SIGNAL(timeout()), this, SLOT(Slot_SendGeneralResquestMsg()));
    connect(m_pTimerSyncTimeMsg, SIGNAL(timeout()), this, SLOT(Slot_SendSyncRequestMsg()));
    connect(m_pTimerKwhMsg, SIGNAL(timeout()), this, SLOT(Slot_SendKwhTimeRequestMsg()));

	m_pTimerOut1->setInterval(TIMER_INTERVAL_T1);
	m_pTimerOut2->setInterval(TIMER_INTERVAL_T2);
	m_pTimerOut3->setInterval(TIMER_INTERVAL_T3);
// 	m_pTimeGeneralSendMsg->setInterval(TIMER_INTERVAL_TG);
//     m_pTimerSyncTimeMsg->setInterval(TIMER_INTERVAL_TG);

// 	this->moveToThread(&m_tdRunThread);
// 	m_tdRunThread.start();
}

CCommPlugin::~CCommPlugin()
{
    m_pSocketThread->deleteLater();
	m_pTimerOut0->deleteLater();
	m_pTimerOut1->deleteLater();
	m_pTimerOut2->deleteLater();
	m_pTimerOut3->deleteLater();
	//delete m_pTimer;
}

/*********************************************************************************************************
** \brief RunStart(const QString &strIP, int iPort)
** \details 启动接收数据
** \param argl   IP地址
** \param arg2   端口号
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::StartRun(const QString &strIP, int iPort)
{
	//m_pTimer->start();
    m_pSocketThread->ConnectSocket(strIP,iPort);
	m_strIP = strIP;
	m_iPort = iPort;
}

/*********************************************************************************************************
** \brief StopRun()
** \details  关闭socket
** \param argl   IP地址
** \param arg2   端口号
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::StopRun()
{
	m_pSocketThread->CloseSocket();
	//关闭所有定时器
	m_pTimerOut1->stop();
	m_pTimerOut2->stop();
	m_pTimerOut3->stop();
	//m_pTimer->stop();

}

/*********************************************************************************************************
** \brief OnReceive(char *pBuff, int nLen)
** \details 接收网络数据
** \param argl   数据
** \param arg2   长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnReceive(char *pBuff, int nLen)
{
	
    if ((unsigned)nLen < sizeof(APCI))
    {
        return false;
    }
    m_pApduRecver->OnReceive(pBuff,nLen);
    return true;
}

/*********************************************************************************************************
** \brief WriteApduData(const char *pSendData, int iLength)
** \details 写入网络数据
** \param argl   数据
** \param arg2   长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::WriteApduData(const char *pSendData, int iLength)
{
    return m_pSocketThread->WriteSocketData(pSendData,iLength);
}

/*********************************************************************************************************
** \brief SendUData(int nType)
** \details 写入网络数据
** \param argl   U格式类型
** \param arg2   
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::SendUData(int nType)
{
    if (!IsRunning())
    {
        return false;
    }

    m_pApduSender->Send_U(nType);
    return true;
}

CApduSender *CCommPlugin::getSender()
{
    return m_pApduSender;
}

/*********************************************************************************************************
** \brief Slot_OnRecvData()
** \details 接收网络数据
** \param argl   U格式类型
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_OnRecvData()
{
    //处理接收到的数据

    QByteArray bRecvData = m_pSocketThread->GetNetworkData();
	
    if (!bRecvData.isEmpty())
    {
        OnReceive(bRecvData.data(), bRecvData.length());

    }

}

/*********************************************************************************************************
** \brief Slot_SocketError(QString error)
** \details socket错误
** \param argl   错误原因
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_SocketError(QString error)
{
	//清空数据
	m_k = 0;
	m_w = 0;
	m_bIsRunning = false;
	m_nRecvNum = 0;
	m_nSendNum = 0;
	//关闭定时器
	//关闭所有定时器
	m_pTimerOut1->stop();
	m_pTimerOut2->stop();
	m_pTimerOut3->stop();


	m_pTimerOut0->start();
    m_pSocketThread->CloseSocket();
	//关闭定时器  置0 
    emit Signal_SocketError(error);
	m_pTimeGeneralSendMsg->stop();
    m_pTimerSyncTimeMsg->stop();
    m_pTimerKwhMsg->stop();
}

/*********************************************************************************************************
** \brief Slot_sockeConnectSuccess()
** \details socket连接成功
** \param argl   
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_sockeConnectSuccess(QString strLocalInfo)
{
    //qDebug()<<"连接成功";
	m_pTimerOut0->stop();
    m_bIsRunning = true;
    //当连接成功时发送激活帧
    m_pApduSender->Send_U(STARTDT_ACT);
	emit Signal_SocketConnectSuccess(strLocalInfo);
	m_pTimeGeneralSendMsg->start();
    m_pTimerSyncTimeMsg->start();
    m_pTimerKwhMsg->start();
}

/*********************************************************************************************************
** \brief OnTimerSend(int nFrameType, int nUtype)
** \details 超时处理
** \param argl
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnTimerSend(int nFrameType, int nUtype)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		//发送方发送一个I帧报文时启动t1超时
		if (!m_pTimerOut1->isActive())
		{
			m_pTimerOut1->start();
		}
		//链路中只要有包就重新触发t3超时
		m_pTimerOut3->start();

	}
	break;
	case TYPE_S:
	{
		//发送S帧， 关闭T2超时
		m_pTimerOut2->stop();
		//链路中只要有包就重新触发t3超时
		m_pTimerOut3->start();
	}
	break;
	case TYPE_U:
	{
		//发送U帧，激活的时候需要t1超时
		if (nUtype == TESTFR_ACT || nUtype == STARTDT_ACT)//需要确认的
		{
			if (!m_pTimerOut1->isActive())
			{
				m_pTimerOut1->start();
			}
		}
		//链路中只要有包就重新触发t3超时
		m_pTimerOut3->start();
	}
	break;
	}

	return true;
}

/*********************************************************************************************************
** \brief OnTimerRecv(int nFrameType)
** \details 超时处理
** \param argl
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnTimerRecv(int nFrameType)
{
	switch (nFrameType)
	{
	case TYPE_I:
	{
		//当在t1时间内收到I帧数据
		m_pTimerOut1->stop();
		//重启t2
		m_pTimerOut2->start();
		//重启t3
		m_pTimerOut3->start();

	}
	break;
	case TYPE_S:
	{
		//重启t3
		m_pTimerOut3->start();
	}
	break;
	case TYPE_U:
	{
		//当在t1时间内收到U帧数据
		m_pTimerOut1->stop();
		//重启t3
		m_pTimerOut3->start();
	}
	break;
	}
	return true;
}

/*********************************************************************************************************
** \brief OnKwSend(int nFrameType)
** \details k w值设置
** \param argl  数据类型  
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnKwSend(int nFrameType)
{
    switch (nFrameType)
    {
    case TYPE_I:
    {
        m_k += 1;
        m_w = 0;
    }
        break;
    case TYPE_S:
    {
        m_w = 0;
    }
        break;
    default:
        break;
    }
    return true;
}

/*********************************************************************************************************
** \brief OnKwRecv(int nFrameType, int nRecvNum)
** \details k w值处理
** \param argl  数据类型 
** \param arg2  接收数据包数量
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnKwRecv(int nFrameType, int nRecvNum)
{
    switch(nFrameType)
    {
    case TYPE_I:
    {
        m_w += 1;
        if(nRecvNum > m_nSendNum)
        {
            m_k = 1;
        }
        else
        {
            m_k = m_nSendNum - nRecvNum;
        }
		//当收到的包 数量达到DEFAULT_MAX_W_VALUE时,发送S格式消息
		if (m_w >= DEFAULT_MAX_W_VALUE)
        {
            this->getSender()->Send_S();
        }
    }
        break;
    case TYPE_S:
    {
        if(nRecvNum > m_nSendNum)
        {
            m_k = 1;
        }
        else
        {
            m_k = m_nSendNum - nRecvNum;
        }
    }
        break;
    default:
        break;
    }
    return true;
}

/*********************************************************************************************************
** \brief OnNumRecv(int nFrameType, int nSendNum)
** \details 对接收到的数据的序列号进行处理 接收到的数据的序列号必须等于已经接收到的数据包的数量
** \param argl  数据类型
** \param arg2  接收数据包数量
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnNumRecv(int nFrameType, int nSendNum)
{
    switch(nFrameType)
    {
    case TYPE_I:
    {
        if(nSendNum != m_nRecvNum)//接收到的发送序号，不等于本地的接收序号
        {
            qDebug()<<"序列号出错"<<m_nSendNum<<":    :"<<nSendNum;
			this->getSender()->Send_S_BeforeClose(m_nRecvNum);
			//断开连接

			m_k = 0;
			m_w = 0;
			m_bIsRunning = false;
			m_nRecvNum = 0;
			m_nSendNum = 0;
			this->StopRun();
			QThread::msleep(500);
			this->StartRun(m_strIP, m_iPort);
            return false;
        }
		
        m_nRecvNum += 1;//本地接收序列号递增


        if(m_nRecvNum > MAX_SERIAL_NUMBER)
        {
            m_nRecvNum = 0;
        }
    }
        break;
    case TYPE_S:
        break;
    }
    return true;
}

/*********************************************************************************************************
** \brief OnNumSend(int nFrameType)
** \details 发送数据的序列号
** \param argl  数据类型
** \param arg2  
** \return void
** \author ww
** \date 2016年5月27日
** \note   数据包的编号是针对I格式数据来说得
********************************************************************************************************/
bool CCommPlugin::OnNumSend(int nFrameType)
{
    switch(nFrameType)
    {
    case TYPE_I:
    {
        m_nSendNum += 1;
        if(m_nSendNum > MAX_SERIAL_NUMBER) m_nSendNum = 0;
    }
        break;
    case TYPE_S:
        break;
    }
    return true;
}

/*********************************************************************************************************
** \brief OnSendFrame(int nFrameType, int nUtype)
** \details 发送数据准确性认证
** \param argl  数据类型
** \param arg2  
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnSendFrame(int nFrameType, int nUtype)
{
    //序列号
    if(!OnNumSend(nFrameType))
        return false;

    //KW值
    if(!OnKwSend(nFrameType))
        return false;

    //计时器
    if(!OnTimerSend(nFrameType,nUtype))
        return false;

    return true;
}

/*********************************************************************************************************
** \brief OnRecvFrame(int nFrameType, int nUtype)
** \details 接收数据准确性认证
** \param argl  数据类型
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnRecvFrame(int nFrameType, int nSendNum, int nRecvNum)
{

    //序列号
    if(!OnNumRecv(nFrameType,nSendNum))
    {
        return false;
    }

    //KW值
    if(!OnKwRecv(nFrameType,nRecvNum))
    {
        return false;
    }

    //计时器
    if(!OnTimerRecv(nFrameType))
    {
        return false;
    }

    return true;
}

/*********************************************************************************************************
** \brief OnCommand(NBM_TELECTRL* pTelectrl)
** \details 发送召唤信息
** \param argl  召唤结构
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CCommPlugin::OnCommand(NBM_TELECTRL* pTelectrl)
{
    pTelectrl->m_nDeviceID = m_pModule->GetDeviceAddr();
    return m_pApduSender->OnCommand(pTelectrl);
}

// 遥控命令
void CCommPlugin::SetControlBckCommand(int iControlType, int ipointNum, int OperateType)
{
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = m_pModule->GetDeviceAddr();
	telectrl.m_nCtrlType = TELECTRL_REQUEST_EXECUTE;

	telectrl.m_nDataID = ipointNum;
	if (OperateType == 0)
	{
		//合
		telectrl.m_fValue = 1;
	}
	else
	{
		//分
		telectrl.m_fValue = 0;
	}

	if (iControlType == 0)
	{
		//单点遥控
		telectrl.m_nCommandType = C_SC_NA_1;
	}
	else
	{
		//双点遥控  1
		telectrl.m_nCommandType = C_DC_NA_1;
	}

	bool IsOK = OnCommand(&telectrl);

	if (!IsOK)
	{
		return;
	}
}

// 遥控命令
/*********************************************************************************************************
** \brief Slot_SetControlCommand(int iControlType, int ipointNum, int OperateType)
** \details 遥控命令
** \param iControlType
** \param ipointNum
** \param OperateType
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_SetControlCommand(int iControlType, int ipointNum, int OperateType,int nMessType)
{
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = m_pModule->GetDeviceAddr();
    if (nMessType == 1)
    {
        //预置
        telectrl.m_nCtrlType = TELECTRL_REQUEST_SELECT;
    }
    else
    {
        //执行
        telectrl.m_nCtrlType = TELECTRL_REQUEST_EXECUTE;
    }
	

	telectrl.m_nDataID = ipointNum;
	if (OperateType == 0)
	{
		//合
		telectrl.m_fValue = 1;
	}
	else
	{
		//分
		telectrl.m_fValue = 0;
	}

	if (iControlType == 0)
	{
		//单点遥控
		telectrl.m_nCommandType = C_SC_NA_1;
	}
	else
	{
		//双点遥控  1
		telectrl.m_nCommandType = C_DC_NA_1;
	}

	bool IsOK = OnCommand(&telectrl);

	if (!IsOK)
	{
		return;
	}

}

//定值指令
void CCommPlugin::Slot_setDevOrder(DEV_BASE &devData)
{
	m_pApduSender->OnSendDevDataRequest(&devData);
}

void CCommPlugin::Slot_SetZoomArea(ZOOM_BASE &zoomBaseInfo)
{
    m_pApduSender->OnSendZoomDataRequest(&zoomBaseInfo);
}




//t0超时
/*********************************************************************************************************
** \brief Slot_TimeOut0
** \details t0超时
** \param argl  
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_TimeOutT0()
{
	emit Signal_SocketError("trying connect TCP server.....");
	this->StartRun(m_strIP, m_iPort);
}
/*********************************************************************************************************
** \brief Slot_TimeOutT1
** \details t
** \param argl  召唤结构
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CCommPlugin::Slot_TimeOutT1()
{
	//t1超时 认为tcp连接发生错误 重新建立连接
	m_k = 0;
	m_w = 0;
	m_bIsRunning = false;
	m_nRecvNum = 0;
	m_nSendNum = 0;
	this->StopRun();
	//关闭所有的定时器
	m_pTimerOut1->stop();
	m_pTimerOut2->stop();
	m_pTimerOut3->stop();

	QThread::msleep(500);
	this->StartRun(m_strIP, m_iPort);

}

//t2超时
void CCommPlugin::Slot_TimeOutT2()
{
	if (!this->getSender()->Send_S())
	{
		emit Signal_SocketError("send S data error,socket closed");
		//关闭所有定时器
		m_pTimerOut1->stop();
		m_pTimerOut2->stop();
		m_pTimerOut3->stop();
	}
}

//t3超时
void CCommPlugin::Slot_timeOutT3()
{
	if (!this->getSender()->Send_U(TESTFR_ACT))
	{
		emit Signal_SocketError("send U data error,socket closed");
		//关闭所有定时器
		m_pTimerOut1->stop();
		m_pTimerOut2->stop();
		m_pTimerOut3->stop();
	}
}

//复位进程
void CCommPlugin::Slot_ResetProcess()
{
    NBM_TELECTRL telectrl;
    telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = 0;
    telectrl.m_nCtrlType = TELECTRL_RESET_PROCESSCALL;
    bool IsOK = OnCommand(&telectrl);
    if (!IsOK)
    {
        return;
    }
}

//切换定值区
void CCommPlugin::Slot_SwitchFixArea(unsigned short uFixArea)
{
	ASDU200 tAsdu200;
	tAsdu200.type = D_FIX_SWITCH;
	tAsdu200.vsq = 0x01;
	tAsdu200.cot.SetCot(COT_ACT);
	tAsdu200.asduAddr.SetAddr(m_pModule->GetDeviceAddr());
	tAsdu200.m_infoaddr.SetAddr(0);
	tAsdu200.m_infAreaIndex.SetAddr(uFixArea);
	m_pApduSender->OnSendFixAreaCode(&tAsdu200);
}

//读取当前定值区号
void CCommPlugin::Slot_ReadCurrentFixArea()
{
	m_pApduSender->OnSendFixSearch();
}


void CCommPlugin::Slot_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest)
{
	m_pApduSender->OnSendGetCatalogRequest(catalogRequest);
}

void CCommPlugin::Slot_WriteAction(FILE_ATTR_INFO &tFileInfo)
{
	m_pApduSender->OnSendWriteFileAction(tFileInfo);
}

void CCommPlugin::Slot_ReadAction(FILE_ATTR_INFO &tFileInfo)
{
	m_pApduSender->OnSendReadFileAction(tFileInfo);
}

//总召超时
void CCommPlugin::Slot_SendGeneralResquestMsg()
{
	//启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
	//召唤全数据
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nCtrlType = TELECTRL_REQUEST_GENERALCALL;
	bool IsOK = OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}

	//召唤全电度
// 	telectrl.m_nCtrlType = TELECTRL_REQUEST_SYNCTIME;
// 	IsOK = OnCommand(&telectrl);
// 	if (!IsOK)
// 	{
// 		return;
// 	}

	telectrl.m_nCtrlType = TELECTRL_REQUEST_KWHGENERALCALL;
	IsOK = OnCommand(&telectrl);
	if (!IsOK)
	{
		return;
	}
}

//时间对时
void CCommPlugin::Slot_SendSyncRequestMsg()
{
    //启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
    //召唤全数据
    NBM_TELECTRL telectrl;
    telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = 0;

    //对时请求
    telectrl.m_nCtrlType = TELECTRL_REQUEST_SYNCTIME;
    bool IsOK = OnCommand(&telectrl);

    if (!IsOK)
    {
        return;
    }
}

void CCommPlugin::Slot_SendKwhTimeRequestMsg()
{
    //启动时 将会发送三种请求数据  召唤全数据  召唤总电度  对时求情    
    //召唤全数据
    NBM_TELECTRL telectrl;
    telectrl.m_nStationID = 0;
    telectrl.m_nDeviceID = 0;

    telectrl.m_nCtrlType = TELECTRL_REQUEST_KWHGENERALCALL;
    bool IsOK = OnCommand(&telectrl);
    if (!IsOK)
    {
        return;
    }
}


void CCommPlugin::Slot_SetRecordOrder(QList<LB_DATA> &lbData)
{
    if (lbData.count() > 0)
    {
        m_pApduSender->SetLbAllInfo(lbData);
        m_pApduSender->OnSendRecordCatalog(lbData.at(0));
    }
}

CFtuModule * CCommPlugin::GetFtpModule()
{
    return m_pModule;
}

void CCommPlugin::SetGeneralTime(int nGeneralTime, int nSynTime,int nKwhTime)
{
    m_pTimeGeneralSendMsg->setInterval(nGeneralTime * 60 * 1000);
    m_pTimerSyncTimeMsg->setInterval(nSynTime * 60 * 1000);
    m_pTimerKwhMsg->setInterval(nKwhTime * 60 * 1000);
}

void CCommPlugin::Slot_SetSycsTime(QDateTime tSycsTime)
{
    m_pApduSender->OnSendSycsOwnTime(m_pModule->GetDeviceAddr(),tSycsTime);
}
