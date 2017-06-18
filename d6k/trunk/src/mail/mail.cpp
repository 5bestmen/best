

#include "mail/mail.h"
#include "mailimpl.h"

#include <QDebug>

extern "C"
{
	MAILAPI bool CreatePostOffice(const char*pszPredicate, size_t nMailBoxNum)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;
		return g_MailImpl.CreatePostOffice(pszPredicate, nMailBoxNum);
	}

	MAILAPI void DestroyPostOffice(const char *pszPredicate)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return ;
		return g_MailImpl.DestroyPostOffice(pszPredicate);
	}

	MAILAPI bool CreateMailBox(const char * pszPredicate, const char *pszMailBoxName, int nNum, int *pID)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		Q_ASSERT(pszMailBoxName);
		if (pszMailBoxName == nullptr || strlen(pszMailBoxName) <= 0)
			return false;

		int nID = g_MailImpl.RegisterMailBox(pszPredicate, pszMailBoxName);
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;

		if (pID)
		{
			*pID = nID;
		}

		int nSizeLetter = sizeof LETTER;

		return g_MailImpl.CreateMailBox(pszPredicate, nID, nNum, nSizeLetter);
	}

 	MAILAPI bool DestroyMailBox(const char *pszPredicate, const char *pszMailBoxName)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		Q_ASSERT(pszMailBoxName);
		if (pszMailBoxName == nullptr || strlen(pszMailBoxName) <= 0)
			return false;

		int nID = g_MailImpl.QueryMailBoxID(pszPredicate, pszMailBoxName);
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;

		return g_MailImpl.DestroyMailBox(pszPredicate, nID);
	}

	MAILAPI bool OpenMailBox(const char *pszPredicate, const char *pszMailBoxName, int *pID)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		Q_ASSERT(pszMailBoxName);
		if (pszMailBoxName == nullptr || strlen(pszMailBoxName) <= 0)
			return false;
	
		int nID = g_MailImpl.QueryMailBoxID(pszPredicate, pszMailBoxName);
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;

		if (pID)
		{
			*pID = nID;
		}

		return g_MailImpl.OpenMailBox(pszPredicate, nID );
	}

	MAILAPI bool OpenMailBoxByID(const char* pszPredicate, int nID)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;
	
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;

		return g_MailImpl.OpenMailBox(pszPredicate, nID);
	}

	MAILAPI bool CloseMailBoxByID(const char* pszPredicate, int nID)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;
		return g_MailImpl.CloseMailBox(pszPredicate, nID);
	}


	MAILAPI bool CloseMailBox(const char *pszPredicate, const char *pszMailBoxName)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		Q_ASSERT(pszMailBoxName);
		if (pszMailBoxName == nullptr || strlen(pszMailBoxName) <= 0)
			return false;

		int nID = g_MailImpl.QueryMailBoxID(pszPredicate, pszMailBoxName);
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;

		return g_MailImpl.CloseMailBox(pszPredicate, nID);
	}
	/*! \fn MAILAPI bool OpenPostOffice(const char*pszPredicate)
	********************************************************************************************************* 
	** \brief OpenPostOffice 
	** \details 打开邮件，只有进程外模块才需要调用，如果不是进程外，可以不调用
	** \param pszPredicate 
	** \return MAILAPI bool 
	** \author LiJin 
	** \date 2016年9月27日 
	** \note 
	********************************************************************************************************/
	MAILAPI bool OpenPostOffice(const char*pszPredicate)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		return g_MailImpl.OpenPostOffice(pszPredicate);
	}
	/*! \fn MAILAPI void ClosePostOffice(const char *pszPredicate)
	********************************************************************************************************* 
	** \brief ClosePostOffice 
	** \details 关闭邮寄
	** \param pszPredicate 
	** \return MAILAPI void 
	** \author LiJin 
	** \date 2016年9月27日 
	** \note 
	********************************************************************************************************/
	MAILAPI void ClosePostOffice(const char *pszPredicate)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return ;

		g_MailImpl.ClosePostOffice(pszPredicate);
	}
	/*! \fn MAILAPI int QueryMailBoxID(const char * pszPredicate, const char * szTagName)
	*********************************************************************************************************
	** \brief QueryMailBoxID
	** \details 查询邮箱ID
	** \param pszPredicate
	** \param szTagName
	** \return MAILAPI int
	** \author LiJin
	** \date 2016年9月27日
	** \note
	********************************************************************************************************/
	MAILAPI int QueryMailBoxID(const char * pszPredicate, const char * szTagName)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return 0;

		Q_ASSERT(szTagName);
		if (szTagName == nullptr || strlen(szTagName) <= 0)
			return 0;

		return g_MailImpl.QueryMailBoxID(pszPredicate, szTagName);
	}

	MAILAPI bool SubscribeMail(const char *pszPredicate, const char *szSelfMailBoxName, int32u nType)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		Q_ASSERT(szSelfMailBoxName);
		if (szSelfMailBoxName == nullptr || strlen(szSelfMailBoxName) <= 0)
			return false;
		
		int nID = g_MailImpl.QueryMailBoxID(pszPredicate, szSelfMailBoxName);
		Q_ASSERT(nID > 0);
		if (nID == 0)
			return false;

		return g_MailImpl.SubscribeMail(pszPredicate, nID,nType);
	}

	MAILAPI bool SendRawMail(const char *pszPredicate, const char * szSendMailBoxName, const char * szRecvMailBoxName, const unsigned char* pMail, int nLength, unsigned int nWaitTime)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		Q_ASSERT(szSendMailBoxName);
		if (szSendMailBoxName == nullptr || strlen(szSendMailBoxName) <= 0)
			return false;

		Q_ASSERT(szRecvMailBoxName);
		if (szRecvMailBoxName == nullptr || strlen(szRecvMailBoxName) <= 0)
			return false;

		int nSenderID = g_MailImpl.QueryMailBoxID(pszPredicate, szSendMailBoxName);
		Q_ASSERT(nSenderID > 0);
		if (nSenderID == 0)
			return false;
		
		int nReceiverID = g_MailImpl.QueryMailBoxID(pszPredicate, szRecvMailBoxName);
		Q_ASSERT(nReceiverID > 0);
		if (nReceiverID == 0)
			return false;

		return g_MailImpl.SendMail(pszPredicate, nSenderID, nReceiverID, pMail, nLength,nWaitTime);
	}

	MAILAPI bool SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		return g_MailImpl.SendMail(pszPredicate, pMsg,nWaitTime);
	} 

	MAILAPI bool RecvMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime)
	{
		Q_ASSERT(pszPredicate);
		if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
			return false;

		return g_MailImpl.RecvMail(pszPredicate, pMsg,nWaitTime);
	}
}



