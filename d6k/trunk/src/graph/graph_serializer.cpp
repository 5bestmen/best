/*! @file graph_serializer.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : graph_serializer.cpp
文件实现功能 : 图形文件序列化基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图形文件序列化基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.28
*******************************************************************************/

#include "graph_file.h"
#include "graph_serializer.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#else
#include "graph_module.h"
#endif

void CGraphSerializer::LogMsg(const char *szLogTxt, int nLevel)
{
#ifdef HMI_SDK_LIB
	::LogMsg("VMCS", szLogTxt, nLevel, nullptr);
#else
	GetModuleApi()->LogMsg(szLogTxt, nLevel);
#endif
}



/** @}*/

