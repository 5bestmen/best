/*! @file hb_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  hb_svc.cpp
�ļ�ʵ�ֹ��� :  UDP�㲥���ͣ����գ�����״̬�ж�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   UDP�㲥���ͣ����գ�����״̬�ж�
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
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
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
		// �������� �ж��յ��ǲ��Ǳ��������Ĺ㲥���ģ��ǲ���ͬһ������


	}


}

/** @}*/
