/*! @file tag_att.h
<PRE>
********************************************************************************
模块名       :
文件名       :  tag_att.h
文件实现功能 :  系统 基础对象 的属性管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     系统 基础对象 的属性管理
*  @author    LiJin
*  @version   1.0
*  @date      2016.10.14
*******************************************************************************/
#ifndef TAG_ATT_H
#define TAG_ATT_H

#include "datatypes.h"
#include "fesapi/fesdatadef.h"
#include <string>
#include <vector>
#include <array>


class CTagAttMgr
{
public:
	CTagAttMgr();
	~CTagAttMgr();

	enum
	{
		ATT_NO = 0,
		ATT_YES = 1,
		ATT_RW = 0,   //!< 可读写
		ATT_RO = 1,   //!< 只读
	};
private:
	enum NodeAttr
	{
		Node_Begin = 0,
		Node_OccNo = 1,
		Node_GrpNo,
		Node_NameOccNo,
		Node_SlaveOccNo,
		Node_IsDefined,
		Node_ScanEnable,
		Node_Init,
		Node_Quality,
		Node_RedState,
		Node_NodeType,
		Node_State,
		Node_Net1State,
		Node_Net2State,

		Node_PointCount,
		Node_Net1IPAddr,
		Node_Net2IPAddr,
		Node_NetA6VAddr,
		Node_NetB6VAddr,

		NODE_TagName,
		NODE_HostName,
		NODE_GroupName,

		//	Node_NodeName,
		//	Node_AliasName,
		Node_End,
	};
	enum ChannelAttr
	{
		Channel_Begin = 0,
		Channel_OccNo,
		Channel_NameOccNo,
		Channel_IsDefined,
		Channel_ScanEnable,
		Channel_Init,
		Channel_Quality,
		Channel_PeerQuality,
		Channel_Heartbeat,
		Channel_PrintOn,
		Channel_WorkNet,

		Channel_CommPara,

		Channel_State,
		Channel_Prority,
		Channel_SetTimeAttr,
		Channel_DeviceCount,
		Channel_NodeOccNo,
		Channel_ChannelName,
		Channel_DriverName,
		Channel_DriverType,
		Channel_StateCore,
		Channel_StateDriver,

		Channel_End
	};
	enum DeviceAttr
	{
		Device_Begin,
		Device_OccNo,
		Device_NameOccNo,
		Device_NodeOccNo,
		Device_ChannelOccNo,
		Device_ModelOccNo,
		Device_Type,

		Device_Pad1,
		Device_Pad2,

		Device_Heartbeat,
		Device_Address,
		Device_Extention,
		Device_IsDefined,
		Device_ScanEnable,
		Device_Init,
		Device_Quality,

		Device_End

	};
	enum AINAttr
	{
		AIN_Begin,
		AIN_OccNo,
		AIN_BlockNo,
		AIN_NameOccNo,
		AIN_IsSpare,
		AIN_Pad2,
		AIN_Address,
		AIN_State,
		AIN_IsDefined,
		AIN_ScanEnable,
		AIN_Init,
		AIN_Quality,
		AIN_ManSet,
		AIN_SignalValue,
		AIN_RawValue,
		AIN_Value,
		AIN_OldValue,
		AIN_InitValue,
		AIN_DataSource,
		AIN_RangeL,
		AIN_RangeH,
		AIN_HighQua,
		AIN_LowQua,
		AIN_MaxRaw,
		AIN_MinRaw,
		AIN_MaxScale,
		AIN_MinScale,
		AIN_NodeOccNo,
		AIN_ChannelOccNo,
		AIN_DeviceOccNo,
		AIN_AlarmOccNo,
		AIN_ExpressOccNo,
		AIN_TransformType,
		AIN_TransformOccNo,
		AIN_DescriptionOccNo,
		AIN_PinLabelOccNo,
		AIN_UnitNameOccNo,
		AIN_LastUpdateTime,

		AIN_End
	};
	enum DINAttr
	{
		DIN_Begin,
		DIN_OccNo,
		DIN_BlockNo,
		DIN_NameOccNo,
		DIN_NeedTranmitFlag,
		DIN_Pad2,
		DIN_IsSpare,
		DIN_Address,
		DIN_InitValEnable,
		DIN_InitValue,
		DIN_InitQua,
		DIN_SlaveOccNo,
		DIN_DDIFilter,
		DIN_DDIFlag,
		DIN_DDIInUse,
		DIN_Value,
		DIN_NegValue,
		DIN_RawValue,
		DIN_State,
		DIN_IsDefined,
		DIN_ScanEnable,
		DIN_Init,
		DIN_Quality,
		DIN_ManSet,
		DIN_CtrlByte,
		DIN_IsSOE,
		DIN_StartCtrl,
		DIN_SignalType,
		DIN_NodeOccNo,
		DIN_ChannelOccNo,
		DIN_DeviceOccNo,
		DIN_AlarmOccNo,
		DIN_DataSource,
		DIN_ExpressOccNo,
		DIN_Desc0_OccNo,
		DIN_Desc1_OccNo,
		DIN_PinLabelOccNo,
		DIN_LastUpdateTime,

		DIN_End

	};
	enum AOUTAttr
	{
		AOUT_Begin,
		AOUT_OccNo,
		AOUT_BlockNo,
		AOUT_NameOccNo,
		AOUT_State,
		AOUT_IsDefined,
		AOUT_ScanEnable,
		AOUT_Init,
		AOUT_Quality,
		AOUT_Output,
		AOUT_RawData,
		AOUT_NodeOccNo,
		AOUT_ChannelOccNo,
		AOUT_DeviceOccNo,
		AOUT_AlarmOccNo,
		AOUT_ExpressOccNo,
		AOUT_TransformType,
		AOUT_TransformOccNo,
		AOUT_PinLabelOccNo,
		AOUT_UnitNameOccNo,
		AOUT_LastUpdateTime,

