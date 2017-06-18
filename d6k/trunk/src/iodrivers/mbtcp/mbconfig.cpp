
#include <ace/Assert.h>
#include <fesapi\fesapi.h>
#include "utils\misc.h"

#include "mbconfig.h"

bool CDeviceData::LoadAIN()
{
	//1、加载所有AIN的OccNo
	int32u nAinCount = 0;
	if (GetAinCount(m_nChannelOccNo, m_nDeviceOccNo, &nAinCount) == K_FALSE)
		return false;
	
	if (nAinCount == 0)
		return true;

	int32u* pArrAinOccNos = new int32u[nAinCount];

	if (GetAinOccNos(m_nChannelOccNo, m_nDeviceOccNo,pArrAinOccNos, nAinCount) == K_FALSE)
	{
		delete[] pArrAinOccNos;

		return false;
	}

	for (int32u i = 0; i < nAinCount; i++)
	{
		int32u nAinOccNo = pArrAinOccNos[i];

		m_vecDeviceAINOccNo.push_back(nAinOccNo);
	}

	delete[] pArrAinOccNos;

	//2、建立OccNo和Address的对应关系,并保存所有点的Address
	for (int k = 0; k < m_vecDeviceAINOccNo.size(); k++)
	{
		int32u nAinOccNo = m_vecDeviceAINOccNo[k];

		int32u nAddress;
		GetAinAddress(nAinOccNo,&nAddress);

		m_mapAINAddressToOccNo[nAddress] = nAinOccNo;

		//保存点的Address
		m_vecDeviceAINAddress.push_back(nAddress);
	}

	return true;
}

bool CDeviceData::LoadDIN()
{
	//1、加载所有DIN的OccNo
	int32u nDinCount = 0;
	if (GetDinCount(m_nChannelOccNo, m_nDeviceOccNo, &nDinCount) == K_FALSE)
		return false;

	if (nDinCount == 0)
		return true;

	int32u* pArrDinOccNos = new int32u[nDinCount];

	if (GetDinOccNos(m_nChannelOccNo, m_nDeviceOccNo, pArrDinOccNos, nDinCount) == K_FALSE)
	{
		delete[] pArrDinOccNos;

		return false;
	}

	for (int32u i = 0; i < nDinCount; i++)
	{
		int32u nDinOccNo = pArrDinOccNos[i];

		m_vecDeviceDINOccNo.push_back(nDinOccNo);
	}

	delete[] pArrDinOccNos;

	//2、建立OccNo和Address的对应关系,并保存所有点的Address
	for (int k = 0; k < m_vecDeviceDINOccNo.size(); k++)
	{
		int32u nDinOccNo = m_vecDeviceDINOccNo[k];

		int32u nAddress;
		GetDinAddress(nDinOccNo,&nAddress);

		m_mapDINAddressToOccNo[nAddress] = nDinOccNo;

		//保存点的Address
		m_vecDeviceDINAddress.push_back(nAddress);
	}

	return true;
}

bool CDeviceData::LoadDeviceAddress(int32u nDeviceOccNo)
{
	GetDeviceAddress(nDeviceOccNo, &m_nDeviceAddress);

	return true;
}
	
int32u CDeviceData::GetAinOccNoFromAddress(int32u nAddress)
{
	return m_mapAINAddressToOccNo[nAddress];
}

int32u CDeviceData::GetDinOccNoFromAddress(int32u nAddress)
{
	return m_mapDINAddressToOccNo[nAddress];
}
bool SortDataFunc(const int &v1, const int &v2)
{
	return v1 < v2;//升序排列  
}

