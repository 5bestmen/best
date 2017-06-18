

#include "comm_msglog.h"

#include "public.h"
#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QThread>

#include "log/comm_log.h"

CCommMsgLog::CCommMsgLog(struct COMM_MSG *pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;

	Q_ASSERT(pLog->ModuleName && strlen(pLog->ModuleName));
	if (pLog->ModuleName && strlen(pLog->ModuleName))
	{
		m_szModuleName = pLog->ModuleName;
	}
	else
	{
		m_szModuleName = "Unkown";
	}
	 
	// 规约名称可以为空
	if (pLog->ProtocalName && strlen(pLog->ProtocalName))
	{
		m_szProtocalName = pLog->ProtocalName;
	} 
	// 备注描述可以为空
	if (pLog->Description && strlen(pLog->Description))
	{
		m_szDescription = pLog->Description;
	}

	Q_ASSERT(pLog->Source && strlen(pLog->Source));
	if (pLog->Source && strlen(pLog->Source))
	{
		m_szSrc = pLog->Source;
	}

	Q_ASSERT(pLog->Dest && strlen(pLog->Dest));
	if (pLog->Dest && strlen(pLog->Dest))
	{
		m_szDest = pLog->Dest;
	}

	Q_ASSERT(pLog->MsgBuff && pLog->MsgLen >0);
	if (pLog->MsgBuff && pLog->MsgLen)
	{
		if (pLog->MsgLen > MAX_MSG_LENGTH)
		{
			pLog->MsgLen = MAX_MSG_LENGTH;
		}
		m_arrContent.reserve(pLog->MsgLen);

		unsigned int i = 0;
		for (i = 0; i < pLog->MsgLen;i++)
		{
			m_arrContent.emplace_back(pLog->MsgBuff[i]);
		}
	}

	m_nMsgType = pLog->MsgType;       //! 报文消息的类型 比如：串口发送？UDP接收？TCP发送...

	if (pLog->MsgSubType && strlen(pLog->MsgSubType))
	{
		m_szMsgSubType = pLog->MsgSubType;    //! 子类型 字符串描述的类型说明，如启动帧、召唤、或者RECV、SEND，由使用者自由使用
	}

	m_nThreadId = pLog->ProcessID;
	m_nProcessId = pLog->ThreadID;

	if (pLog->MsgTm)
	{
		Q_ASSERT(pLog->MsgTm->Year > 1978 && pLog->MsgTm->Year < 2500);
		Q_ASSERT(pLog->MsgTm->Month > 0 && pLog->MsgTm->Month < 13);
		Q_ASSERT(pLog->MsgTm->Day > 0 && pLog->MsgTm->Day < 32);
		Q_ASSERT(pLog->MsgTm->Hour >= 0 && pLog->MsgTm->Hour < 25);
		Q_ASSERT(pLog->MsgTm->Minute >= 0 && pLog->MsgTm->Minute < 61);
		Q_ASSERT(pLog->MsgTm->Second >= 0 && pLog->MsgTm->Second < 61);
		Q_ASSERT(pLog->MsgTm->Milliseconds > 0 && pLog->MsgTm->Milliseconds < 1000);

		QDate dt;
		dt.setDate(pLog->MsgTm->Year, pLog->MsgTm->Month, pLog->MsgTm->Day);
		m_tmLog.setDate(dt);

		QTime tm;
		tm.setHMS(pLog->MsgTm->Hour, pLog->MsgTm->Minute, pLog->MsgTm->Second, pLog->MsgTm->Milliseconds);
		m_tmLog.setTime(tm);
	}
	else
	{
		m_tmLog = QDateTime::currentDateTime();
	}
}

CCommMsgLog::CCommMsgLog(CCommMsgLog *pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;
	m_nIndex = pLog->m_nIndex;

	m_szModuleName = pLog->m_szModuleName;     //! 模块名称
	m_szProtocalName = pLog->m_szProtocalName;   //! 协议名称，可以为空
	m_szDescription = pLog->m_szDescription;    //! 对这封报文的描述，可以为空

	m_nMsgType = pLog->m_nMsgType;       //! 报文消息的类型 比如：串口发送？UDP接收？TCP发送...
	m_szMsgSubType = pLog->m_szMsgSubType;    //! 子类型 字符串描述的类型说明，如启动帧、召唤、或者RECV、SEND，由使用者自由使用

	m_nProcessId = pLog->m_nProcessId;
	m_nThreadId = pLog->m_nThreadId;
	m_tmLog = pLog->m_tmLog;

	m_szDest = pLog->m_szDest;   //! 如果是以太网，则填目标机的IP:端口号，如果是串口，则填本机的串口号
	m_szSrc = pLog->m_szSrc;    //! 如果是以太网，则填本机的IP:端口号，如果是串口，则填本机的串口号

	m_arrContent = pLog->m_arrContent; //! 报文内容
}