		AOUT_End
	};
	enum DOUTAttr
	{
		DOUT_Begin,
		DOUT_OccNo,
		DOUT_BlockNo,
		DOUT_NameOccNo,
		DOUT_State,
		DOUT_IsDefined,
		DOUT_ScanEna,
		DOUT_Init,
		DOUT_Quality,
		DOUT_Output,
		DOUT_RelayState,
		DOUT_DataSource,
		DOUT_CtrlType,
		DOUT_Filter,
		DOUT_NodeOccNo,
		DOUT_ChannelOccNo,
		DOUT_DeviceOccNo,
		DOUT_AlarmOccNo,
		DOUT_ExpressionOccNo,
		DOUT_PinLabelOccNo,
		DOUT_LastUpdateTime,

		DOUT_End
	};

	enum  VARDATA_Attr
	{
		VARDATA_Begin,
		VARDATA_OccNo,
		VARDATA_BlockNo,
		VARDATA_NameOccNo,
		VARDATA_NodeOccNo,

		VARDATA_AlarmOccNo,
		VARDATA_ExpressOccNo,

		VARDATA_DataType,
		VARDATA_IddType,

		VARDATA_SrcNodeOccNo,
		VARDATA_SrcOccNo,
		VARDATA_SrcIddType,
		VARDATA_IsRefTag,

		VARDATA_State,

		VARDATA_IsDefined,
		VARDATA_ScanEnable,
		VARDATA_Init,
		VARDATA_Quality,
		VARDATA_ManSet,

		VARDATA_Value,
		VARDATA_RawValue,

		VARDATA_NegValue,
		VARDATA_CtrlByte,
		VARDATA_IsSOE,

		VARDATA_StartCtrl,
		VARDATA_SignalType,

		VARDATA_DataSource,

		VARDATA_Desc0OccNo,
		VARDATA_Desc1OccNo,

		VARDATA_RangeL,
		VARDATA_RangeH,
		VARDATA_HighQua,
		VARDATA_LowQua,

		VARDATA_MaxRaw,
		VARDATA_MinRaw,

		VARDATA_MaxScale,
		VARDATA_MinScale,

		VARDATA_LastUpdateTime,
		VARDATA_END,
	};
public:
	// 根据 对象类型，属性名称 获取 他的数据类型
	int16u GetAttDataType(int8u nIddType, const char * pszAttrName) const;
	//根据 对象类型，属性名称 获取 他的数据序号
	bool GetAttIndex(int8u nIddType, const char* pszAttrName, int32u& nIndex) const;
	// 获取 对象类型的所有属性名称
	bool GetTotalAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames) const;
	// 获取 对象类型的特定属性名称
	bool GetTotalAttrs(int8u nIddType, int8u nFilter, std::vector<std::string> & arrAttrNames)const;

	// 获取 对象类型的所有 只读 属性名称
	bool GetTotalReadOnlyAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames) const;
	// 获取 对象类型的特定 只读 属性名称
	bool GetTotalReadOnlyAttrs(int8u nIddType, int8u nFilter, std::vector<std::string> & arrAttrNames)const;
	// 获取 对象类型的特定 只读  数据类型 属性名称
	bool GetTotalReadOnlyAttrsWithDataType(int8u nIddType, int8u nDataType, std::vector<std::string> & arrAttrNames)const;

	// 获取 对象类型的所有 可写 属性名称
	bool GetTotalWriteableAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames)const;
	// 获取 对象类型的特定 可写 属性名称
	bool GetTotalWriteableAttrs(int8u nIddType, int8u nFilter, std::vector<std::string> & arrAttrNames)const;

	bool GetTotalWriteableAttrssWithDataType(int8u nIddType, int8u nDataType, std::vector<std::string> & arrAttrNames)const;

	std::array<ATTDEF, Node_End> & GetNodeAttrs()
	{
		return m_arrNodeAtts;
	}
	std::array<ATTDEF, Channel_End> & GetChannelAttrs()
	{
		return m_arrChannelAtts;
	}
	std::array<ATTDEF, Device_End> & GetDeviceAttrs()
	{
		return m_arrDevAtts;
	}
	std::array<ATTDEF, AIN_End> & GetAinAttrs()
	{
		return m_arrAinAtts;
	}
	std::array<ATTDEF, DIN_End> & GetDinAttrs()
	{
		return m_arrDinAtts;
	}
	std::array<ATTDEF, AOUT_End> & GetAoutAttrs()
	{
		return m_arrAoutAtts;
	}
	std::array<ATTDEF, DOUT_End> & GetDoutAttrs()
	{
		return m_arrDoutAtts;
	}
	std::array<ATTDEF, VARDATA_END> & GetUserVariableAttrs()
	{
		return m_arrUserVariableAtts;
	}
	std::array<ATTDEF, VARDATA_END> & GetSysVariableAttrs()
	{
		return m_arrSysVariableAtts;
	}
private:
	std::array<ATTDEF, Node_End> m_arrNodeAtts;
	std::array<ATTDEF, Channel_End> m_arrChannelAtts;
	std::array<ATTDEF, Device_End> m_arrDevAtts;
	std::array<ATTDEF, AIN_End> m_arrAinAtts;
	std::array<ATTDEF, DIN_End> m_arrDinAtts;
	std::array<ATTDEF, AOUT_End> m_arrAoutAtts;
	std::array<ATTDEF, DOUT_End> m_arrDoutAtts;
	std::array<ATTDEF, VARDATA_END> m_arrUserVariableAtts;
	std::array<ATTDEF, VARDATA_END> m_arrSysVariableAtts;
};





#endif // TAG_ATT_H

/** @}*/

