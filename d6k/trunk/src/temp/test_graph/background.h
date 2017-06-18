/*! @file background.h
<PRE>
********************************************************************************
模块名       :
文件名       : background.h
文件实现功能  : 背景
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  背景
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
	//! 画布尺寸
	QRectF m_rcBackground;

	//! 显示栅格
	bool m_bShowGrid;
	//! 显示栅格点
	bool m_bShowSnapGridGap;
	//! 栅格点间隔
	unsigned int m_nSnapGridGap;

	//! 坐标
	bool m_bShowCoord;
	// 背景纹理


	//! 栅格点
	std::vector<QPointF> m_arrSnapPoints;

	// 栅格线
	std::vector<QLine> m_arrGridLines;

	//! 背景颜色
	QColor  m_clrBackground;
};
 
#endif // D_BACKGROUND_H

/** @}*/

