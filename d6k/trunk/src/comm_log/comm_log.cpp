
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

// ����log��������ʹ���ߵ���
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
** \details ���ļ�¼
** \param pszModuleName  ģ������
** \param pszProtocalName  Э�����ƣ�����Ϊ�գ�  ����dbgstuidio����Э�������ʾ��
** \param pMsg ��������
** \param nMsgLen ���ĳ���
** \param nMsgType  �������� ���Ϻ궨��
** \param pMsgSubType  ������ �ַ�������������˵����������֡���ٻ�������RECV��SEND����ʹ��������ʹ��
** \param pSource Դ   ���������̫��������IP:�˿ںţ�����Ǵ��ڣ�������Ĵ��ںţ�
** \param pDest   Ŀ�� ���������̫��������IP:�˿ںţ�����Ǵ��ڣ�������Ĵ��ںţ�
** \return COMM_LOGAPI bool
** \author LiJin
** \date 2016��9��13��
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