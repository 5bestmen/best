/*! @file dync_color.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_color.cpp
�ļ�ʵ�ֹ��� :  ��̬��ɫ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̬��ɫ����
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/


#include "base_widget.h"
#include "dync_color.h"
#include "colour_define.h" 
#include "variant.h"
#include "../scdpub/variant_data.h"
#include "stl_util-inl.h"
#include "scadaapi/scdsvcapi.h"
#include "fesapi/fesdatadef.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#define  COMPARE_EQUAL_VAL  0.000001

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
	Q_UNUSED(nIdx);
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

const CDyncClrValueInfo& CDyncClrValueInfo::operator=(const CDyncClrValueInfo& src)
{
	m_nIndex = src.m_nIndex;
	m_dwColor = src.m_dwColor;
	m_szValue = src.m_szValue;

	m_dwBlinkingColor = src.m_dwBlinkingColor;
	m_nBlinkingType = src.m_nBlinkingType;
	m_nBlinkInterval = src.m_nBlinkInterval;

	m_dValue = src.m_dValue;
	m_dwColor = src.m_dwColor;
	m_nIndex = src.m_nIndex;

	return *this;
}

CDyncClrData::CDyncClrData()
{

}

CDyncClrData::~CDyncClrData()
{
	STLDeleteElements(&m_arrColors);
}


const CDyncClrData& CDyncClrData::operator=(const CDyncClrData& src)
{
	m_nDyncClrType = src.m_nDyncClrType;
	m_dwColor = src.m_dwColor;
	m_nTransparency = src.m_nTransparency;

	for each (CDyncClrValueInfo* var in src.m_arrColors)
	{
		CDyncClrValueInfo* pNewClrValue = CreateClrData();
		//��ֵ
		*pNewClrValue = *var;
	}
	//����
	CBaseDyncData::operator=(src);

	return *this;
}


void CDyncClrData::CopyBaseData(CBaseDyncData *src)
{
	CDyncClrData *tdync = dynamic_cast<CDyncClrData*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}

	m_nDyncClrType = tdync->m_nDyncClrType;
	m_dwColor = tdync->m_dwColor;
	m_nTransparency = tdync->m_nTransparency;

	for each (CDyncClrValueInfo* var in tdync->m_arrColors)
	{
		CDyncClrValueInfo* pNewClrValue = CreateClrData();
		//��ֵ
		*pNewClrValue = *var;
	}

	CBaseDyncData::CopyBaseData(src);
}

bool CDyncClrData::IsDyncTypeEqual(CBaseDyncData *pData) const
{
	Q_ASSERT(pData);
	if (pData == nullptr)
		return false;

	if (pData->m_nDyncType != CBaseDyncData::DYNC_PEN_COLOR)
		return false;

	CDyncClrData *pDyncClr = dynamic_cast <CDyncClrData*> (pData);

	if (pDyncClr->m_nDyncType == this->m_nDyncType && pDyncClr->m_nDyncClrType == this->m_nDyncClrType)
		return true;

	return false;
}

bool CDyncClrData::CheckDyncData()
{
	bool bRet = CBaseDyncData::CheckDyncData();
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
	int nSize = (int)m_arrColors.size();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		m_arrColors[i]->m_nIndex = i + 1;
	}

	CDyncClrValueInfo *pData = new CDyncClrValueInfo(nSize);
	Q_ASSERT(pData);
	m_arrColors.push_back(pData);
	pData->m_nIndex = (int)m_arrColors.size();

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

	int nSize = (int)m_arrColors.size();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		m_arrColors[i]->m_nIndex = i + 1;
	}
	return;
} 

//���Clrdata
void CDyncClrData::ClearAllClrData()
{
	int nSize = (int)m_arrColors.size();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		delete m_arrColors[i];
	}

	m_arrColors.clear();
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
	if (m_nDyncClrType == DYNC_FILL_CLR)
	{
		CBrushInfo *pBrush = pWidget->GetBrushInfo();
 		Q_ASSERT(pBrush);
 		if (pBrush == NULL)
 			return;
		m_nTransparency = pBrush->GetColor().alpha();
		m_dwColor = pBrush->GetColor();
	}

	else if (m_nDyncClrType == DYNC_TEXT_CLR)
	{
		CFontInfo *pFont = pWidget->GetFontInfo();
		Q_ASSERT(pFont);
		if (pFont == NULL)
			return;
		m_nTransparency = pFont->GetColor().alpha();
		m_dwColor = pFont->GetColor();
	}
	else if (m_nDyncClrType == DYNC_LINE_CLR)
	{
		CPenInfo *pPen = pWidget->GetPenInfo();
		Q_ASSERT(pPen);
		if (pPen == NULL)
			return;
		m_nTransparency = pPen->GetColor().alpha();
		m_dwColor = pPen->GetColor();
	}
	CBaseDyncData::PrepareWidgetDync(pWidget);
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

	// ��ȡʵʱ����
//	IO_VARIANT rtval;

	//double dValue = 0.0f;
	
	bool bRet=false;

	CDyncClrValueInfo  pValInfo;

	CVariant var;
//	bRet = ::GetRTData(m_pTagInfo->IddType, m_pTagInfo->NodeOccNo, m_pTagInfo->TagOccNo, m_pTagInfo->FiledID, &rtval);
	bRet = GetRealTimeValue(m_pTagInfo , &var);

	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}

//	CVariant var(rtval);

	QColor clrCur = QColor(0, 0, 0);
	
	bool bInvisible = false;
	//bool bRtn = false;
	if (var.Type == DT_STRING)
	{// ������ַ���
		char *szValue = V_STR(var);
		if (szValue == NULL || strlen(szValue) == 0)
			return false;

		auto  iter = m_arrColors.begin();
		// ���ֵ�����Ƿ�Ϊ��
		for (; iter != m_arrColors.end(); ++iter)
		{
			if ((*iter)->m_szValue.compare(szValue) == 0)
			{
				clrCur = (*iter)->m_dwColor;
				break;
			}
		}
	}
	else
	{
		//clrCur = GetColor(rtval);
		//bRtn = GetColor(rtval, clrCur, bInvisible);
		bRet = GetColor(var, *&pValInfo,bInvisible );
	}

//	if (bRtn == false)
//		return false;

	if (bInvisible)
	{//���ɼ�
		if (m_nDyncClrType == DYNC_FILL_CLR)
		{
			CBrushInfo *pBrush = pWidget->GetBrushInfo();
			Q_ASSERT(pBrush);
			if (pBrush == NULL)
				return false;
			pBrush->GetColor().setAlpha(0);
		}
		else if (m_nDyncClrType == DYNC_TEXT_CLR)
		{
			CFontInfo *pFont = pWidget->GetFontInfo();
			Q_ASSERT(pFont);
			if (pFont == NULL)
				return false;
			pFont->GetColor().setAlpha(0);
		}
		else if (m_nDyncClrType == DYNC_LINE_CLR)
		{
			CPenInfo *pPen = pWidget->GetPenInfo();
			Q_ASSERT(pPen);
			if (pPen == NULL)
				return false;
			pPen->GetColor().setAlpha(0);
		}
		return true;
	}

	if (m_nDyncClrType == DYNC_FILL_CLR)
	{
		CBrushInfo *pBrush = pWidget->GetBrushInfo();
		Q_ASSERT(pBrush);
		if (pBrush == NULL)
			return false;
		//if (pBrush->GetType() == CBrushInfo::NOGRADIENT)
		//{
			pBrush->SetColor(pValInfo.m_dwColor) ;
		//}
		pBrush->SetTranspancy(m_nTransparency);
	}
	else if (m_nDyncClrType == DYNC_TEXT_CLR)
	{
		CFontInfo *pFont = pWidget->GetFontInfo();
		Q_ASSERT(pFont);
		if (pFont == NULL)
			return false;
		pFont->SetColor(pValInfo.m_dwColor);
		pFont->SetTranspancy(m_nTransparency);
	}
	else if (m_nDyncClrType == DYNC_LINE_CLR)
	{
		CPenInfo *pPen = pWidget->GetPenInfo();
		Q_ASSERT(pPen);
		if (pPen == NULL)
			return false;
		pPen->SetColor(pValInfo.m_dwColor);
		pPen->SetTranspancy(m_nTransparency);
	}
	else
	{
		Q_ASSERT(false);
		return false;
	}

	//pWidget->scene()->update(pWidget->boundingRect());

	return true;
}

bool CDyncClrData::GetColor(const CVariant &value, CDyncClrValueInfo &clrCur, bool &bInvisible) const
{
	Q_UNUSED(bInvisible);
	auto  iter = m_arrColors.begin();
	// ���ֵ�����Ƿ�Ϊ��
	for (; iter != m_arrColors.end(); ++iter)
	{
		if (qAbs((*iter)->m_dValue-value.operator double()) <= COMPARE_EQUAL_VAL)
		{
			clrCur = (*iter);
			return true;
		}
	}

	return false;
}
/*! \fn void CDyncClrData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
********************************************************************************************************* 
** \brief CDyncClrData::SaveXml 
** \details ���涯̬��ɫ�仯
** \param  
** \return void 
** \author LiJin 
** \date 2017��6��8�� 
** \note 
********************************************************************************************************/
void CDyncClrData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;

	if (m_nDyncType == CBaseDyncData::DYNC_PEN_COLOR)
	{
		pXmlWriter->writeStartElement("DynamicPenColor");
	}
	else if (m_nDyncType == CBaseDyncData::DYNC_TEXT_COLOR)
	{
		pXmlWriter->writeStartElement("DynamicTextColor");
	}

	pXmlWriter->writeAttribute("Enable", QString::number(m_bEnable));
	pXmlWriter->writeAttribute("Variable", m_szTagName.data());

	for (auto it : m_arrColors)
	{
		pXmlWriter->writeStartElement("ColorRange");
		//pXmlWriter->writeAttribute("Variable", it->GetBindData());
		pXmlWriter->writeAttribute("Color", it->m_dwColor.name(QColor::HexArgb));
		pXmlWriter->writeAttribute("FlashColor", it->m_dwBlinkingColor.name(QColor::HexArgb));

		pXmlWriter->writeAttribute("FlashMode", QString::number(it->m_nBlinkingType));
		pXmlWriter->writeAttribute("FlashTime", QString::number(it->m_nBlinkInterval));
		pXmlWriter->writeAttribute("ThresholdData", QString::number(it->m_dValue));
		pXmlWriter->writeEndElement();
	}

	pXmlWriter->writeEndElement();
}

