/*! @file modbus_channel.cpp
<PRE>
********************************************************************************
ģ����       :  ͨ������
�ļ���       :  modbus_channel.cpp
�ļ�ʵ�ֹ��� :
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : modbus_tcp��ͨ������ͨ��ֻ��һ��װ�ã�
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author xingzhibing
*  @version 1.0
*  @date    2016.10.24
*/
#include "fesapi/fesapi.h"
#include "simu_channel.h"
#include "simu_device.h"
#include <thread>
#include <QObject>
 
CSimuChannel::CSimuChannel()
{

}

CSimuChannel::~CSimuChannel()
{
	m_arrDevices.clear();
}

CSimuChannel::CSimuChannel(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	m_nOccNo = nOccNo;

// 	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
// 		return;	
}
/*! \fn bool CModbusChannel::LoadData(int32u nOccNo)
********************************************************************************************************* 
** \brief CModbusChannel::LoadData 
** \details ���ڴ�������Ϣ
** \param nOccNo 
** \return bool 
** \author LiJin 
** \date 2016��9��13�� 
** \note 
********************************************************************************************************/
bool CSimuChannel::LoadData(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	m_nOccNo = nOccNo;

	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
		return false;
	
	bool bRet = false;

	CHANNEL ch;
	memset(&ch, 0, sizeof(CHANNEL));

	// ��ȡͨ����Ϣ
	bRet = GetChannel(nOccNo, &ch);
	if (bRet == false)
	{// todo : log


	}
	else
	{
		// ͨ������
		m_szChannel = ch.ChannelName;
	}

	// ���װ�á���㣬�����������ݽṹ
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
		return false;
	}

	int32u i = 0;
	for (i = 0; i < nDeviceCount; i++)
	{
		std::shared_ptr<CSimuDevice>pDevice = std::make_shared<CSimuDevice>(pOccNos[i]);
		bRet = pDevice->LoadData(pOccNos[i]);
		Q_ASSERT(bRet);
		if (bRet)
		{
			m_arrDevices.push_back(pDevice);
		}
	}

	return true;
}

bool CSimuChannel::Start()
{
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CSimuChannel::MainTask, this, nullptr));
	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CSimuChannel::MainTask, this, nullptr);
	}
	
	return true;
}


bool CSimuChannel::Stop()
{
	SetTaskStopFlag();
	// �˳��߳�
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
	return true;
}

void CSimuChannel::PollingData()
{
	auto it = m_arrDevices.begin();
	for ( ;it!= m_arrDevices.end(); ++it)
	{
		(*it)->PollingData();
	}
}
/*! \fn int  CModbusChannel::RecvMsgDoNull()
********************************************************************************************************* 
** \brief CModbusChannel::RecvMsgDoNull 
** \details �տմ�ǰ����Ϣ
** \return int 
** \author LiJin 
** \date 2016��10��8�� 
** \note 
********************************************************************************************************/
int  CSimuChannel::RecvMsgDoNull()
{
	return 0;
}

/*! \fn void CModbusChannel::MainTask(void *pChannel)
********************************************************************************************************* 
** \brief CModbusChannel::MainTask 
** \details ͨ��������
** \param pChannel 
** \return void 
** \author LiJin 
** \date 2016��10��8�� 
** \note 
********************************************************************************************************/
void CSimuChannel::MainTask(void *pChannel)
{
	std::string strIp("127.0.0.1");
	int nPort = 502;
	int32u nRet = 0;
	std::chrono::milliseconds dura(10);

	bool bRet = IsFesAlive();
	
	while (true)
	{
		if ( IsFesAlive() == false)
		{
			LogMsg(QObject::tr("Fes is dead,so i have to exit!").toStdString().c_str(),1);
			break;
		}
		PollingData();
	}
}


void CSimuChannel::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}



/** @}*/
