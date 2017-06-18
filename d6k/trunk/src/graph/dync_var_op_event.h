/*! @file base_dync.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_dync_event.h
�ļ�ʵ�ֹ��� :  �����������¼�����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����������¼�����
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
*  \brief ���������¼�
*  \details  ���������У�
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
