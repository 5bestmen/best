/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ����
�ļ���       :  sysmodel.cpp
�ļ�ʵ�ֹ��� :  ϵͳ��̬ģ��
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
** \date 2015��11��26�� 
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
** \date 2015��11��26�� 
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
** \date 2016��8��31�� 
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
	////ϵͳ��̬����
	//auto strTmp = tr("system config");
	//CQuoteItem *pSystemItem = new CQuoteItem(QIcon(SYSTEM_PNG), strTmp);

	//pSystemItem->setData(SYSTEM_CONFIG_ITEM, Qt::UserRole);

	//pSystemItem->setEditable(false);

	//pTop->appendRow(pSystemItem);
}

/*! \fn void CSysConfigurationModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
********************************************************************************************************* 
** \brief CSysConfigurationModule::CreateNewProject 
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
** \details ���湤��
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016��8��31�� 
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
** \details ���ع���
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016��8��31�� 
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
void CSysConfigurationModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(bSaveFlag);
}

// ����ģ��ʵ�֣�����ģ�����
// �������жϣ�ֻҪ���κ�һ����ģ��ġ��ࡱ��־���ڣ����Ƴ��Ի����ǡ���ȡ����������ǣ�����ģ�����������ģ��ı���ӿ�
/*! \fn bool CSysConfigurationModule::GetModifyFlag() const
********************************************************************************************************* 
** \brief CSysConfigurationModule::GetModifyFlag 
** \details 
** \return bool 
** \author gw
** \date 2016��8��31�� 
** \note 
********************************************************************************************************/
bool CSysConfigurationModule::GetModifyFlag() const
{
	return false;
}