/*! @file log.h
<PRE>
********************************************************************************
模块名       :
文件名       :  log.h
文件实现功能 :  LOG模块接口定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   LOG模块接口定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/

#ifndef _LOG_API_H
#define _LOG_API_H

#include <QtCore/QtGlobal>
 
#ifdef LOG_LIB
# define LOGAPI Q_DECL_EXPORT
#else
# define LOGAPI Q_DECL_IMPORT
#endif

extern "C"
{
	struct TIMEPAK;
	// 创建log服务端 主程序调用  szLogName 后台记录的文件名称，比如FES，SCADA，产生的记录则是FES_1.LOG SCADA_1.LOG
	LOGAPI bool CreateLog(const char* szLogName);
	LOGAPI bool DestroyLog(const char* szLogName);

	// 连接log服务器，使用者调用，该接口使用需要注意，只有子模块与主模块不在同一进程内时使用。
	LOGAPI bool OpenLog(const char* szLogName);
	LOGAPI bool CloseLog(const char* szLogName);

	LOGAPI bool LogMsg(const char * pszMoudleName, const char *szLogTxt, int nLevel, struct TIMEPAK *pTm);
	// LogDebugMsg("NAME", "LOG ...........", 1, __FILE__, __LINE__, __FUNCTION__);	  
	LOGAPI bool LogDebugMsg(const char * pszMoudleName, const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, struct TIMEPAK *pTm);

}


#endif // _LOG_API_H

/** @}*/
