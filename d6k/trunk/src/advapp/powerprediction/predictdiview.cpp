#include "predictdiview.h"

CPredictDiView::CPredictDiView(QWidget* parent, CPredictDevice* pDeviceData, IMainModuleInterface* pCore)
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


	m_pTable = new CPredictDiTable(this, pDeviceData->m_vecDis, pCore);

	setCentralWidget(m_pTable);

}

CPredictDiView::~CPredictDiView()
{

}

void CPredictDiView::Delete()
{
	m_pTable->setModel(nullptr);
	deleteLater();
}