bool CDeviceData::PrepareFrame(int32u nDataType, CModbusParam* pModbusParam, std::vector<int32u>& arrAddress)
{
	int nCountPerFrame = (nDataType == DATATYPE_AIN) ? pModbusParam->m_nPerFrameAIN : pModbusParam->m_nPerFrameDIN;
	//对数组排序
	std::sort(arrAddress.begin(), arrAddress.end(), SortDataFunc);

	int nTotalPoints = arrAddress.size();
	if (nTotalPoints <= 0)
		return true;//装置没有数据点

	CFrameData frameData;
	frameData.m_nDataType = nDataType;
	frameData.m_nDeviceAddress = m_nDeviceAddress;
	frameData.m_nDeviceOccNo = m_nDeviceOccNo;

	//分帧
	int nStartID = arrAddress[0];

	std::vector<int> temp;
	for (int i = 0; i < nTotalPoints; i++)
	{
		int nCurID = arrAddress[i];

		if (nCurID >= nStartID && nCurID < nStartID + nCountPerFrame)
		{
			temp.push_back(nCurID);
		}
		else if (nCurID >= nStartID + nCountPerFrame)
		{
			//m_pChannelData->m_arrKwhStartAddr.push_back(nStartID);
			int nTempSize = temp.size();
			int nCount = temp[nTempSize - 1] - temp[0] + 1;
			//m_pChannelData->m_arrKwhCountPerFrame.push_back(nCount);
			temp.clear();

			frameData.m_nCount = nCount;
			frameData.m_nStartAddr = nStartID;
			m_vecDeviceFrames.push_back(frameData);

			nStartID = nCurID;
			temp.push_back(nCurID);
		}

		if (i == nTotalPoints - 1)//不够一帧最大容量,最后一个
		{
			//m_pChannelData->m_arrKwhStartAddr.push_back(nStartID);
			int nTempSize = temp.size();
			int nCount = temp[nTempSize - 1] - temp[0] + 1;
			//m_pChannelData->m_arrKwhCountPerFrame.push_back(nCount);

			frameData.m_nCount = nCount;
			frameData.m_nStartAddr = nStartID;
			m_vecDeviceFrames.push_back(frameData);
		}
	}

	return true;
}

bool CDeviceData::Prepare(CModbusParam* pModbusParam)
{
	if (!PrepareFrame(DATATYPE_AIN, pModbusParam,m_vecDeviceAINAddress))
		return false;

	if (!PrepareFrame(DATATYPE_DIN, pModbusParam,m_vecDeviceDINAddress))
		return false;

	return true;
}

bool CDeviceData::IsDeviceHalt()
{
	int32u nCurTime = Utils::GetTickSecond();
	if (m_nLastHaltTime == 0)
		return false;

	if (nCurTime - m_nLastHaltTime >= MAX_DEVICE_HALT_TIME)
	{
		m_nLastHaltTime = 0;
		return false;
	}
		
	return true;
}
void CDeviceData::SetDeviceHalt()
{
	m_nLastHaltTime = Utils::GetTickSecond();
}

CChannelData::CChannelData()
{ 
}

CChannelData::~CChannelData()
{
}
 
bool CChannelData::LoadDevices(CModbusParam* pModbusParam, int nChannelOccNo)
{
	int32u nDeviceCount = 0;
	if (GetDeviceCount(nChannelOccNo, &nDeviceCount) == K_FALSE)
		return false;

	if (nDeviceCount == 0)
		return true;

	int32u* pArrDeviceOccNos = new int32u[nDeviceCount];

	if (GetDeviceOccNos(nChannelOccNo, pArrDeviceOccNos, nDeviceCount) == K_FALSE)
	{
		delete[] pArrDeviceOccNos;

		return false;
	}
	
	for (int32u i = 0; i < nDeviceCount; i++)
	{
		int32u nDeviceOccNo = pArrDeviceOccNos[i];

		CDeviceData* pDeviceData = new CDeviceData(nChannelOccNo,nDeviceOccNo);
		m_mapDevices[nDeviceOccNo] = pDeviceData;

		pDeviceData->LoadAIN();
		pDeviceData->LoadDIN();
		pDeviceData->LoadDeviceAddress(nDeviceOccNo);

		pDeviceData->Prepare(pModbusParam);
	}

	delete[] pArrDeviceOccNos;

	return true;
}

void CChannelData::DeleteDevices()
{
	typedef std::map<int32u, CDeviceData*>::iterator ITER_MAPDEVICES;
	for (ITER_MAPDEVICES iter = m_mapDevices.begin(); iter != m_mapDevices.end();iter++)
	{
		CDeviceData* pDeviceData = iter->second;
		if (pDeviceData != NULL)
		{
			delete pDeviceData;
			pDeviceData = NULL;
		}
	}
	m_mapDevices.clear();
}

int32u CChannelData::GetAinOccNo(int32u nDeviceOccNo, int32u nAddress)
{
	CDeviceData* pDeviceData = m_mapDevices[nDeviceOccNo];
	if (pDeviceData == NULL)
		return INVALID_OCCNO;

	return pDeviceData->GetAinOccNoFromAddress(nAddress);
}

int32u CChannelData::GetDinOccNo(int32u nDeviceOccNo, int32u nAddress)
{
	CDeviceData* pDeviceData = m_mapDevices[nDeviceOccNo];
	if (pDeviceData == NULL)
		return INVALID_OCCNO;

	return pDeviceData->GetDinOccNoFromAddress(nAddress);
}

