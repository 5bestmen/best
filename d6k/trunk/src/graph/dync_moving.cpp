#include "dync_moving.h"
#include "base_widget.h"
#include "variant.h"
#include "../scdpub/variant_data.h"
#include "stl_util-inl.h"
#include "scadaapi/scdsvcapi.h"
#include "fesapi/fesdatadef.h"
#include <QDebug>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

CDyncMovingData::CDyncMovingData()
	: CBaseDyncData()
{
	m_nEndData = 100;
	m_nEndOffsedData = 1;
	m_nStartOffset = 0;
	m_nStartData = 0;
	m_fXAis = 0;
	m_fYAis = 0;

	//位移属性
	m_dDistance= 100;

	//起始角度
	m_nStartAngle = 0;
	//结束角度
	m_nEndANgle = 360;

	m_bBackFlag = false;
}

CDyncMovingData::~CDyncMovingData()
{
}

void CDyncMovingData::CopyBaseData(CBaseDyncData *src)
{
	CDyncMovingData *tdync = dynamic_cast<CDyncMovingData*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}
	m_bBackFlag = tdync->m_bBackFlag;
	m_nStartOffset = tdync->m_nStartOffset;
	m_nEndOffsedData = tdync->m_nEndOffsedData;
	m_nStartData = tdync->m_nStartData;
	m_nEndData = tdync->m_nEndData;

	m_dDistance = tdync->m_dDistance;
	m_nStartAngle = tdync->m_nStartAngle;
	m_nEndANgle = tdync->m_nEndANgle;

	//父类赋值
	CBaseDyncData::CopyBaseData(src);
}

const CDyncMovingData& CDyncMovingData::operator=(const CDyncMovingData& src)
{
	if (this != &src)
	{
		m_bBackFlag = src.m_bBackFlag;
		m_nStartOffset = src.m_nStartOffset;
		m_nEndOffsedData = src.m_nEndOffsedData;
		m_nStartData = src.m_nStartData;
		m_nEndData = src.m_nEndData;

		m_dDistance = src.m_dDistance;
		m_nStartAngle = src.m_nStartAngle;
		m_nEndANgle = src.m_nEndANgle;
		//父类
		CBaseDyncData::operator=(src);
	}

	return *this;
}

void CDyncMovingData::PrepareWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
		return ;
	//获取最原始坐标数据
	m_fXAis = pWidget->GetPosition().x();
	m_fYAis = pWidget->GetPosition().y();

	CBaseDyncData::PrepareWidgetDync(pWidget);
}

bool CDyncMovingData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	if (pWidget == NULL)
		return false;

	Q_ASSERT(m_bEnable);
	if (m_bEnable == false)
		return false;

	if (m_szTagName.empty())
		return false;

	Q_ASSERT(GetStartData()!=GetEndData());
	Q_ASSERT(GetEndOffset()!=GetStartOffset());

	if (GetEndData()==GetStartData() || GetEndOffset()== GetStartOffset())
	{
		return false;
	}
	CVariant var;
	// 读取实时数据
	//IO_VARIANT rtval;
	double dblValue = 0.0f;

	//static int nIndex = 1;
	//nIndex += 10 ;
	//dblValue = (nIndex);
	/*bool bRet = ::GetRTData(m_pTagInfo->IddType,
		m_pTagInfo->NodeOccNo,
		m_pTagInfo->TagOccNo,
		m_pTagInfo->FiledID, &rtval);*/

	bool bRet = GetRealTimeValue(m_pTagInfo, &var);
	Q_ASSERT(bRet);

	if (!bRet)
 	{
		return false;
	}

	dblValue = var.operator double();

	//先判断值在移动范围之内
	if (dblValue>=GetStartData()  && dblValue<=GetEndData())
	{
		qreal Pos;
		switch (m_nDyncType)
		{		
			case CBaseDyncData::DYNC_HOR_MOVE:
			{
				Pos =(m_fXAis+ dblValue / (GetEndData() - GetStartData()) *(GetEndOffset() - GetStartOffset())) ;
				
				if (Pos > pWidget->scene()->sceneRect().width())
				{
					Pos =pWidget->scene()->sceneRect().width();
				}

				//Pos = int (m_fXAis + dblValue)% 1920;
				pWidget->SetRcPos(QRectF(QPointF(Pos, pWidget->GetPosition().y()),pWidget->GetMovingPosition().size()));
	
				break;
			}		
			case CBaseDyncData::DYNC_VIR_MOVE:
			{
				Pos = (m_fYAis + dblValue / (GetEndData() - GetStartData()) *(GetEndOffset() - GetStartOffset()));
				
				if (Pos > pWidget->scene()->sceneRect().height())
				{
					Pos = pWidget->scene()->sceneRect().height();
				}
				//Pos = int(m_fXAis + dblValue) % 1200;
				pWidget->SetRcPos(QRectF(QPointF( pWidget->GetPosition().y(),Pos), pWidget->GetMovingPosition().size()));
				qDebug() << Pos << pWidget->GetPosition().y() << pWidget->GetMovingPosition().size() << pWidget->pos() << pWidget->IsVisible();
				break;
			}
			case CBaseDyncData::DYNC_GROUP_MOVE:
				break;
			case CBaseDyncData::DYNC_ROTATION:
			{
				qreal dx = dblValue/(GetEndData() - GetStartData()) * (GetEndOffset() - GetStartOffset());
				pWidget->SetRotateAngle(dx);
			}
				break;
			default:
				break;
		}
	}
	else
	{
		return false;
	}

	return true;
}
/*! \fn void CDyncMovingData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
********************************************************************************************************* 
** \brief CDyncMovingData::SaveXml 
** \details 保存动态移动数据
** \param  
** \return void 
** \author LiJin 
** \date 2017年6月8日 
** \note 
********************************************************************************************************/
void CDyncMovingData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;

	if (this->m_nDyncType == CBaseDyncData::DYNC_HOR_MOVE)
	{
		pXmlWriter->writeStartElement("HorizonalMove");
		pXmlWriter->writeAttribute("Diatance", QString::number(this->GetDistance()));
	}
	else if (this->m_nDyncType == CBaseDyncData::DYNC_VIR_MOVE)
	{
		pXmlWriter->writeStartElement("VerticalMove");
		pXmlWriter->writeAttribute("Diatance", QString::number(this->GetDistance()));
	}
	else if (this->m_nDyncType == CBaseDyncData::DYNC_ROTATION)
	{
		pXmlWriter->writeStartElement("RotateMove");
		pXmlWriter->writeAttribute("StartAngel", QString::number(this->GetStartAngle()));
		pXmlWriter->writeAttribute("EndAngel", QString::number(this->GetEndAngel()));
	}

	pXmlWriter->writeAttribute("Enable", QString::number(this->m_bEnable));
	pXmlWriter->writeAttribute("Reverse", QString::number(this->GetBackFlag()));
	pXmlWriter->writeAttribute("Variable", this->m_szTagName.data());

	pXmlWriter->writeAttribute("MoveStart", QString::number(this->GetStartData()));
	pXmlWriter->writeAttribute("MoveEnd", QString::number(this->GetEndData()));
	pXmlWriter->writeAttribute("StartOffset", QString::number(this->GetStartOffset()));
	pXmlWriter->writeAttribute("EndOffset", QString::number(this->GetEndOffset()));

	pXmlWriter->writeEndElement();
}

