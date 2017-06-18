
#include "log/log.h"
#include "fesapi/fesapi.h"
#include "feslib.h"
#include "mem_db.h"
#include "tagname_pool.h"
#include "stringpool.h"
#include <QDebug>

static CFesApi s_FesApi;

extern "C"
{
	/*! \fn FESAPI bool OpenIO(const char *pszDataPath, int32u nChannelOccNO)
	*********************************************************************************************************
	** \brief OpenIO
	** \details 连接前置内核,打开系统邮箱服务等 供驱动程序调用
	** \param pszDataPath
	** \param nChannelOccNO
	** \return FESAPI bool
	** \author LiJin
	** \date 2016年10月8日
	** \note  驱动的ChannelOccNo按实际填写，其他均为0，自己程序打开邮箱
	********************************************************************************************************/
	FESAPI bool OpenIO(const char *pszDataPath, int32u nChannelOccNO, unsigned int nMode)
	{
		bool bRet=s_FesApi.Initialize(pszDataPath, nChannelOccNO,0);
		Q_ASSERT(bRet);
		if (!bRet)
		{
			return false;
		}
		return true;
	}


	FESAPI bool CloseIO(int32u nChannelOccNO)
	{
		s_FesApi.Shutdown(nChannelOccNO);
		return true;
	}
	/*! \fn FESAPI bool GetMyNodeOccNo(const char *pszMyHostName, int32u *pOccNo)
	*********************************************************************************************************
	** \brief GetMyNodeOccNo
	** \details 获取本节点的OCCNO
	** \param pszMyHostName
	** \param pOccNo
	** \return FESAPI bool
	** \author LiJin
	** \date 2016年9月12日
	** \note
	********************************************************************************************************/
	FESAPI bool GetMyNodeOccNoByHostName(const char *pszMyHostName, int32u *pOccNo)
	{
		Q_ASSERT(pszMyHostName);
		Q_ASSERT(pOccNo);

		if (pszMyHostName == nullptr || pOccNo == nullptr)
			return false;

		Q_ASSERT(strlen(pszMyHostName) > 0);
		if (strlen(pszMyHostName) == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pOccNo = pMemDB->GetNodeOccNoByHostName(pszMyHostName);
			return true;
		}
		return false;
	}
	// 获取某前置下的通道数量
	FESAPI bool GetChannelCount(int32u nNodeOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetChannelCount());
			return true;
		}
		return false;
	}

	FESAPI bool GetDeviceCount(int32u nChannelOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetDeviceCount(nChannelOccNo));
			return true;
		}

		return false;
	}
	// 获取某通道下装置排行号列表
	FESAPI bool GetDeviceOccNos(int32u nChannelOccNo, int32u pArrDeviceOccNo[], int32u nArraySize)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(pArrDeviceOccNo != nullptr && nArraySize > 0);
		if (pArrDeviceOccNo == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetDeviceOccNos(nChannelOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					pArrDeviceOccNo[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetAinCountInChannel(int32u nChannelOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetAinCountInChannel(nChannelOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetDinCountInChannel(int32u nChannelOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetDinCountInChannel(nChannelOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetAoutCountInChannel(int32u nChannelOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetAoutCountInChannel(nChannelOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetDoutCountInChannel(int32u nChannelOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetDoutCountInChannel(nChannelOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetAinOccNosInChannel(int32u nChannelOccNo, int32u arrayAinOccNo[], int32u nArraySize)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(arrayAinOccNo != nullptr && nArraySize > 0);
		if (arrayAinOccNo == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetAinOccNosInChannel(nChannelOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					arrayAinOccNo[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetDinOccNosInChannel(int32u nChannelOccNo, int32u arrayDinOccNo[], int32u nArraySize)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(arrayDinOccNo != nullptr && nArraySize > 0);
		if (arrayDinOccNo == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetDinOccNosInChannel(nChannelOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					arrayDinOccNo[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetAoutOccNosInChannel(int32u nChannelOccNo, int32u arrayAoutOccNo[], int32u nArraySize)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(arrayAoutOccNo != nullptr && nArraySize > 0);
		if (arrayAoutOccNo == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetAoutOccNosInChannel(nChannelOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					arrayAoutOccNo[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetDoutOccNosInChannel(int32u nChannelOccNo, int32u arrayDoutsOccNo[], int32u nArraySize)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(arrayDoutsOccNo != nullptr && nArraySize > 0);
		if (arrayDoutsOccNo == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetDoutOccNosInChannel(nChannelOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					arrayDoutsOccNo[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	// 获取装置下 测点数量
	FESAPI bool GetAinCountInDevice(int32u nDeviceOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetAinCountInDevice(nDeviceOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetDinCountInDevice(int32u nDeviceOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetDinCountInDevice(nDeviceOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetAoutCountInDevice(int32u nDeviceOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetAoutCountInDevice(nDeviceOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetDoutCountInDevice(int32u nDeviceOccNo, int32u* pCount)
	{
		Q_ASSERT(pCount);
		if (pCount == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			*pCount = static_cast<int32u> (pMemDB->GetDoutCountInDevice(nDeviceOccNo));
			return true;
		}

		return false;
	}

	FESAPI bool GetAinOccNosInDevice(int32u nDeviceOccNo, int32u pOccNos[], int32u nArraySize)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;

		Q_ASSERT(pOccNos != nullptr && nArraySize > 0);
		if (pOccNos == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetAinOccNosInDevice(nDeviceOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					pOccNos[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetDinOccNosInDevice(int32u nDeviceOccNo, int32u pOccNos[], int32u nArraySize)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;

		Q_ASSERT(pOccNos != nullptr && nArraySize > 0);
		if (pOccNos == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetDinOccNosInDevice(nDeviceOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					pOccNos[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetAoutOccNosInDevice(int32u nDeviceOccNo, int32u pOccNos[], int32u nArraySize)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;

		Q_ASSERT(pOccNos != nullptr && nArraySize > 0);
		if (pOccNos == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetAoutOccNosInDevice(nDeviceOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					pOccNos[i] = arrOccNos[i];
				}
				return true;
			}
		}
		return false;
	}

	FESAPI bool GetDoutOccNosInDevice(int32u nDeviceOccNo, int32u pOccNos[], int32u nArraySize)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;

		Q_ASSERT(pOccNos != nullptr && nArraySize > 0);
		if (pOccNos == nullptr || nArraySize == 0)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			std::vector<int32u>  arrOccNos;
			size_t nRet = pMemDB->GetDoutOccNosInDevice(nDeviceOccNo, arrOccNos);
			if (nRet)
			{
				int32u i = 0;
				for (i = 0; i < nArraySize && i < arrOccNos.size(); i++)
				{
					pOccNos[i] = arrOccNos[i];
				}
				return true;
			}
		}

		return false;
	}

	FESAPI bool GetChannelPtr(int32u nChannelOccNo, CHANNEL** ppFB)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(ppFB && *ppFB);
		if (ppFB == nullptr || *ppFB == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetChannelPtr(nChannelOccNo, ppFB);
		}
		return bRet;
	}

	FESAPI bool GetDevicePtr(int32u nDeviceOccNo, DEVICE** ppFB)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;

		Q_ASSERT(ppFB && *ppFB);
		if (ppFB == nullptr || *ppFB == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDevicePtr(nDeviceOccNo, ppFB);
		}
		return bRet;
	}

	FESAPI bool GetAinPtr(int32u nOccNo, AIN** ppFB)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;
		/*

				Q_ASSERT(ppFB && *ppFB);
				if (ppFB == nullptr || *ppFB == nullptr)
					return false;*/

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetAinPtr(nOccNo, ppFB);
		}
		return bRet;
	}

	FESAPI bool GetDinPtr(int32u nOccNo, DIN** ppFB)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		/*Q_ASSERT(ppFB && *ppFB);
		if (ppFB == nullptr || *ppFB == nullptr)
			return false;*/

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDinPtr(nOccNo, ppFB);
		}
		return bRet;
	}

	FESAPI bool GetAoutPtr(int32u nOccNo, AOUT** ppFB)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(ppFB && *ppFB);
		if (ppFB == nullptr || *ppFB == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetAoutPtr(nOccNo, ppFB);
		}
		return bRet;
	}

	FESAPI bool GetDoutPtr(int32u nOccNo, DOUT** ppFB)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		//Q_ASSERT(ppFB && *ppFB);
		//if (ppFB == nullptr || *ppFB == nullptr)
		//	return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDoutPtr(nOccNo, ppFB);
		}
		return bRet;
	}

	FESAPI bool GetChannel(int32u nChannelOccNo, CHANNEL*pChannel)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		Q_ASSERT(pChannel);
		if (pChannel == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetChannel(nChannelOccNo, *pChannel);
		}
		return bRet;
	}

	FESAPI bool GetDevice(int32u nDeviceOccNo, DEVICE*pDevice)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;

		Q_ASSERT(pDevice);
		if (pDevice == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDevice(nDeviceOccNo, *pDevice);
		}
		return bRet;
	}

	FESAPI bool GetAin(int32u nOccNo, AIN*pAIN)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pAIN);
		if (pAIN == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetAin(nOccNo, *pAIN);
		}
		return bRet;
	}

	FESAPI bool GetDin(int32u nOccNo, DIN*pDIN)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pDIN);
		if (pDIN == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDin(nOccNo, *pDIN);
		}
		return bRet;
	}

	FESAPI bool GetAout(int32u nOccNo, AOUT*pAout)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pAout);
		if (pAout == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetAout(nOccNo, *pAout);
		}
		return bRet;
	}

	FESAPI bool GetDout(int32u nOccNo, DOUT*pDout)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pDout);
		if (pDout == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDout(nOccNo, *pDout);
		}
		return bRet;
	}

	FESAPI char* GetAinAddress(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->GetAinAddress(nOccNo);
		}

		return nullptr;
	}

	FESAPI char* GetDinAddress(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->GetDinAddress(nOccNo);
		}

		return nullptr;
	}

	FESAPI char* GetAoutAddress(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->GetAoutAddress(nOccNo);
		}

		return nullptr;
	}

	FESAPI char* GetDoutAddress(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->GetDoutAddress(nOccNo);
		}

		return nullptr;
	}

	FESAPI bool IsDriverQuit(int32u nChannelOccNo)
	{
		return true;
	}

	FESAPI bool LoadDrivers()
	{
		return true;
	}

	FESAPI bool UnLoadDrivers()
	{
		return true;
	}

	FESAPI bool IsFesAlive()
	{
		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IsDBAlive(0);
		}

		return bRet;
	}

	// 本节点的工作状态
	FESAPI int32u GetMyHostState()
	{
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->GetMyHostState();
		}
		return STATE_UNKOWN;
	}

	/*! \fn FESAPI bool ReadHostCmd(INT32U nChannleNo, FES_CMD *pCmd, INT32U nTimeout)
	*********************************************************************************************************
	** \brief ReadHostCmd
	** \details 读取下发的控制命令
	** \param nChannleNo
	** \param pCmd
	** \param nTimeout
	** \return FESAPI bool
	** \author LiJin
	** \date 2017年2月15日
	** \note
	********************************************************************************************************/
	FESAPI bool ReadHostCmd(INT32U nChannleNo, SETVAL_MSG *pCmd, INT32U nTimeout)
	{
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->ReadHostCmd(nChannleNo, pCmd, nTimeout);
		}
		return false;
	}

	/*! \fn bool AppGetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
	********************************************************************************************************* 
	** \brief AppGetUserVarValue 
	** \details  APP层获取用户变量数值
	** \param nOccNo 
	** \param pVariant 
	** \param pQua 
	** \return bool 
	** \author xingzhibing
	** \date 2017年5月31日 
	** \note 
	********************************************************************************************************/
	FESAPI bool AppGetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
	{
		auto pMem = s_FesApi.GetMemDB();
		if (pMem)
		{
			 return pMem->AppGetUserVarValue(nOccNo, pVariant,pQua);
		}
		return false;
	}

	/*! \fn bool AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
	********************************************************************************************************* 
	** \brief AppGetSysVarValue 
	** \details  APP获取系统变量数值
	** \param nOccNo 
	** \param pVariant 
	** \param pQua 
	** \return bool 
	** \author xingzhibing
	** \date 2017年5月31日 
	** \note 
	********************************************************************************************************/
	FESAPI bool AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
	{
		auto pMem = s_FesApi.GetMemDB();
		if (pMem)
		{
			return pMem->AppGetSysVarValue(nOccNo, pVariant, pQua);
		}
		return false;
	}

	/*! \fn bool AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQua)
	********************************************************************************************************* 
	** \brief AppSetUserVarValue 
	** \details  APP设定用户变量数值
	** \param nOccNo 
	** \param pVariant 
	** \param nQua 
	** \return bool 
	** \author xingzhibing
	** \date 2017年5月31日 
	** \note 
	********************************************************************************************************/
	FESAPI bool AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQua)
	{
		auto pMem = s_FesApi.GetMemDB();
		if (pMem)
		{
			return pMem->AppSetUserVarValue(nOccNo, pVariant , nQua );
		}
		return false;
	}

	/*! \fn bool IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
	********************************************************************************************************* 
	** \brief IoSetUserVarValue 
	** \details 驱动设定用户变量数值
	** \param nOccNo 
	** \param pVariant 
	** \param nQuality 
	** \return bool 
	** \author xingzhibing
	** \date 2017年5月31日 
	** \note 
	********************************************************************************************************/	
	FESAPI bool IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
	{
		auto pMem = s_FesApi.GetMemDB();
		if (pMem)
		{
			return pMem->IoSetUserVarValue(nOccNo, pVariant, nQuality);
		}
		return false;
	}

	/*! \fn bool IoSetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
	********************************************************************************************************* 
	** \brief IoSetSysVarValue 
	** \details 驱动设定用户变量数值
	** \param nOccNo 
	** \param pVariant 
	** \param nQuality 
	** \return bool 
	** \author xingzhibing
	** \date 2017年5月31日 
	** \note 
	********************************************************************************************************/	
	FESAPI bool IoSetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
	{
		auto pMem = s_FesApi.GetMemDB();
		if (pMem)
		{
			return pMem->IoSetUserVarValue(nOccNo, pVariant, nQuality);
		}
		return false;
	}

	FESAPI bool IoSetDeviceQua(int32u nDeviceOccNo, int8u nQuality)
	{
		Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
		if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
			return false;
		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetDeviceQua(nDeviceOccNo, nQuality);
		}
		return bRet;
	}

	FESAPI bool IoSetChannelQua(int32u nChannelOccNo, int8u nQuality)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetChannelQua(nChannelOccNo, nQuality);
		}
		return bRet;
	}

	FESAPI bool IoSetAinValueEx(int32u nOccNo, fp64 fValue, int8u nQuality, TIMEPAK * pTm)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetAinValue(nOccNo, fValue, nQuality);
		}
		return bRet;
	}

	FESAPI bool IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality)
	{
		return IoSetAinValueEx(nOccNo, fValue, nQuality, nullptr);
	}

	FESAPI bool IoGetAinValue(int32u nOccNo, fp64 *pValue, int8u *pQua)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pValue && pQua);
		if (pValue == nullptr || pQua == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoGetAinValue(nOccNo, *pValue, *pQua);
		}
		return bRet;
	}

	FESAPI bool IoSetDinValueEx(int32u nOccNo, int8u nValue, int8u nQuality, bool bCpuEnable, TIMEPAK * pTm)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetDinValue(nOccNo, nValue, nQuality, bCpuEnable, pTm);
		}
		return bRet;
	}

	FESAPI bool IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetDinValue(nOccNo, nValue, nQuality);
		}
		return bRet;
	}

	FESAPI bool IoDinAlarm(int32u nOccNo, int8u nValue, int8u nQuality, TIMEPAK * pTm)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoDinAlarm(nOccNo, nValue, nQuality, pTm);
		}
		return bRet;
	}

	FESAPI bool IoGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pValue && pQua);
		if (pValue == nullptr || pQua == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoGetDinValue(nOccNo, *pValue, *pQua);
		}
		return bRet;
	}
	/*! \fn FESAPI bool IoSetAoutValue(int32u nOccNo, fp64 fValue, int8u nQuality)
	*********************************************************************************************************
	** \brief IoSetAoutValue
	** \details 有部分装置会缓存模拟量输出值，该接口是回读模拟量输出值
	** \param nOccNo
	** \param fValue
	** \param nQuality
	** \return FESAPI bool
	** \author LiJin
	** \date 2016年12月3日
	** \note
	********************************************************************************************************/
	FESAPI bool IoSetAoutValue(int32u nOccNo, fp64 fValue, int8u nQuality)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetAoutValue(nOccNo, fValue, nQuality);
		}
		return bRet;
	}

	FESAPI bool IoSetDoutValue(int32u nOccNo, int8u nValue, int8u nQuality)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->IoSetDoutValue(nOccNo, nValue, nQuality);
		}
		return bRet;
	}
	/*! \fn FESAPI bool GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData)
	*********************************************************************************************************
	** \brief GetRTData
	** \details 获取 系统 基础对象（BaseData）的各个属性值
	** \param nIddType IDD_NODE... IDD_AIN等
	** \param nOccNo
	** \param nFiledID  VALUE\QUA\... 具体见 fescnst.h
	** \param pRetData
	** \return FESAPI bool
	** \author LiJin
	** \date 2016年10月20日
	** \note 注意，如果是字符串需要 回收资源
	********************************************************************************************************/
	FESAPI bool GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData)
	{
		Q_ASSERT(pRetData);
		if (pRetData == nullptr)
			return false;

		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetRTData(nIddType, nOccNo, nFiledID, *pRetData);
		}

		return bRet;
	}

	FESAPI bool PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, int32u nParam, IO_VARIANT *pData, const char * szOperatorName, const char * szMonitorName, void *pExt, bool bOpLog)
	{
		Q_ASSERT(pData);
		if (pData == nullptr)
			return false;

		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			//		bRet = pMemDB->GetRTData(nIddType, nOccNo, nFiledID, *pRetData);
		}

		return true;
	}
	/*! \fn FESAPI int32u GetOccNoByTagName(const char * szTagName, int32u *pDataType)
	*********************************************************************************************************
	** \brief GetOccNoByTagName
	** \details 通过 名字 或者 该测点的排行号
	** \param szTagName
	** \param pDataType
	** \return FESAPI int32u
	** \author LiJin
	** \date 2016年10月24日
	** \note
	********************************************************************************************************/
	FESAPI int32u GetOccNoByTagName(const char * szTagName, int32u *pDataType)
	{
		Q_ASSERT(szTagName && pDataType);
		if (szTagName == nullptr || strlen(szTagName) == 0 || pDataType == nullptr)
		{
			return INVALID_OCCNO;
		}

		auto pTagNamePool = s_FesApi.GetTagNamePool();
		if (pTagNamePool)
		{
			return pTagNamePool->GetOccNoByTagName(szTagName, *pDataType);
		}

		return INVALID_OCCNO;
	}
	FESAPI bool GetTagNameByOccNo(int32u nDataType, int8s* strTagName, int32u* pOccNo)
	{
		Q_ASSERT(nDataType != IDD_NULL  &&nDataType <= MAX_IDD);
		if (nDataType == IDD_NULL || nDataType > MAX_IDD)
		{
			return false;
		}
		Q_ASSERT(*pOccNo != INVALID_OCCNO && *pOccNo <= MAX_OCCNO);
		if (*pOccNo == INVALID_OCCNO || *pOccNo > MAX_OCCNO)
		{
			return false;
		}
		Q_ASSERT(strTagName && strlen(strTagName) > 0);
		if (strTagName || strlen(strTagName) == 0)
		{
			return false;
		}
		auto pTagNamePool = s_FesApi.GetTagNamePool();
		if (pTagNamePool)
		{
			return pTagNamePool->GetTagNameByOccNo(nDataType, *pOccNo, std::string(strTagName));
		}
		return false;
	}


	FESAPI void IoSetDeviceHeartBeat(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			pMemDB->IoSetDeviceHeartBeat(nOccNo);
		}
	}

	FESAPI void IoSetChannelHeartBeat(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			pMemDB->IoSetChannelHeartBeat(nOccNo);
		}
	}
	/*! \fn FESAPI void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt)
	*********************************************************************************************************
	** \brief IoDiagAlarm
	** \details 自诊断报警接口，常用于通信的自诊断，如：XX节点 1#通道 2#装置 通信故障
	** \param nChannleNo
	** \param nDeviceNo
	** \param pszAlarmTxt
	** \return FESAPI void
	** \author LiJin
	** \date 2016年12月2日
	** \note
	********************************************************************************************************/
	FESAPI void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm)
	{
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			pMemDB->IoDiagAlarm(nChannleNo, nDeviceNo, pszAlarmTxt, pTm);
		}
	}

	FESAPI void IoAlarmMsg(int32u nChannleNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm)
	{
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			pMemDB->IoAlarmMsg(nChannleNo, nAlarmType, pszAlarmTxt, pTm);
		}
	}

	FESAPI bool AppSetDoutValue(int32u nOccNo, int8u Value, int8u nSource)
	{
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->AppSetDoutValue(nOccNo, Value, nSource);
		}

		return false;
	}
	FESAPI bool AppSetAoutValue(int32u nOccNo, fp64 Value, int32u nSource)
	{
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->AppSetAoutValue(nOccNo, Value, nSource);
		}

		return false;
	}

	FESAPI bool GetAinAlarm(int32u nOccNo, AIN_ALARM *pAinAlarm)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
			return false;

		Q_ASSERT(pAinAlarm);
		if (pAinAlarm == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetAinAlarmEx(nOccNo, *pAinAlarm);
		}
		return bRet;
	}

	FESAPI bool GetDinAlarm(int32u nOccNo, DIN_ALARM *pDinAlarm)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
			return false;

		Q_ASSERT(pDinAlarm);
		if (pDinAlarm == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDinAlarmEx(nOccNo, *pDinAlarm);
		}
		return bRet;
	}

	FESAPI bool GetAinAlarmLimit(int32u nOccNo, AIN_ALARM_LIMIT *pAinAlarmLimit)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
			return false;

		Q_ASSERT(pAinAlarmLimit);
		if (pAinAlarmLimit == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetAinAlarmLimit(nOccNo, *pAinAlarmLimit);
		}
		return bRet;
	}

	FESAPI bool GetDinAlarmLimit(int32u nOccNo, DIN_ALARM_LIMIT *pDinAlarmLimit)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
			return false;

		Q_ASSERT(pDinAlarmLimit);
		if (pDinAlarmLimit == nullptr)
			return false;

		bool bRet = false;
		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			bRet = pMemDB->GetDinAlarmLimit(nOccNo, *pDinAlarmLimit);
		}
		return bRet;
	}
	/*! \fn FESAPI bool  GetDescByOccNo(int32u nOccNo, bool bIsTranslate, char* pBuff)
	*********************************************************************************************************
	** \brief GetDescByOccNo
	** \details
	** \param nOccNo        描述点OCCNO
	** \param bIsTranslate  是否使用翻译文字
	** \param pBuff         返回的字符串描述
	** \return FESAPI bool
	** \author xingzhibing
	** \date 2017年3月13日
	** \note
	********************************************************************************************************/
	FESAPI bool  GetDescByOccNo(int32u nOccNo, bool bIsTranslate, char* pBuff)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		{
			return nullptr;
		}
		Q_ASSERT(pBuff && strlen(pBuff) > 0);
		if (!pBuff || strlen(pBuff) == 0)
		{
			return false;
		}
		auto pMemDB = s_FesApi.GetStringPool();
		Q_ASSERT(pMemDB);
		if (!pMemDB)
		{
			return nullptr;
		}
		bool bRet = pMemDB->GetDescByOccno(nOccNo, bIsTranslate, pBuff);
		Q_ASSERT(bRet);

		return nullptr;
	}
	/*! \fn FESAPI bool AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua)
	*********************************************************************************************************
	** \brief AppGetAinValue
	** \details 应用层获取模拟量
	** \param nOccNo
	** \param pValue
	** \param pQua
	** \return FESAPI bool
	** \author LiJin
	** \date 2017年4月13日
	** \note
	********************************************************************************************************/
	FESAPI bool AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pValue);
		if (pValue == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->AppGetAinValue(nOccNo, pValue, pQua);
		}

		return true;
	}
	/*! \fn FESAPI bool AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua)
	*********************************************************************************************************
	** \brief AppGetDinValue
	** \details 应用层获取开关量
	** \param nOccNo
	** \param pValue
	** \param pQua
	** \return FESAPI bool
	** \author LiJin
	** \date 2017年4月13日
	** \note
	********************************************************************************************************/
	FESAPI bool AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
			return false;

		Q_ASSERT(pValue);
		if (pValue == nullptr)
			return false;

		auto pMemDB = s_FesApi.GetMemDB();
		if (pMemDB)
		{
			return pMemDB->AppGetDinValue(nOccNo, pValue, pQua);
		}
		return true;
	}
}



/*
FESAPI bool OpenChannelMailBox(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return false;
	auto pMemDB = s_FesApi.GetMemDB();
	if (pMemDB)
	{
		return pMemDB->OpenChannelMailBox(nChannelOccNo);
	}
	return true;
}*/
