/*! @file graph_serializer.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graph_serializer.cpp
�ļ�ʵ�ֹ��� : ͼ���ļ����л�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ���ļ����л�����
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

