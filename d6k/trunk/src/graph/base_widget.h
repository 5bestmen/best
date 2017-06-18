/*! @file base_widget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_widget.h
�ļ�ʵ�ֹ��� :  ����ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����ͼԪ
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef D_WIDGET_H
#define D_WIDGET_H

#include "graph_global.h"

//#include <string>
#include <memory>
#include <QFont>

#include <QGraphicsItem>

// #include <qpen>
// #include <QBrush>
// #include <QFont>
 
class QGraphicsItem;
class QXmlStreamWriter;
class QXmlStreamReader;

class CGraphicsLayer;
class CGroupWidget;
class CSpinItem;
#include <QGraphicsScene>
class CBaseDyncData;
class QObject;
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
	void SetWidth(unsigned int nWidth)
	{
		m_nWidth = nWidth;
	}
	QColor & GetColor() 
	{
		return m_clrPen;
	}
	void SetColor(const QColor & clrValue)
	{
		m_clrPen = clrValue;
	}
	unsigned int GetStyle() const
	{
		return m_nDashStyle;
	}
	void SetStyle(unsigned int nStyle)
	{
		m_nDashStyle = nStyle;
	}
	unsigned int GetDashStyle()const
	{
		return m_nDashStyle;
	}

	void  SetTranspancy(int StartTransparency)
	{
		m_StartTransparency = StartTransparency;
	}

	void ToQPen(QPen & pen)const;

private:
	QColor m_clrPen;                 //! ��ɫ ����͸����
	unsigned int m_nWidth;           //! �߿��
	unsigned int m_nDashStyle;       //! ������
	unsigned int m_nDashCap;         //! ��ñ��״
	unsigned int m_StartTransparency;//! ��ʼ��͸����
};

//����ͼ��Ϣ
class CImageInfo
{
public:
	CImageInfo()
	{
		m_pfImage = QPoint(0, 0);
		m_bTiling = false;
		m_bStretch = false;
		m_szImage = QSize(0, 0);
	}

	~CImageInfo() {}
public:
	const QString &GetBackgroundImage()
	{
		return m_strBackgroundImage;
	}

	void SetBackgroundImage(const QString &strImage)
	{
		m_strBackgroundImage = strImage;
	}

	const QSize &GetImageSize()
	{
		return m_szImage;
	}

	void SetImageSize(const QSize &szImage)
	{
		m_szImage = szImage;
	}

	const QPoint &GetImagePosition()
	{
		return m_pfImage;
	}

	void SetImagePosition(const QPoint &pfImage)
	{
		m_pfImage = pfImage;
	}

	bool GetbTiling()
	{
		return m_bTiling;
	}

	void SetbTiling(bool tFlag)
	{
		m_bTiling = tFlag;
	}


	void SetStretch(bool sFlag)
	{
		m_bStretch = sFlag;
	}

	bool GetStretch()
	{
		return m_bStretch;
	}


private:
	//����ͼƬ
	QString m_strBackgroundImage;
	//����ͼƬ��С
	QSize m_szImage;
	//����λ��
	QPoint m_pfImage;
	//ƽ��
	bool m_bTiling;
	//����
	bool m_bStretch;

};

class CBrushInfo
{
public:
	CBrushInfo();
	~CBrushInfo();
	enum LINEGRADIENT
	{
		NOGRADIENT = 0,    //�ǽ���
		LEFTTORIGHT = 1,   //�����ҽ���
		RIGHTTOLEFT,       //�ҵ��󽥱�
		TOPTOBOTTOM,       //�ϵ���
		BOTTOMTOTOP,       //�µ���
	};
public:
	unsigned int GetType() const
	{
		return m_nBrushType;
	}
	unsigned int GetStyle() const
	{
		return m_nBrushStyle;
	}
		
	void SetStyle(unsigned int nStyle)
	{
		m_nBrushStyle = nStyle;
	}

	QColor & GetColor()
	{
		return m_clrStart;
	}

	void SetColor(const QColor & clr)
	{
		m_clrStart = clr;
	}


	void SetEndColor(const QColor &clr)
	{
		m_clrEnd = clr;
	}

	QColor &GetEndColor()
	{
		return m_clrEnd;
	}

	void SetGradiendMode(int nMode)
	{
		m_nGradientMode = nMode;
	}

	int GetGradiendMode() const
	{
		return m_nGradientMode;
	}
	void  SetTranspancy(int StartTransparency)
	{
		m_StartTransparency = StartTransparency;
	}
	
	std::unique_ptr<QBrush> CreateQBrush();
private:
	QColor m_clrStart;                 //! ������ɫ ����͸���ȣ�Ҳ���õ�ɫ���
	QColor m_clrEnd;                   //! ������ɫ ����͸����

	unsigned int m_nBrushStyle;         //! �������
	unsigned int m_nBrushType;          //! �������
	unsigned int m_nHatchSytle;
	unsigned int m_nGradientMode;
	unsigned int m_nBlendTriangularShape;
	unsigned int m_StartTransparency;//! ��ʼ��͸����
};

class CFontInfo
{
public:
	enum LAYOUT
	{
		FONT_CENTER =0,  //����
		FONT_TOP,        //��
		FONT_BOTTOM,    //��
		FONT_LEFT,      //�� 
		FONT_RIGHT,     //��
		MID_LEFT,       //��  ��
		MID_RIGHT       //��  ��
	};
	CFontInfo();
	~CFontInfo();
public:
	//
	void SetColor(const QColor &clr)
	{
		m_clrFont = clr;
	}

	QColor & GetColor()
	{
		return m_clrFont;
	}

	void SetFont(const QFont &font)
	{
		m_Font = font;
	}

	const QFont & GetFont()
	{
		return m_Font;
	}

	void SetFontLayout(int nLayout)
	{
		m_nLayout = nLayout;
	}

	int GetFontLayout() const 
	{
		return m_nLayout;
	}

	void  SetTranspancy(int StartTransparency)
	{
		m_StartTransparency = StartTransparency;
	}
private:
	QColor m_clrFont;
	QFont  m_Font;
	unsigned int m_nDirection;       //! ���򡢺���
	unsigned int m_StartTransparency;//! ��ʼ��͸����
	unsigned int m_nLayout;
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
	POS_TOP_LEFT = 1,     //!<  ����
	POS_TOP_CENTER,       //!<  ����
	POS_TOP_RIGHT,        //!<  ����
	POS_RIGHT_CENTER,     //!<  ����
	POS_BOTTOM_RIGHT,     //!<  ����
	POS_BOTTOM_CENTER,    //!<  ����
	POS_BOTTOM_LEFT,      //!<  ����
	POS_LEFT_CENTER       //!<  ����
};

/*! \class CBaseWidget  base_widget.h  "base_widget.h"
*  \brief ͼԪ������Ϣ.
*/

