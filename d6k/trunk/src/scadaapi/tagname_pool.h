/*! @file tagname.h
<PRE>
********************************************************************************
模块名       :  对本节点的所有tagname进行管理
文件名       :  tagname.h
文件实现功能 :  
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  tagname按类型存于共享内存中
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author LiJin
*  @version 1.0
*  @date    2016.09.23
*/

#ifndef SCD_TAG_NAME_H
#define SCD_TAG_NAME_H
#pragma once

#include "datatypes.h" 
#include "../../scdpub/sharemem.h"
#include <unordered_map>
#include <memory>
#include <vector>

struct NODE;
struct CHANNEL;
struct DEVICE;
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;
struct TRANSFORM_LINEAR;
struct TRANSFORM_NONLINEAR;
struct TRANSFORM;
struct ALARM;
struct AIN_ALARM;
struct DIN_ALARM;

struct HEAD_MEM;
class CNode;
class CFesChannel;
class CFesDevice;
class CFesAin;
class CFesDin;
class CFesAout;
class CFesDout;
class CAinAlarm;
class CDinAlarm;

struct TAG_OCCNO;

typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;

class CTagNamePool
{
public:
	CTagNamePool();	
	~CTagNamePool();
public:
	bool Initialize(unsigned char* pAddr, int32u nMode);
	void Run();
	void Shutdown();
public: 
	bool GetOccNoByTagName(const char*pszTagName, int32u &nIddType, int32u &nOccNo, int32u &nFiledID);

	// 获取occno
	int32u GetOccNoByTagName(const char * szTagName, int32u &nDataType)const;
	bool   GetTagNameByOccNo(int32u nDataType,int32u nOccNo, std::string& szTagName)const;
	//mem size
	size_t GetEstimateSize();
	int32u GetCurrentNodeOccNo()
	{
		return m_nCurrentNodeOccNo;
	}
protected:
	size_t BuildDB(unsigned char* pData);
	size_t CreateTagNamePool(unsigned char* pHead,int32u nDataType,int32u nCount,int32u maxNum);
	void   LogMsg(const char * pszText, int nLevel);
private:
	// 参数：类型（IDD_TYPE），hash表(参数：tagname，occno )
	std::vector<std::pair<int32u,  std::unordered_map< std::string, int32u> > > m_arrMapString2ID;
	// 参数：类型（IDD_TYPE），hash表(参数：occno,tagname  )
	std::vector<std::pair<int32u, std::unordered_map< int32u,std::string > > > m_arrMapID2String;

	unsigned int m_nNodeCount;
	unsigned int m_nChannelCount;
	unsigned int m_nDeviceCount;
	unsigned int m_nAinCount;
	unsigned int m_nDinCount;
	unsigned int m_nAoutCount;
	unsigned int m_nDoutCount;
	unsigned int m_nAinAlarmCount;
	unsigned int m_nAinAlarmLimitCount;
	unsigned int m_nDinAlarmLimitCount;
	unsigned int m_nDinAlarmCount;
	unsigned int m_nLinerCount;
	unsigned int m_nNonLinerCount;
	unsigned int m_nSystemVariableCount;
	unsigned int m_nUserVariableCount;

	bool m_bInitialized;
	//! 所谓的内存数据库
	std::shared_ptr<CShareMem> m_pMem;

	HEAD_MEM* m_pMagicMem;

	size_t m_nEstimateSize;

	int32u m_nCurrentNodeOccNo;
};

/** @}*/

#endif // SCD_TAG_NAME_H
