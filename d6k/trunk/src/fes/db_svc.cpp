/*! @file db_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  db_svc.cpp
文件实现功能 :  内存数据库服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库服务
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/

#include "db_svc.h"
#include "log/log.h"
#include "../fespub/sharemem.h"
#include "fesapi/fesdatadef.h"
#include "fesapi/fes_magicmem.h" 
#include "io_channel.h"
#include "ft_channel.h"
#include "mail/mail.h"

#include "sys_var.h"

#include "stl_util-inl.h"

#include <QFileInfo>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QDateTime>

CDbSvc::CDbSvc(CFesSvc* pServer, const std::string& szMailBoxName, int &nMailID) : CBaseModule(pServer, szMailBoxName,nMailID)
{
	//normal pool
	m_pMem = std::make_shared<CShareMem>();
	//tagname pool
	m_pTagMem = std::make_shared<CShareMem>();
	//string pool
	m_pStringMen = std::make_shared<CShareMem>();

	m_pChannelMgr = std::make_shared<CChannelMgr>();

	m_pFesInfo = std::make_shared<FesInfo>();

	m_arrTempNodes.clear();
	m_arrTempChannels.clear();
	m_arrTempDevices.clear();
	m_arrTempAins.clear();
	m_arrTempDins.clear();
	m_arrTempAouts.clear();
	m_arrTempDouts.clear();	

	m_nChannelCount = 0;
	m_nDeviceCount = 0;
	m_nAinCount = 0;
	m_nDinCount = 0;
	m_nDoutCount = 0;
	m_nAoutCount = 0;

	m_bStopFlag = false;

	m_nMyNodeOccNo = INVALID_OCCNO;

	m_pSysSvc = std::make_shared<CSysVarSvc>(this);
}

CDbSvc::~CDbSvc(void)
{
	m_arrChannelPtrs.clear();
	m_arrDevices.clear();
	m_arrAins.clear();
	m_arrDins.clear();
	m_arrDouts.clear();
	m_arrAouts.clear();
	m_arrLinears.clear();
	m_arrNonLinears.clear();
	m_arrAinAlarms.clear();
	m_arrAinAlarmLimits.clear();
	m_arrDinAlarms.clear();
	m_arrDinAlarmLimits.clear();

	m_arrNodeOccNos.clear();
	m_arrChannelOccNos.clear();
	m_arrDeviceOccNos.clear();
	m_arrAinOccNos.clear();
	m_arrDinOccNos.clear();
	m_arrAoutOccNos.clear();
	m_arrDoutOccNos.clear();
	m_arrAinAlarmOccNos.clear();
	m_arrDinAlarmOccNos.clear();
	m_arrAinAlarmLimitOccNos.clear();
	m_arrDinAlarmLimitOccNos.clear();
	m_arrTransLinearOccNos.clear();
	m_arrTransNonLinearOccNos.clear();

	m_arrTempNodes.clear();
	m_arrTempChannels.clear();
	m_arrTempDevices.clear();
	m_arrTempAins.clear();
	m_arrTempDins.clear();
	m_arrTempAouts.clear();
	m_arrTempDouts.clear();
	m_arrTempTransFormLinears.clear();
	m_arrTempTransFormNonLinears.clear();
	m_arrTempAinAlarms.clear();
	m_arrTempDinAlarms.clear();
	m_arrTempAinLimitAlarms.clear();
	m_arrTempDinLimitAlarms.clear();

	m_bStopFlag = true;
}
 
/*! \fn bool CDbSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  加载流程如下：
          1.打开该工程下面的node目录下面的node.xml，并解析，找到该hostname的记录，获取其config字段值
		  2.得到上述的config字段的值 （如:xxx），则xxx.xml为前置的配置文件
		  3.打开工程文件 proj，判断xxx.xml是否从属于该项目，如果是解析 xxx.xml文件。
********************************************************************************************************/
bool CDbSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
	//! 1 加载xml文件
	Q_ASSERT(pszDataPath);
	
	if (pszDataPath && strlen(pszDataPath))
	{
		m_szDataBaseName = (pszDataPath);
	}

	QString szRootName = QFileInfo(pszDataPath).path();

	//获取节点名称
	if (!GetNodeInfo(pszDataPath))
	{
		LogMsg(QObject::tr("can not get node info!").toStdString().c_str(), 0);
		Q_ASSERT(false);
		return false;
	}
	Q_ASSERT(m_szNodeName.isEmpty() == false);
	if (m_szNodeName.isEmpty() == true)
	{
		LogMsg(QObject::tr("node name error , can not find this node !").toStdString().c_str(), 0);
		Q_ASSERT(false);
		return false;
	}

	m_szNodeName = szRootName + "/node/" + m_szNodeName;

	//获取当前前置配置文件信息
	if (!GetFesInfo(m_szNodeName.toStdString().c_str()))
	{
		Q_ASSERT(false);
		LogMsg(QObject::tr("Node:Get Fes Info error!").toStdString().c_str(), 0);
		return false;
	}

	if (!GetTotalNodeInfo(m_szNodeName.toStdString().c_str()))
	{
		Q_ASSERT(false);
		LogMsg(QObject::tr("Node:get all nodes Info error!").toStdString().c_str(), 0);
		return false;
	}

	m_szCurFesInfoName = szRootName + "/scada/" + m_szCurFesInfoName;

	//加载配置文件
	if (!ReadXmlInfo(m_szCurFesInfoName.toStdString().c_str(), _STREAM_XML_TYPE))
	{
		Q_ASSERT(false);
		LogMsg(QObject::tr("read xml:can not load fes.xml(stream)").toStdString().c_str(), 0);
		return false;
	}	
	else
	{
		LogMsg(QObject::tr("Load Proj(Node,Fes) File Succeed!").toStdString().c_str(), 0);
	}

	QString szStringName = szRootName + "/scada/desc_string_pool.xml";
	//!读取字符串池文件
	if (!LoadStringNamePool(szStringName.toStdString().c_str()))
	{
		LogMsg(QObject::tr("load string pool error!").toStdString().c_str(), 0);
		return false;
	}
	else
	{
		LogMsg(QObject::tr("Load string pool File Succeed!").toStdString().c_str(), 0);
	}
	//! 根据此前xml读取到的文件，进行申请共享内存
	QString szMemName = QFileInfo(pszDataPath).baseName();

	BuildMemDB(pszDataPath);
	LogMsg(QObject::tr("Build FesSHMPool(Node,Fes) Succeed!").toStdString().c_str(), 0);

	//创建TagName池
	BuildTagNamePoolDB("TagNamePool");
	LogMsg(QObject::tr("Build TagNamePool(Node,Fes) Succeed!").toStdString().c_str(), 0);
	
	QString szStringPoolName =QString(pszDataPath) + "-stringpool";
	BuildStringNamePool(szStringPoolName.toStdString().c_str());
	LogMsg(QObject::tr("Build StringPool Succeed!").toStdString().c_str(), 0);


	// 清空临时内存
	ClearTempArray();
	 
	// EVENT 设置成 手动模式，初始化为 true，名字为内存库名字 第一参数为模式设定 ，第二参数为是否有信号 ，第三参数为进程内外 ，第四为全局信号名称
	m_pDBAliveFlag = std::make_shared<ACE_Event>(1, 0, USYNC_THREAD, m_szDataBaseName.c_str());
	// 设置成有信号状态
	m_pDBAliveFlag->signal();
	LogMsg(QObject::tr("FesDB signal initalize!").toStdString().c_str(), 0);
	 
	LoadDrivers();
	LogMsg(QObject::tr("Fes:Load Drivers OK").toStdString().c_str(), 0);

	if (m_pSysSvc)
	{
		m_pSysSvc->Initialize(pszDataPath,nMode);
	}

	return true;
}