class QPaintEvent;
class CBaseWidget : public QObject,public QGraphicsItem
{
	Q_OBJECT 
	Q_INTERFACES(QGraphicsItem);
public:

	CBaseWidget();
	CBaseWidget(QRectF pos);

	virtual ~CBaseWidget();

	void paintEvent(QPaintEvent* event);

	const CBaseWidget & operator=(const CBaseWidget &src);
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

		WIDGET_GROUP,       //!< ����
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

	//��ת���ĵ�
	enum ROTATE_CENTER
	{
		//����
		ROTATE_MIDCENTER,
		//���Ͻ�
		ROTATE_TOPLEFT,
		//����
		ROTATE_TOPMID,
		//���Ͻ�
		ROTATE_TOPRIGHT,
		// ����
		ROTATE_LEFTMID,
		//����
		ROTATE_RIGHTMID,
		//���½�
		ROTATE_LEFTBOTTOM,
		//����
		ROTATE_BOTTOMMID,
		//����
		ROTATE_RIGHTBOTTOM,
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

    //��ȡѭת���ѭת��ʶλ��
    virtual QPointF GetSpinMovingHandlePos() const;

	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);
	// ��ת�����ĵ� 
	virtual QPointF GetRotationCenterPoint() const;
	//�ƶ���ת����
	virtual QPointF GetMovingRotationCenterPoint() const;


	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPaint,bool bSingleSelected = false);
	// �ƶ����µ�λ��
	virtual void MoveDelta(const QPointF &delta);
	virtual void MovingDelta(const QPointF &delta);
    //ѭת����λ��
    virtual void RotatingDelta(const QPointF &delta);

	// �ƶ����µ�λ��
	virtual void MoveTo(const QRectF & position);
	virtual void MoveTo(const QPointF & newPos);
	// �ж��Ƿ�ָ���ľ��ΰ���
	virtual bool IsContained(const QRectF &rcBox) const;
	// �ж��Ƿ��ڵ����צ����
	virtual int HitTestHandle(const QPointF & point) const;
	// �ж��Ƿ����˸�ͼԪ
	virtual bool HitTest(const QPointF &ptInput) const;

	virtual bool SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);
	//���澲̬����
	virtual bool SaveBaseXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);
	//���澲̬����
	virtual bool SaveDynamicXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);

	virtual bool LoadXml(QXmlStreamReader *pXmlReader);

	virtual QRectF GetPosition() const;
	virtual QRectF GetMovingPosition() const;

	QRectF GetOldPosition() const
	{
		return m_rcSavePos;
	}

	virtual QRectF GetClipRect() const;
	virtual QPainterPath shape() const;
	virtual QRectF boundingRect() const;

	virtual void DrawPath(QPainter *pPainter, QPainterPath &p, CPenInfo &info);
	virtual void FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info);

	virtual int GetVersion() const;

	//��ת���Ľ�
	int GetRotateCenter() const
	{
		return m_eRotateCenter;
	}

	void SetRotateCenter(int nCenter)
	{
		m_eRotateCenter = (ROTATE_CENTER)nCenter;
	}
	//

