/*! @file main_task.c
<PRE>
********************************************************************************
ģ����       : MB ͨ��Э��������
�ļ���       : main_task.c
�ļ�ʵ�ֹ��� :  
����         :
�汾         :
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  �������.
*  @author LiJin
*  @version 1.0
*  @date    2010.02.16
*******************************************************************************/ 

#include "modbus_task.h"

CModbusTask::CModbusTask()
{
	//m_pEC = std::make_shared<CEnCode>();
	//m_pDE = std::make_shared<CEnCode>();
	//m_pSetValCmd = std::make_shared<SETVAL_CMD>();
}
CModbusTask::~CModbusTask()
{

}

void CModbusTask::PollingData()
{
	unsigned int nDeviceNum = 0;

	unsigned int i = 0;
	bool bRet = false;

	//1. �����ٻ�485�����и���װ�õĲ�ֵ
	for ( i = 0; i < nDeviceNum; i++)
	{
		//2. ����ģ����
	//	bRet = ReqAinData(  );
		if (bRet)
		{
		//	bRet = ReadData();
			if ( bRet )
			{// �ɹ�

			}
			else
			{// ʧ��

			}
		}
		//3. ���󿪹���

		//4. �������λ������Ļ�,��������,������������  ��������������


	}
}
/*! \fn unsigned int CModbusTask::ReqAinData(unsigned int nAddr)
********************************************************************************************************* 
** \brief CModbusTask::ReqAinData 
** \details �ٻ�ģ��������
** \param nAddr 
** \return unsigned int 
** \author LiJin 
** \date 2015��9��8�� 
** \note 
********************************************************************************************************/
unsigned int CModbusTask::ReqAinData(unsigned int nAddr)
{


	return 0;
}

unsigned int CModbusTask::ReqDinData(unsigned int nAddr)
{


	return 0;
}



/*@}*/
