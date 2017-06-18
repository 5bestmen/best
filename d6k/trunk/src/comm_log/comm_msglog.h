
#ifndef COMM_MSG_LOG_H
#define COMM_MSG_LOG_H
 
#include <string>
#include <QDateTime>
#include <vector>

#include "public.h"
 
 

class CCommMsgLog
{
public:
	CCommMsgLog()
	{
		m_nIndex = 0;

	 	m_nThreadId = 0;
		m_nProcessId = 0; 
	}
	
	CCommMsgLog(struct COMM_MSG *pLog);
	CCommMsgLog(CCommMsgLog *pCommLog);

	CCommMsgLog& operator= (const CCommMsgLog &);

	~CCommMsgLog()
	{

	}
public:
	// 序列化
	size_t SaveToBuffer(unsigned char *pInBuff,size_t nInBuffLen)const;
	bool  LoadFromBuffer(const char *pInBuff, size_t nInBuffLen);

// 	void SaveToLogData(LOG_DATA  *pData) const;
// 	bool LoadFromLogData(const LOG_DATA *pData);

	// 数据长度（包含同步头，长度等信息）
	size_t GetMsgSize() const
	{
		size_t nSyncHeadLen = sizeof(size_t);
		size_t nNextLen = sizeof(size_t);
		
		return GetDataSize() + nSyncHeadLen + nNextLen;
	}
private:
	// 有效数据长度（不包含同步头，长度等信息）
	size_t GetDataSize() const;

public:
	size_t m_nIndex;

	size_t   m_nThreadId;
	size_t   m_nProcessId;

	unsigned int m_nMsgType;       //! 报文消息的类型 比如：串口发送？UDP接收？TCP发送...
	std::string  m_szMsgSubType;   //! 子类型 字符串描述的类型说明，如启动帧、召唤、或者RECV、SEND，由使用者自由使用

	QDateTime m_tmLog;

	std::string m_szModuleName;     //! 模块名称
	std::string m_szProtocalName;   //! 协议名称，可以为空
	std::string m_szDescription;    //! 对这封报文的描述，可以为空


	std::string m_szDest;   //! 如果是以太网，则填目标机的IP:端口号，如果是串口，则填本机的串口号
	std::string m_szSrc;    //! 如果是以太网，则填本机的IP:端口号，如果是串口，则填本机的串口号

	std::vector<unsigned char> m_arrContent; //! 报文内容
};
 
 

#endif // COMM_MSG_LOG_H

