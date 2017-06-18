
#include "fesapi/fesapi.h"

#include "simu_device.h"
#include "tag.h"
#include "variant.h"
#include "fesapi/fesapi.h"
#include "boost/thread/thread.hpp"
#include <memory>
 
CSimuDevice::CSimuDevice()
{
	m_nOccNo = 0;
}

CSimuDevice::~CSimuDevice()
{

}

CSimuDevice::CSimuDevice(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_CHANNEL_OCCNO);
	m_nOccNo = nOccNo;
	
// 	if (nOccNo == 0 || nOccNo > MAX_CHANNEL_OCCNO)
// 		return;	
}
/*! \fn bool CModbusDevice::LoadData(int32u nOccNo)
********************************************************************************************************* 
** \brief CModbusDevice::LoadData 
** \details 从内存库加载信息 
** \param nOccNo 裝置的大排行号
** \return bool 
** \author LiJin 
** \date 2016年9月13日 
** \note 
********************************************************************************************************/
bool CSimuDevice::LoadData(int32u nOccNo)
{
	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_DEVICE_OCCNO);
	m_nOccNo = nOccNo;
//	m_nChannelOccNo = nChannelOccNo;

	if (nOccNo == 0 || nOccNo > MAX_DEVICE_OCCNO)
		return false;

	bool bRet = false;

	DEVICE mydevice;
	memset(&mydevice, 0, sizeof(DEVICE));

	bRet = GetDevice(nOccNo, &mydevice);
	if (bRet == false)
	{// todo : log


	}
	else
	{
		m_nChannelOccNo = mydevice.ChannelOccNo;
		Q_ASSERT(m_nChannelOccNo != INVALID_OCCNO && m_nChannelOccNo <= MAX_CHANNEL_OCCNO);
	}

	// 添加测点
	int32u nCount = 0;
	int32u i = 0;
	
	// 模拟量
	bRet = GetAinCountInDevice(nOccNo,  &nCount);
	Q_ASSERT(bRet);
	if (bRet == true)
	{
		if (nCount > 0 && nCount<= MAX_OCCNO)
		{
			std::unique_ptr <int32u[]> pOccNos(new int32u[nCount]());

			bRet = GetAinOccNosInDevice(nOccNo, pOccNos.get(), nCount);
			Q_ASSERT(bRet);
			if (bRet == true)
			{
				for (i = 0; i < nCount; i++)
				{
					std::shared_ptr<CIoAin> pAin = std::make_shared<CIoAin>((pOccNos[i]));
					m_arrAins.push_back(pAin);
					pAin->LoadData();
				}
			}
			else
			{// todo : log

			}
		}		
	}
	else
	{// todo : log

	}
	
	// 开关量
	bRet = GetDinCountInDevice(nOccNo, &nCount);
	Q_ASSERT(bRet);
	if (bRet == true)
	{
		if (nCount > 0 && nCount <= MAX_OCCNO)
		{
			std::unique_ptr <int32u[]> pOccNos(new int32u[nCount]());
			bRet = GetDinOccNosInDevice(nOccNo, pOccNos.get(), nCount);
			Q_ASSERT(bRet);
			if (bRet == true)
			{
				for (i = 0; i < nCount; i++)
				{
					std::shared_ptr<CIoDin> pFB = std::make_shared<CIoDin>((pOccNos[i]));
					m_arrDins.push_back(pFB);
					pFB->LoadData();
				}
			}
			else
			{// todo : log

			}
		}
	}
	
	return true;
}

void CSimuDevice::PollingData()
{
	unsigned static int nIndex = 0;
	if (nIndex++%2==1)
	{
		SimuData();
	}
	else
	{
		boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
	}
	
}

fp64 GetVariantValue(const IO_VARIANT & val)
{
	fp64 fpRet = 0.0f;

	switch (val.Type)
	{
	case DT_BYTE:
	case DT_BOOLEAN:
	case  DT_CHAR:
		fpRet = static_cast<fp64> (val.Value.bValue);
		break;
	case  DT_SHORT:
	case  DT_WORD:
		fpRet = static_cast<fp64>(val.Value.wValue);
		break;
	case  DT_INT:
	case  DT_DWORD:
		fpRet = static_cast<fp64>(val.Value.nValue);
		break;
	case  DT_LLONG:
	case  DT_LWORD:
		fpRet = static_cast<fp64>(val.Value.sValue);
		break;
	case  DT_FLOAT:
		fpRet = static_cast<fp64>(val.Value.sValue);
		break;
	case  DT_DOUBLE:
		fpRet = static_cast<fp64>(val.Value.dValue);
		break;
	default:
		break;
	}
	return fpRet;
}

void  CSimuDevice::SimuData()
{
	int32u nOccNo = 0;
	bool bRet = false;
	fp64 nAinVal = 0.0f;
	int8u nQua = 0;
	IO_VARIANT nValHigh,nValLow;

	for (auto i:m_arrAins)
	{
		nOccNo = i->GetOccNo();
		bRet = IoGetAinValue(nOccNo, &nAinVal, &nQua);
		Q_ASSERT(bRet);
		if (bRet)
		{
			//获取高值
			GetRTData(IDD_AIN, nOccNo, ATT_HIGHOUTPUT, &nValHigh);
			//获取低值
			GetRTData(IDD_AIN, nOccNo, ATT_LOWOUTPUT, &nValLow);
			//未到最大值

			float nMaxVal = GetVariantValue(nValHigh);

			float nMinVal = GetVariantValue(nValLow);

			nAinVal += (nMaxVal - nMinVal) / 100;

			if (nAinVal > nMaxVal)
			{
				IoSetAinValue(nOccNo, nMinVal, nQua);
				continue;
			}
			if (nAinVal < nMinVal)
			{
				IoSetAinValue(nOccNo, nMaxVal, nQua);
				continue;
			}			
			IoSetAinValue(nOccNo, nAinVal, nQua);
		}
	}

	int8u nByteValue = 0;
	for (auto i: m_arrDins)
	{
		nOccNo = i->GetOccNo();
		bRet = IoGetDinValue(nOccNo, &nByteValue, &nQua);
		Q_ASSERT(bRet);
		if (bRet)
		{
			bRet = IoSetDinValue(nOccNo, !nByteValue, nQua);
			Q_ASSERT(bRet);
		}
	}	
}

bool CSimuDevice::Start()
{
	return true;
}


bool CSimuDevice::Stop()
{
	return true;
}