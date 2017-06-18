/*! @file dync_color.h
<PRE>
********************************************************************************
模块名       :
文件名       : dync_color.h
文件实现功能 : 动态颜色定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  动态颜色定义
*  @author LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/
#ifndef DYNC_COLOUR_DATA_H
#define DYNC_COLOUR_DATA_H

#include "base_dync.h"
#include <qString>
#include <QColor>
#include <array>
#include <memory>
  
class CBaseWidget;  


/*! \class CDyncClrValueInfo dync_color.h  "dync_color.h"
*  \brief 动态颜色参数
*  \details
*/
class CDyncClrValueInfo
{
public:
	CDyncClrValueInfo();
	CDyncClrValueInfo(const CDyncClrValueInfo  *  pSrc);
	CDyncClrValueInfo(int nIdx);
	~CDyncClrValueInfo();
	const CDyncClrValueInfo& operator=(const CDyncClrValueInfo& src);
	enum BLINKING_TYPE
	{
		BLINKING_NONE = 0,  //!< 无闪烁
		BLINKING_INVISIBLE, //!< 不可见，即透明
		BLINKING_FAST,
		BLINKING_NORMAL,
		BLINKING_SLOW,
		BLINKING_CUSTOM  //!< 自定义周期
	};
	QString GetBlinkingTypeTxt();
	static BLINKING_TYPE GetBlinkingType(QString &szTxt);

	static const QString SZ_BINKING[BLINKING_CUSTOM + 1];

	static const  QString SZ_BLINKING_NONE;
	static const  QString SZ_BLINKING_INVISIBLE;
	static const  QString SZ_BLINKING_FAST;
	static const  QString SZ_BLINKING_NORMAL;
	static const  QString SZ_BLINKING_SLOW;
	static const  QString SZ_BLINKING_CUSTOM;
	//! 预定义颜色数量
	//static const unsigned int ColorTabNum = 8;
	enum { ColorTabNum = 8 };

	using COLOR_ARRAY = std::array < QColor, ColorTabNum>;
	//! 预定义颜色表
	static COLOR_ARRAY m_crTable;
public:
	//! 显示在界面上面的值
	std::string m_szValue;
	//! 用于比较计算
	double  m_dValue;
	//! 显示的颜色
	QColor m_dwColor;
	//! 闪烁的颜色
	QColor m_dwBlinkingColor;
	//! 闪烁类型
	BLINKING_TYPE m_nBlinkingType;
	//! 自定义闪烁的间隔
	unsigned int m_nBlinkInterval;
	//! 顺序号 从1开始
	unsigned int m_nIndex;
	//! 快速闪烁时的标志位，运行时用
	bool m_bBlinkingFlag;
	//! 上一次执行时的时间
	//boost::posix_time::ptime  m_tmLast;
	//! 闪烁的间隔  单位毫秒
	enum { BLINKING_NORMAL_INTERVAL_MS = 1000 };
	//! 闪烁的间隔
	enum{ BLINKING_SLOW_INTERVAL_MS = 2000 };
};
/*! \class CDyncClrData dync_color.h  "dync_color.h"
*  \brief 动态颜色变化
*  \details 包含有：线条，填充，文本的颜色动态变化
*/
class CDyncClrData :public CBaseDyncData
{
public:
	CDyncClrData();
	~CDyncClrData();
public:
	enum DYNC_CLR_TYPE
	{
		DYNC_LINE_CLR = 0,    //!< 线条颜色
		DYNC_FILL_CLR,        //!< 填充颜色
		DYNC_TEXT_CLR,        //!< 文本颜色
		DYNC_OTHER_CLR,
	};

	enum { MAX_CLR_CNT = 64 };
public:
	//! 颜色类型： 有线条，填充，文本
	DYNC_CLR_TYPE m_nDyncClrType;
	//! 颜色列表
	std::vector<CDyncClrValueInfo*> m_arrColors;
public:
	// 比较两个动态连接是否是一样的类型
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;

	CDyncClrValueInfo * CreateClrData();

	void DeleteClrData(CDyncClrValueInfo *pData);
	// 检查是否合法
	virtual bool CheckDyncData();
	// 预备动态
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// 复制克隆  申请空间并复制自己
	virtual CBaseDyncData *CloneDyncData();
protected:
	//bool GetColor(const UNION_VALUE &value, COLORREF &clrCur, bool &bInvisible) const;
	//! 原始的颜色 [画笔，画刷，字体]
	QColor		m_dwColor;
	//! 原始的透明度 
	unsigned int  m_nTransparency;
protected:
 
private:

};

/*! \class CDyncFillingData dync_color.h  "dync_color.h"
*  \brief 动态颜色变化
*  \details 包含有：线条，填充，文本的颜色动态变化
*/
class CDyncFillingData :public CDyncClrData
{
public:
	CDyncFillingData();
	const CDyncFillingData& operator=(const CDyncFillingData& src);
	virtual ~CDyncFillingData();

	enum DYNC_FILLING_POLICY
	{
		FILLING_B2T, //!< 自下到上
		FILLING_T2B, //!< 自上到下
		FILLING_L2R, //!< 自左到右
		FILLING_R2L  //!< 自右到左
	};

	static const QString SZ_FILLING_POLICY[FILLING_R2L + 1];
public:
	//! 填充策略
	DYNC_FILLING_POLICY m_nFillingPolicy;
	//! 最小值
	double  m_dMinVal;
	//! 最大值
	double  m_dMaxVal;
	//! 起点值 百分比 0-100 之间
	double  m_dStartPos;
	//! 终点值
	double  m_dEndPos;
public:
// 	virtual void SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const;
// 	virtual bool LoadXml(JWXml::CXmlNodePtr  pDyncPtr);

	// 比较两个动态连接是否是一样的类型
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;
	// 检查是否合法
	virtual bool CheckDyncData();
	// 预备动态
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// 复制克隆  申请空间并复制自己
	virtual CBaseDyncData *CloneDyncData();
};

#endif // BASE_DYNC_DATA_H


/** @}*/
