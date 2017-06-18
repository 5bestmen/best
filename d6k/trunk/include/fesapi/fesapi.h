
#ifndef _FES_API_H
#define _FES_API_H

#include "fesdatadef.h"
#include "../variant.h"

#include <QtCore/QtGlobal>

#if defined(FESAPI_LIBRARY)
#  define FESAPI Q_DECL_EXPORT
#else
#  define FESAPI Q_DECL_IMPORT
#endif


class IO_CLOCK;
//class FES_CMD;
extern "C"
{
	//IO库
	FESAPI bool CreateIO();
	FESAPI bool DestroyIO();

	// 供驱动程序调用
	FESAPI bool OpenIO(const char *pszDataPath, int32u nChannelOccNo,unsigned int nMode);
	FESAPI bool CloseIO(int32u nChannelOccNo);

	//系统变量
	//bool SetSYSValue(INT32U OccNo,VARIANT vValue,INT32U nQuality);
	
	//数据模型
	// 获取本机的节点号
	FESAPI bool GetMyNodeOccNoByHostName(const char *szMyHostName,int32u *pOccNo);
	// 获取某前置下的通道数量
	FESAPI bool GetChannelCount(int32u nNodeOccNo,int32u* pCount);
	// 获取某通道下装置的数量
	FESAPI bool GetDeviceCount(int32u nChannelOccNo,int32u* pCount);
	// 获取某通道下装置排行号列表
	FESAPI bool GetDeviceOccNos(int32u nChannelOccNo, int32u arrayDeviceOccNo[], int32u nArraySize);

	// 谨慎
	FESAPI bool GetChannelPtr(int32u nChannelOccNo, CHANNEL** ppFB);
	FESAPI bool GetDevicePtr(int32u nDeviceOccNo, DEVICE** ppFB);
	FESAPI bool GetAinPtr(int32u OccNo, AIN** ppFB);
	FESAPI bool GetDinPtr(int32u OccNo, DIN** ppFB);
	FESAPI bool GetAoutPtr(int32u OccNo, AOUT** ppFB);
	FESAPI bool GetDoutPtr(int32u OccNo, DOUT** ppFB);

	FESAPI bool GetChannel(int32u nChannelOccNo, CHANNEL*pChannel);
	FESAPI bool GetDevice(int32u nDeviceOccNo, DEVICE*pDevice);
	FESAPI bool GetAin(int32u OccNo, AIN*pAIN);
	FESAPI bool GetDin(int32u OccNo, DIN*pDIN);
	FESAPI bool GetAout(int32u OccNo, AOUT*pAout);
	FESAPI bool GetDout(int32u OccNo, DOUT*pDout);

	FESAPI bool GetAinAlarm(int32u OccNo, AIN_ALARM *pAinAlarm);
	FESAPI bool GetDinAlarm(int32u OccNo, DIN_ALARM *pDinAlarm);
	FESAPI bool GetAinAlarmLimit(int32u OccNo, AIN_ALARM_LIMIT *pAinAlarmLimit);
	FESAPI bool GetDinAlarmLimit(int32u OccNo, DIN_ALARM_LIMIT *pDinAlarmLimit);

	// 获取通道下 测点数量
	FESAPI bool GetAinCountInChannel(int32u nChannelOccNo, int32u* pCount);
	FESAPI bool GetDinCountInChannel(int32u nChannelOccNo, int32u* pCount);
	FESAPI bool GetAoutCountInChannel(int32u nChannelOccNo, int32u* pCount);
	FESAPI bool GetDoutCountInChannel(int32u nChannelOccNo, int32u* pCount);

	FESAPI bool GetAinOccNosInChannel(int32u nChannelOccNo, int32u arrayAinOccNo[], int32u nArraySize);
	FESAPI bool GetDinOccNosInChannel(int32u nChannelOccNo, int32u arrayDinOccNo[], int32u nArraySize);
	FESAPI bool GetAoutOccNosInChannel(int32u nChannelOccNo, int32u arrayAoutOccNo[], int32u nArraySize);
	FESAPI bool GetDoutOccNosInChannel(int32u nChannelOccNo, int32u arrayDoutsOccNo[], int32u nArraySize);

	// 获取装置下 测点数量
	FESAPI bool GetAinCountInDevice(int32u nDeviceOccNo, int32u* pCount);
	FESAPI bool GetDinCountInDevice(int32u nDeviceOccNo, int32u* pCount);
	FESAPI bool GetAoutCountInDevice(int32u nDeviceOccNo, int32u* pCount);
	FESAPI bool GetDoutCountInDevice(int32u nDeviceOccNo, int32u* pCount);

	FESAPI bool GetAinOccNosInDevice(int32u nDeviceOccNo, int32u arrayAinOccNo[], int32u nArraySize);
	FESAPI bool GetDinOccNosInDevice(int32u nDeviceOccNo, int32u arrayDinOccNo[], int32u nArraySize);
	FESAPI bool GetAoutOccNosInDevice(int32u nDeviceOccNo, int32u arrayAoutOccNo[], int32u nArraySize);
	FESAPI bool GetDoutOccNosInDevice(int32u nDeviceOccNo, int32u arrayDoutsOccNo[], int32u nArraySize);

	FESAPI char* GetAinAddress(int32u nAinOccNo);
	FESAPI char* GetDinAddress(int32u nDinOccNo);

	FESAPI char* GetAoutAddress(int32u nOccNo);
	FESAPI char* GetDoutAddress(int32u nOccNo);


	//FESAPI bool GetSYS(int32u OccNo，SYS** ppSYS);
	
	//IO设值
	FESAPI bool IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality);
	FESAPI bool IoSetAinValueEx(int32u nOccNo, fp64 fValue, int8u nQuality,TIMEPAK * pTm);

	FESAPI bool IoGetAinValue(int32u nOccNo, fp64 *pValue, int8u *pQua);
	
	FESAPI bool IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality);
	// cpu_eanble true 由FES计算并报警变位，false，不产生告警信息，告警信息由装置单独上送，单独调用下一个接口
	FESAPI bool IoSetDinValueEx(int32u nOccNo, int8u nValue, int8u nQuality, bool bCpuEnable, TIMEPAK * pTm);
	// 状变告警（包含SOE），只做告警，不写库？
	FESAPI bool IoDinAlarm(int32u nOccNo, int8u nValue, int8u nQuality, TIMEPAK * pTm);

	FESAPI bool IoGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua);

	FESAPI bool IoSetAoutValue(int32u nOccNo, fp64 fValue, int8u nQuality);
	FESAPI bool IoSetDoutValue(int32u nOccNo, int8u nValue, int8u nQuality);

	// 心跳
	FESAPI void IoSetDeviceHeartBeat(int32u nOccNo);
	FESAPI void IoSetChannelHeartBeat(int32u nOccNo);

	// 设置装置的品质，如果为坏，则该装置下所有测点品质为坏，如果为好，则...
	FESAPI bool IoSetDeviceQua(int32u nDeviceOccNo, int8u nQuality);
	FESAPI bool IoSetChannelQua(int32u nChannelOccNo, int8u nQuality);

	//驱动管理接口
	FESAPI bool IsDriverQuit(int32u nChannelOccNo);
	FESAPI bool LoadDrivers();
	FESAPI bool UnLoadDrivers();

	//打开本驱动的邮箱系统
	//FESAPI bool OpenChannelMailBox(int32u nChannelOccNo);

	FESAPI bool IsFesAlive();

	// 本节点的工作状态
	FESAPI int32u GetMyHostState();

	//FESAPI bool IsFesQuit();
	
	// 驱动程序调用的报警接口
	// 自诊断
	FESAPI void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// 操作报警
	FESAPI void IoOperAlarm(int32u nChannleNo, int32u nDeviceNo, const char * pszAlarmTxt);
	// 保护的自诊断
	FESAPI void IoRelayDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// 通用的告警
	FESAPI void IoAlarmMsg(int32u nChannleNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);

	//用户消息接口 用于遥控、调节、保护定值类以及内部调试等的消息传递，在驱动程序中定时调用，检查是否有控制命令等下发
	FESAPI bool ReadHostCmd(INT32U nChannleNo, SETVAL_MSG *pCmd, INT32U nTimeout);


	// 获取用户变量的测值
	FESAPI bool AppGetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua);
	// 获取系统变量的测值
	FESAPI bool AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua);
	// 应用层修改用户变量的测值
	FESAPI bool AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u  nQua);
	// 驱动层修改用户变量的测值
	FESAPI bool IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality);
	// 驱动层修改系统变量的测值
	FESAPI bool IoSetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality);

	//应用接口
	//用户用户获取对应描述字段 可进行告警 界面显示区分
	FESAPI bool GetDescByOccNo(int32u nOccNo , bool bIsTranslate , char* pBuff );
	//FESAPI bool GetOccNoByTagName(int8s* strTagName,int32u* pOccNo);
//	FESAPI bool GetOccNoByTagName(int32u nDataType, int8s* strTagName, int32u* pOccNo);
	FESAPI int32u GetOccNoByTagName(const char * szTagName, int32u *pDataType);
	FESAPI bool AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua);
	FESAPI bool AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua);
	//FESAPI bool GetSysValue(INT32U nOccNo,VARIANT* pValue);
	// 只对虚拟点有效
	FESAPI bool AppSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality);

	// 设开出
	FESAPI bool AppSetDoutValue(int32u nOccNo, int8u Value, int8u nSource);
	// 设值-给远程节点的开出设值
	FESAPI bool AppSetDoutValueEx(int32u nDestNode, int32u nOccNo, int8u Value, void *pExtra);
	// 写值
	FESAPI bool AppSetAoutValue(int32u nOccNo, fp64 Value, int32u nSource);


	// 通用接口
	// 获取基础对象的属性值
	FESAPI bool GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData);
	FESAPI bool PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, int32u nParam, IO_VARIANT *pData,const char * szOperatorName,const char * szMonitorName, void *pExt, bool bOpLog);


}


#endif // _FES_API_H

