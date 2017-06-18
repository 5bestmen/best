

#include "fesconfig.h"
#include "fesapi/fes_magicmem.h"
#include <QtCore/QtGlobal>


//内部排序
template<typename T>
bool SortMyMgrFuction(const T& T1, const T& T2)
{
	return T1->GetOccNo() < T2->GetOccNo();
}

template<typename T>
bool SortFuction(const T& T1, const T& T2)
{
	return T1->OccNo < T2->OccNo;
}


CChannel::CChannel(CHANNEL_PTR pChannel)
{
	Q_ASSERT(pChannel);
	m_ChannelPtr = pChannel;
}

CChannel::~CChannel()
{
	/*m_VecDev.clear();
	m_arrAins.clear();
	m_arrDins.clear();
	m_arrAouts.clear();
	m_arrDouts.clear();*/
}
/*! \fn size_t CChannel::SaveToMem(unsigned char *pMem)
********************************************************************************************************* 
** \brief CChannel::SaveToMem 
** \details 将通道的引用信息写入
** \param pMem 
** \return size_t 
** \author LiJin 
** \date 2016年9月9日 
** \note 
********************************************************************************************************/
size_t CChannel::SaveToMem(unsigned char *pMem)
{
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return 0;

	size_t nLen = sizeof(CHANNEL_REF);

	//CHANNEL
	CHANNEL_REF *pChannelRef = reinterpret_cast <CHANNEL_REF*>(pMem);
	pChannelRef->MagicHead = MAGIC_HEAD;
	pChannelRef->IddType = IDD_CHANNEL;
	Q_ASSERT(m_ChannelPtr);
	if (m_ChannelPtr)
	{
		pChannelRef->ChannelOccNo = m_ChannelPtr->OccNo;
	}
	int32u nDeviceCount = static_cast<int32u>(m_VecDev.size());
	pChannelRef->DeviceCount = nDeviceCount;

	pChannelRef->AinCount = static_cast<int32u>(m_arrAins.size());
	pChannelRef->DinCount = static_cast<int32u>(m_arrDins.size());
	pChannelRef->AoutCount = static_cast<int32u>(m_arrAouts.size());
	pChannelRef->DoutCount = static_cast<int32u>(m_arrDouts.size());
	pChannelRef->ExtraLen = static_cast<int32u>(GetChannelInfoExtraLen());

	pMem =(unsigned char*) pChannelRef+ sizeof(CHANNEL_REF);
 
	//DEVICE
	Q_ASSERT(pChannelRef->DeviceCount < MAX_DEVICE_COUNT);
	if (pChannelRef->DeviceCount && pChannelRef->DeviceCount <= MAX_DEVICE_COUNT)
	{
		int32u *pOccNos = new int32u[pChannelRef->DeviceCount];
		memset(pOccNos, 0, sizeof(int32u)*pChannelRef->DeviceCount);

		unsigned int i = 0;
		for (auto it: m_VecDev)
		{
			pOccNos[i] = it->GetOccNo();
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pChannelRef->DeviceCount);

		delete [] pOccNos;
		nLen += sizeof(int32u)*pChannelRef->DeviceCount;
		pMem += sizeof(int32u)*pChannelRef->DeviceCount;
	}

	// AIN
	Q_ASSERT(pChannelRef->AinCount <= MAX_OCCNO);
	if (pChannelRef->AinCount && pChannelRef->AinCount <= MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pChannelRef->AinCount];
		std::memset(pOccNos, 0, sizeof(int32u)*pChannelRef->AinCount);

		unsigned int i = 0;
		for (auto it : m_arrAins)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pChannelRef->AinCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pChannelRef->AinCount;
		pMem += sizeof(int32u)*pChannelRef->AinCount;
	}
	// DIN  
	Q_ASSERT(pChannelRef->DinCount <= MAX_OCCNO);
	if (pChannelRef->DinCount && pChannelRef->DinCount <= MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pChannelRef->DinCount];
		memset(pOccNos, 0, sizeof(int32u)*pChannelRef->DinCount);

		unsigned int i = 0;
		for (auto it : m_arrDins)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pChannelRef->DinCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pChannelRef->DinCount;
		pMem += sizeof(int32u)*pChannelRef->DinCount;
	}

	//AOUT
	Q_ASSERT(pChannelRef->AoutCount <= MAX_OCCNO);
	if (pChannelRef->AoutCount && pChannelRef->AoutCount <= MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pChannelRef->AoutCount];
		memset(pOccNos, 0, sizeof(int32u)*pChannelRef->AoutCount);

		unsigned int i = 0;
		for (auto it : m_arrAouts)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pChannelRef->AoutCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pChannelRef->AoutCount;
		pMem += sizeof(int32u)*pChannelRef->AoutCount;
	}

	//DOUT
	Q_ASSERT(pChannelRef->DoutCount <= MAX_OCCNO);
	if (pChannelRef->DoutCount && pChannelRef->DoutCount <= MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pChannelRef->DoutCount];
		memset(pOccNos, 0, sizeof(int32u)*pChannelRef->DoutCount);

		unsigned int i = 0;
		for (auto it : m_arrDouts)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pChannelRef->DoutCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pChannelRef->AoutCount;
		pMem += sizeof(int32u)*pChannelRef->AoutCount;
	}

	return nLen;
}
/*! \fn size_t CChannel::GetChannelInfoExtraLen()
********************************************************************************************************* 
** \brief CChannel::GetChannelInfoExtraLen 
** \details 点表的长度
** \return size_t 
** \author LiJin 
** \date 2016年9月9日 
** \note 
********************************************************************************************************/
size_t CChannel::GetChannelInfoExtraLen()
{
	size_t nLen = 0;
	
	nLen += sizeof(int32u) * m_VecDev.size();

	nLen += sizeof(int32u) * m_arrAins.size();
	nLen += sizeof(int32u) * m_arrDins.size();
	nLen += sizeof(int32u) * m_arrAouts.size();
	nLen += sizeof(int32u) * m_arrDouts.size();

	return nLen;
}

