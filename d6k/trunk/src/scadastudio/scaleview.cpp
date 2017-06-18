#include "scaleview.h"
#include "scaletable.h"
#include "scalemodel.h"

CScaleView::CScaleView(QWidget *parent, Config::CTransform *pTransform, CQuoteItem *pItem, CScale *pScaleModel
	, Config::CFesData *pFesConfig)
	: QMainWindow(parent)
{
	CScaleTable *pTable = new CScaleTable(parent, pTransform, pItem, pScaleModel, pFesConfig);

	m_pTable = pTable;

	setCentralWidget(pTable);
}

CScaleView::~CScaleView()
{

}

void CScaleView::Save()
{

}

void CScaleView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CScaleView::Delete()
{
	m_pTable->setModel(nullptr);
}