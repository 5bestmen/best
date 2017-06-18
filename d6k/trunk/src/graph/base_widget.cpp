/*! @file base_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : base_widget.cpp
文件实现功能 : 基础图元
作者         : LiJin
版本         : V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基础图元
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "base_widget.h"
#include "graph_file.h"
#include "spin_item.h"
 #include "base_dync.h"
#include "dynamic_factory.h"
#include "group_widget.h"

#include "colour_define.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>

#include <sstream>
#include <QTimer>
 
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

void CPenInfo::ToQPen(QPen & pen)const
{
	pen.setColor(m_clrPen);
	pen.setWidth(m_nWidth);
	
	pen.setStyle(static_cast<Qt::PenStyle>(m_nDashStyle));
}

CBrushInfo::CBrushInfo()
{
	//非渐变
	m_nGradientMode = 0;
	//初始化样式
	m_nBrushStyle = Qt::SolidPattern;

	m_clrStart = XGraphics::LightCoral;
	m_clrStart.setAlpha(255);
}

CBrushInfo::~CBrushInfo()
{
}
  
std::unique_ptr<QBrush>  CBrushInfo::CreateQBrush()
{
	Qt::BrushStyle brstyle = static_cast <Qt::BrushStyle>(m_nBrushStyle);
	if (brstyle == Qt::SolidPattern 
		|| brstyle == Qt::Dense1Pattern
		|| brstyle == Qt::Dense2Pattern
		|| brstyle == Qt::Dense3Pattern
		|| brstyle == Qt::Dense4Pattern
		|| brstyle == Qt::Dense5Pattern
		|| brstyle == Qt::Dense6Pattern
		|| brstyle == Qt::Dense7Pattern
		|| brstyle == Qt::NoBrush
		|| brstyle == Qt::HorPattern
		|| brstyle == Qt::VerPattern
		|| brstyle == Qt::CrossPattern
		|| brstyle == Qt::BDiagPattern
		|| brstyle == Qt::FDiagPattern
		|| brstyle == Qt::DiagCrossPattern
 		)
	{
		std::unique_ptr <QBrush> br = std::make_unique<QBrush>( );
		br->setStyle(brstyle);
		br->setColor(m_clrStart);

		return br;
	}
	else if ( brstyle == Qt::LinearGradientPattern )
	{
		QLinearGradient gradient;
		std::unique_ptr <QBrush> br = std::make_unique<QBrush>(gradient);
		br->setStyle(brstyle);


		return br;
	}
	else if ( brstyle == Qt::RadialGradientPattern  )
	{
		QRadialGradient gradient(50, 50, 50, 50, 50);
		std::unique_ptr <QBrush> br = std::make_unique<QBrush>(gradient);
		br->setStyle(brstyle);


		return br;
	}
	else if (brstyle == Qt::ConicalGradientPattern)
	{
		QConicalGradient gradient;
		std::unique_ptr <QBrush> br = std::make_unique<QBrush>(gradient);
		br->setStyle(brstyle);

		return br;
	}
	else
	{
		Q_ASSERT(false);

		std::unique_ptr <QBrush> br = std::make_unique<QBrush>();
		br->setColor(m_clrStart);

		return br;
	}
}

CFontInfo::CFontInfo()
{
	//中心
	m_nLayout = 0;
}

CFontInfo::~CFontInfo()
{
}

CBaseWidget::CBaseWidget()
{
	m_nStartAngle = 0;
	m_nEndAngle = 120;

	m_eRotateCenter = ROTATE_MIDCENTER;

	InitDefault();
}

CBaseWidget::~CBaseWidget()
{
	for each (CBaseDyncData * var in m_arrDyncs)
	{
		delete var;
	}
}

const CBaseWidget & CBaseWidget::operator=(const CBaseWidget &src)
{
	m_pGroup = nullptr;
	//选择属性不能复制
	//m_bSelected = src.m_bSelected;
	m_bIsBlinking = src.m_bIsBlinking;
	m_nBlinkingCnt = src.m_nBlinkingCnt;
	m_nObjType = src.m_nObjType;
	m_nClassType = src.m_nClassType;

	m_szName = src.m_szName;
	m_szLayerName = src.m_szLayerName;
	m_szGraphName = src.m_szGraphName;
	m_szFolderName = src.m_szFolderName;
	m_nID = src.m_nID;

	m_szWidgetName = src.m_szWidgetName;
	m_rcPos = src.m_rcPos;
	m_rcSavePos = m_rcPos;
	m_rcMovingPos = src.m_rcMovingPos;

	m_rcBounding = src.m_rcBounding;
	m_bLocked = src.m_bLocked;
	m_bRotate = src.m_bRotate;
	m_eRotateCenter = src.m_eRotateCenter;

	m_dblRotateAngle = src.m_dblRotateAngle;
	m_nTabOrder = src.m_nTabOrder;
	m_nDisplayInRun = src.m_nDisplayInRun;

// 	*m_pPen = *src.m_pPen;
// 	*m_pBrush = *src.m_pBrush;
// 	*m_pFont = *src.m_pFont;

	m_szTxt = src.m_szTxt;
	m_nLayerIndex = src.m_nLayerIndex;

	m_PenInfo = src.m_PenInfo;
	m_BrushInfo = src.m_BrushInfo;
	m_FontInfo = src.m_FontInfo;
	m_bVisible = src.m_bVisible;
	//
	m_nStartAngle = src.m_nStartAngle;
	m_nEndAngle = src.m_nEndAngle;

	
	CopyDynamicPro(src.m_arrDyncs);

	return *this;
}

CBaseWidget::CBaseWidget(QRectF pos)
{
	m_nStartAngle = 0;
	m_nEndAngle = 120;
	InitDefault();
	m_rcPos = pos;
	m_rcMovingPos = pos;

	m_eRotateCenter = ROTATE_MIDCENTER;
	m_bPressFlag = false;

}


void CBaseWidget::InitDefault()
{
	m_nObjType = WIDGET_NULL;
//	m_nClassType = STD_ELEMENT;
	m_nID = 0;
	m_bSelected = false;
	m_pGraphicsLayer = nullptr;
// 	m_fZoomFactor = 1.0f;
// 	m_szName = _T("");
 	m_bLocked = false;
	m_bRotate = false;
	m_dblRotateAngle = 0.0f;
//	m_nLayerIndex = 0;
// 	m_bVisible = TRUE;
// 	m_3DType = FLAT_STYLE;
// 
  	m_pGroup = nullptr;

//	m_pPen = std::make_shared<CPenInfo>();
 
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

	m_pSpinItem = std::make_shared<CSpinItem>(this);
	//动态属性
	//InitDyDataModel();
}

int CBaseWidget::GetVersion() const
{
	return 1;
}

CBaseDyncData * CBaseWidget::CreateDynamicPro(int nDyType)
{
	for each (CBaseDyncData * var in m_arrDyncs)
	{
		if (var->m_nDyncType == nDyType)
		{
			return var;
		}
	}

	//当需要获取属性时 创建对象
	CBaseDyncData *pFillData = CDynamicFactory::CreateDynamicAttr(nDyType);
	Q_ASSERT(pFillData);
	if (pFillData == nullptr)
	{
		return nullptr;
	}
	m_arrDyncs.push_back(pFillData);


	return pFillData;
}

void CBaseWidget::CopyDynamicPro(const std::vector<CBaseDyncData *> &arrDyncs)
{
	for each (CBaseDyncData *pDync in arrDyncs)
	{
		CBaseDyncData *pNewDynaPro = CreateDynamicPro(pDync->m_nDyncType);

		pNewDynaPro->CopyBaseData(pDync);
	}

#if 0
	for each (CBaseDyncData *var in arrDyncs)
	{
		if (var->m_nDyncType == CBaseDyncData::DYNC_FILLING)
		{
			//填充
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncFillingData *pFillingData = dynamic_cast<CDyncFillingData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncFillingData*>(var);
			}
		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_PEN_COLOR || var->m_nDyncType == CBaseDyncData::DYNC_TEXT_COLOR)
		{
			//颜色
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncClrData *pFillingData = dynamic_cast<CDyncClrData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncClrData*>(var);
			}

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_HOR_MOVE || var->m_nDyncType == CBaseDyncData::DYNC_VIR_MOVE
			|| var->m_nDyncType == CBaseDyncData::DYNC_ROTATION)
		{
			//移动
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncMovingData *pFillingData = dynamic_cast<CDyncMovingData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncMovingData*>(var);
			}

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_TRANSPARENCY)
		{
			//动态透明
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncTransparencyData *pFillingData = dynamic_cast<CDyncTransparencyData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncTransparencyData*>(var);
			}

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_VISIBLE)
		{
			//动态可见
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncVisibleData *pFillingData = dynamic_cast<CDyncVisibleData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncVisibleData*>(var);
			}

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_VALUE)
		{
			//动态数值
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncValueData *pFillingData = dynamic_cast<CDyncValueData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncValueData*>(var);
			}

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_STRING || var->m_nDyncType == CBaseDyncData::DYNC_MAP)
		{
			//动态文本   动态图片
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncText *pFillingData = dynamic_cast<CDyncText*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncText*>(var);
			}

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_GROUP_MOVE)
		{
			//组合移动
			CBaseDyncData *pNewDynaPro = GetDynamicPro(var->m_nDyncType);

			CDyncGroupMovingData *pFillingData = dynamic_cast<CDyncGroupMovingData*>(pNewDynaPro);

			if (pFillingData != nullptr)
			{
				*pFillingData = *dynamic_cast<CDyncGroupMovingData*>(var);
			}
		}
	}
#endif
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
** \details 移动某个爪子到新的位置，调整图元的大小
** \param nHandle 
** \param point 
** \return void 
** \author LiJin
** \date 2016年2月11日 
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
// 获取第nHandle个Handle的中心点坐标
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
** \details 获取图元的旋转的指示位置
** \return QT_NAMESPACE::QPointF 
** \author LiJin 
** \date 2016年1月20日 
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

//获取循转后的循转标识位置
QPointF CBaseWidget::GetSpinMovingHandlePos() const
{
    //double x = 0.0, y = 0.0, xCenter = 0.0, yCenter = 0.0;
    QRectF rcBounding = GetPosition();
    QPointF ptSpin;

    ptSpin.setX(rcBounding.x() + rcBounding.width() / 2);
    ptSpin.setY(rcBounding.y()-30);



        QPointF tRoundPf;
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        tRoundPf = tr_temp.map(ptSpin);
        
        return tRoundPf;


    return ptSpin;
}


/*! \fn QRectF CBaseWidget::GetRawHandleRect(int nHandle) const
********************************************************************************************************* 
** \brief CBaseWidget::GetRawHandleRect 
** \details 获取第nHandle个Handle的坐标-原始-旋转前
** \param nHandle 
** \return QT_NAMESPACE::QRectF 
** \author LiJin 
** \date 2016年3月21日 
** \note 
********************************************************************************************************/
QRectF CBaseWidget::GetRawHandleRect(int nHandle) const
{
	QRectF rcHandle;	
	QPointF point = GetHandlePos(nHandle);
	rcHandle.setX(point.x() - WIDGET_HANDLE_RECT_HALF_LEN);
	rcHandle.setY(point.y() - WIDGET_HANDLE_RECT_HALF_LEN);
	rcHandle.setWidth(WIDGET_HANDLE_RECT_HALF_LEN * 2);
	rcHandle.setHeight(WIDGET_HANDLE_RECT_HALF_LEN * 2);
	return rcHandle;
}
/*! \fn QPainterPath CBaseWidget::GetHandlePath(int nHandle)const
********************************************************************************************************* 
** \brief CBaseWidget::GetHandlePath 
** \details 获取第nHandle个Handle的坐标-旋转后
** \param nHandle 
** \return QT_NAMESPACE::QPainterPath 
** \author LiJin 
** \date 2016年3月21日 
** \note 
********************************************************************************************************/
QPainterPath CBaseWidget::GetHandlePath(int nHandle)const
{
	QRectF rcHandle;
	QPainterPath p1, p2;

	QPointF point = GetHandlePos(nHandle);
	rcHandle.setX(point.x() - WIDGET_HANDLE_RECT_HALF_LEN);
	rcHandle.setY(point.y() - WIDGET_HANDLE_RECT_HALF_LEN);
	rcHandle.setWidth(WIDGET_HANDLE_RECT_HALF_LEN * 2);
	rcHandle.setHeight(WIDGET_HANDLE_RECT_HALF_LEN * 2);

	if (m_dblRotateAngle != 0.0f)
	{
		QPointF ptCenter = GetRotationCenterPoint();
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(ptCenter.x(), ptCenter.y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		tr_temp.translate(-ptCenter.x(), -ptCenter.y());

		p1.addRect(rcHandle);
		p2 = tr_temp.map(p1);
		return p2;
	}
	else
	{
		p2.addRect(rcHandle);
		return p2;
	}
}

QPointF CBaseWidget::GetRotationCenterPoint()const
{
/*	return m_rcPos.center();*/
	QPointF ptCenter;

	if (m_eRotateCenter == ROTATE_TOPLEFT)
	{
		//左上角
		ptCenter = m_rcPos.topLeft();
	}
	else if (m_eRotateCenter == ROTATE_TOPMID)
	{
		//中上
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.topLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_TOPRIGHT)
	{
		//右上
		ptCenter = m_rcPos.topRight();
	}
	else if (m_eRotateCenter == ROTATE_LEFTMID)
	{
		//左中
		ptCenter = QPointF(m_rcPos.topLeft().x(), m_rcPos.center().y());;
	}
	else if (m_eRotateCenter == ROTATE_MIDCENTER)
	{
		//中心
		ptCenter = m_rcPos.center();
	}
	else if (m_eRotateCenter == ROTATE_RIGHTMID)
	{
		//右中
		ptCenter = QPointF(m_rcPos.topRight().x(), m_rcPos.center().y());
	}
	else if (m_eRotateCenter == ROTATE_LEFTBOTTOM)
	{
		//左下角
		ptCenter = m_rcPos.bottomLeft();
	}
	else if (m_eRotateCenter == ROTATE_BOTTOMMID)
	{
		//下中
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.bottomLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_RIGHTBOTTOM)
	{
		//右下
		ptCenter = QPointF(m_rcPos.bottomRight());
	}

	return ptCenter;
} 

QPointF CBaseWidget::GetMovingRotationCenterPoint() const
{
	QPointF ptCenter;

	if (m_eRotateCenter == ROTATE_TOPLEFT)
	{
		//左上角
		ptCenter = m_rcMovingPos.topLeft();
	}
	else if (m_eRotateCenter == ROTATE_TOPMID)
	{
		//中上
		ptCenter = QPointF(m_rcMovingPos.center().x(), m_rcMovingPos.topLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_TOPRIGHT)
	{
		//右上
		ptCenter = m_rcMovingPos.topRight();
	}
	else if (m_eRotateCenter == ROTATE_LEFTMID)
	{
		//左中
		ptCenter = QPointF(m_rcMovingPos.topLeft().x(), m_rcMovingPos.center().y());;
	}
	else if (m_eRotateCenter == ROTATE_MIDCENTER)
	{
		//中心
		ptCenter = m_rcMovingPos.center();
	}
	else if (m_eRotateCenter == ROTATE_RIGHTMID)
	{
		//右中
		ptCenter = QPointF(m_rcMovingPos.topRight().x(), m_rcMovingPos.center().y());
	}
	else if (m_eRotateCenter == ROTATE_LEFTBOTTOM)
	{
		//左下角
		ptCenter = m_rcMovingPos.bottomLeft();
	}
	else if (m_eRotateCenter == ROTATE_BOTTOMMID)
	{
		//下中
		ptCenter = QPointF(m_rcMovingPos.center().x(), m_rcMovingPos.bottomLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_RIGHTBOTTOM)
	{
		//右下
		ptCenter = QPointF(m_rcMovingPos.bottomRight());
	}

	return ptCenter;
}

/*! \fn void CBaseWidget::DrawSingleSelectedTracker(QPainter *pPainter)
********************************************************************************************************* 
** \brief CBaseWidget::DrawSingleSelectedTracker 
** \details 画单选状态的外选择框
** \param pPainter 
** \return void 
** \author LiJin 
** \date 2016年3月21日 
** \note 
********************************************************************************************************/
void CBaseWidget::DrawSingleSelectedTracker(QPainter *pPainter)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	pPainter->save();

	QPointF ptCenter = GetRotationCenterPoint();
	
	if (m_dblRotateAngle != 0.0f)
	{
		pPainter->translate(ptCenter);
		pPainter->rotate(m_dblRotateAngle);
		pPainter->translate(-ptCenter);
	}

	QRectF rcFrame = this->GetPosition();  
	// 框
	QPen pen(Qt::darkGray);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);

	// 爪
	unsigned int nHandles = GetHandleCount();

	QPointF ptSpinStart = GetSpinHandlePos(); // 靠近矩形框（图元）的起始点
	QPointF ptSpinEnd(ptSpinStart);
	ptSpinEnd.setY(ptSpinStart.y() - 15);
	QLine lnSpin(ptSpinStart.toPoint(), ptSpinEnd.toPoint());

	if (static_cast<int>(m_dblRotateAngle) % 90 != 0)
	{
    //绕一圈后  会产生锯齿
		pPainter->setRenderHint(QPainter::Antialiasing);
	}

	pPainter->setPen(pen);

    
	// 画外部矩形框
	pPainter->drawRect(rcFrame);

	pPainter->drawLine(lnSpin);
	unsigned int i = 0;
	QRectF rcHandler;

	//QBrush br(Qt::green);
	for (i = 1; i <= nHandles; i++)
	{
		rcHandler = GetRawHandleRect(i);
		pPainter->setBrush(Qt::white);
		pPainter->setPen(Qt::darkGray);
		pPainter->drawRect(rcHandler);
	}

	pPainter->restore();
	// 画旋转的旋钮
	DrawSpinItem(pPainter);
}

void CBaseWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;


    QPointF ptCenter = GetRotationCenterPoint();
    pPainter->save();

    if (m_dblRotateAngle != 0.0f)
    {
        pPainter->translate(ptCenter);
        pPainter->rotate(m_dblRotateAngle);
        pPainter->translate(-ptCenter);
        pPainter->setRenderHint(QPainter::Antialiasing);
    }

	QRectF rcFrame = this->GetPosition(); 
	//	rcFrame.adjust(-1, -1, 1, 1);
	// 框
	QPen pen(XGraphics::DarkBlue);
	if (bFirstSelected)
	{
		pen.setWidth(3);
	}
	else
	{
		pen.setWidth(2);
	}
	pen.setStyle(Qt::SolidLine);

	pPainter->setPen(pen);
	// 画外部矩形框
	pPainter->drawRect(rcFrame);

	pPainter->restore();
}


