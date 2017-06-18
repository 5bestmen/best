/*! @file nb_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  nb_svc.cpp
�ļ�ʵ�ֹ��� :	�������߷���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �������߷���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "nb_svc.h"
#include "db_svc.h"
#include "log/log.h"
#include "scdsvc.h"
#include "mail/mail.h"
#include "data_def.h"
#include "netbus/nbdef.h"
#include "netbus/nbapi.h"
#include "audiodataunit.h"
#include "audio_alarm_svc.h"
#include <qglobal.h>
#include <QObject> 
#include <QString> 
#include <QHostAddress>
#include <QDebug>

//�澯����״̬
enum ALARM_STATE
{
	alarm_unknown,       //״̬λ��
	alarm_confim,       //������ȷ��
	alarm_unconfim,     //����δȷ��
	alarm_delete,       //������ɾ��
};

enum
{
	MAX_CONTENT_LENGTH = 256,
};

struct CAlarmDataConfig
{
public:
	int     m_nAlarmState;
	int     m_nAlarmID;
	int     m_nType;
	int8s   m_szTimeTag[MAX_CONTENT_LENGTH];
	float   m_fVal;
	int8s   m_szContent[MAX_CONTENT_LENGTH*2];
};

CNetbusSvc::CNetbusSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;

	m_pNodeConfigs = nullptr;
	m_pBuf = new EMSG_BUF;
	m_pNetConfig = std::make_shared<NET_CONFIG>();
}

CNetbusSvc::~CNetbusSvc(void)
{
	if (m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = nullptr;
	}
	if (m_pNodeConfigs)
	{
		delete[] m_pNodeConfigs;
		m_pNodeConfigs = nullptr;
	}
}
/*! \fn bool CNetbusSvc::Initialize(RUN_MODE mode)
*********************************************************************************************************
** \brief CDbgSvc::Initialize
** \details ��ʼ��
** \return bool
** \author LiJin
** \date 2016��9��1��
** \note
********************************************************************************************************/
bool CNetbusSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
	
	CScadaSvc *pSvr = GetScadaSvc();
	Q_ASSERT(pSvr);
	if (pSvr)
	{
		//�ڴ����
		std::shared_ptr<CDbSvc>& pDBSvc = pSvr->GetDBSvc();
		Q_ASSERT(pDBSvc);
		if (pDBSvc == nullptr)
			return false;

		int32u nMyNodeOccNo = pDBSvc->GetMyNodeOccNo();
		Q_ASSERT(nMyNodeOccNo > INVALID_OCCNO && nMyNodeOccNo < MAX_NODE_OCCNO);
		if (nMyNodeOccNo == INVALID_OCCNO || nMyNodeOccNo > MAX_NODE_OCCNO)
		{
			return false;
		}
		m_pNetConfig->MyNodeOccNo = nMyNodeOccNo;

		m_nNodeCount = pDBSvc->GetNodeCount();
		Q_ASSERT(m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO);

		m_pNetConfig->NodeCount = m_nNodeCount;

		std::tie(m_nNodeCount, m_pNodes) = pDBSvc->GetNodeData();

		//m_pNetConfig->IsDoubleNet = true;
		if (m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO && m_pNodes)
		{
			Q_ASSERT(m_pNodeConfigs == nullptr);
			m_pNodeConfigs = new NODE_CONFIG[m_nNodeCount];
			std::memset(m_pNodeConfigs, 0, sizeof(NODE_CONFIG)*m_nNodeCount);

			// �ӹ����ڴ��п������ݳ���
			unsigned int i = 0;
			for (i = 0; i < m_nNodeCount; i++)
			{// TODO
				m_pNodeConfigs[i].OccNo = m_pNodes[i].OccNo;
				m_pNodeConfigs[i].SlaveOccNo = m_pNodes[i].SlaveOccNo;
				m_pNodeConfigs[i].NodeType = m_pNodes[i].NodeType;
				m_pNodeConfigs[i].NetAIPAddr = m_pNodes[i].NetAIPAddr;
				m_pNodeConfigs[i].NetBIPAddr = m_pNodes[i].NetBIPAddr;
				memset(m_pNodeConfigs[i].chIP[0], 0, sizeof m_pNodeConfigs[i].chIP[0]);
				strncpy(m_pNodeConfigs[i].chIP[0], QHostAddress(m_pNodes[i].NetAIPAddr).toString().toStdString().c_str(),
					qMin(NAME_SIZE + 1, QHostAddress(m_pNodes[i].NetAIPAddr).toString().length()));

				memset(m_pNodeConfigs[i].chIP[1], 0, sizeof m_pNodeConfigs[i].chIP[1]);
				strncpy(m_pNodeConfigs[i].chIP[1], QHostAddress(m_pNodes[i].NetBIPAddr).toString().toStdString().c_str(),
					qMin(NAME_SIZE + 1, QHostAddress(m_pNodes[i].NetBIPAddr).toString().length()));


				m_pNodeConfigs[i].HostSlave = m_pNodes[i].State;

				strncpy(m_pNodeConfigs[i].HostName, m_pNodes[i].HostName, NAME_SIZE);

				m_pNodeConfigs[i].SlaveOccNo = m_pNodes[i].SlaveOccNo;

				m_pNodeConfigs[i].CardStatus[0] = m_pNodes[i].Net1State;

				m_pNodeConfigs[i].CardStatus[1] = m_pNodes[i].Net2State;
			}

			m_pNetConfig->pNodeConfig = m_pNodeConfigs;
			// ������������ģ��
			return StartNetBus(m_pNetConfig.get(), pszDataPath, "SCADA", NODE_SVR, nMode);
		}
	}
	return false;
}

