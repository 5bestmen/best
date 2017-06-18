/*! @file sharemem.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sharemem.h
�ļ�ʵ�ֹ��� :  �����ڴ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     �����ڴ�
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
*   \brief  �����ڴ�ͷ */
struct HEAD_MEM
{
	int32u  MagicHead1;
	int32u  MagicHead2;

	int32u  ShmLength;     //! �����ڴ�ĳ���
	int32u  MyNodeOccNo;   //! ���ڵ�����к�

	char    Config[MEM_CONFIG_LEN];   //! �����ļ�·��

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

	// �澯���
	int32u  AinAlarmTableOffset;
	int32u  AinAlarmCount;

	int32u AinAlarmLimitTableOffset;
	int32u AinAlarmLimitCount;

	int32u DinAlarmTableOffset;
	int32u DinAlarmCount;
	
	int32u DinAlarmLimtTableOffset;
	int32u DinAlarmLimitCount;

	//������Ϣ
	int32u SystemVariableOffSet;
	int32u SystemVariableCount;

	int32u UserVariableOffset;
	int32u UserVariableCount;

	//�䳤�豸��Ϣ
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

