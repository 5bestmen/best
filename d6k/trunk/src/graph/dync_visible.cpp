/*! @file dync_visible.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_visible.cpp
�ļ�ʵ�ֹ��� :  ��̬�ɼ�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��̬�ɼ�����
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/


#include "base_widget.h"
#include "dync_visible.h" 
#include "variant.h"
#include "../scdpub/variant_data.h"
#include "stl_util-inl.h"
#include "scadaapi/scdsvcapi.h"
#include "fesapi/fesdatadef.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

CDyncVisibleData::CDyncVisibleData() :CBaseDyncData()
{
	m_bEnable = false;
	m_nDyncType = DYNC_VISIBLE;
	//! ���ڱȽϼ���
	m_dValue = 0.0f;
	//! ������Χ
	m_dDeadBand = 0.0f;

	m_nCondition = CONDITION_EQ;
	m_nBlinking = BLINKING_NONE;

	m_nBlinkInterval = 500;
}

void CDyncVisibleData::CopyBaseData(CBaseDyncData *src)
{
	CDyncVisibleData *tdync = dynamic_cast<CDyncVisibleData*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}

	m_bEnable = tdync->m_bEnable;
	m_nDyncType = tdync->m_nDyncType;
	m_szTagName = tdync->m_szTagName;

	m_dValue = tdync->m_dValue;
	m_dDeadBand = tdync->m_dDeadBand;

	m_nCondition = tdync->m_nCondition;
	m_nBlinking = tdync->m_nBlinking;

	m_nBlinkInterval = tdync->m_nBlinkInterval;

	//���ำֵ
	CBaseDyncData::CopyBaseData(src);
}

CDyncVisibleData::~CDyncVisibleData()
{

}
const CDyncVisibleData& CDyncVisibleData::operator=(const CDyncVisibleData& src)
{
	if (this != &src)
	{
		m_bEnable = src.m_bEnable;
		m_nDyncType = src.m_nDyncType;
		m_szTagName = src.m_szTagName;

		m_dValue = src.m_dValue;
		m_dDeadBand = src.m_dDeadBand;

		m_nCondition = src.m_nCondition;
		m_nBlinking = src.m_nBlinking;

		m_nBlinkInterval = src.m_nBlinkInterval;
	}
	return *this;
}

bool CDyncVisibleData::SetVisible(bool bFlag, CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
		return false;

	bool bInvalidFlag = false;

	if (bFlag)
	{
		if (pWidget->IsVisible() == false)
			bInvalidFlag = true;
		pWidget->SetVisible(true);
	}
	else
	{
		if (pWidget->IsVisible() == true)
			bInvalidFlag = true;
		pWidget->SetVisible(false);
	}
	return bInvalidFlag;
}

void CDyncVisibleData::PluseWidget(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
		return;
	if (pWidget->IsVisible())
	{
		pWidget->SetVisible(false);
	}
	else
	{
		pWidget->SetVisible(true);
	}
}

bool  CDyncVisibleData::SetBlinking(bool bFlag, CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
		return false;

	bool bInvalidFlag = false;

	// ȡ����˸
	if (bFlag == false)
	{
		pWidget->m_nBlinkingCnt = 0;
		if (pWidget->m_bIsBlinking)
		{
			pWidget->m_bIsBlinking = false;
			return true;
		}
		return false;
	}

	if (m_nBlinking == BLINKING_NONE)
	{
		return false;
	}
	else if (m_nBlinking == BLINKING_FAST)
	{// ������˸ֱ�ӷ�ת 
		PluseWidget(pWidget);
		bInvalidFlag = true;
	}
	else if (m_nBlinking == BLINKING_NORMAL)
	{// 2��ʱ����
		pWidget->m_nBlinkingCnt++;
		if (pWidget->m_nBlinkingCnt > BLINKING_NORMAL_CNT)
		{
			PluseWidget(pWidget);
			bInvalidFlag = true;
			pWidget->m_nBlinkingCnt = 0;
		}
	}
	else if (m_nBlinking == BLINKING_SLOW)
	{// 4��ʱ����
		pWidget->m_nBlinkingCnt++;
		if (pWidget->m_nBlinkingCnt > BLINKING_SLOW_CNT)
		{
			PluseWidget(pWidget);
			bInvalidFlag = true;
			pWidget->m_nBlinkingCnt = 0;
		}
	}

	pWidget->m_bIsBlinking = true;

	return bInvalidFlag;
}

__inline bool CDyncVisibleData::HasBlinking()
{
	return (m_nBlinking != BLINKING_NONE) ? true : false;
}

CBaseDyncData *CDyncVisibleData::CloneDyncData()
{
	CDyncVisibleData *pNewDync = new CDyncVisibleData;
	*pNewDync = *this;
	return dynamic_cast<CDyncVisibleData*> (pNewDync);
}

bool CDyncVisibleData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
		return false;

	Q_ASSERT(m_bEnable);
	if (m_bEnable == false)
		return false;

	if (m_szTagName.empty() == true)
		return false;

	bool bVisibleRtn = false, bTwinkleRtn = false;

	// ��ȡʵʱ����
//	IO_VARIANT  rtval;
//	bool bRtn =::GetRTData(m_pTagInfo->IddType, m_pTagInfo->NodeOccNo, m_pTagInfo->TagOccNo, m_pTagInfo->FiledID, &rtval);
	CVariant var;
	bool bRtn = GetRealTimeValue(m_pTagInfo, &var);

	Q_ASSERT(bRtn);
	if (!bRtn)
	{
		return false;
	}

	QColor clrCur = QColor(0, 0, 0);

//	CVariant var(rtval);

	if (var.Type == DT_STRING)
	{// ������ַ��� ������֧��
		return false;
	}
	else
	{
		double dblValue =  var.operator double();

		if (m_nCondition == CONDITION_EQ)
		{// ����
			if (dblValue >= m_dValue - m_dDeadBand && dblValue <= m_dValue + m_dDeadBand)
			{
				if (HasBlinking())
				{
					bTwinkleRtn = SetBlinking(true, pWidget);
				}
				else
				{
					bVisibleRtn = SetVisible(true, pWidget);
				}
			}
			else
			{
				bVisibleRtn = SetVisible(false, pWidget);
				bTwinkleRtn = SetBlinking(false, pWidget);
			}
		}
		else if (m_nCondition == CONDITION_NE)
		{
			if (dblValue >= m_dValue - m_dDeadBand && dblValue <= m_dValue + m_dDeadBand)
			{
				if (HasBlinking())
				{
					bTwinkleRtn = SetBlinking(true, pWidget);
				}
				else
				{
					bVisibleRtn = SetVisible(true, pWidget);
				}
			}
			else
			{
				bVisibleRtn = SetVisible(false, pWidget);
				bTwinkleRtn = SetBlinking(false, pWidget);
			}
		}
		else if (m_nCondition == CONDITION_GE)
		{// ���ڵ���
			if (dblValue >= m_dValue)
			{
				if (HasBlinking())
				{
					bTwinkleRtn = SetBlinking(true, pWidget);
				}
				else
				{
					bVisibleRtn = SetVisible(true, pWidget);
				}
			}
			else
			{
				bVisibleRtn = SetVisible(false, pWidget);
				bTwinkleRtn = SetBlinking(false, pWidget);
			}
		}
		else if (m_nCondition == CONDITION_GT)
		{// ���� 
			if (dblValue > m_dValue)
			{
				if (HasBlinking())
				{
					bTwinkleRtn = SetBlinking(true, pWidget);
				}
				else
				{
					bVisibleRtn = SetVisible(true, pWidget);
				}
			}
			else
			{
				bVisibleRtn = SetVisible(false, pWidget);
				bTwinkleRtn = SetBlinking(false, pWidget);
			}
		}
		else if (m_nCondition == CONDITION_LE)
		{// С�ڵ��� 
			if (dblValue > m_dValue)
			{
				if (HasBlinking())
				{
					bTwinkleRtn = SetBlinking(true, pWidget);
				}
				else
				{
					bVisibleRtn = SetVisible(true, pWidget);
				}
			}
			else
			{
				bVisibleRtn = SetVisible(false, pWidget);
				bTwinkleRtn = SetBlinking(false, pWidget);
			}
		}
		else if (m_nCondition == CONDITION_LT)
		{// С�� 
			if (dblValue > m_dValue)
			{
				if (HasBlinking())
				{
					bTwinkleRtn = SetBlinking(true, pWidget);
				}
				else
				{
					bVisibleRtn = SetVisible(true, pWidget);
				}
			}
			else
			{
				bVisibleRtn = SetVisible(false, pWidget);
				bTwinkleRtn = SetBlinking(false, pWidget);
			}
		}
		else
		{
			Q_ASSERT(false);
			return false;
		}
	}
	return bVisibleRtn | bTwinkleRtn;
}
/*! \fn void CDyncVisibleData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
********************************************************************************************************* 
** \brief CDyncVisibleData::SaveXml 
** \details ���涯̬�ɼ�������
** \param  
** \return void 
** \author LiJin 
** \date 2017��6��8�� 
** \note 
********************************************************************************************************/
void CDyncVisibleData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;

	pXmlWriter->writeStartElement("DyncVisable");
	pXmlWriter->writeAttribute("Enable", QString::number(this->m_bEnable));
	pXmlWriter->writeAttribute("Variable", this->m_szTagName.data());

	pXmlWriter->writeAttribute("VisableValue", QString::number(this->GetVisableValue()));

	pXmlWriter->writeAttribute("DeadZone", QString::number(this->GetDeadBand()));
	pXmlWriter->writeAttribute("Condition", QString::number(this->GetCondition()));
	pXmlWriter->writeAttribute("FlashingType", QString::number(this->GetFlashType()));
	pXmlWriter->writeAttribute("FlashingTime", QString::number(this->GetFlashTime()));

	pXmlWriter->writeEndElement();
}

bool CDyncVisibleData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}
/** @}*/