void CDbSvc::Run()
{
	StartMainTask();
	LogMsg(QObject::tr("Fes::Start Main Task").toStdString().c_str(), 0);
	if (m_pSysSvc)
	{
		m_pSysSvc->Run( );
	}

	StartDrivers();
	LogMsg(QObject::tr("Fes::Start Drivers").toStdString().c_str(), 0);
}

void CDbSvc::Shutdown()
{
	if (m_pSysSvc)
	{
		m_pSysSvc->Shutdown();
	}

	if (m_pDBAliveFlag)
	{
		// 结束之后，设置成无信号状态
		m_pDBAliveFlag->reset();
		m_pDBAliveFlag.reset();
	}

	StopMainTask();
	LogMsg(QObject::tr("Fes:Shut down").toStdString().c_str(), 0);
	m_bStopFlag = true;
}
/*! \fn void CDbSvc::BuildMemDB(const char *szDBName)
********************************************************************************************************* 
** \brief CDbSvc::BuildMemDB 
** \details 申请共享内存，并建立内存数据库
** \return void 
** \author LiJin 
** \date 2016年9月3日 
** \note 
********************************************************************************************************/
void CDbSvc::BuildMemDB(const char *szDBName)
{
	Q_ASSERT(szDBName);
	if (!szDBName)
	{
		return;
	}
	m_szRdbName = szDBName;
	//定长
	size_t nSize = sizeof(HEAD_MEM) +  //GEerenalInfo
		sizeof(AIN)*m_arrTempAins.size() +
		sizeof(DIN)*m_arrTempDins.size() +
		sizeof(AOUT)*m_arrTempAouts.size() +
		sizeof(DOUT)*m_arrTempDouts.size() +
		sizeof(NODE)*m_arrTempNodes.size() +
		sizeof(CHANNEL)*m_arrTempChannels.size() +
		sizeof(DEVICE)*m_arrTempDevices.size() +
		sizeof AIN_ALARM * m_arrTempAinAlarms.size() +
		sizeof AIN_ALARM_LIMIT * m_arrTempAinLimitAlarms.size() +
		sizeof DIN_ALARM * m_arrTempDinAlarms.size() +
		sizeof DIN_ALARM_LIMIT * m_arrTempDinLimitAlarms.size() +
		sizeof TRANSFORM_LINEAR * m_arrTempTransFormLinears.size() +
		sizeof TRANSFORM_NONLINEAR * m_arrTempTransFormNonLinears.size()+
		sizeof VARDATA * m_arrTempSystemVariables.size() +
		sizeof VARDATA * m_arrTempUserVariables.size()
				;
	//变长
	for (int i = 0; i < m_pChannelMgr->GetChannelCount();++i)
	{
		nSize += m_pChannelMgr->GetChannel(i)->GetChannelInfoLen();
	}

	for (size_t i = 0; i < m_pChannelMgr->GetDevCount(); i++)
	{
		nSize += m_pChannelMgr->GetDevice(i)->GetDevLen();
	}
	
	// 稍微预留一点
	nSize = static_cast<size_t>(nSize*1.1);

	char * pData =(char* ) m_pMem->CreateShareMem(szDBName, nSize);

	Q_ASSERT(pData);
	if (pData == nullptr)
	{
		// todo log
		LogMsg(QObject::tr("Fes::Create SHM failed!").toStdString().c_str(), 0);
		return;
	}

	memset(pData,0,nSize);

	Q_ASSERT(m_nMyNodeOccNo != INVALID_OCCNO && m_nMyNodeOccNo <= MAX_NODE_OCCNO && m_nMyNodeOccNo <= m_nNodeCount);

	m_pMem->Lock();

	//1. MAGIC
	m_pMagicMem = (HEAD_MEM*)(pData);
	m_pMagicMem->MagicHead1 = MAGIC_HEAD;
	m_pMagicMem->MagicHead2 = MAGIC_HEAD;
	m_pMagicMem->ShmLength = nSize;
	m_pMagicMem->MyNodeOccNo = m_nMyNodeOccNo;
	strncpy(m_pMagicMem->Config, szDBName, qMin((size_t)MEM_CONFIG_LEN, strlen(szDBName)));

	//2. RDB

	size_t nLen = 0;
	pData += sizeof HEAD_MEM;
	//NODE
	nLen=BuildNodeDB(pData);
	pData += nLen;
	//CHANNEL
	nLen = BuildChannelDB(pData);
	pData += nLen;
	//DEVICE
	nLen = BuildDeviceDB(pData);
	pData += nLen;

	//告警表在AIN 之前创建

	//AIN_ALARM
	nLen = BuildAinAlarmDB(pData);
	pData += nLen;

	//DIN_ALARM
	nLen = BuildDinAlarmDB(pData);
	pData += nLen;

	//AIN
	nLen = BuildAinDB(pData);
	pData += nLen;
	//DIN
	nLen = BuildDinDB(pData);
	pData += nLen;
	//AOUT
	nLen = BuildAoutDB(pData);
	pData += nLen;
	//DOUT
	nLen = BuildDoutDB(pData);
	pData += nLen;
	//Linear
	nLen = BuildLinearDB(pData);
	pData += nLen;
	//NonLinear
	nLen = BuildNonLinearDB(pData);
	pData += nLen;

	nLen = BuildSystemVariableDB(pData);
	pData += nLen;

	nLen = BuildUserVariableDB(pData);
	pData += nLen;

	//EXTRA
	nLen=BuildExtraDB(pData);

	OutPutShm(pData);

	m_pMem->UnLock();

};
/*! \fn void CDbSvc::GetChannelByOccNo(int nOccNo, CHANNEL** pChannel)
********************************************************************************************************* 
** \brief CDbSvc::GetChannelByOccNo 
** \details 通过OccNO 访问相应通道信息
** \param nOccNo 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetChannelByOccNo(int32u nOccNo, CHANNEL** pChannel)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO  && nOccNo<=MAX_CHANNEL_OCCNO);
	if (nOccNo== INVALID_OCCNO || nOccNo> MAX_CHANNEL_OCCNO)
	{
		return false;
	}
	if (nOccNo > m_nChannelCount)
	{
		return false;
	}
	*pChannel = &m_pChannels[nOccNo - 1];

	return true;
}
/*! \fn void CDbSvc::GetDeviceByOccNo(int nOccNo, DEVICE** pDev)
********************************************************************************************************* 
** \brief CDbSvc::GetDeviceByOccNo 
** \details 获取设备信息
** \param nOccNo 
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetDeviceByOccNo(int32u nOccNo, DEVICE** pDev)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO  && nOccNo <= MAX_DEVICE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_DEVICE_OCCNO)
	{
		return false;
	}
	if (nOccNo > m_nChannelCount)
	{
		return false;
	}

	*pDev = &m_pDevices[nOccNo - 1];

	return true;
}
/*! \fn void CDbSvc::GetAINByOccNo(int nOccNo, AIN** pAIN)
********************************************************************************************************* 
** \brief CDbSvc::GetAINByOccNo 
** \details 根据OccNo获取AIN点信息
** \param nOccNo 
** \param pAIN 
** \return void 
** \author xingzhibing
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetAinByOccNo(int32u nOccNo, AIN** pAIN)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAinCount)
	{
		return false;
	}

	*pAIN = &m_pAins[nOccNo - 1];

	return true;
}
/*! \fn void CDbSvc::GetDINByOccNo(int nOccNo, DIN** pDIN)
********************************************************************************************************* 
** \brief CDbSvc::GetDINByOccNo 
** \details 根据Occno获取DIN点信息
** \param nOccNo 
** \param pDIN 
** \return void 
** \author xingzhibing
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetDinByOccNo(int32u nOccNo, DIN** pDIN)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);

	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDinCount)
	{
		return false;
	}

	*pDIN = &m_pDins[nOccNo - 1];

	return true;
}
/*! \fn void CDbSvc::GetAoutByOccNO(int nOccNo, AOUT** pAout)
********************************************************************************************************* 
** \brief CDbSvc::GetAoutByOccNO 
** \details 获取AOUT信息
** \param nOccNo 
** \param pAout 
** \return void 
** \author xingzhibing
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetAoutByOccNo(int32u nOccNo, AOUT** pAout)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);

	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAoutCount)
	{
		return false;
	}

	*pAout = &m_pAouts[nOccNo - 1];

	return true;
}
/*! \fn void CDbSvc::GetDoutByOccNo(int nOccNo, DOUT** pDout)
********************************************************************************************************* 
** \brief CDbSvc::GetDoutByOccNo 
** \details 获取DOUT信息
** \param nOccNo 
** \param pDout 
** \return void 
** \author xingzhibing
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetDoutByOccNo(int32u nOccNo, DOUT** pDout)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);

	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDoutCount)
	{
		return false;
	}

	*pDout = &m_pDouts[nOccNo - 1];

	return true;
}
/*! \fn void CDbSvc::BuildTagNamePoolDB(const char* szName)
********************************************************************************************************* 
** \brief CDbSvc::BuildTagNamePoolDB 
** \details 创建tagname内存池
** \param szName 
** \return void 
** \author xingzhibing
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
void CDbSvc::BuildTagNamePoolDB(const char* szName)
{
	Q_ASSERT(szName);

	size_t nSize = sizeof(HEAD_MEM) +  //GEerenalInfo
		sizeof(TAG_OCCNO)*m_arrTempAins.size() +
		sizeof(TAG_OCCNO)*m_arrTempDins.size() +
		sizeof(TAG_OCCNO)*m_arrTempAouts.size() +
		sizeof(TAG_OCCNO)*m_arrTempDouts.size() +
		sizeof(TAG_OCCNO)*m_arrTempNodes.size() +
		sizeof(TAG_OCCNO)*m_arrTempChannels.size() +
		sizeof(TAG_OCCNO)*m_arrTempDevices.size() +
		sizeof TAG_OCCNO * m_arrTempAinAlarms.size() +
		sizeof TAG_OCCNO * m_arrTempAinLimitAlarms.size() +
		sizeof TAG_OCCNO * m_arrTempDinAlarms.size() +
		sizeof TAG_OCCNO * m_arrTempDinLimitAlarms.size() +
		sizeof TAG_OCCNO * m_arrTempTransFormLinears.size() +
		sizeof TAG_OCCNO * m_arrTempTransFormNonLinears.size()+
		sizeof TAG_OCCNO * m_arrTempSystemVariables.size()+
		sizeof TAG_OCCNO* m_arrTempUserVariables.size()
		;

	// 稍微预留一点
	nSize = static_cast<size_t>(nSize*1.1);

	char * pData = (char*)m_pTagMem->CreateShareMem(szName, nSize);
	Q_ASSERT(pData);
	if (pData == nullptr)
	{
		// todo log

		return;
	}

	memset(pData, 0, nSize);

	m_pMem->Lock();

	size_t nLen = 0;

	//1. MAGIC
	m_pMagicTagMem = (HEAD_MEM*)(pData);
	m_pMagicTagMem->MagicHead1 = MAGIC_HEAD;
	m_pMagicTagMem->MagicHead2 = MAGIC_HEAD;
	m_pMagicTagMem->ShmLength = nSize;
	strncpy(m_pMagicTagMem->Config, szName, qMin((size_t)MEM_CONFIG_LEN, strlen(szName)));
	pData =(char*)pData+ sizeof HEAD_MEM;
	//2. RDB

	//NODE
	nLen = BuildTagNameDB(pData, m_arrNodeOccNos);
	pData +=  nLen;
	m_pMagicTagMem->NodeCount = m_arrNodeOccNos.size();
	m_pMagicTagMem->NodeTableOffset = sizeof HEAD_MEM;

	//CHANNEL
	nLen = BuildTagNameDB(pData,m_arrChannelOccNos);
	pData +=  nLen;
	m_pMagicTagMem->ChannelCount = m_arrChannelOccNos.size();
	m_pMagicTagMem->ChannelTableOffset = m_pMagicTagMem->NodeTableOffset + m_pMagicTagMem->NodeCount* sizeof TAG_OCCNO;

	//DEVICE
	nLen = BuildTagNameDB(pData,m_arrDeviceOccNos);
	pData += nLen;
	m_pMagicTagMem->DeviceCount = m_arrDeviceOccNos.size();
	m_pMagicTagMem->DeviceTableOffset = m_pMagicTagMem->ChannelTableOffset+ m_arrTempChannels.size()* sizeof TAG_OCCNO;

	//AIN_ALARM
	nLen = BuildTagNameDB(pData, m_arrAinAlarmOccNos);
	pData += nLen;
	m_pMagicTagMem->AinAlarmCount = m_arrAinAlarmOccNos.size();
	m_pMagicTagMem->AinAlarmTableOffset = m_pMagicTagMem->DeviceTableOffset+ m_arrTempDevices.size()* sizeof TAG_OCCNO;

	//AIN_ALARM_LIMIT
	nLen = BuildTagNameDB(pData, m_arrAinAlarmLimitOccNos);
	pData += nLen;
	m_pMagicTagMem->AinAlarmLimitCount = m_arrAinAlarmLimitOccNos.size();
	m_pMagicTagMem->AinAlarmLimitTableOffset = m_pMagicTagMem->AinAlarmTableOffset +m_arrTempAinAlarms.size()* sizeof TAG_OCCNO;

	//DIN_ALARM
	nLen = BuildTagNameDB(pData, m_arrDinAlarmOccNos);
	pData += nLen;
	m_pMagicTagMem->DinAlarmCount = m_arrDinAlarmOccNos.size();
	m_pMagicTagMem->DinAlarmLimtTableOffset = m_pMagicTagMem->AinAlarmLimitTableOffset+m_arrTempAinLimitAlarms.size()*sizeof TAG_OCCNO;

	//DIN_ALARM_LIMIT
	nLen = BuildTagNameDB(pData, m_arrDinAlarmLimitOccNos);
	pData += nLen;
	m_pMagicTagMem->DinAlarmLimitCount = m_arrTempDinLimitAlarms.size();
	m_pMagicTagMem->DinAlarmLimtTableOffset = m_pMagicTagMem->DinAlarmLimtTableOffset+m_arrTempDinAlarms.size() * sizeof TAG_OCCNO;

	//AOUT
	nLen = BuildTagNameDB(pData, m_arrAoutOccNos);
	pData += nLen;
	m_pMagicTagMem->AoutCount = m_arrTempAouts.size();
	m_pMagicTagMem->AoutTableOffset = m_pMagicTagMem->DinAlarmLimtTableOffset +m_arrTempDinLimitAlarms.size()* sizeof TAG_OCCNO;

	//DOUT
	nLen = BuildTagNameDB(pData, m_arrDoutOccNos);
	pData += nLen;
	m_pMagicTagMem->DoutCount = m_arrTempDouts.size();
	m_pMagicTagMem->DountTableOffset = m_pMagicTagMem->AoutTableOffset +m_arrTempAouts.size() * sizeof TAG_OCCNO;

	//AIN
	nLen = BuildTagNameDB(pData, m_arrAinOccNos);
	pData += nLen;
	m_pMagicTagMem->AinCount = m_arrTempAins.size();
	m_pMagicTagMem->AinTableOffset =  m_pMagicTagMem->DountTableOffset+ m_arrTempDouts.size() * sizeof TAG_OCCNO;

	//DIN
	nLen = BuildTagNameDB(pData, m_arrDinOccNos);
	pData += nLen;
	m_pMagicTagMem->DinCount = m_arrTempDins.size();
	m_pMagicTagMem->DinTableOffset =  m_pMagicTagMem->AinTableOffset + m_arrTempAins.size() * sizeof TAG_OCCNO;

	//system var
	nLen = BuildTagNameDB(pData, m_arrSyetemVarOccNos);
	pData += nLen;
	m_pMagicTagMem->SystemVariableCount = m_arrSyetemVarOccNos.size();
	m_pMagicTagMem->SystemVariableOffSet = m_pMagicTagMem->DinTableOffset + m_arrTempDins.size() * sizeof TAG_OCCNO;

	//user var
	nLen = BuildTagNameDB(pData, m_arrUserVarOccNos);
	pData += nLen;
	m_pMagicTagMem->UserVariableCount = m_arrUserVarOccNos.size();
	m_pMagicTagMem->UserVariableOffset = m_pMagicTagMem->SystemVariableOffSet + m_arrTempSystemVariables.size() * sizeof TAG_OCCNO;

	m_pMem->UnLock();

}
/*! \fn size_t CDbSvc::BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec)
********************************************************************************************************* 
** \brief CDbSvc::BuildTagNameDB 
** \details 开辟TagName资源
** \param pAddress 
** \param vec 
** \return size_t  获取地址偏移
** \author xingzhibing
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
size_t CDbSvc::BuildTagNameDB(char* pAddress,const  std::vector<TAG_OCCNO_DEF>& vec)
{
	Q_ASSERT(pAddress);

	if (pAddress==Q_NULLPTR || vec.empty())
	{
		return 0;
	}

	int  nIndex = 0;

	TAG_OCCNO* pTagOccNo=Q_NULLPTR;

	//基准地址
	TAG_OCCNO * pBaseTagOccNos = reinterpret_cast<TAG_OCCNO*>(pAddress);

	for (auto i:vec)
	{
		pTagOccNo =(TAG_OCCNO*)((char*) pBaseTagOccNos + sizeof TAG_OCCNO * nIndex);

		std::memcpy(pTagOccNo,i.get(),sizeof TAG_OCCNO);

		nIndex++;
	}

	return sizeof TAG_OCCNO * vec.size();
}
/*! \fn void CDbSvc::OutPutShm(char* pAddress)
********************************************************************************************************* 
** \brief CDbSvc::OutPutShm 
** \details 文件结构存储
** \param pAddress 
** \return void 
** \author xingzhibing
** \date 2016年9月28日 
** \note 
********************************************************************************************************/
void CDbSvc::OutPutShm(char* pAddress)
{
	Q_UNUSED(pAddress);

	QFile file("fsh.bin");

	file.open(QIODevice::WriteOnly);

	QDataStream out(&file);   

	char* pData = new char[m_pMagicMem->ShmLength + 1];

	memset(pData, 0, (m_pMagicMem->ShmLength + 1));

	std::memcpy(pData, (char*)m_pMagicMem, m_pMagicMem->ShmLength);

	pData[m_pMagicMem->ShmLength] = '\0';

	QByteArray arrayData(pData, m_pMagicMem->ShmLength);

	out << arrayData;

	delete []pData;

	file.close();
	
}
/*! \fn void CDbSvc::ClearTempArray()
********************************************************************************************************* 
** \brief CDbSvc::ClearTempArray 
** \details 强制清空临时内存
** \return void 
** \author LiJin 
** \date 2016年10月9日 
** \note 
********************************************************************************************************/
void CDbSvc::ClearTempArray()
{ 
	STLClearObject(&m_arrTempNodes);
	STLClearObject(&m_arrTempChannels);
	STLClearObject(&m_arrTempDevices);
	STLClearObject(&m_arrTempAins);
	STLClearObject(&m_arrTempDins);
	STLClearObject(&m_arrTempAouts);
	STLClearObject(&m_arrTempDouts); 
}

