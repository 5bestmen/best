/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :
�ļ�ʵ�ֹ��� :  
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author xingzhibing
*  @version 1.0
*  @date    2016.10.24
*/

#ifndef _CSIMU_CHANNEL_H
#define _CSIMU_CHANNEL_H
#pragma once

#include "channel.h"
#include "datatypes.h"

#include <memory>
 
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  


class CSimuChannel : public CIoChannel
{
public:
	CSimuChannel();
	explicit CSimuChannel(int32u nOccNo);

	virtual ~CSimuChannel();
public:
	virtual bool Start();
	virtual bool Stop() ;

	virtual bool LoadData(int32u nOccNo) ;

protected:
	void MainTask(void *pChannel);
	 
	int  RecvMsgDoNull();
	void PollingData();

	unsigned int ReqDinData(unsigned int nAddr);
	unsigned int ReqAinData(unsigned int nAddr);
	unsigned int WriteDoutData(unsigned int nAddr);
	unsigned int WriteAoutData(unsigned int nAddr);

private:
//	std::shared_ptr<CTcpClient> m_pTcpClient;

	void SetTaskStopFlag();
	void SetTaskStartFlag();

	bool IsStopped()
	{
		return m_bStopFlag;
	}
	mutable std::mutex m_LogLock;

	//! ������
	std::shared_ptr< std::thread > m_pMainTask;
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;
	bool m_bStopFlag;

private:
	bool    m_bIsThreadRunning;
	bool    m_bIsThreadStopped;
};


/** @}*/

#endif // _MODBUS_CHANNEL_H