CCommMsgLog& CCommMsgLog::operator= (const CCommMsgLog & other)
{
	if (this == &other)
	{
		return *this;
	}
	m_nIndex = other.m_nIndex;
	
	m_szModuleName = other.m_szModuleName;     //! 模块名称
	m_szProtocalName = other.m_szProtocalName;   //! 协议名称，可以为空
	m_szDescription = other.m_szDescription;    //! 对这封报文的描述，可以为空

	m_nMsgType = other.m_nMsgType;       //! 报文消息的类型 比如：串口发送？UDP接收？TCP发送...
	m_szMsgSubType = other.m_szMsgSubType;    //! 子类型 字符串描述的类型说明，如启动帧、召唤、或者RECV、SEND，由使用者自由使用

	m_nProcessId = other.m_nProcessId;
	m_nThreadId = other.m_nThreadId;
	m_tmLog = other.m_tmLog;
	 
	m_szDest = other.m_szDest;   //! 如果是以太网，则填目标机的IP:端口号，如果是串口，则填本机的串口号
	m_szSrc = other.m_szSrc;    //! 如果是以太网，则填本机的IP:端口号，如果是串口，则填本机的串口号

	m_arrContent = other.m_arrContent; //! 报文内容
	 
	return *this;
}
/*! \fn size_t CCommMsgLog::GetDataSize() const
*********************************************************************************************************
** \brief CCommMsgLog::GetDataSize
** \details 有效内容长度，字符串的存储方式：长度+内容
** \return size_t
** \author LiJin
** \date 2016年12月30日
** \note
********************************************************************************************************/
size_t CCommMsgLog::GetDataSize() const
{
	size_t nOffset = 0;
	size_t nSize = 0;

	nSize = sizeof(m_nIndex);
	nOffset += nSize;


	nSize = sizeof(m_nThreadId);
	nOffset += nSize;

	nSize = sizeof(m_nProcessId);
	nOffset += nSize;
	
	nSize = sizeof(m_nMsgType);
	nOffset += nSize;
	
	nSize = sizeof(TIMEPAK);
	nOffset += nSize;

	size_t nLen = 0;

	nLen = m_szModuleName.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szProtocalName.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szMsgSubType.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szDescription.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szDest.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szSrc.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_arrContent.size();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	return nOffset;
}
/*! \fn size_t CCommMsgLog::SaveToBuffer(unsigned char *pInBuff, size_t nInBuffLen) const
*********************************************************************************************************
** \brief CCommMsgLog::SaveToBuffer
** \details 序列化
** \param pInBuff
** \param nInBuffLen
** \return size_t
** \author LiJin
** \date 2016年12月30日
** \note
********************************************************************************************************/
size_t CCommMsgLog::SaveToBuffer(unsigned char *pInBuff, size_t nInBuffLen) const
{
	Q_ASSERT(pInBuff);
	if (pInBuff == nullptr)
		return 0;
	Q_ASSERT(nInBuffLen > sizeof(m_nIndex));
	if (nInBuffLen < sizeof(m_nIndex))
		return 0;

	size_t nOffset = 0;
	size_t nSize = 0;
	//	size_t nRet = 0;

	nSize = GetDataSize();
	if (nSize > nInBuffLen)
	{
		Q_ASSERT(false);
		return 0;
	}

	size_t nSync = 0x494c494c;
    // 同步头
	std::memcpy(&pInBuff[nOffset], &nSync, sizeof(size_t));
	nOffset += sizeof(size_t);
	// 长度
	std::memcpy(&pInBuff[nOffset], &nSize, sizeof(size_t));
	nOffset += sizeof(size_t);
	// 正文
	nSize = sizeof(m_nIndex);
	std::memcpy(&pInBuff[nOffset], &m_nIndex, nSize);
	nOffset += nSize;

	nSize = sizeof(m_nThreadId);
	std::memcpy(&pInBuff[nOffset], &m_nThreadId, nSize);
	nOffset += nSize;

	nSize = sizeof(m_nProcessId);
	std::memcpy(&pInBuff[nOffset], &m_nProcessId, nSize);
	nOffset += nSize;

	nSize = sizeof(m_nMsgType);
	std::memcpy(&pInBuff[nOffset], &m_nMsgType, nSize);
	nOffset += nSize;

		// 时间
	//m_tmLog.toTime_t()
	TIMEPAK tp;
	tp.Year = m_tmLog.date().year();
	tp.Month = m_tmLog.date().month();
	tp.Day = m_tmLog.date().day();
	tp.DayOfWeek = m_tmLog.date().dayOfWeek();

	tp.Hour = m_tmLog.time().hour();
	tp.Minute = m_tmLog.time().minute();
	tp.Second = m_tmLog.time().second();
	tp.Milliseconds = m_tmLog.time().msec();

	nSize = sizeof(tp);
	std::memcpy(&pInBuff[nOffset], &tp, nSize);
	nOffset += nSize;

	size_t nLen = 0;
	// 字符串存储方式：先是长度，然后是内容。
	// 模块名称
	nLen = m_szModuleName.length();
	Q_ASSERT(nLen >= 0 && nLen < 2048);

	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szModuleName.data(), nLen);
		nOffset += nLen;
	}
	// 协议名称
	nLen = m_szProtocalName.length();
	Q_ASSERT(nLen >= 0 && nLen < 2048);

	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szProtocalName.data(), nLen);
		nOffset += nLen;
	}
	// 子类型说明
	nLen = m_szMsgSubType.length();
	Q_ASSERT(nLen >= 0 && nLen < 2048);

	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szMsgSubType.data(), nLen);
		nOffset += nLen;
	} 

	nLen = m_szDescription.length();
	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szDescription.data(), nLen);
		nOffset += nLen;
	}

	nLen = m_szDest.length();
	Q_ASSERT(nLen >= 0 && nLen < 2048);

	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szDest.data(), nLen);
		nOffset += nLen;
	}

	nLen = m_szSrc.length();
	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szSrc.data(), nLen);
		nOffset += nLen;
	}

	// 报文内容
	nLen = m_arrContent.size();
	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_arrContent.data(), nLen);
		nOffset += nLen;
	}

	return nOffset;
}

