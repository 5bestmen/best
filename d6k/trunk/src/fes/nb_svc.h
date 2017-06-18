/*! @file nb_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  nb_svc.h
文件实现功能 :  网络总线模块适配器
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   网络总线模块适配器
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef NB_SVC_MODULE_H
#define NB_SVC_MODULE_H

#include "base_module.h"
#include "datatypes.h"
#include <deque>

struct NET_CONFIG;
struct NODE_CONFIG;
struct BASE_MSG;
struct AINALARM_MSG;

struct NODE;
class CServer;
class CDbSvc;
struct EMSG_BUF;

template<typename DBSvc> class CRTNetMsgPacker;

class CNetbusSvc : public CBaseModule
{
public:
	CNetbusSvc(CFesSvc* pServer,const std::string& szMailBoxName, int &MailID);
	virtual ~CNetbusSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
protected:
	virtual void MainLoop();

	void UpdateNetState();

	void TransEmails();

	void TransDataInfo();

	void SendDinToSvr();

	void SendAinToSvr();

	void SendAlarmData();

	void SendAinAlarmData(EMSG_BUF *pEMsgBuf);
	 
	EMSG_BUF * m_pBuf;
//	size_t PackageAllRTData(std::shared_ptr<CDbSvc> pDB,unsigned char *pInBuff,size_t nBuffLen);
private:
	NODE *m_pNodes;
	unsigned int m_nNodeCount;

	NODE_CONFIG  * m_pNodeConfigs;
	std::shared_ptr<NET_CONFIG> m_pNetConfig;

	std::shared_ptr< CRTNetMsgPacker<CDbSvc> >  m_pNetMsgPacker;
	 

	bool m_bIsRedNode;  //! 本机是否是冗余节点


	std::deque<BASE_MSG*> m_arrAlarmMsg;

	struct AIN_ALARM_DATA
	{
		int32u DataCount;
		int32u DataTotalLen;
		AINALARM_MSG *pMsg;
	};

	//! 模拟量的越限复限告警，模拟量发生告警时，可以不立即上送，缓存5-10秒左右再上送
	std::deque<AINALARM_MSG*> m_arrAinAlarmMsg;
};

#endif // NB_SVC_MODULE_H


/** @}*/

