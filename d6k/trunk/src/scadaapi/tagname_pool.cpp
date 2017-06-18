/*! @file tagname_pool.cpp
<PRE>
********************************************************************************
模块名       :  对本节点的所有tagname进行管理
文件名       :  tagname_pool.cpp
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


#include "tagname_pool.h"

#include "fesapi/fesdatadef.h"
#include "log/log.h" 
#include "fesapi/fesdatadef.h"
#include "fesapi/fes_magicmem.h"

#include <QDebug>
#include <string>


CTagNamePool::CTagNamePool()
{
	m_pMem = std::make_shared<CShareMem>();
	m_bInitialized = false;
}

CTagNamePool::~CTagNamePool()
{
	for (auto it : m_arrMapString2ID)
	{
		it.second.clear();
	}
	m_arrMapString2ID.clear();
}
/*! \fn int32u CTagNamePool::GetOccNoByTagName(const char * szTagName, int32u &nDataType)
********************************************************************************************************* 
** \brief CTagNamePool::GetOccNoByTagName 
** \details 通过TagName字符串获取到 对应的OccNo以及类型（IDD_TYPE）
** \param szTagName 
** \param nDataType  返回值IDD_TYPE:AI\DI\...
** \return int32u 
** \author LiJin 
** \date 2016年9月23日 
** \note 依次查找每个表，找到立即返回，因此，小表尽量在前面。
********************************************************************************************************/
int32u CTagNamePool::GetOccNoByTagName(const char * szTagName, int32u &nDataType) const
{
	Q_ASSERT(szTagName);
	if (szTagName == nullptr)
		return INVALID_OCCNO;
	Q_ASSERT(strlen(szTagName));
	if (strlen(szTagName) <= 0)
		return INVALID_OCCNO;

	for (auto it :  m_arrMapString2ID)
	{
		auto it_find = it.second.find(szTagName);
		if (it_find == it.second.end())
		{//
			nDataType = it.first;
			return it_find->second;
		}
	}
	nDataType = IDD_NULL;
	return INVALID_OCCNO;
}
/*! \fn bool CTagNamePool::GetTagNameByOccNo(int32u& nOccNo, std::string szTagName)
********************************************************************************************************* 
** \brief CTagNamePool::GetTagNameByOccNo 
** \details 根据OccNo 获取对应的描述
** \param nOccNo 
** \param szTagName 
** \return bool 
** \author xingzhibing
** \date 2016年10月24日 
** \note 
********************************************************************************************************/
bool CTagNamePool::GetTagNameByOccNo(int32u nDataType, int32u nOccNo, std::string& szTagName)const
{
	Q_ASSERT(nDataType <= MAX_IDD && nDataType > IDD_NULL);
	if (nDataType == IDD_NULL || nDataType > MAX_IDD)
	{
		return false;
	}
	szTagName.empty();

	Q_ASSERT(nOccNo > INVALID_OCCNO && nOccNo <= MAX_OCCNO);

	if (nOccNo <= 0 || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	// 注意这个效率比较低下，--循环遍历
	/*for (auto it : m_arrMapString2ID)
	{
		if (it.first == nDataType)
		{
			for (auto j : it.second)
			{
				if (j.second == nOccNo)
				{
					szTagName = j.second;

					return true;
				}
			}
		}
	}*/
	for (auto it : m_arrMapID2String)
	{
		if (it.first == nDataType)
		{
			auto it_find = it.second.find(nOccNo);
			if (it_find == it.second.end())
			{			 
				szTagName = it_find->second;
				return true;
			}
		}
	}

	return false;
} 

size_t CTagNamePool::GetEstimateSize()
{
	return m_nEstimateSize;
}

/*! \fn bool CTagNamePool::Initialize(const char* pAddr, int32u nMode)
********************************************************************************************************* 
** \brief CTagNamePool::Initialize 
** \details tagname 初始化
** \param pszProjName 
** \param nMode 
** \return bool 
** \author xingzhibing
** \date 2016年10月25日 
** \note 
********************************************************************************************************/
bool CTagNamePool::Initialize(unsigned char* pAddr, int32u nMode)
{
	Q_ASSERT(pAddr);
	if (pAddr == nullptr)
		return false;
	
	if (m_bInitialized == true)
	{
		return true;
	}
	QString szLog;

	m_pMagicMem = reinterpret_cast<HEAD_MEM*>(pAddr);

	Q_ASSERT(m_pMagicMem->MagicHead1 == MAGIC_HEAD);
	Q_ASSERT(m_pMagicMem->MagicHead2 == MAGIC_HEAD);

//	Q_ASSERT(m_pMagicMem->NodeCount > 0 && m_pMagicMem->NodeCount <= MAX_NODE_COUNT);
	
	/*if (m_pMagicMem->NodeCount == 0 || m_pMagicMem->NodeCount > MAX_NODE_COUNT)
	{
		m_pMem->UnLock();

		szLog = QString(QObject::tr("The node's count [ %1 ] in memory db is wrong ")).arg(m_pMagicMem->NodeCount);
		LogMsg(szLog.toStdString().c_str(), 0);
		return false;
	}*/

	//m_nNodeCount = m_pMagicMem->NodeCount;
	m_nChannelCount = m_pMagicMem->ChannelCount;
	m_nDeviceCount = m_pMagicMem->DeviceCount;
	//m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	//m_nAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;
	//m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	//m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;

	m_nAinCount = m_pMagicMem->AinCount;
	m_nDinCount = m_pMagicMem->DinCount;
	m_nAoutCount = m_pMagicMem->AoutCount;
	m_nDoutCount = m_pMagicMem->DoutCount;

	m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	m_nAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;
	m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;
	m_nLinerCount = m_pMagicMem->LinearCount;
	m_nNonLinerCount = m_pMagicMem->NonLinearCount;
	m_nUserVariableCount = m_pMagicMem->UserVariableCount;
	m_nSystemVariableCount = m_pMagicMem->SystemVariableCount;

	m_nCurrentNodeOccNo = m_pMagicMem->MyNodeOccNo;

	unsigned char *pHead = reinterpret_cast<unsigned char*>(pAddr + sizeof(HEAD_MEM));

	size_t nRet = 0;

	int32u i = 0;
	
	//创建通道表
	nRet = CreateTagNamePool(pHead, IDD_CHANNEL, m_pMagicMem->ChannelCount, MAX_CHANNEL_OCCNO);
	pHead += nRet;

	//创建设备表
	nRet = CreateTagNamePool(pHead, IDD_DEVICE, m_pMagicMem->DeviceCount, MAX_DEVICE_OCCNO);
	pHead += nRet;

	//创建AIN
	nRet = CreateTagNamePool(pHead, IDD_AIN, m_pMagicMem->AinCount, MAX_OCCNO);
	pHead += nRet;

	//创建DIN
	nRet = CreateTagNamePool(pHead, IDD_DIN, m_pMagicMem->DinCount, MAX_OCCNO);
	pHead += nRet;

	//创建AOUT
	nRet = CreateTagNamePool(pHead, IDD_AOUT, m_pMagicMem->AoutCount, MAX_OCCNO);
	pHead += nRet;

	//创建DOUT
	nRet = CreateTagNamePool(pHead, IDD_DOUT, m_pMagicMem->DoutCount, MAX_OCCNO);
	pHead += nRet;

	//ainalarm
	nRet = CreateTagNamePool(pHead, IDD_AINALARM, m_pMagicMem->AinAlarmCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_AINALARMLIMIT, m_pMagicMem->AinAlarmLimitCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_DINALARM, m_pMagicMem->DinAlarmCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_DINALARMLIMIT, m_pMagicMem->DinAlarmLimitCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_TRANSLINEAR, m_pMagicMem->LinearCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_TRANSNONLINEAR, m_pMagicMem->NonLinearCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_SYSVAR, m_pMagicMem->SystemVariableCount, MAX_OCCNO);
	pHead += nRet;

	nRet = CreateTagNamePool(pHead, IDD_USERVAR, m_pMagicMem->UserVariableCount, MAX_OCCNO);
	pHead += nRet;


	m_nEstimateSize =sizeof HEAD_MEM +  sizeof TAG_OCCNO *(m_nChannelCount+ m_nDeviceCount +
										 m_nAinCount +m_nDinCount +
										 m_nAoutCount+m_nDoutCount +
										 m_nAinAlarmCount +m_nAinAlarmLimitCount+
										 m_nDinAlarmCount+m_nDinAlarmLimitCount +
										 m_nLinerCount+m_nNonLinerCount+
										 m_nUserVariableCount + m_nSystemVariableCount);

	return true;
}
/*! \fn void CTagNamePool::LogMsg(const char * pszText, int nLevel)
********************************************************************************************************* 
** \brief CTagNamePool::LogMsg 
** \details log 日志
** \param pszText 
** \param nLevel 
** \return void 
** \author xingzhibing
** \date 2016年10月25日 
** \note 
********************************************************************************************************/
void CTagNamePool::LogMsg(const char * pszText, int nLevel)
{
	::LogMsg("FESAPI", pszText, nLevel, nullptr);
}
/*! \fn size_t CTagNamePool::CreateNodeTagNamePool(unsigned char* pHead)
********************************************************************************************************* 
** \brief CTagNamePool::CreateNodeTagNamePool 
** \details 节点tagname
** \param pHead 
** \return size_t 
** \author xingzhibing
** \date 2016年10月25日 
** \note 
********************************************************************************************************/
size_t CTagNamePool::CreateTagNamePool(unsigned char* pHead, int32u nDataType, int32u nCount, int32u maxNum)
{
	Q_ASSERT(pHead);
	if (pHead==Q_NULLPTR || nCount <= 0)
	{	
		return 0;
	}

	std::unordered_map< std::string, int32u> mapRel;
	mapRel.reserve(nCount);

	std::unordered_map<int32u,std::string> mapRelID2String;
	mapRelID2String.reserve(nCount);

	TAG_OCCNO * m_pData = reinterpret_cast<TAG_OCCNO *>(pHead);

	QString szLog;
	for (int32u i = 0; i < nCount; ++i)
	{
		Q_ASSERT(m_pData[i].OccNo != INVALID_OCCNO && m_pData[i].OccNo <= maxNum);
		if (m_pData[i].OccNo == INVALID_OCCNO || m_pData[i].OccNo > maxNum)
		{
			szLog = QString(QObject::tr("[%1]  OccNo [ %2 ] in memory db is wrong ")).arg(nDataType).arg(m_pData[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		Q_ASSERT(strlen(m_pData[i].TagName) > 0 && strlen(m_pData[i].TagName) <= MAX_NAME_LENGTH);
		if (strlen(m_pData[i].TagName) <= 0 ||strlen(m_pData[i].TagName) > MAX_NAME_LENGTH)
		{
			szLog = QString(QObject::tr("[%1]  OccNo [ %2 ] tagname's length is wrong ")).arg(nDataType).arg(m_pData[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		auto ret = mapRel.insert(std::make_pair(m_pData[i].TagName, m_pData[i].OccNo));
		Q_ASSERT(ret.second == true);

		auto ret2 = mapRelID2String.insert(std::make_pair(m_pData[i].OccNo,m_pData[i].TagName));
		Q_ASSERT(ret2.second == true);
	}

	m_arrMapString2ID.emplace_back(std::make_pair(nDataType, mapRel));

	m_arrMapID2String.emplace_back(std::make_pair(nDataType,mapRelID2String));

	return sizeof(TAG_OCCNO) * nCount;
}

void CTagNamePool::Run()
{

}

void CTagNamePool::Shutdown()
{

}


bool CTagNamePool::GetOccNoByTagName(const char*pszTagName, int32u &nIddType, int32u &nOccNo, int32u &nFiledID)
{
	Q_ASSERT(pszTagName && strlen(pszTagName)>0);
	if (!pszTagName || strlen(pszTagName)==0)
	{
		return false;
	}

	for (auto it:m_arrMapString2ID)
	{
		auto it_find = it.second.find(pszTagName);

		if (it_find!=it.second.end())
		{
			nIddType = it.first;
			nOccNo =  it_find->second;
			return true;
		}
	}

	nIddType = IDD_NULL;
	nOccNo = INVALID_OCCNO;

	return false;
}


/** @}*/

