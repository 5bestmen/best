/*! @file
<PRE>
********************************************************************************
模块名       :  Pdr模块
文件名       :  pdr.cpp
文件实现功能 :  Pdr模块
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
** \date 2015年12月9日 
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
** \date 2015年12月9日 
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
** \date 2015年12月9日 
** \note 
********************************************************************************************************/
void  CPdr::showMouseRightButton(const QPoint& point)
{
	Q_UNUSED(point);
}
