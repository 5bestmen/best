/*! @file graph_file.h
<PRE>
********************************************************************************
模块名       :
文件名       : graph_file.h
文件实现功能 : 图形文件
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  基本定义
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef D_GRAPH_FILE_H
#define D_GRAPH_FILE_H
 
//#include "base_widget.h"

#include "QSize"
#include <vector>
#include <string>
#include <memory>
#include <array> 

#include <QPainter>

#include "graphics_layer.h"

class CBaseWidget;
class CGraphFolder;
class CGraphicsLayer;


/*! \class CGraphFileVerInfo  graph_file.h  "graph_file.h"
*  \brief 文件的版本信息.
*/
class CGraphFileVerInfo
{
public:
	CGraphFileVerInfo(){};
	~CGraphFileVerInfo(){};
private:
	unsigned int  m_nMajorVer; //主版本号  如：1.00 :wMajorVer = 1 wMinorVer= 0，2.01 = wMajorVer = 2 wMinorVer= 1 主版本号和应用程序有关 
	unsigned int  m_nMinorVer; //从版本号，
	unsigned int  m_nBuild;    //由文件保存时产生，每保存一次，版本号增加1	

	unsigned int  m_nYear;
	unsigned int  m_nMonth;
	unsigned int  m_nDay;
	unsigned int  m_nHour;
	unsigned int  m_nMinute;
	unsigned int  m_nSecond;
	unsigned int  m_nMilliseconds;
	std::string   m_szEditorName;  //! 最后保存的用户名
};
 

class CBackground;
class CGraphScene;
/*! \class CGraphFile  graph_file.h  "graph_file.h"
*  \brief 图形文件.
*/
class CGraphFile
{
public:
	CGraphFile();
	CGraphFile(CGraphScene *pScene);
	CGraphFile(CGraphFolder *pFolder);

	~CGraphFile();
public:
	enum LayerNumber
	{
		MAX_LAYER_CNT = 16  //!< 最多图层数量，图层的顺序号从0开始
	};
	void SetPeriod(unsigned int nPeriod)
	{
		if (nPeriod < 200)
		{
			nPeriod = 200;
		}
		m_nPeriod = nPeriod;
	}

	void SetGraphSize(QSize &sizeGraph);
	// 添加一个对象
	void AddWidget(unsigned int nLayerIdx, CBaseWidget *pObj);
	// 在图层中删除一个指定对象，并回收资源
	bool EraseWidget(unsigned int nLayerIdx, CBaseWidget *pObj);
	// 在图层中删除一个指定对象，不回收资源
	bool RemoveWidget(unsigned int nLayerIdx, CBaseWidget *pObj);
	// 获取画布的背景尺寸
	QRectF& GetBackgroundRect() const;
	// 绘制背景
	void DrawBackground(QPainter *painter, const QRectF &rect);
	
	void DrawMovingWidgets(QPainter *pPainter,unsigned int nEditingLayerIdx);
	//
	bool IsWidgetsCanMove(const QPointF & delta, const std::vector <CBaseWidget *>& arr) const;

	unsigned int GetLayerCount()
	{
	//	return MAX_LAYER_CNT;
		return m_arrLayers.size();
	}

	std::shared_ptr<CGraphicsLayer> GetLayer(unsigned int nIdx);
//	CGraphicsLayer* GetLayer(unsigned int nIdx);
	bool IsLayerVisible(unsigned int nLayerIdx);

	void SetLayerVisible(unsigned int nLayerIdx, bool bDisp, unsigned int & nCurEditLayer);

	std::vector <CBaseWidget *> GetSelectedWidgets(unsigned int nLayIdx);

	void SelectMultiWidgets(unsigned int nLayerIdx,CBaseWidget *pWidget);

	bool SelectAll(unsigned int nLayerIdx);
	bool SelectNone(unsigned int nLayerIdx);
	bool SelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void SelectWidgetsInRect(unsigned int nLayerIdx, const QRectF &rcSel);
	bool ReSelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void Deselect(unsigned int nLayerIdx, CBaseWidget* pObj);


	CBaseWidget *GetCurrentWidget(unsigned int nLayerIdx);
	void SetCurrentWidget(unsigned int nLayerIdx, CBaseWidget*pWidget);
public:
	// 画面名称 不允许重复，文件名不得超过 字节长度
	std::string m_szGraphName;
	// 描述
	std::string m_szDescript;
	//! 图层
	std::array<std::shared_ptr<CGraphicsLayer>, MAX_LAYER_CNT> m_arrLayers;
//	CGraphicsLayer* m_arrLayers[MAX_LAYER_CNT];
private:
	//! 图像文件的尺寸，每个图形的尺寸可以不一样
	QSize  m_Size;
	//! 文件的版本信息
	CGraphFileVerInfo m_FileInfo;
	//! 图元的最大编号
	unsigned int m_nMaxWidgetID;
	//! 所属目录，如果为空则是root下面的
	CGraphFolder *m_pFolder;

	//! 刷新周期 毫秒为单位
	unsigned int m_nPeriod;

	std::shared_ptr<CBackground> m_pBackground;

	CGraphScene *m_pScene;
	//! 当前正在编辑的图层编号
//	unsigned int m_nCurEditingLayer;
//	std::vector <CBaseWidget*> m_arrWidgets;
};

#endif // D_GRAPH_FILE_H

/** @}*/

