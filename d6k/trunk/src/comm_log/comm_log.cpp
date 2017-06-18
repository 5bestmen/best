
#include "log/comm_log.h"

#include "commlog_impl.h"

#include <vector>


COMM_LOGAPI bool CreateCommLog(const char* szLogName)
{
	return CCommLogImpl::GetInstance()->CreateLog(szLogName); 
}

COMM_LOGAPI bool DestroyCommLog(const char* szLogName)
{
	return CCommLogImpl::GetInstance()->DestroyLog(szLogName);
}

// 连接log服务器，使用者调用
COMM_LOGAPI bool OpenCommLog(const char* szLogName)
{
	return CCommLogImpl::GetInstance()->OpenLog(szLogName);
}

COMM_LOGAPI bool CloseCommLog(const char* szLogName)
{
	return CCommLogImpl::GetInstance()->CloseLog(szLogName);
}

/*! \fn COMM_LOGAPI bool CommMsgLogExt(const char *pszModuleName,  const char *pszProtocalName,const char *pMsg, int nMsgLen,
unsigned int nMsgType,const char * pMsgSubType, const char *pSource, const char *pDesc);
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
	const char * pMsgSubType, const char *pSource, const char *pDest, const char *pDescription, size_t Pid, size_t Tid, TIMEPAK * pTm)
{
	struct COMM_MSG MyLog;
	MyLog.ModuleName = const_cast<char*> (pszModuleName);
	MyLog.ProtocalName = const_cast<char*> (pszProtocalName);
	MyLog.MsgBuff = const_cast<unsigned char*> (pMsg);

	MyLog.MsgLen = nMsgLen;
	MyLog.MsgType = nMsgType;
	MyLog.MsgSubType = const_cast<char*> (pMsgSubType); 
	MyLog.Source = const_cast<char*> (pSource);
	MyLog.Dest = const_cast<char*> (pDest);
	MyLog.Description = const_cast<char*> (pDescription);
	MyLog.ProcessID = Pid;
	MyLog.ThreadID = Tid;
	MyLog.MsgTm = pTm;

	return CommMsgLog(&MyLog);
}


COMM_LOGAPI bool CommMsgLog(struct COMM_MSG *pMsgLog)
{
	return CCommLogImpl::GetInstance()->CommMsgLog(pMsgLog);
}