void CBaseWidget::DrawMovingWidget(QPainter *pPaint, bool bSingleSelected)
{
	Q_UNUSED(pPaint);
	Q_UNUSED(bSingleSelected);
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

QRectF  CBaseWidget::GetClipRect() const
{
	return boundingRect();
}

QPainterPath CBaseWidget::shape() const
{
	QPainterPath p1, p2;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QPointF ptCenter = GetRotationCenterPoint();		
		QTransform tr_temp; 
		tr_temp.reset();
		tr_temp.translate(ptCenter.x(), ptCenter.y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
	//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-ptCenter.x(), -ptCenter.y());

		p1.addRect(rcInput);
		p2 = tr_temp.map(p1);
	}
	else
	{
		p2.addRect(rcInput);
	}
	return p2;
}

QRectF CBaseWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();
// 	double dPenWidth = m_PenInfo.GetWidth() / 2;
// 
// 	if (dPenWidth > 0.0f)
// 	{	// 扩大半个线宽
// 		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
// 	}
	return rcBounding;
}
QRectF CBaseWidget::RotateRect(const QRectF & rcInput, double dblAngle)
{
	QRectF rcRtn = rcInput;
	if (dblAngle != 0)
	{
	}
	return rcRtn;
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
	m_rcPos.translate(delta);
	m_rcMovingPos = m_rcPos;
}

