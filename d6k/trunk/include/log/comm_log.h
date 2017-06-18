/*! @file comm_log.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  comm_log.h
�ļ�ʵ�ֹ��� :  ���ļ�¼ģ��ӿڶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   LOG���ļ�¼ģ��ӿڶ���
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

#define  CMSG_SERIALPORT_SEND   1  //! ���ڷ���
#define	 CMSG_SERALPORT_RECV    2  //! ���ڽ���
#define	 CMSG_UDP_SEND          3  //! UDP����
#define	 CMSG_UDP_RECV          4  //! UDP����
#define	 CMSG_TCP_SEND          5  //! TCP����
#define	 CMSG_TCP_RECV          6  //! TCP����
#define  CMSG_UNKOWN            7  //! ����

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
	// ����log��������ʹ���ߵ���
	COMM_LOGAPI bool OpenCommLog(const char* szLogName);
	COMM_LOGAPI bool CloseCommLog(const char* szLogName);

	/*! \fn COMM_LOGAPI bool CommMsgLogExt(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType,
		const char * pMsgSubType, const char *pSource, const char *pDesc);
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
		const char * pMsgSubType, const char *pSource, const char *pDest, const char *pDescription, size_t Pid, size_t Tid, TIMEPAK * pTm);

	COMM_LOGAPI bool CommMsgLog(struct COMM_MSG *pMsgLog);
}


#endif // COMM_LOG_API_H

/** @}*/
