
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
	//IO��
	FESAPI bool CreateIO();
	FESAPI bool DestroyIO();

	// �������������
	FESAPI bool OpenIO(const char *pszDataPath, int32u nChannelOccNo,unsigned int nMode);
	FESAPI bool CloseIO(int32u nChannelOccNo);

	//ϵͳ����
	//bool SetSYSValue(INT32U OccNo,VARIANT vValue,INT32U nQuality);
	
	//����ģ��
	// ��ȡ�����Ľڵ��
	FESAPI bool GetMyNodeOccNoByHostName(const char *szMyHostName,int32u *pOccNo);
	// ��ȡĳǰ���µ�ͨ������
	FESAPI bool GetChannelCount(int32u nNodeOccNo,int32u* pCount);
	// ��ȡĳͨ����װ�õ�����
	FESAPI bool GetDeviceCount(int32u nChannelOccNo,int32u* pCount);
	// ��ȡĳͨ����װ�����к��б�
	FESAPI bool GetDeviceOccNos(int32u nChannelOccNo, int32u arrayDeviceOccNo[], int32u nArraySize);

	// ����
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

	// ��ȡͨ���� �������
	FESAPI bool GetAinCountInChannel(int32u nChannelOccNo, int32u* pCount);
	FESAPI bool GetDinCountInChannel(int32u nChannelOccNo, int32u* pCount);
	FESAPI bool GetAoutCountInChannel(int32u nChannelOccNo, int32u* pCount);
	FESAPI bool GetDoutCountInChannel(int32u nChannelOccNo, int32u* pCount);

	FESAPI bool GetAinOccNosInChannel(int32u nChannelOccNo, int32u arrayAinOccNo[], int32u nArraySize);
	FESAPI bool GetDinOccNosInChannel(int32u nChannelOccNo, int32u arrayDinOccNo[], int32u nArraySize);
	FESAPI bool GetAoutOccNosInChannel(int32u nChannelOccNo, int32u arrayAoutOccNo[], int32u nArraySize);
	FESAPI bool GetDoutOccNosInChannel(int32u nChannelOccNo, int32u arrayDoutsOccNo[], int32u nArraySize);

	// ��ȡװ���� �������
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


	//FESAPI bool GetSYS(int32u OccNo��SYS** ppSYS);
	
	//IO��ֵ
	FESAPI bool IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality);
	FESAPI bool IoSetAinValueEx(int32u nOccNo, fp64 fValue, int8u nQuality,TIMEPAK * pTm);

	FESAPI bool IoGetAinValue(int32u nOccNo, fp64 *pValue, int8u *pQua);
	
	FESAPI bool IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality);
	// cpu_eanble true ��FES���㲢������λ��false���������澯��Ϣ���澯��Ϣ��װ�õ������ͣ�����������һ���ӿ�
	FESAPI bool IoSetDinValueEx(int32u nOccNo, int8u nValue, int8u nQuality, bool bCpuEnable, TIMEPAK * pTm);
	// ״��澯������SOE����ֻ���澯����д�⣿
	FESAPI bool IoDinAlarm(int32u nOccNo, int8u nValue, int8u nQuality, TIMEPAK * pTm);

	FESAPI bool IoGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua);

	FESAPI bool IoSetAoutValue(int32u nOccNo, fp64 fValue, int8u nQuality);
	FESAPI bool IoSetDoutValue(int32u nOccNo, int8u nValue, int8u nQuality);

	// ����
	FESAPI void IoSetDeviceHeartBeat(int32u nOccNo);
	FESAPI void IoSetChannelHeartBeat(int32u nOccNo);

	// ����װ�õ�Ʒ�ʣ����Ϊ�������װ�������в��Ʒ��Ϊ�������Ϊ�ã���...
	FESAPI bool IoSetDeviceQua(int32u nDeviceOccNo, int8u nQuality);
	FESAPI bool IoSetChannelQua(int32u nChannelOccNo, int8u nQuality);

	//��������ӿ�
	FESAPI bool IsDriverQuit(int32u nChannelOccNo);
	FESAPI bool LoadDrivers();
	FESAPI bool UnLoadDrivers();

	//�򿪱�����������ϵͳ
	//FESAPI bool OpenChannelMailBox(int32u nChannelOccNo);

	FESAPI bool IsFesAlive();

	// ���ڵ�Ĺ���״̬
	FESAPI int32u GetMyHostState();

	//FESAPI bool IsFesQuit();
	
	// ����������õı����ӿ�
	// �����
	FESAPI void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// ��������
	FESAPI void IoOperAlarm(int32u nChannleNo, int32u nDeviceNo, const char * pszAlarmTxt);
	// �����������
	FESAPI void IoRelayDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// ͨ�õĸ澯
	FESAPI void IoAlarmMsg(int32u nChannleNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);

	//�û���Ϣ�ӿ� ����ң�ء����ڡ�������ֵ���Լ��ڲ����Եȵ���Ϣ���ݣ������������ж�ʱ���ã�����Ƿ��п���������·�
	FESAPI bool ReadHostCmd(INT32U nChannleNo, SETVAL_MSG *pCmd, INT32U nTimeout);


	// ��ȡ�û������Ĳ�ֵ
	FESAPI bool AppGetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua);
	// ��ȡϵͳ�����Ĳ�ֵ
	FESAPI bool AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua);
	// Ӧ�ò��޸��û������Ĳ�ֵ
	FESAPI bool AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u  nQua);
	// �������޸��û������Ĳ�ֵ
	FESAPI bool IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality);
	// �������޸�ϵͳ�����Ĳ�ֵ
	FESAPI bool IoSetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality);

	//Ӧ�ýӿ�
	//�û��û���ȡ��Ӧ�����ֶ� �ɽ��и澯 ������ʾ����
	FESAPI bool GetDescByOccNo(int32u nOccNo , bool bIsTranslate , char* pBuff );
	//FESAPI bool GetOccNoByTagName(int8s* strTagName,int32u* pOccNo);
//	FESAPI bool GetOccNoByTagName(int32u nDataType, int8s* strTagName, int32u* pOccNo);
	FESAPI int32u GetOccNoByTagName(const char * szTagName, int32u *pDataType);
	FESAPI bool AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua);
	FESAPI bool AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua);
	//FESAPI bool GetSysValue(INT32U nOccNo,VARIANT* pValue);
	// ֻ���������Ч
	FESAPI bool AppSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality);

	// �迪��
	FESAPI bool AppSetDoutValue(int32u nOccNo, int8u Value, int8u nSource);
	// ��ֵ-��Զ�̽ڵ�Ŀ�����ֵ
	FESAPI bool AppSetDoutValueEx(int32u nDestNode, int32u nOccNo, int8u Value, void *pExtra);
	// дֵ
	FESAPI bool AppSetAoutValue(int32u nOccNo, fp64 Value, int32u nSource);


	// ͨ�ýӿ�
	// ��ȡ�������������ֵ
	FESAPI bool GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData);
	FESAPI bool PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, int32u nParam, IO_VARIANT *pData,const char * szOperatorName,const char * szMonitorName, void *pExt, bool bOpLog);


}


#endif // _FES_API_H

