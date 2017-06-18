/*! @file line_widget.h
<PRE>
********************************************************************************
模块名       :
文件名       :  line_widget.h
文件实现功能 :  图层
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图层
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/


#ifndef D_LINE_WIDGET_H
#define D_LINE_WIDGET_H
 
#include<QTextOption>
#include "base_widget.h"
  
class CLineWidget : public CBaseWidget
{
	Q_OBJECT
public:
    enum LINE
    {
        BEE_LINE,    //直线
        POLY_LINE,    //折线
		POLYGONO,     //多边形
		PIPE,        //管道 
    };

	CLineWidget();
    CLineWidget(QRectF rcPos, CLineWidget::LINE shape);

	~CLineWidget();
public:
	enum ARROW_TYPE
	{
		NO_ARROW,
		PURE_ARROW,
		OPEN_ARROW,
		STEALTH_ARROW,
		DIAMOND_ARROW,
		ROUND_ARROW,
		SQUARE_ARROW
	};


public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

    virtual void CLineWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected);
     
    virtual void GetPath(QPainterPath& path);

    void GetMovingPath(QPainterPath& path);
	//往集合中插入点
	void InsertNewPoint(const QPointF &pfClick);
	//更新相对点
	void UpdateAllPoints();
	//
	const QVector<QPointF> &GetAllPoints()
	{
		return m_arrPonits;
	}
	//
	const QVector<QPointF> &GetRelativePoints()
	{
		return m_arrRelativePoints;
	}
	//
	virtual void FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info);

	//管道颜色填充
	void FillPiPePath(QPainter *pPainter,CBrushInfo &info);

	void SetFontLayout(QTextOption &txtOpt);

	virtual bool SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);

	//绘制管道
	void DrawPipe(QPainter *pPainter);
	//计算旋转角度
	double CalculateRoutateAngel(QPointF pfStart, QPointF pfEnd);

protected:
 
private:
	//! 线的起点
	QPointF   m_ptStart;
	//! 线的终点
	QPointF   m_ptEnd;
	//!  相对矩形位置的比例: 通常不是0就是1
	QPointF   m_ptStartScale;
	//!  相对矩形位置的比例: 通常不是0就是1
	QPointF   m_ptEndScale;

	QPointF  	m_CenterPoint;
	//线的点集合   绝对点 
	QVector<QPointF> m_arrPonits;
	//线的集合  相对点
	QVector<QPointF> m_arrRelativePoints;
    	
	
	unsigned char		m_StartArrowSize;
	unsigned char		m_EndArrowSize;
	unsigned char		m_StartArrow;
	unsigned char		m_EndArrow;
};


#endif // D_LINE_WIDGET_H


/** @}*/
