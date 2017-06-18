

#include <string>

#include "ace/Thread_Manager.h"

#include "fesapi\fesapi.h"

#include "tcpclient.h"

#include "modbustcp.h"


/*! \fn void RecvMailDoNull(int32u nChannelOccNo)
********************************************************************************************************* 
** \brief RecvMailDoNull 
** \details 接收内核发来的邮件，但是不做任何处理
** \param nChannelOccNo 
** \return void 
** \author LiJin 
** \date 2015年12月18日 
** \note 
********************************************************************************************************/
/*
void RecvMailDoNull(int32u nChannelOccNo)
{
	bool bRet = false;
	FES_CMD cmd;

	while (true)
	{
		bRet = ReadHostCmd(nChannelOccNo, &cmd, 10);
		if (bRet == false)
		{
			break;
		}
	}
}
*/

extern "C" __declspec(dllexport) int drivermain(int nChannelOccNo)
{
	ACE_ASSERT(nChannelOccNo > 0 && nChannelOccNo <= MAX_OCCNO);
	if (nChannelOccNo <= 0 || nChannelOccNo > MAX_OCCNO)
		return K_FALSE;

	CModbusTcpThreadParam* pThreadParam = new CModbusTcpThreadParam(nChannelOccNo);
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)ModbusTCPThreadProc, pThreadParam);

	return 0;
}

/*
int32u  MainTask(int32u nChannelID)
{
	if (OpenIO(nChannelID) == K_FALSE)
		return 0;

	//  模块启动之后，清空所有邮箱，防止此前积压下来的垃圾邮件，防止误操作。
	RecvMailDoNull(nChannelID);

	std::string strIp("127.0.0.1");
	int nPort = 502;

	//GetChannelIpandPort(nChannelID,strIp,nPort);

	CTcpClient m_tcpclient;

	//1、连接过程
#if 0
	while (true)
	{
		if (m_tcpclient.Open((char*)strIp.c_str(), nPort))
			break;

		// 
		if (IsDriverQuit(nChannelID)==K_TRUE)
			return 1;
	}
#endif

	//2、通讯过程
	int32u nHostState = 0;
	while (IsDriverQuit(nChannelID)==K_FALSE)
	{
		nHostState = GetMyHostState();
		// 假定 0 是从机状态
		if (nHostState == 0)
		{
			// 从机，不做任何事情
			RecvMailDoNull(nChannelID);
			continue;
		}

		// 如果是主机

	}

	//3、关闭过程
	m_tcpclient.Close();
}
*/