void CNetbusSvc::Run()
{
	m_bQuit = false;
	OpenPostOffice("SCADA");
	StartModule();

}

void CNetbusSvc::Shutdown()
{
	m_bQuit = true;
	StopNetBus("SCADA");
	StopModule();
}


/*! \fn void CNetbusSvc::TransScdEmails()
*********************************************************************************************************
** \brief CNetbusSvc::TransScdEmails
** \details ת�����ڵ��ʼ� �¼� �澯��������
** \return void
** \author xingzhibing
** \date 2017��3��15��
** \note
********************************************************************************************************/
void CNetbusSvc::TransScdEmails()
{
	//�����豸�澯�ϴ�
	DMSG msg;
	msg.RecverID = m_nMailBoxID;
	static unsigned int nCount = 0;

	while (!m_bStopFlag)
	{
		bool bRet = false;
		bRet = RecvMail("SCADA", &msg, 0);
		//�ɹ��յ�һ���ʼ�,�յ��ʼ�����ת����scada��
		if (bRet)
		{
			memset(m_pBuf, 0, EMSG_BUF_SIZE);

			m_pBuf->MsgPath = TO_FES;
			m_pBuf->MsgType =  1;
			m_pBuf->FuncCode = COT_SETVAL;
			m_pBuf->SrcOccNo = m_pNetConfig->MyNodeOccNo;

			size_t nSize = EMSG_BUF_HEAD_SIZE + sizeof DMSG;
			memcpy(m_pBuf->BuffData, &msg, qMin(sizeof DMSG, (size_t)EMSG_BUF_SIZE));

			int nRet = NBSendData("SCADA", reinterpret_cast<int8u*>(m_pBuf), nSize);

			nCount = 0;
		}
		else
		{
			nCount++;
			std::chrono::milliseconds timespan(10);
			std::this_thread::sleep_for(timespan);
		}
		//���г��Զ���ռ��� �鿴�Ƿ����ʼ���������
		if (nCount >= 3)
		{
			nCount = 0;
			return;
		}
	}
}


