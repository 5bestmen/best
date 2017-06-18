/*! @file dync_visible.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_visible.h
文件实现功能 :  动态可见定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   动态可见定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/
#ifndef DYNC_VISIBLE_H
#define DYNC_VISIBLE_H

#include "base_dync.h"
#include <qString>
#include <array>
#include <memory>
  
class CBaseWidget;  

 
/*! \class CDyncVisibleData  dync_visible.h  "dync_visible.h"
*  \brief 动态可见
*  \details  支持动态闪烁（无闪烁，慢速，中速，快速闪烁）
*/
class CDyncVisibleData :public CBaseDyncData
{
public:
	CDyncVisibleData();
	const CDyncVisibleData& operator=(const CDyncVisibleData& src);
	//复制数据
	virtual void CopyBaseData(CBaseDyncData *src);

	virtual ~CDyncVisibleData();
public:
	enum BLINKING_TYPE
	{
		BLINKING_NONE = 0,
		BLINKING_FAST,
		BLINKING_NORMAL,
		BLINKING_SLOW,
		BLINKING_CUSTOM  //!< 自定义周期
	};
	enum CMP_CONDITION
	{
		CONDITION_EQ = 0, //!< 等于
		CONDITION_NE,    //!< 不等于
		CONDITION_GE,    //!< 大于等于
		CONDITION_GT,    //!< 大于
		CONDITION_LE,    //!< 小于等于
		CONDITION_LT,    //!< 小于
	};
	void SetVisableValue(double nValue)
	{
		m_dValue = nValue;
	}

	double GetVisableValue() const
	{
		return m_dValue;
	}

	void SetDeadBand(double nBand)
	{
		m_dDeadBand = nBand;
	}

	double GetDeadBand() const
	{
		return m_dDeadBand;
	}

	void SetCondition(unsigned int nCond)
	{
		m_nCondition = nCond;
	}

	unsigned int GetCondition() const
	{
		return m_nCondition;
	}

	void SetFlashType(unsigned int nFlashType)
	{
		m_nBlinking = nFlashType;
	}

	unsigned int GetFlashType() const
	{
		return m_nBlinking;
	}

	void SetFlashTime(unsigned int nFlashTime)
	{
		m_nBlinkInterval = nFlashTime;
	}

	unsigned int GetFlashTime() const
	{
		return m_nBlinkInterval;
	}
private:
	//! 用于比较计算
	double  m_dValue;
	//! 死区范围   只有等于/不等于 条件才有效
	double  m_dDeadBand;
	//! 条件: > >= == <= <
	unsigned int m_nCondition;
	//! 闪烁类型 条件成立时闪烁
	unsigned int m_nBlinking;
	//! 自定义闪烁类型的闪烁周期
	unsigned int m_nBlinkInterval;
public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// 复制克隆  申请空间并复制自己
	virtual CBaseDyncData *CloneDyncData();
private:
	bool SetVisible(bool bFlag, CBaseWidget *pWidget);
	bool SetBlinking(bool bFlag, CBaseWidget *pWidget);
	void PluseWidget(CBaseWidget *pWidget);
	bool HasBlinking();
	//! 闪烁的间隔
	static const unsigned int BLINKING_NORMAL_CNT = 2;
	//! 闪烁的间隔
	static const unsigned int BLINKING_SLOW_CNT = 4;
};

#endif // DYNC_VISIBLE_H


/** @}*/
