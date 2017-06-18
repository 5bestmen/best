/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ���view
�ļ���       :  outputmessagerserver.cpp
�ļ�ʵ�ֹ��� :  ϵͳ���view
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
*  @date    2015.11.30
*/

#include "systemoutputview.h"
#include <QHeaderView>
/*! \fn CSystemOutputView::CSystemOutputView(QWidget *parent)
********************************************************************************************************* 
** \brief CSystemOutputView::CSystemOutputView 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
CSystemOutputView::CSystemOutputView(QWidget *parent)
	: QTableView(parent)
{
	horizontalHeader()->setStretchLastSection(QHeaderView::Stretch);

	setAlternatingRowColors(true);
}

/*! \fn CSystemOutputView::~CSystemOutputView()
********************************************************************************************************* 
** \brief CSystemOutputView::~CSystemOutputView 
** \details 
** \return  
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
CSystemOutputView::~CSystemOutputView()
{

}

void CSystemOutputView::resizeEvent(QResizeEvent *event) 
{


	QTableView::resizeEvent(event);
}
