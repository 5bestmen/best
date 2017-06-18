/*! @file device.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  device.cpp
文件实现功能 :  装置的处理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   装置的处理
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/

#include "device.h"
#include "fesapi/fesdatadef.h"
#include "fesapi/fes_magicmem.h"

#include "log/log.h"  

#include <QObject>
#include <QDebug>
#include <memory>
 
CFesDevice::CFesDevice()
{
	m_pFB = nullptr;
	m_fnSetAinQua = nullptr;
	m_fnSetDinQua = nullptr;
	m_fnSetAoutQua = nullptr;
	m_fnSetDoutQua = nullptr;
}

CFesDevice::~CFesDevice()
{

}

CFesDevice::CFesDevice(DEVICE *pDevice) :m_pFB(pDevice)
{
	m_nOccNo = pDevice->OccNo;
	m_fnSetAinQua = nullptr;
	m_fnSetDinQua = nullptr;
	m_fnSetAoutQua = nullptr;
	m_fnSetDoutQua = nullptr;
}

size_t CFesDevice::LoadFromMem(unsigned char *pMem)
{
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return 0;
	DEVICE_REF *pDeviceRef = reinterpret_cast<DEVICE_REF*>(pMem);

	Q_ASSERT(pDeviceRef->MagicHead == MAGIC_HEAD);
	Q_ASSERT(pDeviceRef->IddType == IDD_DEVICE);
	size_t nLen = 0;
	if (pDeviceRef->MagicHead == MAGIC_HEAD && pDeviceRef->IddType == IDD_DEVICE)	
	{
		Q_ASSERT(pDeviceRef->DeviceOccNo != INVALID_OCCNO && pDeviceRef->DeviceOccNo <= MAX_CHANNEL_OCCNO);
		Q_ASSERT(pDeviceRef->DeviceOccNo == this->m_nOccNo);

		pMem += sizeof(DEVICE_REF);
		nLen += sizeof(DEVICE_REF);


		Q_ASSERT(pDeviceRef->AinCount <= MAX_OCCNO);
		if (pDeviceRef->AinCount && pDeviceRef->AinCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pDeviceRef->AinCount]());
// 			int32u *pOccNos = new int32u[pDeviceRef->AinCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pDeviceRef->AinCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pDeviceRef->AinCount);

			int32u i = 0;
			for (i = 0; i < pDeviceRef->AinCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrAins.push_back(pOccNos[i]);
				}
			}

//			delete[] pOccNos;
			nLen += sizeof(int32u)*pDeviceRef->AinCount;
			pMem += sizeof(int32u)*pDeviceRef->AinCount;
		}

		Q_ASSERT(pDeviceRef->DinCount <= MAX_OCCNO);
		if (pDeviceRef->DinCount && pDeviceRef->DinCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pDeviceRef->DinCount]());
// 			int32u *pOccNos = new int32u[pDeviceRef->DinCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pDeviceRef->DinCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pDeviceRef->DinCount);

			int32u i = 0;
			for (i = 0; i < pDeviceRef->DinCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrDins.push_back(pOccNos[i]);
				}
			}

//			delete[] pOccNos;
			nLen += sizeof(int32u)*pDeviceRef->DinCount;
			pMem += sizeof(int32u)*pDeviceRef->DinCount;
		}

		Q_ASSERT(pDeviceRef->AoutCount <= MAX_OCCNO);
		if (pDeviceRef->AoutCount && pDeviceRef->AoutCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pDeviceRef->AoutCount]());
// 			int32u *pOccNos = new int32u[pDeviceRef->AoutCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pDeviceRef->AoutCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pDeviceRef->AoutCount);

			int32u i = 0;
			for (i = 0; i < pDeviceRef->AoutCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrAouts.push_back(pOccNos[i]);
				}
			}

	//		delete[] pOccNos;
			nLen += sizeof(int32u)*pDeviceRef->AoutCount;
			pMem += sizeof(int32u)*pDeviceRef->AoutCount;
		}

		Q_ASSERT(pDeviceRef->DoutCount <= MAX_OCCNO);
		if (pDeviceRef->DoutCount && pDeviceRef->DoutCount <= MAX_OCCNO)
		{
			std::unique_ptr<int32u[]>pOccNos(new int32u[pDeviceRef->DoutCount]());
// 			int32u *pOccNos = new int32u[pDeviceRef->DoutCount];
// 			memset(pOccNos, 0, sizeof(int32u)* pDeviceRef->DoutCount);
			std::memcpy(pOccNos.get(), pMem, sizeof(int32u)*pDeviceRef->DoutCount);

			int32u i = 0;
			for (i = 0; i < pDeviceRef->DoutCount; i++)
			{
				Q_ASSERT(pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO);
				if (pOccNos[i] != INVALID_OCCNO && pOccNos[i] <= MAX_OCCNO)
				{
					m_arrDouts.push_back(pOccNos[i]);
				}
			}

		//	delete[] pOccNos;
			nLen += sizeof(int32u)*pDeviceRef->DoutCount;
			pMem += sizeof(int32u)*pDeviceRef->DoutCount;
		}
	}

	return nLen;
}

size_t CFesDevice::GetAinOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrAins;
	return arrOccNos.size();
}

size_t CFesDevice::GetDinOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrDins;
	return arrOccNos.size();
}

size_t CFesDevice::GetAoutOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrAouts;
	return arrOccNos.size();
}

size_t CFesDevice::GetDoutOccNos(std::vector<int32u> & arrOccNos)
{
	arrOccNos.clear();
	arrOccNos = m_arrDouts;
	return arrOccNos.size();
}
/*! \fn bool  CFesDevice::SetQuality(int8u nQuality)
********************************************************************************************************* 
** \brief CFesDevice::SetQuality 
** \details 设置装置的品质位
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月27日 
** \note 
********************************************************************************************************/
bool  CFesDevice::SetQuality(int8u nQuality)
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
		Q_ASSERT(m_fnSetAinQua);
		if (m_fnSetAinQua)
		{
			for ( auto it_ai : m_arrAins )
			{
				bRet = m_fnSetAinQua(it_ai, nQuality);
			}
		}

		Q_ASSERT(m_fnSetDinQua);
		if (m_fnSetDinQua)
		{
			for (auto it_di : m_arrDins)
			{
				bRet = m_fnSetDinQua(it_di, nQuality);
			}
		}

		Q_ASSERT(m_fnSetAoutQua);
		if (m_fnSetAinQua)
		{
			for (auto it_aq : m_arrAouts)
			{
				bRet = m_fnSetAoutQua(it_aq, nQuality);
			}
		}

		Q_ASSERT(m_fnSetDoutQua);
		if (m_fnSetDoutQua)
		{
			for (auto it_dq : m_arrDouts)
			{
				bRet = m_fnSetDoutQua(it_dq, nQuality);
			}
		}

	}
	return true;
}
/*! \fn void  CFesDevice::SetHeartBeat()
********************************************************************************************************* 
** \brief CFesDevice::SetHeartBeat 
** \details 设置装置的心跳信号
** \return void 
** \author LiJin 
** \date 2016年12月1日 
** \note 
********************************************************************************************************/
void  CFesDevice::SetHeartBeat()
{
	Q_ASSERT(m_pFB);
	if (m_pFB == nullptr)
		return;

	m_pFB->Heartbeat++;
}
/** @}*/
