

#include "dll_lib.h" 

#include "fesapi/fesdatadef.h"
#include "channel.h"
#include "modbus_channel.h"

#include <QDebug>

CIoDriverLib::CIoDriverLib()
{

}

CIoDriverLib::~CIoDriverLib()
{
	m_arrChannels.clear();
}
/*! \fn bool CIoDriverLib::RegisterChannel(int32u nOccNo)
********************************************************************************************************* 
** \brief CIoDriverLib::RegisterChannel 
** \details ע��һ��ͨ������FES�ںˣ������ڴ棩��ȡ��������
** \param nOccNo ͨ����OCCNO
** \return bool 
** \author LiJin 
** \date 2016��10��8�� 
** \note 
********************************************************************************************************/
bool CIoDriverLib::RegisterChannel(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return false;
	}

	for (auto i:m_arrChannels)
	{
		if (i->GetOccNo() == nOccNo)
		{// �Ѿ�ע���
			Q_ASSERT(false);
			return false;
		}
	}

	std::shared_ptr<CModbusChannel> pChannel = std::make_shared<CModbusChannel>();

	bool bRet = pChannel->LoadData(nOccNo);
	Q_ASSERT(bRet);
	if (bRet)
	{
		m_arrChannels.push_back(pChannel);
	}
	 
	return bRet;
}

bool CIoDriverLib::Start(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return false;
	}

	for (auto i : m_arrChannels)
	{
		if (i->GetOccNo() == nOccNo)
		{
			return i->Start();
		}
	}
	Q_ASSERT(false);
	return false;
}

bool  CIoDriverLib::Stop(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return false;
	}

	for (auto i : m_arrChannels)
	{
		if (i->GetOccNo() == nOccNo)
		{
			return i->Stop();
		}
	}
	Q_ASSERT(false);
	return false;
}

