/*! @file nb_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  nb_svc.h
�ļ�ʵ�ֹ��� :  ��������ģ��������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��������ģ��������
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
	 

	bool m_bIsRedNode;  //! �����Ƿ�������ڵ�


	std::deque<BASE_MSG*> m_arrAlarmMsg;

	struct AIN_ALARM_DATA
	{
		int32u DataCount;
		int32u DataTotalLen;
		AINALARM_MSG *pMsg;
	};

	//! ģ������Խ�޸��޸澯��ģ���������澯ʱ�����Բ��������ͣ�����5-10������������
	std::deque<AINALARM_MSG*> m_arrAinAlarmMsg;
};

#endif // NB_SVC_MODULE_H


/** @}*/

