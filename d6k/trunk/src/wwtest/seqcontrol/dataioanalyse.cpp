#include <QDebug>
#include "dataioanalyse.h"
#include "logicxmlreader.h"
#include "fdborderscene.h"
#include "funblockdiagramitem.h"
#include "fdborderview.h"
#include "connectionrouter.h"
#include "commentitem.h"
#include "seqdefine.h"
#include "moveorderitem.h"
#include "mathorderitem.h"
#include "compareorderitem.h"
#include "callsritem.h"
#include "timersorderitem.h"

CDataIOAnalyse::CDataIOAnalyse(CFdbOrderScene *pScene,QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(pScene);
    if (pScene == nullptr)
    {
        return;
    }
    m_pScene = pScene;
}

CDataIOAnalyse::~CDataIOAnalyse()
{

}

void CDataIOAnalyse::MakeLogicItem(const QList<ITEMINFO> &lstItems)
{
    m_mapCreateNumItem.clear();
    //
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    pView->ClearCreateNum();

    for each (ITEMINFO itemInfo in lstItems)
    {
        //����item����  �߼�����
        CFdbOrderBaseItem *pLaderrItem = nullptr;

        if (itemInfo.nItemType >= BITAND && itemInfo.nItemType <= BITXOR)
        {
            //item ���ͺʹ������
            pLaderrItem = new CFunBlockDiagramItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= BITSHL && itemInfo.nItemType <= BITCLR)
        {
            pLaderrItem = new CMoveOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= MATHMOVE && itemInfo.nItemType <= MATHMIN)
        {
            pLaderrItem = new CMathOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= COMPAREEQ && itemInfo.nItemType <= COMPARENE)
        {
            pLaderrItem = new CCompareOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType == CALLSR)
        {
            pLaderrItem = new CCallSRItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= TIMERCTD && itemInfo.nItemType <= TIMERTP)
        {
            pLaderrItem = new CTimersOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }

        if (pLaderrItem == nullptr)
        {
            continue;
        }

        //itemִ�б��
        pLaderrItem->SetIndexNumber(itemInfo.nExecNum);
        //item��С
        pLaderrItem->ResetRect(QRectF(0, 0, itemInfo.itemRect.width(), itemInfo.itemRect.height()));
        //itemע��
        pLaderrItem->Slot_UpdateComment(itemInfo.strItemComment);
        //en
        if (itemInfo.bEx)
        {
            pLaderrItem->ShowEnoItems();
        }
        else
        {
            pLaderrItem->CloseEnoItems();
        }
        //TODO   �ܵ���������Ժ����ʵ�������

        pView->AddCreateNum();
        pView->InsertOneItem(pLaderrItem);
        //item����
        pLaderrItem->setPos(itemInfo.itemRect.x(), itemInfo.itemRect.y());
        m_pScene->addItem(pLaderrItem);
        m_mapCreateNumItem.insert(itemInfo.nCreateNum, pLaderrItem);
    }
}

void CDataIOAnalyse::MaieLineItem(const QList<LINEROUNTER> &lstLines)
{
    //����
    for each (LINEROUNTER lineItem in lstLines)
    {
        if (m_mapCreateNumItem.contains(lineItem.strBgeItem.toInt()) && m_mapCreateNumItem.contains(lineItem.strEndItem.toInt()))
        {
            CFdbOrderBaseItem *startItem = m_mapCreateNumItem[lineItem.strBgeItem.toInt()];
            CFdbOrderBaseItem *endItem = m_mapCreateNumItem[lineItem.strEndItem.toInt()];

            startItem->SetItemStatus(IOOUTPUT);
            startItem->SetOutPutIndex(lineItem.strBeginIndex.toInt());

            endItem->SetItemStatus(IOINPUT);
            endItem->SetInPutIndex(lineItem.strEndIndex.toInt());

            CConnectionRouter *plyLine = new CConnectionRouter(startItem, endItem, m_pScene);

            plyLine->SetEndInput(endItem->GetInputIndex());
            plyLine->SetBegiOutput(startItem->GetOutputIndex());
            //���ӹ���
            startItem->InsertOuputItem(startItem->GetOutputIndex(),endItem);
            endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
            //����ֱ����
            plyLine->SetAllPoints(lineItem.lstAllPoints);
            plyLine->DrawLineFromPoints();

            endItem->InsertPins(endItem->GetInputIndex(), startItem->GetOutputIndex());

            startItem->AddPloyLine(plyLine);
            endItem->AddPloyLine(plyLine);

        }
    }
}

void CDataIOAnalyse::MakeCommentItem(const QList<COMMENTDATA> &lstLines)
{
    for each (COMMENTDATA commentItem in lstLines)
    {
        CCommentItem *pComment = new CCommentItem;
        pComment->setPos(commentItem.pfPos);
        pComment->setPlainText(commentItem.strComment);
        pComment->ResetRect(commentItem.rfRect);

        m_pScene->addItem(pComment);
        //m_pScene->AddOneComment(pComment);
    }
}