/*! \fn std::string  CDbSvc::GetTagName(int32u nOccNo, int32u nDataType) const
********************************************************************************************************* 
** \brief CDbSvc::GetTagName 
** \details 根据OCCNO获取tagname
** \param nOccNo 
** \param nDataType 
** \return std::string 
** \author LiJin 
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
std::string  CDbSvc::GetTagName(int32u nOccNo, int32u nDataType) const
{
	std::string szTagName;

	Q_ASSERT(nOccNo > 0 && nOccNo <= MAX_OCCNO);
	if (nOccNo == 0 || nOccNo > MAX_OCCNO)
	{
		return szTagName;
	}

	switch (nDataType)
	{
	case IDD_NODE:
		if (nOccNo <= m_arrNodeOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrNodeOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrNodeOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_AIN:
		if (nOccNo <= m_arrAinOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrAinOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrAinOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break; 
	case IDD_DIN:
		if (nOccNo <= m_arrDinOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrDinOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrDinOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_AOUT:
		if (nOccNo <= m_arrAoutOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrAoutOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrAoutOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_DOUT:
		if (nOccNo <= m_arrDoutOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrDoutOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrDoutOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_CHANNEL:
		if (nOccNo <= m_arrChannelOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrChannelOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrChannelOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_DEVICE:
		if (nOccNo <= m_arrDeviceOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrDeviceOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrDeviceOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_OBJECT:
		Q_ASSERT(false);
		break;
	case IDD_AINALARM:
		if (nOccNo <= m_arrAinAlarmOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrAinAlarmOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrAinAlarmOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_DINALARM:
		if (nOccNo <= m_arrDinAlarmOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrDinAlarmOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrDinAlarmOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_AINALARMLIMIT:
		if (nOccNo <= m_arrAinAlarmLimitOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrAinAlarmLimitOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrAinAlarmLimitOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_DINALARMLIMIT:
		if (nOccNo <= m_arrDinAlarmLimitOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrDinAlarmLimitOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrDinAlarmLimitOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_TRANSLINEAR:
		if (nOccNo <= m_arrTransLinearOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrTransLinearOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrTransLinearOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	case IDD_TRANSNONLINEAR:
		if (nOccNo <= m_arrTransNonLinearOccNos.size())
		{
			Q_ASSERT(nOccNo == m_arrTransNonLinearOccNos[nOccNo - 1]->OccNo);
			szTagName = m_arrTransNonLinearOccNos[nOccNo - 1]->TagName;
			Q_ASSERT(szTagName.empty() == false);
		}
		else
		{
			Q_ASSERT(false);
		}
		break;
	default:
		break;
	}
	return szTagName;
}
/*! \fn size_t  CDbSvc::GetIoChannelNames(std::vector<std::string>& arrTagNames)const
********************************************************************************************************* 
** \brief CDbSvc::GetIoChannelNames 
** \details 获取系统内部的所有通道的tagname
** \param arrTagNames  返回值，系统内部的所有通道的tagname
** \return size_t 
** \author LiJin 
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
size_t  CDbSvc::GetIoChannelNames(std::vector<std::string>& arrTagNames)const
{
	arrTagNames.clear();
	std::string szTagName;

	for (auto i : m_arrIoChannels)
	{
		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
		Q_ASSERT(szTagName.empty() == false);
		if (szTagName.empty() == false)
		{
			arrTagNames.push_back(szTagName);
		}
	}

	return arrTagNames.size();
}

size_t CDbSvc::GetRemoteChannelNames(std::vector<std::string>& arrTagNames)const
{

	return arrTagNames.size();
}

/*! \fn void CDbSvc::LoadDrivers()
********************************************************************************************************* 
** \brief CDbSvc::LoadDrivers 
** \details 加载所有的驱动模块
** \return void 
** \author LiJin 
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CDbSvc::LoadDrivers()
{
	for (auto i : m_arrIoChannels)
	{
	//	i->Start(m_szDataBaseName.c_str());
		i->Load( );
	}
}

void  CDbSvc::StartDrivers()
{
	for (auto i : m_arrIoChannels)
	{
		i->Start(m_szDataBaseName.c_str());	
	}
}

void  CDbSvc::StopDrivers()
{
	for (auto i : m_arrIoChannels)
	{
		i->Stop(m_szDataBaseName.c_str());
	}
}

void  CDbSvc::CreateMailBoxs()
{
	std::string szTagName;
	int nID = 0;

	for (auto i : m_arrIoChannels)
	{
		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
		Q_ASSERT(szTagName.empty() == false);
		if (szTagName.empty() == false)
		{
			CreateMailBox("FES", szTagName.c_str(), 100, &nID);
			Q_ASSERT(nID != 0);
			i->SetMailBoxID(nID);
		}
	}

	for (auto i : m_arrFTChannels)
	{
		szTagName = GetTagName(i->GetOccNo(), IDD_FTCHANNEL);
		Q_ASSERT(szTagName.empty() == false);
		if (szTagName.empty() == false)
		{
			CreateMailBox("FES", szTagName.c_str(), 100, &nID);
			Q_ASSERT(nID != 0);
			i->SetMailBoxID(nID);
		}
	}
}

void  CDbSvc::DestroyMailBoxs()
{
	std::string szTagName;
	for (auto i : m_arrIoChannels)
	{
		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
		Q_ASSERT(szTagName.empty() == false);
		if (szTagName.empty() == false)
		{
			DestroyMailBox("FES", szTagName.c_str());
		}
	}

	for (auto i : m_arrFTChannels)
	{
		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
		Q_ASSERT(szTagName.empty() == false);
		if (szTagName.empty() == false)
		{
			DestroyMailBox("FES", szTagName.c_str());
		}
	}
}

void CDbSvc::StartMainTask()
{
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CDbSvc::MainTask, this, nullptr));
		//m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CDbSvc::MainTask, this, nullptr);
	}
}

void CDbSvc::StopMainTask()
{
	SetTaskStopFlag();
	// 退出线程
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
}

void CDbSvc::MainTask(void *pImpl)
{
	bool bRet = false;
	//	LogString("start main task....", 2);
	qDebug() << "start main task....";
	int nAlarmMailBoxId = QueryMailBoxID("FES", "ALARM_WIN");
	Q_ASSERT(nAlarmMailBoxId);
	
	DMSG msg;
	memset(&msg,0, sizeof(DMSG));
	int nTestCount = 0;
	std::chrono::milliseconds dura(5);

	bRet = OpenPostOffice( "FES");
	Q_ASSERT(bRet);

	bRet = OpenMailBox("FES", "ALARM_WIN", &nAlarmMailBoxId);
	Q_ASSERT(bRet);

	LogMsg(QObject::tr("FES ::Main Thread Task Started!").toStdString().c_str(), 0);

	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CBaseModule::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit main task....";
			//			LogString("exit main task1....", 2);
			return;
		}

		/*QString szTemp;
		static int nTestNum = 1;
		
		QString szTime = QDateTime::currentDateTime().toString();
	//	szTemp = QString("Time:%1 Test mail %2").arg(szTime).arg(nTestNum);
		szTemp = QString("Time:%1 test log....").arg(nTestNum);
		LogMsg(szTemp.toStdString().c_str(), 2);
		//nTestNum++;
// 		szTemp = QString("你好 .... Time:%1 Test mail %2").arg(szTime).arg(nTestNum);
// 		LogMsg(szTemp.toStdString().c_str(), 2);
		LogDebugMsg(szTemp.toStdString().c_str(), 0, __FILE__, __LINE__, __FUNCTION__);
		nTestNum++;

		for (int i = 0; i < 100;++i)
		{
			QString szTemp;

			QString nTime = QDateTime::currentDateTime().toString();
			szTemp = QString("Time:%1Test mail %2").arg(nTime).arg(++nTestCount);

			msg.RecverID = nAlarmMailBoxId;
			msg.SenderID = 0;
			msg.Type = MSG_EVT_DIGI;
			msg.Size = 100;
			strncpy(reinterpret_cast <char*>(msg.Buff), szTemp.toStdString().c_str(), szTemp.toStdString().length());

			//bool SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime);
			bRet = SendMail("FES", &msg, 0);
			if (bRet)
			{
				//		std::this_thread::sleep_for(dura);		 
			}
		}
		*/


		// 		if (bRet != std::cv_status::timeout)
		// 		{
		// 			LogString("rly103", "exit main task....", 2);
		// 			return;
		// 		}
		//		LogString("rly103", "in main task....", 2);

		//DoSth();
	}
}

