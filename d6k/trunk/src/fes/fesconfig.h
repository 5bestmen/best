#pragma once

#include "fesapi/fesdatadef.h"
//#include "stl_util-inl.h"


#include <map>
#include <vector>
#include <memory>


struct CHANNEL;
struct DEVICE;
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;

typedef std::shared_ptr<FesInfo> FesInfo_PTR;
typedef std::shared_ptr<NODE>    NODE_PTR;
typedef std::shared_ptr<CHANNEL> CHANNEL_PTR;
typedef std::shared_ptr<DEVICE>  DEVICE_PTR;
typedef std::shared_ptr<AIN>     AIN_PTR;
typedef std::shared_ptr<DIN>     DIN_PTR;
typedef std::shared_ptr<AOUT>    AOUT_PTR;
typedef std::shared_ptr<DOUT>    DOUT_PTR;


class CChannel;
class CDevice;

typedef std::shared_ptr<CChannel> CChannelPtr;
typedef std::shared_ptr<CDevice>  CDevicePtr;


class CChannel
{
public:
	CChannel(CHANNEL_PTR pChannel);
	~CChannel();
public:
	void AddDev(CDevicePtr pDev);

	CDevicePtr CreateDevice(DEVICE_PTR pDev);

	size_t  GetDevCount()
	{
		return m_VecDev.size();
	}

	CHANNEL_PTR GetChannelPtr()
	{
		return m_ChannelPtr;
	}

	CDevicePtr GetDevice(int nIndex)
	{
		if (nIndex >=  m_VecDev.size())
		{
			return NULL;
		}
		else
		{
			return m_VecDev.at(nIndex);
		}
	}
	void AddAin(AIN_PTR pAin)
	{
		m_arrAins.push_back(pAin);
	}
	void AddDin(DIN_PTR pDin)
	{
		m_arrDins.push_back(pDin);
	}
	void AddAout(AOUT_PTR pAout)
	{
		m_arrAouts.push_back(pAout);
	}
	void AddDout(DOUT_PTR pDout)
	{
		m_arrDouts.push_back(pDout);
	}


	size_t SaveToMem(unsigned char *pMem);

	size_t GetChannelInfoLen();
	size_t GetChannelInfoExtraLen();

	int32u GetOccNo()
	{
		if (m_ChannelPtr)
		{
			return m_ChannelPtr->OccNo;
		}
		return INVALID_OCCNO;
	}

	void SortChannelFunction();

	CHANNEL_PTR GetChannelInfo()
	{
		return m_ChannelPtr;
	}
private:
	std::vector< CDevicePtr > m_VecDev;

	//! 该通道所属的AIN
	std::vector < std::shared_ptr<AIN> > m_arrAins;
	std::vector < std::shared_ptr<DIN> > m_arrDins;
	std::vector < std::shared_ptr<AOUT> > m_arrAouts;
	std::vector < std::shared_ptr<DOUT> > m_arrDouts;


private:
	CHANNEL_PTR m_ChannelPtr;
};

//关系表
class CChannelMgr
{
public:
	CChannelMgr()
	{

	}

	~CChannelMgr()
	{
		m_ChannelVec.clear();
		m_DeviceVec.clear();
	}
public:
	void AddChannel(CChannelPtr  pChannel);

	//创建通道
	CChannelPtr CreateChannel(CHANNEL_PTR pChannel);
	//创建设备
	CDevicePtr CreateDevice(CChannelPtr pChannelPtr, DEVICE_PTR pDevPtr);

	size_t GetChannelCount()
	{
		return m_ChannelVec.size();
	}
	CChannelPtr GetChannel(int nIndex)
	{
		if (nIndex>m_ChannelVec.size())
		{
			return nullptr;
		}
		else
		{
			return m_ChannelVec.at(nIndex);
		}
	}
	size_t GetDevCount()
	{
		return m_DeviceVec.size();
	}

	CDevicePtr GetDevice(int nIndex)
	{
		if (nIndex > m_DeviceVec.size())
		{
			return nullptr;
		}
		else
		{
			return m_DeviceVec.at(nIndex);
		}
	}
	
	void SortMgrFunction();
private:
	std::vector<CChannelPtr> m_ChannelVec;
	std::vector<CDevicePtr>  m_DeviceVec;
};


class CDevice
{
public:
	CDevice(DEVICE_PTR pDev);
	~CDevice();
public:
	size_t SaveToMem(unsigned char *pMem);
	void AddAin(AIN_PTR pAin);
	void AddDin(DIN_PTR pDin);
	void AddAout(AOUT_PTR pAout);
	void AddDout(DOUT_PTR pDout);

	AIN_PTR GetAIN(int nIndex)
	{
		if (nIndex > m_Ains.size())
		{
			return nullptr;
		}
		else
		{
			return m_Ains.at(nIndex);
		}
	}

	DIN_PTR GetDin(int nIndex)
	{
		if (nIndex>m_Dins.size())
		{
			return nullptr;
		}
		else
		{
			return m_Dins.at(nIndex);
		}
	}

	AOUT_PTR GetAout(int nIndex)
	{
		if (nIndex >m_Aouts.size())
		{
			return nullptr;
		}
		else
		{
			return m_Aouts.at(nIndex);
		}
	}

	DOUT_PTR GetDout(int nIndex)
	{
		if (nIndex >m_Douts.size())
		{
			return nullptr;
		}
		else
		{
			return m_Douts.at(nIndex);
		}
	}
	size_t GetAinCount() 
	{
		return m_Ains.size();
	}

	size_t GetDinCount()
	{
		return m_Dins.size();
	}

	size_t GetAoutCount()
	{
		return m_Aouts.size();
	}

	size_t GetDoutCount()
	{
		return m_Douts.size();
	}

	DEVICE_PTR  GetDevicePtr()
	{
		return m_DevPtr;
	}

	size_t GetDevInfoExtraLen();

	size_t GetDevLen();

	int32u GetOccNo()
	{
		if (m_DevPtr)
		{
			return m_DevPtr->OccNo;
		}
		return INVALID_OCCNO;
	}
	void SortDevFunction();
private:
	std::vector<AIN_PTR > m_Ains;
	std::vector<DIN_PTR > m_Dins;
	std::vector<AOUT_PTR> m_Aouts;
	std::vector<DOUT_PTR> m_Douts;

private:
	DEVICE_PTR m_DevPtr;
};