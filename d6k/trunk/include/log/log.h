/*! @file log.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  log.h
�ļ�ʵ�ֹ��� :  LOGģ��ӿڶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   LOGģ��ӿڶ���
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
	// ����log����� ���������  szLogName ��̨��¼���ļ����ƣ�����FES��SCADA�������ļ�¼����FES_1.LOG SCADA_1.LOG
	LOGAPI bool CreateLog(const char* szLogName);
	LOGAPI bool DestroyLog(const char* szLogName);

	// ����log��������ʹ���ߵ��ã��ýӿ�ʹ����Ҫע�⣬ֻ����ģ������ģ�鲻��ͬһ������ʱʹ�á�
	LOGAPI bool OpenLog(const char* szLogName);
	LOGAPI bool CloseLog(const char* szLogName);

	LOGAPI bool LogMsg(const char * pszMoudleName, const char *szLogTxt, int nLevel, struct TIMEPAK *pTm);
	// LogDebugMsg("NAME", "LOG ...........", 1, __FILE__, __LINE__, __FUNCTION__);	  
	LOGAPI bool LogDebugMsg(const char * pszMoudleName, const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, struct TIMEPAK *pTm);

}


#endif // _LOG_API_H

/** @}*/