/*! \fn void CNetbusSvc::RecvNodeData()
*********************************************************************************************************
** \brief CNetbusSvc::RecvNodeData
** \details ���ո��ڵ�����
** \return void
** \author xingzhibing
** \date 2017��3��15��
** \note
********************************************************************************************************/
void CNetbusSvc::RecvNodeData()
{
	CScadaSvc *pSvr = GetScadaSvc();
	Q_ASSERT(pSvr);

	if (pSvr)
	{
		int8u cMsg[MAX_EMSG_L + 1] = { 0 };

		int32u nCopySize = 0;

		bool bRet = NBRecvData("SCADA", cMsg, MAX_EMSG_L, &nCopySize, 0);

		if (bRet)
		{
			EMSG_BUF* msg = (EMSG_BUF*)cMsg;

			if (msg)
			{
				switch (msg->FuncCode)
				{
				case COT_PERCYC:  //���������ϴ�����ǰֻ����ȫ�������͵�AI��DI
				{
					size_t nMsgHeadLen = sizeof EMSG_BUF_HEAD;

					DATA_BASE *pBase;

					if (msg)
					{
						size_t nSize = msg->MsgDataSize - nMsgHeadLen;
						if (nSize < sizeof DATA_BASE)
						{
							//TODO msg error��length error������һ�����ݰ����ȣ��϶�Ϊ�������ݰ�
							return;
						}
						else
						{
							pBase = (DATA_BASE*)(&msg->BuffData);
							Q_ASSERT(pBase);
							if (pBase)
							{
								if (pBase->m_nType == AIN_TYPE)
								{
									for (int i = 0; i < pBase->m_nCount; ++i)
									{
										fp64 fVal;
										memcpy(&fVal, &msg->BuffData[sizeof DATA_BASE + i * sizeof fp64], sizeof fp64);
										pSvr->GetDBSvc()->UpdateAinValue(pBase->m_nNodeOccNo, i + pBase->m_nStartOccNo, fVal);
									}
								}
								else if (pBase->m_nType == DIN_TYPE)
								{
									for (int i = 0; i < pBase->m_nCount; ++i)
									{
										int8u fVal;
										memcpy(&fVal, &msg->BuffData[sizeof DATA_BASE + i * sizeof int8u], sizeof int8u);
										pSvr->GetDBSvc()->UpdateDinValue(pBase->m_nNodeOccNo, i + pBase->m_nStartOccNo, fVal);
									}
								}
							}
						}
					}
					break;
				}
				case  COT_SPONT:  //����ͻ�����ͣ�����ʵ��֮����
				{
					break;
				}
				case COT_ALARM:   //�յ��澯��Ϣ �������귢�ͳ�ȥ���ĸ�ģ�鶩�������з���
				{
					DMSG tMsg;
					memcpy(&tMsg, (DMSG*)(&msg->BuffData),sizeof DMSG);
					//�����״ν����������������������ɸ澯�ʼ�����
					bool bRet=ParaseAlarmEmails(&tMsg);	
					
					qDebug() << "RECVID::" << tMsg.RecverID << "SENDID::" << tMsg.SenderID;
					if (!bRet)
					{
						//��δ�ж����¼�����
						return ;
					}
					break;
				}
				case COT_EVENT:   //�¼�Ҳ��������;
				{
					DMSG *pMailMsg;
					pMailMsg = (DMSG*)(&msg->BuffData);
					Q_ASSERT(pMailMsg);
					if (pMailMsg)
					{
						SendMail("SCADA", pMailMsg, 0);
					}
					break;
				}
				case COT_SUB: //�����������ͣ����嶨�����
					break;
				default:
					break;
				}
			}
		}
		else
		{
			std::chrono::milliseconds timespan(1);
			std::this_thread::sleep_for(timespan);
		}
	}
}



QString CNetbusSvc::GetEvtDescByCode(int32u nCode)
{
	if (nCode == AIN_ALARM_OFF)
	{
		//!< ģ����Խ���޸澯
		return QObject::tr("AIN_ALARM_OFF");
	}
	else if (nCode == AIN_ALARM_ON)
	{
		//!< ģ����Խ���޸澯
		return QObject::tr("AIN_ALARM_ON");
	}
	return QString::null;
}

void  CNetbusSvc::FormatAiAlarmDesc(AINALARM_MSG* pMsg, QString& szDesc)
{
	Q_ASSERT(pMsg);
	if (!pMsg)
	{
		return ;
	}
	AINALARM_MSG *pAlarmMsg = reinterpret_cast<AINALARM_MSG*> (pMsg);
	if (pAlarmMsg)
	{
		szDesc =  QObject::tr("CurrentNode:%1").arg(pAlarmMsg->NodeOccNo);
		szDesc += QObject::tr("Occurs:") + GetEvtDescByCode(pAlarmMsg->EvtCode);
		szDesc += QObject::tr("AlarmEvent:");
		szDesc += QObject::tr("AIN:%1_Alarm:%2_AIN_ALARM_Limit:%3_Value:%4").arg(pAlarmMsg->OccNo).arg(pAlarmMsg->AlarmOccNo).arg(pAlarmMsg->AlarmLimitOccNo).arg(pAlarmMsg->Value);
	}
}

