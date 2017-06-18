/*! @file dync_transparency.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_transparency.cpp
�ļ�ʵ�ֹ��� :  ��̬͸������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̬͸������
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/


#include "base_widget.h"
#include "dync_transparency.h" 

#include "stl_util-inl.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

CDyncTransparencyData::CDyncTransparencyData() :CBaseDyncData()
{
	m_bEnable = false;
	m_nDyncType = DYNC_TRANSPARENCY;

	m_dMaxValue = 255;
	m_dMinValue = 0.0f;
}

void CDyncTransparencyData::CopyBaseData(CBaseDyncData *src)
{
	CDyncTransparencyData *tdync = dynamic_cast<CDyncTransparencyData*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}

	m_dMinValue = tdync->m_dMinValue;
	m_dMaxValue = tdync->m_dMaxValue;

	//���ำֵ
	CBaseDyncData::CopyBaseData(src);
}

CDyncTransparencyData::~CDyncTransparencyData()
{

}
const CDyncTransparencyData& CDyncTransparencyData::operator=(const CDyncTransparencyData& src)
{
	if (this != &src)
	{
// 		m_bEnable = src.m_bEnable;
// 		m_nDyncType = src.m_nDyncType;
// 		m_szTagName = src.m_szTagName;
		m_dMinValue = src.m_dMinValue;
		m_dMaxValue = src.m_dMaxValue;
		//����
		CBaseDyncData::operator=(src);
	}
	return *this;
}

CBaseDyncData *CDyncTransparencyData::CloneDyncData()
{
	CDyncTransparencyData *pNewDync = new CDyncTransparencyData;
	*pNewDync = *this;
	return dynamic_cast<CDyncTransparencyData*> (pNewDync);
}

bool CDyncTransparencyData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
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

	if (rtval.dt == DT_STRING)
	{// ������ַ��� ������֧��


		return false;
	}
	else
	{

	}

#endif

	return true;
}
void CDyncTransparencyData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;
	
	pXmlWriter->writeStartElement("Transparency");
	pXmlWriter->writeAttribute("Enable", QString::number(m_bEnable));
	pXmlWriter->writeAttribute("Variable", m_szTagName.data());

	pXmlWriter->writeAttribute("TransStart", QString::number(GetMinValue()));
	pXmlWriter->writeAttribute("TransEnd", QString::number(GetMaxValue()));
	pXmlWriter->writeEndElement();
}

bool CDyncTransparencyData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}

/** @}*/

