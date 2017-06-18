/*! @file main_task.c
<PRE>
********************************************************************************
模块名       : MB 通信协议主任务
文件名       : main_task.c
文件实现功能 :  
作者         :
版本         :
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  任务入口.
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

	//1. 依次召唤485网络中各个装置的测值
	for ( i = 0; i < nDeviceNum; i++)
	{
		//2. 请求模拟量
	//	bRet = ReqAinData(  );
		if (bRet)
		{
		//	bRet = ReadData();
			if ( bRet )
			{// 成功

			}
			else
			{// 失败

			}
		}
		//3. 请求开关量

		//4. 如果有上位机命令的话,解析命令,并做操作报警  解析完后清空内容


	}
}
/*! \fn unsigned int CModbusTask::ReqAinData(unsigned int nAddr)
********************************************************************************************************* 
** \brief CModbusTask::ReqAinData 
** \details 召唤模拟量数据
** \param nAddr 
** \return unsigned int 
** \author LiJin 
** \date 2015年9月8日 
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
