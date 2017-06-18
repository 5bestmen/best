/*! @file base_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  base_widget.cpp
文件实现功能 :  基本的动态定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的动态定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/

#include "dync_sys_op_event.h"

#include "fesapi/fesdatadef.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif

#include "../scdpub/variant_data.h"
 


CDyncSysOpEventItem::CDyncSysOpEventItem()
{

}

CDyncSysOpEventItem::~CDyncSysOpEventItem()
{

}

std::string CDyncSysOpEventItem::GetEventDescript() const
{
	return ("");
}




/** @}*/
