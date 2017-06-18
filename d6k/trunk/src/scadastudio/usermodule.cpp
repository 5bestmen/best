/*! @file
<PRE>
********************************************************************************
ģ����       :  �û�ģ��
�ļ���       :  usermodel.cpp
�ļ�ʵ�ֹ��� :  �û�ģ��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> ͨ�� װ��
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.26
*/

#include <QDir>
#include <QDomDocument>

#include "usermodule.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
//#include "user.h"
#include "scadastudio/imainwindow.h"
#include "user.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "scadastudio/icore.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/iprojmgr.h"

/*! \fn CUserModel::CUserModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CUserModel::CUserModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CUserManageModule::CUserManageModule()
	:m_pUi(nullptr), m_pCore(nullptr)
{

}

/*! \fn CUserModel::~CUserModel()
********************************************************************************************************* 
** \brief CUserModel::~CUserModel 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CUserManageModule::~CUserManageModule()
{

}

/*! \fn void CUserManageModule::Init(IMainModuleInterface *pCore)
********************************************************************************************************* 
** \brief CUserModel::Init 
** \details 
** \param pUi 
** \return void 
** \author gw
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
void CUserManageModule::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	
	m_pCore = pCore;
	m_pUi = m_pCore->GetUIMgr();

	//CQuoteItem *pTop = pUi->GetTopItem();
	//
	////����ڵ�
	//CQuoteItem *pItem = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�û�"));

	//pItem->setData(USER_ITEM, Qt::UserRole);

	//pItem->setEditable(false);

	//pTop->appendRow(pItem);
}

/*! \fn void CUserManageModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
********************************************************************************************************* 
** \brief CUserManageModule::CreateNewProject 
** \details �½�����
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param pTopItem 
** \return void 
** \author gw
** \date 2016��8��31�� 
** \note 
********************************************************************************************************/
void CUserManageModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	Q_ASSERT(pXml);
	if (!pXml)
	{
		auto strError = QString(tr("dom document is invalid!!!"));
		m_pCore->LogMsg(USER_MANAGE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	Q_ASSERT(pRoot);
	if (!pRoot)
	{
		auto strError = QString(tr("the root of dom document is invalid!!!"));
		m_pCore->LogMsg(USER_MANAGE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	QDir dir;
	bool ok = dir.mkdir(szRoot + "/" + "user");
	if (!ok)
	{
		auto strError = QString(tr("%1/user folder mkdir failed!!!")).arg(szRoot);

		m_pCore->LogMsg(USER_MANAGE_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
	}

	auto strTmp = tr("user");
	QDomElement root = pXml->createElement(strTmp);
	pRoot->appendChild(root);

	CQuoteItem *pItem = new CQuoteItem(strTmp);
	pItem->setData(SYSTEM_CONFIG_ITEM, Qt::UserRole);
	pItem->setIcon(QIcon(USER_MANAGER_PNG));
	pItem->setEditable(false);

	pTopItem->appendRow(pItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(pTopItem));
}

/*! \fn bool CUserManageModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CUserManageModule::SaveProject 
** \details ���湤��
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016��8��31�� 
** \note 
********************************************************************************************************/
bool CUserManageModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	
	return true;
}

/*! \fn bool CUserManageModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CSysConfigurationModule::LoadProject 
** \details bool CUserManageModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016��8��31�� 
** \note 
********************************************************************************************************/
bool CUserManageModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	
	return true;
}

/*! \fn void CUserManageModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
********************************************************************************************************* 
** \brief CUserManageModule::CloseProject 
** \details �رչ���
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param bSaveFlag 
** \return void 
** \author gw
** \date 2016��8��31�� 
** \note 
********************************************************************************************************/
void CUserManageModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(bSaveFlag);
}

/*! \fn bool CUserManageModule::GetModifyFlag() const
********************************************************************************************************* 
** \brief CUserManageModule::GetModifyFlag 
** \details 
** \return bool 
** \author gw
** \date 2016��8��31�� 
** \note 
********************************************************************************************************/
bool CUserManageModule::GetModifyFlag() const
{
	return false;
}