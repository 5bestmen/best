#include <QList>
#include "socketmgr.h"
#include "ui_mb_exchange.h"
#include "readxml.h"

#include "handle_thread.h"

CSocketMgr::CSocketMgr()
{
	m_pSocketClient = nullptr;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CSocketMgr::Init
** \details 初始化socket连接
** \return void
** \author HuangYuqing
** \date   2016年8月18日
** \note 
********************************************************************************************************/
void CSocketMgr::Init(const QString &strIp,int nPort)
{ 	

		m_pSocketClient = new CSocketClient();

		m_pSocketClient->ConnectSocket(strIp, nPort);

}

CSocketMgr::~CSocketMgr()
{
	if (m_pSocketClient != nullptr)
	{
		delete m_pSocketClient;
		m_pSocketClient = nullptr;
	}
}

//获取socketclient对象
CSocketClient* CSocketMgr::GetSocketClient()
{
	return m_pSocketClient;
}
