/*! @file
<PRE>
********************************************************************************
模块名       :  系统参数
文件名       :  sysmodel.cpp
文件实现功能 :  系统组态模块
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 通道 装置
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.26
*/

#include <QDir>
#include <QDomDocument>

#include "sysmodule.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "scadastudio/icore.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/iprojmgr.h"

/*! \fn CSysModel::CSysModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CSysModel::CSysModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CSysConfigurationModule::CSysConfigurationModule()
	:m_pCore(nullptr), m_pUi(nullptr)
{

}

/*! \fn CSysModel::~CSysModel()
********************************************************************************************************* 
** \brief CSysModel::~CSysModel 
** \details 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CSysConfigurationModule::~CSysConfigurationModule()
{

}

/*! \fn void CSysConfigurationModule::Init(IMainModuleInterface *pCore)
********************************************************************************************************* 
** \brief CSysConfigurationModule::Init 
** \details 
** \param pCore 
** \return void 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
void CSysConfigurationModule::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;
	m_pUi = m_pCore->GetUIMgr();

	//CQuoteItem *pTop = m_pUi->GetTopItem();
	//
	////系统组态配置
	//auto strTmp = tr("system config");
	//CQuoteItem *pSystemItem = new CQuoteItem(QIcon(SYSTEM_PNG), strTmp);

	//pSystemItem->setData(SYSTEM_CONFIG_ITEM, Qt::UserRole);

	//pSystemItem->setEditable(false);

	//pTop->appendRow(pSystemItem);
}

/*! \fn void CSysConfigurationModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
********************************************************************************************************* 
** \brief CSysConfigurationModule::CreateNewProject 
** \details 新建工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param pTopItem 
** \return void 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
void CSysConfigurationModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	QDir dir;
	bool ok = dir.mkdir(szRoot + "/" + "system");
	if (!ok)
	{
		auto strError = QString(tr("%1/system folder mkdir failed!!!")).arg(szRoot);

		m_pCore->LogMsg(NODE_CONFIG_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
	}

	auto strTmp = tr("system");
	QDomElement root = pXml->createElement(strTmp);
	pRoot->appendChild(root);

	CQuoteItem *pItem = new CQuoteItem(strTmp);
	pItem->setData(SYSTEM_CONFIG_ITEM, Qt::UserRole);
	pItem->setIcon(QIcon(SYSTEM_PNG));
	pItem->setEditable(false);

	pTopItem->appendRow(pItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(pTopItem));
}

/*! \fn bool CSysConfigurationModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CSysConfigurationModule::SaveProject 
** \details 保存工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CSysConfigurationModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CSysConfigurationModule::LoadProject 
** \details 加载工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CSysConfigurationModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);

	return true;
}

/*! \fn void CSysConfigurationModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
********************************************************************************************************* 
** \brief CSysConfigurationModule::CloseProject 
** \details 关闭工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param bSaveFlag 
** \return void 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
void CSysConfigurationModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(bSaveFlag);
}

// 由子模块实现，由主模块调用
// 主程序判断，只要有任何一个子模块的“脏”标志存在，就推出对话框《是、否、取消》，如果是，则主模块调用所有子模块的保存接口
/*! \fn bool CSysConfigurationModule::GetModifyFlag() const
********************************************************************************************************* 
** \brief CSysConfigurationModule::GetModifyFlag 
** \details 
** \return bool 
** \author gw
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CSysConfigurationModule::GetModifyFlag() const
{
	return false;
}