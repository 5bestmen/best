/*! @file nb_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  nb_svc.cpp
�ļ�ʵ�ֹ��� :  ����ӿ�ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����ӿ�ģ��
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "nb_svc.h"
#include "db_svc.h"
#include "fes.h"
#include "log/log.h"
#include "mail/mail.h"
#include "netbus/nbdef.h"
#include "netbus/nbapi.h"
#include "stl_util-inl.h"
#include "rtnet_msg_packer.h"

#include <QObject> 
#include <QString> 
#include <QHostAddress>
#include <vector>
#include <memory>

CNetbusSvc::CNetbusSvc(CFesSvc* pServer, const std::string& szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	m_pNodes = nullptr;
	m_nNodeCount = 0;
	m_pNodeConfigs = nullptr;
	m_pNetConfig = std::make_shared<NET_CONFIG>();
	m_pBuf = new EMSG_BUF;
}

CNetbusSvc::~CNetbusSvc(void)
{
	Shutdown();

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
	CFesSvc *pSvr = GetFesSvc();
	Q_ASSERT(pSvr);
	if (pSvr)
	{
		std::shared_ptr<CDbSvc>& pDBSvc = pSvr->GetDBSvc();
		Q_ASSERT(pDBSvc);
		if (pDBSvc == nullptr)
			return false;

		m_pNetMsgPacker = std::make_shared<CRTNetMsgPacker<CDbSvc>>(pDBSvc);

		int32u nMyNodeOccNo =  pDBSvc->GetMyNodeOccNo();
		Q_ASSERT(nMyNodeOccNo > INVALID_OCCNO && nMyNodeOccNo < MAX_NODE_OCCNO);
		if (nMyNodeOccNo == INVALID_OCCNO || nMyNodeOccNo > MAX_NODE_OCCNO)
		{
			return false;
		}
		m_pNetConfig->MyNodeOccNo = nMyNodeOccNo;

		std::tie(m_nNodeCount, m_pNodes) = pDBSvc->GetNodeData();
		Q_ASSERT(m_nNodeCount > 0 && m_nNodeCount <= MAX_NODE_OCCNO);
		Q_ASSERT(m_pNodes);
		m_pNetConfig->NodeCount = m_nNodeCount;
	//	m_pNetConfig->IsDoubleNet = true;
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
			return StartNetBus(m_pNetConfig.get(), pszDataPath, "FES", NODE_FES, nMode);
		}
	}

	return false;
}

void CNetbusSvc::Run()
{
	StartModule();
}

void CNetbusSvc::Shutdown()
{
	StopNetBus("FES");
	StopModule();
}
/*! \fn void  CNetbusSvc::MainLoop()
********************************************************************************************************* 
** \brief CNetbusSvc::MainLoop 
** \details ���������е���
** \return void 
** \author LiJin 
** \date 2016��10��21�� 
** \note 1.ת���ڲ��ʼ�����̫�������ϣ� 2.������̫�����еı��ģ�3.���½ڵ�״̬
********************************************************************************************************/
void  CNetbusSvc::MainLoop()
{
//!<1>�ȼ���ʼ�
	TransEmails();
//!<2>����ͬ�������Դ�������ǰ��ȫ��ͬ��
	TransDataInfo();
//!<3>��ʱ���½ڵ�״̬
	UpdateNetState();	
}

/*! \fn void CNetbusSvc::TransEmails()
********************************************************************************************************* 
** \brief CNetbusSvc::TransEmails 
** \details �յ��������ʼ� ����ת�����ڲ����ߣ�������ģ����� 
** \return void 
** \author xingzhibing
** \date 2017��3��14�� 
** \note  ��ǰ�����߼��� һֱ���� ֱ���ղ��� ִ����һ������ͬ��
********************************************************************************************************/
void CNetbusSvc::TransEmails()
{
	DMSG msg;
	msg.RecverID = m_nMailBoxID;

	static unsigned int nCount = 0;

	while (true)
	{
		bool bRet = false;
		bRet = RecvMail("FES", &msg, 0);
		//�ɹ��յ�һ���ʼ�,�յ��ʼ�����ת����scada��
		if (bRet)
		{
			qDebug() <<"RECVID::"<< msg.RecverID << "SENDID::"<<msg.SenderID;
			memset(m_pBuf, 0, EMSG_BUF_SIZE);

			m_pBuf->MsgPath = TO_SERVER;
			m_pBuf->MsgType = 1;
			m_pBuf->FuncCode = COT_ALARM;

			m_pBuf->SrcOccNo=m_pNetConfig->MyNodeOccNo;

			size_t nSize = EMSG_BUF_HEAD_SIZE + sizeof DMSG;
			m_pBuf->MsgDataSize = nSize;

			memcpy(m_pBuf->BuffData,&msg,sizeof DMSG);

			int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);

			if (!nRet)
			{
				//TODO LOG
			}
			nCount = 0;
		}
		else
		{			
			nCount++;
			std::chrono::milliseconds timespan(10);
			std::this_thread::sleep_for(timespan);
		}

		//���г��Զ���ռ��� �鿴�Ƿ����ʼ���������
		if (nCount>=3)
		{
			nCount = 0;
			return;
		}
	}
}


