/*! @file hb_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  hb_svc.h
文件实现功能 :  UDP广播发送，接收，网络状态判定
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   UDP广播发送，接收，网络状态判定
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef HB_SVC_MODULE_H
#define HB_SVC_MODULE_H
 
#include "base_module.h"

#include <string>
#include "datatypes.h"

class ACE_SOCK_Dgram_Bcast;
 
class CHeartBeatSvc : public CBaseModule
{
public: 
	CHeartBeatSvc(CNetbusSvc* pServe);
	~CHeartBeatSvc();
protected:
	CHeartBeatSvc( );

	enum :unsigned short
	{
		HB_PORT = 18764,   //!< 广播端口

	};

public:
	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
protected:
	void SendBroadMsg();

	void RecvBroadMsg();
 
protected:
	// 广播
	std::shared_ptr<ACE_SOCK_Dgram_Bcast> m_pDgramBcast;
 
};

#endif // HB_SVC_MODULE_H


/** @}*/

