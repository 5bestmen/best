/*! @file dync_sys_op_event.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_sys_op_event.h
文件实现功能 :  系统类的命令事件定义
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
*  \brief 系统操作事件
*  \details  系统操作有：退出运行系统，运行程序，播放声音，蜂鸣，ReBoot
*/
class CDyncSysOpEventItem : public CBaseDyncEventItem
{
public:
	CDyncSysOpEventItem();
	virtual ~CDyncSysOpEventItem();
public:
	enum EVT_OP_TYPE
	{
		EVT_EXIT_RUNTIME = 0,  //!< 关闭
		EVT_RUN_PROGRAM,
		EVT_PLAY_SOUND,
		EVT_BEEP,
		EVT_REBOOT_OS,
	};
public:
	virtual std::string GetEventDescript() const;
private:
	//! 运行的程序的路径名及程序名称，此处用相对路径
	std::string  m_szProgramName;
	//! 运行参数
	std::string m_szRunParam;
	//! 语音文件名，包含后缀名 语音文件全部放在项目目录下面的sound下
	std::string m_szSoundFileName;

};
#endif // DYNC_SYS_OP_EVENT_H


/** @}*/