void CBaseWidget::MovingDelta(const QPointF &delta)
{
	m_rcMovingPos.translate(delta);
}

//循转到新位置
void CBaseWidget::RotatingDelta(const QPointF &delta)
{
    QPointF ptCenter = GetRotationCenterPoint();

//     double angle_tanValue = (delta.y()-ptCenter.y())/(delta.x()-ptCenter.x());
//     //求出tan 与 sin 的弧度  
//     double angle_atanValue = atan(angle_tanValue);
//     //弧度转换成角度  
//     double angle = angle_atanValue * 180 / 3.1415;
// 
//     if (delta.x() >= ptCenter.x())
//     {
//         //取绝对值  
//         m_dblRotateAngle = angle + 90;
//     }
//     else
//     {
// 
//         m_dblRotateAngle = angle + 270;
// 
//     }

	double nStart = 90;
	if (m_eRotateCenter == ROTATE_TOPLEFT)
	{
		//左上
		nStart = atan(20.0/this->GetPosition().width()/2.0) * 180 / 3.141592;
	}
	else if (m_eRotateCenter == ROTATE_LEFTMID)
	{
		//左中
		nStart = atan((20.0 + this->GetPosition().height() / 2.0) / this->GetPosition().width() / 2.0) * 180 / 3.141592;
	}
	else if (m_eRotateCenter == ROTATE_LEFTBOTTOM)
	{
		//左下
		nStart = atan((20.0 + this->GetPosition().height()) / this->GetPosition().width() / 2.0) * 180 / 3.141592;
	}
	else if (m_eRotateCenter == ROTATE_TOPRIGHT)
	{
		//右上
		nStart = 180-atan((20.0 + this->GetPosition().height()) / this->GetPosition().width() / 2.0) * 180 / 3.141592;
	}
	else if (m_eRotateCenter == ROTATE_RIGHTMID)
	{
		//右上
		nStart = 180 -atan((20.0 + this->GetPosition().height() / 2.0) / this->GetPosition().width() / 2.0) * 180 / 3.141592;
	}
	else if (m_eRotateCenter == ROTATE_RIGHTBOTTOM)
	{
		//右下
		nStart = 180 -atan((20.0 + this->GetPosition().height()) / this->GetPosition().width() / 2.0) * 180 / 3.141592;
	}
    
	if (delta.x() >= ptCenter.x() && delta.y() >= ptCenter.y())
	{
		//区间1
		m_dblRotateAngle = asin(1.0*(delta.y() - ptCenter.y()) / (sqrt((delta.x() - ptCenter.x())*(delta.x() - ptCenter.x()) +
			(delta.y() - ptCenter.y())*(delta.y() - ptCenter.y())))) * 180 / 3.141592 + nStart;
	}
	else if (delta.x() <= ptCenter.x() && delta.y() >= ptCenter.y())
	{
		//区间2
		m_dblRotateAngle = 180 - asin(1.0*(delta.y() - ptCenter.y()) / (sqrt((delta.x() - ptCenter.x())*(delta.x() - ptCenter.x()) +
			(delta.y() - ptCenter.y())*(delta.y() - ptCenter.y())))) * 180 / 3.141592 + nStart;
	}
	else if (delta.x() <= ptCenter.x() && delta.y() <= ptCenter.y())
	{
		//区间3
		m_dblRotateAngle = 180 + asin(1.0*(ptCenter.y() - delta.y()) / (sqrt((delta.x() - ptCenter.x())*(delta.x() - ptCenter.x()) +
			(delta.y() - ptCenter.y())*(delta.y() - ptCenter.y())))) * 180 / 3.141592 + nStart;
	}
	else if (delta.x() >= ptCenter.x() && delta.y() <= ptCenter.y())
	{
		//区间4
		m_dblRotateAngle = nStart - asin(1.0*(ptCenter.y() - delta.y()) / (sqrt((delta.x() - ptCenter.x())*(delta.x() - ptCenter.x()) +
			(delta.y() - ptCenter.y())*(delta.y() - ptCenter.y())))) * 180 / 3.141592;
	}


	m_dblRotateAngle = m_dblRotateAngle - ((int)m_dblRotateAngle/360)*360;
}

 /*! \fn void CBaseWidget::MoveTo(const QPointF & newPos)
********************************************************************************************************* 
** \brief CBaseWidget::MoveTo 
** \details 移动到新的位置
** \param newPos 
** \return void 
** \author LiJin
** \date 2016年2月15日 
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

/*! \fn bool CBaseWidget::SaveXml(std::shared_ptr<QXmlStreamWriter> pXmlWriter) const
********************************************************************************************************* 
** \brief CBaseWidget::SaveXml 
** \details 保存图元信息 todo 今后把这部分挪到序列号中，在派生的图元中只保存派生信息
** \param pXmlWriter 
** \return bool 
** \author LiJin 
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CBaseWidget::SaveXml(std::shared_ptr<QXmlStreamWriter> pXmlWriter)
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return false;

	pXmlWriter->writeStartElement("Widget");
	pXmlWriter->writeAttribute("Type", QString::number(m_nObjType)); 
	//暂时没有用
	//pXmlWriter->writeAttribute("ClassType", QString::number(m_nClassType)); 
	pXmlWriter->writeAttribute("ID",QString::number(m_nID));
	//zvalue
	pXmlWriter->writeAttribute("ZValue", QString::number(this->zValue()));
	pXmlWriter->writeAttribute("Name", m_szName.c_str());
	pXmlWriter->writeAttribute("WidgetName", QString::fromLocal8Bit(m_szWidgetName.c_str()));
	pXmlWriter->writeAttribute("Pos.x", QString::number(m_rcPos.x()));
	pXmlWriter->writeAttribute("Pos.y", QString::number(m_rcPos.y()));
	//大小
	pXmlWriter->writeAttribute("Width", QString::number(m_rcPos.width()));
	pXmlWriter->writeAttribute("Height", QString::number(m_rcPos.height()));
	//旋转中心
	pXmlWriter->writeAttribute("RotateCenter", QString::number(m_eRotateCenter));
	//旋转角度
	pXmlWriter->writeAttribute("Rotate", QString::number(m_dblRotateAngle));
	//结束角度
	pXmlWriter->writeAttribute("EndAngle",QString::number(m_nEndAngle));
	//锁定
	pXmlWriter->writeAttribute("Lock", QString::number(m_bLocked));
	//静态属性
	SaveBaseXml(pXmlWriter);
	//动态属性
	SaveDynamicXml(pXmlWriter);

	//组合属性加载
	if (GetWidgetType() == WIDGET_GROUP)
	{
		CGroupWidget *pGroupWgt = (CGroupWidget *)this;
		Q_ASSERT(pGroupWgt);

		if (pGroupWgt == nullptr)
		{
			return false;
		}
		std::vector <CBaseWidget *> arrBaseWgt;

		for (int i = 0; i < pGroupWgt->GetWidgetCount(); i++)
		{
			pGroupWgt->GetWidget(i)->SaveXml(pXmlWriter);
		}
	}

	pXmlWriter->writeEndElement();


	return true;
}

//保存静态数据
bool CBaseWidget::SaveBaseXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) 
{
	//pen静态属性
	pXmlWriter->writeStartElement("BasePen");
	pXmlWriter->writeAttribute("Color",m_PenInfo.GetColor().name(QColor::HexArgb));
	pXmlWriter->writeAttribute("PenWidth", QString::number(m_PenInfo.GetWidth()));
	pXmlWriter->writeAttribute("PenType", QString::number(m_PenInfo.GetStyle()));
	pXmlWriter->writeEndElement();

	//brush静态属性
	pXmlWriter->writeStartElement("BaseBrush");
	pXmlWriter->writeAttribute("Color", m_BrushInfo.GetColor().name(QColor::HexArgb));
	pXmlWriter->writeAttribute("BrushType", QString::number(m_BrushInfo.GetStyle()));
	pXmlWriter->writeAttribute("GradientType",QString::number(m_BrushInfo.GetGradiendMode()));
	pXmlWriter->writeAttribute("GradientColor",m_BrushInfo.GetEndColor().name(QColor::HexArgb));
	pXmlWriter->writeEndElement();

	//文字属性
	pXmlWriter->writeStartElement("BaseText");
	pXmlWriter->writeAttribute("Color", m_FontInfo.GetColor().name(QColor::HexArgb));
	pXmlWriter->writeAttribute("Font", m_FontInfo.GetFont().toString());
	pXmlWriter->writeAttribute("Layout",QString::number(m_FontInfo.GetFontLayout()));
	pXmlWriter->writeAttribute("Text", m_szTxt);

	pXmlWriter->writeEndElement();

	//背景图片属性

	pXmlWriter->writeStartElement("ItemBackgroud");

	pXmlWriter->writeAttribute("Item-background-Image", m_ImageInfo.GetBackgroundImage());
	pXmlWriter->writeAttribute("Item-Image.x", QString::number(m_ImageInfo.GetImagePosition().x()));;
	pXmlWriter->writeAttribute("Item-Image.y", QString::number(m_ImageInfo.GetImagePosition().y()));;

	pXmlWriter->writeAttribute("Item-Image.width", QString::number(m_ImageInfo.GetImageSize().width()));;
	pXmlWriter->writeAttribute("Item-Image.height", QString::number(m_ImageInfo.GetImageSize().height()));;

	int bTiling = 0;
	if (m_ImageInfo.GetbTiling())
	{
		bTiling = 1;
	}
	else
	{
		bTiling = 0;
	}
	pXmlWriter->writeAttribute("Item-Tiling", QString::number(bTiling));;

	int bStretch = 0;
	if (m_ImageInfo.GetStretch())
	{
		bStretch = 1;
	}
	else
	{
		bStretch = 0;
	}
	pXmlWriter->writeAttribute("Item-Stretch", QString::number(bStretch));

	pXmlWriter->writeEndElement();

	//end
	return true;
}

//保存动态数据
bool CBaseWidget::SaveDynamicXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter)
{	
	for each (CBaseDyncData * pDync in m_arrDyncs)
	{
		Q_ASSERT(pDync);
		if (pDync == nullptr)
			continue;

		if (pDync->m_bEnable == false)	 
			continue;		 

		pDync->SaveXml(pXmlWriter);
	}
#if 0
	for each (CBaseDyncData * var in m_arrDyncs)
	{
		if (var->m_bEnable == false)
		{
			continue;
		}

		if (var->m_nDyncType == CBaseDyncData::DYNC_FILLING || var->m_nDyncType == CBaseDyncData::DYNC_PEN_COLOR
			|| var->m_nDyncType == CBaseDyncData::DYNC_TEXT_COLOR)
		{
			CDyncClrData *pColorData = dynamic_cast<CDyncClrData*>(var);

			Q_ASSERT(pColorData);
			if (pColorData == nullptr)
			{
				return false;
			}

			if (var->m_nDyncType == CBaseDyncData::DYNC_FILLING)
			{
				pXmlWriter->writeStartElement("DynamicBrush");
				//模拟色
				CDyncFillingData *pFillinigData = dynamic_cast<CDyncFillingData*>(var);

				Q_ASSERT(pFillinigData);
				if (pFillinigData == nullptr)
				{
					return false;
				}

				//模拟色
				pXmlWriter->writeAttribute("AnalogColor", QString::number(pFillinigData->m_bAnalogColor));
				//最大值
				pXmlWriter->writeAttribute("StartValue", QString::number(pFillinigData->m_dMinVal));
				pXmlWriter->writeAttribute("EndValue", QString::number(pFillinigData->m_dMaxVal));
				pXmlWriter->writeAttribute("StartOffset", QString::number(pFillinigData->m_dStartPos));
				pXmlWriter->writeAttribute("EndOffset", QString::number(pFillinigData->m_dEndPos));
				//模拟色类型
				pXmlWriter->writeAttribute("ColorType", QString::number(pFillinigData->m_nFillingPolicy));
			}
			else if (var->m_nDyncType == CBaseDyncData::DYNC_PEN_COLOR)
			{
				pXmlWriter->writeStartElement("DynamicPenColor");
			}
			else if (var->m_nDyncType == CBaseDyncData::DYNC_TEXT_COLOR)
			{
				pXmlWriter->writeStartElement("DynamicTextColor");
			}
			
			pXmlWriter->writeAttribute("Enable", QString::number(var->m_bEnable));
			pXmlWriter->writeAttribute("Variable", var->m_szTagName.data());

			for (auto it : pColorData->m_arrColors)
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
		else if (var->m_nDyncType == CBaseDyncData::DYNC_HOR_MOVE || var->m_nDyncType == CBaseDyncData::DYNC_VIR_MOVE
			|| var->m_nDyncType == CBaseDyncData::DYNC_ROTATION)
		{
			//动态移动
			CDyncMovingData *pMovingData = dynamic_cast<CDyncMovingData*>(var);

			Q_ASSERT(pMovingData);
			if (pMovingData == nullptr)
			{
				return false;
			}

			if (var->m_nDyncType == CBaseDyncData::DYNC_HOR_MOVE)
			{
				pXmlWriter->writeStartElement("HorizonalMove");
				pXmlWriter->writeAttribute("Diatance", QString::number(pMovingData->GetDistance()));
			}
			else if (var->m_nDyncType == CBaseDyncData::DYNC_VIR_MOVE)
			{
				pXmlWriter->writeStartElement("VerticalMove");
				pXmlWriter->writeAttribute("Diatance", QString::number(pMovingData->GetDistance()));
			}
			else if (var->m_nDyncType == CBaseDyncData::DYNC_ROTATION)
			{
				pXmlWriter->writeStartElement("RotateMove");
				pXmlWriter->writeAttribute("StartAngel", QString::number(pMovingData->GetStartAngle()));
				pXmlWriter->writeAttribute("EndAngel", QString::number(pMovingData->GetEndAngel()));
			}

			pXmlWriter->writeAttribute("Enable", QString::number(var->m_bEnable));
			pXmlWriter->writeAttribute("Reverse",QString::number(pMovingData->GetBackFlag()));
			pXmlWriter->writeAttribute("Variable", var->m_szTagName.data());

			pXmlWriter->writeAttribute("MoveStart", QString::number(pMovingData->GetStartData()));
			pXmlWriter->writeAttribute("MoveEnd", QString::number(pMovingData->GetEndData()));
			pXmlWriter->writeAttribute("StartOffset", QString::number(pMovingData->GetStartOffset()));
			pXmlWriter->writeAttribute("EndOffset", QString::number(pMovingData->GetEndOffset()));

			pXmlWriter->writeEndElement();
		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_GROUP_MOVE)
		{
			//组合移动
			CDyncGroupMovingData *pMovingData = dynamic_cast<CDyncGroupMovingData*>(var);

			Q_ASSERT(pMovingData);
			if (pMovingData == nullptr)
			{
				return false;
			}


			pXmlWriter->writeStartElement("DyncGroupMove");

			pXmlWriter->writeAttribute("Enable", QString::number(var->m_bEnable));
			pXmlWriter->writeAttribute("Variable", var->m_szTagName.data());

			pXmlWriter->writeAttribute("MoveStart", QString::number(pMovingData->GetStartData()));
			pXmlWriter->writeAttribute("MoveEnd", QString::number(pMovingData->GetEndData()));


			for each (QPointF var in pMovingData->GetPoints())
			{
				pXmlWriter->writeStartElement("GroupPoint");

				QString strPoint = QString::number(var.x()) + "," + QString::number(var.y());
				pXmlWriter->writeCharacters(strPoint);
				pXmlWriter->writeEndElement();
			}


			pXmlWriter->writeEndElement();

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_TRANSPARENCY)
		{
			//透明
			CDyncTransparencyData *pTransData = dynamic_cast<CDyncTransparencyData*>(var);

			Q_ASSERT(pTransData);
			if (pTransData == nullptr)
			{
				return false;
			}

			pXmlWriter->writeStartElement("Transparency");
			pXmlWriter->writeAttribute("Enable", QString::number(pTransData->m_bEnable));
			pXmlWriter->writeAttribute("Variable", pTransData->m_szTagName.data());

			pXmlWriter->writeAttribute("TransStart", QString::number(pTransData->GetMinValue()));
			pXmlWriter->writeAttribute("TransEnd", QString::number(pTransData->GetMaxValue()));
			pXmlWriter->writeEndElement();
		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_VISIBLE)
		{
			//可见
			CDyncVisibleData *pVisableData = dynamic_cast<CDyncVisibleData*>(var);

			Q_ASSERT(pVisableData);
			if (pVisableData == nullptr)
			{
				return false;
			}

			pXmlWriter->writeStartElement("DyncVisable");
			pXmlWriter->writeAttribute("Enable", QString::number(pVisableData->m_bEnable));
			pXmlWriter->writeAttribute("Variable", pVisableData->m_szTagName.data());

			pXmlWriter->writeAttribute("VisableValue", QString::number(pVisableData->GetVisableValue()));

			pXmlWriter->writeAttribute("DeadZone", QString::number(pVisableData->GetDeadBand()));
			pXmlWriter->writeAttribute("Condition", QString::number(pVisableData->GetCondition()));
			pXmlWriter->writeAttribute("FlashingType", QString::number(pVisableData->GetFlashType()));
			pXmlWriter->writeAttribute("FlashingTime", QString::number(pVisableData->GetFlashTime()));

			pXmlWriter->writeEndElement();

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_VALUE)
		{
			//数值
			CDyncValueData *pValueData = dynamic_cast<CDyncValueData*>(var);

			Q_ASSERT(pValueData);
			if (pValueData == nullptr)
			{
				return false;
			}

			pXmlWriter->writeStartElement("DyncValue");
			pXmlWriter->writeAttribute("Enable", QString::number(pValueData->m_bEnable));
			pXmlWriter->writeAttribute("Variable", pValueData->m_szTagName.data());

			pXmlWriter->writeAttribute("DynamicValue", QString::number(pValueData->GetFormat()));

			pXmlWriter->writeEndElement();

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_STRING)
		{
			//动态文本
			CDyncText *pTextData = dynamic_cast<CDyncText*>(var);

			Q_ASSERT(pTextData);
			if (pTextData == nullptr)
			{
				return false;
			}

		    pXmlWriter->writeStartElement("DynamicTextContext");
			
			pXmlWriter->writeAttribute("Enable", QString::number(pTextData->m_bEnable));
			pXmlWriter->writeAttribute("Variable", pTextData->m_szTagName.data());

			for (auto it : pTextData->m_arrTexts)
			{
				pXmlWriter->writeStartElement("TextRange");

				pXmlWriter->writeAttribute("Source", it->GetorigData());
				pXmlWriter->writeAttribute("Desc", it->GetDescData());

				pXmlWriter->writeEndElement();
			}

			pXmlWriter->writeEndElement();

		}
		else if (var->m_nDyncType == CBaseDyncData::DYNC_MAP)
		{
			//动态图形
			CDyncText *pTextData = dynamic_cast<CDyncText*>(var);

			Q_ASSERT(pTextData);
			if (pTextData == nullptr)
			{
				return false;
			}

			pXmlWriter->writeStartElement("DynamicMap");

			pXmlWriter->writeAttribute("Enable", QString::number(pTextData->m_bEnable));
			pXmlWriter->writeAttribute("Variable", pTextData->m_szTagName.data());

			for (auto it : pTextData->m_arrTexts)
			{
				pXmlWriter->writeStartElement("TextRange");

				pXmlWriter->writeAttribute("Source", it->GetorigData());
				pXmlWriter->writeAttribute("Desc", it->GetDescData());

				pXmlWriter->writeEndElement();
			}

			pXmlWriter->writeEndElement();

		}
	}
#endif

	return true;
}


bool CBaseWidget::LoadXml(QXmlStreamReader *pXmlReader)
{
	Q_UNUSED(pXmlReader);
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
** \details 生产图元的详细名称
** \param nID 
** \param nLayerID 
** \param szGraphName 
** \param szGraphFolderName 
** \return void 
** \author LiJin 
** \date 2016年1月27日 
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
	Q_UNUSED(path);
	Q_ASSERT(false);
}

void  CBaseWidget::GetMovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
	Q_ASSERT(false);
}
/*! \fn int CBaseWidget::HitTestHandle(const QPointF & point)const
********************************************************************************************************* 
** \brief CBaseWidget::HitTestHandle 
** \details 判断点击的是选择框上的哪一个爪子
** \param point 
** \return int 
** \author LiJin
** \date 2016年2月9日 
** \note 
********************************************************************************************************/
int CBaseWidget::HitTestHandle(const QPointF & point)const
{
	int nHandles = GetHandleCount();
	QPainterPath p1;
	for (int nHandle = 1; nHandle <= nHandles; nHandle++)
	{
		p1 = GetHandlePath(nHandle);
		if (p1.contains(point))
			return nHandle;
	}
	return 0;
}

