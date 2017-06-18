/*! @file main_module.h
<PRE>
********************************************************************************
模块名       :
文件名       :  main_module.h
文件实现功能 :  主模块的接口定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   主模块的接口定义
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

	//获得main ui指针 由主模块实现，供dll调用
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
