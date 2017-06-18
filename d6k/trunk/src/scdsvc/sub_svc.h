/*! @file sub_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  sub_svc.h
文件实现功能 :  订阅服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  订阅服务维护订阅清单，并根据清单定时扫描数据库，生成订阅报文，然后
                由网络总线服务定时发送该报文，考虑到实时问题报文不做队列式缓存。
				因此遥测可以采用订阅，遥信尽量不要采用订阅模式。
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   订阅服务
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.03
*******************************************************************************/
#ifndef SUB_SVC_MODULE_H
#define SUB_SVC_MODULE_H

#include "base_module.h"

#include <memory>
  
class CSubScriptionSvc : public CBaseModule
{
public:
	CSubScriptionSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &MailID);
	virtual ~CSubScriptionSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
private: 
};

#endif // SUB_SVC_MODULE_H


/** @}*/

