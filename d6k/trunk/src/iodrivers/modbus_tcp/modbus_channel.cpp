/*! @file modbus_channel.cpp
<PRE>
********************************************************************************
ģ����       :  ͨ������
�ļ���       :  modbus_channel.cpp
�ļ�ʵ�ֹ��� :
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : modbus_tcp��ͨ������ͨ��ֻ��һ��װ�ã�
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \details ���ڴ�������Ϣ
** \param nOccNo 
** \return bool 
** \author LiJin 
** \date 2016��9��13�� 
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
	// �˳��߳�
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
	//1. �����ٻ�485�����и���װ�õĲ�ֵ
	for (i = 0; i < nDeviceNum; i++)
	{
		//2. ����ģ����
		//	bRet = ReqAinData(  );
		if (bRet)
		{
			//	bRet = ReadData();
			if (bRet)
			{// �ɹ�

			}
			else
			{// ʧ��

			}
		}
		//3. ���󿪹���

		//4. �������λ������Ļ�,��������,������������  ��������������
	}
#endif
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
** \details ͨ��������
** \param pChannel 
** \return void 
** \author LiJin 
** \date 2016��10��8�� 
** \note 
********************************************************************************************************/
void CModbusChannel::MainTask(void *pChannel)
{
	std::string strIp("127.0.0.1");
	int nPort = 502;
	int32u nRet = 0;
	std::chrono::milliseconds dura(10);

	bool bRet = IsFesAlive();
	// �տմ�ǰ����Ϣ
	RecvMsgDoNull();
	
	FES_CMD cmd;

	while (true)
	{
		if (m_pTcpClient->Open((char*)strIp.c_str(), nPort))
			break;

		// �������Ҫ���˳������˳�		 
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
		 // �������Ҫ���˳������˳�
		 if (IsDriverQuit(0) == true)
		 {
			 break;
		 }
		 // ǰ���ں��˳�
		 if (IsFesAlive() == false)
		 {
			 break;
		 }

		 nRet = GetMyHostState();
		 if (nRet == 0)
		 {// �ӻ�
			 RecvMsgDoNull();
			 std::this_thread::sleep_for(dura);
			 continue;
		 }
		 else
		 {// ����״̬ ��ʼͨ��
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