/*! \fn void CNetbusSvc::TransDataInfo()
********************************************************************************************************* 
** \brief CNetbusSvc::TransDataInfo 
** \details �������ݰ� ���Դ��� ��ǰȫ���ݾ�ͬ��
** \return void 
** \author xingzhibing
** \date 2017��3��14�� 
** \note 
********************************************************************************************************/
void CNetbusSvc::TransDataInfo()
{
	SendDinToSvr();
	SendAinToSvr();
}

/*! \fn void CNetbusSvc::UpdateNetState()
********************************************************************************************************* 
** \brief CNetbusSvc::UpdateNetState 
** \details ���½ڵ�״̬����ʱ�����������л�ȡϵͳ�и����ڵ������״̬
** \return void 
** \author LiJin 
** \date 2016��10��22�� 
** \note 
********************************************************************************************************/
void CNetbusSvc::UpdateNetState()
{
	// ����ǰ����Ҫ�ж��� ʵʱ�ڴ���Ƿ��Ѿ��˳�
	CFesSvc *pSvr = GetFesSvc();
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
#if 0
/*! \fn size_t CNetbusSvc::PackageAllRTData(std::shared_ptr<CDbSvc> pDB, unsigned char *pInBuff, size_t nBuffLen)
********************************************************************************************************* 
** \brief CNetbusSvc::PackageAllRTData 
** \details ��ȡȫ���Ĳ�ֵ��Ϣ��AI\DI\...��
** \param pDB 
** \param pInBuff 
** \param nBuffLen 
** \return size_t 
** \author LiJin 
** \date 2016��11��29�� 
** \note 
********************************************************************************************************/
size_t CNetbusSvc::PackageAllRTData(std::shared_ptr<CDbSvc> pDB, unsigned char *pInBuff, size_t nBuffLen)
{
	Q_ASSERT(pInBuff);
	if (pInBuff == nullptr)
		return 0;
	Q_ASSERT(pDB);
	if (pDB == nullptr)
		return 0;

	if (pDB->IsDBAlive(0))
	{

	}


	return 0;
}

#endif



void  CNetbusSvc::SendDinToSvr()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//��һ��pair Ϊ��ʼOccNo �ڶ���ΪԪ����Ŀ
	std::vector<std::pair<int32u, int32u > > arrDiNums;
	m_pNetMsgPacker->GetDIPackageNum(arrDiNums);
	
	int32u nSize;

	for (auto iter : arrDiNums)
	{
		m_pNetMsgPacker->PackageAllDI(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}


void CNetbusSvc::SendAinAlarmData(EMSG_BUF *pEMsgBuf)
{
	Q_ASSERT(pEMsgBuf);
	if (pEMsgBuf == nullptr)
		return;

	size_t nNeedLen = 0;
	size_t nCopyNum = 0;
	// ��������Ҫ��󻺳���
	for ( auto it : m_arrAinAlarmMsg)
	{
		Q_ASSERT(it->Len <= sizeof(AINALARM_MSG));
		nNeedLen += it->Len;
		if (nNeedLen > MAX_EMSG_L)
		{
			nNeedLen -= it->Len;
			break;
		}
		nCopyNum++;
	}

	if (nNeedLen == 0)
		return;


/*
	AIN_ALARM_DATA *pData = reinterpret_cast<AIN_ALARM_DATA*>(pEMsgBuf->MsgData);
	pData->DataCount = nCopyNum;
	pData->DataTotalLen = nNeedLen;

	size_t i = 0;
	for (auto it : m_arrAinAlarmMsg)
	{

	}
*/


	int nRet = NBSendData("FES", reinterpret_cast<int8u*>(pEMsgBuf), 100);
	// ������Ͳ��ɹ�����log��log�᲻��������

}

void CNetbusSvc::SendAinToSvr()
{
	Q_ASSERT(m_pNetMsgPacker);
	if (m_pNetMsgPacker == nullptr)
		return;

	memset(m_pBuf, 0, sizeof EMSG_BUF);

	m_pBuf->MsgPath  = EMSG_PATH::TO_SERVER;
	m_pBuf->MsgType  = MSG_TYPE::COT_PERCYC;
	m_pBuf->FuncCode = MSG_TYPE::COT_PERCYC;
	//��һ��pair Ϊ��ʼOccNo �ڶ���ΪԪ����Ŀ
	std::vector<std::pair<int32u, int32u > > arrAiNums;
	m_pNetMsgPacker->GetAIPackageNum(arrAiNums);

	int32u nSize;

	for (auto iter : arrAiNums)
	{
		m_pNetMsgPacker->PackageAllAI(m_pBuf, iter, nSize);
		int nRet = NBSendData("FES", reinterpret_cast<int8u*>(m_pBuf), nSize);
		if (!nRet)
		{
			//TODO LOG
		}
	}
}

/** @}*/
