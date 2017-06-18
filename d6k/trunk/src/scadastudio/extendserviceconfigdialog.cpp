#include "extendserviceconfigdialog.h"
#include "extenservicemodulemgr.h"
#include "scadastudio/icore.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "scadastudio/imodule.h"

#include <QDomDocument>

extern IMainModuleInterface *s_pGlobleCore;

CExtendServiceConfigDialog::CExtendServiceConfigDialog(QWidget *parent)
	: QDialog(parent), m_pMgr(nullptr)
{
	ui.setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);

	ui.comboBox_ServiceDesc->setEditable(false);

	connect(ui.comboBox_ServiceType, SIGNAL(currentIndexChanged(int)), this, SLOT(CurrentIndexChanged(int)));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CExtendServiceConfigDialog::~CExtendServiceConfigDialog()
{

}

/*! \fn void CExtendServiceConfigDialog::Init(CExtendServiceModuleMgr *pMgr)
********************************************************************************************************* 
** \brief CExtendServiceConfigDialog::Init 
** \details 初始化
** \param pMgr 
** \return void 
** \author gw
** \date 2016年12月3日 
** \note 
********************************************************************************************************/
void CExtendServiceConfigDialog::Init(CExtendServiceModuleMgr *pMgr)
{
	Q_ASSERT(pMgr);
	if (!pMgr)
	{
		return;
	}
	m_pMgr = pMgr;

	auto arr = pMgr->GetModuleArr();

	for each (auto var in arr)
	{
		ui.comboBox_ServiceType->addItem(var->m_szName);
	}
}

/*! \fn void CExtendServiceConfigDialog::CurrentIndexChanged(int index)
********************************************************************************************************* 
** \brief CExtendServiceConfigDialog::CurrentIndexChanged 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月3日 
** \note 
********************************************************************************************************/
void CExtendServiceConfigDialog::CurrentIndexChanged(int index)
{
	Q_UNUSED(index);
	
	auto strName = ui.comboBox_ServiceType->currentText();
	
	auto bFlag = false;
	for each (auto var in m_pMgr->GetModuleArr())
	{
		if (strName == var->m_szName)
		{
			ui.comboBox_ServiceDesc->clear();
			ui.comboBox_ServiceDesc->addItem(var->m_szDescript);
			ui.comboBox_ServiceDesc->setCurrentText(var->m_szDescript);

			bFlag = true;

			break;
		}
	}
}

void CExtendServiceConfigDialog::OnOk()
{
	auto strName = ui.comboBox_ServiceType->currentText();
	for each (auto var in m_pMgr->GetModuleArr())
	{
		if (strName == var->m_szName)
		{
			auto pDocument = m_pMgr->GetDocument();
			Q_ASSERT(pDocument);
			if (!pDocument)
			{
				return;
			}

			

			auto pElement = m_pMgr->GetElement();
			Q_ASSERT(pElement);
			if (!pElement)
			{
				return;
			}

			auto strPath = m_pMgr->GetPath();
			Q_ASSERT(!strPath.isEmpty());
			if (strPath.isEmpty())
			{
				return;
			}

			auto pTopItem = m_pMgr->GetExtendServiceItem();
			Q_ASSERT(pTopItem);
			if (!pTopItem)
			{
				return;
			}

			Q_ASSERT(var->m_pModuleInterface);
			if (!var->m_pModuleInterface)
			{
				return;
			}

			var->m_pModuleInterface->CreateNewProject(pDocument, pElement, strPath, pTopItem);
		}
	}

	accept();
}

/*! \fn void CExtendServiceConfigDialog::OnCancel()
********************************************************************************************************* 
** \brief CExtendServiceConfigDialog::OnCancel 
** \details close window
** \return void 
** \author gw
** \date 2017年3月22日 
** \note 
********************************************************************************************************/
void CExtendServiceConfigDialog::OnCancel()
{
	reject();

	close();
}
