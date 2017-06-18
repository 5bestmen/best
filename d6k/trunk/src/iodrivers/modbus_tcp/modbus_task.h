/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :
�ļ�ʵ�ֹ��� :  
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
	//! ����
	std::shared_ptr<CEnCode> m_pEC;
//	CEnCode  m_ec;
	//! ����
	std::shared_ptr<CDeCode> m_pDE;

//	CDeCode  m_de;


//	SETVAL_CMD m_SetValCmd;//��ֵ����
	std::shared_ptr<SETVAL_CMD> m_pSetValCmd;

};





/** @}*/

#endif // _MODBUS_TASK_H
