/*! @file base_widget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : base_widget.h
�ļ�ʵ�ֹ��� : ����ͼԪ
����         :  LiJin
�汾         :  V1.00
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
#ifndef D_WIDGET_H
#define D_WIDGET_H

#include "graph_global.h"

//#include <string>
#include <memory>

#include <QGraphicsItem>

// #include <qpen>
// #include <QBrush>
// #include <QFont>
 
class QGraphicsItem;
class QXmlStreamWriter;
class QXmlStreamReader;

class CGraphicsLayer;
class CGroupWidget;
/*! \class CPenInfo  base_widget.h  "base_widget.h"
*  \brief ������Ϣ.
*/
class CPenInfo
{
public:
	 CPenInfo();
	 ~CPenInfo();
public:
	unsigned int GetWidth() const;
private:
	QColor m_clrPen;
	unsigned int m_nTransparency;
	unsigned int m_nWidth;
	unsigned int m_nDashStyle;
	unsigned int m_nDashCap;
};

class CBrushInfo
{
public:
	CBrushInfo();
	~CBrushInfo();
private:

};

class CFontInfo
{
public:
	CFontInfo();
	~CFontInfo();
private:
	QColor m_clrFont;
	unsigned int m_nTransparency;
	unsigned int m_nDirection; //! ���򡢺���

};

//#define  WIDGET_ITEM_PAD  1
enum 
{
	WIDGET_ITEM_PAD = 1,             //! ͼԪ�����Χ
	WIDGET_HANDLE_RECT_HALF_LEN = 3, //! ͼԪѡ��ʱ������ϵ�С���εı߳�һ��
	WIDGET_MULTI_SELECTED_MARGIN = 5, //! ���ͼԪѡ��ʱ����������ĳ���
};

enum POS_BOX
{
	POS_TOP_LEFT = 1,
	POS_TOP_CENTER,
	POS_TOP_RIGHT,
	POS_RIGHT_CENTER,
	POS_BOTTOM_RIGHT,
	POS_BOTTOM_CENTER,
	POS_BOTTOM_LEFT,
	POS_LEFT_CENTER
};

class CBaseWidget : public QGraphicsItem
{
public:
	CBaseWidget();
	CBaseWidget(QRectF pos);

	virtual ~CBaseWidget();
public:
	enum WIDGET_TYPE
	{
		WIDGET_NULL = 0,
		WIDGET_LINK_LINE,  //!< ��������
		WIDGET_LINE,       //!< ֱ��
		WIDGET_POLYLINE,   //!< ����
		WIDGET_RECT,
		WIDGET_POLYGON,   //!< �����
		WIDGET_BUTTON,
		WIDGET_TEXT,
		WIDGET_PROGRESSBAR, //!< ������
		WIDGET_METER,       //!< �Ǳ�
		WIDGET_RTCURVE,     //!< ʵʱ����
		WIDGET_GRID,        //!< ����
		WIDGET_DATETIME,    //!< ����ʱ��ؼ�
		WIDGET_CLOCK,       //!< ʱ��ؼ�

		WIDGET_GROUP        //!< ����
	};

	enum ALIGN
	{
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_CENTER
	};

	enum THREE_DIMENSION_STYLE
	{
		FLAT_STYLE = 0,
		SUNKEN_STYLE,
		RAISED_STYLE
	};

