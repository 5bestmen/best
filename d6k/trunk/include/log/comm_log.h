/*! @file comm_log.h
<PRE>
********************************************************************************
模块名       :
文件名       :  comm_log.h
文件实现功能 :  报文记录模块接口定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   LOG报文记录模块接口定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.13
*******************************************************************************/

#ifndef COMM_LOG_API_H
#define COMM_LOG_API_H

#include <QtCore/QtGlobal>
 
#ifdef COMMLOG_LIB
# define COMM_LOGAPI Q_DECL_EXPORT
#else
# define COMM_LOGAPI Q_DECL_IMPORT
#endif

#define  CMSG_SERIALPORT_SEND   1  //! 串口发送
#define	 CMSG_SERALPORT_RECV    2  //! 串口接收
#define	 CMSG_UDP_SEND          3  //! UDP发送
#define	 CMSG_UDP_RECV          4  //! UDP接收
#define	 CMSG_TCP_SEND          5  //! TCP发送
#define	 CMSG_TCP_RECV          6  //! TCP接收
#define  CMSG_UNKOWN            7  //! 其他

#define  MAX_MSG_LENGTH  4096

extern "C"
{
	struct TIMEPAK;
	struct COMM_MSG
	{
		char * ModuleName;
		char * ProtocalName;
		unsigned char * MsgBuff;

		unsigned int MsgLen;
		unsigned int MsgType;
		char   * MsgSubType;
		char   * Source;
		char   * Dest;
		char   * Description;
		size_t  ProcessID;
		size_t  ThreadID;
		TIMEPAK *MsgTm;
	};

	COMM_LOGAPI bool CreateCommLog(const char* szLogName);
	COMM_LOGAPI bool DestroyCommLog(const char* szLogName);
	// 连接log服务器，使用者调用
	COMM_LOGAPI bool OpenCommLog(const char* szLogName);
	COMM_LOGAPI bool CloseCommLog(const char* szLogName);

	/*! \fn COMM_LOGAPI bool CommMsgLogExt(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType,
		const char * pMsgSubType, const char *pSource, const char *pDesc);
	********************************************************************************************************* 
	** \brief CommMsgLogExt 
	** \details 报文记录
	** \param pszModuleName  模块名称
	** \param pszProtocalName  协议名称，可以为空，  用于dbgstuidio进行协议解释显示用
	** \param pMsg 报文内容
	** \param nMsgLen 报文长度
	** \param nMsgType  报文类型 见上宏定义
	** \param pMsgSubType  子类型 字符串描述的类型说明，如启动帧、召唤、或者RECV、SEND，由使用者自由使用
	** \param pSource 源   （如果是以太网，则填IP:端口号，如果是串口，则填本机的串口号）
	** \param pDest   目的 （如果是以太网，则填IP:端口号，如果是串口，则填本机的串口号）
	** \return COMM_LOGAPI bool 
	** \author LiJin 
	** \date 2016年9月13日 
	** \note 
	********************************************************************************************************/
	COMM_LOGAPI bool CommMsgLogExt(const char *pszModuleName, const char *pszProtocalName, const unsigned char *pMsg, unsigned int nMsgLen, unsigned int nMsgType,
		const char * pMsgSubType, const char *pSource, const char *pDest, const char *pDescription, size_t Pid, size_t Tid, TIMEPAK * pTm);

	COMM_LOGAPI bool CommMsgLog(struct COMM_MSG *pMsgLog);
}


#endif // COMM_LOG_API_H

/** @}*/