void CNetbusSvc::FormatDiAlarmDesc(DINALARM_MSG* pMsg, QString& szDesc)
{
	Q_ASSERT(pMsg);
	if (!pMsg)
	{
		return ;
	}
	DINALARM_MSG *pAlarmMsg = reinterpret_cast<DINALARM_MSG*> (pMsg);
	if (pAlarmMsg)
	{
		szDesc =  QObject::tr("CurrentNode:%1").arg(pAlarmMsg->NodeOccNo);
		szDesc += QObject::tr("Occurs:") + GetEvtDescByCode(pAlarmMsg->EvtCode);
		szDesc += QObject::tr("AlarmEvent:");
		szDesc += QObject::tr("DIN:%1_Alarm:%2_DIN_ALARM_Limit:%3_Value:%4").arg(pAlarmMsg->OccNo).arg(pAlarmMsg->AlarmOccNo).arg(pAlarmMsg->AlarmLimitOccNo).arg(pAlarmMsg->Value);
	}
}

void CNetbusSvc::FormatDiagAlarmDesc(DIAG_MSG* pMsg, QString& szDesc)
{
	Q_ASSERT(pMsg);
	if (!pMsg)
	{
		return;
	}

	DIAG_MSG *pDiagMsg = reinterpret_cast<DIAG_MSG*> (pMsg);
	if (pDiagMsg)
	{
		szDesc =  QObject::tr("Node:%1").arg(pDiagMsg->NodeOccNo);
		szDesc += QObject::tr("Channel:%1").arg(pDiagMsg->ChannelOccNo);
		szDesc += QObject::tr("Device:%1").arg(pDiagMsg->DeviceOccNo);
		szDesc += QObject::tr("Occurs:") + QString::fromLocal8Bit((char*)pDiagMsg->ExtraData);
	}
}

void CNetbusSvc::FormatNormalAlarmDesc(ALARM_MSG* pMsg, QString& szDesc)
{
	Q_ASSERT(pMsg);
	if (!pMsg)
	{
		return ;
	}

	ALARM_MSG *pAlarmMsg = reinterpret_cast<ALARM_MSG*> (pMsg);
	if (pAlarmMsg)
	{
		szDesc =  QObject::tr("CurrentNode:%1").arg(pAlarmMsg->NodeOccNo);
		szDesc += QObject::tr(" Event:%1").arg(QString::fromLocal8Bit((char*)pAlarmMsg->ExtraData));
	}
}

void CNetbusSvc::FormatTimeStamp(TIMEPAK* pak, char* szTime)
{
	Q_ASSERT(pak);
	if (!pak)
	{
		return;
	}
	Q_ASSERT(szTime );
	if (!szTime)
	{
		return;
	}
	QString str = QString("%1.%2.%3--%4:%5:%6::%7").arg(pak->Year).arg(pak->Month).arg(pak->Day).arg(pak->Hour).arg(pak->Minute).arg(pak->Second).arg(pak->Milliseconds);

	strncpy (szTime , str.toLocal8Bit().data(),std::min((size_t)MAX_FILENAME_LENGTH,(size_t)str.length()));
}

