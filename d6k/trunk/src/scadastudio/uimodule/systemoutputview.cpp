/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出view
文件名       :  outputmessagerserver.cpp
文件实现功能 :  系统输出view
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
** \date 2015年11月30日 
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
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
CSystemOutputView::~CSystemOutputView()
{

}

void CSystemOutputView::resizeEvent(QResizeEvent *event) 
{


	QTableView::resizeEvent(event);
}
