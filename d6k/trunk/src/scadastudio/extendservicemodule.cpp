#include "extendservicemodule.h"
#include "extenservicemodulemgr.h"
#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "extendserviceconfigdialog.h"

#include <QMenu>

CExtendServiceModule::CExtendServiceModule()
	: m_pCore(nullptr)
{

}

CExtendServiceModule::~CExtendServiceModule()
{

}

void CExtendServiceModule::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	connect(m_pCore->GetUIMgr()->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
}

bool CExtendServiceModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);

	return true;
}

/*! \fn bool CExtendServiceModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CExtendServiceModule::LoadProject 
** \details 加载扩展服务
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2016年12月3日 
** \note 
********************************************************************************************************/
bool CExtendServiceModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);

	return true;
}

// 关闭工程文件
void CExtendServiceModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);	
	Q_UNUSED(bSaveFlag);
}

// 新建一个空的工程文件
/*! \fn void CExtendServiceModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
********************************************************************************************************* 
** \brief CExtendServiceModule::CreateNewProject 
** \details 创建扩展服务
** \param pXml 
** \param pRoot 
** \param szRoot 
** \param pTopItem 
** \return void 
** \author gw
** \date 2016年12月3日 
** \note 
********************************************************************************************************/
void CExtendServiceModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(pTopItem);
	
	//Q_ASSERT(pXml);
	//if (!pXml)
	//{
	//	return;
	//}

	//Q_ASSERT(pRoot);
	//if (!pRoot)
	//{
	//	return;
	//}
	//
	//Q_ASSERT(pTopItem);
	//if (!pTopItem)
	//{
	//	return;
	//}

	//m_pMgr->CreateNewProj(pXml, pRoot, szRoot, pTopItem);
}

bool CExtendServiceModule::GetModifyFlag() const
{
	return true;
}

/*! \fn void CExtendServiceModule::ShowMouseRightButton(const QPoint &point)
********************************************************************************************************* 
** \brief CExtendServiceModule::ShowMouseRightButton 
** \details 右键槽
** \param point 
** \return void 
** \author gw
** \date 2016年12月3日 
** \note 
********************************************************************************************************/
void CExtendServiceModule::ShowMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pCore->GetUIMgr()->GetLeftTree()->indexAt(point);
	int nType = indexSelect.data(Qt::UserRole).toInt();
	if (nType == EXTEND_SERVICE_ITEM)
	{
		QMenu *pMenu = new QMenu;
		
		QAction *pAddPoint = new QAction(tr("add service"), pMenu);
		pAddPoint->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pAddPoint);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pAddPoint)
		{
			CExtendServiceConfigDialog dialog;
			dialog.Init(m_pCore->GetExtendServiceModuleMgr());
			dialog.exec();
		}

		pMenu->deleteLater();
	}
}