bool  CNetbusSvc::ParaseAlarmEmails(DMSG* pMsg)
{
	Q_ASSERT(pMsg);
	if (!pMsg)
	{
		return false;
	}
	QString szAlarmDesc;
	switch (pMsg->Type)
	{
	case MSG_EVT_ANA:
	{	
		AINALARM_MSG *pAlarmMsg = reinterpret_cast<AINALARM_MSG*> (pMsg->Buff);
		if (pAlarmMsg)
		{
			FormatAiAlarmDesc(pAlarmMsg, szAlarmDesc);
		}
		//�γ������澯
		AIN_ALARM_LIMIT* pAinAlarmLimit=nullptr;
		GetScadaSvc()->GetDBSvc()->GetAinAlarmLimitByOccNo(pAlarmMsg->NodeOccNo, pAlarmMsg->AlarmOccNo, &pAinAlarmLimit);
		Q_ASSERT(pAinAlarmLimit);

		//��֯������Ϣ
		CAudioBaseData_DEF  pAudio = std::make_shared<CAudioBaseData>();
		pAudio->m_bAck = pAinAlarmLimit->EnableAck;
		pAudio->m_nAckType = pAinAlarmLimit->AckType;
		pAudio->m_nPlayTimes = pAinAlarmLimit->PlaySoundTimes;
		pAudio->m_nPriority = pAinAlarmLimit->Priority;
		pAudio->m_bSoundEnable = (strlen(pAinAlarmLimit->SoundFile)==0)?true:false;
		strncpy (pAudio->m_szSoundFile,pAinAlarmLimit->SoundFile,MAX_FILENAME_LENGTH);
		pAudio->m_bSpeechTextEnable = true;
		strncpy(pAudio->m_szSpeechText,szAlarmDesc.toLocal8Bit().data(),qMin(size_t(MAX_FILENAME_LENGTH*4),(size_t)szAlarmDesc.length()));
		pAudio->m_bBeepEnable = pAinAlarmLimit->Beep;

		GetScadaSvc()->GetAudioSvc()->AddAudioAlarmInfo(pAudio);

		CAlarmDataConfig msgInfo;
		std::memset(&msgInfo, 0, sizeof CAlarmDataConfig);
		msgInfo.m_nAlarmState= alarm_unconfim;
		msgInfo.m_fVal = pAlarmMsg->Value;
		msgInfo.m_nType = pMsg->Type;
		strncpy(msgInfo.m_szContent,szAlarmDesc.toLocal8Bit().data(),qMin((size_t)MAX_CONTENT_LENGTH*2,(size_t)szAlarmDesc.length()));
		FormatTimeStamp(&pAlarmMsg->Tm, msgInfo.m_szTimeTag);

		DMSG dmsg;
		std::memset(&dmsg, 0, sizeof(DMSG));
		dmsg.Type = MSG_EVT8;
		dmsg.SenderID = 0;
		int m_nNBSvcMailBoxID = QueryMailBoxID("SCADA", "ALARM_SCD_WIN");
		dmsg.RecverID = m_nNBSvcMailBoxID;
		dmsg.Size = sizeof CAlarmDataConfig;
		memcpy(dmsg.Buff,&msgInfo,std::min((size_t)dmsg.Size,sizeof CAlarmDataConfig));
		return SendMail("SCADA",&dmsg,0);

		break;
	}
	case MSG_EVT_DIGI:
	{
		DINALARM_MSG *pAlarmMsg = reinterpret_cast<DINALARM_MSG*> (pMsg->Buff);
		if (pAlarmMsg)
		{
			FormatDiAlarmDesc(pAlarmMsg, szAlarmDesc);
		}
		//�γ������澯
		DIN_ALARM_LIMIT* pDinAlarmLimit = nullptr;
		GetScadaSvc()->GetDBSvc()->GetDinAlarmLimitByOccNo(pAlarmMsg->NodeOccNo, pAlarmMsg->AlarmLimitOccNo, &pDinAlarmLimit);
		Q_ASSERT(pDinAlarmLimit);

		//��֯������Ϣ
		CAudioBaseData_DEF  pAudio = std::make_shared<CAudioBaseData>();
		pAudio->m_bAck = pDinAlarmLimit->EnableAck;
		pAudio->m_nAckType = pDinAlarmLimit->AckType;
		pAudio->m_nPlayTimes = pDinAlarmLimit->PlaySoundTimes;
		pAudio->m_nPriority = pDinAlarmLimit->Priority;
		pAudio->m_bSoundEnable = (strlen(pDinAlarmLimit->SoundFile) == 0) ? false : true;
		strncpy(pAudio->m_szSoundFile, pDinAlarmLimit->SoundFile, MAX_FILENAME_LENGTH);
		pAudio->m_bSpeechTextEnable = true;
		strncpy(pAudio->m_szSpeechText, szAlarmDesc.toLocal8Bit().data(), qMin(size_t(MAX_FILENAME_LENGTH * 4), (size_t)szAlarmDesc.length()));
		pAudio->m_bBeepEnable = pDinAlarmLimit->Beep;

		GetScadaSvc()->GetAudioSvc()->AddAudioAlarmInfo(pAudio);

		//�����澯��Ϣ
		CAlarmDataConfig msgInfo;
		std::memset(&msgInfo, 0, sizeof CAlarmDataConfig);
		msgInfo.m_nAlarmState = alarm_unconfim;
		msgInfo.m_fVal = pAlarmMsg->Value;
		msgInfo.m_nType = pMsg->Type;
		strncpy(msgInfo.m_szContent, szAlarmDesc.toLocal8Bit().data(), qMin((size_t)MAX_CONTENT_LENGTH * 2, (size_t)szAlarmDesc.length()));
		FormatTimeStamp(&pAlarmMsg->Tm, msgInfo.m_szTimeTag);

		DMSG dmsg;
		std::memset(&dmsg, 0, sizeof(DMSG));
		dmsg.Type = MSG_EVT8;
		dmsg.SenderID = 0;
		int m_nNBSvcMailBoxID = QueryMailBoxID("SCADA", "ALARM_SCD_WIN");
		dmsg.RecverID = m_nNBSvcMailBoxID;
		dmsg.Size = sizeof CAlarmDataConfig;
		memcpy(dmsg.Buff, &msgInfo, std::min((size_t)dmsg.Size, sizeof CAlarmDataConfig));
		return SendMail("SCADA", &dmsg, 0);

		break;
	}		
	case MSG_EVT_DIAG:
	{
		DIAG_MSG *pDiagMsg = reinterpret_cast<DIAG_MSG*> (pMsg->Buff);
		if (pDiagMsg)
		{
			FormatDiagAlarmDesc(pDiagMsg, szAlarmDesc);
		}
		//�γ������澯
		CAudioBaseData_DEF  pAudio = std::make_shared<CAudioBaseData>();
		pAudio->m_bAck = true;
		pAudio->m_nAckType = AFTER_CONFIRM_DELETE;
		pAudio->m_nPlayTimes = 1;
		pAudio->m_nPriority = 1;
		pAudio->m_bSoundEnable = false ;
		pAudio->m_bSpeechTextEnable = true;
		strncpy(pAudio->m_szSpeechText, szAlarmDesc.toLocal8Bit().data(), qMin(size_t(MAX_FILENAME_LENGTH * 4), (size_t)szAlarmDesc.length()));
		pAudio->m_bBeepEnable = false;

		GetScadaSvc()->GetAudioSvc()->AddAudioAlarmInfo(pAudio);

		//�����澯��Ϣ
		CAlarmDataConfig msgInfo;
		std::memset(&msgInfo, 0, sizeof CAlarmDataConfig);
		msgInfo.m_nAlarmState = alarm_unconfim;
		msgInfo.m_fVal = 1;
		msgInfo.m_nType = pMsg->Type;
		strncpy(msgInfo.m_szContent, szAlarmDesc.toLocal8Bit().data(), qMin((size_t)MAX_CONTENT_LENGTH * 2,(size_t) szAlarmDesc.length()));
		FormatTimeStamp(&pDiagMsg->Tm, msgInfo.m_szTimeTag);

		DMSG dmsg;
		std::memset(&dmsg, 0, sizeof(DMSG));
		dmsg.Type = MSG_EVT8;
		dmsg.SenderID = 0;
		int m_nNBSvcMailBoxID = QueryMailBoxID("SCADA", "ALARM_SCD_WIN");
		dmsg.RecverID = m_nNBSvcMailBoxID;
		dmsg.Size = sizeof CAlarmDataConfig;
		memcpy(dmsg.Buff, &msgInfo, std::min((size_t)dmsg.Size, sizeof CAlarmDataConfig));
		return SendMail("SCADA", &dmsg, 0);
		break;
	}		
	case MSG_EVT_OPER:
		break;
	case MSG_EVT_SETVAL:
		break;
	case MSG_EVT6:
	case MSG_EVT7:
	{
		ALARM_MSG *pAlarmMsg = reinterpret_cast<ALARM_MSG*> (pMsg->Buff);
		if (pAlarmMsg)
		{
			FormatNormalAlarmDesc(pAlarmMsg, szAlarmDesc);
		}
		CAudioBaseData_DEF  pAudio = std::make_shared<CAudioBaseData>();
		pAudio->m_bAck = true;
		pAudio->m_nAckType = AFTER_CONFIRM_DELETE;
		pAudio->m_nPlayTimes = 1;
		pAudio->m_nPriority = 1;
		pAudio->m_bSoundEnable = false;
		pAudio->m_bSpeechTextEnable = true;
		strncpy(pAudio->m_szSpeechText, szAlarmDesc.toLocal8Bit().data(), qMin(size_t(MAX_FILENAME_LENGTH * 4), (size_t)szAlarmDesc.length()));
		pAudio->m_bBeepEnable = false;

		GetScadaSvc()->GetAudioSvc()->AddAudioAlarmInfo(pAudio);

		CAlarmDataConfig msgInfo;
		std::memset(&msgInfo,0,sizeof CAlarmDataConfig);
		msgInfo.m_nAlarmState = alarm_unconfim;
		msgInfo.m_fVal = 1;
		msgInfo.m_nType = pMsg->Type;
		strncpy(msgInfo.m_szContent, szAlarmDesc.toLocal8Bit().data(), qMin((size_t)MAX_CONTENT_LENGTH * 2, (size_t)szAlarmDesc.length()));
		FormatTimeStamp(&pAlarmMsg->Tm, msgInfo.m_szTimeTag);

		DMSG dmsg;
		std::memset(&dmsg, 0, sizeof(DMSG));
		dmsg.Type = MSG_EVT8;
		dmsg.SenderID = 0;
		int m_nNBSvcMailBoxID = QueryMailBoxID("SCADA", "ALARM_SCD_WIN");
		dmsg.RecverID = m_nNBSvcMailBoxID;
		dmsg.Size = sizeof CAlarmDataConfig;
		memcpy(dmsg.Buff, &msgInfo, std::min((size_t)dmsg.Size, sizeof CAlarmDataConfig));
		return SendMail("SCADA", &dmsg, 0);
		break;
	}		
	default:
		break;
	}

	return true;
}

