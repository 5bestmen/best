/*! @file dync_event.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_event.h
文件实现功能 :  命令事件定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   命令事件定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef DYNC_EVENT_H
#define DYNC_EVENT_H
 
#include <string>
#include <memory>
#include <vector>

#include "base_dync.h"

class QXmlStreamWriter;
class QXmlStreamReader;

struct TAGITEM;

class CVariant;

 
class CBaseWidget;
class CBaseDyncEventItem; 

class CEventIntent
{
public:
	CEventIntent();
	virtual ~CEventIntent();
public:
	//! 打开新图形
	//std::function< bool(const std::string &)> m_fnOpenNewGraph;


};

/*! \class CDyncEventData dync_event.h  "dync_event.h"
*  \brief 动态命令事件
*  \details
*/
class CDyncEventData :public CBaseDyncData
{
public:
	CDyncEventData();
	const CDyncEventData& operator=(const CDyncEventData& src);
	bool operator==(const CDyncEventData& src) const;
	virtual ~CDyncEventData();
public:
	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
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
	// 处理动态事件
	virtual bool ProcessAction(CBaseWidget *pWidget, CEventIntent & intent);

	// 删除事件
	void DeleteOpEvent(CBaseDyncEventItem *pEvent);

	void DeleteEvent(CBaseDyncEventItem *pEvent);
	// 检查事件，删除已经被取消掉的事件
	void TrimData();

	//创建事件
	CBaseDyncEventItem* CreateEventItem(int nEventType);

protected:
	//! 每个网格可能存在多种动态连接
	std::vector<CBaseDyncEventItem*> m_arrEvents; 

};
#endif // BASE_DYNC_EVENT_H


/** @}*/
