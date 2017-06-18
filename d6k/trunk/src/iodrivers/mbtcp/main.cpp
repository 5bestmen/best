

#include <string>

#include "ace/Thread_Manager.h"

#include "fesapi\fesapi.h"

#include "tcpclient.h"

#include "modbustcp.h"


/*! \fn void RecvMailDoNull(int32u nChannelOccNo)
********************************************************************************************************* 
** \brief RecvMailDoNull 
** \details �����ں˷������ʼ������ǲ����κδ���
** \param nChannelOccNo 
** \return void 
** \author LiJin 
** \date 2015��12��18�� 
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

	//  ģ������֮������������䣬��ֹ��ǰ��ѹ�����������ʼ�����ֹ�������
	RecvMailDoNull(nChannelID);

	std::string strIp("127.0.0.1");
	int nPort = 502;

	//GetChannelIpandPort(nChannelID,strIp,nPort);

	CTcpClient m_tcpclient;

	//1�����ӹ���
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

	//2��ͨѶ����
	int32u nHostState = 0;
	while (IsDriverQuit(nChannelID)==K_FALSE)
	{
		nHostState = GetMyHostState();
		// �ٶ� 0 �Ǵӻ�״̬
		if (nHostState == 0)
		{
			// �ӻ��������κ�����
			RecvMailDoNull(nChannelID);
			continue;
		}

		// ���������

	}

	//3���رչ���
	m_tcpclient.Close();
}
*/
