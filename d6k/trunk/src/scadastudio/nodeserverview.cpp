/*! @file
<PRE>
********************************************************************************
ģ����       :  �ڵ�ģ�� �������ڵ�����view
�ļ���       :  nodeserverview.cpp
�ļ�ʵ�ֹ��� :  ǰ��ģ��
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
#include "nodeserverview.h"
#include "nodeservertable.h"

CNodeServerView::CNodeServerView(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeServer *pServerData
	, Config::CNodeConfigData *pNodeConfig)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CNodeServerTable(this, pCore, pServerData, pNodeConfig);

	setCentralWidget(m_pTable);
}

CNodeServerView::~CNodeServerView()
{

}

/*! \fn void CNodeServerView::Delete()
********************************************************************************************************* 
** \brief CNodeServerView::Delete 
** \details Ϊ���½����̡��򿪹��� �ر�tabwidgets deatch view��model
** \return void 
** \author gw
** \date 2016��9��27�� 
** \note 
********************************************************************************************************/
void CNodeServerView::Delete()
{
	m_pTable->setModel(nullptr);
}