/*! \fn size_t CChannel::GetChannelInfoLen()
********************************************************************************************************* 
** \brief CChannel::GetChannelInfoLen 
** \details  获取channel的信息长度
** \return size_t 
** \author LiJin 
** \date 2016年9月9日 
** \note 
********************************************************************************************************/
size_t CChannel::GetChannelInfoLen()
{
	size_t nLen = sizeof(CHANNEL_REF);
//	Q_ASSERT( m_VecDev.size() < MAX_DEVICE_COUNT);
	nLen += GetChannelInfoExtraLen();

	return nLen;
}
/*! \fn void CCHANNEL::AddDev(DEVICE* pDev)
********************************************************************************************************* 
** \brief CCHANNEL::AddDev 
** \details 通道管理器：添加设备
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CChannel::AddDev(CDevicePtr pDev)
{
	Q_ASSERT(pDev);
	for (auto i:m_VecDev)
	{
		if (i == pDev)
		{
			Q_ASSERT(false);
			return;
		}
	}
	if (pDev)
	{
		m_VecDev.push_back(pDev);
	}	
}
/*! \fn void CCHANNEL::CreateDevice(DEVICE* pDev)
********************************************************************************************************* 
** \brief CCHANNEL::CreateDevice 
** \details 创建设备
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
CDevicePtr CChannel::CreateDevice(DEVICE_PTR pDev)
{
	for (auto i:m_VecDev)
	{
		if (i->GetDevicePtr()== pDev)
		{
			Q_ASSERT(false);

			return i;
		}
	}
	CDevicePtr pDe = std::make_shared<CDevice>(pDev);

	m_VecDev.push_back(pDe);

	return pDe;
}
/*! \fn void CChannel::SortChannelFunction()
********************************************************************************************************* 
** \brief CChannel::SortChannelFunction 
** \details 通道内部 设备 点排序
** \return void 
** \author xingzhibing
** \date 2016年9月14日 
** \note 
********************************************************************************************************/
void CChannel::SortChannelFunction()
{
	//设备排序
	std::sort(m_VecDev.begin(), m_VecDev.end(), SortMyMgrFuction<CDevicePtr>);

	//点号排序
	std::sort(m_arrAins.begin(), m_arrAins.end(), SortFuction<AIN_PTR>);
	std::sort(m_arrDins.begin(), m_arrDins.end(), SortFuction<DIN_PTR>);
	std::sort(m_arrAouts.begin(), m_arrAouts.end(), SortFuction<AOUT_PTR>);
	std::sort(m_arrDouts.begin(), m_arrDouts.end(), SortFuction<DOUT_PTR>);
}
/*! \fn void CDevice::AddAin(AIN* pAin)
********************************************************************************************************* 
** \brief CDevice::AddAin 
** \details 
** \param pAin 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CDevice::AddAin(AIN_PTR pAin)
{
	Q_ASSERT(pAin);
	/*for (auto i:m_Ains)
	{
		if (i == pAin)
		{
			Q_ASSERT(false);
			return;
		}
	}*/

	if (pAin)
	{
		m_Ains.push_back(pAin);
	}
}
/*! \fn void CDevice::AddDin(DIN* pDin)
********************************************************************************************************* 
** \brief CDevice::AddDin 
** \details 
** \param pDin 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CDevice::AddDin(DIN_PTR pDin)
{
	Q_ASSERT(pDin);

	/*for (auto i:m_Dins)
	{
		if (i == pDin)
		{
			Q_ASSERT(false);
			return;
		}
	}*/

	if (pDin)
	{
		m_Dins.push_back(pDin);
	}
}
/*! \fn void CDevice::AddAout(AOUT* pAout)
********************************************************************************************************* 
** \brief CDevice::AddAout 
** \details 
** \param pAout 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CDevice::AddAout(AOUT_PTR pAout)
{
	Q_ASSERT(pAout);
	/*for (auto i:m_Aouts)
	{
		if (i == pAout)
		{
			Q_ASSERT(false);
			return;
		}
	}*/
	if (pAout)
	{
		m_Aouts.push_back(pAout);
	}
}
/*! \fn void CDevice::AddDout(DOUT* pDout)
********************************************************************************************************* 
** \brief CDevice::AddDout 
** \details 
** \param pDout 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CDevice::AddDout(DOUT_PTR pDout)
{
	Q_ASSERT(pDout);
/*
#ifdef  DEBUG
	for (auto i : m_Douts)
	{
		if (i == pDout)
		{
			Q_ASSERT(false);
			return;
		}
	}

#endif //  DEBUG*/

	if (pDout)
	{
		m_Douts.push_back(pDout);
	}
}

