#pragma once
#include "mem_db.h"
#include "fesconfig.h"
#include "scd_data.h"
#include <unordered_map>
#include <QObject>
#include <QString>
#include <string>

struct  HEAD_MEM;

class CFesDBR//:public CMemDB
{
public:
	CFesDBR()
	{
		m_pMgr = std::make_shared<CScdChannelMgr>();
	}
	~CFesDBR(){}
public:
	//构建内存关系
	bool LoadFesMemDB(const char* pAddr,int nSize);
	//获取内存关系
	std::shared_ptr<CScdChannelMgr >& GetScdMemRel()
	{
		return m_pMgr;
	}
	std::vector<CHANNEL*>& GetChannelArrs()
	{
		return m_arrChannels;
	}
	CHANNEL* GetChannelByOccNo(int32u  nOccNo);
	DEVICE* GetDeviceByOccNo(int32u nOccNo);
	AIN* GetAinByOccNo(int32u nOccNo);
	DIN* GetDinByOccNo(int32u nOccNo);
	AOUT * GetAoutByOccNo(int32u nOccNo);
	DOUT* GetDoutByOccNo(int32u nOccNo);
protected:
	size_t CreateChannel(unsigned char* pAddr);
	size_t CreateDevice(unsigned char* pAddr);
	size_t CreateAin(unsigned char* pAddr);
	size_t CreateDin(unsigned char* pAddr);
	size_t CreateAout(unsigned char* pAddr);
	size_t CreateDout(unsigned char* pAddr);
protected:
	void LogMsg(const char * pszText, int nLevel);
private:
	//关系表
	std::shared_ptr<CScdChannelMgr > m_pMgr;

	std::string  m_szProjName;

	HEAD_MEM* m_pMagicMem;

	CHANNEL *m_pChannels;
	unsigned int m_nChannelCount;
	std::vector<CHANNEL*> m_arrChannels;  //! 记住地址，方便遍历
	std::unordered_map<int32u, CHANNEL*> m_mapChannels;

	DEVICE *m_pDevices;
	unsigned int m_nDeviceCount;
	std::vector<DEVICE*> m_arrDevices;
	std::unordered_map<int32u, DEVICE*> m_mapDevices;
	
	AIN *m_pAins;
	unsigned int m_nAinCount;
	std::vector<AIN*> m_arrAins;
	std::unordered_map<int32u, AIN*> m_mapAins;

	DIN *m_pDins;
	unsigned int m_nDinCount;
	std::vector<DIN*> m_arrDins;
	std::unordered_map<int32u, DIN*> m_mapDins;

	DOUT *m_pDouts;
	unsigned int m_nDoutCount;
	std::vector<DOUT*> m_arrDouts;
	std::unordered_map<int32u, DOUT*> m_mapDouts;

	AOUT *m_pAouts;
	unsigned int m_nAoutCount;
	std::vector<AOUT*> m_arrAouts;
	std::unordered_map<int32u, AOUT*> m_mapAouts;
};