void CNetbusSvc::MainLoop()
{
	while (!m_bQuit)
	{
		//!<1�ȼ���Ƿ��������·�,�������·���Ȼ����������
		TransScdEmails();

		//!<2���ո��ڵ�����
		RecvNodeData();

		//!<3��������ڵ�״̬
		UpdateNetState();
	}
}

void CNetbusSvc::UpdateNetState()
{
	CScadaSvc *pSvr = GetScadaSvc();
	Q_ASSERT(pSvr);
	if (pSvr)
	{
		std::shared_ptr<CDbSvc> pDBSvc = pSvr->GetDBSvc();
		Q_ASSERT(pDBSvc);
		if (pDBSvc)
		{
			if (pDBSvc->IsDBAlive(0))
			{
				Q_ASSERT(m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO);
				Q_ASSERT(m_pNodes);

				if (m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO && m_pNodes && m_pNodeConfigs && m_pNetConfig)
				{
					unsigned int i = 0;
					int32u nOccNo = 0;
					for (i = 0; i < m_nNodeCount; i++)
					{
						nOccNo = m_pNodeConfigs[i].OccNo;
						Q_ASSERT(m_pNodeConfigs[i].OccNo > INVALID_OCCNO && m_pNodeConfigs[i].OccNo <= MAX_NODE_OCCNO);
						if (m_pNodeConfigs[i].OccNo > INVALID_OCCNO && m_pNodeConfigs[i].OccNo <= MAX_NODE_OCCNO)
						{
							m_pNodes[nOccNo - 1].State = m_pNodeConfigs[i].HostSlave;
							m_pNodes[nOccNo - 1].Net1State = m_pNodeConfigs[i].CardStatus[0];
							m_pNodes[nOccNo - 1].Net2State = m_pNodeConfigs[i].CardStatus[1];
						}
					}
				}
			}
		}
	}
}



/** @}*/
