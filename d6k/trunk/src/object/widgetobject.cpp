
#include "widgetobject.h"

#include <QPushButton>

#include "objectdefine.h"
#include "targetname_delegate.h"


CObjectView::CObjectView(CObjectItem* pObjItem) : m_pObjItem(pObjItem)
{
    
}

CObjectView::~CObjectView()
{
}

void CObjectView::Save()
{

}

void CObjectView::Refresh()
{

}

void CObjectView::SetModifyFlag(bool bFlag)
{

}

bool CObjectView::GetModifyFlag()
{
    return true;
}

CObjectItem* CObjectView::GetObjItem() const
{
    Q_ASSERT (m_pObjItem);
    if (m_pObjItem)
    {
        return m_pObjItem;
    }
    else
    {
        return Q_NULLPTR;
    }
}


CObjectWidget::CObjectWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore)
{
    Q_ASSERT(pTableWidget && pCore);

    m_pTableWidget = pTableWidget;

    m_pCore = pCore;

    m_pContentModel = nullptr;
}

CObjectWidget::~CObjectWidget()
{
    if (m_pContentModel != nullptr)
    {
        delete m_pContentModel;
        m_pContentModel = nullptr;
    }
}

void CObjectWidget::AddToTableCtrl(CObjectItem* pObjItem)
{
    if (pObjItem == Q_NULLPTR)
    {
        return;
    }

    if (pObjItem->m_vecTableInfo.size() <= 0)
    {
        return;
    }

    int nCount = m_pTableWidget->count();

    for (int i = 0; i < nCount; ++i)
    {
        CObjectView* pObjView = dynamic_cast<CObjectView*>(m_pTableWidget->widget(i));
        if (pObjView)
        {
            CObjectItem* pItem = pObjView->GetObjItem();
            if (pItem && (pItem == pObjItem))
            {
                // 设置当前存在索引
                m_pTableWidget->setCurrentIndex(i);
                return;
            }
        }
    }

    //ReleaseData();

    CObjectView* pObjView = new CObjectView(pObjItem);

    CContentModel* pContentModel = new CContentModel(*pObjItem);

    CTargetNameButtonDelegate* pBtnDelegate = new CTargetNameButtonDelegate();
    pObjView->setItemDelegateForColumn(5, pBtnDelegate);
    
    // 设置头
    //SetTabViewHead();
    QStringList headerList;

    headerList << HEAD_NAME_ID
        << HEAD_NAME_NAME
        << HEAD_NAME_DESCIBE
        << HEAD_NAME_TYPE
        << HEAD_NAME_RW_ATTR
        << HEAD_NAME_RELATION_POINT;

    pContentModel->SetHeadTitles(headerList);
    // 设置model
    pObjView->setModel(pContentModel);

    m_pTableWidget->AddTab(pObjView, pObjItem->m_strObjectName, 1, "");

}

void CObjectWidget::SetTabViewHead()
{
    QStringList headerList;

    headerList << HEAD_NAME_ID
        << HEAD_NAME_NAME
        << HEAD_NAME_DESCIBE
        << HEAD_NAME_TYPE
        << HEAD_NAME_RW_ATTR
        << HEAD_NAME_RELATION_POINT;

    m_pContentModel->SetHeadTitles(headerList);
}

void CObjectWidget::ReleaseData()
{
    if (m_pContentModel != nullptr)
    {
        delete m_pContentModel;
        m_pContentModel = nullptr;
    }
}
