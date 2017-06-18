/*! @file widget_factory.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  widget_factory.h
�ļ�ʵ�ֹ��� :  ͼԪ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ���󹤳����ģʽ��Ϊ����Ԥ���ӿ�
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼԪ������
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.30
*******************************************************************************/
#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H   
 
 
enum DRAW_TOOLS
{
	DRAW_TOOLS_SEL = 0,
	DRAW_TOOLS_LINE,   //!< ����
	DRAW_TOOLS_ARC,    //!< ��
	DRAW_TOOLS_POLYLINE,  //!< ����
	DRAW_TOOLS_TRIANGLE, //!< ������
	DRAW_TOOLS_RECTANGLE, //!< ����
	DRAW_TOOLS_ELLIPSE,   //!< ��Բ
	DRAW_TOOLS_ROUND,   //!< Բ
	DRAW_TOOLS_ROUNDRECT, //!< Բ�Ǿ���
	DRAW_TOOLS_TRAPEZOID,//!< ����
	DRAW_TOOLS_PARALLELOGRAM, //!< ƽ���ı���
	DRAW_TOOLS_DIAMOND,       //!< ����
	DRAW_TOOLS_POLYGON,       //!< �����
	DRAW_TOOLS_CHORD,         //!< ����
	DRAW_TOOLS_GROUPS,        //���
	DRAW_TOOLS_CAMBER,         //����
	DRAW_TOOLS_PIE,           //!< ����
	DRAW_TOOLS_DONUT,         //! ���� 
	DRAW_TOOLS_TEXT,          //�ı�
	DRAW_TOOLS_BUTTON,
	DRAW_TOOLS_PIPE,           //�ܵ�
	DRAW_TOOLS_TABLE,       //!< ����
	DRAW_TOOLS_PROGRESSBAR, //!< ��ͼ
	DRAW_TOOLS_METER,       //!< �Ǳ�
	DRAW_TOOLS_RTCURVE,     //!< ʵʱ����

	DRAW_TOOLS_DATETIME,     //!< ����ʱ��

	DRAW_TOOLS_HTREPORT,
	DRAW_TOOLS_RTREPORT,
	DRAW_TOOLS_HTCURVE,

	DRAW_TOOLS_HTALARM,
	DRAW_TOOLS_RTALARM,
	DRAW_TOOLS_SYMBOL,

	DRAW_TOOLS_TEMPLATE,  //ͼ�ο�
	DRAW_TOOLS_TEMPLATE_FOLDER,  //ͼ�ο�Ŀ¼

	//��ť
	DRAW_TOOLS_BTN_NORMAL,        //!< һ��
	DRAW_TOOLS_BTN_3D1,           //3d��ť1
	DRAW_TOOLS_BTN_3D2,
	DRAW_TOOLS_BTN_3D3,
	DRAW_TOOLS_BTN_3D4,
	DRAW_TOOLS_BTN_EMGRCY_STOP_A,              //!< ��ͣA
	DRAW_TOOLS_BTN_EMGRCY_STOP_B,            //!< ��ͣB
	DRAW_TOOLS_BTN_EMGRCY_STOP_C,            //!< ��ͣC
	DRAW_TOOLS_BTN_YELLOW_LIGHT,        //!< �Ƶ�
	DRAW_TOOLS_BTN_BLUE_LIGHT,              //!< ����
	DRAW_TOOLS_BTN_GREEN_LIGHT,     //�̵�
	DRAW_TOOLS_BTN_RED_NIGHT, //���
	DRAW_TOOLS_BTN_YELLOW_LED,                //!< ��LED
	DRAW_TOOLS_BTN_BLUE_LED,             //!< ��LED
	DRAW_TOOLS_BTN_GREEN_LED,               //��LED
	DRAW_TOOLS_BTN_RED_LED,                //��LED

	//�����ؼ�
	DRAW_ADDR_CURE_PLOT,   //����ͼ
};

class CBaseWidget;

class CWidgetFactory
{
public:
	CWidgetFactory();
	~CWidgetFactory();
public: 
	static CBaseWidget * CreateWidget(const QRectF & rcPos,unsigned int nWidgetType);

//	static void DestroySerializer(CGraphSerializer *pSerializer);

};



#endif // WIDGET_FACTORY_H

/** @}*/

