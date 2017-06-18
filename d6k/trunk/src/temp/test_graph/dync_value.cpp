/*! @file dync_value.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_value.cpp
文件实现功能 :  动态数值定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  动态数值定义
*  @author LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/


#include "base_widget.h"
#include "dync_value.h" 

#include "stl_util-inl.h"
 

CDyncValueData::CDyncValueData() :CBaseDyncData()
{
	m_bEnable = false;
	m_nDyncType = DYNC_VALUE;
	m_nFormat = X;
}

CDyncValueData::~CDyncValueData()
{

}

const CDyncValueData& CDyncValueData::operator=(const CDyncValueData& src)
{
	if (this != &src)
	{
		m_bEnable = src.m_bEnable;
		m_nDyncType = src.m_nDyncType;
		m_szTagName = src.m_szTagName;
		m_nFormat = src.m_nFormat;
	}
	return *this;
}

CBaseDyncData *CDyncValueData::CloneDyncData()
{
	CDyncValueData *pNewDync = new CDyncValueData;
	*pNewDync = *this;
	return dynamic_cast<CDyncValueData*> (pNewDync);
}

bool CDyncValueData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	return true;
}
#if 0
bool CDyncValueData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	ASSERT(pWidget);
	if (pWidget == NULL)
		return false;
	ASSERT(m_bEnable);
	if (m_bEnable == FALSE)
		return false;

	if (m_szTagName.IsEmpty())
		return false;

	// 读取实时数据
	UNION_VALUE rtval;
	double dValue = 0.0f;

	bool bRtn = GetRTVarValue(m_szTagName, rtval);
	if (bRtn == false)
		return false;

	if (rtval.dt == DT_STRING)
	{// 如果是字符串 变量不支持
		return false;
	}
	else
	{
		Base::CVariant cvt_val(rtval);
		double dblValue = cvt_val.GetDoubleValue();

		CString szValue;
		switch (m_nFormat)
		{
		case X:
			szValue.Format(_T("%d"), static_cast <long> (dblValue));
			break;
		case XX:
			szValue.Format(_T("%02d"), static_cast <long> (dblValue));
			break;
		case XXX:
			szValue.Format(_T("%03d"), static_cast <long> (dblValue));
			break;
		case XXXX:
			szValue.Format(_T("%04d"), static_cast <long> (dblValue));
			break;
		case XXXXX:
			szValue.Format(_T("%05d"), static_cast <long> (dblValue));
			break;
		case X_X:
			szValue.Format(_T("%.1f"), (dblValue));
			break;
		case X_XX:
			szValue.Format(_T("%.2f"), (dblValue));
			break;
		case X_XXX:
			szValue.Format(_T("%.3f"), (dblValue));
			break;
		case X_XXXX:
			szValue.Format(_T("%.4f"), (dblValue));
			break;
		case X_XXXXX:
			szValue.Format(_T("%.5f"), (dblValue));
			break;
		default:
		{
			szValue.Format(_T("%f"), (dblValue));
			break;
		}
		}
		pWidget->SetText(szValue);
		return true;
	}

	return false;
}

/*! \fn void CDyncValueData::SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const
*********************************************************************************************************
** \brief 保存
** \details
** \param pDyncPtr
** \return void
** \author LiJin
** \date 2010年10月2日
** \note
********************************************************************************************************/
void CDyncValueData::SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const
{
	ASSERT(!pDyncPtr->IsNull());
	if (pDyncPtr->IsNull())
		return;
	ASSERT(m_bEnable);
	if (m_bEnable == FALSE)
		return;
	ASSERT(!m_szTagName.IsEmpty());
	if (m_szTagName.IsEmpty())
		return;

	using namespace JWXml;

	__super::SaveXml(pDyncPtr);

	CXmlNodePtr pSubNode = pDyncPtr->NewChild(_T("DYNC_DATA"));
	ASSERT(!pSubNode->IsNull());
	if (!pSubNode->IsNull())
	{
		pSubNode->SetAttribute(_T("FORMAT"), static_cast <DWORD> (m_nFormat));
	}
}

bool CDyncValueData::LoadXml(JWXml::CXmlNodePtr  pDyncPtr)
{
	using namespace JWXml;

	m_bEnable = FALSE;

	bool bRet = __super::LoadXml(pDyncPtr);
	if (bRet == false)
		return false;

	ASSERT(m_nDyncType == DYNC_VALUE);
	if (m_nDyncType != DYNC_VALUE)
		return false;

	CXmlNodePtr pDyncDat = pDyncPtr->GetChild(_T("DYNC_DATA"), FALSE);
	ASSERT(!pDyncDat->IsNull());
	if (!pDyncDat->IsNull())
	{
		DWORD dwFormat = pDyncDat->GetAttribute(_T("FORMAT"), 0);
		if (dwFormat > X_XXXXX)
		{
			dwFormat = 0;
		}
		m_nFormat = static_cast <VAL_FORMAT> (dwFormat);
		m_bEnable = TRUE;
		return true;
	}
	return false;
}
#endif

/** @}*/
