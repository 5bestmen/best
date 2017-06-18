/*! @file hb_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  hb_svc.cpp
文件实现功能 :  UDP广播发送，接收，网络状态判定
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   UDP广播发送，接收，网络状态判定
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#include "hb_svc.h"
#include "log/log.h" 
#include "nb_svc.h"
#include <QObject> 
#include <QString> 

#include "ace/SOCK_Dgram_Bcast.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SOCK_Dgram.h"
 

CHeartBeatSvc::CHeartBeatSvc(CNetbusSvc* pServe) :CBaseModule(pServe)
{

}

CHeartBeatSvc::~CHeartBeatSvc( )
{

}
/*! \fn bool CUdpSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CUdpSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CHeartBeatSvc::Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode)
{
	 

	return true;
}

void CHeartBeatSvc::Run()
{
 
}

void CHeartBeatSvc::Shutdown()
{ 
	 
}
 

void CHeartBeatSvc::SendBroadMsg()
{
	char t[100] = { 0 };
	if (m_pDgramBcast)
	{


		m_pDgramBcast->send(t, 100, HB_PORT);
	}
}

void CHeartBeatSvc::RecvBroadMsg()
{
	ACE_INET_Addr peer_addr;
	char   buf[255] = { 0 };
	int    nSize = 0;


	if (m_pDgramBcast)
	{
		nSize = m_pDgramBcast->recv(buf, 255, peer_addr);
		if (nSize<=0)
		{
			return;
		}
		// 解析报文 判断收到是不是本机发出的广播报文，是不是同一个子网


	}


}

/** @}*/
