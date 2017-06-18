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
** \param pszDataPath �������ָ������ӵ�ǰĿ¼�µ������ļ�ȥ��ȡ·��,��߸������ǹ���ȫ·������Ҫָ������
** \param nMode 
** \return extern " FES_EXPORT int 
** \author LiJin 
** \date 2016��9��3�� 
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
** \details ֹͣǰ��
** \return extern " FES_EXPORT int 
** \author LiJin 
** \date 2016��9��13�� 
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
** \details ��ʾʵʱ���ݵ�
** \return extern " FES_EXPORT int 
** \author LiJin 
** \date 2016��9��12�� 
** \note 
********************************************************************************************************/
extern "C" FES_EXPORT  int ShowDataView(const char *pszDataPath)
{
	s_pSvc->OnDataShow(pszDataPath);
	return 0;
}