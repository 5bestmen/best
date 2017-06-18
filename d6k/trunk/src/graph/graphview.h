/*! @file graphview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graphview.h
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
	//��ͼ����
	ITEM_GRAPHICS_BASE,   //��ͼ��������
	//���ű��� 
	ITEM_GRAPHICS_SCALE,
	//��С
	ITEM_GREPHICS_SIZE,
	//����ɫ
	ITEM_BACKGROUND,
	//������ɫ
	ITEM_BACKGROUND_COLOR,
	//������ɫ����ɫ
	ITEM_BACKGROUND_COLOR_COLOR,
	//������ɫ  -  ��ˢ����
	ITEM_BACKGROUND_COLOR_TYPE,
	//������ɫ ��������
	ITEM_BACKGROUND_COLOR_GRADIENT_TYPE,
	//������ɫ
	ITEM_BACKGROUND_COLOR_GRADIENT_COLOR,

	//
	//����ͼƬ
	ITEM_BACKGROUND_IMAGE,
	//����ͼƬ ѡ��
	ITEM_BACKGROUND_IMAGE_IMAGE,
	//ͼ������
	ITEM_BACKGROUND_IMAGE_XY,
	//ͼ�δ�С
	ITEM_BACKGROUND_IMAGE_SIZE,
	//ƽ��
	ITEM_BACKGROUND_IMAGE_TILING,
	//����
	ITEM_BACKGROUND_IMAGE_STRETCH,

	//ͼԪ����ͼƬ
	ITEM_BACKGROUND_ITEM_IAMGE,
	//ͼԪ ����ͼƬ ѡ��
	ITEM_BACKGROUND_ITEM_IMAGE_IMAGE,
	//ͼԪ ͼ������
	ITEM_BACKGROUND_ITEM_IMAGE_XY,
	//ͼԪ ͼ�δ�С
	ITEM_BACKGROUND_ITEM_IMAGE_SIZE,
	//ͼԪ ƽ��
	ITEM_BACKGROUND_ITEM_IMAGE_TILING,
	//ͼԪ ����
	ITEM_BACKGROUND_ITEM_IMAGE_STRETCH,

	//ͼԪ����
	ITEM_BASE_ID_NAME,  // ��������

	ITEM_BASE_PRO,   //item��̬����

	ITEM_NAME,      //����
	ITEM_ID,        //ID

	ITEM_LOCK,    //����
	ITEM_PSOITION, //λ��
	ITEM_SIZE,    //�ߴ�
	ITEM_ROTATE_CENTER,   //��ת����
	ITEM_ROTATE,   //��ת�Ƕ�

	ITEM_END_ANGLE,  //��ֹ�Ƕ�

	ITEM_PEN,         //����
	ITEM_PEN_COLOR,  //������ɫ
	ITEM_PEN_WIDTH,   //���ʿ��
	ITEM_PEN_TYPE,     //��������

	ITEM_BRUSH,         //��ˢ
	ITEM_BRUSH_TYPE,    //��ˢ����
	ITEM_BURSH_COLOR,    //��ˢ��ɫ
	ITEM_BRUSH_GRADIENT,  //��ˢ��������
	ITEM_BRUSH_ENDGRADIENTCOLOR,   //��ˢ��ֹ��ɫ

	ITEM_TEXT_COLOR,      //�ı���ɫ
	ITEM_TEXT,            //�ı�
	ITEM_TEXT_FONT,        //��������
	ITEM_TEXT_POSITION,     //�������з�ʽ

	ITEM_DYNAMC_ATTR,     //��̬����

	ITEM_DYNAMIC_BRUSH,         //��̬��ˢ
	ITEM_DYNAMIC_BRUSH_ENABLE,   //��̬��ˢ���� 
	ITEM_DYNAMIC_BRUSH_DATA_TYPE,  //��̬��ˢ����
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


	ITEM_DYNAMIC_BRUSH_DATA_WRITEABLE,   //�Ƿ��д
	ITEM_DYNAMIC_BRUSH_COLOR,            //��̬��ˢ��ɫ
	ITEM_DYNAMIC_BRUSH_BINDPOINT,        //���
	ITEM_DYNAMIC_BRUSH_ANALOG_COLOR,     //ģ��ɫ
	ITEM_DYNAMIC_BRUSH_START_VALUE,      //��ʼֵ
	ITEM_DYNAMIC_BRUSH_END_VALUE,        //��ֵֹ
	ITEM_DYNAMIC_BRUSH_START_OFFSET,     //��ʼƫ��
	ITEM_DYNAMIC_BRUSH_END_OFFSET,      //��ֹƫ��
	ITEM_DYNAMIC_BRUSH_TYPE,            //����



	ITEM_COLOR_RANGE = 1043,              //��ɫ��ֵ
	ITEM_BIND_VAR,                 //��ֵ
	ITEM_TEXT_RANGE,               //�ı���ֵ 
	ITEM_MAP_RANGE,                //ͼƬ��ֵ
	ITEM_BACKGROUND_IMAGE_CODE,         //����ͼƬ
	ITEM_PUISHBUTTON_ORDER_CODE,     //��button

	ITEM_DYNAMIC_PEN_COLOR,   //��̬PEN ��ɫ
	ITEM_DYNAMIC_PEN_COLOR_ENABLE,
	ITEM_DYNAMIC_PEN_COLOR_COLOR,
	ITEM_DYNAMIC_PEN_COLOR_BINDATA,


	ITEM_DYNAMIC_TEXT_COLOR,   //��̬������ɫ
	ITEM_DYNAMIC_TEXT_COLOR_ENABLE,
	ITEM_DYNAMIC_TEXT_COLOR_COLOR,
	ITEM_DYNAMIC_TEXT_COLOR_BINDATA,

	//ˮƽ�ƶ�
	ITEM_DYNAMIC_HORIZONTAL,    //ˮƽ
	ITEM_DYNAMIC_HORIZONTAL_ENABLE,   //�Ƿ����
	ITEM_DYNAMIC_HORIZONTAL_RESVESE,  //��ת
	ITEM_DYNAMIC_HORIZONTAL_BINDDATA,  //��
	ITEM_DYNAMIC_HORIZONTAL_START,  //��ʼֵ
	ITEM_DYNAMIC_HORIZONTAL_END,   //����ֵ
	ITEM_DYNAMIC_HORIZONTAL_STARTOFFSET,  //��ʼƫ��ֵ
	ITEM_DYNAMIC_HORIZONTAL_ENDOFFSET,    //����ƫ��ֵ
	ITEM_DYNAMIC_HORIZONTAL_DISTANCE,     //ˮƽλ��


	//��ֱ�ƶ�
	ITEM_DYNAMIC_VERTICAL,   //��ֱ
	ITEM_DYNAMIC_VERTICAL_ENABLE,   //�Ƿ����
	ITEM_DYNAMIC_VERTICAL_RESVESE,  //��ת
	ITEM_DYNAMIC_VERTICAL_BINDDATA,  //��
	ITEM_DYNAMIC_VERTICAL_START,  //��ʼֵ
	ITEM_DYNAMIC_VERTICAL_END,   //����ֵ
	ITEM_DYNAMIC_VERTICAL_STARTOFFSET,  //��ʼƫ��ֵ
	ITEM_DYNAMIC_VERTICAL_ENDOFFSET,    //����ƫ��ֵ
	ITEM_DYNAMIC_VERTICAL_DISTANCE,     //��ֱλ��

	//����ƶ�
	ITEM_DYNAMIC_GROUP_MOVING,
	ITEM_DYNAMIC_GROUP_MOVING_ENABLE,   //�Ƿ����
	ITEM_DYNAMIC_GROUP_MOVING_BINDDATA,   //��
	ITEM_DYNAMIC_GROUP_MOVING_START,    //��ʼֵ
	ITEM_DYNAMIC_GROUP_MOVING_END,   //����ֵ
	ITEM_DYNAMIC_GROUP_MOVING_EDIT,   //�༭
	ITEM_DYNAMIC_GROUP_MOVING_RESET,   //��λ

	//��ת
	ITEM_DYNAMIC_ROTATE,   //��ת
	ITEM_DYNAMIC_ROTATE_ENABLE,   //�Ƿ����
	ITEM_DYNAMIC_ROTATE_RESVESE,  //��ת
	ITEM_DYNAMIC_ROTATE_BINDDATA,  //��
	ITEM_DYNAMIC_ROTATE_START,  //��ʼֵ
	ITEM_DYNAMIC_ROTATE_END,   //����ֵ
	ITEM_DYNAMIC_ROTATE_STARTOFFSET,  //��ʼƫ��ֵ
	ITEM_DYNAMIC_ROTATE_ENDOFFSET,    //����ƫ��ֵ
	ITEM_DYNAMIC_ROTATE_STARTANGLE,  //��ʼ�Ƕ�
	ITEM_DYNAMIC_ROTATE_ENDANGLE,    //�����Ƕ�


	//͸��
	ITEM_DYNAMIC_TRANSPARENCY,     //͸��
	ITEM_DYNAMIC_TRANSPARENCY_ENABLE,   //�Ƿ����
	ITEM_DYNAMIC_TRANSPARENCY_BINDDATA,   //��ֵ
	ITEM_DYNAMIC_TRANSPARENCY_START,     //��ֵ��ʼֵ
	ITEM_DYNAMIC_TRANSPARENCY_END,       //��ֵ ����ֵ

	//�ɼ�
	ITEM_DYNAMIC_VISIBLE,    //��̬�ɼ�
	ITEM_DYNAMIC_VISABLE_ENABLE,   //�Ƿ����
	ITEM_DYNAMIC_VISABLE_BINDDATA,   //��ֵ
	ITEM_DYNAMIC_VISAVLE_VALUE,  //ֵ
	ITEM_DYNAMCI_VISABLE_AREA,  //����
	ITEM_DYNAMIC_VISABLE_COMPARE,  //�Ƚ�����
	ITEM_DYNAMIC_VISABLE_FLASHTYPE,  //��˸���� 
	ITEM_DYNAMIC_VISABLE_CYCLE,    //��˸����

    //��̬��ֵ
    ITEM_DYNAMIC_VALUE,               //��̬��ֵ
	ITEM_DYNAMIC_VALUE_ENABLE,        //�Ƿ����
	ITEM_DYNAMIC_VALUE_BINDDATA,      //��ֵ
	ITEM_DYNAMIC_VALUE_VALUE_FORMAT,  //��ʽ

	//��̬�ı�
	ITEM_DYNAMIC_TEXT,    
	ITEM_DYNAMIC_TEXT_ENABLE,        //�Ƿ�����
    ITEM_DYNAMIC_TEXT_BINDDATA,      //��ֵ
	ITEM_DYNAMIC_TEXT_VALUES,   //��ʾ�ı�

    //��̬ͼƬ
	ITEM_DYNAMIC_MAP,    //��̬ͼƬ
	ITEM_DYNAMIC_MAP_ENABLE,       //�Ƿ�����
	ITEM_DYNAMIC_MAP_BINIDDATA,    //��ֵ
	ITEM_DYNAMIC_MAP_VALUES,      //ͼƬ·��

	//����ͼ����ʽ
	ITEM_CURVEPLOT_STYLE,
    //���߱߿���
	ITEM_CURVEFRAM_WIDTH,
	//����ͼ��ˢ����
    ITEM_CUREVEPLOT_BRUSH_STYLE,
	//����ͼ��ˢ��ɫ
    ITEM_CURVEPLOT_BURSH_COLOR,


   ITEM_PHSUBBUTTON_EXEC,     //pushbuttonִ��
   TTEM_PUSHBUTTON_VAR,      //pusubutton��ֵ
   ITEM_PUSHBUTTON_TYPE,     //��������
   ITEM_PUSHBUTTON_PRESSORDER,    //press����
   ITEM_PUSHBUTTON_RELEASEORDER,    //release����

    
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
		//���redo  undo ����
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
	//������ʷ��Ϣ  ��redo  undoʹ��
	void AddHistFiles();
	//
	void ChangeProperty();
public slots:
	
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnCut();
	void OnAttr();
	//��ת
	void OnRotate();
	//�ƶ�
	void OnSizeChange();

	bool MoveWidgets(unsigned int nPixel, int nKey);

	void OnWidgetsGroup();
	void OnWidgetsBreak();

	//ͼ�ο�
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
	//ͼ������
	void OnLayerUp();
	//ͼ������
	void OnLayerDown();
	//����
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
	// ��ȡ�����ͼԪ�ϵ�צ�ӵ�λ��
	int GetMouseOnHandleIdx(const QPointF & pointf);

	void ClearPropertyBrowser();
	void ClearPropertyBrowser(QtTreePropertyBrowser *pTreeBrowser);

	void ShowPropertyBrowser(CBaseWidget *pWidget, QtTreePropertyBrowser *pTreeBrowser);
	//��ʼ����ͼ����
	void InitGrpahicAttr(QtTreePropertyBrowser *pTreeBrowser);

	void ShowPenProperty(QtProperty *pParent, CPenInfo *pInfo);
	void ShowBrushProperty(QtProperty *pParent, CBrushInfo *pInfo);
	void ShowTextProperty(QtProperty *pParent, QString & szTxt, CFontInfo *pInfo);
	//ͼԪ����ͼƬ
	void ShowItemImageProperty(QtProperty *pParent);

	//��̬��ɫ����ṹ
	void ShowDynamicColorProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	//��̬�ƶ�������Ŷ
	void ShowDynamicMovintProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//����ƶ�����ṹ
	void ShowDynamicGroupMovingProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	//��̬��������ṹ
	void ShowDynamicTransparencyProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//��̬�ɼ�����ṹ
	void ShowDynamicVisableProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	//��̬��ֵ����ṹ
	void ShowDynamicValueProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//��̬�ı�����ṹ
	void ShowDynamicTextProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);
	//��̬ͼƬ����ṹ
	void ShowDynamicMapProperty(QtProperty *pParent, CBaseDyncData::DYNC_TYPE);

	void SetBindDataTypeProperty(QtProperty *pParent);

	void AddPropertyToMap(QtProperty *pProp, int nProCode);
	void ClearPropMap();
	//��ѡ
	QPointF GetMutlStartPoint();
	//���ƻ�������
	void CopyBaseProperty(CBaseWidget *pDescWgt, CBaseWidget *pSourceWgt);
	//���ƶ�̬����
	void CopyDynamicProperty(CBaseWidget *pDescWgt, CBaseWidget *pSourceWgt);
	//��ɫ
	//void ChangeBrowserColor(QtBrowserItem *pBrowser,const QColor& color,int nIndex);
	//��������ƶ���
	void InsertGroupMovePoints();
	//ɾ������ƶ���
	void DeleteGroupMovePoints();
	//����ͼԪ����
	void SetGraphicItemAttrHidden();
	//������ͼ����
	void SetGraphicAttrHidden();
	//������
	CGroupWidget *CopyGroupInfo(CGroupWidget* pGroup);
// 	//�������ű���
// 	void SetScaled(int nScale)
// 	{
// 		m_scaleFactor = nScale;
// 	}
// 
// 	//��ȡ���ű���
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
	//��ʾ����
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

	//�Զ���������
	CBindInfoManager *m_pBindInfoManager;
	CBindInfoFactory *m_pBindInfoFactory;

	QMap<QtProperty *, int> m_mapPropertyToId;
	QMap<int, QtProperty *> m_mapIdToProperty;
	CBaseWidget *m_pPorpWidget;

	//���س���
	CGraphScene *GetGraphicsScene() const;

	QPointF m_ptFisrst, m_ptLast;
	//

	int				m_nDragHandle;

	unsigned int m_nCurEditingLayerIndex; //! ��ǰ���ڱ༭��ͼ��
	//! ��ѡ���
	bool m_bDrawSelRect;

	//! ѡ���
	QRectF m_rcSelectBox;
	QRectF m_rcOldSelBox;

	CAxisFrame *m_pXFrame;
	CAxisFrame *m_pYFrame;
	//����
	int m_nViewPortWidth;
	//�߶�
	int m_nViewPortHeight;
	//�ƶ���־  ����ѡ�к��ƶ�
	bool m_bMovingFlag;
	//����  ���ж���
	std::vector <CBaseWidget *> m_pArrCpyCutWgt;
	//����  ���б�ǩ  1����   2����
	int m_nCutCpyFlag;
	//��ǰ�����λ��
	QPointF m_CurrentClickPoint;
	//����
	CLineWidget *m_pPloyLine;
	//
// 	int m_scaleFactor;
	//������Ϣ
	QtTreePropertyBrowser *m_pTreeProperty;
	//
	QtEnumPropertyManager *m_pComBoProperty;
	//checkbox
	QtBoolPropertyManager *m_pBoolManager;
	//���治�ɱ༭״̬
	bool m_bViewEdable;
	//
	CGroupMoveLine *m_pGroupMoveLine;
	//redo  undo
	QList<CGraphFile*> m_lstGraphFiles;
	//��ǰ������redo  undo λ��
	int m_nCurrentFile;
	//���±�ʶ
	bool m_bChangeFlag;
	//����ͼ�ο����
	CCreateNewTemplateWgt *m_pCreateTemplateWgt;
};

#endif // GRAPH_VIEW_H

/** @}*/

