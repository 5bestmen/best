/*! @file app_main.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  app_main.cpp
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

#include "scadaapi/scdapp_api.h"
#include "scadaapi/scdapp_def.h"
#include "log/log.h"
#include "app_main.h"

#include <QObject> 
#include <QDateTime>


std::shared_ptr<CPpgf> s_Ppgf;

extern "C"
{
	/*! \fn SCDAPP int StartScadaApp(const char * pszName, int32u nAppOccNo, int nExtraArgc,  char *pszExtraArgv[])
	********************************************************************************************************* 
	** \brief StartScadaApp 
	** \details 启动应用模块
	** \param pszName 工程示例名称，如果为空的话，则从bin下面加载scada.xml，从其中读取工程实例名称
	** \param nAppOccNo 应用的OccNo，如果为0，则与上同
	** \param nExtraArgc 额外参数
	** \param pszExtraArgv 额外参数
	** \return SCDAPP int 
	** \author LiJin 
	** \date 2017年3月24日 
	** \note 
	********************************************************************************************************/
	SCDAPP int StartScadaApp(const char * pszName, int32u nAppOccNo)
	{
		Q_ASSERT(pszName);
		if (pszName == nullptr)
		{
			return false;
		}
		if (s_Ppgf)
		{
			s_Ppgf.reset();
		}

		s_Ppgf = std::make_shared<CPpgf>();

		if (!s_Ppgf->Initalize(pszName))
		{
			return false;

		}

		return 0;
	}
	/*! \fn SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo)
	********************************************************************************************************* 
	** \brief StopScadaApp 
	** \details 退出应用模块
	** \param pszName 
	** \param nAppOccNo 
	** \return  int 
	** \author LiJin 
	** \date 2017年3月23日 
	** \note 
	********************************************************************************************************/
	SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo)
	{
		Q_ASSERT(pszName);
		if (pszName == 0)
		{
			return false;
		}

		return 0;
	}
}


CPpgf::CPpgf()
{

}


CPpgf::~CPpgf()
{


}

bool CPpgf::Initalize(const char * pszProjectName)
{
	Q_ASSERT(pszProjectName && strlen(pszProjectName) > 0);
	if (!pszProjectName || strlen(pszProjectName) == 0)
	{
		return false;
	}

	return true;

}


void CPpgf::LogMsg(const char* logMsg, int nLevel)
{
	::LogMsg("PPGF", logMsg, nLevel, nullptr);
}




/** @}*/
