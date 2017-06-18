/*! @file app_main.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  app_main.cpp
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
	** \details ����Ӧ��ģ��
	** \param pszName ����ʾ�����ƣ����Ϊ�յĻ������bin�������scada.xml�������ж�ȡ����ʵ������
	** \param nAppOccNo Ӧ�õ�OccNo�����Ϊ0��������ͬ
	** \param nExtraArgc �������
	** \param pszExtraArgv �������
	** \return SCDAPP int 
	** \author LiJin 
	** \date 2017��3��24�� 
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
	** \details �˳�Ӧ��ģ��
	** \param pszName 
	** \param nAppOccNo 
	** \return  int 
	** \author LiJin 
	** \date 2017��3��23�� 
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