	enum FLIP_TYPE
	{
		HORZ_FLIP = 0, //!< ˮƽ��ת
		VERT_FLIP      //!< ��ֱ��ת
	};

public:
	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	//
	virtual unsigned int GetHandleCount() const;
	// ѡ��״̬ʱ�����ⲿ��ѡ���
	virtual void DrawSingleSelectedTracker(QPainter *painter);
	// ���ѡ��ʱ
	virtual void DrawMultiSelectedTracker(QPainter *pPainter,bool bFirstSelected);
	// ��ȡ�ⲿѡ����λ��
	virtual QPointF GetHandlePos(int nHandle) const;
	//
	virtual QPointF GetSpinHandlePos( ) const;
	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);


	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPaint,bool bSingleSelected = false);
	// �ƶ����µ�λ��
	virtual void MoveDelta(const QPointF &delta);
	virtual void MovingDelta(const QPointF &delta);

	// �ƶ����µ�λ��
	virtual void MoveTo(const QRectF & position);
	virtual void MoveTo(const QPointF & newPos);
	// �ж��Ƿ�ָ���ľ��ΰ���
	virtual bool IsContained(const QRectF &rcBox) const;
	// �ж��Ƿ��ڵ����צ����
	virtual int HitTestHandle(const QPointF & point) const;

	virtual bool SaveXml(QXmlStreamWriter *pXmlWriter) const;

	virtual bool LoadXml(QXmlStreamReader *pXmlReader);

public:
	bool IsLocked()
	{
		return m_bLocked;
	}
	bool IsRotate()
	{
		return m_bRotate;
	}

	bool IsSelected()
	{
		return m_bSelected;
	}

	QRectF GetPosition() const;
	QRectF GetMovingPosition() const;

	void  NormalizePosition();

	unsigned int GetLayerIndex()
	{
		return m_nLayerIndex;
	}
	unsigned int GetWidgetType() const
	{
		return m_nObjType;
	}
	std::string  GetWidgetName()const
	{
		return m_szWidgetName;
	}
	void SetLayer(CGraphicsLayer *pLayer);

	void SetGroup(CGroupWidget *pGroup);

	CGroupWidget *GetGroup();

	// ���� ͼԪ����
	void BuildWidgetName(unsigned int nID, unsigned int nLayerID, const std::string &szGraphName, const std::string &szGraphFolderName);

protected:
	// ��ȡ�ⲿѡ���ĳߴ�
	QRectF GetHandleRect(int nHandle) const;

	void InitDefault();
public:
	//! �༭ʱ���Ƿ�ѡ��״̬
	bool   m_bSelected;


protected:
	// ͼԪ��������
	//! ����
	unsigned int m_nObjType;
	//! ������һ��ģ� ����ͼ��/WIN�ؼ�/�߼�ͼ��
	unsigned int m_nClassType;
	//!ͼԪ���� ��Ҫ��������
	std::string m_szName;
	//! ͼ������ 
	std::string m_szLayerName;
	//! �ļ���
	std::string m_szGraphName;
	//! Ŀ¼����
	std::string  m_szFolderName;
	//! ID ���ڱ�ʶ ͼ����ÿ��ͼ��Ԫ�ص�Ψһ��ʶ
	unsigned int m_nID;
	//! ͼԪ�����Ƿ�
	std::string m_szWidgetName;

	//! λ��
	QRectF m_rcPos;
	//! �ƶ��е�ͼԪλ��
	QRectF m_rcMovingPos;
	//! ���Χ
	QRectF m_rcBounding;

	//! ����λ��
	bool  m_bLocked;
	//! ��ת
	bool	m_bRotate;
	//! tab˳���
	unsigned int m_nTabOrder;
	//! ����ʱ�Ƿ�ɼ� 0 �ɼ� 1 ���ɼ�
	unsigned int m_nDisplayInRun;

	//! ���� ����ЩͼԪ�������ò���
	//QPen  m_Pen;
	std::shared_ptr<QPen> m_pPen;
	//! ��ˢ
	//QBrush m_Brush;
	std::shared_ptr<QBrush> m_pBrush;
	//! ����
	//QFont m_Font;
	std::shared_ptr<QFont> m_pFont;

	//! ��ʾ������
	QString m_szTxt;

	//! ͼ��
	unsigned int m_nLayerIndex;
	//! ����ͼ��
	CGraphicsLayer *m_pGraphicsLayer;
	 
	CPenInfo m_PenInfo;
	CBrushInfo m_BrushInfo;
	CFontInfo m_FontInfo;
	

	//! ���ͼԪ�ڳ���״̬ʱ ����󡣲��ñ��浽�ļ���
	CGroupWidget *m_pGroup;
private:

};


#endif // D_WIDGET_H

/** @}*/
