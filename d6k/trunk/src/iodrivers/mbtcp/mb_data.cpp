
#include <ace/Assert.h>
#include "mb_data.h"



CMBEReg::CMBEReg()
{
}

CMBEReg::~CMBEReg()
{
}


CMBDevice::CMBDevice()
{
}


CMBDevice::CMBDevice(int32u OccNo)
{
	m_nDeviceOccNo = OccNo;
}


CMBDevice::~CMBDevice()
{
}

bool CMBDevice::InitDeviceData(int32u nOccNO)
{

	return true;
}


CMBChannel::CMBChannel()
{ 
}

CMBChannel::~CMBChannel(void)
{
	auto i = m_arrDevices.begin();
	for (; i != m_arrDevices.end(); ++i)
	{
		if (*i)
		{
			delete (*i);
			*i = nullptr;
		}
	}
}
 

bool CMBChannel::InitChannelData(int32u nOccNO)
{
	/*
	int32u nDeviceCnt = 0;
	KRESULT kRet = K_FALSE;

	kRet = GetDeviceCount(nOccNO, &nDeviceCnt);
	ACE_ASSERT(kRet == K_TRUE);
	if (kRet == K_TRUE && nDeviceCnt )
	{
		int32u *pTempOccNO = new int32u[nDeviceCnt];
		kRet = GetDeviceOccNos(pTempOccNO, nDeviceCnt);
		ACE_ASSERT(kRet == K_TRUE);

		if (kRet)
		{
			int32u i = 0;
			for (i = 0; i < nDeviceCnt; i++)
			{
				ACE_ASSERT(pTempOccNO[i] > 0 && pTempOccNO[i] <= MAX_OCCNO);
				if (pTempOccNO[i] > 0 && pTempOccNO[i] <= MAX_OCCNO)
				{
					CMBDevice *pDevice = new CMBDevice(pTempOccNO[i]);
					pDevice->InitDeviceData(pTempOccNO[i]);
					m_arrDevices.push_back(pDevice);
				}
			}
		}

		delete[] pTempOccNO;
		return true;
	}
	*/
	return false;
}
