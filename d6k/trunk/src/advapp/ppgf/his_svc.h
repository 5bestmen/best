/*! @file his_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  his_svc.h
文件实现功能 :  历史存储服务，定时存储实时数据到
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   历史存储服务
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef HIS_SVC_H
#define HIS_SVC_H

#include "base_module.h"

class CHisSvc : public CBaseModule
{
public:
	CHisSvc();
	virtual ~CHisSvc();
public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
protected:
	virtual void MainLoop();

};



 
 


#endif // HIS_SVC_H



/** @}*/

