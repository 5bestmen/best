/*! @file
<PRE>
********************************************************************************
模块名       :  内部网络总线操作接口定义
文件名       :
文件实现功能 :
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author  LiJin
*  @version 1.0
*  @date    2015.09.1
*/
#ifndef NETBUS_API_H
#define NETBUS_API_H
 

#include <QtCore/QtGlobal>

#ifdef NBAPI_LIB
# define NBAPI Q_DECL_EXPORT
#else
# define NBAPI Q_DECL_IMPORT
#endif

#include <netbus/nbdef.h> 
#include "../datatypes.h"

extern "C"
{
	/*! \fn NBAPI bool StartNetBus(NET_CONFIG * pConf, const char * pLogStationName, int32u nNodeType, int32u nStartMode);
	********************************************************************************************************* 
	** \brief StartNetBus 
	** \details 启动网络总线
	** \param pConf 系统节点配置信息
	** \param pszPredicateName 调用者名称，也做LOG记录前缀（此处一般输入："FES"或者"SCADA"），
	** \param pszProjPath  项目路径 
	** \param nNodeType : NODE_SVR    NODE_FES  	NODE_CLIENT 
	** \param nStartMode  启动模式
	** \return NBAPI bool 
	** \author LiJin 
	** \date 2016年10月22日 
	** \note 
	********************************************************************************************************/
	NBAPI bool StartNetBus(NET_CONFIG * pConf, const char *pszProjPath, const char * pszPredicateName, int32u nNodeType, int32u nStartMode);
	NBAPI void StopNetBus(const char * pszPredicateName);

	// 动态增加系统客户端节点
	NBAPI void AddNode(NET_CONFIG * pConf);
	   
	// 发送接口
	NBAPI int NBSendData(const char * pszPredicateName, const int8u * pMsg, int32u nMsgLen);

	// 接收接口，
	NBAPI bool NBRecvData(const char * pszPredicateName, const int8u * pMsg, int32u nMsgLen, int32u *pCopySize, int32u ulTimeout);
	// 手动切换
	NBAPI bool ManualSwitchNode(char *pszPredicateName);
}



/** @}*/
#endif // NETBUS_API_H
