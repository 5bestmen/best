/*! @file base_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : base_widget.cpp
文件实现功能 : 基本的动态定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  基本的动态定义
*  @author LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/


#include "base_widget.h"
#include "base_dync.h"
 

CBaseDyncData::CBaseDyncData()
{
}

CBaseDyncData::~CBaseDyncData()
{
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
}

/** @}*/