/*! \fn bool CDbSvc::BuildStringNamePool(const char* pszPathName)
********************************************************************************************************* 
** \brief CDbSvc::BuildStringNamePool 
** \details 创建内存池
** \param pszPathName 
** \return bool 
** \author xingzhibing
** \date 2017年3月9日 
** \note 
********************************************************************************************************/
bool CDbSvc::BuildStringNamePool(const char* pszPathName)
{
	Q_ASSERT(pszPathName && strlen(pszPathName)>0);
	if (!pszPathName || strlen(pszPathName) ==0)
	{
		return  false;
	}
	size_t nSize = m_arrStrings.size() * sizeof StringUnit  + sizeof HEAD_STRING;

	char* pData=reinterpret_cast<char *>(m_pStringMen->CreateShareMem(pszPathName, nSize));
	Q_ASSERT(pData);
	if (!pData)
	{
		LogMsg(QObject::tr("Fes:Create StringPool failed!").toStdString().c_str(), 0);
		return false;
	}
	memset(pData, 0, nSize);
	
	m_pStringMen->Lock();

	m_pMagicStringMem = (HEAD_STRING*)(pData);
	m_pMagicStringMem->MagicHead1 = MAGIC_HEAD;
	m_pMagicStringMem->MagicHead2 = MAGIC_HEAD;
	m_pMagicStringMem->TotalCount = m_arrStrings.size();
	m_pMagicStringMem->ShmLength = nSize;

	size_t nLen = 0;
	pData += sizeof HEAD_STRING;

	int nIndex = 0;

	m_pStringUnit = (StringUnit *)((unsigned char*)pData);

	StringUnit* pStringUnit = Q_NULLPTR;

	for (auto i:m_arrStrings)
	{
		pStringUnit = (StringUnit*)((unsigned char*)m_pStringUnit+ sizeof StringUnit* nIndex);
		memcpy(pStringUnit,i.get(),sizeof StringUnit);
		nIndex++;
	}
	m_pStringMen->UnLock();

	return true;
}

