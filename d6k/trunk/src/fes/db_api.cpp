/*! @file db_api.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  db_api.cpp
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
*  @date    2016.12.01
*******************************************************************************/

#include "fes.h" 
#include "db_svc.h" 
#include "mail/mail.h"
#include <QDebug>
 

/*! \fn AIN* CDbSvc::GetAinByIndex(int32u nIdx)
********************************************************************************************************* 
** \brief CDbSvc::GetAinByIndex 
** \details  根据序号获取模拟量功能块
** \param nIdx 
** \return AIN* 
** \author LiJin 
** \date 2016年11月30日 
** \note 
********************************************************************************************************/
AIN* CDbSvc::GetAinByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nAinCount);
	if (nIdx >= m_nAinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return & m_pAins[nIdx];
}

DIN*CDbSvc::GetDinByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nDinCount);
	if (nIdx >= m_nDinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pDins[nIdx];
}

AOUT*CDbSvc::GetAoutByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nAoutCount);
	if (nIdx >= m_nAinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pAouts[nIdx];
}

DOUT*CDbSvc::GetDoutByIndex(int32u nIdx)
{
	Q_ASSERT(nIdx < MAX_OCCNO && nIdx < m_nDoutCount);
	if (nIdx >= m_nAinCount || nIdx >= MAX_OCCNO)
		return nullptr;

	return &m_pDouts[nIdx];
}
/*! \fn int32s CDbSvc::GetChannelMailBoxID(int32u nChannelOccNo)
*********************************************************************************************************
** \brief CDbSvc::GetChannelMailBoxID
** \details 通过通道排行号，获取该通道的邮箱ID
** \param nChannelOccNo
** \return int
** \author LiJin
** \date 2016年12月12日
** \note
********************************************************************************************************/
int32s CDbSvc::GetChannelMailBoxID(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	if (nChannelOccNo > m_nChannelCount)
	{
		return 0;
	}

	Q_ASSERT(m_pChannels[nChannelOccNo - 1].MailBoxID);
	return m_pChannels[nChannelOccNo - 1].MailBoxID;
}
/*! \fn int32u CDbSvc::GetMyHostState()const
*********************************************************************************************************
** \brief CDbSvc::GetMyHostState
** \details
** \return int32u
** \author LiJin
** \date 2016年12月1日
** \note
********************************************************************************************************/
int32u CDbSvc::GetMyHostState() const
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return STATE_UNKOWN;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (m_pMagicMem->MyNodeOccNo == INVALID_OCCNO || m_pMagicMem->MyNodeOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	return m_pNodes[m_pMagicMem->MyNodeOccNo - 1].State;
}

int8u  CDbSvc::GetMyHostScanEnable()const
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return STATE_UNKOWN;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (m_pMagicMem->MyNodeOccNo == INVALID_OCCNO || m_pMagicMem->MyNodeOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	return m_pNodes[m_pMagicMem->MyNodeOccNo - 1].ScanEnable;
}
/*! \fn bool  CDbSvc::GetDoutBlockState(DOUT & dout)
*********************************************************************************************************
** \brief CDbSvc::GetDoutBlockState
** \details 检查开出闭锁状态
** \param dout
** \return bool
** \author LiJin
** \date 2017年2月17日
** \note
********************************************************************************************************/
void  CDbSvc::CheckDoutBlockState(DOUT & dout)
{
	if (dout.BlockingProgramOccNo != INVALID_OCCNO)
	{// 执行下开出闭锁程序，发信号给内核，由内核执行

	}
	else
	{
		if (dout.BlockingSignalOccNo != INVALID_OCCNO)
		{// 只有闭锁信号，没有闭锁程序
			int8u nValue = 0;
			int8u nQuality = 0;
			bool bRet = IoGetDinValue(dout.BlockingSignalOccNo, nValue, nQuality);
			Q_ASSERT(bRet);
			if (dout.BlockSignalType == NORMAL_CLOSE)
			{// 常闭信号，值为1
				if (nValue == 1)
				{
					dout.CheckOK = 1;
				}
				else
				{
					dout.CheckOK = 0;
				}
			}
			else
			{
				if (nValue == 0)
				{
					dout.CheckOK = 1;
				}
				else
				{
					dout.CheckOK = 0;
				}
			}
		}
	}
	return;
}
/*! \fn bool  CDbSvc::IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const
*********************************************************************************************************
** \brief CDbSvc::IoGetDinValue
** \details 获取DIN的测值以及品质
** \param nOccNo
** \param nValue
** \param nQuality
** \return bool
** \author LiJin
** \date 2016年10月9日
** \note
********************************************************************************************************/
bool  CDbSvc::IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDinCount)
	{
		return false;
	}

	DIN *pFB = & m_pDins[nOccNo - 1];

	nValue = pFB->Value;
	nQuality = pFB->Quality;

	return true;
}

