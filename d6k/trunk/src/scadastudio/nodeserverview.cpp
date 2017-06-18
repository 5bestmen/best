/*! @file
<PRE>
********************************************************************************
模块名       :  节点模块 服务器节点配置view
文件名       :  nodeserverview.cpp
文件实现功能 :  前置模块
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
** \details 为了新建工程、打开工程 关闭tabwidgets deatch view和model
** \return void 
** \author gw
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
void CNodeServerView::Delete()
{
	m_pTable->setModel(nullptr);
}
