#include "predictparamview.h"

CPredictParamView::CPredictParamView(QWidget* parent, CPredictDevice* pDeviceData, IMainModuleInterface* pCore)
	:QMainWindow(parent)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	Q_ASSERT(pDeviceData);
	if (!pDeviceData)
	{
		return;
	}


	m_pTable = new CPredictParamTable(this, pDeviceData->m_vecParams, pCore);

	setCentralWidget(m_pTable);


}

CPredictParamView::~CPredictParamView()
{

}

void CPredictParamView::Delete()
{
	m_pTable->setModel(nullptr);
	deleteLater();
}