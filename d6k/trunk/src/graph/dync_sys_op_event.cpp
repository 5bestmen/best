/*! @file base_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_widget.cpp
�ļ�ʵ�ֹ��� :  �����Ķ�̬����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����Ķ�̬����
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