bool CDyncClrData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{



	return true;
}


const CDyncFillingData& CDyncFillingData::operator=(const CDyncFillingData& src)
{
	m_bAnalogColor = src.m_bAnalogColor;
	m_nFillingPolicy = src.m_nFillingPolicy;
	m_dMinVal = src.m_dMinVal;
	m_dMaxVal = src.m_dMaxVal;
	m_dStartPos = src.m_dStartPos;
	m_dEndPos = src.m_dEndPos;
	//���ำֵ
	CDyncClrData::operator=(src);

	return *this;
}


bool CDyncFillingData::IsDyncTypeEqual(CBaseDyncData * pData) const
{
	Q_UNUSED(pData);
	return true;
}

bool CDyncFillingData::CheckDyncData()
{
	return true;
}

void CDyncFillingData::PrepareWidgetDync(CBaseWidget * pWidget)
{
	Q_UNUSED(pWidget);
}

bool CDyncFillingData::ProcessWidgetDync(CBaseWidget * pWidget)
{
	Q_UNUSED(pWidget);
	return true;
}

CBaseDyncData * CDyncFillingData::CloneDyncData()
{
	return nullptr;
}

void CDyncFillingData::CopyBaseData(CBaseDyncData *src)
{
	CDyncFillingData *tdync = dynamic_cast<CDyncFillingData*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}
	m_bAnalogColor = tdync->m_bAnalogColor;
	m_nFillingPolicy = tdync->m_nFillingPolicy;
	m_dMinVal = tdync->m_dMinVal;
	m_dMaxVal = tdync->m_dMaxVal;
	m_dStartPos = tdync->m_dStartPos;
	m_dEndPos = tdync->m_dEndPos;
	//���ำֵ
	CDyncClrData::CopyBaseData(src);
}

