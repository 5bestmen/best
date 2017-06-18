/*! @file background.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : background.h
�ļ�ʵ�ֹ���  : ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ����
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef D_BACKGROUND_H
#define D_BACKGROUND_H

#include "graph_global.h"

#include <vector>

#include <QPainter>

class QPainter;
class CBackground
{
public:
	CBackground();
	~CBackground();
public:
	void DrawBackground(QPainter *painter, const QRectF &rect);
	QRectF &GetBackgroundRect() ;

	void SetSize(QSize sizeGraph);
private:
	//! �����ߴ�
	QRectF m_rcBackground;

	//! ��ʾդ��
	bool m_bShowGrid;
	//! ��ʾդ���
	bool m_bShowSnapGridGap;
	//! դ�����
	unsigned int m_nSnapGridGap;

	//! ����
	bool m_bShowCoord;
	// ��������


	//! դ���
	std::vector<QPointF> m_arrSnapPoints;

	// դ����
	std::vector<QLine> m_arrGridLines;

	//! ������ɫ
	QColor  m_clrBackground;
};
 
#endif // D_BACKGROUND_H

/** @}*/

