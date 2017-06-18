/*! @file modbus_channel.cpp
<PRE>
********************************************************************************
模块名       :  通道管理
文件名       :  modbus_channel.cpp
文件实现功能 :
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : modbus_tcp的通道下面通常只有一个装置，
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author LiJin
*  @version 1.0
*  @date    2016.10.08
*/
#include "fesapi/fesapi.h"
#include "modbus_channel.h"
#include "modbus_device.h"
#include "tcpclient.h"

#include <thread>
 
CModbusChannel::CModbusChannel()
{

}

CModbusChannel::~CModbusChannel()
{
	m_arrDevices.clear();
}

CModbusChannel::CModbusChannel(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	m_nOccNo = nOccNo;

// 	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
// 		return;	
}
/*! \fn bool CModbusChannel::LoadData(int32u nOccNo)
********************************************************************************************************* 
** \brief CModbusChannel::LoadData 
** \details 从内存库加载信息
** \param nOccNo 
** \return bool 
** \author LiJin 
** \date 2016年9月13日 
** \note 
********************************************************************************************************/
bool CModbusChannel::LoadData(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	m_nOccNo = nOccNo;

	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
		return false;
	
	bool bRet = false;

	CHANNEL ch;
	memset(&ch, 0, sizeof(CHANNEL));

	// 获取通道信息
	bRet = GetChannel(nOccNo, &ch);
	if (bRet == false)
	{// todo : log


	}
	else
	{
		// 通道名称
		m_szChannel = ch.ChannelName;
	}
	// 添加装置、测点，建立树形数据结构
	int32u nDeviceCount = 0;
	bRet = GetDeviceCount(nOccNo, &nDeviceCount);
	Q_ASSERT(bRet);
	if (bRet == false)
	{// todo : log

		return false;
	}
	if (nDeviceCount == 0 || nDeviceCount > MAX_CHANNEL_COUNT)
		return false;	

	std::unique_ptr<int32u[]>pOccNos(new int32u[nDeviceCount]()); 
	memset(pOccNos.get(),0, sizeof(int32u)*nDeviceCount);

	bRet = GetDeviceOccNos(nOccNo, pOccNos.get(), nDeviceCount);
	Q_ASSERT(bRet);
	if (bRet == false)
	{// todo : log

// 		delete [] pOccNos;
// 		pOccNos = nullptr;
		return false;
	}

	int32u i = 0;
	for (i = 0; i < nDeviceCount; i++)
	{
		std::shared_ptr<CModbusDevice>pDevice = std::make_shared<CModbusDevice>(pOccNos[i]);
		bRet = pDevice->LoadData(pOccNos[i]);
		Q_ASSERT(bRet);
		if (bRet)
		{
			m_arrDevices.push_back(pDevice);
		}
	}

// 	delete[] pOccNos;
// 	pOccNos = nullptr;

	return true;
}

bool CModbusChannel::Start()
{
	if (m_pTcpClient == nullptr)
	{
		m_pTcpClient = std::make_shared<CTcpClient>();
	}
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CModbusChannel::MainTask, this, nullptr));
		//m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CModbusChannel::MainTask, this, nullptr);
	}

	return true;
}


bool CModbusChannel::Stop()
{
	SetTaskStopFlag();
	// 退出线程
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
	return true;
}

void CModbusChannel::PollingData()
{
	unsigned int nDeviceNum = 0;

	unsigned int i = 0;
	bool bRet = false;

	auto it = m_arrDevices.begin();
	for ( ;it!= m_arrDevices.end(); ++it)
	{
		(*it)->PollingData();
	}
#if 0
	//1. 依次召唤485网络中各个装置的测值
	for (i = 0; i < nDeviceNum; i++)
	{
		//2. 请求模拟量
		//	bRet = ReqAinData(  );
		if (bRet)
		{
			//	bRet = ReadData();
			if (bRet)
			{// 成功

			}
			else
			{// 失败

			}
		}
		//3. 请求开关量

		//4. 如果有上位机命令的话,解析命令,并做操作报警  解析完后清空内容
	}
#endif
}
/*! \fn int  CModbusChannel::RecvMsgDoNull()
********************************************************************************************************* 
** \brief CModbusChannel::RecvMsgDoNull 
** \details 收空此前的信息
** \return int 
** \author LiJin 
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
int  CModbusChannel::RecvMsgDoNull()
{
	FES_CMD cmd;
	while (ReadHostCmd(0, &cmd, 0))
	{
		;
	}
	return 0;
}

/*! \fn void CModbusChannel::MainTask(void *pChannel)
********************************************************************************************************* 
** \brief CModbusChannel::MainTask 
** \details 通信主任务
** \param pChannel 
** \return void 
** \author LiJin 
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
void CModbusChannel::MainTask(void *pChannel)
{
	std::string strIp("127.0.0.1");
	int nPort = 502;
	int32u nRet = 0;
	std::chrono::milliseconds dura(10);

	bool bRet = IsFesAlive();
	// 收空此前的信息
	RecvMsgDoNull();
	
	FES_CMD cmd;

	while (true)
	{
		if (m_pTcpClient->Open((char*)strIp.c_str(), nPort))
			break;

		// 如果发现要求退出，则退出		 
		if (IsDriverQuit(m_nOccNo) == true)
			return  ;

		std::this_thread::sleep_for(dura);
	}
	
	while (true)
	{
		 if (bRet == false)
		 {
			 // todo log

			 break;
		 }
		 // 如果发现要求退出，则退出
		 if (IsDriverQuit(0) == true)
		 {
			 break;
		 }
		 // 前置内核退出
		 if (IsFesAlive() == false)
		 {
			 break;
		 }

		 nRet = GetMyHostState();
		 if (nRet == 0)
		 {// 从机
			 RecvMsgDoNull();
			 std::this_thread::sleep_for(dura);
			 continue;
		 }
		 else
		 {// 主机状态 开始通信
			 PollingData();
		 }
	}
}


void CModbusChannel::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}



/** @}*/