bool CDyncMovingData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}

CBaseDyncData * CDyncMovingData::CloneDyncData()
{
	CDyncMovingData *pNewDync = new CDyncMovingData;
	*pNewDync = *this;
	return dynamic_cast<CDyncMovingData*> (pNewDync);
}

CDyncGroupMovingData::CDyncGroupMovingData()
{
	m_nStartData = 0;
	//结束值
	m_nEndData = 100;
}

CDyncGroupMovingData::~CDyncGroupMovingData()
{

}

void CDyncGroupMovingData::CopyBaseData(CBaseDyncData *src)
{
	CDyncGroupMovingData *tdync = dynamic_cast<CDyncGroupMovingData*>(src);
	Q_ASSERT(tdync);
	if (tdync == nullptr)
	{
		return;
	}

	m_nStartData = tdync->m_nStartData;
	m_nEndData = tdync->m_nEndData;

	//点集合
	for each (QPointF var in tdync->m_arrPoints)
	{
		InsertPonit(var);
	}

	//父类赋值
	CBaseDyncData::CopyBaseData(src);
}

const CDyncGroupMovingData& CDyncGroupMovingData::operator=(const CDyncGroupMovingData& src)
{
	m_nStartData = src.m_nStartData;
	m_nEndData = src.m_nEndData;

	//点集合
	for each (QPointF var in src.m_arrPoints)
	{
		InsertPonit(var);
	}

	//父类
	CBaseDyncData::operator=(src);

	return *this;
}
/*! \fn void CDyncGroupMovingData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
*********************************************************************************************************
** \brief CDyncGroupMovingData::SaveXml
** \details 保存动态移动数据
** \param
** \return void
** \author LiJin
** \date 2017年6月8日
** \note
********************************************************************************************************/
void CDyncGroupMovingData::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return;

	pXmlWriter->writeStartElement("DyncGroupMove");

	pXmlWriter->writeAttribute("Enable", QString::number(this->m_bEnable));
	pXmlWriter->writeAttribute("Variable", this->m_szTagName.data());

	pXmlWriter->writeAttribute("MoveStart", QString::number(this->GetStartData()));
	pXmlWriter->writeAttribute("MoveEnd", QString::number(this->GetEndData()));

	QString strPoint;
	for each (QPointF var in m_arrPoints)
	{
		pXmlWriter->writeStartElement("GroupPoint");

		strPoint = QString::number(var.x()) + "," + QString::number(var.y());
		pXmlWriter->writeCharacters(strPoint);
		pXmlWriter->writeEndElement();
	}
	
	pXmlWriter->writeEndElement();
}

bool CDyncGroupMovingData::LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader)
{
	return true;
}