CDevice::CDevice(DEVICE_PTR pDev)
{
	Q_ASSERT(pDev);
	m_DevPtr = pDev;
}

CDevice::~CDevice()
{
	m_Ains.clear();
	m_Dins.clear();
	m_Aouts.clear();
	m_Douts.clear();
}

size_t CDevice::SaveToMem(unsigned char *pMem)
{
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return 0;

	size_t nLen = sizeof(DEVICE_REF);

	//CHANNEL
	DEVICE_REF *pDevRef = reinterpret_cast <DEVICE_REF*>(pMem);
	pDevRef->MagicHead = MAGIC_HEAD;
	pDevRef->IddType = IDD_DEVICE;

	Q_ASSERT(m_DevPtr);

	if (m_DevPtr)
	{
		pDevRef->DeviceOccNo = m_DevPtr->OccNo;
	}

	pDevRef->AinCount = static_cast<int32u>(m_Ains.size());
	pDevRef->DinCount = static_cast<int32u>(m_Dins.size());
	pDevRef->AoutCount = static_cast<int32u>(m_Aouts.size());
	pDevRef->DoutCount = static_cast<int32u>(m_Douts.size());
	pDevRef->ExtraLen = static_cast<int32u>(GetDevInfoExtraLen());

	pMem = (unsigned char*)(pMem)+sizeof(DEVICE_REF);

	// AIN
	Q_ASSERT(pDevRef->AinCount <= MAX_OCCNO);
	if (pDevRef->AinCount && pDevRef->AinCount <= MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pDevRef->AinCount];

		unsigned int i = 0;
		for (auto it : m_Ains)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pDevRef->AinCount);

		delete[] pOccNos;
		nLen += sizeof(int32u) *pDevRef->AinCount;
		pMem += sizeof(int32u) *pDevRef->AinCount;
	}
	// DIN  
	Q_ASSERT(pDevRef->DinCount < MAX_OCCNO);
	if (pDevRef->DinCount && pDevRef->DinCount < MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pDevRef->DinCount];

		unsigned int i = 0;
		for (auto it : m_Dins)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pDevRef->DinCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pDevRef->DinCount;
		pMem += sizeof(int32u)*pDevRef->DinCount;
	}

	//AOUT
	Q_ASSERT(pDevRef->AoutCount < MAX_OCCNO);
	if (pDevRef->AoutCount && pDevRef->AoutCount < MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pDevRef->AoutCount];

		unsigned int i = 0;
		for (auto it : m_Aouts)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pDevRef->AoutCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pDevRef->AoutCount;
		pMem += sizeof(int32u)*pDevRef->AoutCount;
	}

	//DOUT
	Q_ASSERT(pDevRef->DoutCount < MAX_OCCNO);
	if (pDevRef->DoutCount && pDevRef->DoutCount < MAX_OCCNO)
	{
		int32u *pOccNos = new int32u[pDevRef->DoutCount];

		unsigned int i = 0;
		for (auto it : m_Douts)
		{
			Q_ASSERT(it);
			if (it)
			{
				pOccNos[i] = it->OccNo;
			}
			i++;
		}

		std::memcpy(pMem, pOccNos, sizeof(int32u)*pDevRef->DoutCount);

		delete[] pOccNos;
		nLen += sizeof(int32u)*pDevRef->AoutCount;
		pMem += sizeof(int32u)*pDevRef->AoutCount;
	}

	return nLen;
}

