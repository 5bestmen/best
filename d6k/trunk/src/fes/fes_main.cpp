/*

#if _MSC_VER >= 1600
#define ENABLE_VLD
#ifdef ENABLE_VLD
#include <vld.h>
#endif
#endif
*/



#include "fes.h"
#include "fes_main.h"

//#define MODULE_LIB
#include "module_export.h"

#include <memory>


std::shared_ptr<CFesSvc> s_pSvc;

/*! \fn extern "C" FES_EXPORT  int FesMain(const char *pszDataPath, unsigned int nMode) 
********************************************************************************************************* 
** \brief FesMain 
** \details 
** \param pszDataPath 如果不做指定，则从当前目录下的配置文件去读取路径,这边给定的是工程全路径，需要指定解析
** \param nMode 
** \return extern " FES_EXPORT int 
** \author LiJin 
** \date 2016年9月3日 
** \note 
********************************************************************************************************/
extern "C" FES_EXPORT  int StartFes(const char *pszDataPath, unsigned int nMode) 
{
	s_pSvc = std::make_shared<CFesSvc>();
	bool bRet = s_pSvc->Initialize(pszDataPath,nMode);
	Q_ASSERT(bRet);
	if (bRet)
	{
		s_pSvc->Run();
	}
	else
	{
		s_pSvc.reset();
	}
//	pSvc.reset();

	return 0;
}
/*! \fn extern "C" FES_EXPORT  int StopFes( )
********************************************************************************************************* 
** \brief StopFes 
** \details 停止前置
** \return extern " FES_EXPORT int 
** \author LiJin 
** \date 2016年9月13日 
** \note 
********************************************************************************************************/
extern "C" FES_EXPORT  int StopFes( )
{
	s_pSvc->Shutdown();

	return 0;
}
/*! \fn extern "C" FES_EXPORT  int ShowDataView( )
********************************************************************************************************* 
** \brief ShowDataView 
** \details 显示实时数据等
** \return extern " FES_EXPORT int 
** \author LiJin 
** \date 2016年9月12日 
** \note 
********************************************************************************************************/
extern "C" FES_EXPORT  int ShowDataView(const char *pszDataPath)
{
	s_pSvc->OnDataShow(pszDataPath);
	return 0;
}