public:
	//////////////////////////////////////////////////////////////////////////
	// ������ʵʱ���в��ֵ���
	virtual void PrepareDynamic();

	virtual void ExitDynamic();

	virtual bool ProcessDynamic();

Q_SIGNALS:
	void UpdateItemRect(CBaseWidget*,const QRectF& rect, const QRectF& rect1);
public:
	bool IsLocked() const
	{
		return m_bLocked;
	}

	void SetLocked(bool bLock)
	{
		m_bLocked = bLock;
	}

	bool IsRotate() const
	{
		return m_bRotate;
	}

    void SetRotate(bool bRotate)
    {
        m_bRotate = bRotate;
    }

	bool IsSelected() const 
	{
		return m_bSelected;
	}

	QRectF RotateRect(const QRectF & rcInput, double dblAngle);
	
	void  NormalizePosition();

	unsigned int GetLayerIndex() const
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
	unsigned int GetID() const
	{
		return m_nID;
	}

	void SetID(unsigned int nID)
	{
		m_nID = nID;
	}
	double GetRotateAngle()const
	{
		return m_dblRotateAngle;
	}
	void SetRotateAngle(double dValue)
	{
		m_dblRotateAngle = dValue;
	}
	CPenInfo* GetPenInfo()  
	{
		return &m_PenInfo;
	}
	CBrushInfo* GetBrushInfo()
	{
		return &m_BrushInfo;
	}

