
#if _MSC_VER >= 1600
#define ENABLE_VLD
#ifdef ENABLE_VLD
#include <vld.h>
#endif
#endif


 
#include "scada_main.h"
#include "scdsvc.h"

#include "scdsvc_global.h"

#include <memory>


std::shared_ptr<CScadaSvc> s_pSvc;

/*! \fn extern "C" SCDSVC_API  int StartScada(const char *pszDataPath, unsigned int nMode) 
********************************************************************************************************* 
** \brief StartScada 
** \details 
** \param pszDataPath 如果不做指定，则从当前目录下的配置文件去读取路径,这边给定的是工程全路径，需要指定解析
** \param nMode 
** \return extern " SCDSVC_API int 
** \author LiJin 
** \date 2016年9月3日 
** \note 
********************************************************************************************************/
extern "C" SCDSVC_API  int StartScada(const char *pszDataPath, unsigned int nMode)
{
	s_pSvc = std::make_shared<CScadaSvc>();
	bool bRet=s_pSvc->Initialize(pszDataPath,nMode);
//	s_pSvc->Run();
	Q_ASSERT(bRet);
	if (bRet)
	{
		s_pSvc->Run();
	}
	else
	{
		s_pSvc.reset();
	}
	return 0;
}
/*! \fn extern "C" SCDSVC_API  int StopScada( )
********************************************************************************************************* 
** \brief StopScada 
** \details 停止SCADA
** \return extern " SCDSVC_API int 
** \author LiJin 
** \date 2016年9月13日 
** \note 
********************************************************************************************************/
extern "C" SCDSVC_API  int StopScada()
{
	s_pSvc->Shutdown();
	s_pSvc.reset();
	return 0;
}
/*! \fn extern "C" SCDSVC_API  int ShowDataView( )
********************************************************************************************************* 
** \brief ShowDataView 
** \details 显示实时数据等
** \return extern " SCDSVC_API int 
** \author LiJin 
** \date 2016年9月12日 
** \note 
********************************************************************************************************/
extern "C" SCDSVC_API  int ShowDataView()
{
//	s_pSvc->OnDataShow();
	return 0;
}