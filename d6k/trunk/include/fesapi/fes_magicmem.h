/*! @file sharemem.h
<PRE>
********************************************************************************
模块名       :
文件名       :  sharemem.h
文件实现功能 :  共享内存
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     共享内存
*  @author    LiJin
*  @version   1.0
*  @date      2016.04.24
*******************************************************************************/
#ifndef FES_MAGIC_MEM_H
#define FES_MAGIC_MEM_H

#include "../datatypes.h"
#include "fesdatadef.h"

#define MEM_CONFIG_LEN  1024
#define MAGIC_HEAD 0X4C49


struct HEAD_STRING
{
	int32u MagicHead1;
	int32u MagicHead2;
	int32u ShmLength;
	int TotalCount;

};

/*! \struct  MAGIC_MEM
*   \brief  共享内存头 */
struct HEAD_MEM
{
	int32u  MagicHead1;
	int32u  MagicHead2;

	int32u  ShmLength;     //! 共享内存的长度
	int32u  MyNodeOccNo;   //! 本节点的排行号

	char    Config[MEM_CONFIG_LEN];   //! 配置文件路径

	int32u  NodeTableOffset;
	int32u  NodeCount;

	int32u  ChannelTableOffset;
	int32u  ChannelCount;

	int32u  DeviceTableOffset;
	int32u  DeviceCount;

	int32u  AinTableOffset;
	int32u  AinCount;

	int32u  DinTableOffset;
	int32u  DinCount;

	int32u  AoutTableOffset;
	int32u  AoutCount;

	int32u  DountTableOffset;
	int32u  DoutCount;

	int32u  LinearTableOffset;
	int32u  LinearCount;

	int32u  NonLinearTableOffset;
	int32u  NonLinearCount;

	// 告警相关
	int32u  AinAlarmTableOffset;
	int32u  AinAlarmCount;

	int32u AinAlarmLimitTableOffset;
	int32u AinAlarmLimitCount;

	int32u DinAlarmTableOffset;
	int32u DinAlarmCount;
	
	int32u DinAlarmLimtTableOffset;
	int32u DinAlarmLimitCount;

	//变量信息
	int32u SystemVariableOffSet;
	int32u SystemVariableCount;

	int32u UserVariableOffset;
	int32u UserVariableCount;

	//变长设备信息
	int32u VarDevInfoOffset;

};

struct NODE_REF
{
	int32u  MagicHead;
	int32u  IddType;
	int32u  NodeOccNo;

	int32u  ChannelCount;
	int32u  ExtraLen;
}; 

struct CHANNEL_REF
{
	int32u  MagicHead;
	int32u  IddType; 
	int32u  ChannelOccNo;

	int32u  DeviceCount;
	
	int32u  AinCount;		
	int32u  DinCount;
	int32u  AoutCount;
	int32u  DoutCount;

	int32u  ExtraLen;
};

struct DEVICE_REF
{
	int32u  MagicHead;
	int32u  IddType;
	int32u  DeviceOccNo;
	
	int32u  AinCount;
	int32u  DinCount;
	int32u  AoutCount;
	int32u  DoutCount;
	
	int ExtraLen;
};

struct TAG_OCCNO
{
	char TagName[MAX_NAME_LENGTH + STRING_PAD];

	int32u OccNo;
//	int32u TagType;
};
 

#endif // FES_MAGIC_MEM_H


/** @}*/

