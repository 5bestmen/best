
#ifndef SCADA_API_H
#define SCADA_API_H
 
#include "datatypes.h"
#include "../variant.h"

#include <QtCore/QtGlobal>

#ifdef SCADA_SDK_LIB
# define SCADA_API Q_DECL_EXPORT
#else
# define SCADA_API Q_DECL_IMPORT
#endif
 


extern "C"
{
	struct TIMEPAK;
	// ����SCADA ��
	SCADA_API bool ConnectScada(const char * pszProjectName, const char * szAppName);
	// �ر�
	SCADA_API bool DisconnectScada(const char * pszProjectName, const char * szAppName);
	// �ж�SCADA�����Ƿ��˳�
	SCADA_API bool IsScadaExit(unsigned int nTimeout);
	
	SCADA_API void SetScadaExit(bool bExit);
	// ȡ����¼
	SCADA_API void CancelLogin();
	SCADA_API bool ShowLoginDialog();

	// ���ͱ����¼�
	SCADA_API bool AppSendAlarmMsg(const char *pszAppName, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);

	//SCADA_API unsigned int GetTagID(const char *szTagName);

	SCADA_API bool GetOccNoByTagName(const char*pszTagName, int32u *pNodeOccNo, int32u *pIddType, int32u *pOccNo, int32u *pFiledID);
	
	// ������͡��ڵ�����������������������
	SCADA_API bool GetRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData);
	SCADA_API bool PutRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt,void *pSrc);

	// APP��ʱ1����������ź�
	SCADA_API void SetAppHeatBeat(int32u nOccNo );
	SCADA_API void SetAppHeatBeatByTagName(const char *pszTagName);
// 	struct HotCacheItemInfo
// 	{
// 		int32u NodeOccNo;  //! �ڵ�
// 		int32u IddType;    //! ������� 
// 		int32u TagOccNo;   //! �������
// 		int32u FiledID;    //! �������
// 	};
	struct TAGITEM;
	// ע�ᾲ̬����
	SCADA_API bool RegisterRTData(struct TAGITEM *pItems,int32u nNum);
	// ȡ����̬����
	SCADA_API bool UnregisterRTData(struct TAGITEM *pItems, int32u nNum);
}


#endif // SCADA_API_H

