/*! @file
<PRE>
********************************************************************************
模块名       :  socket通信管理
文件名       :
文件实现功能 :
作者         :  HuangYuqing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author  HuangYuqing
*  @version 1.0
*  @date    2016.8.18
*/
#ifndef SOCKETMGR_H
#define SOCKETMGR_H

#include <QThread>
#include <QTimer>
#include "socket_client.h"
#include "readxml.h"

class CSocketMgr
{
public:
	CSocketMgr();
	~CSocketMgr();
	CSocketClient* GetSocketClient();

	void Init(const QString &strIp, int nPort);

private:
	CSocketClient* m_pSocketClient;

};
#endif // SOCKETMGR_H