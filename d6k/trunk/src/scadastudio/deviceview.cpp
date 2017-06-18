#include "deviceview.h"
#include "fesmodule.h"
#include "aitable.h"
#include "ditable.h"
#include "aotable.h"
#include "dotable.h"
#include "scadastudiodefine.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "devicetable.h"
#include "devicemodel.h"

CDeviceView::CDeviceView(IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
	CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFesData)
	: QMainWindow(nullptr)/*, m_pTabDevice(nullptr)*/
{
	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		QString strError = QString(tr("device tagname %1 can not find!!!")).arg(strTag);
		pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	CDeviceTable *pTable = new CDeviceTable(this, pCore, pDeviceData, pItem, (CChannel *)pModule, pFesData);
	m_pTable = pTable;

	setCentralWidget(pTable);

	//m_pTabDevice = new QTabWidget(this);
	//m_pTabDevice->setTabPosition(QTabWidget::South);

	//setCentralWidget(m_pTabDevice);

	//auto *pTmp = CFesModel::GetDevice(pFes, strTag);

	////auto *pFes = CFesModel::GetFesData(arrFes, strTag, FES__CHANNEL_CHILD_DEVICE_ITEM);

	//auto &arrScale = pFes->m_arrTransforms;

	//pTmp->m_arrAIs;



	//CAITable *pAI = new CAITable(this, pTmp->m_arrAIs, pCore, pModule, pFes);
	//CDITable *pDI = new CDITable(this, pTmp->m_arrDIs, pCore, pModule);
	//CAOTable *pAO = new CAOTable(this, pTmp->m_arrAOs, pCore, pModule);
	//CDOTable *pDO = new CDOTable(this, pTmp->m_arrDOs, pCore, pModule);


	//m_pTabDevice->addTab(pAI, tr("ai"));
	//m_pTabDevice->addTab(pDI, tr("di"));
	//m_pTabDevice->addTab(pAO, tr("ao"));
	//m_pTabDevice->addTab(pDO, tr("do"));
}

CDeviceView::~CDeviceView()
{

}

void CDeviceView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CDeviceView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}