void CDataIOAnalyse::CopyLogicItem(const QList<ITEMINFO> &lstItems)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }
    
    //QPointF pfMouse = pView->mapToScene(pView->mapFromGlobal(QCursor::pos()));
    m_mapCopyNumItem.clear();

    for each (ITEMINFO itemInfo in lstItems)
    {
        //����item����  �߼�����
        CFdbOrderBaseItem *pLaderrItem = nullptr;

        if (itemInfo.nItemType >= BITAND && itemInfo.nItemType <= BITXOR)
        {
            //item ���ͺʹ������
            pLaderrItem = new CFunBlockDiagramItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= BITSHL && itemInfo.nItemType <= BITCLR)
        {
            pLaderrItem = new CMoveOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= MATHMOVE && itemInfo.nItemType <= MATHMIN)
        {
            pLaderrItem = new CMathOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= COMPAREEQ && itemInfo.nItemType <= COMPARENE)
        {
            pLaderrItem = new CCompareOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType == CALLSR)
        {
            pLaderrItem = new CCallSRItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= TIMERCTD && itemInfo.nItemType <= TIMERTP)
        {
            pLaderrItem = new CTimersOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }

        if (pLaderrItem == nullptr)
        {
            continue;
        }
        //itemִ�б��
        pLaderrItem->SetIndexNumber(pView->GetNextCreateNum());
        //item��С
        pLaderrItem->ResetRect(QRectF(0, 0, itemInfo.itemRect.width(), itemInfo.itemRect.height()));
        //itemע��
        pLaderrItem->Slot_UpdateComment(itemInfo.strItemComment);
        //en
        if (itemInfo.bEx)
        {
            pLaderrItem->ShowEnoItems();
        }
        else
        {
            pLaderrItem->CloseEnoItems();
        }
        //TODO   �ܵ���������Ժ����ʵ�������

        pView->AddCreateNum();
        pView->InsertOneItem(pLaderrItem);
        //item����
        pLaderrItem->setPos(m_pfEndPoint.x() + itemInfo.itemRect.x() - m_pfStartPoint.x(), m_pfEndPoint.y() + itemInfo.itemRect.y() - m_pfStartPoint.y());
        m_pScene->addItem(pLaderrItem);
        m_mapCopyNumItem.insert(itemInfo.nCreateNum, pLaderrItem);
        
    }
}

void CDataIOAnalyse::CopyLineItem(const QList<LINEROUNTER> &lstLines)
{
    for each (LINEROUNTER lineItem in lstLines)
    {
        if (m_mapCopyNumItem.contains(lineItem.strBgeItem.toInt()) && m_mapCopyNumItem.contains(lineItem.strEndItem.toInt()))
        {
            CFdbOrderBaseItem *startItem = m_mapCopyNumItem[lineItem.strBgeItem.toInt()];
            CFdbOrderBaseItem *endItem = m_mapCopyNumItem[lineItem.strEndItem.toInt()];

            startItem->SetItemStatus(IOOUTPUT);
            startItem->SetOutPutIndex(lineItem.strBeginIndex.toInt());

            endItem->SetItemStatus(IOINPUT);
            endItem->SetInPutIndex(lineItem.strEndIndex.toInt());

            CConnectionRouter *plyLine = new CConnectionRouter(startItem, endItem, m_pScene);

            plyLine->SetEndInput(endItem->GetInputIndex());
            plyLine->SetBegiOutput(startItem->GetOutputIndex());
            //���ӹ���
            startItem->InsertOuputItem(startItem->GetOutputIndex(),endItem);
            endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
            //����ֱ����
            plyLine->SetAllPoints(lineItem.lstAllPoints);
            plyLine->DrawLineFromPoints();

            endItem->InsertPins(endItem->GetInputIndex(), startItem->GetOutputIndex());

            startItem->AddPloyLine(plyLine);
            endItem->AddPloyLine(plyLine);
            plyLine->UpdatePosition();

        }
    }

    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }
    //����ִ��˳��
    pView->UpdateItemsExecNum();

}

void CDataIOAnalyse::CopyCommentItem(const QList<COMMENTDATA> &lstLines)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    for each (COMMENTDATA commentItem in lstLines)
    {
        CCommentItem *pComment = new CCommentItem;

        pComment->setPos(m_pfEndPoint.x() + commentItem.pfPos.x() - m_pfStartPoint.x(), m_pfEndPoint.y() + commentItem.pfPos.y() - m_pfStartPoint.y());
        pComment->setPlainText(commentItem.strComment);
        pComment->ResetRect(commentItem.rfRect);

        m_pScene->addItem(pComment);
        //m_pScene->AddOneComment(pComment);
    }
}

void CDataIOAnalyse::SetStartPoint(QPointF pfStart)
{
    m_pfStartPoint = pfStart;
}

