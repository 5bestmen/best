/*! @file base_dync.h
<PRE>
********************************************************************************
模块名       :
文件名       :  base_dync_event.h
文件实现功能 :  基本的命令事件定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的命令事件定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef DYNC_VAR_OP_EVENT_H
#define DYNC_VAR_OP_EVENT_H
 
#include <string>
#include <memory>
#include <vector>

#include "base_dync_event_item.h"

class QXmlStreamWriter;
class QXmlStreamReader;

/*! \class CDyncVarOpEventItem DynamicDef.h  "DynamicDef.h"
*  \brief 变量操作事件
*  \details  变量操作有：
*/
class  CDyncVarOpEventItem : public CBaseDyncEventItem
{
public:
	CDyncVarOpEventItem();
	virtual ~CDyncVarOpEventItem();
public:
	virtual std::string GetEventDescript() const;
};
#endif // BASE_DYNC_EVENT_H


/** @}*/