// 	CDynamicBrushInfo *GetDynamicBrushInfo()
// 	{
// 		return &m_DynamicBrushInfo;
// 	}
	//���ݶ�̬��������  ��ȡ��̬����
	CBaseDyncData * CreateDynamicPro(int nDyType);

	CFontInfo* GetFontInfo()
	{
		return &m_FontInfo;
	}
	//
	CImageInfo* GetImageInfo()
	{
		return &m_ImageInfo;
	}

	QString GetText()const
	{
		return m_szTxt; 
	}
	void SetText(const QString & szValue)
	{
		m_szTxt = szValue;
		//this->scene()->update(this->boundingRect());
	}

	void SetWidgetName(const std::string &szWidgetName)
	{
		m_szWidgetName = szWidgetName;
	}

	void SetName(const std::string &szName)
	{
		m_szName = szName;
	}

    void SetMovingPosition(const QRectF &pfMoving)
    {
        m_rcMovingPos = pfMoving;
    }
	void SetLayer(CGraphicsLayer *pLayer);

	void SetGroup(CGroupWidget *pGroup);

	CGroupWidget *GetGroup();

	// ���� ͼԪ����
	void BuildWidgetName(unsigned int nID, unsigned int nLayerID, const std::string &szGraphName, const std::string &szGraphFolderName);

	// ͨ�����������ж��Ƿ�ɼ�
	bool IsVisible() const
	{
		return m_bVisible;
	}

	void SetRcPos(const QRectF& rect)
	{
		m_rcPos = rect;
	}

	void SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
		setVisible(m_bVisible);
	}
	const std::vector<CBaseDyncData *> & CreateDynamicPro()
	{
		return m_arrDyncs;
	}

	double GetEndAngle() const
	{
		return m_nEndAngle;
	}

	void SetEndAngle(double dEnd)
	{
		m_nEndAngle = dEnd;
	}

	void SetPressFlag(bool bFlag)
	{
		m_bPressFlag = bFlag;
	}

	bool GetPressFlag() const
	{
		return m_bPressFlag;
	}

	/*QPixmap GetPixMap()
	{
		return m_PixMap;
	}
	void SetPixMap(QPixmap pixmap)
	{
		m_PixMap = pixmap;
	}*/
	//���ƶ�̬����
	void CopyDynamicPro(const std::vector<CBaseDyncData *> &arrDyncs);

	//��ʼ����̬����
	//void InitDyDataModel();
	//������̬����
protected:
	// ��ѡ��״̬
	void DrawSelectedTracker(QPainter *pPainter);

	void DrawSpinItem(QPainter *pPainter);
	// ��ȡ�ⲿѡ����ԭʼλ��
	QRectF GetRawHandleRect(int nHandle) const;
	// ��ȡ�ⲿѡ������ת���λ��
	QPainterPath GetHandlePath(int nHandle)const;

	void InitDefault();
public:
	//! �༭ʱ���Ƿ�ѡ��״̬
	bool   m_bSelected;

	//  ����ʱ
	//! �Ƿ�������˸
	bool    m_bIsBlinking;
	//! ���ڿ��٣�������˸
	unsigned int m_nBlinkingCnt;
protected:
	// ͼԪ��������
	//! �������ͱ�ʶ
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
	//��¼��ʼλ��
	QRectF m_rcSavePos;
	//! �ƶ��е�ͼԪλ��
	QRectF m_rcMovingPos;
	//! ���Χ
	QRectF m_rcBounding;

	//! ����λ��
	bool  m_bLocked;
	//! ��ת
	bool  m_bRotate;
	//!  ��ת�Ƕ�
	double m_dblRotateAngle;
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
	 
	//! ������Ϣ
	CPenInfo m_PenInfo;
	//! ��ˢ��Ϣ
	CBrushInfo m_BrushInfo;
	//! ������Ϣ
	CFontInfo m_FontInfo;
	//ͼԪ����
	CImageInfo m_ImageInfo;
	//��̬��ˢ
	//CDynamicBrushInfo m_DynamicBrushInfo;
	
	//! ��ת�õ�С��ť
	std::shared_ptr<CSpinItem> m_pSpinItem;
	//! ���ͼԪ�ڳ���״̬ʱ ����󡣲��ñ��浽�ļ���
	CGroupWidget *m_pGroup;

	//! ʵʱ����ʱ�Ƿ�ɼ���
	bool	m_bVisible;

	//������ʼ���Ⱥͽ�������
	double m_nStartAngle;
	double m_nEndAngle;

	//QPixmap m_PixMap;
	//��ת�����Ľ�
	ROTATE_CENTER m_eRotateCenter;
	//���״̬
	bool m_bPressFlag;

private:
	//! ��̬����
	std::vector<CBaseDyncData *> m_arrDyncs;
};


#endif // D_WIDGET_H

/** @}*/