/*! \fn void CDyncFillingData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
********************************************************************************************************* 
** \brief CDyncFillingData::SaveXml 
** \details ���涯̬�������
** \param  
** \return void 
** \author LiJin 
** \date 2017��6��8�� 
** \note 
********************************************************************************************************/
void CDyncFillingData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;

	pXmlWriter->writeStartElement("DynamicBrush");

	pXmlWriter->writeAttribute("Enable", QString::number(m_bEnable));
	pXmlWriter->writeAttribute("Variable", m_szTagName.data());

	//ģ��ɫ
	pXmlWriter->writeAttribute("AnalogColor", QString::number(m_bAnalogColor));
	//���ֵ
	pXmlWriter->writeAttribute("StartValue", QString::number(m_dMinVal));
	pXmlWriter->writeAttribute("EndValue", QString::number(m_dMaxVal));
	pXmlWriter->writeAttribute("StartOffset", QString::number(m_dStartPos));
	pXmlWriter->writeAttribute("EndOffset", QString::number(m_dEndPos));
	//ģ��ɫ����
	pXmlWriter->writeAttribute("ColorType", QString::number(m_nFillingPolicy));

	for (auto it : m_arrColors)
	{
		pXmlWriter->writeStartElement("ColorRange");
		//pXmlWriter->writeAttribute("Variable", it->GetBindData());
		pXmlWriter->writeAttribute("Color", it->m_dwColor.name(QColor::HexArgb));
		pXmlWriter->writeAttribute("FlashColor", it->m_dwBlinkingColor.name(QColor::HexArgb));

		pXmlWriter->writeAttribute("FlashMode", QString::number(it->m_nBlinkingType));
		pXmlWriter->writeAttribute("FlashTime", QString::number(it->m_nBlinkInterval));
		pXmlWriter->writeAttribute("ThresholdData", QString::number(it->m_dValue));
		pXmlWriter->writeEndElement();
	}

	pXmlWriter->writeEndElement();
}

bool CDyncFillingData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{

	return true;
}

/** @}*/
