#ifndef  _SCD_DATA_DEF_H
#define  _SCD_DATA_DEF_H
#include "fesapi/fescnst.h"
#include "stl_util-inl.h"
#include "datatypes.h"
#include <qglobal.h>
#include <memory>
#include <vector>
#include <unordered_map>

//左侧树节点类型
enum 
{
	INVALID_TYPE = 0,
	E_ROOT=40,
	E_NODE=401,
	E_CHANNEL=402,
	E_DEVICE,
	E_AIN,
	E_DIN,
	E_AOUT,
	E_DOUT,
	MAX_TYPE=500,
};

struct AIN;
struct DIN;
struct AOUT;
struct DOUT;
struct CHANNEL;
struct DEVICE;

class CScdDevice
{
public:
	CScdDevice(DEVICE* pDev, int32u nOccNo)
	{
		Q_ASSERT(pDev);
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_CHANNEL_OCCNO);
		m_nDeviceOccNo = nOccNo;
		m_pDevice = pDev;
	}
	~CScdDevice()
	{
	}
public:
	std::vector<AIN*>& GetArrAins()
	{
		return m_arrAins;
	}
	std::vector<DIN*>& GetArrDins()
	{
		return m_arrDins;
	}
	std::vector<AOUT*>& GetArrAouts()
	{
		return m_arrAouts;
	}
	std::vector<DOUT*>& GetArrDouts()
	{
		return m_arrDouts;
	}
public:
	void AddAin(AIN* pAin)
	{
		Q_ASSERT(pAin);
		if (pAin)
		{
			m_arrAins.push_back(pAin);
		}
	}
	void AddDin(DIN* pDin)
	{
		Q_ASSERT(pDin);
		if (pDin)
		{
			m_arrDins.push_back(pDin);
		}
	}
	void AddAout(AOUT* pAout)
	{
		Q_ASSERT(pAout);
		if (pAout)
		{
			m_arrAouts.push_back(pAout);
		}
	}
	void AddDout(DOUT* pDout)
	{
		Q_ASSERT(pDout);
		if (pDout)
		{
			m_arrDouts.push_back(pDout);
		}
	}
public:
	size_t GetAinsSize()
	{
		return m_arrAins.size();
	}
	size_t GetDinSize()
	{
		return m_arrDins.size();
	}
	size_t GetAoutSize()
	{
		return m_arrAouts.size();
	}
	size_t GetDoutSize()
	{
		return m_arrDouts.size();
	}
public:
	int32u m_nDeviceOccNo;
	DEVICE* m_pDevice;
private:	
	std::vector<AIN*> m_arrAins;
	std::vector<DIN*> m_arrDins;
	std::vector<AOUT*> m_arrAouts;
	std::vector<DOUT*> m_arrDouts;
};


class CScdChannel
{
public:
	CScdChannel(CHANNEL* pCh, int32u nOccNo)
	{
		Q_ASSERT(pCh);
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_CHANNEL_OCCNO);
		m_nOccNo = nOccNo;
		m_pChannel = pCh;
	}
	~CScdChannel()
	{
		STLClearObject(&m_arrDevices);
		m_mapDevices.clear();
	}
public:
	void AddDev(CScdDevice* pDev, int32u nOccNo)
	{
		Q_ASSERT(pDev);
		Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo<MAX_DEVICE_OCCNO);

		if (pDev)
		{
			m_arrDevices.push_back(pDev);
			m_mapDevices.insert(std::make_pair(nOccNo,pDev));
		}	
	}
	std::vector<CScdDevice*>& GetDevArrs()
	{
		return m_arrDevices;
	}
	size_t GetDevSize()
	{
		return m_arrDevices.size();
	}
	CScdDevice* GetDeviceByOccNo(int32u nOccNo)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo<MAX_DEVICE_OCCNO);
		if (nOccNo==INVALID_OCCNO || nOccNo >MAX_DEVICE_OCCNO)
		{
			return nullptr;
		}
		auto i = m_mapDevices.find(nOccNo);
		if (i==m_mapDevices.end())
		{
			return nullptr;
		}
		else
		{
			return i->second;
		}
	}
public:
	int32u m_nOccNo;
private:
	CHANNEL* m_pChannel;
	std::vector<CScdDevice*> m_arrDevices;
	std::unordered_map<int32u, CScdDevice* >m_mapDevices;
};

class CScdChannelMgr
{
public:
	CScdChannelMgr()
	{

	}

	~CScdChannelMgr()
	{
		STLClearObject(&m_arrChannels);
		m_mapChannel.clear();
	}
public:
	void AddChannel(CScdChannel* pChannel)
	{
		Q_ASSERT(pChannel);
		if (pChannel)
		{
			m_arrChannels.push_back(pChannel);
			m_mapChannel.insert(std::make_pair(pChannel->m_nOccNo,pChannel));
		}
	}
	size_t GetChannelSize()
	{
		return m_arrChannels.size();
	}
	std::vector<CScdChannel* >& GetChananelArrs()
	{
		return m_arrChannels;
	}
	bool GetDevArrs(int32u nOccNo, std::vector<CScdDevice*>& arrDevices)
	{
		Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
		if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
		{
			return false;
		}

		CScdChannel* pChannel = GetChannelByOccNo(nOccNo);
		if (pChannel)
		{
			arrDevices = pChannel->GetDevArrs();
			return true;
		}		
		else
		{
			return false;
		}
	}

	CScdDevice* GetDevice(int32u nChannelOccNo, int32u nDevOccNo)
	{
		Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			return nullptr;
		}
		Q_ASSERT(nDevOccNo!=INVALID_OCCNO && nDevOccNo<=MAX_DEVICE_OCCNO);
		if (nDevOccNo==INVALID_OCCNO || nDevOccNo >MAX_DEVICE_OCCNO)
		{
			return nullptr;
		}
		CScdChannel* pChannel = GetChannelByOccNo(nChannelOccNo);
		if (pChannel)
		{
			CScdDevice* pDev=pChannel->GetDeviceByOccNo(nDevOccNo);
			if (pDev)
			{
				return pDev;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}
	CScdChannel* GetChannelByOccNo(int32u nOccNo)
	{
		Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
		if (nOccNo==INVALID_OCCNO || nOccNo >MAX_CHANNEL_OCCNO)
		{
			return nullptr;
		}
		auto i = m_mapChannel.find(nOccNo);
		if (i==m_mapChannel.end())
		{
			return nullptr;
		}
		else
		{
			return i->second;
		}
	}
public:
	std::vector<CScdChannel* > m_arrChannels;
	std::unordered_map<int32u, CScdChannel* >m_mapChannel;
};
#endif