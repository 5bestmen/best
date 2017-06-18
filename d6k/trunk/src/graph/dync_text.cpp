#include "base_widget.h"
#include "dync_text.h"
#include "variant.h"
#include "../scdpub/variant_data.h"
#include "stl_util-inl.h"
#include "scadaapi/scdsvcapi.h"
#include "fesapi/fesdatadef.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

CDyncText::CDyncText()
{
}

CDyncText::~CDyncText()
{
	STLDeleteElements(&m_arrTexts);
}

const CDyncText& CDyncText::operator=(const CDyncText& src)
{
	if (this != &src)
	{
		for each (CDyncTextValueInfo* var in src.m_arrTexts)
		{
			CDyncTextValueInfo *pValue = CreateTextData();
			*pValue = *var;
		}
		//父类
		CBaseDyncData::operator=(src);
	}
	return *this;
}

void CDyncText::CopyBaseData(CBaseDyncData *src)
{
	CDyncText *tdync = dynamic_cast<CDyncText*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}

	for each (CDyncTextValueInfo* var in tdync->m_arrTexts)
	{
		CDyncTextValueInfo *pValue = CreateTextData();
		*pValue = *var;
	}

	//父类赋值
	CBaseDyncData::CopyBaseData(src);
}

CDyncTextValueInfo * CDyncText::CreateTextData()
{
	CDyncTextValueInfo *pData = new CDyncTextValueInfo;
	Q_ASSERT(pData);
	m_arrTexts.push_back(pData);

	return pData;
}

void CDyncText::DeleteTextData(CDyncTextValueInfo *pData)
{
	Q_ASSERT(pData);
	if (pData == NULL)
		return;

	auto it_find = std::find(m_arrTexts.begin(), m_arrTexts.end(), pData);
	if (it_find != m_arrTexts.end())
	{//找到
		delete pData;
		m_arrTexts.erase(it_find);
	}
	else
	{
		delete pData;
		Q_ASSERT(false);
	}
	return;
}

void CDyncText::ClearAllTextData()
{
	int nSize = (int)m_arrTexts.size();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		delete m_arrTexts[i];
	}

	m_arrTexts.clear();
}

void CDyncText::PrepareWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget==nullptr)
	{
		return;
	}
	CBaseDyncData::PrepareWidgetDync(pWidget);
}

bool CDyncText::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
	{
		return false;
	}
	Q_ASSERT(m_bEnable);
	if (m_bEnable == false)
		return false;

	CVariant val;
	bool bRet = GetRealTimeValue(m_pTagInfo, &val);

	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}

	switch (m_nDyncType)
	{	
	case CBaseDyncData::DYNC_STRING:
	{
		QString strDesc = GetDesc(val.operator double());

		if (strDesc.isEmpty())
		{
			return false;
		}

		pWidget->SetText(strDesc);

		break;
	}
	case CBaseDyncData::DYNC_MAP:
	{
		//QString strDesc = GetDesc(4);
		QString strDesc = GetDesc(val.operator double());

		if (strDesc.isEmpty())
		{
			return false;
		}

		pWidget->GetImageInfo()->SetBackgroundImage(strDesc);
	
		break;
	}
	default:
		break;
	}

	

	return true;
}

QString CDyncText::GetDesc(int nVal)
{
	for (auto iter:m_arrTexts)
	{
		if (iter->GetorigData().toInt()==nVal)
		{
			return iter->GetDescData();
		}
	}
	return QString::null;
}
/*! \fn void CDyncText::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
*********************************************************************************************************
** \brief CDyncText::SaveXml
** \details 保存动态可见的数据
** \param
** \return void
** \author LiJin
** \date 2017年6月8日
** \note
********************************************************************************************************/
void CDyncText::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;
	pXmlWriter->writeStartElement("DynamicTextContext");

	pXmlWriter->writeAttribute("Enable", QString::number(m_bEnable));
	pXmlWriter->writeAttribute("Variable", m_szTagName.data());

	for (auto it : m_arrTexts)
	{
		pXmlWriter->writeStartElement("TextRange");

		pXmlWriter->writeAttribute("Source", it->GetorigData());
		pXmlWriter->writeAttribute("Desc", it->GetDescData());

		pXmlWriter->writeEndElement();
	}

	pXmlWriter->writeEndElement();
}

bool CDyncText::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}