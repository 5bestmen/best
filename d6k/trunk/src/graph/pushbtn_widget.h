#pragma once

#include <QTextOption>
#include "base_widget.h"


class CDyncEventData;
class CPushBtnWidget : public CBaseWidget
{
	Q_OBJECT

public:
	CPushBtnWidget();
	virtual ~CPushBtnWidget();

public:
	enum BTN_TYPE
	{
		NORMAL = 0,        //!< һ��
		BTN_3D1,           //3d��ť1
		BTN_3D2,
		BTN_3D3,
		BTN_3D4,
		EMGRCY_STOP_A,              //!< ��ͣA
		EMGRCY_STOP_B,            //!< ��ͣB
		EMGRCY_STOP_C,            //!< ��ͣC
		YELLOW_LIGHT,        //!< �Ƶ�
		BLUE_LIGHT,              //!< ����
		GREEN_LIGHT,     //�̵�
		RED_NIGHT, //���
		YELLOW_LED,                //!< ��LED
		BLUE_LED,             //!< ��LED
		GREEN_LED,               //��LED
		RED_LED,                //��LED
	};
public:

	CPushBtnWidget(QRectF rcPos, CPushBtnWidget::BTN_TYPE shape);
public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	// ��ת�����ĵ� 
	virtual QPointF GetRotationCenterPoint()const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	// 	virtual void DrawSingleSelectedTracker(QPainter *pPainter);
	// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);

	virtual void FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info);


	//������ťpath
	void GetNormalPath(QPainterPath& path);
	//������ť����path
	void GetNormalAddPath(QPainterPath& path);
	//������ť����path press
	void GetNormalRressAddPath(QPainterPath& path);
	//������ť�ƶ�path
	void GetNormalMovingPath(QPainterPath& path);

	//3d1path
	void Get3D1Path(QPainterPath& path);
	//3d1  movingpath
	void Get3D1MovingPath(QPainterPath& path);

	//3d2path
	void Get3D2Path(QPainterPath& path);
	//3d2  movingpath
	void Get3D2MovingPath(QPainterPath& path);

	//3d3path
	void Get3D3Path(QPainterPath& path);
	//3d1  movingpath
	void Get3D3MovingPath(QPainterPath& path);

	//3d4path
	void Get3D4Path(QPainterPath& path);
	//3d4  movingpath
	void Get3D4MovingPath(QPainterPath& path);

	//��������λ��
	void SetFontLayout(QTextOption &txtOpt);

	//��ȡ������Ϣ
	CDyncEventData* GetEventIntent()
	{
		return m_pEventIntent;
	}

private:
	BTN_TYPE m_Shape;
	//����
	CDyncEventData* m_pEventIntent;
};
