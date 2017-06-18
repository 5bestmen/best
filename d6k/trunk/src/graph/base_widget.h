/*! @file base_widget.h
<PRE>
********************************************************************************
模块名       :
文件名       :  base_widget.h
文件实现功能 :  基础图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
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
*  \brief 画笔信息.
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
	QColor m_clrPen;                 //! 颜色 包含透明度
	unsigned int m_nWidth;           //! 线宽度
	unsigned int m_nDashStyle;       //! 线类型
	unsigned int m_nDashCap;         //! 线帽形状
	unsigned int m_StartTransparency;//! 初始化透明度
};

//背景图信息
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
	//背景图片
	QString m_strBackgroundImage;
	//背景图片大小
	QSize m_szImage;
	//背景位置
	QPoint m_pfImage;
	//平铺
	bool m_bTiling;
	//拉伸
	bool m_bStretch;

};

class CBrushInfo
{
public:
	CBrushInfo();
	~CBrushInfo();
	enum LINEGRADIENT
	{
		NOGRADIENT = 0,    //非渐变
		LEFTTORIGHT = 1,   //从左到右渐变
		RIGHTTOLEFT,       //右到左渐变
		TOPTOBOTTOM,       //上到下
		BOTTOMTOTOP,       //下到上
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
	QColor m_clrStart;                 //! 渐变颜色 包含透明度，也适用单色填充
	QColor m_clrEnd;                   //! 渐变颜色 包含透明度

	unsigned int m_nBrushStyle;         //! 填充类型
	unsigned int m_nBrushType;          //! 填充类型
	unsigned int m_nHatchSytle;
	unsigned int m_nGradientMode;
	unsigned int m_nBlendTriangularShape;
	unsigned int m_StartTransparency;//! 初始化透明度
};

class CFontInfo
{
public:
	enum LAYOUT
	{
		FONT_CENTER =0,  //中心
		FONT_TOP,        //上
		FONT_BOTTOM,    //下
		FONT_LEFT,      //左 
		FONT_RIGHT,     //又
		MID_LEFT,       //中  左
		MID_RIGHT       //中  右
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
	unsigned int m_nDirection;       //! 纵向、横向
	unsigned int m_StartTransparency;//! 初始化透明度
	unsigned int m_nLayout;
};

//#define  WIDGET_ITEM_PAD  1
enum 
{
	WIDGET_ITEM_PAD = 1,             //! 图元的外包围
	WIDGET_HANDLE_RECT_HALF_LEN = 3, //! 图元选择时，外框上的小矩形的边长一半
	WIDGET_MULTI_SELECTED_MARGIN = 5, //! 多个图元选择时，外框外扩的长度
};

enum POS_BOX
{
	POS_TOP_LEFT = 1,     //!<  左上
	POS_TOP_CENTER,       //!<  中上
	POS_TOP_RIGHT,        //!<  右上
	POS_RIGHT_CENTER,     //!<  右中
	POS_BOTTOM_RIGHT,     //!<  右下
	POS_BOTTOM_CENTER,    //!<  中下
	POS_BOTTOM_LEFT,      //!<  做下
	POS_LEFT_CENTER       //!<  左中
};

/*! \class CBaseWidget  base_widget.h  "base_widget.h"
*  \brief 图元基类信息.
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
		WIDGET_LINK_LINE,  //!< 智能连线
		WIDGET_LINE,       //!< 直线
		WIDGET_POLYLINE,   //!< 折线
		WIDGET_RECT,
		WIDGET_POLYGON,   //!< 多边形
		WIDGET_BUTTON,
		WIDGET_TEXT,
		WIDGET_PROGRESSBAR, //!< 进度条
		WIDGET_METER,       //!< 仪表
		WIDGET_RTCURVE,     //!< 实时曲线
		WIDGET_GRID,        //!< 网格
		WIDGET_DATETIME,    //!< 日期时间控件
		WIDGET_CLOCK,       //!< 时间控件

		WIDGET_GROUP,       //!< 成组
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
		HORZ_FLIP = 0, //!< 水平翻转
		VERT_FLIP      //!< 垂直翻转
	};

	//旋转中心点
	enum ROTATE_CENTER
	{
		//中心
		ROTATE_MIDCENTER,
		//左上角
		ROTATE_TOPLEFT,
		//中上
		ROTATE_TOPMID,
		//右上角
		ROTATE_TOPRIGHT,
		// 左中
		ROTATE_LEFTMID,
		//右中
		ROTATE_RIGHTMID,
		//左下角
		ROTATE_LEFTBOTTOM,
		//下中
		ROTATE_BOTTOMMID,
		//右下
		ROTATE_RIGHTBOTTOM,
	};

public:
	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	//
	virtual unsigned int GetHandleCount() const;
	// 选中状态时，画外部的选择框
	virtual void DrawSingleSelectedTracker(QPainter *painter);
	// 多个选中时
	virtual void DrawMultiSelectedTracker(QPainter *pPainter,bool bFirstSelected);
	// 获取外部选择框的位置
	virtual QPointF GetHandlePos(int nHandle) const;
	//
	virtual QPointF GetSpinHandlePos( ) const;

    //获取循转后的循转标识位置
    virtual QPointF GetSpinMovingHandlePos() const;

	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);
	// 旋转的中心点 
	virtual QPointF GetRotationCenterPoint() const;
	//移动旋转中心
	virtual QPointF GetMovingRotationCenterPoint() const;


	// 移动中的图元
	virtual void DrawMovingWidget(QPainter *pPaint,bool bSingleSelected = false);
	// 移动到新的位置
	virtual void MoveDelta(const QPointF &delta);
	virtual void MovingDelta(const QPointF &delta);
    //循转到新位置
    virtual void RotatingDelta(const QPointF &delta);

	// 移动到新的位置
	virtual void MoveTo(const QRectF & position);
	virtual void MoveTo(const QPointF & newPos);
	// 判断是否被指定的矩形包含
	virtual bool IsContained(const QRectF &rcBox) const;
	// 判断是否在点击的爪子中
	virtual int HitTestHandle(const QPointF & point) const;
	// 判断是否点击了该图元
	virtual bool HitTest(const QPointF &ptInput) const;

	virtual bool SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);
	//保存静态数据
	virtual bool SaveBaseXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);
	//保存静态数据
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

	//旋转中心角
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
	// 以下是实时运行部分调用
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
	//根据动态属性类型  获取动态对象
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

	// 生成 图元名称
	void BuildWidgetName(unsigned int nID, unsigned int nLayerID, const std::string &szGraphName, const std::string &szGraphFolderName);

	// 通常是运行是判断是否可见
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
	//复制动态属性
	void CopyDynamicPro(const std::vector<CBaseDyncData *> &arrDyncs);

	//初始化动态属性
	//void InitDyDataModel();
	//创建动态属性
protected:
	// 画选择状态
	void DrawSelectedTracker(QPainter *pPainter);

	void DrawSpinItem(QPainter *pPainter);
	// 获取外部选择框的原始位置
	QRectF GetRawHandleRect(int nHandle) const;
	// 获取外部选择框的旋转后的位置
	QPainterPath GetHandlePath(int nHandle)const;

	void InitDefault();
public:
	//! 编辑时，是否选中状态
	bool   m_bSelected;

	//  运行时
	//! 是否正在闪烁
	bool    m_bIsBlinking;
	//! 用于快速，慢速闪烁
	unsigned int m_nBlinkingCnt;
protected:
	// 图元基本属性
	//! 具体类型标识
	unsigned int m_nObjType;
	//! 属于哪一类的： 基本图形/WIN控件/高级图形
	unsigned int m_nClassType;
	//!图元名称 主要用于助记
	std::string m_szName;
	//! 图层名称 
	std::string m_szLayerName;
	//! 文件名
	std::string m_szGraphName;
	//! 目录名称
	std::string  m_szFolderName;
	//! ID 用于标识 图形中每个图形元素的唯一标识
	unsigned int m_nID;
	//! 图元的助记符
	std::string m_szWidgetName;

	//! 位置
	QRectF m_rcPos;
	//记录初始位置
	QRectF m_rcSavePos;
	//! 移动中的图元位置
	QRectF m_rcMovingPos;
	//! 外包围
	QRectF m_rcBounding;

	//! 锁定位置
	bool  m_bLocked;
	//! 旋转
	bool  m_bRotate;
	//!  旋转角度
	double m_dblRotateAngle;
	//! tab顺序号
	unsigned int m_nTabOrder;
	//! 运行时是否可见 0 可见 1 不可见
	unsigned int m_nDisplayInRun;

	//! 画笔 对有些图元，可能用不到
	//QPen  m_Pen;
	std::shared_ptr<QPen> m_pPen;
	//! 画刷
	//QBrush m_Brush;
	std::shared_ptr<QBrush> m_pBrush;
	//! 字体
	//QFont m_Font;
	std::shared_ptr<QFont> m_pFont;

	//! 显示的内容
	QString m_szTxt;

	//! 图层
	unsigned int m_nLayerIndex;
	//! 所属图层
	CGraphicsLayer *m_pGraphicsLayer;
	 
	//! 画笔信息
	CPenInfo m_PenInfo;
	//! 画刷信息
	CBrushInfo m_BrushInfo;
	//! 字体信息
	CFontInfo m_FontInfo;
	//图元背景
	CImageInfo m_ImageInfo;
	//动态画刷
	//CDynamicBrushInfo m_DynamicBrushInfo;
	
	//! 旋转用的小旋钮
	std::shared_ptr<CSpinItem> m_pSpinItem;
	//! 如果图元在成组状态时 组对象。不用保存到文件中
	CGroupWidget *m_pGroup;

	//! 实时运行时是否可见？
	bool	m_bVisible;

	//弧线起始弧度和结束弧度
	double m_nStartAngle;
	double m_nEndAngle;

	//QPixmap m_PixMap;
	//旋转的中心角
	ROTATE_CENTER m_eRotateCenter;
	//点击状态
	bool m_bPressFlag;

private:
	//! 动态连接
	std::vector<CBaseDyncData *> m_arrDyncs;
};


#endif // D_WIDGET_H

/** @}*/
