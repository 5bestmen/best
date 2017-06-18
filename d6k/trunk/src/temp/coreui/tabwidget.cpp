/*! @file
<PRE>
********************************************************************************
ģ����       :  Tab
�ļ���       :  tabwidget.cpp
�ļ�ʵ�ֹ��� :  ������
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
** \date 2015��11��16�� 
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
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
CTabWidget::~CTabWidget()
{

}

/*! \fn void CTabWidget::closeTileWidget(int index)
********************************************************************************************************* 
** \brief CTabWidget::closeTileWidget 
** \details ����widget����
** \param index 
** \return void 
** \author gw
** \date 2016��2��18�� 
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
** \date 2016��2��18�� 
** \note 
********************************************************************************************************/
void CTabWidget::tabChanged(int index)
{

}
