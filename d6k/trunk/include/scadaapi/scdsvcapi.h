
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
	// 连接SCADA 库
	SCADA_API bool ConnectScada(const char * pszProjectName, const char * szAppName);
	// 关闭
	SCADA_API bool DisconnectScada(const char * pszProjectName, const char * szAppName);
	// 判断SCADA服务是否退出
	SCADA_API bool IsScadaExit(unsigned int nTimeout);
	
	SCADA_API void SetScadaExit(bool bExit);
	// 取消登录
	SCADA_API void CancelLogin();
	SCADA_API bool ShowLoginDialog();

	// 发送报警事件
	SCADA_API bool AppSendAlarmMsg(const char *pszAppName, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);

	//SCADA_API unsigned int GetTagID(const char *szTagName);

	SCADA_API bool GetOccNoByTagName(const char*pszTagName, int32u *pNodeOccNo, int32u *pIddType, int32u *pOccNo, int32u *pFiledID);
	
	// 测点类型、节点索引、测点索引、测点属性
	SCADA_API bool GetRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData);
	SCADA_API bool PutRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt,void *pSrc);

	// APP定时1秒更新心跳信号
	SCADA_API void SetAppHeatBeat(int32u nOccNo );
	SCADA_API void SetAppHeatBeatByTagName(const char *pszTagName);
// 	struct HotCacheItemInfo
// 	{
// 		int32u NodeOccNo;  //! 节点
// 		int32u IddType;    //! 测点类型 
// 		int32u TagOccNo;   //! 测点排行
// 		int32u FiledID;    //! 测点属性
// 	};
	struct TAGITEM;
	// 注册静态缓存
	SCADA_API bool RegisterRTData(struct TAGITEM *pItems,int32u nNum);
	// 取消静态缓存
	SCADA_API bool UnregisterRTData(struct TAGITEM *pItems, int32u nNum);
}


#endif // SCADA_API_H

