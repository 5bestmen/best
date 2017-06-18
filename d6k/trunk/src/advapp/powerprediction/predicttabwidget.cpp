#include "predicttabwidget.h"
#include "powerpredictdefine.h"


CPredictView::CPredictView(CPredictItem* pItem) : m_pItem(pItem)
{

}

CPredictView::~CPredictView()
{

}

void CPredictView::Save()
{

}

void CPredictView::Refresh()
{

}

void CPredictView::SetModifyFlag(bool bFlag)
{

}

bool CPredictView::GetModifyFlag()
{
	return true;
}

CPredictItem* CPredictView::GetPdtItem() const
{
	Q_ASSERT(m_pItem);
	if (m_pItem)
	{
		return m_pItem;
	} 
	else
	{
		return Q_NULLPTR;
	}
}

CPredictTabWidget::CPredictTabWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
	Q_ASSERT(pTableWidget && pCore);

	m_pTabWidget = pTableWidget;

	m_pCore = pCore;

	m_pPredictModel = nullptr;
}

CPredictTabWidget::~CPredictTabWidget()
{
	if (m_pPredictModel != nullptr)
	{
		delete m_pPredictModel;
		m_pPredictModel = nullptr;
	}
}

void CPredictTabWidget::AddToTable(CPredictItem* pItem)
{
	if (pItem == Q_NULLPTR)
	{
		return;
	}

	if (pItem->m_vecTableInfo.size() <= 0)
	{
		return;
	}

	int nCount = m_pTabWidget->count();

	for (int i = 0; i < nCount; ++i)
	{
		CPredictView* pView = dynamic_cast<CPredictView*>(m_pTabWidget->widget(i));
		if (pView)
		{
			CPredictItem* pPItem = pView->GetPdtItem();
			if (pItem && (pPItem == pItem))
			{
				m_pTabWidget->setCurrentIndex(i);
				return;
			}
		}
	}

	CPredictView* pView = new CPredictView(pItem);

	CPredictModel* pPredictModel = new CPredictModel(*pItem);
	//CTargetNameButtonDelegate* pBtnDelegate = new CTargetNameButtonDelegate();
	//pView->setItemDelegateForColumn(5, pBtnDelegate);

	QStringList headerList;

	headerList << HEAD_NAME_ID
			   << HEAD_NAME_TAGNAME
			   << HEAD_NAME_DESCIBE
			   << HEAD_NAME_OCCNO;

	pPredictModel->SetHeadTitles(headerList);

	pView->setModel(pPredictModel);

	m_pTabWidget->AddTab(pView, pItem->m_strItemName, 1, "");
}

void CPredictTabWidget::SetTabViewHead()
{
	QStringList headerList;

	headerList << HEAD_NAME_ID
		   	   << HEAD_NAME_TAGNAME
			   << HEAD_NAME_DESCIBE
			   << HEAD_NAME_OCCNO;

	m_pPredictModel->SetHeadTitles(headerList);
}

void CPredictTabWidget::ReleaseData()
{
	if (m_pPredictModel != nullptr)
	{
		delete m_pPredictModel;
		m_pPredictModel = nullptr;
	}
}