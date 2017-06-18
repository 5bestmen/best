/*! @file base_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : base_widget.cpp
�ļ�ʵ�ֹ��� : ����ͼԪ
����         : LiJin
�汾         : V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ����ͼԪ
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "base_widget.h"
#include "graph_file.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <qDebug>

#include <sstream>
 
CPenInfo::CPenInfo()
{
	m_clrPen = Qt::black;
	m_nWidth = 1;
	m_nDashStyle = Qt::SolidLine;

}

CPenInfo::~CPenInfo()
{
}

unsigned int CPenInfo::GetWidth() const
{
	return m_nWidth;
}

CBrushInfo::CBrushInfo()
{
}

CBrushInfo::~CBrushInfo()
{
}
CFontInfo::CFontInfo()
{
}

CFontInfo::~CFontInfo()
{
}

CBaseWidget::CBaseWidget()
{
	InitDefault();
}

CBaseWidget::~CBaseWidget()
{

}

CBaseWidget::CBaseWidget(QRectF pos)
{
	InitDefault();
	m_rcPos = pos;
	m_rcMovingPos = pos;

	qDebug()<< "pos: " << this->pos() ;
	qDebug() << "m_rcPos: " << m_rcPos;

//	setPos(pos.topLeft());
}

void CBaseWidget::InitDefault()
{
	m_nObjType = WIDGET_NULL;
//	m_nClassType = STD_ELEMENT;

	m_bSelected = false;
	m_pGraphicsLayer = nullptr;
// 	m_fZoomFactor = 1.0f;
// 	m_szName = _T("");
 	m_bLocked = false;
	m_bRotate = false;
//	m_nLayerIndex = 0;
// 	m_bVisible = TRUE;
// 	m_3DType = FLAT_STYLE;
// 
  	m_pGroup = nullptr;
// 
// 	m_bIsBlinking = false;
// 	m_nBlinkingCnt = 0;
// 
// 	InitDefaultFont(&m_font);
// 	InitDefaultBrush(&m_brush);
// 	InitDefaultPen(&m_pen);
// 
// 	m_dFillingPercent = 0;
// 
// 	m_nFillingPolicy = CDyncFillingData::FILLING_B2T;
}

void  CBaseWidget::SetLayer(CGraphicsLayer *pLayer)
{
//	Q_ASSERT(nVal < CGraphFile::MAX_LAYER_CNT);
	Q_ASSERT(pLayer);

	m_pGraphicsLayer = pLayer;
	if (pLayer)
	{
		Q_ASSERT(pLayer->GetLayerIdx() > 0 && pLayer->GetLayerIdx() <= CGraphFile::MAX_LAYER_CNT);
		m_nLayerIndex = pLayer->GetLayerIdx();
	}
//	m_nLayerIndex = nVal;
}
/*! \fn void CBaseWidget::MoveHandleTo(int nHandle, const QPointF &point)
********************************************************************************************************* 
** \brief CBaseWidget::MoveHandleTo 
** \details �ƶ�ĳ��צ�ӵ��µ�λ�ã�����ͼԪ�Ĵ�С
** \param nHandle 
** \param point 
** \return void 
** \author LiJin
** \date 2016��2��11�� 
** \note 
********************************************************************************************************/
void CBaseWidget::MoveHandleTo(int nHandle, const QPointF &point)
{
	switch (nHandle)
	{
	case 1:
		m_rcPos.setX(point.x());
		m_rcPos.setY(point.y());
		m_rcPos.setWidth(m_rcPos.right() - point.x());
		m_rcPos.setHeight(m_rcPos.bottom() - point.y());
		break;
	case 2:
		m_rcPos.setHeight( m_rcPos.bottom() - point.y());
		m_rcPos.setY(point.y());
		break;

	case 3:
		m_rcPos.setWidth  (point.x() - m_rcPos.x());
		m_rcPos.setHeight (m_rcPos.bottom() - point.y());
		m_rcPos.setY(point.y());
		break;

	case 4:
		m_rcPos.setWidth  (point.x()- m_rcPos.x());
		break;

	case 5:
		m_rcPos.setWidth  (point.x()- m_rcPos.x());
		m_rcPos.setHeight (point.y() - m_rcPos.y());
		break;

	case 6:
		m_rcPos.setHeight (point.y() - m_rcPos.y());
		break;

	case 7:
		m_rcPos.setX(point.x());
		m_rcPos.setWidth (m_rcPos.right() - point.x());
		m_rcPos.setHeight (point.y() - m_rcPos.y());
		break;
	case 8:
		m_rcPos.setX(point.x());
		m_rcPos.setWidth  (m_rcPos.right() - point.x());
		break;
	}
	m_rcPos = m_rcPos.normalized();
//	m_rcPos.setWidth(point.x() - m_rcPos.x());
//	m_rcPos.setHeight(point.y() - m_rcPos.y());
	m_rcMovingPos = m_rcPos;
}

