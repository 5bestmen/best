/*! @file dync_color.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : dync_color.cpp
�ļ�ʵ�ֹ��� : ��̬��ɫ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ��̬��ɫ����
*  @author LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/


#include "base_widget.h"
#include "dync_color.h"
#include "colour_define.h" 

#include "stl_util-inl.h"

const QString CDyncClrValueInfo::SZ_BINKING[CDyncClrValueInfo::BLINKING_CUSTOM + 1] =
{
	QStringLiteral("��"),
	QStringLiteral("���ɼ�"),
	QStringLiteral("����"),
	QStringLiteral("����"),
	QStringLiteral("����"),
	QStringLiteral("�Զ���")
};

const  QString CDyncClrValueInfo::SZ_BLINKING_NONE      = QStringLiteral("��");
const  QString CDyncClrValueInfo::SZ_BLINKING_INVISIBLE = QStringLiteral("���ɼ�");
const  QString CDyncClrValueInfo::SZ_BLINKING_FAST      = QStringLiteral("����");
const  QString CDyncClrValueInfo::SZ_BLINKING_NORMAL    = QStringLiteral("����");
const  QString CDyncClrValueInfo::SZ_BLINKING_SLOW      = QStringLiteral("����");
const  QString CDyncClrValueInfo::SZ_BLINKING_CUSTOM    = QStringLiteral("�Զ���");

CDyncClrValueInfo::COLOR_ARRAY CDyncClrValueInfo::m_crTable =
{
	XGraphics::Red,
	XGraphics::Green,
	XGraphics::Blue,
	XGraphics::Yellow,
	XGraphics::Orange,
	XGraphics::Gray,
	XGraphics::Gold,
	XGraphics::Brown,
};

CDyncClrValueInfo::CDyncClrValueInfo()
{
}

CDyncClrValueInfo::CDyncClrValueInfo(int nIdx)
{
//	m_dwColor = m_crTable[nIdx%ColorTabNum].ToCOLORREF();
	m_nIndex = 0;
//	m_dwBlinkingColor = ::GetSysColor(COLOR_WINDOW);

	m_nBlinkingType = BLINKING_NONE;
	m_nBlinkInterval = 1000;
//	m_tmLast = boost::posix_time::microsec_clock::local_time();
}

CDyncClrValueInfo::CDyncClrValueInfo(const CDyncClrValueInfo  * pSrc)
{
	m_nIndex = pSrc->m_nIndex;
	m_dwColor = pSrc->m_dwColor;
	m_szValue = pSrc->m_szValue;

	m_dwBlinkingColor = pSrc->m_dwBlinkingColor;
	m_nBlinkingType = pSrc->m_nBlinkingType;
	m_nBlinkInterval = pSrc->m_nBlinkInterval;
}

CDyncClrValueInfo::~CDyncClrValueInfo()
{

}

CDyncClrData::CDyncClrData()
{

}

CDyncClrData::~CDyncClrData()
{
	STLDeleteElements(&m_arrColors);
}


bool CDyncClrData::IsDyncTypeEqual(CBaseDyncData *pData) const
{
	Q_ASSERT(pData);
	if (pData == nullptr)
		return false;

	if (pData->m_nDyncType != CBaseDyncData::DYNC_COLOR)
		return false;

	CDyncClrData *pDyncClr = dynamic_cast <CDyncClrData*> (pData);

	if (pDyncClr->m_nDyncType == this->m_nDyncType && pDyncClr->m_nDyncClrType == this->m_nDyncClrType)
		return true;

	return false;
}

bool CDyncClrData::CheckDyncData()
{
	bool bRet = __super::CheckDyncData();
	if (bRet == false)
		return false;

	auto iter = m_arrColors.begin();
	auto iter_tmp = m_arrColors.begin();
	// ���ֵ�����Ƿ�Ϊ��
	for (; iter != m_arrColors.end(); ++iter)
	{
		if ((*iter)->m_szValue.empty() == false)
			return false;
	}
	// ����Ƿ����ظ���ֵ
	iter = m_arrColors.begin();

	for (; iter != m_arrColors.end(); ++iter)
	{
		for (; iter_tmp != m_arrColors.end(); ++iter_tmp)
		{
			if ((*iter) != (*iter_tmp))
			{
				if ((*iter)->m_szValue == (*iter_tmp)->m_szValue)
					return false;
			}
		}
	}
	return true;
}
/*! \fn  CDyncClrValueInfo *CDyncClrData::CreateClrData( )
*********************************************************************************************************
** \brief   CDyncClrData::CreateClrData
** \details ����һ����ɫ����
** \return CDyncClrValueInfo *
** \author LiJin
** \date  2011��1��10��
** \note
********************************************************************************************************/
CDyncClrValueInfo * CDyncClrData::CreateClrData()
{
	int nSize = m_arrColors.size();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		m_arrColors[i]->m_nIndex = i + 1;
	}

	CDyncClrValueInfo *pData = new CDyncClrValueInfo(nSize);
	Q_ASSERT(pData);
	m_arrColors.push_back(pData);
	pData->m_nIndex = m_arrColors.size();

	return pData;
}

