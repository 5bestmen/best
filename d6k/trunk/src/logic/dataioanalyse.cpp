/*! @file
<PRE>
********************************************************************************
模块名       :  
文件名       :  dataioanalyse.cpp
文件实现功能 :  逻辑图的创建 复制 剪切等数据操作
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
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
#include "logiccontainter.h"
#include "logicmodel.h"

/*! \fn CDataIOAnalyse::CDataIOAnalyse(CFdbOrderScene *pScene)
*********************************************************************************************************
** \brief CDataIOAnalyse::CDataIOAnalyse(CFdbOrderScene *pScene)
** \details 构造函数
** \param   pScene:场景
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CDataIOAnalyse::CDataIOAnalyse(CFdbOrderScene *pScene)
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

/*! \fn CDataIOAnalyse::CopyLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock)
*********************************************************************************************************
** \brief CDataIOAnalyse::CopyLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock)
** \details 复制对象
** \param   tBlock:复制的对象信息
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CDataIOAnalyse::CopyLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }
    //复制对象的编号  和  对应的逻辑块
    m_mapCopyNumItem.clear();

    //遍历数据 创建对象
    for each (std::shared_ptr<CBlockProperty> itemInfo in tBlock)
    {
        //创建item对象  逻辑对象
        CFdbOrderBaseItem *pLaderrItem = m_pScene->CreateItemObj(itemInfo->GetItemType(), pView->GetNextCreateNum());

        if (pLaderrItem == nullptr)
        {
            continue;
        }
        //item执行编号
        pLaderrItem->SetIndexNumber(pView->GetNextCreateNum());
        //item大小
        pLaderrItem->ResetRect(QRectF(0, 0, itemInfo->GetBlockRect().width(), itemInfo->GetBlockRect().height()));
        //item注释
        pLaderrItem->Slot_UpdateComment(itemInfo->GetComment());
        //en
        if (itemInfo->GetEnoShowFlag())
        {
            pLaderrItem->ShowEnoItems();
        }
        else
        {
            pLaderrItem->CloseEnoItems();
        }
        //TODO   管点具体属性以后根据实际情况定

        pView->AddCreateNum();
        pView->InsertOneItem(pLaderrItem);
        //item坐标
        pLaderrItem->setPos(m_pfEndPoint.x() + itemInfo->GetBlockRect().x() - m_pfStartPoint.x(), m_pfEndPoint.y() + itemInfo->GetBlockRect().y() - m_pfStartPoint.y());
        m_pScene->addItem(pLaderrItem);
        //复制对象创建编号和对象
        m_mapCopyNumItem.insert(itemInfo->GetCreateNum(), pLaderrItem);

    }
}

/*! \fn CDataIOAnalyse::CopyLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine)
*********************************************************************************************************
** \brief CDataIOAnalyse::CopyLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine)
** \details 复制连线
** \param   tBlock:复制的对象信息
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CDataIOAnalyse::CopyLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine)
{
    //遍历逻辑线
    for each (std::shared_ptr<CLineProperty> lineItem in tLine)
    {
        if (m_mapCopyNumItem.contains(lineItem->GetBeginItemIndex()) && m_mapCopyNumItem.contains(lineItem->GetEndItemIndex()))
        {
            CFdbOrderBaseItem *startItem = m_mapCopyNumItem[lineItem->GetBeginItemIndex()];
            CFdbOrderBaseItem *endItem = m_mapCopyNumItem[lineItem->GetEndItemIndex()];

            startItem->SetItemStatus(IO_OUTPUT);
            startItem->SetOutPutIndex(lineItem->GetBeginPinIndex());

            endItem->SetItemStatus(IO_INPUT);
            endItem->SetInPutIndex(lineItem->GetEndPinIndex());

            CConnectionRouter *plyLine = new CConnectionRouter(startItem, endItem, m_pScene);

            plyLine->SetEndInput(endItem->GetInputIndex());
            plyLine->SetBegiOutput(startItem->GetOutputIndex());
            //增加关联
            startItem->InsertOuputItem(startItem->GetOutputIndex(), endItem);
            endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
            //更新直角线
            //更新直角线
            QList<QPointF> lstPoints;

            // 
            for (auto it : lineItem->GetAllPoints())
            {
                lstPoints.append(QPointF(it->x(), it->y()));
            }

            plyLine->SetAllPoints(lstPoints);

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
    //更新执行顺序
    pView->UpdateItemsExecNum();
}


void CDataIOAnalyse::CopyCommentItem(std::vector<std::shared_ptr<CCommentProperty>> tComment)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    for each (std::shared_ptr<CCommentProperty> commentItem in tComment)
    {
        CCommentItem *pComment = new CCommentItem;

        pComment->setPos(m_pfEndPoint.x() + commentItem->GetCommentPos().x() - m_pfStartPoint.x(), m_pfEndPoint.y() + commentItem->GetCommentPos().y() - m_pfStartPoint.y());
        pComment->setPlainText(commentItem->GetComment());
        pComment->ResetRect(commentItem->GetRect());

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


void CDataIOAnalyse::CutLineItem(std::vector<std::shared_ptr<CLineProperty>> tLine)
{
    for each (std::shared_ptr<CLineProperty> lineItem in tLine)
    {
        if (m_mapCopyNumItem.contains(lineItem->GetBeginItemIndex()) && m_mapCopyNumItem.contains(lineItem->GetEndItemIndex()))
        {
            CFdbOrderBaseItem *startItem = m_mapCopyNumItem[lineItem->GetBeginItemIndex()];
            CFdbOrderBaseItem *endItem = m_mapCopyNumItem[lineItem->GetEndItemIndex()];

            startItem->SetItemStatus(IO_OUTPUT);
            startItem->SetOutPutIndex(lineItem->GetBeginPinIndex());

            endItem->SetItemStatus(IO_INPUT);
            endItem->SetInPutIndex(lineItem->GetEndPinIndex());

            CConnectionRouter *plyLine = new CConnectionRouter(startItem, endItem, m_pScene);

            plyLine->SetEndInput(endItem->GetInputIndex());
            plyLine->SetBegiOutput(startItem->GetOutputIndex());
            //增加关联
            startItem->InsertOuputItem(startItem->GetOutputIndex(), endItem);
            endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
            //更新直角线
            QList<QPointF> lstPoints;

            for (auto it : lineItem->GetAllPoints())
            {
                lstPoints.append(QPointF(it->x(), it->y()));
            }

            plyLine->SetAllPoints(lstPoints);

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
    //更新执行顺序
    //pView->UpdateItemsExecNum();
}

void CDataIOAnalyse::CreateLogic()
{
    CreateLogicItem(m_pScene->GetObjectModel()->GetBlockProperty());
    
    CreateLogicLine(m_pScene->GetObjectModel()->GetLineProperty());

    CreateLogicComment(m_pScene->GetObjectModel()->GetCommentProperty());
}

void CDataIOAnalyse::CreateLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock)
{
    //
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    pView->ClearCreateNum();

    for each (std::shared_ptr<CBlockProperty> itemInfo in tBlock)
    {
        //创建item对象  逻辑对象
        CFdbOrderBaseItem *pLaderrItem = nullptr;

        pLaderrItem = m_pScene->CreateItemObj(itemInfo->GetItemType(), itemInfo->GetCreateNum());

        if (pLaderrItem == nullptr)
        {
            continue;
        }

        //item执行编号
        pLaderrItem->SetIndexNumber(itemInfo->GetExecNum());
        //item大小
        pLaderrItem->ResetRect(QRectF(0, 0, itemInfo->GetBlockRect().width(), itemInfo->GetBlockRect().height()));
        //item注释
        pLaderrItem->Slot_UpdateComment(itemInfo->GetComment());
        //en
        if (itemInfo->GetEnoShowFlag())
        {
            pLaderrItem->ShowEnoItems();
        }
        else
        {
            pLaderrItem->CloseEnoItems();
        }
        //TODO   管点具体属性以后根据实际情况定
		QMap<int, QString> tmapBindValue;

		for (auto item : itemInfo->GetPinValues())
		{
			if (item->GetCellPro() != nullptr)
			{
				if (item->GetPinType() == 2)
				{
					//绑定值
					tmapBindValue.insert(item->GetPinIndex(), item->GetCellPro()->GetCellName());

				}
			}
		}

		pLaderrItem->Slot_updateBindValueInfo(tmapBindValue);

        pView->AddCreateNum();
        pView->InsertOneItem(pLaderrItem);
        //item坐标
        pLaderrItem->setPos(itemInfo->GetBlockRect().x(), itemInfo->GetBlockRect().y());
        m_pScene->addItem(pLaderrItem);
        m_mapCreateNumItem.insert(itemInfo->GetCreateNum(), pLaderrItem);
    }
}

void CDataIOAnalyse::CreateLogicLine(std::vector<std::shared_ptr<CLineProperty>> tLine)
{
    //连线
    for each (std::shared_ptr<CLineProperty> lineItem in tLine)
    {
        if (m_mapCreateNumItem.contains(lineItem->GetBeginItemIndex()) && m_mapCreateNumItem.contains(lineItem->GetEndItemIndex()))
        {
            CFdbOrderBaseItem *startItem = m_mapCreateNumItem[lineItem->GetBeginItemIndex()];
            CFdbOrderBaseItem *endItem = m_mapCreateNumItem[lineItem->GetEndItemIndex()];

            startItem->SetItemStatus(IO_OUTPUT);
            startItem->SetOutPutIndex(lineItem->GetBeginPinIndex());

            endItem->SetItemStatus(IO_INPUT);
            endItem->SetInPutIndex(lineItem->GetEndPinIndex());

            CConnectionRouter *plyLine = new CConnectionRouter(startItem, endItem, m_pScene);

            plyLine->SetEndInput(endItem->GetInputIndex());
            plyLine->SetBegiOutput(startItem->GetOutputIndex());
            //增加关联
            startItem->InsertOuputItem(startItem->GetOutputIndex(), endItem);
            endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
            //更新直角线
            QList<QPointF> lstPoints;
            
             // 
            for (auto it : lineItem->GetAllPoints())
            {
                lstPoints.append(QPointF(it->x(),it->y()));
            }

            plyLine->SetAllPoints(lstPoints);
            plyLine->DrawLineFromPoints();

            endItem->InsertPins(endItem->GetInputIndex(), startItem->GetOutputIndex());

            startItem->AddPloyLine(plyLine);
            endItem->AddPloyLine(plyLine);

        }
    }
}

void CDataIOAnalyse::CreateLogicComment(std::vector<std::shared_ptr<CCommentProperty>> tComment)
{
    for each (std::shared_ptr<CCommentProperty> commentItem in tComment)
    {
        CCommentItem *pComment = new CCommentItem;
        pComment->setPos(commentItem->GetCommentPos());
        pComment->setPlainText(commentItem->GetComment());
        pComment->ResetRect(commentItem->GetRect());

        m_pScene->addItem(pComment);
        //m_pScene->AddOneComment(pComment);
    }
}

void CDataIOAnalyse::CutLogicItem(std::vector<std::shared_ptr<CBlockProperty>> tBlock)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    //QPointF pfMouse = pView->mapToScene(pView->mapFromGlobal(QCursor::pos()));

    m_mapCopyNumItem.clear();

    for each (std::shared_ptr<CBlockProperty> itemInfo in tBlock)
    {
        //创建item对象  逻辑对象
        //CFdbOrderBaseItem *pLaderrItem = nullptr;

        CFdbOrderBaseItem *pLaderrItem = m_pScene->CreateItemObj(itemInfo->GetItemType(), pView->GetNextCreateNum());

        if (pLaderrItem == nullptr)
        {
            continue;
        }

        //设置创建编号
        pLaderrItem->SetCreateNum(itemInfo->GetCreateNum());
        //item执行编号
        pLaderrItem->SetIndexNumber(itemInfo->GetExecNum());
        //item大小
        pLaderrItem->ResetRect(QRectF(0, 0, itemInfo->GetBlockRect().width(), itemInfo->GetBlockRect().height()));
        //item注释
        pLaderrItem->Slot_UpdateComment(itemInfo->GetComment());
        //en
        if (itemInfo->GetEnoShowFlag())
        {
            pLaderrItem->ShowEnoItems();
        }
        else
        {
            pLaderrItem->CloseEnoItems();
        }
        //TODO   管点具体属性以后根据实际情况定

        pView->AddCreateNum();
        pView->InsertOneItem(pLaderrItem);
        //item坐标
        pLaderrItem->setPos(m_pfEndPoint.x() + itemInfo->GetBlockRect().x() - m_pfStartPoint.x(), m_pfEndPoint.y() + itemInfo->GetBlockRect().y() - m_pfStartPoint.y());
        m_pScene->addItem(pLaderrItem);
        m_mapCopyNumItem.insert(itemInfo->GetCreateNum(), pLaderrItem);

    }
}

void CDataIOAnalyse::CutCommentItem(std::vector<std::shared_ptr<CCommentProperty>> tComment)
{
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    for each (std::shared_ptr<CCommentProperty> commentItem in tComment)
    {
        CCommentItem *pComment = new CCommentItem;

        pComment->setPos(m_pfEndPoint.x() + commentItem->GetCommentPos().x() - m_pfStartPoint.x(), m_pfEndPoint.y() + commentItem->GetCommentPos().y() - m_pfStartPoint.y());
        pComment->setPlainText(commentItem->GetComment());
        pComment->ResetRect(commentItem->GetRect());

        m_pScene->addItem(pComment);
        //m_pScene->AddOneComment(pComment);
    }
}
