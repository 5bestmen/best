/*! @file pieview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  pieview.h
�ļ�ʵ�ֹ��� :  ��״ͼ�ؼ�
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��״ͼ�ؼ�
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/
#ifndef PIEVIEW_H
#define PIEVIEW_H
#include <QPaintEvent>
#include <QLabel>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include "configinfo.h"

QT_CHARTS_USE_NAMESPACE

class CPieView : public QLabel
{
	Q_OBJECT

public:
	CPieView(CCtrlData* pData,QWidget *parent = Q_NULLPTR);
	~CPieView();
protected:
	void paintEvent(QPaintEvent *e);
	void InitView();
	void SetPieValue();
private:
	CCtrlData* m_CtrlData;
};

#endif // PIEVIEW_H