void CDyncClrData::DeleteClrData(CDyncClrValueInfo *pData)
{
	Q_ASSERT(pData);
	if (pData == NULL)
		return;

	auto it_find = std::find(m_arrColors.begin(), m_arrColors.end(), pData);
	if (it_find != m_arrColors.end())
	{//�ҵ�
		delete pData;
		m_arrColors.erase(it_find);
	}
	else
	{
		delete pData;
		Q_ASSERT(false);
	}

	int nSize = m_arrColors.size();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		m_arrColors[i]->m_nIndex = i + 1;
	}
	return;
} 

CBaseDyncData *CDyncClrData::CloneDyncData()
{
	CDyncClrData *pNewDync = new CDyncClrData;
	*pNewDync = *this;
	return dynamic_cast<CDyncClrData*> (pNewDync);
}

void CDyncClrData::PrepareWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;
#if 0
	if (m_nDyncClrType == DYNC_FILL_CLR)
	{
		BRUSH *pBrush = pWidget->GetBrush();
		ASSERT(pBrush);
		if (pBrush == NULL)
			return;
		m_nTransparency = pBrush->m_StartTransparency;
		m_dwColor = pBrush->m_StartColor;
	}
	else if (m_nDyncClrType == DYNC_TEXT_CLR)
	{
		FONT *pFont = pWidget->GetFont();
		ASSERT(pFont);
		if (pFont == NULL)
			return;
		m_nTransparency = pFont->m_transparency;
		m_dwColor = pFont->m_color;
	}
	else if (m_nDyncClrType == DYNC_LINE_CLR)
	{
		PEN *pPen = pWidget->GetPen();
		ASSERT(pPen);
		if (pPen == NULL)
			return;
		m_nTransparency = pPen->m_transparency;
		m_dwColor = pPen->m_color;
	}
#endif
}

bool CDyncClrData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return false;

	Q_ASSERT(m_bEnable);
	if (m_bEnable == false)
		return false;

	if (m_szTagName.empty() == true)
		return false;

#if 0
	// ��ȡʵʱ����
	UNION_VALUE rtval;
	double dValue = 0.0f;

	bool bRtn = GetRTVarValue(m_szTagName, rtval);
	if (bRtn == false)
		return false;

	COLORREF clrCur = RGB(0, 0, 0);
	bool bInvisible = false;

	if (rtval.dt == DT_STRING)
	{// ������ַ���
		TCHAR *szValue = V_STRING(rtval);
		if (szValue == NULL || _tcslen(szValue) == 0)
			return false;

		//	CString szValue;
		//	szValue.Format(_T("%s"),V_STRING(rtval));
		std::vector<CDyncClrValueInfo*>::iterator iter = m_arrColors.begin();
		// ���ֵ�����Ƿ�Ϊ��
		for (; iter != m_arrColors.end(); ++iter)
		{
			if ((*iter)->m_szValue.Compare(szValue) == 0)
			{
				clrCur = (*iter)->m_dwColor;
				break;
			}
		}
	}
	else
	{
		//clrCur = GetColor(rtval);
		bRtn = GetColor(rtval, clrCur, bInvisible);
	}

	if (bRtn == false)
		return false;

	if (bInvisible)
	{//���ɼ�
		if (m_nDyncClrType == DYNC_FILL_CLR)
		{
			BRUSH *pBrush = pWidget->GetBrush();
			ASSERT(pBrush);
			if (pBrush == NULL)
				return false;
			pBrush->m_StartTransparency = 100;
		}
		else if (m_nDyncClrType == DYNC_TEXT_CLR)
		{
			FONT *pFont = pWidget->GetFont();
			ASSERT(pFont);
			if (pFont == NULL)
				return false;
			pFont->m_transparency = 100;
		}
		else if (m_nDyncClrType == DYNC_LINE_CLR)
		{
			PEN *pPen = pWidget->GetPen();
			ASSERT(pPen);
			if (pPen == NULL)
				return false;
			pPen->m_transparency = 100;
		}
		return true;
	}

	if (m_nDyncClrType == DYNC_FILL_CLR)
	{
		BRUSH *pBrush = pWidget->GetBrush();
		ASSERT(pBrush);
		if (pBrush == NULL)
			return false;
		if (pBrush->m_BrushType == Gdiplus::BrushTypeSolidColor)
		{
			pBrush->m_StartColor = clrCur;
		}
		pBrush->m_StartTransparency = m_nTransparency;
	}
	else if (m_nDyncClrType == DYNC_TEXT_CLR)
	{
		FONT *pFont = pWidget->GetFont();
		ASSERT(pFont);
		if (pFont == NULL)
			return false;
		pFont->m_color = clrCur;
		pFont->m_transparency = m_nTransparency;
	}
	else if (m_nDyncClrType == DYNC_LINE_CLR)
	{
		PEN *pPen = pWidget->GetPen();
		ASSERT(pPen);
		if (pPen == NULL)
			return false;
		pPen->m_color = clrCur;
		pPen->m_transparency = m_nTransparency;
	}
	else
	{
		ASSERT(FALSE);
		return false;
	}
#endif
	return true;
}


/** @}*/