bool  CBaseWidget::HitTest(const QPointF &ptInput) const
{
	QPainterPath sp = shape();

	return sp.contains(ptInput);
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
/*! \fn void CBaseWidget::DrawSpinItem(QPainter *pPainter)
********************************************************************************************************* 
** \brief CBaseWidget::DrawSpinItem 
** \details 画旋钮
** \param pPainter 
** \return void 
** \author LiJin 
** \date 2016年3月21日 
** \note 
********************************************************************************************************/
void CBaseWidget::DrawSpinItem(QPainter *pPainter)
{
	Q_ASSERT(m_pSpinItem != nullptr);
	if (m_pSpinItem != nullptr)
	{
		m_pSpinItem->DrawSpin(pPainter);
	}
}
// void CBaseWidget::InitDyDataModel()
// {
// 	//填充颜色
// 	CBaseDyncData *pFillData = CDynamicFactory::CreateDynamicAttr(CBaseDyncData::DYNC_FILLING);
// 	Q_ASSERT(pFillData);
// 	if (pFillData == nullptr)
// 	{
// 		return;
// 	}
// 	m_arrDyncs.push_back(pFillData);
// 
// 	//pen颜色
// 	CBaseDyncData *pPenData = CDynamicFactory::CreateDynamicAttr(CBaseDyncData::DYNC_PEN_COLOR);
// 	Q_ASSERT(pPenData);
// 	if (pPenData == nullptr)
// 	{
// 		return;
// 	}
// 	m_arrDyncs.push_back(pPenData);
// 
// 	//textcolor
// 	CBaseDyncData *pTextColorData = CDynamicFactory::CreateDynamicAttr(CBaseDyncData::DYNC_TEXT_COLOR);
// 	Q_ASSERT(pTextColorData);
// 	if (pTextColorData == nullptr)
// 	{
// 		return;
// 	}
// 	m_arrDyncs.push_back(pTextColorData);
// 
// 	//水平移动
// 	CBaseDyncData *pHMovingData = CDynamicFactory::CreateDynamicAttr(CBaseDyncData::DYNC_HOR_MOVE);
// 	Q_ASSERT(pHMovingData);
// 	if (pHMovingData == nullptr)
// 	{
// 		return;
// 	}
// 	m_arrDyncs.push_back(pHMovingData);
// 
// 	//竖直移动
// 	CBaseDyncData *pVMovingData = CDynamicFactory::CreateDynamicAttr(CBaseDyncData::DYNC_VIR_MOVE);
// 	Q_ASSERT(pVMovingData);
// 	if (pVMovingData == nullptr)
// 	{
// 		return;
// 	}
// 	m_arrDyncs.push_back(pVMovingData);
// 
// 	//旋转
// 	CBaseDyncData *pRotateData = CDynamicFactory::CreateDynamicAttr(CBaseDyncData::DYNC_ROTATION);
// 	Q_ASSERT(pRotateData);
// 	if (pRotateData == nullptr)
// 	{
// 		return;
// 	}
// 	m_arrDyncs.push_back(pRotateData);
// 
// }
/*! \fn void CBaseWidget::DrawSelectedTracker(QPainter *pPainter)
********************************************************************************************************* 
** \brief CBaseWidget::DrawSelectedTracker 
** \details 画选择外框
** \param pPainter 
** \return void 
** \author LiJin 
** \date 2016年3月21日 
** \note 
********************************************************************************************************/
void CBaseWidget::DrawSelectedTracker(QPainter *pPainter)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	if (m_bSelected)
	{// 选择状态
		Q_ASSERT(m_pGraphicsLayer);
		if (m_pGraphicsLayer && m_pGraphicsLayer->IsCurrentLayer())
		{
			auto nRet = m_pGraphicsLayer->IsSingleSelection();
			if (nRet == CGraphicsLayer::SINGLE_SELECTED)
			{// 单选
				DrawSingleSelectedTracker(pPainter);
			}
			else if (nRet == CGraphicsLayer::MULTI_SELECTED)
			{// 多选
				DrawMultiSelectedTracker(pPainter, m_pGraphicsLayer->IsFirstSelectedWidget(this));
			}
		}
	}
}

