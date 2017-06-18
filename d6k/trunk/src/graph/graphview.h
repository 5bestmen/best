/*! @file graphview.h
<PRE>
********************************************************************************
模块名       :
文件名       :  graphview.h
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

#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <QGraphicsView>
#include <vector> 
#include <QMap>

#include "dync_color.h"

class CGraphFile;
class CBaseWidget;
class CPenInfo;
class CBrushInfo;
class CFontInfo;
class CGraphScene;

class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtTreePropertyBrowser;
class QtIntPropertyManager;
class QtDoublePropertyManager;
class QtStringPropertyManager;
class QtColorPropertyManager;
class QtFontPropertyManager;
class QtPointPropertyManager;
class QtSizePropertyManager;
class QtGroupPropertyManager;

class QtDoubleSpinBoxFactory;
class QtCheckBoxFactory;
class QtSpinBoxFactory;
class QtLineEditFactory;
class QtEnumEditorFactory;
class QtScrollBarFactory;

class QtProperty;
class CAxisFrame;
class CLineWidget;
class QtTreePropertyBrowser;
class QtEnumPropertyManager;
class CBindInfoFactory;
class CBindInfoManager;
class QtBoolPropertyManager;
class CCreateNewTemplateWgt;


enum WIDGET_MOUSE_MODES
{
	WIDGET_MOUSE_NOTHING,
	WIDGET_MOUSE_SELECT,
	WIDGET_MOUSE_MOVE,
	//	WIDGET_MOUSE_MOVEING,
	WIDGET_MOUSE_SIZE,
	WIDGET_MOUSE_SIZING,
	WIDGET_MOUSE_ROTATE,
	WIDGET_MOUSE_ROTATING
};

enum PROPERTYITEM
{
	//视图属性
	ITEM_GRAPHICS_BASE,   //视图基本属性
	//缩放比例 
	ITEM_GRAPHICS_SCALE,
	//大小
	ITEM_GREPHICS_SIZE,
	//背景色
	ITEM_BACKGROUND,
	//背景颜色
	ITEM_BACKGROUND_COLOR,
	//背景颜色—颜色
	ITEM_BACKGROUND_COLOR_COLOR,
	//背景颜色  -  画刷类型
	ITEM_BACKGROUND_COLOR_TYPE,
	//背景颜色 渐变类型
	ITEM_BACKGROUND_COLOR_GRADIENT_TYPE,
	//渐变颜色
	ITEM_BACKGROUND_COLOR_GRADIENT_COLOR,

	//
	//背景图片
	ITEM_BACKGROUND_IMAGE,
	//背景图片 选择
	ITEM_BACKGROUND_IMAGE_IMAGE,
	//图形坐标
	ITEM_BACKGROUND_IMAGE_XY,
	//图形大小
	ITEM_BACKGROUND_IMAGE_SIZE,
	//平铺
	ITEM_BACKGROUND_IMAGE_TILING,
	//拉伸
	ITEM_BACKGROUND_IMAGE_STRETCH,

	//图元背景图片
	ITEM_BACKGROUND_ITEM_IAMGE,
	//图元 背景图片 选择
	ITEM_BACKGROUND_ITEM_IMAGE_IMAGE,
	//图元 图形坐标
	ITEM_BACKGROUND_ITEM_IMAGE_XY,
	//图元 图形大小
	ITEM_BACKGROUND_ITEM_IMAGE_SIZE,
	//图元 平铺
	ITEM_BACKGROUND_ITEM_IMAGE_TILING,
	//图元 拉伸
	ITEM_BACKGROUND_ITEM_IMAGE_STRETCH,

	//图元属性
	ITEM_BASE_ID_NAME,  // 基础属性

	ITEM_BASE_PRO,   //item静态属性

	ITEM_NAME,      //名称
	ITEM_ID,        //ID

	ITEM_LOCK,    //锁定
	ITEM_PSOITION, //位置
	ITEM_SIZE,    //尺寸
	ITEM_ROTATE_CENTER,   //旋转中西
	ITEM_ROTATE,   //旋转角度

	ITEM_END_ANGLE,  //终止角度

	ITEM_PEN,         //画笔
	ITEM_PEN_COLOR,  //画笔颜色
	ITEM_PEN_WIDTH,   //画笔宽度
	ITEM_PEN_TYPE,     //画笔类型

	ITEM_BRUSH,         //画刷
	ITEM_BRUSH_TYPE,    //画刷类型
	ITEM_BURSH_COLOR,    //画刷颜色
	ITEM_BRUSH_GRADIENT,  //画刷渐变类型
	ITEM_BRUSH_ENDGRADIENTCOLOR,   //画刷终止颜色

	ITEM_TEXT_COLOR,      //文本颜色
	ITEM_TEXT,            //文本
	ITEM_TEXT_FONT,        //字体属性
	ITEM_TEXT_POSITION,     //文字排列方式

	ITEM_DYNAMC_ATTR,     //动态属性

	ITEM_DYNAMIC_BRUSH,         //动态画刷
	ITEM_DYNAMIC_BRUSH_ENABLE,   //动态画刷启动 
	ITEM_DYNAMIC_BRUSH_DATA_TYPE,  //动态画刷类型
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_NULL,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_BOOL,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_CHAR,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_BYTE,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_SHORT,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_WORD,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_INT,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_DWORD,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_LLONG,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_LWORD,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_FOLAT,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_DOUBLE,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_STRING,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_BLOCK,
	ITEM_DYNAMIC_BRUSH_DATA_TYPE_MAX,


	ITEM_DYNAMIC_BRUSH_DATA_WRITEABLE,   //是否可写
	ITEM_DYNAMIC_BRUSH_COLOR,            //动态画刷颜色
	ITEM_DYNAMIC_BRUSH_BINDPOINT,        //绑点
	ITEM_DYNAMIC_BRUSH_ANALOG_COLOR,     //模拟色
	ITEM_DYNAMIC_BRUSH_START_VALUE,      //起始值
	ITEM_DYNAMIC_BRUSH_END_VALUE,        //终止值
	ITEM_DYNAMIC_BRUSH_START_OFFSET,     //起始偏移
	ITEM_DYNAMIC_BRUSH_END_OFFSET,      //终止偏移
	ITEM_DYNAMIC_BRUSH_TYPE,            //类型



	ITEM_COLOR_RANGE = 1043,              //颜色阈值
	ITEM_BIND_VAR,                 //绑定值
	ITEM_TEXT_RANGE,               //文本阈值 
	ITEM_MAP_RANGE,                //图片阈值
	ITEM_BACKGROUND_IMAGE_CODE,         //背景图片
	ITEM_PUISHBUTTON_ORDER_CODE,     //绑定button

	ITEM_DYNAMIC_PEN_COLOR,   //动态PEN 颜色
	ITEM_DYNAMIC_PEN_COLOR_ENABLE,
	ITEM_DYNAMIC_PEN_COLOR_COLOR,
	ITEM_DYNAMIC_PEN_COLOR_BINDATA,


	ITEM_DYNAMIC_TEXT_COLOR,   //动态字体颜色
	ITEM_DYNAMIC_TEXT_COLOR_ENABLE,
	ITEM_DYNAMIC_TEXT_COLOR_COLOR,
	ITEM_DYNAMIC_TEXT_COLOR_BINDATA,

	//水平移动
	ITEM_DYNAMIC_HORIZONTAL,    //水平
	ITEM_DYNAMIC_HORIZONTAL_ENABLE,   //是否可用
	ITEM_DYNAMIC_HORIZONTAL_RESVESE,  //反转
	ITEM_DYNAMIC_HORIZONTAL_BINDDATA,  //绑定
	ITEM_DYNAMIC_HORIZONTAL_START,  //起始值
	ITEM_DYNAMIC_HORIZONTAL_END,   //结束值
	ITEM_DYNAMIC_HORIZONTAL_STARTOFFSET,  //起始偏移值
	ITEM_DYNAMIC_HORIZONTAL_ENDOFFSET,    //结束偏移值
	ITEM_DYNAMIC_HORIZONTAL_DISTANCE,     //水平位移


	//竖直移动
	ITEM_DYNAMIC_VERTICAL,   //竖直
	ITEM_DYNAMIC_VERTICAL_ENABLE,   //是否可用
	ITEM_DYNAMIC_VERTICAL_RESVESE,  //反转
	ITEM_DYNAMIC_VERTICAL_BINDDATA,  //绑定
	ITEM_DYNAMIC_VERTICAL_START,  //起始值
	ITEM_DYNAMIC_VERTICAL_END,   //结束值
	ITEM_DYNAMIC_VERTICAL_STARTOFFSET,  //起始偏移值
	ITEM_DYNAMIC_VERTICAL_ENDOFFSET,    //结束偏移值
	ITEM_DYNAMIC_VERTICAL_DISTANCE,     //竖直位移

	//组合移动
	ITEM_DYNAMIC_GROUP_MOVING,
	ITEM_DYNAMIC_GROUP_MOVING_ENABLE,   //是否可用
	ITEM_DYNAMIC_GROUP_MOVING_BINDDATA,   //绑定
	ITEM_DYNAMIC_GROUP_MOVING_START,    //起始值
	ITEM_DYNAMIC_GROUP_MOVING_END,   //结束值
	ITEM_DYNAMIC_GROUP_MOVING_EDIT,   //编辑
	ITEM_DYNAMIC_GROUP_MOVING_RESET,   //复位

	//旋转
	ITEM_DYNAMIC_ROTATE,   //旋转
	ITEM_DYNAMIC_ROTATE_ENABLE,   //是否可用
	ITEM_DYNAMIC_ROTATE_RESVESE,  //反转
	ITEM_DYNAMIC_ROTATE_BINDDATA,  //绑定
	ITEM_DYNAMIC_ROTATE_START,  //起始值
	ITEM_DYNAMIC_ROTATE_END,   //结束值
	ITEM_DYNAMIC_ROTATE_STARTOFFSET,  //起始偏移值
	ITEM_DYNAMIC_ROTATE_ENDOFFSET,    //结束偏移值
	ITEM_DYNAMIC_ROTATE_STARTANGLE,  //起始角度
	ITEM_DYNAMIC_ROTATE_ENDANGLE,    //结束角度


	//透明
	ITEM_DYNAMIC_TRANSPARENCY,     //透明
	ITEM_DYNAMIC_TRANSPARENCY_ENABLE,   //是否可用
	ITEM_DYNAMIC_TRANSPARENCY_BINDDATA,   //绑定值
	ITEM_DYNAMIC_TRANSPARENCY_START,     //绑定值起始值
	ITEM_DYNAMIC_TRANSPARENCY_END,       //绑定值 结束值

	//可见
	ITEM_DYNAMIC_VISIBLE,    //动态可见
	ITEM_DYNAMIC_VISABLE_ENABLE,   //是否可用
	ITEM_DYNAMIC_VISABLE_BINDDATA,   //绑定值
	ITEM_DYNAMIC_VISAVLE_VALUE,  //值
	ITEM_DYNAMCI_VISABLE_AREA,  //死区
	ITEM_DYNAMIC_VISABLE_COMPARE,  //比较条件
	ITEM_DYNAMIC_VISABLE_FLASHTYPE,  //闪烁类型 
	ITEM_DYNAMIC_VISABLE_CYCLE,    //闪烁周期

    //动态数值
    ITEM_DYNAMIC_VALUE,               //动态数值
	ITEM_DYNAMIC_VALUE_ENABLE,        //是否可用
	ITEM_DYNAMIC_VALUE_BINDDATA,      //绑定值
	ITEM_DYNAMIC_VALUE_VALUE_FORMAT,  //格式

	//动态文本
	ITEM_DYNAMIC_TEXT,    
	ITEM_DYNAMIC_TEXT_ENABLE,        //是否启用
    ITEM_DYNAMIC_TEXT_BINDDATA,      //绑定值
	ITEM_DYNAMIC_TEXT_VALUES,   //显示文本

    //动态图片
	ITEM_DYNAMIC_MAP,    //动态图片
	ITEM_DYNAMIC_MAP_ENABLE,       //是否启用
	ITEM_DYNAMIC_MAP_BINIDDATA,    //绑定值
	ITEM_DYNAMIC_MAP_VALUES,      //图片路径

	//曲线图表样式
	ITEM_CURVEPLOT_STYLE,
    //曲线边框宽度
	ITEM_CURVEFRAM_WIDTH,
	//曲线图表画刷类型
    ITEM_CUREVEPLOT_BRUSH_STYLE,
	//曲线图表画刷颜色
    ITEM_CURVEPLOT_BURSH_COLOR,


   ITEM_PHSUBBUTTON_EXEC,     //pushbutton执行
   TTEM_PUSHBUTTON_VAR,      //pusubutton绑定值
   ITEM_PUSHBUTTON_TYPE,     //命令类型
   ITEM_PUSHBUTTON_PRESSORDER,    //press命令
   ITEM_PUSHBUTTON_RELEASEORDER,    //release命令

    
};


class QtBrowserItem;
class CGroupMoveLine;
class CGroupWidget;
class CGraphView : public QGraphicsView
{
	Q_OBJECT
public:

	CGraphView(QWidget *parent = 0);
	CGraphView(QGraphicsScene *scene, QWidget * parent = 0);

	~CGraphView();

	enum 
	{
		//最多redo  undo 次数
		MAX_REDO_UNDO_TIMES = 16,
	};
public:
	unsigned int  m_nDrawTool;
	unsigned int  m_nMouseMode;
public:
	bool AddNewWidget(const QRectF & rcPos, unsigned int nWidgetType);
	//	void SelectWidget(CBaseWidget* pObj);

	void MoveWidgets(const QPointF & delta);

	void MovingWidgets(const QPointF & delta);

	void SelectAll(unsigned int nLayerIdx);
	void Select(CBaseWidget* pObj);
	void Deselect(CBaseWidget* pObj);
	void SelectWidgetsInRect(const QRectF &rcSel);

	void InvalidateWidget(CBaseWidget* pObj);
	//保持历史信息  供redo  undo使用
	void AddHistFiles();
	//
	void ChangeProperty();
public slots:
	
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnCut();
	void OnAttr();
	//旋转
	void OnRotate();
	//移动
	void OnSizeChange();

	bool MoveWidgets(unsigned int nPixel, int nKey);

	void OnWidgetsGroup();
	void OnWidgetsBreak();

	//图形库
	void OnSaveTemplate();

	void OnAutoAlign();
	void OnLeftAlign();
	void OnRightAlign();
	void OnCenterHorzAlign();
	void OnTopAlign();
	void OnCenterVertAlign();
	void OnButtomAlign();

	void MoveToBottomLevel();
	void MoveToTopLevel();
	void MoveUp();
	void MoveDown();
	//图层上移
	void OnLayerUp();
	//图层下移
	void OnLayerDown();
	//导出
	void OnExport();


public:

	unsigned int GetCurEditingLayerIdx()
	{
		return m_nCurEditingLayerIndex;
	}
	unsigned int  SetLayerVisible(unsigned int nLayIdx, bool bDisp);
	void SetCurEditingLayer(unsigned int nLayerIdx);
protected:
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
	virtual void drawForeground(QPainter *painter, const QRectF &rect);
	void resizeEvent(QResizeEvent * event);

	void DrawRuler(QPainter *pPainter, const QRectF & rcViewPort, const QRectF & rcScene);

	// 	void MoveSelectionWidgets(const QPoint & ptMove);
	void ResizeSelectionWidgets(QPointF & ptInput);

	void BuildSelectBox(const QRectF & rcSel);
	void BuildSelectBox(const QPointF & ptCur);
	void DrawSelectBox(QPainter *painter);

	void OnLButtonDown(QMouseEvent * e);
	void OnLButtonUp(QMouseEvent * e);

	CBaseWidget *HitTest(QPoint & pos, unsigned int nLayerIdx);
	// 获取鼠标在图元上的爪子的位置
	int GetMouseOnHandleIdx(const QPointF & pointf);

	void ClearPropertyBrowser();
	void ClearPropertyBrowser(QtTreePropertyBrowser *pTreeBrowser);

	void ShowPropertyBrowser(CBaseWidget *pWidget, QtTreePropertyBrowser *pTreeBrowser);
	//初始化视图属性
	void InitGrpahicAttr(QtTreePropertyBrowser *pTreeBrowser);

	void ShowPenProperty(QtProperty *pParent, CPenInfo *pInfo);
	void ShowBrushProperty(QtProperty *pParent, CBrushInfo *pInfo);
	void ShowTextProperty(QtProperty *pParent, QString & szTxt, CFontInfo *pInfo);
	//图元背景图片
	void ShowItemImageProperty(QtProperty *pParent);

	//动态颜色二层结构
	void ShowDynamicColorProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	//动态移动二层结果哦
	void ShowDynamicMovintProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//组合移动二层结构
	void ShowDynamicGroupMovingProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	//动态特命二层结构
	void ShowDynamicTransparencyProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//动态可见二层结构
	void ShowDynamicVisableProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	//动态数值二层结构
	void ShowDynamicValueProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//动态文本二层结构
	void ShowDynamicTextProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//动态图片二层结构
	void ShowDynamicMapProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	void SetBindDataTypeProperty(QtProperty *pParent);

	void AddPropertyToMap(QtProperty *pProp, int nProCode);
	void ClearPropMap();
	//多选
	QPointF GetMutlStartPoint();
	//复制基础属性
	void CopyBaseProperty(CBaseWidget *pDescWgt, CBaseWidget *pSourceWgt);
	//复制动态属性
	void CopyDynamicProperty(CBaseWidget *pDescWgt, CBaseWidget *pSourceWgt);
	//颜色
	//void ChangeBrowserColor(QtBrowserItem *pBrowser,const QColor& color,int nIndex);
	//加载组合移动点
	void InsertGroupMovePoints();
	//删除组合移动点
	void DeleteGroupMovePoints();
	//隐藏图元属性
	void SetGraphicItemAttrHidden();
	//隐藏视图属性
	void SetGraphicAttrHidden();
	//复制组
	CGroupWidget *CopyGroupInfo(CGroupWidget* pGroup);
// 	//设置缩放比例
// 	void SetScaled(int nScale)
// 	{
// 		m_scaleFactor = nScale;
// 	}
// 
// 	//获取缩放比例
// 	int GetSacled()
// 	{
// 		return m_scaleFactor;
// 	}

protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);
	virtual void mouseDoubleClickEvent(QMouseEvent * e);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void paintEvent(QPaintEvent *event);

	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void dropEvent(QDropEvent *event);
	//
	void wheelEvent(QWheelEvent *e);

	private slots:
	void slot_SliderMoved();
	void valueChanged(QtProperty *property, int value);
	void valueChanged(QtProperty *property, double value);
	void valueChanged(QtProperty *property, const QString &value);
	void valueChanged(QtProperty *property, const QVariant &tValue);
	void valueChanged(QtProperty *property, const QFont &value);
	void valueChanged(QtProperty *property, const QPoint &value);
	void valueChanged(QtProperty *property, const QSize &value);

	void Slot_BoolValueChange(QtProperty *property, bool bFlag);
	//
	void Slot_ScrollBarChange();
	//显示属性
	void Slot_ShowItemProperty();
private:
	QtIntPropertyManager    *m_pIntManager;
	QtDoublePropertyManager *m_pDoubleManager;
	QtStringPropertyManager	*m_pStringManager;
	//
	QtVariantPropertyManager  *m_pColorManager;
	QtFontPropertyManager   *m_pFontManager;
	QtPointPropertyManager  *m_pPointManager;
	QtSizePropertyManager   *m_pSizeManager;
	QtGroupPropertyManager  *m_pGroupManager;

	QtDoubleSpinBoxFactory  *m_pDoubleSpinBoxFactory;
	QtCheckBoxFactory       *m_pCheckBoxFactory;
	QtSpinBoxFactory        *m_pSpinBoxFactory;
	QtLineEditFactory       *m_pLineEditFactory;
	QtEnumEditorFactory     *m_pComboBoxFactory;
	QtScrollBarFactory      *m_pScrollBarFactory;

	QtVariantPropertyManager *m_pVariantManager;
	QtVariantEditorFactory   *m_pVariantFactory;

	//自定义属性栏
	CBindInfoManager *m_pBindInfoManager;
	CBindInfoFactory *m_pBindInfoFactory;

	QMap<QtProperty *, int> m_mapPropertyToId;
	QMap<int, QtProperty *> m_mapIdToProperty;
	CBaseWidget *m_pPorpWidget;

	//返回场景
	CGraphScene *GetGraphicsScene() const;

	QPointF m_ptFisrst, m_ptLast;
	//

	int				m_nDragHandle;

	unsigned int m_nCurEditingLayerIndex; //! 当前正在编辑的图层
	//! 画选择框
	bool m_bDrawSelRect;

	//! 选择框
	QRectF m_rcSelectBox;
	QRectF m_rcOldSelBox;

	CAxisFrame *m_pXFrame;
	CAxisFrame *m_pYFrame;
	//长度
	int m_nViewPortWidth;
	//高度
	int m_nViewPortHeight;
	//移动标志  区分选中和移动
	bool m_bMovingFlag;
	//复制  剪切对象
	std::vector <CBaseWidget *> m_pArrCpyCutWgt;
	//复制  剪切标签  1复制   2剪切
	int m_nCutCpyFlag;
	//当前鼠标点击位置
	QPointF m_CurrentClickPoint;
	//折线
	CLineWidget *m_pPloyLine;
	//
// 	int m_scaleFactor;
	//属性信息
	QtTreePropertyBrowser *m_pTreeProperty;
	//
	QtEnumPropertyManager *m_pComBoProperty;
	//checkbox
	QtBoolPropertyManager *m_pBoolManager;
	//界面不可编辑状态
	bool m_bViewEdable;
	//
	CGroupMoveLine *m_pGroupMoveLine;
	//redo  undo
	QList<CGraphFile*> m_lstGraphFiles;
	//当前所处的redo  undo 位置
	int m_nCurrentFile;
	//更新标识
	bool m_bChangeFlag;
	//创建图形库界面
	CCreateNewTemplateWgt *m_pCreateTemplateWgt;
};

#endif // GRAPH_VIEW_H

/** @}*/

