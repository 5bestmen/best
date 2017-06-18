/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :
文件实现功能 :  
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author LiJin
*  @version 1.0
*  @date    2007.09.11
*/

#ifndef _MODBUS_TASK_H
#define _MODBUS_TASK_H
#pragma once


#include "fesapi/fesapi.h"

#include <memory>


class CEnCode;
class CDeCode;
struct SETVAL_CMD;


class CModbusTask
{
public:
	CModbusTask();
	~CModbusTask();
public:
	bool InitTask( void *pIoSvr = nullptr);
	void StopTask( );
	int  RecvMsgDoNull();
	void PollingData();

private:
	unsigned int ReqDinData(unsigned int nAddr);
	unsigned int ReqAinData(unsigned int nAddr);
	unsigned int WriteDoutData(unsigned int nAddr);
	unsigned int WriteAoutData(unsigned int nAddr);


private:
	//! 编码
	std::shared_ptr<CEnCode> m_pEC;
//	CEnCode  m_ec;
	//! 解码
	std::shared_ptr<CDeCode> m_pDE;

//	CDeCode  m_de;


//	SETVAL_CMD m_SetValCmd;//设值命令
	std::shared_ptr<SETVAL_CMD> m_pSetValCmd;

};





/** @}*/

#endif // _MODBUS_TASK_H
