/*! @file node.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  node.cpp
文件实现功能 :  系统的节点定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   系统的节点定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/

#include "channel.h"
#include "fesapi/fesdatadef.h"
#include "fesapi/fes_magicmem.h"

#include "log/log.h"  

#include <QObject>
#include <memory>
#include <QDebug>
 

CFesChannel::CFesChannel()
{
	m_fnSetDeviceQua = nullptr;
}

CFesChannel::~CFesChannel()
{

}

CFesChannel::CFesChannel(CHANNEL *pChannel) :m_pFB(pChannel)
{
	m_fnSetDeviceQua = nullptr;

	Q_ASSERT(pChannel);
	if (pChannel == nullptr)
		return;

	m_nOccNo = pChannel->OccNo;
}
/*! \fn size_t CFesChannel::LoadFromMem(unsigned char *pMem)
********************************************************************************************************* 
** \brief CFesChannel::LoadFromMem 
** \details 从内存中加载配置
** \param pMem 
** \return size_t 
** \author LiJin 
** \date 2016年9月16日 
** \note 
********************************************************************************************************/
size_t CFesChannel::LoadFromMem(unsigned char *pMem)
{
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return 0;
	
	CHANNEL_REF *pChannelRef = reinterpret_cast<CHANNEL_REF*>(pMem);
	
	Q_ASSERT(pChannelRef->MagicHead == MAGIC_HEAD);
	Q_ASSERT(pChannelRef->IddType == IDD_CHANNEL);
	size_t nLen = 0;
	if (pChannelRef->MagicHead == MAGIC_HEAD && pChannelRef->IddType == IDD_CHANNEL)
	{
		Q_ASSERT(pChannelRef->ChannelOccNo != INVALID_OCCNO && pChannelRef->ChannelOccNo <= MAX_CHANNEL_OCCNO);
		Q_ASSERT(pChannelRef->ChannelOccNo == this->m_nOccNo);

		pMem += sizeof(CHANNEL_REF);
		nLen += sizeof(CHANNEL_REF);

		// 装置
		Q_ASSERT(pChannelRef->DeviceCount <= MAX_DEVICE_COUNT);
		if (pChannelRef->DeviceCount && pChannelRef->DeviceCount <= MAX_DEVICE_COUNT)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pChannelRef->DeviceCount]());
// 			int32u *pOccNos = new int32u[pChannelRef->DeviceCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pChannelRef->DeviceCount);
 			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pChannelRef->DeviceCount);

			int32u i = 0;
			for (i = 0; i < pChannelRef->DeviceCount;i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_DEVICE_OCCNO);
				if (pOccNos[i]!=INVALID_OCCNO && pOccNos[i] <= MAX_DEVICE_OCCNO )
				{
					m_arrDevices.push_back(pOccNos[i]);
				}				
			}
		//  delete[] pOccNos;
			nLen += sizeof(int32u)*pChannelRef->DeviceCount;
			pMem += sizeof(int32u)*pChannelRef->DeviceCount;
		}

		Q_ASSERT(pChannelRef->AinCount <= MAX_OCCNO);
		if (pChannelRef->AinCount && pChannelRef->AinCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pChannelRef->AinCount]());
//			int32u *pOccNos = new int32u[pChannelRef->AinCount];
//			memset(pOccNos, 0, sizeof(int32u)* pChannelRef->AinCount);  	 
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pChannelRef->AinCount);

			int32u i = 0;
			for (i = 0; i < pChannelRef->AinCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrAins.push_back(pOccNos[i]);
				}
			}

		//	delete[] pOccNos;
			nLen += sizeof(int32u)*pChannelRef->AinCount;
			pMem += sizeof(int32u)*pChannelRef->AinCount;
		}

		Q_ASSERT(pChannelRef->DinCount <= MAX_OCCNO);
		if (pChannelRef->DinCount && pChannelRef->DinCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pChannelRef->DinCount]());
//			int32u *pOccNos = new int32u[pChannelRef->DinCount];
//			memset(pOccNos, 0, sizeof(int32u)* pChannelRef->DinCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pChannelRef->DinCount);

			int32u i = 0;
			for (i = 0; i < pChannelRef->DinCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrDins.push_back(pOccNos[i]);
				}
			}

//			delete[] pOccNos;
			nLen += sizeof(int32u)*pChannelRef->DinCount;
			pMem += sizeof(int32u)*pChannelRef->DinCount;
		}

		Q_ASSERT(pChannelRef->AoutCount <= MAX_OCCNO);
		if (pChannelRef->AoutCount && pChannelRef->AoutCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pChannelRef->AoutCount]());
// 			int32u *pOccNos = new int32u[pChannelRef->AoutCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pChannelRef->AoutCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pChannelRef->AoutCount);

			int32u i = 0;
			for (i = 0; i < pChannelRef->AoutCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrAouts.push_back(pOccNos[i]);
				}
			}

		//	delete[] pOccNos;
			nLen += sizeof(int32u)*pChannelRef->AoutCount;
			pMem += sizeof(int32u)*pChannelRef->AoutCount;
		}

		Q_ASSERT(pChannelRef->DoutCount <= MAX_OCCNO);
		if (pChannelRef->DoutCount && pChannelRef->DoutCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pChannelRef->DoutCount]());
// 			int32u *pOccNos = new int32u[pChannelRef->DoutCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pChannelRef->DoutCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pChannelRef->DoutCount);

			int32u i = 0;
			for (i = 0; i < pChannelRef->DoutCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrDouts.push_back(pOccNos[i]);
				}
			}

		//	delete[] pOccNos;
			nLen += sizeof(int32u)*pChannelRef->DoutCount;
			pMem += sizeof(int32u)*pChannelRef->DoutCount;
		}
	}

	return nLen;
}

size_t CFesChannel::GetDeviceOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrDevices;
	return arrOccNos.size();
}

size_t CFesChannel::GetAinOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrAins;
	return arrOccNos.size();
}

size_t CFesChannel::GetDinOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrDins;
	return arrOccNos.size();
}

size_t CFesChannel::GetAoutOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrAouts;
	return arrOccNos.size();
}

size_t CFesChannel::GetDoutOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrDouts;
	return arrOccNos.size();
}
/*! \fn bool  CFesChannel::SetQuality(int8u nQuality)
********************************************************************************************************* 
** \brief CFesChannel::SetQuality 
** \details 修改通道的品质位
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月27日 
** \note 
********************************************************************************************************/
bool  CFesChannel::SetQuality(int8u nQuality)
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return false;
	
	if (nQuality == QUALITY_OK)
	{
		m_pFB->Quality = QUALITY_OK;
	}
	else
	{
		bool bRet = false;
		m_pFB->Quality = QUALITY_BAD;
		Q_ASSERT(m_fnSetDeviceQua);
		if (m_fnSetDeviceQua)
		{
			for (auto it : m_arrDevices)
			{
				bRet |= m_fnSetDeviceQua(it, nQuality);
			}
		}
		return bRet;
	}
	return false;
}
/*! \fn void  CFesChannel::SetHeartBeat()
********************************************************************************************************* 
** \brief CFesChannel::SetHeartBeat 
** \details 设置心跳信号
** \return void 
** \author LiJin 
** \date 2016年12月1日 
** \note 
********************************************************************************************************/
void  CFesChannel::SetHeartBeat()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return  ;

	m_pFB->Heartbeat++;
}

/** @}*/