/*! \fn bool CDbSvc::LoadStringNamePool(const char* pszPatnName)
********************************************************************************************************* 
** \brief CDbSvc::LoadStringNamePool 
** \details 读取字符串池文件
** \param pszPatnName 
** \return bool 
** \author xingzhibing
** \date 2017年3月9日 
** \note 
********************************************************************************************************/
bool CDbSvc::LoadStringNamePool(const char* pszPatnName)
{
	Q_ASSERT(pszPatnName && strlen(pszPatnName )>0);
	if (!pszPatnName || strlen( pszPatnName )==0)
	{
		return false;
	}

	QFile file(pszPatnName);

	if (!file.open(QIODevice::ReadOnly | QFile::Text))
	{
		LogMsg(QObject::tr("string pool file open error,please check file!!").toStdString().c_str(), 0);
		return false;
	}

	QXmlStreamReader reader;
	reader.setDevice(&file);

	reader.readNext();

	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if (reader.name()=="StringPool")
			{
				ReadStringUnit(reader);
			}
		}
		else
		{
			reader.readNext();
		}
	}

	return true;
}

/*! \fn bool CDbSvc::ReadStringUnit(QXmlStreamReader& reader)
********************************************************************************************************* 
** \brief CDbSvc::ReadStringUnit 
** \details 读取每个字符串文件定义
** \param reader 
** \return bool 
** \author xingzhibing
** \date 2017年3月9日 
** \note 
********************************************************************************************************/
bool CDbSvc::ReadStringUnit(QXmlStreamReader& reader)
{
	reader.readNext();
	std::shared_ptr<StringUnit> pStringUnit;

	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if (reader.name()=="Element")
			{
				pStringUnit = std::make_shared<StringUnit>();

				pStringUnit->OccNo = reader.attributes().value("OccNo").toUInt();

				memset(pStringUnit->Desc, 0, MAX_NAME_LENGTH + STRING_PAD);
				strncpy(pStringUnit->Desc,reader.attributes().value("Description").toString().toStdString().c_str(),
					qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t (reader.attributes().value("Description").length())));

				pStringUnit->IsTranslate = reader.attributes().value("IsTranslate").toInt();

				memset(pStringUnit->Language, 0, MAX_NAME_LENGTH + STRING_PAD);
				strncpy(pStringUnit->Language, reader.attributes().value("Language").toString().toStdString().c_str(),
					qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(reader.attributes().value("Language").length())));
			
				m_arrStrings.push_back(pStringUnit);
				
				reader.readNext();
				if (reader.isEndElement())
				{
					reader.readNext();
				}
			}			
		}
		else
		{
			reader.readNext();
		}
	}
	return true;
}

/*! \fn bool CDbSvc::IsDBAlive(int nTimeout)
********************************************************************************************************* 
** \brief CDbSvc::IsDBAlive 
** \details 数据库是否正在工作
** \param nTimeout 
** \return bool false 数据库退出，true 数据库正常工作
** \author LiJin 
** \date 2016年11月1日 
** \note 
********************************************************************************************************/
bool CDbSvc::IsDBAlive(unsigned int nTimeout)
{
	if (m_pDBAliveFlag == nullptr)
		return false;

	ACE_Time_Value tm_val(0, nTimeout * 1000);

	int nRet = m_pDBAliveFlag->wait(&tm_val,0);
	// 注意 返回值
	return (nRet == 0) ? true : false;
}

/** @}*/
