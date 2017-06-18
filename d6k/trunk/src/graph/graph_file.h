/*! @file graph_file.h
<PRE>
********************************************************************************
模块名       :
文件名       :  graph_file.h
文件实现功能 :  图形文件
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     图形文件
*  @author    LiJin
*  @version   1.0
*  @date      2014.09.11
*******************************************************************************/
#ifndef D_GRAPH_FILE_H
#define D_GRAPH_FILE_H
 
//#include "base_widget.h"
#include "graphics_layer.h"
#include "graphbasedata.h"

#include <QSize>
#include <vector>
#include <string>
#include <memory>
#include <array> 
#include <QPainter>
#include <QObject>

class CBaseWidget;
class CGraphFolder;
class CGraphicsLayer;

/*! \class CGraphFileVerInfo  graph_file.h  "graph_file.h"
*  \brief 文件的版本信息.
*/
class CGraphFileVerInfo 
{
public:
	CGraphFileVerInfo();
	~CGraphFileVerInfo();
public:
	bool SaveFile(QXmlStreamWriter *pWriter) const;

	unsigned int GetMajorVer()const
	{
		return m_nMajorVer;
	}
	unsigned int GetMinorVer()const
	{
		return m_nMinorVer;
	}
	unsigned int GetBuild()const
	{
		return m_nBuild;
	}

	void SetMajorVer(unsigned int nVer)
	{
		m_nMajorVer = nVer;
	}

	void SetMinorVer(unsigned int nVer)
	{
		m_nMinorVer = nVer;
	}

	void SetBuild(unsigned int nVer)
	{
		m_nBuild = nVer;
	}

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
	std::string   m_szCreaterName; //! 文档的创建者
	std::string   m_szEditorName;  //! 最后保存的用户名
};
 

class CBackground;
class CGraphScene;
/*! \class CGraphFile  graph_file.h  "graph_file.h"
*  \brief 图形文件.
*/
class CGraphFile :public QObject, public CGraphBaseData
{
	Q_OBJECT 
public:
	CGraphFile();
	CGraphFile(CGraphScene *pScene);
	CGraphFile(CGraphFolder *pFolder);

	~CGraphFile();
    //重写赋值函数  
	const CGraphFile& operator=(const CGraphFile& src);

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
	//
	void SetGraphcsSize(QSize &sizeGraph);
	QRectF GetGraphRect()const;
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

	size_t GetLayerCount()
	{
	//	return MAX_LAYER_CNT;
		return m_arrLayers.size();
	}

	std::shared_ptr<CGraphicsLayer> GetLayer(unsigned int nIdx);

	std::shared_ptr<CGraphicsLayer> GetLayer(unsigned int idx) const;
//	CGraphicsLayer* GetLayer(unsigned int nIdx);
	bool IsLayerVisible(unsigned int nLayerIdx);

	void SetLayerVisible(unsigned int nLayerIdx, bool bDisp, unsigned int & nCurEditLayer);

	std::vector <CBaseWidget *>& GetSelectedWidgets(unsigned int nLayIdx);

	void SelectMultiWidgets(unsigned int nLayerIdx,CBaseWidget *pWidget);

	bool SelectAll(unsigned int nLayerIdx);
	bool SelectNone(unsigned int nLayerIdx);
	bool SelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void SelectWidgetsInRect(unsigned int nLayerIdx, const QRectF &rcSel);
	bool ReSelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void Deselect(unsigned int nLayerIdx, CBaseWidget* pObj);
	
	CBaseWidget *GetCurrentWidget(unsigned int nLayerIdx);
	void SetCurrentWidget(unsigned int nLayerIdx, CBaseWidget*pWidget);

	std::string GetGraphName()const
	{
		return m_szGraphName;
	}
	void SetGraphName(const std::string & szName)
	{
		m_szGraphName = szName;
	}
	CGraphFolder *GetFolder()
	{
		return m_pFolder;
	}
	void SetScene(CGraphScene *pScene)
	{
		m_pScene = pScene;
		for (auto i:m_arrLayers)
		{
			i->SetScene(pScene);
		}
	}
	//清空数据
	void ClearLayer();

	CGraphScene * GetScene()
	{
		return m_pScene;
	}

	//保存图形文件
	bool SaveFile(const QString &szFilePath) ;
	//加载图形文件
	bool LoadFile(const QString &szFileName);

     CGraphFileVerInfo *GetFileVerInfo() 
	{
		return &m_FileInfo;
	}
// 	const CGraphFileVerInfo& GetFileVerInfo() const
// 	{
// 		return m_FileInfo;
// 	}

	std::pair<int, int> GetSize()
	{
		return std::make_pair(m_Size.height(), m_Size.width());
	}

	
	std::string & GetDescript()
	{
		return m_szDescript;
	}

	void SetFlashFlag(bool bFlag)
	{
		m_bFlashFlag = bFlag;
	}

	bool GetFlashFlag()
	{
		return m_bFlashFlag;
	}

	std::shared_ptr<CBackground> GetBackground()
	{
		return m_pBackground;
	}
	QRectF GetNewRect(const QRectF& rect1,const QRectF& rect2);

protected Q_SLOTS:
	void slot_on_updateScene(CBaseWidget*,const QRectF& rect, const QRectF& rect1);
public:
	//! 画面名称 不允许重复，文件名不得超过 字节长度
	std::string m_szGraphName;
	//! 描述
	std::string m_szDescript;
	//! 图层
	std::array<std::shared_ptr<CGraphicsLayer>, MAX_LAYER_CNT> m_arrLayers;
//	CGraphicsLayer* m_arrLayers[MAX_LAYER_CNT];
	//图元下移
	void DownItemLayer(unsigned int nLayer);
	//图元上移
	void UploadItemLayer(unsigned int nLayer);
	//有效标识
	bool m_bVaildFlag;

	//设置缩放比例
	void SetScaled(int nScale)
	{
		m_scaleFactor = nScale;
	}

	//获取缩放比例
	int GetSacled()
	{
		return m_scaleFactor;
	}

	//命名
	void RenameWidgetItem(unsigned int nLayerIdx, CBaseWidget *pObj);

private:
	//! 文件的版本信息
	CGraphFileVerInfo m_FileInfo;
	//! 图元的最大编号
	unsigned int m_nMaxWidgetID;
	//! 所属目录，如果为空则是root下面的
	CGraphFolder *m_pFolder;
	//! 图像文件的尺寸，每个图形的尺寸可以不一样
	QSize  m_Size;

	//! 刷新周期 毫秒为单位
	unsigned int m_nPeriod;

	std::shared_ptr<CBackground> m_pBackground;

	CGraphScene *m_pScene;
	//! 当前正在编辑的图层编号
//	unsigned int m_nCurEditingLayer;
//	std::vector <CBaseWidget*> m_arrWidgets;
	//是否刷新标志
	bool m_bFlashFlag;
	//缩放比例
	int m_scaleFactor;

};

#endif // D_GRAPH_FILE_H

/** @}*/

