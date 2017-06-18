/*! @file main_module.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  main_module.h
�ļ�ʵ�ֹ��� :  ��ģ��Ľӿڶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��ģ��Ľӿڶ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.05.11
*******************************************************************************/

#ifndef I_MAIN_MODULE_H
#define I_MAIN_MODULE_H

#include <QObject>

class IMainWindow;
class IMainModule : public QObject
{
public:
	virtual ~IMainModule()
	{
	}

	//���main uiָ�� ����ģ��ʵ�֣���dll����
	virtual IMainWindow *GetMainWindow() = 0;
	//
	virtual bool LogString(const char *pszModuleName,const char *szLogTxt, int nLevel) = 0;

	enum COMMMSG_TYPE
	{
		CMSG_SERIALPORT_SEND =1,
		CMSG_SERALPORT_RECV =2,
		CMSG_UDP_SEND = 3,
		CMSG_UDP_RECV = 4,
		CMSG_TCP_SEND = 5,
		CMSG_TCP_RECV = 6
	};

	virtual bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc) = 0;

private:	
};

#endif // I_MAIN_MODULE_H
