/*! @file
<PRE>
********************************************************************************
ģ����       :  socketͨ�Ź���
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  HuangYuqing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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