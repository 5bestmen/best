/*! @file dync_value.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_value.h
文件实现功能 :  动态数值定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   动态数值定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.19
*******************************************************************************/
#ifndef DYNC_VALUE_DATA_H
#define DYNC_VALUE_DATA_H

#include "base_dync.h"
#include <qString>
#include <array>
#include <memory>
  
class CBaseWidget;  

/*! \class CDyncValueData dync_value.h  "dync_value.h"
*  \brief 动态数值变化
*  \details
*/
class CDyncValueData :public CBaseDyncData
{
public:
	CDyncValueData();
	const CDyncValueData& operator=(const CDyncValueData& src);
	//复制数据
	virtual void CopyBaseData(CBaseDyncData *src);

	virtual ~CDyncValueData();
public:
	enum VAL_FORMAT
	{
		X = 0,
		XX,
		XXX,
		XXXX,
		XXXXX,
		X_X, //!< 一位小数
		X_XX,
		X_XXX,
		X_XXXX,
		X_XXXXX
	};
public:
	void SetFormat(VAL_FORMAT eFormat)
	{
		m_nFormat = eFormat;
	}

	int GetFormat() const
	{
		return m_nFormat;
	}
private:
	//! 显示的格式
	VAL_FORMAT m_nFormat;
public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);

	// 动态部分的处理
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// 复制克隆  申请空间并复制自己
	virtual CBaseDyncData *CloneDyncData();
};

 

#endif // DYNC_VALUE_DATA_H


/** @}*/
