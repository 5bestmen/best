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


#ifndef _MAIL_GRP_H
#define _MAIL_GRP_H

#include <memory> 
#include <set>
#include <unordered_map> 
#include "queue_impl.h"
#include "maildefine.h"

#include <boost/interprocess/sync/named_mutex.hpp>

/*! \class  CMailBoxGrp
*   \brief 邮箱组 FES\SCADA */
class CMailBoxGrp
{
public:
	CMailBoxGrp();
	~CMailBoxGrp();

	// 初始化时告诉系统有多少个邮箱需要创建，确定静态管理表的尺寸
	explicit CMailBoxGrp(const char * szPredicate);
public:
	std::string & GetName()
	{
		return m_szPrediateName;
	}
	// 主程序端需要先为各个模块建立邮箱
	bool Create(int nMailBoxID);
	bool Create(int nMailBoxID,size_t nNum, size_t nSize);
	bool Destroy(int nMailBoxID); 

	// 子模块打开
	bool Open(int nMailBoxID);
	bool Close(int nMailBoxID);

	bool SendMail(DMSG *pMsg, unsigned int nWaitTime = 10);
	bool RecvMail(int nRecvMailBoxID, unsigned char* pMail, size_t nBufLen, size_t&nRecvdLen,unsigned int nWaitTime = 10);
	bool RecvMail(DMSG *pMsg, unsigned int nWaitTime = 10);
	bool TrySendMail(int nRecvMailBoxID, const DMSG* pMail);
protected:
	bool SendMail(int nRecvMailBoxID, const DMSG* pMail);
private:
	std::string m_szPrediateName;
	// id 队列
	std::unordered_map< int, std::shared_ptr<CQueue> > m_mapMailQueue;

	std::shared_ptr<boost::interprocess::named_mutex>  m_pQueMux;
};

 

#endif // _MAIL_GRP_H
/** @}*/
