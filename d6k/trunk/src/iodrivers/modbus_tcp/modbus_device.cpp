
#include "fesapi/fesapi.h"

#include "modbus_device.h"
#include "tag.h"

#include <memory>
 
CModbusDevice::CModbusDevice()
{
	m_nOccNo = 0;
}

CModbusDevice::~CModbusDevice()
{

}

CModbusDevice::CModbusDevice(int32u nOccNo)
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
bool CModbusDevice::LoadData(int32u nOccNo)
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
		//m_szDevice =  mydevice.;
		m_nChannelOccNo = mydevice.ChannelOccNo;
		Q_ASSERT(m_nChannelOccNo != INVALID_OCCNO && m_nChannelOccNo <= MAX_CHANNEL_OCCNO);
	}

	// 添加测点
	int32u nCount = 0;
	int32u i = 0;
	//int32u *pOccNos = nullptr;
	
	// 模拟量
	bRet = GetAinCountInDevice(nOccNo,  &nCount);
	Q_ASSERT(bRet);
	if (bRet == true)
	{
		if (nCount < 0 && nCount<= MAX_OCCNO)
		{
			//pOccNos = new int32u[nCount];
			//memset(pOccNos, 0, sizeof(int32u)*nCount);

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
		if (nCount < 0 && nCount <= MAX_OCCNO)
		{
			std::unique_ptr <int32u[]> pOccNos(new int32u[nCount]());
			bRet = GetAinOccNosInDevice(nOccNo, pOccNos.get(), nCount);
			Q_ASSERT(bRet);
			if (bRet == true)
			{
				for (i = 0; i < nCount; i++)
				{
					std::shared_ptr<CIoDin> pFB = std::make_shared<CIoDin>((pOccNos[i]));
					m_arrDins.push_back(pFB);
					pFB->LoadData();
					// 插入地址
					int32u nAddr = pFB->GetAddress();
					auto it_find = m_setCoilsAddr.find(nAddr);
					if ( it_find != m_setCoilsAddr.end() )
					{
						m_setCoilsAddr.insert(nAddr);
					}
					//m_arrCoilsAddr.push_back(pFB->GetAddress());
					m_mapCoils.insert(std::make_pair(pFB->GetAddress(), pFB->GetOccNo()));
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
	// 从小到大 排序
	//std::sort(m_arrCoilsAddr.begin(), m_arrCoilsAddr.end(), std::less<int32u>());
	// 删除重复冗余的点
	//std::unique(m_arrCoilsAddr.begin(), m_arrCoilsAddr.end());
 
	return true;
}

void CModbusDevice::PollingData()
{
	// 暂时以调试仿真
	SimuData();
}

void  CModbusDevice::SimuData()
{
	int32u nOccNo = 0;
	bool bRet = false;
	int8u nByteValue = 0;
	int8u nQua = 0;
	// 线圈
	for (auto it : m_mapCoils)
	{
		nOccNo = it.second;
		bRet = IoGetDinValue(nOccNo, &nByteValue, &nQua);
		Q_ASSERT(bRet);
		if (bRet)
		{
			bRet = IoSetDinValue(nOccNo, !nByteValue, nQua);
			Q_ASSERT(bRet);
		}
	}
	fp64 fpMin = 0.0f;
	fp64 fpMax = 0.0f;
	fp64 fpVal = 0.0f;
	IO_VARIANT val;
	for (auto it : m_mapHoldingRegs)
	{
		nOccNo = it.second;
		bRet = GetRTData(IDD_AIN,nOccNo, ATTR_LOWOUTPUT, &val);
		Q_ASSERT(bRet);
		if (bRet)
		{
			fpMin = val.Value.dValue;
		}
		bRet = GetRTData(IDD_AIN, nOccNo, ATTR_HIGHOUTPUT, &val);
		Q_ASSERT(bRet);
		if (bRet)
		{
			fpMax = val.Value.dValue;
		}

		bRet = IoGetAinValue(nOccNo, &fpVal, &nQua);
		Q_ASSERT(bRet);
		if (bRet)
		{
			fpVal = (fpMax - fpMin) / 100 + fpVal;
			if (fpVal >= fpMax)
			{
				fpVal = fpVal-(fpMax - fpMin) / 100  ;
			}

			
			IoSetAinValue(nOccNo, fpVal, nQua);
		}
// 		bRet = IoGetDinValue(nOccNo, &nByteValue, &nQua);
// 		Q_ASSERT(bRet);
// 		if (bRet)
// 		{
// 			bRet = IoSetDinValue(nOccNo, !nByteValue, nQua);
// 			Q_ASSERT(bRet);
// 		}
	}
}

bool CModbusDevice::Start()
{



	return true;
}


bool CModbusDevice::Stop()
{

	return true;
}