/*! @file
<PRE>
********************************************************************************
ģ����       :  Pdrģ��
�ļ���       :  pdr.cpp
�ļ�ʵ�ֹ��� :  Pdrģ��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.12.8
*/

#include "pdr.h"
#include "scadastudio/imainwindow.h"

/*! \fn CPdr::CPdr(ICoreui *pUi)
********************************************************************************************************* 
** \brief CPdr::CPdr 
** \details 
** \param pUi 
** \return  
** \author gw
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
CPdr::CPdr(IMainWindow *pUi)
	: QObject(pUi)
{
	m_pUi = pUi;

	connect((QObject *)pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
}

/*! \fn CPdr::~CPdr()
********************************************************************************************************* 
** \brief CPdr::~CPdr 
** \details 
** \return  
** \author gw
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
CPdr::~CPdr()
{

}

/*! \fn void  CPdr::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CPdr::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
void  CPdr::showMouseRightButton(const QPoint& point)
{
	Q_UNUSED(point);
}
