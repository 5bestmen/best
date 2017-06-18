/*! @file dync_sys_op_event.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_sys_op_event.h
�ļ�ʵ�ֹ��� :  ϵͳ��������¼�����
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
#ifndef DYNC_SYS_OP_EVENT_H
#define DYNC_SYS_OP_EVENT_H
 
#include <string>
#include <memory>
#include <vector>

#include "dync_event.h"
#include "base_dync_event_item.h"

class QXmlStreamWriter;
class QXmlStreamReader;

/*! \class CDyncSysOpEventItem dync_sys_op_event.h  "dync_sys_op_event.h"
*  \brief ϵͳ�����¼�
*  \details  ϵͳ�����У��˳�����ϵͳ�����г��򣬲���������������ReBoot
*/
class CDyncSysOpEventItem : public CBaseDyncEventItem
{
public:
	CDyncSysOpEventItem();
	virtual ~CDyncSysOpEventItem();
public:
	enum EVT_OP_TYPE
	{
		EVT_EXIT_RUNTIME = 0,  //!< �ر�
		EVT_RUN_PROGRAM,
		EVT_PLAY_SOUND,
		EVT_BEEP,
		EVT_REBOOT_OS,
	};
public:
	virtual std::string GetEventDescript() const;
private:
	//! ���еĳ����·�������������ƣ��˴������·��
	std::string  m_szProgramName;
	//! ���в���
	std::string m_szRunParam;
	//! �����ļ�����������׺�� �����ļ�ȫ��������ĿĿ¼�����sound��
	std::string m_szSoundFileName;

};
#endif // DYNC_SYS_OP_EVENT_H


/** @}*/
