/*! @file base_widget.h
<PRE>
********************************************************************************
模块名       :
文件名       : base_widget.h
文件实现功能 : 基础图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  基础图元
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
*  \brief 画笔信息.
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
	unsigned int m_nDirection; //! 纵向、横向

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

		WIDGET_GROUP        //!< 成组
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
	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);


	// 移动中的图元
	virtual void DrawMovingWidget(QPainter *pPaint,bool bSingleSelected = false);
	// 移动到新的位置
	virtual void MoveDelta(const QPointF &delta);
	virtual void MovingDelta(const QPointF &delta);

	// 移动到新的位置
	virtual void MoveTo(const QRectF & position);
	virtual void MoveTo(const QPointF & newPos);
	// 判断是否被指定的矩形包含
	virtual bool IsContained(const QRectF &rcBox) const;
	// 判断是否在点击的爪子中
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

	// 生成 图元名称
	void BuildWidgetName(unsigned int nID, unsigned int nLayerID, const std::string &szGraphName, const std::string &szGraphFolderName);

protected:
	// 获取外部选择框的尺寸
	QRectF GetHandleRect(int nHandle) const;

	void InitDefault();
public:
	//! 编辑时，是否选中状态
	bool   m_bSelected;


protected:
	// 图元基本属性
	//! 类型
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
	//! 移动中的图元位置
	QRectF m_rcMovingPos;
	//! 外包围
	QRectF m_rcBounding;

	//! 锁定位置
	bool  m_bLocked;
	//! 旋转
	bool	m_bRotate;
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
	 
	CPenInfo m_PenInfo;
	CBrushInfo m_BrushInfo;
	CFontInfo m_FontInfo;
	

	//! 如果图元在成组状态时 组对象。不用保存到文件中
	CGroupWidget *m_pGroup;
private:

};


#endif // D_WIDGET_H

/** @}*/