bool  CDbSvc::IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAinCount)
	{
		return false;
	}

	AIN *pFB = &m_pAins[nOccNo - 1];

	fValue = pFB->Value;
	nQuality = pFB->Quality;

	return true;
}

bool CDbSvc::FesSetDoutValue(int32u nOccNo, int8u Value, int8u nSource)
{
	//! 判断 OccNo是否有效
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nDoutCount)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	DOUT *pFB = &m_pDouts[nOccNo - 1];

	if (pFB->IsSBO)
	{// SBO


	 //		return false;
	}

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
		CheckDoutBlockState(*pFB);
		return false;
	}

	// 检查条件，如果控制闭锁没有通过，则告警
	if (pFB->CheckOK == 0)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Because the remotecontrol's bloking condition is't ready.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		// 告警-提示运行人员： 遥控闭锁条件检查失败。

		return false;
	}
	//! 如果是启动程序，则 转发消息到 顺控模块（支持异机）
	if (pFB->StartProgramOccNo != INVALID_OCCNO)
	{

	}
	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_DOUT;
	pSetValeEvt->Att = ATTW_DOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
	pSetValeEvt->Source1 = nSource;
	pSetValeEvt->Datatype = DT_BOOLEAN;

	S_BOOL(&pSetValeEvt->Value[0], &Value);

	//! 再次封装成邮件发送MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	//! 判断是否关联遥信，如果有关联遥信，则发送到db_svc，由db_svc去做处理，此处需要注意超时处理
	if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
	{
// 		Q_ASSERT(m_nDbSvcMailBoxID != 0);
// 		dmsg.RecverID = m_nDbSvcMailBoxID;
	}
	else
	{// 否则直接下发给通道驱动
		if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
		else
		{
			dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
			if (dmsg.RecverID == 0)
			{
				szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
				LogMsg(szLog.toStdString().c_str(), 1);

				return false;
			}
		}
	}
	bool bRet = SendMail("FES", &dmsg, 0);
	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}

	return bRet;
}
// 写值
bool CDbSvc::FesSetAoutValue(int32u nOccNo, fp64 Value, int32u nSource)
{
	//! 判断 OccNo是否有效
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nAoutCount)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	AOUT *pFB = &m_pAouts[nOccNo - 1];

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;

		return false;
	}
	//! 获取最大、最小范围，如果超过范围，则报警提示，并且不设值下去
	if (Value < pFB->RangeL || Value > pFB->RangeH)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. The value is wrong.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}

	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_AOUT;
	pSetValeEvt->Att = ATTW_AOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
	pSetValeEvt->Source1 = nSource;
	pSetValeEvt->Datatype = DT_DOUBLE;

	S_DOUBLE(&pSetValeEvt->Value[0], &Value);

	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}
	else
	{
		dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
		if (dmsg.RecverID == 0)
		{
			szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
	}

	bool bRet = SendMail("FES", &dmsg, 0);
	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}

	return bRet;
}


/** @}*/
