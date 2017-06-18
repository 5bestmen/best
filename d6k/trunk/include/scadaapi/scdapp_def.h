/*! @file scdapp_def.h
<PRE>
********************************************************************************
模块名       :
文件名       :  scdapp_def.h
文件实现功能 :  SCADA 应用的定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     SCADA 应用的定义
*  @author    LiJin
*  @version   1.0
*  @date      2017.03.24
*******************************************************************************/
#ifndef SCD_APP_DEF_H
#define SCD_APP_DEF_H

#include "../datatypes.h"
#include "../variant.h"
#include "../public.h" 
#include "../fesapi/fescnst.h"
 
 

#pragma pack (1)
enum AppType 
{
	Inn_APP=0,    //内置应用
	Ext_App=1,    //扩展服务
};

enum LoadType
{
	Load_Dll=0,   //dll启动
	Load_Exe=1,   //exe启动
};
 
/*! \struct  SAPP
*   \brief  SCADA 应用 */
struct SAPP
{
	int32u  OccNo;                //! 关键字 唯一 Id 从1开始
	int32u  NameOccNo;            //! 只读字符串池内的ID序号

	int8u   IsDefined;           /* YES : is defined, NO: spare */
	int8u   ScanEnable;          /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u   Init;                /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u   Quality;             /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	
	int8u   Heartbeat;          /* 心跳 */
	int8u   PrintOn;            //调试开关：1：输出记录，0：不输出 

	int32u State;
	int8u  Prority;
	LoadType nLoadType;    //! 调用类型：dll形式 ，exe形式
	AppType  nAppType;	   //! 应用类型：内置应用，扩展服务
	
//	char    ChannelName[MAX_NAME_LENGTH + STRING_PAD]; //通道名称
	char    ProgramName[MAX_NAME_LENGTH + STRING_PAD]; //！ 应用程序名称
	 
	int32s  MailBoxID;  //! 邮箱ID
};

#pragma pack ()

#endif // SCD_APP_DEF_H


/** @}*/

