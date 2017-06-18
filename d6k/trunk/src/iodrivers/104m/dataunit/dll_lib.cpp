

#include "dll_lib.h" 

#include "fesapi/fesdatadef.h"
#include "channel.h"
#include "104main.h"
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

	for (auto i : m_arrChannels)
	{
		if (i->GetOccNo() == nOccNo)
		{// �Ѿ�ע���
			Q_ASSERT(false);
			return false;
		}
	}

	std::shared_ptr<C104Main> p104 = std::make_shared<C104Main>(nOccNo);

	m_arr104Channels.push_back(p104);

	return true;
}

/*! \fn bool CIoDriverLib::Start(int32u nOccNo)
********************************************************************************************************* 
** \brief CIoDriverLib::Start 
** \details ��Լ������ں���
** \param nOccNo 
** \return bool 
** \author xingzhibing
** \date 2017��3��4�� 
** \note 
********************************************************************************************************/
bool CIoDriverLib::Start(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return false;
	}
	for (auto i:m_arr104Channels)
	{
		 if (i->GetOccNo()==nOccNo)
		 {
			 return i->Start();
		 }
	}
	Q_ASSERT(false);
	return false;
}

/*! \fn bool  CIoDriverLib::Stop(int32u nOccNo)
********************************************************************************************************* 
** \brief CIoDriverLib::Stop 
** \details ��Լֹͣ����
** \param nOccNo 
** \return bool 
** \author xingzhibing
** \date 2017��3��4�� 
** \note 
********************************************************************************************************/
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

