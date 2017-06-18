/*! @file base_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       : base_widget.cpp
�ļ�ʵ�ֹ��� : �����Ķ�̬����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  �����Ķ�̬����
*  @author LiJin
*  @version 1.0
*  @date    2016.02.14
*******************************************************************************/


#include "base_widget.h"
#include "base_dync.h"
 

CBaseDyncData::CBaseDyncData()
{
}

CBaseDyncData::~CBaseDyncData()
{
}


/*! \fn  bool CBaseDyncData::IsDyncTypeEqual(CBaseDyncData *pData) const
*********************************************************************************************************
** \brief   CBaseDyncData::IsDyncTypeEqual
** \details �Ƚ�������̬�����Ƿ���һ��������
** \param   pData
** \return bool
** \author LiJin
** \date  2010��12��20��
** \note
********************************************************************************************************/
bool CBaseDyncData::IsDyncTypeEqual(CBaseDyncData *pData) const
{
	Q_ASSERT(pData);
	if (pData == nullptr)
		return false;
	if (pData->m_nDyncType == this->m_nDyncType)
	{
		return true;
	}
	return false;
}
/*! \fn  bool CBaseDyncData::CheckDyncData( )
*********************************************************************************************************
** \brief   CBaseDyncData::CheckDyncData
** \details ��鶯̬���Ӳ����Ƿ�Ϸ�
** \return bool
** \author LiJin
** \date  2010��12��20��
** \note
********************************************************************************************************/
bool CBaseDyncData::CheckDyncData()
{
	if (m_szTagName.empty() == true)
		return false;
	return true;
}

bool CBaseDyncData::ProcessWidgetDync(CBaseWidget *pWidget)
{
	Q_ASSERT(pWidget);
	return false;
}

void CBaseDyncData::PrepareWidgetDync(CBaseWidget *pWidget)
{
	Q_UNUSED(pWidget); 
}

/** @}*/
