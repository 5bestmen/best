/*! @file dbg_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dbg_svc.h
�ļ�ʵ�ֹ��� :  ���Դ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���Դ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef NB_SVC_MODULE_H
#define NB_SVC_MODULE_H

#include "base_module.h"
#include "datatypes.h"
#include <QString>
#include <memory>
 
struct NET_CONFIG;
struct NODE_CONFIG;
struct EMSG_BUF;
struct NODE;
struct DMSG;
struct AINALARM_MSG;
struct DINALARM_MSG;
struct DIAG_MSG;
struct ALARM_MSG;
struct TIMEPAK;

class CNetbusSvc : public CBaseModule
{
public:
	CNetbusSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &MailID);
	virtual ~CNetbusSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();

protected:
	virtual void MainLoop();
	void UpdateNetState();
	void TransScdEmails();
	void RecvNodeData();

private:
	bool ParaseAlarmEmails(DMSG* pMsg);
	//������Ϣ������֯
	QString GetEvtDescByCode(int32u nCode);
	void FormatAiAlarmDesc(AINALARM_MSG* pMsg, QString& szDesc);
	void FormatDiAlarmDesc(DINALARM_MSG* pMsg, QString& szDesc);
	void FormatDiagAlarmDesc(DIAG_MSG* pMsg, QString& szDesc);
	void FormatNormalAlarmDesc(ALARM_MSG* pMsg,QString& szDesc);
	void FormatTimeStamp(TIMEPAK* time,char* szTime);
private:
	EMSG_BUF* m_pBuf;

	NODE *m_pNodes;
	unsigned int m_nNodeCount;

	NODE_CONFIG  * m_pNodeConfigs;
	std::shared_ptr<NET_CONFIG> m_pNetConfig;

private:
	volatile bool m_bQuit;
};

#endif // NB_SVC_MODULE_H


/** @}*/

