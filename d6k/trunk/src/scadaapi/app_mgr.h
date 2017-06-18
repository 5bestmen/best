/*! @file app_mgr.h
<PRE>
********************************************************************************
ģ����       :  �Ա��ڵ������APP���й���
�ļ���       :  app_mgr.h
�ļ�ʵ�ֹ��� :  
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :   
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author LiJin
*  @version 1.0
*  @date    2016.09.23
*/

#ifndef APP_MGR_H
#define APP_MGR_H
#pragma once

#include "datatypes.h" 
#include <unordered_map>
#include <memory>
#include <vector>


class CAppMgrApi
{
public:
	CAppMgrApi();
	~CAppMgrApi();
public:
	bool Initialize(const char* pszProjName, int32u nMode);
	void Run();
	void Shutdown();
};
 

/** @}*/

#endif // APP_MGR_H
