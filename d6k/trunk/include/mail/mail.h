/*! @file mail.h
<PRE>
********************************************************************************
ģ����       :  �ʼ�ģ��
�ļ���       :  mail.h
�ļ�ʵ�ֹ��� :  �ʼ�ģ��ӿڶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ʼ�ģ��ӿڶ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.15
*******************************************************************************/

#ifndef _MAIL_API_H
#define _MAIL_API_H

#include "../datatypes.h"
#include <QtCore/QtGlobal>

#ifdef MAIL_LIB
# define MAILAPI Q_DECL_EXPORT
#else
# define MAILAPI Q_DECL_IMPORT
#endif
 

extern "C"
{
	enum
	{
		INVALID_MAILBOX_ID = 0,
		MAIL_MAX_SIZE = 1024,	//ÿ���ʼ�����ֽ���
	};

	enum EVT_TYPE
	{
		MSG_EVT0 = 0X00,  //!< ������
		MSG_EVT_ANA = 0X01,  //!< ģ����Խ���޸澯
		MSG_EVT_DIGI = 0X02,  //!< ������
		MSG_EVT_DIAG = 0X04,  //!< �������澯
		MSG_EVT_OPER = 0X08,  //!< ������澯
		MSG_EVT_SETVAL = 0X10,  //!< ��ֵ
		MSG_EVT6 = 0X20,        //!< SOE�¼�
		MSG_EVT7 = 0X40,        //!< ң���¼�
		MSG_EVT8 = 0X80,
		MSG_EVT9 = 0X0100,
		MSG_EVT10 = 0X0200,
		MSG_EVT11 = 0X0400,
		MSG_EVT12 = 0X0800,
		MSG_EVT13 = 0X1000,
		MSG_EVT14 = 0X2000,
		MSG_EVT15 = 0X4000,
		MSG_EVT16 = 0X8000,
		MSG_EVT_ALL = MSG_EVT_ANA | MSG_EVT_DIGI
		| MSG_EVT_DIAG | MSG_EVT_OPER
		| MSG_EVT_SETVAL | MSG_EVT6
		| MSG_EVT7 | MSG_EVT8
		| MSG_EVT9 | MSG_EVT10
		| MSG_EVT11 | MSG_EVT12
		| MSG_EVT13 | MSG_EVT14
		| MSG_EVT15 | MSG_EVT16,
		MSG_EVT_NUM = 16,  //!< 16����Ϣ����
	};

	struct DMSG
	{
		int32u   SenderID;    //! ������ID
		int32u   RecverID;    //! ������ID
		int32u   SubID;       //! ������ID
		int32u   Type;        //! ��Ϣ����
		int32u	 Size;		  //! �ʼ�����
		int8u	 Buff[MAIL_MAX_SIZE];  //! �ʼ�����
	}; 
	/*! \fn MAILAPI bool CreatePostOffice(const char*pszPredicate, size_t nMailBoxNum)
	********************************************************************************************************* 
	** \brief CreatePostOffice 
	** \details ������������֮�������ʾ�
	** \param pszPredicate ǰ׺�����磺SCADA����FES
	** \param nMailBoxNum �����������ͨ������������+ת������+ϵͳ����
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016��10��8�� 
	** \note ��ͬ��ǰ׺֮�����޷�����ͨ�ŵģ��ʼ���ͨ��ֻ���ڸ��ʾ��ڽ��С�����˵��SCADA���ʼ�ֻ����SCADA��
	��ת��FESֻ����ǰ���ڲ��ַ���FES��SCADA֮�䲻�ܽ����ʼ�����ת��
	********************************************************************************************************/
	MAILAPI bool CreatePostOffice(const char*pszPredicate, size_t nMailBoxNum);
	MAILAPI void DestroyPostOffice(const char *pszPredicate);	 
	/*! \fn MAILAPI bool CreateMailBox(const char * pszPredicate, const char *szMailBoxName, int nNum, int nSize,int *pID)
	********************************************************************************************************* 
	** \brief CreateMailBox 
	** \details �������䣬
	** \param pszPredicate ǰ׺�����磺SCADA����FES
	** \param szMailBoxName ϵͳ������ʱ����ÿ�������tagname������������ת������tagname��Ϊ��������.
	** \param nNum ���仺������
	** \param nSize ÿ���ʼ����ߴ�
	** \param pID ����ֵ�������ID
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016��10��8�� 
	** \note 
	********************************************************************************************************/
	MAILAPI bool CreateMailBox(const char * pszPredicate, const char *szMailBoxName, int nNum,int *pID);
	MAILAPI bool DestroyMailBox(const char *pszPredicate, const char *szMailBoxName);

	// ���ʼ���ֻ�н�����ģ�����Ҫ���ã�������ǽ����⣬���Բ�����
	MAILAPI bool OpenPostOffice(const char*pszPredicate);
	// �ر��ʼ�
	MAILAPI void ClosePostOffice(const char *pszPredicate);

	// �ɿͻ�������ã������� ��ͨ����fesapi.dll\scadaapi.dll ����߼������η�װ
	MAILAPI bool OpenMailBox(const char *pszPredicate, const char *szMailBoxName, int *pID);
	//����Ѿ���ȡmailID������ϵͳ ֱ�Ӵ�mailbox
	MAILAPI bool OpenMailBoxByID(const char* pszPredicate,int nID);

	MAILAPI bool CloseMailBox(const char *pszPredicate, const char *szMailBoxName);
	//����Ѿ���ȡmailID������ϵͳ ֱ�ӹر�mailbox
	MAILAPI bool CloseMailBoxByID(const char* pszPredicate, int nID);
	/*! \fn MAILAPI bool SubscribeMail(const char *pszPredicate, const char *szSelfMailBoxName, int32u nType)
	********************************************************************************************************* 
	** \brief SubscribeMail 
	** \details ĳ�������û�����ĳ�����͵��ʼ�
	** \param pszPredicate  ǰ׺�����磺SCADA����FES
	** \param szSelfMailBoxName �����ߵ���������
	** \param nType �������ͣ�ע���ǰ�λ����
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016��10��8�� 
	** \note ĳ�����䶩����ĳ�����͵��ʼ��󣬸����͵��ʼ����Զ����͵������ߵ�������
	********************************************************************************************************/
	MAILAPI bool SubscribeMail(const char *pszPredicate, const char *szSelfMailBoxName, int32u nType);

	// ÿ�����䶼��һ��ID�����ID��ϵͳ����֮�������
	MAILAPI int QueryMailBoxID(const char * pszPredicate, const char * szMailBoxName);
  
	MAILAPI bool SendRawMail(const char *pszPredicate, const char * szSendMailBoxName, const char * szRecvMailBoxName, const unsigned char* pMail, int nLength, unsigned int nWaitTimeMs);
	MAILAPI bool SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTimeMs);

//	MAILAPI bool RecvMail(const char *pszPredicate,int nRecvMailBoxID, unsigned char* pMail, int nLength, int nWaitTime = 10);
	MAILAPI bool RecvMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTimeMs);
}


#endif // _MAIL_API_H

/** @}*/