size_t CDevice::GetDevInfoExtraLen()
{
	size_t nLen = 0;

	nLen += sizeof(int32u) * m_Ains.size();
	nLen += sizeof(int32u) * m_Dins.size();
	nLen += sizeof(int32u) * m_Aouts.size();
	nLen += sizeof(int32u) * m_Douts.size();

	return nLen;
}
/*! \fn size_t CDevice::GetDevLen()
********************************************************************************************************* 
** \brief CDevice::GetDevLen 
** \details 获取设备总长度
** \return size_t 
** \author xingzhibing
** \date 2016年9月13日 
** \note 
********************************************************************************************************/
size_t CDevice::GetDevLen()
{
	size_t nLen = sizeof(DEVICE_REF);
	nLen += GetDevInfoExtraLen();
	return nLen;
}

/*! \fn void CDevice::SortDevFunction()
********************************************************************************************************* 
** \brief CDevice::SortDevFunction 
** \details 设备点排序
** \return void 
** \author xingzhibing
** \date 2016年9月14日 
** \note 
********************************************************************************************************/
void CDevice::SortDevFunction()
{
	std::sort(m_Ains.begin(), m_Ains.end(), SortFuction<AIN_PTR>);
	std::sort(m_Dins.begin(), m_Dins.end(), SortFuction<DIN_PTR>);
	std::sort(m_Aouts.begin(), m_Aouts.end(), SortFuction<AOUT_PTR>);
	std::sort(m_Douts.begin(), m_Douts.end(), SortFuction<DOUT_PTR>);
}
/*! \fn void CChannelMgr::AddChannel(CCHANNEL* pChannel)
********************************************************************************************************* 
** \brief CChannelMgr::AddChannel 
** \details 通道管理器：添加通道
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CChannelMgr::AddChannel(CChannelPtr pChannel)
{
	Q_ASSERT(pChannel);
	for (auto i: m_ChannelVec)
	{
		if (i == pChannel)
		{
			Q_ASSERT(false);
			return;
		}
	}

	if (pChannel)
	{
		m_ChannelVec.push_back(pChannel);
	}
}
/*! \fn void CChannelMgr::CreateChannel(CHANNEL* pChannel)
********************************************************************************************************* 
** \brief CChannelMgr::CreateChannel 
** \details 创建通道数据
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
CChannelPtr CChannelMgr::CreateChannel(CHANNEL_PTR pChannel)
{
	//查询是否存在此通道,已经存在则无需创建
	for (auto i:m_ChannelVec)
	{
		if (i->GetChannelPtr() == pChannel)
		{
			// TODO log 记录下
			return i;
		}
	}

	CChannelPtr pCh = std::make_shared<CChannel>(pChannel);

	m_ChannelVec.push_back(pCh);

	return pCh;
}
/*! \fn CDevice_PTR CChannelMgr::CreateDevice(CHANNEL_PTR, CDevice_PTR)
********************************************************************************************************* 
** \brief CChannelMgr::CreateDevice 
** \details 
** \param  
** \param  
** \return CDevice_PTR 
** \author xingzhibing
** \date 2016年9月10日 
** \note 
********************************************************************************************************/
CDevicePtr CChannelMgr::CreateDevice(CChannelPtr pChannel, DEVICE_PTR pDev)
{
	for (auto i:m_ChannelVec)
	{
		if (i==pChannel)
		{
			CDevicePtr pDe = std::make_shared<CDevice>(pDev);

			m_DeviceVec.push_back(pDe);

			pChannel->AddDev(pDe);

			return pDe;
		}
	}
	return Q_NULLPTR;
}
/*! \fn void CChannelMgr::SortFunction()
********************************************************************************************************* 
** \brief CChannelMgr::SortFunction 
** \details 内存通道设备 升序排列
** \return void 
** \author xingzhibing
** \date 2016年9月14日 
** \note 
********************************************************************************************************/
void CChannelMgr::SortMgrFunction()
{
	//通道号排序
	std::sort(m_ChannelVec.begin(), m_ChannelVec.end(), SortMyMgrFuction<CChannelPtr>);
	//通道内点号排序
	for (auto i:m_ChannelVec)
	{
		i->SortChannelFunction();
	}
	//设备号排序
	std::sort(m_DeviceVec.begin(), m_DeviceVec.end(), SortMyMgrFuction<CDevicePtr>);
	//设备内点号排序
	for (auto i:m_DeviceVec)
	{
		i->SortDevFunction();
	}
}
