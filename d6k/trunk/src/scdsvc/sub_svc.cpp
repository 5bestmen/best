/*! @file sub_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sub_svc.cpp
�ļ�ʵ�ֹ��� :	���ķ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ķ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.03
*******************************************************************************/
#include "sub_svc.h"
#include "log/log.h"
 

#include <QObject> 
#include <QString> 




CSubScriptionSvc::CSubScriptionSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	 
}

CSubScriptionSvc::~CSubScriptionSvc(void)
{
 
}
/*! \fn bool CSubScriptionSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbgSvc::Initialize 
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
** \note  
********************************************************************************************************/
bool CSubScriptionSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{




	return true;
}

void CSubScriptionSvc::Run()
{

}

void CSubScriptionSvc::Shutdown()
{

}



/** @}*/