void CBaseWidget::DrawPath(QPainter *pPainter, QPainterPath &p, CPenInfo &info)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	QPen mypen;
	info.ToQPen(mypen);

	pPainter->save();
	pPainter->setPen(mypen);
	pPainter->drawPath(p);
	pPainter->restore();
}

void CBaseWidget::FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info)
{
		Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	auto pBrush = info.CreateQBrush();
	
	pPainter->save();

	if (info.GetGradiendMode() != 0)
	{
		//渐变
		QLinearGradient myGradient;
		if (info.GetGradiendMode() == CBrushInfo::LEFTTORIGHT)
		{
			myGradient.setStart(GetPosition().topLeft());
			myGradient.setFinalStop(GetPosition().topRight());
		}
		else if (info.GetGradiendMode() == CBrushInfo::RIGHTTOLEFT)
		{
			myGradient.setStart(GetPosition().topRight());
			myGradient.setFinalStop(GetPosition().topLeft());

		}
		else if (info.GetGradiendMode() == CBrushInfo::TOPTOBOTTOM)
		{
			myGradient.setStart(GetPosition().topLeft());
			myGradient.setFinalStop(GetPosition().bottomLeft());

		}
		else if (info.GetGradiendMode() == CBrushInfo::BOTTOMTOTOP)
		{
			myGradient.setStart(GetPosition().bottomLeft());
			myGradient.setFinalStop(GetPosition().topLeft());

		}

		myGradient.setColorAt(0.0, info.GetColor());
		myGradient.setColorAt(0.9, info.GetEndColor());

		//pPainter->setBrush(myGradient);
		pPainter->fillPath(p, QBrush(myGradient)); 
	}
	else
	{
		pPainter->fillPath(p, *pBrush);
	}
	

	pPainter->restore();
}


