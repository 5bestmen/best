/*! @file
<PRE>
********************************************************************************
模块名       :  Tab
文件名       :  tabwidget.cpp
文件实现功能 :  分离器
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
*  @date    2015.11.15
*/

#include "icore/tabwidget.h"
#include "icore/base.h"

/*! \fn CTabWidget::CTabWidget(QObject *parent)
********************************************************************************************************* 
** \brief CTabWidget::CTabWidget 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTabWidget::CTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	this->setTabsClosable(true);

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTileWidget(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

/*! \fn CTabWidget::~CTabWidget()
********************************************************************************************************* 
** \brief CTabWidget::~CTabWidget 
** \details 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTabWidget::~CTabWidget()
{

}

/*! \fn void CTabWidget::closeTileWidget(int index)
********************************************************************************************************* 
** \brief CTabWidget::closeTileWidget 
** \details 保存widget数据
** \param index 
** \return void 
** \author gw
** \date 2016年2月18日 
** \note 
********************************************************************************************************/
void CTabWidget::closeTileWidget(int index)
{
	if (m_fnBeforeCloseTab)
	{
		m_fnBeforeCloseTab(m_pBeforeCloseTabData);
	}
	
	IBase *pBase = dynamic_cast<IBase *>(widget(index));
	Q_ASSERT(pBase);
	if (pBase)
	{
		pBase->Save();

		this->removeTab(index);

		widget(index)->deleteLater();
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CTabWidget::tabChanged 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年2月18日 
** \note 
********************************************************************************************************/
void CTabWidget::tabChanged(int index)
{

}
