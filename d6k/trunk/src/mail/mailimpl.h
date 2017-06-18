/*! @file mailimpl.h
<PRE>
********************************************************************************
ģ����       :  �ʼ�ģ��
�ļ���       :  mailimpl.h
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


#ifndef _MAIL_IMPL_H
#define _MAIL_IMPL_H

#include "queue_impl.h"
#include "maildefine.h"

#include <memory> 
#include <set>
#include <unordered_map> 
#include <boost/interprocess/sync/named_mutex.hpp>

class CShareMem;

#define  MAGIC_HEAD 0X4C49
#define  MAX_NAME_LENGTH   512
#define  STRING_PAD  sizeof(size_t)

#define  MAX_MAILBOX_NUM 2000000   // 200W
#define  GRP_MAILBOX_NUM 10000   // 1W


struct SLOT_HEAD_MEM
{
	int32u  MagicHead1;
	int32u  MagicHead2;

	int32u  ShmLength;     //! �����ڴ�ĳ���
	int32u  SlotCount;
};

struct MB_SLOT
{
	int    MailBoxID;                         //! ����ID
	char   MailBoxName[MAX_NAME_LENGTH];      //! �������֣�tagname��
	int32u Subscription;                      //! ���ĵ����ͣ���λ����
};

class CMailBoxSlotGrp
{
public:
	CMailBoxSlotGrp();
	explicit CMailBoxSlotGrp(const char *szPredicateName, size_t nSlotNum, int nStartID = GRP_MAILBOX_NUM);
	~CMailBoxSlotGrp();
public:
	std::string &GetName()  
	{
		return m_szPredicate;
	}

	int QueryMailBoxID(const char *pszMailBoxName);

	int RegisterMailBox(const char * pszMailBoxName);
	// ע����Ϣ����
	bool SubscribeMail(int nID,int32u nType);

	size_t GetSubscriptionList(int32u nType, std::vector<int> & arrSub);
 
	// ��������
	void Create(const char *szPredicateName, size_t nSlotNum, int nStartID = GRP_MAILBOX_NUM);
	// ��ģ����
	bool Open(const char *szPredicateName);
	void Close( );

private:
	//!  ����Ŀ
	size_t m_nSlotNum;
	//! fes scada ����ʼID ��һ��
	int m_nStartID;

	std::string m_szPredicate;
	// �����ڴ�
	std::unordered_map<int, MB_SLOT *> m_mapSlotsID;
	std::unordered_map<std::string, int> m_mapSlotsName;

	std::shared_ptr<CShareMem> m_pShareMem;
};

class CMailBoxGrp;
class CMailImpl
{
public:
	CMailImpl(void);
	~CMailImpl(void); 
public:
	// �����ʾ�
	bool CreatePostOffice(const char*pszPredicate, size_t nSlotNum);
	// �����ʾ�
	void DestroyPostOffice(const char *pszPredicate);
	// ��ģ�� ע������
	int RegisterMailBox(const char * pszPredicate, const char * pszMailBoxName);

	// ���ʾ� ��Ҫ���ڽ������ģ��
	bool OpenPostOffice(const char*pszPredicate);
	// �ر��ʼ�
	void ClosePostOffice(const char *pszPredicate);

	// ��ѯID
	int QueryMailBoxID(const char * pszPredicate, const char * szTagName);
	
	// ��
//	bool OpenMailBoxGrp(const char * pszPredicate);

	bool CreateMailBox(const char *pszPredicate, int nMailBoxID, size_t nNum, size_t nSize);
	bool DestroyMailBox(const char *pszPredicate, int nMailBoxID);
	bool OpenMailBox(const char *pszPredicate, int nMailBoxID);
	bool CloseMailBox(const char *pszPredicate, int nMailBoxID);

	bool CreateMailBox(const char *pszPredicate, const char *pszMailName, size_t nNum, size_t nSize, int & nRetID);

	// ǰ׺�� FES   SCADA   �������֣� ͨ��TAGNAME
 	bool OpenMailBox(const char *pszPredicate, const char *pszMailName,int & nRetID );

	// ע����Ϣ���� һ���Ե�ע�ᣬ����´���ע�������ǰ
	bool SubscribeMail(const char *pszPredicate,int nSelfMailBoxID, int32u nType);

	bool SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime);
	bool RecvMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime = 10);

	bool SendMail(const char *pszPredicate, int nSendMailBoxID, int nRecvMailBoxID, const unsigned char* pMail, int nLength, unsigned int nWaitTime);
	bool RecvMail(const char *pszPredicate, int nRecvMailBoxID, unsigned char* pMail, size_t nBufLen, size_t&nRecvdLen, unsigned int nWaitTime = 10);

protected:
	// �������ƻ�ȡ �ʼ�����
	std::shared_ptr<CMailBoxSlotGrp> GetMailBoxSlotGrp(const char * pszPredicate);
	// �������ƻ�ȡ ���ʼ���
	std::shared_ptr<CMailBoxGrp> GetMasterMailBoxGrp(const char *pszPredicate);
	// �������ƻ�ȡ ���ʼ���
	std::shared_ptr<CMailBoxGrp> GetSlaveMailBoxGrp(const char *pszPredicate);
private:
 	// ��ģ���ã���Ҫ���ڴ���
	std::vector< std::shared_ptr<CMailBoxGrp>  > m_arrMasterMailBoxs;
	// ��ģ�飬�����飬һ�� FES������ SCADA
	std::vector< std::shared_ptr<CMailBoxGrp>  > m_arrSlaveMailBoxs;

	std::vector< std::shared_ptr<CMailBoxSlotGrp> > m_arrSlots;

	int m_nStartID;

	std::shared_ptr<boost::interprocess::named_mutex>  m_pSlotMux;
};

extern CMailImpl g_MailImpl;


#endif // _MAIL_IMPL_H
/** @}*/