unsigned int CBaseWidget::GetHandleCount() const
{
	return 8;
}
// ��ȡ��nHandle��Handle�����ĵ�����
QPointF CBaseWidget::GetHandlePos(int nHandle) const
{
	Q_ASSERT(nHandle > 0);

	double x = 0.0, y = 0.0, xCenter = 0.0, yCenter = 0.0;
	QRectF rcBounding = GetPosition();

	xCenter = rcBounding.x() + rcBounding.width() / 2;
	yCenter = rcBounding.y() + rcBounding.height() / 2;

	switch (nHandle)
	{
	case 1:
		x = rcBounding.x();
		y = rcBounding.y();
		break;

	case 2:
		x = xCenter;
		y = rcBounding.y();
		break;

	case 3:
		x = rcBounding.right();
		y = rcBounding.y();
		break;

	case 4:
		x = rcBounding.right();
		y = yCenter;
		break;

	case 5:
		x = rcBounding.right();
		y = rcBounding.bottom();
		break;

	case 6:
		x = xCenter;
		y = rcBounding.bottom();
		break;

	case 7:
		x = rcBounding.x();
		y = rcBounding.bottom();
		break;

	case 8:
		x = rcBounding.x();
		y = yCenter;
		break;
	default:
		Q_ASSERT(false);
		break;
	}

	return QPointF(x, y);
}
/*! \fn QPointF CBaseWidget::GetSpinPos()const
********************************************************************************************************* 
** \brief CBaseWidget::GetSpinPos 
** \details ��ȡͼԪ����ת��ָʾλ��
** \return QT_NAMESPACE::QPointF 
** \author LiJin 
** \date 2016��1��20�� 
** \note 
********************************************************************************************************/
QPointF CBaseWidget::GetSpinHandlePos()const
{
	//double x = 0.0, y = 0.0, xCenter = 0.0, yCenter = 0.0;
	QRectF rcBounding = GetPosition();
	QPointF ptSpin;

	ptSpin.setX(rcBounding.x() + rcBounding.width() / 2);
	ptSpin.setY(rcBounding.y());

	return ptSpin;
}


// ��ȡ��nHandle��Handle������
QRectF CBaseWidget::GetHandleRect(int nHandle) const
{
	QRectF rect;
	QPointF point = GetHandlePos(nHandle);
	rect.setX(point.x() - WIDGET_HANDLE_RECT_HALF_LEN);
	rect.setY(point.y() - WIDGET_HANDLE_RECT_HALF_LEN);
	rect.setWidth(WIDGET_HANDLE_RECT_HALF_LEN * 2);
	rect.setHeight(WIDGET_HANDLE_RECT_HALF_LEN * 2);

	return rect;
}

void CBaseWidget::DrawSingleSelectedTracker(QPainter *painter)
{
	Q_ASSERT(false);
}

void CBaseWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
{
	Q_ASSERT(false);
}


void CBaseWidget::DrawMovingWidget(QPainter *pPaint, bool bSingleSelected)
{
	Q_ASSERT(false);
}

QRectF  CBaseWidget::GetPosition() const
{
	return boundingRect();
}

QRectF  CBaseWidget::GetMovingPosition() const
{
	return m_rcMovingPos;
}

