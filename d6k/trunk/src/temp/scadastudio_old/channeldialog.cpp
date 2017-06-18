/*! @file
<PRE>
********************************************************************************
ģ����       :  ͨ���Ի���
�ļ���       :  channeldialog.cpp
�ļ�ʵ�ֹ��� :  ͨ���Ի���
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
** \date 2015��11��17�� 
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
CChannelDialog::~CChannelDialog()
{

}
