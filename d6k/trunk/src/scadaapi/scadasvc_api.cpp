/*! @file scdsvcapi.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : scdsvcapi.cpp
文件实现功能 :
作者         :
版本         :
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   接口实现部分.
*  @author  LiJin
*  @version 1.0
*  @date    2016.12.21
*******************************************************************************/

#include "scadaapi/scdsvcapi.h"

#include "scadaapi.h"

#include <QDebug>

static CScadaApi s_ScdApi;

/*! \fn SCADA_API bool ConnectScada(const char * pszProjectName, const char * szAppName)
********************************************************************************************************* 
** \brief ConnectScada 
** \details 连接到实时数据库，
** \param pszProjectName 
** \param szAppName 
** \return SCADA_API bool 
** \author LiJin 
** \date 2017年1月14日 
** \note  所有的应用程序在启动之初，需要先调用这个接口
********************************************************************************************************/
SCADA_API bool ConnectScada(const char * pszProjectName, const char * szAppName)
{
	bool bRet=s_ScdApi.Initialize(pszProjectName,1);
	return true;
} 
/*! \fn SCADA_API bool DisconnectScada(const char * pszProjectName, const char * szAppName)
********************************************************************************************************* 
** \brief DisconnectScada 
** \details 关闭 实时数据库
** \param pszProjectName 
** \param szAppName 
** \return SCADA_API bool 
** \author LiJin 
** \date 2017年1月14日 
** \note 
********************************************************************************************************/
SCADA_API bool DisconnectScada(const char * pszProjectName, const char * szAppName)
{

	return true;
}  
/*! \fn SCADA_API bool IsScadaExit(unsigned int nTimeout)
********************************************************************************************************* 
** \brief IsScadaExit 
** \details 服务是否退出
** \param nTimeout 毫秒
** \return SCADA_API bool 
** \author LiJin 
** \date 2016年12月21日 
** \note 
********************************************************************************************************/
SCADA_API bool IsScadaExit(unsigned int nTimeout)
{

	return true;
}

SCADA_API void SetScadaExit(bool bExit)
{

}


/*! \fn SCADA_API bool AppSendAlarmMsg(const char *pszAppName, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm)
********************************************************************************************************* 
** \brief AppSendAlarmMsg 
** \details  应用程序的告警
** \param pszAppName 
** \param nAlarmType 
** \param pszAlarmTxt 
** \param pTm 
** \return SCADA_API bool 
** \author LiJin 
** \date 2017年1月14日 
** \note 
********************************************************************************************************/
SCADA_API bool AppSendAlarmMsg(const char *pszAppName, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm)
{
	return true;
}

/*! \fn SCADA_API bool RegisterRTData(struct HotCacheItemInfo *pItems, int32u nNum)
********************************************************************************************************* 
** \brief RegisterRTData 
** \details 注册静态缓存
** \param pItems 
** \param nNum 
** \return SCADA_API bool 
** \author LiJin 
** \date 2016年12月15日 
** \note 
********************************************************************************************************/
SCADA_API bool RegisterRTData(struct HotCacheItemInfo *pItems, int32u nNum)
{
	
	// 发邮件给scdsvc

	return true;
}
// 取消静态缓存
SCADA_API bool UnregisterRTData(struct HotCacheItemInfo *pItems, int32u nNum)
{
	return true;
}

SCADA_API bool GetRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData)
{
	Q_ASSERT(nIddType != IDD_NULL  && nIddType <= MAX_IDD);
	if (nIddType == IDD_NULL || nIddType > MAX_IDD)
	{
		return false;
	}
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	bool bRet=s_ScdApi.GetRTData(nIddType,nNodeOccNo,nOccNo,nFiledID,pRetData);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}
	return true;
}

SCADA_API bool PutRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nIddType != IDD_NULL  && nIddType <= MAX_IDD);
	if (nIddType == IDD_NULL || nIddType > MAX_IDD)
	{
		return false;
	}
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	bool bRet = s_ScdApi.PutRTData(nIddType, nNodeOccNo, nOccNo, nFiledID, pData, pExt, pSrc);
	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}
	return true;
}

SCADA_API bool GetOccNoByTagName(const char*pszTagName, int32u *pNodeOccNo, int32u *pIddType, int32u *pOccNo, int32u *pFiledID)
{
	Q_ASSERT(pszTagName && strlen(pszTagName) > 0);
	if (!pszTagName || strlen(pszTagName) == 0)
	{
		return false;
	}
	bool bRet=s_ScdApi.GetOccNoByTagName(pszTagName, pNodeOccNo, pIddType,pOccNo,pFiledID);
	if (!bRet)
	{
		return false;
	}
	return true;
}

 
/*@}*/


 