/*! \fn void CBaseWidget::PrepareDynamic()
********************************************************************************************************* 
** \brief CBaseWidget::PrepareDynamic 
** \details  实时运行时，准备开始动态连接属性的展示
** \return void 
** \author LiJin 
** \date 2017年2月10日 
** \note 
********************************************************************************************************/
void CBaseWidget::PrepareDynamic()
{
	auto iter = m_arrDyncs.begin();
//	m_rcInitPos = m_rcPos;
	for (; iter != m_arrDyncs.end(); ++iter)
	{
		(*iter)->PrepareWidgetDync(this);
	}
}
/*! \fn bool CBaseWidget::ProcessDynamic()
********************************************************************************************************* 
** \brief CBaseWidget::ProcessDynamic 
** \details 实时运行时，处理动态连接属性的展示
** \return bool 
** \author LiJin 
** \date 2017年2月10日 
** \note 
********************************************************************************************************/
bool CBaseWidget::ProcessDynamic()
{
	bool bRet = false;
	QRectF oraPos=m_rcPos;
	std::vector <CBaseDyncData*>::iterator iter = m_arrDyncs.begin();
	for (; iter != m_arrDyncs.end(); ++iter)
	{
		bRet |= (*iter)->ProcessWidgetDync(this);
	}
	if (bRet)
	{	
		emit UpdateItemRect(this,m_rcPos,oraPos);
	}
	return bRet;
}
/*! \fn void CBaseWidget::ExitDynamic()
********************************************************************************************************* 
** \brief CBaseWidget::ExitDynamic 
** \details  实时运行时，系统退出前清扫动态连接属性相关
** \return void 
** \author LiJin 
** \date 2017年2月10日 
** \note 
********************************************************************************************************/
void CBaseWidget::ExitDynamic()
{

}

/** @}*/
