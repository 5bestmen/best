/*! @file app_mgr.h
<PRE>
********************************************************************************
模块名       :  对本节点的所有APP进行管理
文件名       :  app_mgr.h
文件实现功能 :  
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :   
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
