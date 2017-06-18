/*! @file base_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  base_widget.cpp
文件实现功能 :  基本的动态定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的动态定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/


#include "base_widget.h"
#include "base_dync.h"

#include "fesapi/fesdatadef.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif

#include "../scdpub/variant_data.h"
 

CBaseDyncData::CBaseDyncData()
{
	m_pTagInfo = std::make_shared<struct TAGITEM>();
	m_bEnable = false;
}

CBaseDyncData::~CBaseDyncData()
{

}

void CBaseDyncData::CopyBaseData(CBaseDyncData *src)
{
	//动态属性赋值
	Q_ASSERT(src);
	if (src == nullptr)
	{
		return;
	}
	m_bEnable = src->m_bEnable;
	m_nDyncType = src->m_nDyncType;
	m_szTagName = src->m_szTagName;
}

const CBaseDyncData& CBaseDyncData::operator=(const CBaseDyncData& src)
{
	//动态属性赋值
	m_bEnable = src.m_bEnable;
	m_nDyncType = src.m_nDyncType;
	m_szTagName = src.m_szTagName;

	//m_pTagInfo  TODO
	return *this;
}

/*! \fn  bool CBaseDyncData::IsDyncTypeEqual(CBaseDyncData *pData) const
*********************************************************************************************************
** \brief   CBaseDyncData::IsDyncTypeEqual
** \details 比较两个动态连接是否是一样的类型
** \param   pData
** \return bool
** \author LiJin
** \date  2010年12月20日
** \note
********************************************************************************************************/
bool CBaseDyncData::IsDyncTypeEqual(CBaseDyncData *pData) const
{
	Q_ASSERT(pData);
	if (pData == nullptr)
		return false;
	if (pData->m_nDyncType == this->m_nDyncType)
	{
		return true;
	}
	return false;
}
/*! \fn  bool CBaseDyncData::CheckDyncData( )
*********************************************************************************************************
** \brief   CBaseDyncData::CheckDyncData
** \details 检查动态连接参数是否合法
** \return bool
** \author LiJin
** \date  2010年12月20日
** \note
********************************************************************************************************/
bool CBaseDyncData::CheckDyncData()
{
	if (m_szTagName.empty() == true)
		return false;
	return true;
}

bool CBaseDyncData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	return false;
}

void CBaseDyncData::PrepareWidgetDync(CBaseWidget *pWidget)
{
	Q_UNUSED(pWidget); 

	GetOccNoByTagName( );
}

void CBaseDyncData::LogMsg(const char *szLogTxt, int nLevel)
{
#ifdef HMI_SDK_LIB
	::LogMsg("VMCS", szLogTxt, nLevel, nullptr);
#else
	GetModuleApi()->LogMsg(szLogTxt, nLevel);
#endif
}
/*! \fn void CBaseDyncData::GetOccNoByTagName( )
********************************************************************************************************* 
** \brief CBaseDyncData::GetOccNoByTagName 
** \details 根据tagname获取排行号等信息
** \return void 
** \author LiJin 
** \date 2017年2月24日 
** \note 
********************************************************************************************************/
void CBaseDyncData::GetOccNoByTagName( )
{
#ifdef HMI_SDK_LIB
	Q_ASSERT(m_pTagInfo);
	if (m_pTagInfo)
	{
		QString szLog;
		bool bRet = false;
		if (m_szTagName.empty() == true)
		{
			szLog = QString(QObject::tr("Widget's dync data is error..."));
			LogMsg(szLog.toStdString().c_str(), 0);
		}
		else
		{
			bRet = ::GetOccNoByTagName(m_szTagName.c_str(), &m_pTagInfo->NodeOccNo, &m_pTagInfo->IddType, &m_pTagInfo->TagOccNo, &m_pTagInfo->FiledID);
			if (bRet == false)
			{
				szLog = QString(QObject::tr("Convert [%1] failed...")).arg(m_szTagName.c_str());
				LogMsg(szLog.toStdString().c_str(), 0);
			}
		}
	}
#endif	
}

bool CBaseDyncData::GetRealTimeValue(std::shared_ptr<struct  TAGITEM > pInfo, CVariant *pValue)
{
	Q_ASSERT(pInfo && pValue);
	if (pInfo == nullptr || pValue == nullptr)
		return false;

	bool bRet = false;

	if (pInfo->IddType == IDD_NULL || pInfo->IddType >= MAX_IDD)
	{
		return false;
	}

	if (pInfo->NodeOccNo == INVALID_OCCNO || pInfo->NodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	if (pInfo->TagOccNo == INVALID_OCCNO || pInfo->TagOccNo > MAX_OCCNO)
	{
		return false;
	}

#ifdef HMI_SDK_LIB	
	bRet = ::GetRTData(pInfo->IddType, pInfo->NodeOccNo, pInfo->TagOccNo, pInfo->FiledID,pValue);
#else





#endif
	return bRet;
}

void CBaseDyncData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return  ;

}

bool CBaseDyncData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}


/** @}*/
