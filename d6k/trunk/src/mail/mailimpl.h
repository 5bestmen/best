/*! @file mailimpl.h
<PRE>
********************************************************************************
模块名       :  邮件模块
文件名       :  mailimpl.h
文件实现功能 :  邮件模块接口定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   邮件模块接口定义
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

	int32u  ShmLength;     //! 共享内存的长度
	int32u  SlotCount;
};

struct MB_SLOT
{
	int    MailBoxID;                         //! 邮箱ID
	char   MailBoxName[MAX_NAME_LENGTH];      //! 邮箱名字（tagname）
	int32u Subscription;                      //! 订阅的类型，按位计算
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
	// 注册消息类型
	bool SubscribeMail(int nID,int32u nType);

	size_t GetSubscriptionList(int32u nType, std::vector<int> & arrSub);
 
	// 主程序用
	void Create(const char *szPredicateName, size_t nSlotNum, int nStartID = GRP_MAILBOX_NUM);
	// 子模块用
	bool Open(const char *szPredicateName);
	void Close( );

private:
	//!  槽数目
	size_t m_nSlotNum;
	//! fes scada 的起始ID 不一样
	int m_nStartID;

	std::string m_szPredicate;
	// 共享内存
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
	// 创建邮局
	bool CreatePostOffice(const char*pszPredicate, size_t nSlotNum);
	// 销毁邮局
	void DestroyPostOffice(const char *pszPredicate);
	// 主模块 注册邮箱
	int RegisterMailBox(const char * pszPredicate, const char * pszMailBoxName);

	// 打开邮局 主要用于进程外的模块
	bool OpenPostOffice(const char*pszPredicate);
	// 关闭邮寄
	void ClosePostOffice(const char *pszPredicate);

	// 查询ID
	int QueryMailBoxID(const char * pszPredicate, const char * szTagName);
	
	// 打开
//	bool OpenMailBoxGrp(const char * pszPredicate);

	bool CreateMailBox(const char *pszPredicate, int nMailBoxID, size_t nNum, size_t nSize);
	bool DestroyMailBox(const char *pszPredicate, int nMailBoxID);
	bool OpenMailBox(const char *pszPredicate, int nMailBoxID);
	bool CloseMailBox(const char *pszPredicate, int nMailBoxID);

	bool CreateMailBox(const char *pszPredicate, const char *pszMailName, size_t nNum, size_t nSize, int & nRetID);

	// 前缀： FES   SCADA   邮箱名字： 通道TAGNAME
 	bool OpenMailBox(const char *pszPredicate, const char *pszMailName,int & nRetID );

	// 注册消息类型 一次性的注册，如果下次再注册会冲掉此前
	bool SubscribeMail(const char *pszPredicate,int nSelfMailBoxID, int32u nType);

	bool SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime);
	bool RecvMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime = 10);

	bool SendMail(const char *pszPredicate, int nSendMailBoxID, int nRecvMailBoxID, const unsigned char* pMail, int nLength, unsigned int nWaitTime);
	bool RecvMail(const char *pszPredicate, int nRecvMailBoxID, unsigned char* pMail, size_t nBufLen, size_t&nRecvdLen, unsigned int nWaitTime = 10);

protected:
	// 根据名称获取 邮件槽组
	std::shared_ptr<CMailBoxSlotGrp> GetMailBoxSlotGrp(const char * pszPredicate);
	// 根据名称获取 主邮件组
	std::shared_ptr<CMailBoxGrp> GetMasterMailBoxGrp(const char *pszPredicate);
	// 根据名称获取 从邮件组
	std::shared_ptr<CMailBoxGrp> GetSlaveMailBoxGrp(const char *pszPredicate);
private:
 	// 主模块用，主要用于创建
	std::vector< std::shared_ptr<CMailBoxGrp>  > m_arrMasterMailBoxs;
	// 子模块，就两组，一是 FES，二是 SCADA
	std::vector< std::shared_ptr<CMailBoxGrp>  > m_arrSlaveMailBoxs;

	std::vector< std::shared_ptr<CMailBoxSlotGrp> > m_arrSlots;

	int m_nStartID;

	std::shared_ptr<boost::interprocess::named_mutex>  m_pSlotMux;
};

extern CMailImpl g_MailImpl;


#endif // _MAIL_IMPL_H
/** @}*/