void CDataIOAnalyse::SetEndPoint(QPointF pfEnd)
{
    m_pfEndPoint = pfEnd;
}

void CDataIOAnalyse::CutLoginItem(const QList<ITEMINFO> &lstItems)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    //QPointF pfMouse = pView->mapToScene(pView->mapFromGlobal(QCursor::pos()));

    m_mapCopyNumItem.clear();

    for each (ITEMINFO itemInfo in lstItems)
    {
        //����item����  �߼�����
        CFdbOrderBaseItem *pLaderrItem = nullptr;

        if (itemInfo.nItemType >= BITAND && itemInfo.nItemType <= BITXOR)
        {
            //item ���ͺʹ������
            pLaderrItem = new CFunBlockDiagramItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= BITSHL && itemInfo.nItemType <= BITCLR)
        {
            pLaderrItem = new CMoveOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= MATHMOVE && itemInfo.nItemType <= MATHMIN)
        {
            pLaderrItem = new CMathOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= COMPAREEQ && itemInfo.nItemType <= COMPARENE)
        {
            pLaderrItem = new CCompareOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType == CALLSR)
        {
            pLaderrItem = new CCallSRItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }
        else if (itemInfo.nItemType >= TIMERCTD && itemInfo.nItemType <= TIMERTP)
        {
            pLaderrItem = new CTimersOrderItem(itemInfo.nItemType, pView->GetNextCreateNum());
        }

        if (pLaderrItem == nullptr)
        {
            continue;
        }

        //���ô������
        pLaderrItem->SetCreateNum(itemInfo.nCreateNum);
        //itemִ�б��
        pLaderrItem->SetIndexNumber(itemInfo.nExecNum);
        //item��С
        pLaderrItem->ResetRect(QRectF(0, 0, itemInfo.itemRect.width(), itemInfo.itemRect.height()));
        //itemע��
        pLaderrItem->Slot_UpdateComment(itemInfo.strItemComment);
        //en
        if (itemInfo.bEx)
        {
            pLaderrItem->ShowEnoItems();
        }
        else
        {
            pLaderrItem->CloseEnoItems();
        }
        //TODO   �ܵ���������Ժ����ʵ�������

        pView->AddCreateNum();
        pView->InsertOneItem(pLaderrItem);
        //item����
        pLaderrItem->setPos(m_pfEndPoint.x() + itemInfo.itemRect.x() - m_pfStartPoint.x(), m_pfEndPoint.y() + itemInfo.itemRect.y() - m_pfStartPoint.y());
        m_pScene->addItem(pLaderrItem);
        m_mapCopyNumItem.insert(itemInfo.nCreateNum, pLaderrItem);
        
    }
}

void CDataIOAnalyse::CutLineItem(const QList<LINEROUNTER> &lstLines)
{
    for each (LINEROUNTER lineItem in lstLines)
    {
        if (m_mapCopyNumItem.contains(lineItem.strBgeItem.toInt()) && m_mapCopyNumItem.contains(lineItem.strEndItem.toInt()))
        {
            CFdbOrderBaseItem *startItem = m_mapCopyNumItem[lineItem.strBgeItem.toInt()];
            CFdbOrderBaseItem *endItem = m_mapCopyNumItem[lineItem.strEndItem.toInt()];

            startItem->SetItemStatus(IOOUTPUT);
            startItem->SetOutPutIndex(lineItem.strBeginIndex.toInt());

            endItem->SetItemStatus(IOINPUT);
            endItem->SetInPutIndex(lineItem.strEndIndex.toInt());

            CConnectionRouter *plyLine = new CConnectionRouter(startItem, endItem, m_pScene);

            plyLine->SetEndInput(endItem->GetInputIndex());
            plyLine->SetBegiOutput(startItem->GetOutputIndex());
            //���ӹ���
            startItem->InsertOuputItem(startItem->GetOutputIndex(), endItem);
            endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
            //����ֱ����
            plyLine->SetAllPoints(lineItem.lstAllPoints);
            plyLine->DrawLineFromPoints();

            endItem->InsertPins(endItem->GetInputIndex(), startItem->GetOutputIndex());

            startItem->AddPloyLine(plyLine);
            endItem->AddPloyLine(plyLine);
            plyLine->UpdatePosition();
        }
    }

    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }
    //����ִ��˳��
    //pView->UpdateItemsExecNum();
}

void CDataIOAnalyse::CutCommentItem(const QList<COMMENTDATA> &lstLines)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    for each (COMMENTDATA commentItem in lstLines)
    {
        CCommentItem *pComment = new CCommentItem;

        pComment->setPos(m_pfEndPoint.x() + commentItem.pfPos.x() - m_pfStartPoint.x(), m_pfEndPoint.y() + commentItem.pfPos.y() - m_pfStartPoint.y());
        pComment->setPlainText(commentItem.strComment);
        pComment->ResetRect(commentItem.rfRect);

        m_pScene->addItem(pComment);
        //m_pScene->AddOneComment(pComment);
    }
}
