/*! @file line_widget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  line_widget.h
�ļ�ʵ�ֹ��� :  ͼ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ��
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
        BEE_LINE,    //ֱ��
        POLY_LINE,    //����
		POLYGONO,     //�����
		PIPE,        //�ܵ� 
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
	//�������в����
	void InsertNewPoint(const QPointF &pfClick);
	//������Ե�
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

	//�ܵ���ɫ���
	void FillPiPePath(QPainter *pPainter,CBrushInfo &info);

	void SetFontLayout(QTextOption &txtOpt);

	virtual bool SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);

	//���ƹܵ�
	void DrawPipe(QPainter *pPainter);
	//������ת�Ƕ�
	double CalculateRoutateAngel(QPointF pfStart, QPointF pfEnd);

protected:
 
private:
	//! �ߵ����
	QPointF   m_ptStart;
	//! �ߵ��յ�
	QPointF   m_ptEnd;
	//!  ��Ծ���λ�õı���: ͨ������0����1
	QPointF   m_ptStartScale;
	//!  ��Ծ���λ�õı���: ͨ������0����1
	QPointF   m_ptEndScale;

	QPointF  	m_CenterPoint;
	//�ߵĵ㼯��   ���Ե� 
	QVector<QPointF> m_arrPonits;
	//�ߵļ���  ��Ե�
	QVector<QPointF> m_arrRelativePoints;
    	
	
	unsigned char		m_StartArrowSize;
	unsigned char		m_EndArrowSize;
	unsigned char		m_StartArrow;
	unsigned char		m_EndArrow;
};


#endif // D_LINE_WIDGET_H


/** @}*/
