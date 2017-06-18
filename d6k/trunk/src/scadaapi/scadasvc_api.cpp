/*! @file scdsvcapi.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : scdsvcapi.cpp
�ļ�ʵ�ֹ��� :
����         :
�汾         :
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ӿ�ʵ�ֲ���.
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
** \details ���ӵ�ʵʱ���ݿ⣬
** \param pszProjectName 
** \param szAppName 
** \return SCADA_API bool 
** \author LiJin 
** \date 2017��1��14�� 
** \note  ���е�Ӧ�ó���������֮������Ҫ�ȵ�������ӿ�
********************************************************************************************************/
SCADA_API bool ConnectScada(const char * pszProjectName, const char * szAppName)
{
	bool bRet=s_ScdApi.Initialize(pszProjectName,1);
	return true;
} 
/*! \fn SCADA_API bool DisconnectScada(const char * pszProjectName, const char * szAppName)
********************************************************************************************************* 
** \brief DisconnectScada 
** \details �ر� ʵʱ���ݿ�
** \param pszProjectName 
** \param szAppName 
** \return SCADA_API bool 
** \author LiJin 
** \date 2017��1��14�� 
** \note 
********************************************************************************************************/
SCADA_API bool DisconnectScada(const char * pszProjectName, const char * szAppName)
{

	return true;
}  
/*! \fn SCADA_API bool IsScadaExit(unsigned int nTimeout)
********************************************************************************************************* 
** \brief IsScadaExit 
** \details �����Ƿ��˳�
** \param nTimeout ����
** \return SCADA_API bool 
** \author LiJin 
** \date 2016��12��21�� 
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
** \details  Ӧ�ó���ĸ澯
** \param pszAppName 
** \param nAlarmType 
** \param pszAlarmTxt 
** \param pTm 
** \return SCADA_API bool 
** \author LiJin 
** \date 2017��1��14�� 
** \note 
********************************************************************************************************/
SCADA_API bool AppSendAlarmMsg(const char *pszAppName, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm)
{
	return true;
}

/*! \fn SCADA_API bool RegisterRTData(struct HotCacheItemInfo *pItems, int32u nNum)
********************************************************************************************************* 
** \brief RegisterRTData 
** \details ע�ᾲ̬����
** \param pItems 
** \param nNum 
** \return SCADA_API bool 
** \author LiJin 
** \date 2016��12��15�� 
** \note 
********************************************************************************************************/
SCADA_API bool RegisterRTData(struct HotCacheItemInfo *pItems, int32u nNum)
{
	
	// ���ʼ���scdsvc

	return true;
}
// ȡ����̬����
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


 

