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

#include "fesapi/fesdatadef.h"
#include "fesapi/fescnst.h"

#include "tag_att.h"

#include <QDebug>
#include <stddef.h>
 

CTagAttMgr::CTagAttMgr()
{
	//NODE 
	/*序号      偏移          类型      ATT    ARRAY   RW        PathName[28]  PathNz[...]   */
	m_arrNodeAtts[Node_Begin]     = { Node_Begin, sizeof(NODE), DT_NULL, Node_End, ATT_NO,  ATT_RO, "", "" };
	
	m_arrNodeAtts[Node_OccNo] = { Node_OccNo, offsetof(NODE,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrNodeAtts[Node_GrpNo] = { Node_GrpNo,offsetof(NODE,GrpNo), DT_DWORD, ATT_NO, 0, ATT_RO, "GrpNo", "" };
	m_arrNodeAtts[Node_NameOccNo] = { Node_NameOccNo,offsetof(NODE,NameOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrNodeAtts[Node_SlaveOccNo] = { Node_SlaveOccNo,offsetof(NODE,SlaveOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "SlaveOccNo", "" };

	m_arrNodeAtts[Node_IsDefined] = { Node_IsDefined, offsetof(NODE,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RO, "IsDefined", "" };
	m_arrNodeAtts[Node_ScanEnable] = { Node_ScanEnable, offsetof(NODE,ScanEnable), DT_CHAR, ATTR_IN_OUT, 0, ATT_RW, "ScanEnable", "" };
	m_arrNodeAtts[Node_Init] = { Node_Init, offsetof(NODE,Init), DT_CHAR, ATT_RW, 0, ATT_RO, "Init", "" };
	m_arrNodeAtts[Node_Quality] = { Node_Quality, offsetof(NODE,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	m_arrNodeAtts[Node_RedState] = { Node_RedState, offsetof(NODE,RedState), DT_CHAR, ATT_STATE, 0, ATT_RO, "RedState", "" };
	m_arrNodeAtts[Node_NodeType]   = { Node_NodeType, offsetof(NODE,NodeType), DT_CHAR, ATT_RW, 0, ATT_RO, "NodeType", "" };
	
	m_arrNodeAtts[Node_State] = { Node_State, offsetof(NODE,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrNodeAtts[Node_Net1State] = { Node_Net1State, offsetof(NODE,Net1State), DT_SHORT, ATT_NETA, 0, ATT_RO, "Net1State", "" };
	m_arrNodeAtts[Node_Net2State] = { Node_Net2State, offsetof(NODE,Net2State), DT_SHORT, ATT_NETB, 0, ATT_RO, "Net2State", "" };
	
	m_arrNodeAtts[Node_PointCount] = { Node_PointCount, offsetof(NODE,PointCount), DT_DWORD, ATT_RW, 0, ATT_RO, "PointCount", "" };
	m_arrNodeAtts[Node_Net1IPAddr] = { Node_Net1IPAddr, offsetof(NODE,NetAIPAddr), DT_DWORD, ATT_NETA, 0, ATT_RO, "Net1IPAddr", "" };
	m_arrNodeAtts[Node_Net2IPAddr] = { Node_Net2IPAddr, offsetof(NODE,NetBIPAddr), DT_DWORD, ATT_NETB, 0, ATT_RO, "Net2IPAddr", "" };
	
	m_arrNodeAtts[Node_NetA6VAddr] = { Node_NetA6VAddr, offsetof(NODE,NetAIpV6Addr), DT_DWORD, ATT_NETA, sizeof int32u * 4, ATT_RO, "NetAV6IPAddr", "" };
	m_arrNodeAtts[Node_NetB6VAddr] = { Node_NetB6VAddr, offsetof(NODE,NetBIpV6Addr), DT_DWORD, ATT_NETB, sizeof int32u * 4, ATT_RO, "NetBV6IPAddr", "" };

	m_arrNodeAtts[NODE_TagName]   = { NODE_TagName, offsetof(NODE,TagName), DT_CHAR, ATT_RW, MAX_NAME_LENGTH + STRING_PAD, ATT_RO, "TagName", "" };
	m_arrNodeAtts[NODE_HostName]   = { NODE_HostName, offsetof(NODE,HostName), DT_CHAR, ATT_RW, MAX_NAME_LENGTH + STRING_PAD, ATT_RO, "HostName", "" };
	m_arrNodeAtts[NODE_GroupName]   = { NODE_GroupName, offsetof(NODE,GroupName), DT_CHAR, ATT_RW, MAX_NAME_LENGTH + STRING_PAD, ATT_RO, "GroupName", "" };
//	m_arrNodeAtts[Node_AliasName]  = { Node_AliasName, offsetof(NODE,IsDefined) + MAX_NAME_LENGTH + STRING_PAD, DT_DWORD, ATT_RW, MAX_NAME_LENGTH + STRING_PAD, ATT_RO, "AliasName", "" };

	//CHANNEL
	/*序号      偏移          类型      ATT    ARRAY   RW        PathName[28]  PathNz[...]   */
	m_arrChannelAtts[Channel_Begin] = { Channel_Begin, sizeof  CHANNEL, DT_NULL, Channel_End, ATT_NO, ATT_RO, "", "" };
	m_arrChannelAtts[Channel_OccNo] = { Channel_OccNo, offsetof(CHANNEL,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrChannelAtts[Channel_NameOccNo] = { Channel_NameOccNo, offsetof(CHANNEL,NameOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrChannelAtts[Channel_IsDefined]  = { Channel_IsDefined, offsetof(CHANNEL,IsDefined), DT_CHAR, ATT_NO, 0, ATT_RO, "IsDefined", "" };
	
	m_arrChannelAtts[Channel_ScanEnable] = { Channel_ScanEnable, offsetof(CHANNEL,ScanEnable), DT_CHAR, ATTR_IN_OUT, 0, ATT_RW, "", "" };
	m_arrChannelAtts[Channel_Init]       = { Channel_Init, offsetof(CHANNEL,Init), DT_CHAR, 0, ATT_RW, 0, "Init", "" };
	m_arrChannelAtts[Channel_Quality]    = { Channel_Quality, offsetof(CHANNEL,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	m_arrChannelAtts[Channel_PeerQuality] = { Channel_PeerQuality, offsetof(CHANNEL,PeerQuality), DT_CHAR, ATT_QUA, 0, ATT_RO, "PeerQuality", "" };
	m_arrChannelAtts[Channel_Heartbeat] = { Channel_Heartbeat, offsetof(CHANNEL,Heartbeat), DT_CHAR, ATT_NO, 0, ATT_RO, "Heartbeat", "" };
	m_arrChannelAtts[Channel_PrintOn] = { Channel_PrintOn, offsetof(CHANNEL,PrintOn), DT_CHAR, ATT_RW, 0, ATT_RO, "PrintOn", "" };
	m_arrChannelAtts[Channel_WorkNet] = { Channel_WorkNet, offsetof(CHANNEL,WorkNet), DT_CHAR, ATT_RW, 0, ATT_RO, "WorkNet", "" };
	m_arrChannelAtts[Channel_CommPara] = { Channel_CommPara, offsetof(CHANNEL,CommPara), DT_CHAR, ATT_RW, 0, ATT_RO, "CommPara", "" };
	m_arrChannelAtts[Channel_State] = { Channel_State, offsetof(CHANNEL,State), DT_DWORD, ATTR_STATE, 0, ATT_RO, "State", "" };
	m_arrChannelAtts[Channel_Prority] = { Channel_Prority, offsetof(CHANNEL,Prority), DT_CHAR, ATT_RW, 0, ATT_RO, "Prority", "" };
	m_arrChannelAtts[Channel_SetTimeAttr] = { Channel_SetTimeAttr, offsetof(CHANNEL,SetTimeAttr), DT_CHAR, ATT_RW, 0, ATT_RO, "SetTimeAttr", "" };
	m_arrChannelAtts[Channel_DeviceCount] = { Channel_DeviceCount, offsetof(CHANNEL,DeviceCount), DT_DWORD, ATT_RW, 0, ATT_RO, "DeviceCount", "" };
	m_arrChannelAtts[Channel_NodeOccNo] = { Channel_NodeOccNo, offsetof(CHANNEL,NodeOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "NodeOccNo", "" };
	m_arrChannelAtts[Channel_ChannelName] = { Channel_ChannelName, offsetof(CHANNEL,ChannelName), DT_CHAR, Channel_ChannelName,static_cast<int32u> (MAX_NAME_LENGTH + STRING_PAD), ATT_RO, "ChannelName", "" };
	m_arrChannelAtts[Channel_DriverName] = { Channel_DriverName, offsetof(CHANNEL,DriverName), DT_CHAR,Channel_DriverName, static_cast<int32u> (MAX_NAME_LENGTH + STRING_PAD),  ATT_RO, "DriverName", "" };
	m_arrChannelAtts[Channel_DriverType] = { Channel_DriverType, offsetof(CHANNEL,DriverType), DT_CHAR, ATT_RW, 0, ATT_RO, "DriverType", "" };
	m_arrChannelAtts[Channel_StateCore] = { Channel_StateCore, offsetof(CHANNEL,StateCore), DT_CHAR, ATT_RW, 0, ATT_RO, "StateCore", "" };
	m_arrChannelAtts[Channel_StateDriver] = { Channel_StateDriver, offsetof(CHANNEL,StateDriver), DT_CHAR, ATT_RW, 0, ATT_RO, "StateDriver", "" };

	//DEVICE
	/*序号      偏移          类型      ATT    ARRAY   RW        PathName[28]  PathNz[...]   */
	m_arrDevAtts[Device_Begin] = { Device_Begin, sizeof DEVICE, DT_NULL, ATT_NO, 0, ATT_RO, "", "" };
	m_arrDevAtts[Device_OccNo] = { Device_OccNo,  offsetof(DEVICE,OccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "OccNo", "" };
	m_arrDevAtts[Device_NameOccNo] = { Device_NameOccNo, offsetof(DEVICE,NameOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "NameOccNo", "" };
	m_arrDevAtts[Device_NodeOccNo] = { Device_NodeOccNo, offsetof(DEVICE,NodeOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "NodeOccNo", "" };
	m_arrDevAtts[Device_ChannelOccNo] = { Device_ChannelOccNo, offsetof(DEVICE,ChannelOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "ChannelOccNo", "" };
	m_arrDevAtts[Device_ModelOccNo] = { Device_ModelOccNo,offsetof(DEVICE,ModelOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "ModelOccNo", "" };
	m_arrDevAtts[Device_Type] = { Device_Type, offsetof(DEVICE,Type), DT_CHAR, ATT_RW, 0, ATT_RO, "Type", "" };
	m_arrDevAtts[Device_Pad1] = { Device_Pad1, offsetof(DEVICE,Pad1), DT_CHAR, ATT_RW, 0, ATT_RO, "Pad1", "" };
	m_arrDevAtts[Device_Pad2] = { Device_Pad2,offsetof(DEVICE,Pad2), DT_WORD,ATT_RW, 0, ATT_RO, "Pad2", "" };
	m_arrDevAtts[Device_Heartbeat] = { Device_Heartbeat, offsetof(DEVICE,Heartbeat), DT_DWORD, ATT_RW, 0, ATT_RO, "Heartbeat", "" };
	m_arrDevAtts[Device_Address] = { Device_Address, offsetof(DEVICE,Address), DT_CHAR, ATT_RW, 0, ATT_RO, "Address", "" };
	m_arrDevAtts[Device_Extention] = { Device_Extention, offsetof(DEVICE,Extention), DT_CHAR, ATT_RW, 0, ATT_RO, "Extention", "" };
	m_arrDevAtts[Device_IsDefined] = { Device_IsDefined,offsetof(DEVICE,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RO, "IsDefined", "" };
	
	m_arrDevAtts[Device_ScanEnable] = { Device_ScanEnable,offsetof(DEVICE,ScanEnable), DT_CHAR, ATTR_IN_OUT, 0, ATT_RO, "ScanEnable", "" };
	m_arrDevAtts[Device_Init] = { Device_Init, offsetof(DEVICE,Init), DT_CHAR, ATT_NO, 0, ATT_RO, "Init", "" };
	m_arrDevAtts[Device_Quality] = { Device_Quality, offsetof(DEVICE,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };

	//AIN
	/*序号      偏移          类型      ATT    ARRAY   RW        PathName[28]  PathNz[...]   */
	m_arrAinAtts[AIN_Begin] = { AIN_Begin, sizeof AIN, DT_NULL, AIN_End, ATT_NO, ATT_RO, "", "" };
	m_arrAinAtts[AIN_OccNo] = { AIN_OccNo, offsetof(AIN,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrAinAtts[AIN_BlockNo] = { AIN_BlockNo, offsetof(AIN,BlockNo), DT_DWORD, ATT_NO, 0, ATT_RO, "BlockNo", "" };
	m_arrAinAtts[AIN_NameOccNo] = { AIN_NameOccNo, offsetof(AIN,NameOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrAinAtts[AIN_IsSpare] = { AIN_IsSpare, offsetof(AIN,IsSpare), DT_CHAR, ATT_RW, 0, ATT_RO, "IsSpare", "" };
	m_arrAinAtts[AIN_Pad2] = { AIN_Pad2, offsetof(AIN,Pad2), DT_WORD, ATT_NO, 0, ATT_RO, "Pad2", "" };
	m_arrAinAtts[AIN_Address] = { AIN_Address, offsetof(AIN,Address), DT_CHAR, ATT_RW, MAX_ADDRESS_LENGTH + STRING_PAD, ATT_RO, "Address", "" };
	m_arrAinAtts[AIN_State] = { AIN_State, offsetof(AIN,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrAinAtts[AIN_IsDefined] = { AIN_IsDefined, offsetof(AIN,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RO, "IsDefined", "" };
	m_arrAinAtts[AIN_ScanEnable] = { AIN_ScanEnable, offsetof(AIN,ScanEnable), DT_CHAR, ATTR_IN_OUT, 0, ATT_RO, "ScanEnable", "" };
	m_arrAinAtts[AIN_Init] = { AIN_Init, offsetof(AIN,Init), DT_CHAR, ATT_NO, 0, ATT_RO, "Init", "" };
	m_arrAinAtts[AIN_Quality] = { AIN_Quality, offsetof(AIN,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	m_arrAinAtts[AIN_ManSet] = { AIN_ManSet, offsetof(AIN,ManSet), DT_CHAR, ATT_MANSET, 0, ATT_RW, "ManSet", "" };
	m_arrAinAtts[AIN_SignalValue] = { AIN_SignalValue, offsetof(AIN,SignalValue), DT_DOUBLE, ATT_SIGNAL_VALUE, 0, ATT_RW, "SignalValue", "" };
	m_arrAinAtts[AIN_RawValue] = { AIN_RawValue, offsetof(AIN,RawValue), DT_DOUBLE, ATT_RAW_VALUE, 0, ATT_RW, "RawValue", "" };
	m_arrAinAtts[AIN_Value] = { AIN_Value, offsetof(AIN,Value), DT_DOUBLE, ATT_VALUE, 0, ATT_RW, "Value", "" };
	m_arrAinAtts[AIN_OldValue] = { AIN_OldValue, offsetof(AIN,OldValue), DT_DOUBLE, ATT_OLD_VALUE, 0, ATT_RW, "OldValue", "" };
	m_arrAinAtts[AIN_InitValue] = { AIN_InitValue, offsetof(AIN,InitValue), DT_DOUBLE, ATT_RW, 0, ATT_RW, "InitValue", "" };
	m_arrAinAtts[AIN_DataSource] = { AIN_DataSource, offsetof(AIN,DataSource), DT_DWORD, ATT_RW, 0, ATT_RO, "DataSource", "" };
	m_arrAinAtts[AIN_RangeL] = { AIN_RangeL, offsetof(AIN,RangeL), DT_DOUBLE, ATTR_MINRANGE, 0, ATT_RO, "RangeL", "" };
	m_arrAinAtts[AIN_RangeH] = { AIN_RangeH, offsetof(AIN,RangeH), DT_DOUBLE, ATTR_MAXRANGE, 0, ATT_RO, "RangeH", "" };
	m_arrAinAtts[AIN_HighQua] = { AIN_HighQua, offsetof(AIN,HighQua), DT_DOUBLE, ATT_HIQUA, 0, ATT_RO, "HighQua", "" };
	m_arrAinAtts[AIN_LowQua] = { AIN_LowQua, offsetof(AIN,LowQua), DT_DOUBLE, ATT_LOQUA, 0, ATT_RO, "LowQua", "" };
	m_arrAinAtts[AIN_MaxRaw] = { AIN_MaxRaw, offsetof(AIN,MaxRaw), DT_DOUBLE, ATT_MAXOUTPUT, 0, ATT_RO, "MaxRaw", "" };
	m_arrAinAtts[AIN_MinRaw] = { AIN_MinRaw, offsetof(AIN,MinRaw), DT_DOUBLE, ATT_MINOUTPUT, 0, ATT_RO, "MinRaw", "" };
	m_arrAinAtts[AIN_NodeOccNo]    = { AIN_NodeOccNo, offsetof(AIN,NodeOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NodeOccNo", "" };
	m_arrAinAtts[AIN_ChannelOccNo] = { AIN_ChannelOccNo, offsetof(AIN,ChannelOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "ChannelOccNo", "" };
	m_arrAinAtts[AIN_DeviceOccNo]  = { AIN_DeviceOccNo, offsetof(AIN,DeviceOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "DeviceOccNo", "" };
	m_arrAinAtts[AIN_AlarmOccNo]   = { AIN_AlarmOccNo, offsetof(AIN,AlarmOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "AlarmOccNo", "" };
	m_arrAinAtts[AIN_ExpressOccNo] = { AIN_ExpressOccNo, offsetof(AIN,ExpressOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "ExpressOccNo", "" };
	m_arrAinAtts[AIN_TransformType] = { AIN_TransformType, offsetof(AIN,TransformType), DT_CHAR, ATT_NO, 0, ATT_RO, "TransformType", "" };
	m_arrAinAtts[AIN_TransformOccNo] = { AIN_TransformOccNo, offsetof(AIN,TransformOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "TransformOccNo", "" };
	m_arrAinAtts[AIN_DescriptionOccNo] = { AIN_DescriptionOccNo, offsetof(AIN,DescriptionOccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "DescriptionOccNo", "" };
	m_arrAinAtts[AIN_PinLabelOccNo] = { AIN_PinLabelOccNo, offsetof(AIN,PinLabelOccNo), DT_DWORD, ATT_PINLABEL, 0, ATT_RO, "PinLabelOccNo", "" };
	m_arrAinAtts[AIN_UnitNameOccNo] = { AIN_UnitNameOccNo, offsetof(AIN,UnitNameOccNo), DT_DWORD, ATT_UNIT, 0, ATT_RO, "UnitNameOccNo", "" };
	m_arrAinAtts[AIN_LastUpdateTime] = { AIN_LastUpdateTime, offsetof(AIN,LastUpdateTime), DT_STRING, ATT_NO, 0, ATT_RO, "LastUpdateTime", "" };
	
	//DIN
	m_arrDinAtts[DIN_Begin] = { DIN_Begin, sizeof DIN, DT_NULL, DIN_End, ATT_NO, ATT_RO, "", "" };
	m_arrDinAtts[DIN_OccNo] = { DIN_OccNo, offsetof(DIN,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrDinAtts[DIN_BlockNo] = { DIN_BlockNo, offsetof(DIN,BlockNo), DT_DWORD, ATT_NO, 0, ATT_RO, "BlockNo", "" };
	m_arrDinAtts[DIN_NameOccNo] = { DIN_NameOccNo, offsetof(DIN,NameOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrDinAtts[DIN_NeedTranmitFlag] = { DIN_NameOccNo, offsetof(DIN,NeedTranmitFlag), DT_CHAR, ATT_RW, 0, ATT_RO, "NeedTranmitFlag", "" };
	m_arrDinAtts[DIN_Pad2] = { DIN_Pad2, offsetof(DIN,Pad2), DT_WORD, ATT_RW, 0, ATT_RW, "Pad2", "" };
	m_arrDinAtts[DIN_IsSpare] = { DIN_IsSpare, offsetof(DIN,IsSpare), DT_CHAR, ATT_RW, 0, ATT_RO, "IsSpare", "" };
	m_arrDinAtts[DIN_Address] = { DIN_Address, offsetof(DIN,Address), DT_CHAR, ATT_RW, 0, ATT_RO, "Address", "" };
	
	m_arrDinAtts[DIN_InitValEnable] = { DIN_InitValEnable, offsetof(DIN,InitValEnable), DT_CHAR, ATT_RW, 0, ATT_RW, "InitValEnable", "" };
	m_arrDinAtts[DIN_InitValue] = { DIN_InitValue, offsetof(DIN,InitValue), DT_CHAR, ATT_RW, 0, ATT_RW, "InitValue", "" };
	m_arrDinAtts[DIN_InitQua] = { DIN_InitQua, offsetof(DIN,InitQua), DT_CHAR, ATT_RW, 0, ATT_RO, "InitQua", "" };
	m_arrDinAtts[DIN_SlaveOccNo] = { DIN_SlaveOccNo, offsetof(DIN,SlaveOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "SlaveOccNo", "" };
	m_arrDinAtts[DIN_DDIFilter] = { DIN_DDIFilter, offsetof(DIN,DDIFilter), DT_CHAR, ATT_RW, 0, ATT_RW, "DDIFilter", "" };
	m_arrDinAtts[DIN_DDIFlag] = { DIN_DDIFlag, offsetof(DIN,DDIFlag), DT_CHAR, ATT_RW, 0, ATT_RW, "DDIFlag", "" };
	m_arrDinAtts[DIN_DDIInUse] = { DIN_DDIInUse, offsetof(DIN,DDIInUse), DT_CHAR, ATT_RW, 0, ATT_RW, "DDIInUse", "" };
	
	m_arrDinAtts[DIN_State] = { DIN_State, offsetof(DIN,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrDinAtts[DIN_IsDefined] = { DIN_IsDefined, offsetof(DIN,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RO, "IsDefined", "" };
	m_arrDinAtts[DIN_ScanEnable] = { DIN_ScanEnable, offsetof(DIN,ScanEnable), DT_CHAR, ATTR_IN_OUT, 0, ATT_RW, "ScanEnable", "" };
	
	m_arrDinAtts[DIN_Init] = { DIN_Init, offsetof(DIN,Init), DT_CHAR, ATT_RW, 0, ATT_RO, "Init", "" };
	m_arrDinAtts[DIN_Quality] = { DIN_Quality, offsetof(DIN,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	m_arrDinAtts[DIN_ManSet] = { DIN_ManSet, offsetof(DIN,ManSet), DT_CHAR, ATT_MANSET, 0, ATT_RW, "ManSet", "" };
	m_arrDinAtts[DIN_Value] = { DIN_Value, offsetof(DIN,Value), DT_CHAR, ATT_VALUE, 0, ATT_RO, "Value", "" };
	m_arrDinAtts[DIN_NegValue] = { DIN_NegValue, offsetof(DIN,NegValue), DT_CHAR, ATT_OLD_VALUE, 0, ATT_RO, "NegValue", "" };
	m_arrDinAtts[DIN_RawValue] = { DIN_RawValue, offsetof(DIN,RawValue), DT_CHAR, ATT_RAW_VALUE, 0, ATT_RO, "RawValue", "" };
	m_arrDinAtts[DIN_CtrlByte] = { DIN_CtrlByte, offsetof(DIN,CtrlByte), DT_CHAR, ATT_RW, 0, ATT_RO, "CtrlByte", "" };
	m_arrDinAtts[DIN_IsSOE] = { DIN_IsSOE, offsetof(DIN,IsSOE), DT_CHAR, ATT_RW, 0, ATT_RO, "IsSOE", "" };
	m_arrDinAtts[DIN_StartCtrl] = { DIN_StartCtrl, offsetof(DIN,StartCtrl), DT_CHAR, ATT_START, 0, ATT_RO, "StartCtrl", "" };
	
	m_arrDinAtts[DIN_SignalType] = { DIN_SignalType, offsetof(DIN,SignalType), DT_DWORD, ATT_NO, 0, ATT_RO, "SignalType", "" };
	m_arrDinAtts[DIN_NodeOccNo] = { DIN_NodeOccNo, offsetof(DIN,NodeOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NodeOccNo", "" };
	m_arrDinAtts[DIN_ChannelOccNo] = { DIN_ChannelOccNo, offsetof(DIN,ChannelOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "ChannelOccNo", "" };
	m_arrDinAtts[DIN_DeviceOccNo] = { DIN_DeviceOccNo, offsetof(DIN,DeviceOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "DeviceOccNo", "" };
	m_arrDinAtts[DIN_AlarmOccNo] = { DIN_AlarmOccNo, offsetof(DIN,AlarmOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "AlarmOccNo", "" };
	m_arrDinAtts[DIN_DataSource] = { DIN_DataSource, offsetof(DIN,DataSource), DT_DWORD, ATT_NO, 0, ATT_RO, "DataSource", "" };
	m_arrDinAtts[DIN_ExpressOccNo] = { DIN_ExpressOccNo, offsetof(DIN,ExpressOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "ExpressOccNo", "" };
	m_arrDinAtts[DIN_Desc0_OccNo] = { DIN_Desc0_OccNo, offsetof(DIN,Desc0_OccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "Desc0_OccNo", "" };
	m_arrDinAtts[DIN_Desc1_OccNo] = { DIN_Desc1_OccNo, offsetof(DIN,Desc1_OccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "Desc1_OccNo", "" };
	m_arrDinAtts[DIN_PinLabelOccNo] = { DIN_PinLabelOccNo, offsetof(DIN,PinLabelOccNo), DT_DWORD, ATT_PINLABEL, 0, ATT_RO, "PinLabelOccNo", "" };
	m_arrDinAtts[DIN_LastUpdateTime] = { DIN_LastUpdateTime, offsetof(DIN,LastUpdateTime), DT_DWORD, ATT_NO, sizeof TIMEPAK, ATT_RO, "LastUpdateTime", "" };
	
	
	//AOUT
	/*序号      偏移          类型      ATT    ARRAY   RW        PathName[28]  PathNz[...]   */
	m_arrAoutAtts[AOUT_Begin] = { AOUT_Begin, sizeof AOUT, DT_NULL, AOUT_End, ATT_NO, ATT_RO, "", "" };
	m_arrAoutAtts[AOUT_OccNo] = { AOUT_OccNo, offsetof(AOUT,OccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "OccNo", "" };
	m_arrAoutAtts[AOUT_BlockNo] = { AOUT_BlockNo, offsetof(AOUT,BlockNo), DT_DWORD, ATT_RO, 0, ATT_RO, "BlockNo", "" };
	m_arrAoutAtts[AOUT_NameOccNo] = { AOUT_NameOccNo, offsetof(AOUT,NameOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "NameOccNo", "" };
	m_arrAoutAtts[AOUT_State] = { AOUT_State, offsetof(AOUT,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrAoutAtts[AOUT_IsDefined] = { AOUT_IsDefined, offsetof(AOUT,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RW, "IsDefined", "" };
	m_arrAoutAtts[AOUT_ScanEnable] = { AOUT_ScanEnable, offsetof(AOUT,ScanEnable), DT_CHAR, ATT_IN_OUT, 0, ATT_RW, "ScanEnable", "" };
	m_arrAoutAtts[AOUT_Init] = { AOUT_Init, offsetof(AOUT,Init), DT_CHAR, ATT_RW, 0, ATT_RO, "Init", "" };
	m_arrAoutAtts[AOUT_Quality] = { AOUT_Quality, offsetof(AOUT,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	m_arrAoutAtts[AOUT_Output] = { AOUT_Output, offsetof(AOUT,Output), DT_DOUBLE, ATT_AOUT, 0, ATT_RW, "Output", "" };
	
	m_arrAoutAtts[AOUT_RawData] = { AOUT_RawData, offsetof(AOUT,RawData), DT_DOUBLE, ATT_RAW_VALUE, 0, ATT_RO, "RawData", "" };
	m_arrAoutAtts[AOUT_NodeOccNo] = { AOUT_NodeOccNo, offsetof(AOUT,NodeOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "NodeOccNo", "" };
	m_arrAoutAtts[AOUT_ChannelOccNo] = { AOUT_ChannelOccNo, offsetof(AOUT,ChannelOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "ChannelOccNo", "" };
	m_arrAoutAtts[AOUT_DeviceOccNo] = { AOUT_DeviceOccNo, offsetof(AOUT,DeviceOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "DeviceOccNo", "" };
	m_arrAoutAtts[AOUT_AlarmOccNo] = { AOUT_AlarmOccNo, offsetof(AOUT,AlarmOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "AlarmOccNo", "" };
	m_arrAoutAtts[AOUT_ExpressOccNo] = { AOUT_ExpressOccNo, offsetof(AOUT,ExpressOccNo), DT_DWORD, ATT_RO, 0, ATT_RO, "ExpressOccNo", "" };
	m_arrAoutAtts[AOUT_TransformType] = { AOUT_TransformType, offsetof(AOUT,TransformType), DT_CHAR, ATT_RW, 0, ATT_RO, "TransformType", "" };
	m_arrAoutAtts[AOUT_TransformOccNo] = { AOUT_TransformOccNo, offsetof(AOUT,TransformOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "TransformOccNo", "" };
	m_arrAoutAtts[AOUT_PinLabelOccNo] = { AOUT_PinLabelOccNo, offsetof(AOUT,PinLabelOccNo), DT_DWORD, ATT_PINLABEL, 0, ATT_RO, "PinLabelOccNo", "" };
	m_arrAoutAtts[AOUT_UnitNameOccNo] = { AOUT_UnitNameOccNo, offsetof(AOUT,UnitNameOccNo), DT_DWORD, ATT_UNIT, 0, ATT_RO, "UnitNameOccNo", "" };
	m_arrAoutAtts[AOUT_LastUpdateTime] = { AOUT_LastUpdateTime, offsetof(AOUT,LastUpdateTime), DT_STRING, ATT_RW, sizeof TIMEPAK, ATT_RO, "LastUpdateTime", "" };
	
	
	//DOUT
	m_arrDoutAtts[DOUT_Begin] = { DOUT_Begin, sizeof DOUT, DT_NULL, DOUT_End, ATT_NO, ATT_RO, "", "" };
	m_arrDoutAtts[DOUT_OccNo] = { DOUT_OccNo, offsetof(DOUT,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrDoutAtts[DOUT_BlockNo] = { DOUT_BlockNo, offsetof(DOUT,BlockNo),DT_DWORD, ATT_NO, 0, ATT_RO, "BlockNo", "" };
	m_arrDoutAtts[DOUT_NameOccNo] = { DOUT_NameOccNo, offsetof(DOUT,NameOccNo),DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	
	m_arrDoutAtts[DOUT_State] = { DOUT_State, offsetof(DOUT,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrDoutAtts[DOUT_IsDefined] = { DOUT_IsDefined, offsetof(DOUT,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RW, "IsDefined", "" };
	m_arrDoutAtts[DOUT_ScanEna] = { DOUT_ScanEna, offsetof(DOUT,ScanEnable), DT_CHAR, ATT_IN_OUT, 0, ATT_RW, "ScanEna", "" };
	m_arrDoutAtts[DOUT_Init] = { DOUT_Init, offsetof(DOUT,Init), DT_CHAR, ATT_RW, 0, ATT_RO, "Init", "" };
	
	m_arrDoutAtts[DOUT_Quality] = { DOUT_Quality, offsetof(DOUT,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	m_arrDoutAtts[DOUT_Output] = { DOUT_Output, offsetof(DOUT,Output), DT_DOUBLE, ATT_DOUT, 0, ATT_RW, "Output", "" };
	
	m_arrDoutAtts[DOUT_RelayState] = { DOUT_RelayState, offsetof(DOUT,RelayState), DT_CHAR, ATT_RW, 0, ATT_RO, "RelayState", "" };
	m_arrDoutAtts[DOUT_DataSource] = { DOUT_DataSource, offsetof(DOUT,DataSource), DT_DOUBLE, ATT_RW, 0, ATT_RO, "DataSource", "" };
	m_arrDoutAtts[DOUT_CtrlType] = { DOUT_CtrlType, offsetof(DOUT,CtrlType), DT_CHAR, ATT_RW, 0, ATT_RO, "CtrlType", "" };
	m_arrDoutAtts[DOUT_Filter] = { DOUT_Filter, offsetof(DOUT,Filter), DT_CHAR, ATT_RW, 0, ATT_RO, "Filter", "" };
	m_arrDoutAtts[DOUT_NodeOccNo] = { DOUT_NodeOccNo, offsetof(DOUT,NodeOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "NodeOccNo", "" };
	m_arrDoutAtts[DOUT_ChannelOccNo] = { DOUT_ChannelOccNo, offsetof(DOUT,ChannelOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "ChannelOccNo", "" };
	m_arrDoutAtts[DOUT_DeviceOccNo] = { DOUT_DeviceOccNo, offsetof(DOUT,DeviceOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "DeviceOccNo", "" };
	m_arrDoutAtts[DOUT_AlarmOccNo] = { DOUT_AlarmOccNo, offsetof(DOUT,AlarmOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "AlarmOccNo", "" };
	m_arrDoutAtts[DOUT_ExpressionOccNo] = { DOUT_ExpressionOccNo, offsetof(DOUT,ExpressionOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "ExpressionOccNo", "" };
	m_arrDoutAtts[DOUT_PinLabelOccNo] = { DOUT_PinLabelOccNo, offsetof(DOUT,PinLabelOccNo), DT_DWORD, ATT_PINLABEL, 0, ATT_RO, "PinLabelOccNo", "" };
	m_arrDoutAtts[DOUT_LastUpdateTime] = { DOUT_LastUpdateTime, offsetof(DOUT,LastUpdateTime), DT_STRING, ATT_RW, sizeof TIMEPAK, ATT_RO, "LastUpdateTime", "" };
	
	//uservariable
	m_arrUserVariableAtts[VARDATA_Begin] = { VARDATA_Begin, sizeof VARDATA, DT_NULL, VARDATA_END, ATT_NO, ATT_RO, "", "" };
	m_arrUserVariableAtts[VARDATA_OccNo] = { VARDATA_OccNo, offsetof(VARDATA,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrUserVariableAtts[VARDATA_BlockNo] = { VARDATA_BlockNo, offsetof(VARDATA,BlockNo),DT_DWORD, ATT_NO, 0, ATT_RO, "BlockNo", "" };
	m_arrUserVariableAtts[VARDATA_NameOccNo] = { VARDATA_NameOccNo, offsetof(VARDATA,NameOccNo),DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrUserVariableAtts[VARDATA_NodeOccNo] = { VARDATA_NodeOccNo, offsetof(VARDATA,NodeOccNo),DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrUserVariableAtts[VARDATA_AlarmOccNo] = { VARDATA_AlarmOccNo, offsetof(VARDATA,AlarmOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "AlarmOccNo", "" };
	m_arrUserVariableAtts[VARDATA_ExpressOccNo] = { VARDATA_ExpressOccNo, offsetof(VARDATA,ExpressOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "ExpressionOccNo", "" };

	m_arrUserVariableAtts[VARDATA_DataType] = { VARDATA_DataType, offsetof(VARDATA,DataType), DT_CHAR, ATT_RW, 0, ATT_RO, "DataType", "" };
	m_arrUserVariableAtts[VARDATA_IddType] = { VARDATA_IddType, offsetof(VARDATA,IddType), DT_CHAR, ATT_RW, 0, ATT_RO, "IddType", "" };
	m_arrUserVariableAtts[VARDATA_SrcNodeOccNo] = { VARDATA_SrcNodeOccNo, offsetof(VARDATA,SrcNodeOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "SrcNodeOccNo", "" };
	m_arrUserVariableAtts[VARDATA_SrcOccNo] = { VARDATA_SrcOccNo, offsetof(VARDATA,SrcOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "SrcOccNo", "" };
	m_arrUserVariableAtts[VARDATA_SrcIddType] = { VARDATA_SrcIddType, offsetof(VARDATA,SrcIddType), DT_CHAR, ATT_RW, 0, ATT_RO, "SrcIddType", "" };
	m_arrUserVariableAtts[VARDATA_IsRefTag] = { VARDATA_IsRefTag, offsetof(VARDATA,IsRefTag), DT_CHAR, ATT_RW, 0, ATT_RO, "IsRefTag", "" };
	
	m_arrUserVariableAtts[VARDATA_State] = { VARDATA_State, offsetof(VARDATA,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrUserVariableAtts[VARDATA_IsDefined] = { VARDATA_IsDefined, offsetof(VARDATA,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RW, "IsDefined", "" };
	m_arrUserVariableAtts[VARDATA_ScanEnable] = { VARDATA_ScanEnable, offsetof(VARDATA,ScanEnable), DT_CHAR, ATT_IN_OUT, 0, ATT_RW, "ScanEna", "" };
	m_arrUserVariableAtts[VARDATA_Init] = { VARDATA_Init, offsetof(VARDATA,Init), DT_CHAR, ATT_RW, 0, ATT_RO, "Init", "" };
	m_arrUserVariableAtts[VARDATA_Quality] = { VARDATA_Quality, offsetof(VARDATA,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };
	
	m_arrUserVariableAtts[VARDATA_ManSet] = { VARDATA_ManSet, offsetof(VARDATA,ManSet), DT_CHAR, ATT_MANSET, 0, ATT_RW, "ManSet", "" };
	m_arrUserVariableAtts[VARDATA_Value] = { VARDATA_Value, offsetof(VARDATA,Value), DT_DOUBLE, ATT_VALUE, 0, ATT_RW, "Value", "" };
	m_arrUserVariableAtts[VARDATA_RawValue] = { AIN_RawValue, offsetof(VARDATA,RawValue), DT_DOUBLE, ATT_RAW_VALUE, 0, ATT_RW, "RawValue", "" };
	m_arrUserVariableAtts[VARDATA_NegValue] = { VARDATA_NegValue, offsetof(VARDATA,NegValue), DT_DOUBLE, ATT_OLD_VALUE, 0, ATT_RW, "NegValue", "" };

	m_arrUserVariableAtts[VARDATA_CtrlByte] = { VARDATA_CtrlByte, offsetof(VARDATA,CtrlByte), DT_CHAR, ATT_RW, 0, ATT_RO, "CtrlByte", "" };
	m_arrUserVariableAtts[VARDATA_IsSOE] = { VARDATA_IsSOE, offsetof(VARDATA,IsSOE), DT_CHAR, ATT_RW, 0, ATT_RO, "IsSOE", "" };
	m_arrUserVariableAtts[VARDATA_StartCtrl] = { VARDATA_StartCtrl, offsetof(VARDATA,StartCtrl), DT_CHAR, ATT_START, 0, ATT_RO, "StartCtrl", "" };
	m_arrUserVariableAtts[VARDATA_SignalType] = { VARDATA_SignalType, offsetof(VARDATA,SignalType), DT_DWORD, ATT_NO, 0, ATT_RO, "SignalType", "" };

	m_arrUserVariableAtts[VARDATA_DataSource] = { VARDATA_DataSource, offsetof(VARDATA,DataSource), DT_DWORD, ATT_RW, 0, ATT_RO, "DataSource", "" };
	
	m_arrUserVariableAtts[VARDATA_Desc0OccNo] = { VARDATA_Desc0OccNo, offsetof(VARDATA,Desc0OccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "Desc0OccNo", "" };
	m_arrUserVariableAtts[VARDATA_Desc1OccNo] = { VARDATA_Desc1OccNo, offsetof(VARDATA,Desc1OccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "Desc1OccNo", "" };

	m_arrUserVariableAtts[VARDATA_RangeL] = { VARDATA_RangeL, offsetof(VARDATA,RangeL), DT_DOUBLE, ATTR_MINRANGE, 0, ATT_RO, "RangeL", "" };
	m_arrUserVariableAtts[VARDATA_RangeH] = { VARDATA_RangeH, offsetof(VARDATA,RangeH), DT_DOUBLE, ATTR_MAXRANGE, 0, ATT_RO, "RangeH", "" };
	m_arrUserVariableAtts[VARDATA_HighQua] = { VARDATA_HighQua, offsetof(VARDATA,HighQua), DT_DOUBLE, ATT_HIQUA, 0, ATT_RO, "HighQua", "" };
	m_arrUserVariableAtts[VARDATA_LowQua] = { VARDATA_LowQua, offsetof(VARDATA,LowQua), DT_DOUBLE, ATT_LOQUA, 0, ATT_RO, "LowQua", "" };
	m_arrUserVariableAtts[VARDATA_MaxRaw] = { VARDATA_MaxRaw, offsetof(VARDATA,MaxRaw), DT_DOUBLE, ATT_MAXOUTPUT, 0, ATT_RO, "MaxRaw", "" };
	m_arrUserVariableAtts[VARDATA_MinRaw] = { VARDATA_MinRaw, offsetof(VARDATA,MinRaw), DT_DOUBLE, ATT_MINOUTPUT, 0, ATT_RO, "MinRaw", "" };

	m_arrUserVariableAtts[VARDATA_LastUpdateTime] = { VARDATA_LastUpdateTime, offsetof(VARDATA,LastUpdateTime), DT_STRING, ATT_RW, sizeof TIMEPAK, ATT_RO, "LastUpdateTime", "" };
	
	//sysvariable
	m_arrSysVariableAtts[VARDATA_Begin] = { VARDATA_Begin, sizeof VARDATA, DT_NULL, VARDATA_END, ATT_NO, ATT_RO, "", "" };
	m_arrSysVariableAtts[VARDATA_OccNo] = { VARDATA_OccNo, offsetof(VARDATA,OccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "OccNo", "" };
	m_arrSysVariableAtts[VARDATA_BlockNo] = { VARDATA_BlockNo, offsetof(VARDATA,BlockNo),DT_DWORD, ATT_NO, 0, ATT_RO, "BlockNo", "" };
	m_arrSysVariableAtts[VARDATA_NameOccNo] = { VARDATA_NameOccNo, offsetof(VARDATA,NameOccNo),DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrSysVariableAtts[VARDATA_NodeOccNo] = { VARDATA_NodeOccNo, offsetof(VARDATA,NodeOccNo),DT_DWORD, ATT_NO, 0, ATT_RO, "NameOccNo", "" };
	m_arrSysVariableAtts[VARDATA_AlarmOccNo] = { VARDATA_AlarmOccNo, offsetof(VARDATA,AlarmOccNo), DT_DWORD, ATT_NO, 0, ATT_RO, "AlarmOccNo", "" };
	m_arrSysVariableAtts[VARDATA_ExpressOccNo] = { VARDATA_ExpressOccNo, offsetof(VARDATA,ExpressOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "ExpressionOccNo", "" };

	m_arrSysVariableAtts[VARDATA_DataType] = { VARDATA_DataType, offsetof(VARDATA,DataType), DT_CHAR, ATT_RW, 0, ATT_RO, "DataType", "" };
	m_arrSysVariableAtts[VARDATA_IddType] = { VARDATA_IddType, offsetof(VARDATA,IddType), DT_CHAR, ATT_RW, 0, ATT_RO, "IddType", "" };
	m_arrSysVariableAtts[VARDATA_SrcNodeOccNo] = { VARDATA_SrcNodeOccNo, offsetof(VARDATA,SrcNodeOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "SrcNodeOccNo", "" };
	m_arrSysVariableAtts[VARDATA_SrcOccNo] = { VARDATA_SrcOccNo, offsetof(VARDATA,SrcOccNo), DT_DWORD, ATT_RW, 0, ATT_RO, "SrcOccNo", "" };
	m_arrSysVariableAtts[VARDATA_SrcIddType] = { VARDATA_SrcIddType, offsetof(VARDATA,SrcIddType), DT_CHAR, ATT_RW, 0, ATT_RO, "SrcIddType", "" };
	m_arrSysVariableAtts[VARDATA_IsRefTag] = { VARDATA_IsRefTag, offsetof(VARDATA,IsRefTag), DT_CHAR, ATT_RW, 0, ATT_RO, "IsRefTag", "" };

	m_arrSysVariableAtts[VARDATA_State] = { VARDATA_State, offsetof(VARDATA,State), DT_DWORD, ATT_STATE, 0, ATT_RO, "State", "" };
	m_arrSysVariableAtts[VARDATA_IsDefined] = { VARDATA_IsDefined, offsetof(VARDATA,IsDefined), DT_CHAR, ATT_RW, 0, ATT_RW, "IsDefined", "" };
	m_arrSysVariableAtts[VARDATA_ScanEnable] = { VARDATA_ScanEnable, offsetof(VARDATA,ScanEnable), DT_CHAR, ATT_IN_OUT, 0, ATT_RW, "ScanEna", "" };
	m_arrSysVariableAtts[VARDATA_Init] = { VARDATA_Init, offsetof(VARDATA,Init), DT_CHAR, ATT_RW, 0, ATT_RO, "Init", "" };
	m_arrSysVariableAtts[VARDATA_Quality] = { VARDATA_Quality, offsetof(VARDATA,Quality), DT_CHAR, ATT_QUA, 0, ATT_RO, "Quality", "" };

	m_arrSysVariableAtts[VARDATA_ManSet] = { VARDATA_ManSet, offsetof(VARDATA,ManSet), DT_CHAR, ATT_MANSET, 0, ATT_RW, "ManSet", "" };
	m_arrSysVariableAtts[VARDATA_Value] = { VARDATA_Value, offsetof(VARDATA,Value), DT_DOUBLE, ATT_VALUE, 0, ATT_RW, "Value", "" };
	m_arrSysVariableAtts[VARDATA_RawValue] = { AIN_RawValue, offsetof(VARDATA,RawValue), DT_DOUBLE, ATT_RAW_VALUE, 0, ATT_RW, "RawValue", "" };
	m_arrSysVariableAtts[VARDATA_NegValue] = { VARDATA_NegValue, offsetof(VARDATA,NegValue), DT_DOUBLE, ATT_OLD_VALUE, 0, ATT_RW, "NegValue", "" };

	m_arrSysVariableAtts[VARDATA_CtrlByte] = { VARDATA_CtrlByte, offsetof(VARDATA,CtrlByte), DT_CHAR, ATT_RW, 0, ATT_RO, "CtrlByte", "" };
	m_arrSysVariableAtts[VARDATA_IsSOE] = { VARDATA_IsSOE, offsetof(VARDATA,IsSOE), DT_CHAR, ATT_RW, 0, ATT_RO, "IsSOE", "" };
	m_arrSysVariableAtts[VARDATA_StartCtrl] = { VARDATA_StartCtrl, offsetof(VARDATA,StartCtrl), DT_CHAR, ATT_START, 0, ATT_RO, "StartCtrl", "" };
	m_arrSysVariableAtts[VARDATA_SignalType] = { VARDATA_SignalType, offsetof(VARDATA,SignalType), DT_DWORD, ATT_NO, 0, ATT_RO, "SignalType", "" };

	m_arrSysVariableAtts[VARDATA_DataSource] = { VARDATA_DataSource, offsetof(VARDATA,DataSource), DT_DWORD, ATT_RW, 0, ATT_RO, "DataSource", "" };

	m_arrSysVariableAtts[VARDATA_Desc0OccNo] = { VARDATA_Desc0OccNo, offsetof(VARDATA,Desc0OccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "Desc0OccNo", "" };
	m_arrSysVariableAtts[VARDATA_Desc1OccNo] = { VARDATA_Desc1OccNo, offsetof(VARDATA,Desc1OccNo), DT_DWORD, ATT_DESCRIPTION, 0, ATT_RO, "Desc1OccNo", "" };

	m_arrSysVariableAtts[VARDATA_RangeL] = { VARDATA_RangeL, offsetof(VARDATA,RangeL), DT_DOUBLE, ATTR_MINRANGE, 0, ATT_RO, "RangeL", "" };
	m_arrSysVariableAtts[VARDATA_RangeH] = { VARDATA_RangeH, offsetof(VARDATA,RangeH), DT_DOUBLE, ATTR_MAXRANGE, 0, ATT_RO, "RangeH", "" };
	m_arrSysVariableAtts[VARDATA_HighQua] = { VARDATA_HighQua, offsetof(VARDATA,HighQua), DT_DOUBLE, ATT_HIQUA, 0, ATT_RO, "HighQua", "" };
	m_arrSysVariableAtts[VARDATA_LowQua] = { VARDATA_LowQua, offsetof(VARDATA,LowQua), DT_DOUBLE, ATT_LOQUA, 0, ATT_RO, "LowQua", "" };
	m_arrSysVariableAtts[VARDATA_MaxRaw] = { VARDATA_MaxRaw, offsetof(VARDATA,MaxRaw), DT_DOUBLE, ATT_MAXOUTPUT, 0, ATT_RO, "MaxRaw", "" };
	m_arrSysVariableAtts[VARDATA_MinRaw] = { VARDATA_MinRaw, offsetof(VARDATA,MinRaw), DT_DOUBLE, ATT_MINOUTPUT, 0, ATT_RO, "MinRaw", "" };

	m_arrSysVariableAtts[VARDATA_LastUpdateTime] = { VARDATA_LastUpdateTime, offsetof(VARDATA,LastUpdateTime), DT_STRING, ATT_RW, sizeof TIMEPAK, ATT_RO, "LastUpdateTime", "" };

}

CTagAttMgr::~CTagAttMgr()
{

}
/*! \fn int8u CTagAttMgr::GetAttDataType(int8u nIddType, const char * pszAttrName) const
********************************************************************************************************* 
** \brief CTagAttMgr::GetAttDataType 
** \details 根据基础对象的类型，属性名称，获取该属性的数据类型
** \param nIddType 基础对象的类型：比如IDD_DIN 、IDD_AIN....
** \param pszAttrName  属性名称，比如Value，Quality...  注意大小写敏感
** \return int16u 数据类型: DT_BOOLEAN\DT_DWORD....
** \author LiJin 
** \date 2016年10月14日 
** \note 
********************************************************************************************************/
int16u CTagAttMgr::GetAttDataType(int8u nIddType, const char * pszAttrName) const
{
	Q_ASSERT(pszAttrName);
	if (pszAttrName == nullptr)
		return DT_NULL;

	Q_ASSERT(strlen(pszAttrName) != 0);
	if (strlen(pszAttrName) == 0)
		return DT_NULL;

	switch (nIddType)
	{
	case IDD_NODE:
		for (auto i : m_arrNodeAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	case IDD_CHANNEL:
		{
			for (auto i:m_arrChannelAtts)
			{
				if (std::strcmp(pszAttrName,i.AttName) == 0 )
				{
					return i.DataType;
				}
			}
		}
		break;
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				return i.DataType;
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return DT_NULL;
}
/*! \fn int32u CTagAttMgr::GetAttIndex(int8u nIddType, const char* pszAttrName) const
********************************************************************************************************* 
** \brief CTagAttMgr::GetAttIndex 
** \details 
** \param nIddType 
** \param pszAttrName 
** \return int32u 
** \author xingzhibing
** \date 2017年2月12日 
** \note 
********************************************************************************************************/
bool  CTagAttMgr::GetAttIndex(int8u nIddType, const char* pszAttrName,int32u& nIndex) const
{
	Q_ASSERT(pszAttrName);
	if (pszAttrName == nullptr)
		return false;

	Q_ASSERT(strlen(pszAttrName) != 0);
	if (strlen(pszAttrName) == 0)
		return false;

	bool bRet = false;

	switch (nIddType)
	{
	case IDD_NODE:
		for (auto i : m_arrNodeAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
	}
	break;
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if (std::strcmp(pszAttrName, i.AttName) == 0)
			{
				nIndex = i.Att;
				bRet = true;
			}
		}
		break;
	}
	default:
		bRet = false;
		Q_ASSERT(false);
		break;
	}
	return bRet;
}
/*! \fn bool CTagAttMgr::GetTotalAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames) const
********************************************************************************************************* 
** \brief CTagAttMgr::GetTotalAttrs 
** \details 获取对象所有属性
** \param nIddType 
** \param arrAttrNames 
** \return bool 
** \author xingzhibing
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
bool CTagAttMgr::GetTotalAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames) const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
		case IDD_NODE:
		{	
			for (auto i : m_arrNodeAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}		
		case IDD_CHANNEL:
		{
			for (auto i : m_arrChannelAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_DEVICE:
		{
			for (auto i : m_arrDevAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_AIN:
		{
			for (auto i : m_arrAinAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_DIN:
		{
			for (auto i : m_arrDinAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_AOUT:
		{
			for (auto i : m_arrAoutAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_DOUT:
		{
			for (auto i : m_arrDoutAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_USERVAR:
		{
			for (auto i : m_arrUserVariableAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
		case IDD_SYSVAR:
		{
			for (auto i : m_arrSysVariableAtts)
			{
				arrAttrNames.push_back(i.AttName);
			}
			break;
		}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}
/*! \fn bool CTagAttMgr::GetTotalAttrs(int8u nIddType,int8u nFilter, std::vector<std::string> & arrAttrNames)const
********************************************************************************************************* 
** \brief CTagAttMgr::GetTotalAttrs 
** \details 获取 对象类型的特定属性名称
** \param nIddType 
** \param nFilter 
** \param arrAttrNames 
** \return bool 
** \author xingzhibing
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
bool CTagAttMgr::GetTotalAttrs(int8u nIddType,int8u nFilter, std::vector<std::string> & arrAttrNames)const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
	case IDD_NODE:
	{	
		for (auto i : m_arrNodeAtts)
		{
			if (i.Att==nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}			
		}
		break;
	}		
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if (i.Att == nFilter)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}
/*! \fn bool CTagAttMgr::GetTotalReadOnlyAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames)const
********************************************************************************************************* 
** \brief CTagAttMgr::GetTotalReadOnlyAttrs 
** \details 获取 对象类型的所有 只读 属性名称
** \param nIddType 
** \param arrAttrNames 
** \return bool 
** \author xingzhibing
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
bool CTagAttMgr::GetTotalReadOnlyAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames)const
{
	switch (nIddType)
	{
	case IDD_NODE:
	{
		for (auto i : m_arrNodeAtts)
		{
			if (i.RW==ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if (i.RW == ATT_RO)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}
/*! \fn bool CTagAttMgr::GetTotalReadOnlyAttrs(int8u nIddType, int8u nFilter, std::vector<std::string> & arrAttrNames)const
********************************************************************************************************* 
** \brief CTagAttMgr::GetTotalReadOnlyAttrs 
** \details  获取 对象类型的特定 只读 属性名称
** \param nIddType 
** \param nFilter 
** \param arrAttrNames 
** \return bool 
** \author xingzhibing
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
bool CTagAttMgr::GetTotalReadOnlyAttrs(int8u nIddType, int8u nFilter, std::vector<std::string> & arrAttrNames)const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
	case IDD_NODE:
	{
		for (auto i : m_arrNodeAtts)
		{
			if ((i.RW == ATT_RO)  && (i.Att==nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if ((i.RW == ATT_RO) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}

bool CTagAttMgr::GetTotalReadOnlyAttrsWithDataType(int8u nIddType, int8u nDataType, std::vector<std::string> & arrAttrNames)const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
	case IDD_NODE:
	{
		for (auto i : m_arrNodeAtts)
		{
			if ((i.RW == ATT_RO)  &&(i.DataType==nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if ((i.RW == ATT_RO)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if ((i.RW == ATT_RO) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if ((i.RW == ATT_RO)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if ((i.RW == ATT_RO) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if ((i.RW == ATT_RO) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if ((i.RW == ATT_RO) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if ((i.RW == ATT_RO) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if ((i.RW == ATT_RO) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}
/*! \fn bool CTagAttMgr::GetTotalWriteableAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames)
********************************************************************************************************* 
** \brief CTagAttMgr::GetTotalWriteableAttrs 
** \details 获取 对象类型的所有 可写 属性名称
** \param nIddType 
** \param arrAttrNames 
** \return bool 
** \author xingzhibing
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
bool CTagAttMgr::GetTotalWriteableAttrs(int8u nIddType, std::vector<std::string> & arrAttrNames)const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
	case IDD_NODE:
	{
		for (auto i : m_arrNodeAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if (i.RW == ATT_RW)
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}

bool CTagAttMgr::GetTotalWriteableAttrs(int8u nIddType, int8u nFilter, std::vector<std::string> & arrAttrNames)const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
	case IDD_NODE:
	{
		for (auto i : m_arrNodeAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att==nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if ((i.RW == ATT_RW) && (i.Att == nFilter))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}

bool CTagAttMgr::GetTotalWriteableAttrssWithDataType(int8u nIddType,  int8u nDataType, std::vector<std::string> & arrAttrNames)const
{
	arrAttrNames.clear();
	switch (nIddType)
	{
	case IDD_NODE:
	{
		for (auto i : m_arrNodeAtts)
		{
			if ((i.RW == ATT_RW)  &&(i.DataType==nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_CHANNEL:
	{
		for (auto i : m_arrChannelAtts)
		{
			if ((i.RW == ATT_RW)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DEVICE:
	{
		for (auto i : m_arrDevAtts)
		{
			if ((i.RW == ATT_RW)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AIN:
	{
		for (auto i : m_arrAinAtts)
		{
			if ((i.RW == ATT_RW) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DIN:
	{
		for (auto i : m_arrDinAtts)
		{
			if ((i.RW == ATT_RW)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_AOUT:
	{
		for (auto i : m_arrAoutAtts)
		{
			if ((i.RW == ATT_RW)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_DOUT:
	{
		for (auto i : m_arrDoutAtts)
		{
			if ((i.RW == ATT_RW)  && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_USERVAR:
	{
		for (auto i : m_arrUserVariableAtts)
		{
			if ((i.RW == ATT_RW) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	case IDD_SYSVAR:
	{
		for (auto i : m_arrSysVariableAtts)
		{
			if ((i.RW == ATT_RW) && (i.DataType == nDataType))
			{
				arrAttrNames.push_back(i.AttName);
			}
		}
		break;
	}
	default:
		Q_ASSERT(false);
		break;
	}
	return true;
}


/** @}*/