void CBaseWidget::NormalizePosition()
{
	if (m_rcPos.width() < 0)
	{
		m_rcPos.setX(m_rcPos.x() + m_rcPos.width());
		m_rcPos.setWidth( -m_rcPos.width());
	}
	if (m_rcPos.height() < 0)
	{
		m_rcPos.setY( m_rcPos.y() + m_rcPos.height());
		m_rcPos.setHeight( -m_rcPos.height());
	}
	m_rcPos = m_rcPos.normalized();
}

void CBaseWidget::MoveDelta(const QPointF &delta)
{
	qDebug() << delta;
	m_rcPos.translate(delta);
	m_rcMovingPos = m_rcPos;
}

void CBaseWidget::MovingDelta(const QPointF &delta)
{
	qDebug() << delta;
	m_rcMovingPos.translate(delta);
}
 /*! \fn void CBaseWidget::MoveTo(const QPointF & newPos)
********************************************************************************************************* 
** \brief CBaseWidget::MoveTo 
** \details �ƶ����µ�λ��
** \param newPos 
** \return void 
** \author LiJin
** \date 2016��2��15�� 
** \note 
********************************************************************************************************/
void CBaseWidget::MoveTo(const QPointF & newPos)
{
	m_rcPos.moveTo(newPos);
	m_rcMovingPos = m_rcPos;
}

void CBaseWidget::MoveTo(const QRectF & position)
{
	m_rcPos = position;
	m_rcMovingPos = m_rcPos;
}

bool CBaseWidget::SaveXml(QXmlStreamWriter *pXmlWriter) const
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return false;


	return true;
}

bool CBaseWidget::LoadXml(QXmlStreamReader *pXmlReader)
{
	return true;
}

bool CBaseWidget::IsContained(const QRectF &rcBox) const
{
	return rcBox.contains(m_rcPos);
}
/*! \fn void  CBaseWidget::BuildWidgetName(unsigned int nID, unsigned int nLayerID, const std::string &szGraphName,
const std::string &szGraphFolderName)
********************************************************************************************************* 
** \brief CBaseWidget::BuildWidgetName 
** \details ����ͼԪ����ϸ����
** \param nID 
** \param nLayerID 
** \param szGraphName 
** \param szGraphFolderName 
** \return void 
** \author LiJin 
** \date 2016��1��27�� 
** \note 
********************************************************************************************************/
void  CBaseWidget::BuildWidgetName(unsigned int nID, unsigned int nLayerID, const std::string &szGraphName, 
	const std::string &szGraphFolderName)
{
	std::stringstream strTemp;
	if (szGraphFolderName.empty() == true)
	{
		strTemp << "[" << szGraphName << "].[LAYER" << nLayerID << "].[" << m_szName << nID << "]";
		m_szWidgetName = strTemp.str();
	}
	else
	{
		strTemp << "[" << szGraphFolderName << "].[" << szGraphName << "].[LAYER" << nLayerID << "].[" << m_szName << nID << "]";
		m_szWidgetName = strTemp.str();
	}
}
void  CBaseWidget::GetPath(QPainterPath& path)
{
	Q_ASSERT(false);
}
void  CBaseWidget::GetMovingPath(QPainterPath& path)
{
	Q_ASSERT(false);
}
/*! \fn int CBaseWidget::HitTestHandle(const QPointF & point)const
********************************************************************************************************* 
** \brief CBaseWidget::HitTestHandle 
** \details �жϵ������ѡ����ϵ���һ��צ��
** \param point 
** \return int 
** \author LiJin
** \date 2016��2��9�� 
** \note 
********************************************************************************************************/
int CBaseWidget::HitTestHandle(const QPointF & point)const
{
	int nHandles = GetHandleCount();
	QRectF rect;
	for (int nHandle = 1; nHandle <= nHandles; nHandle++)
	{
		rect = GetHandleRect(nHandle);
		if (rect.contains(point))
			return nHandle;
	}
	return 0;
}
void CBaseWidget::SetGroup(CGroupWidget *pGroup)
{
// 	if (pGroup == nullptr)
// 	{
// 		Q_ASSERT(m_pGroup != nullptr);
// 	}
// 	else
// 	{
// 		Q_ASSERT(m_pGroup == nullptr);		
// 	}
	m_pGroup = pGroup;
}
CGroupWidget *CBaseWidget::GetGroup()
{
	return m_pGroup;
}

/** @}*/
