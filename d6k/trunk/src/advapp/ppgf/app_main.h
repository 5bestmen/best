/*! @file app_main.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  app_main.h
�ļ�ʵ�ֹ��� :  ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ������
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

