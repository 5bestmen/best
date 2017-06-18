/*! @file app_main.h
<PRE>
********************************************************************************
模块名       :
文件名       :  app_main.h
文件实现功能 :  插件入口
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   插件入口
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.23
*******************************************************************************/
#ifndef DLL_LIB_H
#define DLL_LIB_H


#include "datatypes.h"

#include <QObject>
#include <vector>
#include <memory>

class CPpgf : public QObject
{
	Q_OBJECT

public:
	CPpgf();
	~CPpgf();

public:
	
	virtual bool Initalize(const char * pszProjectName);

	virtual void Run();

	virtual void Shutdown();

private:

	void GetDbData();

	void GetRealData();

	void ReadXml();

	void LogMsg(const char* logMsg, int nLevel);

private:

	
	

};

#endif // DLL_LIB_H



/** @}*/

