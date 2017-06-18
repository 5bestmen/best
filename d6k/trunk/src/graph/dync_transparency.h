/*! @file dync_transparency.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_transparency.h
文件实现功能 :  动态透明定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   动态透明定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/
#ifndef DYNC_TRANSPARENCY_H
#define DYNC_TRANSPARENCY_H

#include "base_dync.h"
#include <qString>
#include <array>
#include <memory>
  
class CBaseWidget;  

/*! \class CDyncTransparencyData DynamicDef.h  "DynamicDef.h"
*  \brief 动态透明
*  \details   有些元件动态透明没有效果
*/
class CDyncTransparencyData :public CBaseDyncData
{
public:
	CDyncTransparencyData();
	const CDyncTransparencyData& operator=(const CDyncTransparencyData& src);
	//复制数据
	virtual void CopyBaseData(CBaseDyncData *src);

	virtual ~CDyncTransparencyData();
public:
	double GetMaxValue() const
	{
		return m_dMaxValue;
	}

	void SetMaxValue(double dMax)
	{
		m_dMaxValue = dMax;
	}

	double GetMinValue() const
	{
		return m_dMinValue;
	}

	void SetMinValue(double dMin)
	{
		m_dMinValue = dMin;
	}

private:
	//! 255对应的值 完全可见
	double  m_dMaxValue;
	//! 0 对应的值 完全不可见
	double  m_dMinValue;
public:
 	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// 复制克隆  申请空间并复制自己
	virtual CBaseDyncData *CloneDyncData();

	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
};

#endif // DYNC_TRANSPARENCY_H


/** @}*/
