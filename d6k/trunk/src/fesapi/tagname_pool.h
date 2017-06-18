/*! @file tagname.h
<PRE>
********************************************************************************
ģ����       :  �Ա��ڵ������tagname���й���
�ļ���       :  tagname.h
�ļ�ʵ�ֹ��� :  
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  tagname�����ʹ��ڹ����ڴ���
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author LiJin
*  @version 1.0
*  @date    2016.09.23
*/

#ifndef FES_TAG_NAME_H
#define FES_TAG_NAME_H
#pragma once

#include "datatypes.h" 
#include "../../fespub/sharemem.h"
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
	bool Initialize(const char* pszProjName, int32u nMode);
	void Run();
	void Shutdown();
public: 
	// ��ȡoccno
	int32u GetOccNoByTagName(const char * szTagName, int32u &nDataType)const;
	bool   GetTagNameByOccNo(int32u nDataType,int32u nOccNo, std::string& szTagName)const;
protected:
	size_t BuildDB(unsigned char* pData);
	size_t CreateTagNamePool(unsigned char* pHead,int32u nDataType,int32u nCount,int32u maxNum);
	void   LogMsg(const char * pszText, int nLevel);
private:
	// ���������ͣ�IDD_TYPE����hash��(������tagname��occno )
	std::vector<std::pair<int32u,  std::unordered_map< std::string, int32u> > > m_arrMapString2ID;
	// ���������ͣ�IDD_TYPE����hash��(������occno,tagname  )
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
	unsigned int m_nSystemVarCount;
	unsigned int m_nUserVarCount;

	bool m_bInitialized;
	//! ��ν���ڴ����ݿ�
	std::shared_ptr<CShareMem> m_pMem;

	HEAD_MEM* m_pMagicMem;
};

/** @}*/

#endif // FES_TAG_NAME_H
