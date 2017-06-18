/*! @file
<PRE>
********************************************************************************
模块名       :  通道对话框
文件名       :  channeldialog.cpp
文件实现功能 :  通道对话框
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
*  @date    2015.11.17
*/

#include "channeldialog.h"
#include "singleton.h"
/*! \fn CChannelDialog::CChannelDialog(QWidget *parent, QString strName)
********************************************************************************************************* 
** \brief CChannelDialog::CChannelDialog 
** \details 
** \param parent 
** \param strName 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CChannelDialog::CChannelDialog(QWidget *parent, QString strName)
	: QMainWindow(parent)
{
	m_pTalbeView = new CTableView(this, strName);

	setCentralWidget(m_pTalbeView);

	//CSingleton *pDbi = CSingleton::GetInstance();
	//
	//m_pDbi = pDbi->GetDbi();

	m_pTalbeView->GetFieldInfo("FES_CHANNEL");
}

/*! \fn CChannelDialog::~CChannelDialog()
********************************************************************************************************* 
** \brief CChannelDialog::~CChannelDialog 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CChannelDialog::~CChannelDialog()
{

}
