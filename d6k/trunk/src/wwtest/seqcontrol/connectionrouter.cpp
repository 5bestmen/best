#include <QGraphicsScene>
#include <QDebug>
#include "connectionrouter.h"
#include "funblockdiagramitem.h"
#include "fdborderbaseitem.h"
#include "connectionlineitem.h"
#include "fdborderscene.h"
#include "fdborderview.h"


CConnectionRouter::CConnectionRouter(CFdbOrderBaseItem *startItem, CFdbOrderBaseItem *endItem,QGraphicsScene *scene)
{
    Q_ASSERT(startItem);
    Q_ASSERT(endItem);
    Q_ASSERT(scene);
    if (startItem == nullptr || endItem == nullptr || scene == nullptr)
    {
        return;
    }
    
    m_nEndEnterIndex = 0;
    m_nBeginEnterIndex = 0;
    m_pStartItem = startItem;
    m_pEndItem = endItem;
    m_pScene = scene;
    m_nBeginWidth = m_pStartItem->GetPointWidth();
    m_nBeginHeight = m_pStartItem->getPointHeight();
    m_nEndWidth = m_pEndItem->GetPointWidth();
    m_nEndHeight = m_pEndItem->getPointHeight();
}

CConnectionRouter::~CConnectionRouter()
{
    //删除所有的直线
    CConnectionLineItem *item;
    QVector<CConnectionLineItem*>::iterator lineItem;

    for(lineItem = m_pVectorLines.begin(); lineItem != m_pVectorLines.end(); lineItem++)
    {
        item = (CConnectionLineItem*)*lineItem;
        delete item;
    }
    //删除关系
    m_pStartItem->RemovePloyLine(this);
    m_pEndItem->RemovePloyLine(this);

    m_pVectorLines.clear();
}

void CConnectionRouter::UpdatePosition()
{
    //
    //删除所有的直线
    CConnectionLineItem *item = nullptr;
    QVector<CConnectionLineItem*>::iterator lineItem;

    for(lineItem = m_pVectorLines.begin(); lineItem != m_pVectorLines.end(); lineItem++)
    {
        item = (CConnectionLineItem*)*lineItem;
        delete item;
    }

    m_pVectorLines.clear();
    //获取更新后的点信息
    GetPoint();
    //分析点
    UpdateAllLines();

    //根据点集划线
    DrawLineFromPoints();

}

void CConnectionRouter::GetPoint()
{
    m_SPoints.top_x = m_pStartItem->pos().x() + m_nBeginWidth;
    m_SPoints.top_y = m_pStartItem->pos().y() + m_nBeginHeight;
    m_SPoints.buttom_x = m_pEndItem->pos().x() + m_nEndWidth;
    m_SPoints.buttom_y = m_pEndItem->pos().y() + m_nEndHeight;
}

void CConnectionRouter::GetNewPoint()
{
    if (m_pVectorLines.count() > 0)
    {
        QPointF point1 = this->m_pVectorLines.at(0)->mapFromItem(m_pStartItem, 0, 0);
        QPointF point2 = this->m_pVectorLines.at(0)->mapFromItem(m_pEndItem, 0, 0);

        m_SPoints.top_x = point1.x() + m_nBeginWidth;
        m_SPoints.top_y = point1.y() + m_nBeginHeight;;
        m_SPoints.buttom_x = point2.x() + m_nEndWidth;
        m_SPoints.buttom_y = point2.y() + m_nEndHeight;;
    }
}

void CConnectionRouter::UpdateAllLines()
{
    //根据起点和重点位置  寻找拐点

    m_pLstAllPoints.clear();

    m_pLstAllPoints.append(QPointF(m_SPoints.top_x,m_SPoints.top_y));

    int nNodeLegth = 14;
    //起始点
    CFdbOrderBaseItem *pBgenGraphicItem = dynamic_cast<CFdbOrderBaseItem*>(m_pScene->itemAt(m_SPoints.top_x + nNodeLegth, m_SPoints.top_y, QTransform()));

    if (pBgenGraphicItem != NULL)
    {
        m_pfBeginPoint = QPointF(m_SPoints.top_x-nNodeLegth,m_SPoints.top_y);
    }
    else
    {
        m_pfBeginPoint = QPointF(m_SPoints.top_x+nNodeLegth,m_SPoints.top_y);
    }

    //结束点
    if (dynamic_cast<CFdbOrderBaseItem*>(m_pScene->itemAt(m_SPoints.buttom_x + nNodeLegth, m_SPoints.buttom_y, QTransform())) != NULL)
    {
        m_pfEndPoint = QPointF(m_SPoints.buttom_x-nNodeLegth,m_SPoints.buttom_y);

    }
    else
    {
        m_pfEndPoint = QPointF(m_SPoints.buttom_x+nNodeLegth,m_SPoints.buttom_y);
    }

    m_pLstAllPoints.append(m_pfBeginPoint);
    //分析中间点
    AnalyseAllPoints();


    //m_pLstAllPoints.append(m_pfEndPoint);
    
    
    m_pLstAllPoints.append(QPointF(m_SPoints.buttom_x,m_SPoints.buttom_y));

    int nCountIndex = m_pLstAllPoints.count() - 1;

    if (m_pLstAllPoints.count() > 3)
    {
        if (m_pLstAllPoints.at(nCountIndex).y() == m_pLstAllPoints.at(nCountIndex - 1).y() 
            && m_pLstAllPoints.at(nCountIndex-1).y() == m_pLstAllPoints.at(nCountIndex - 2).y())
        {
            m_pLstAllPoints.removeAt(nCountIndex - 1);
        }
    }

    DeleteLinesPoints();
}

void CConnectionRouter::AnalyseAllPoints()
{
    StepToEnd();
}

void CConnectionRouter::DrawLineFromPoints()
{
    for (int i=1; i<m_pLstAllPoints.count(); i++)
    {

        CConnectionLineItem *item = new CConnectionLineItem(m_pLstAllPoints.at(i-1).x(),m_pLstAllPoints.at(i-1).y(),
                                                        m_pLstAllPoints.at(i).x(),m_pLstAllPoints.at(i).y());
        item->SetOwnRounter(this);

        if (i == 1 || i == m_pLstAllPoints.count() - 1)
        {
            //item->SetUnMoveable();
        }

        m_pScene->addItem(item);

        m_pVectorLines.push_back(item);

    }

    for (int m=0; m<m_pVectorLines.count(); m++)
    {
        if (m_pVectorLines.count() > m+1)
        {
            m_pVectorLines.at(m)->SetLinkLineItem1(m_pVectorLines.at(m+1));
        }

        if (m>0)
        {
            m_pVectorLines.at(m)->SetLinkLineItem2(m_pVectorLines.at(m-1));
        }
    }
}

void CConnectionRouter::StepToEnd()
{
    //
    QPointF pBegin = m_pfBeginPoint;
    QPointF pEnd = m_pfEndPoint;
    //走x为true
    bool bFlagX = true;
    int nBackStepX = 14;
    int nBackStepY = 14;

    int ndosteps = 0;

    while (pBegin != pEnd)
    {
        ndosteps++;
        if (ndosteps >300)
        {
            break;
        }
        int nXStep = 0;
        if (pEnd.x() > pBegin.x())
        {
            nXStep = 14;
        }
        else if (pEnd.x() < pBegin.x())
        {
            nXStep = -14;
        }
        else
        {
            nXStep = 0;
        }

        int nYStep = 0;
        if (pEnd.y() > pBegin.y())
        {
            nYStep = 14;
        }
        else if (pEnd.y() < pBegin.y())
        {
            nYStep = -14;
        }
        else
        {
            nYStep = 0;
        }


        if (bFlagX)
        {
            QGraphicsItem *PCurrrentItem = NULL;
            //是否是拐点
            bool bCornorFlag = false;

            if (fabs(pBegin.x()-pEnd.x()) <= fabs(double(nXStep)))
            {
                //拐点
                PCurrrentItem = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pEnd.x(), pBegin.y(), QTransform()));
                bCornorFlag = true;
            }
            else
            {
                PCurrrentItem = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x() + nXStep, pBegin.y(), QTransform()));
            }

            if (PCurrrentItem != NULL && PCurrrentItem->type() == OBJECTITEM)
            {
                m_pLstAllPoints.append(pBegin);
                pBegin.setX(pBegin.x());

                if (pBegin.y() == pEnd.y())
                {
                    pBegin.setY(pBegin.y() + nBackStepY);

                    QGraphicsItem *item0 =
                        qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x() + nBackStepX, pBegin.y(), QTransform()));

                    int nItemoflag = 0;
                    while (item0 != NULL && item0->type() == OBJECTITEM)
                    {
                        nItemoflag++;
                        if (nItemoflag > 300)
                        {
                            break;
                        }
                        pBegin.setY(pBegin.y() + nBackStepY);
                        item0 = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x() + nBackStepX, pBegin.y(), QTransform()));
                    }
                    bFlagX = true;
                    m_pLstAllPoints.append(pBegin);

                }
                else
                {
                    if (fabs(pBegin.y()-pEnd.y()) <= fabs(double(nYStep)))
                    {
                        pBegin.setY(pEnd.y());
                    }
                    else
                    {
                        pBegin.setY(pBegin.y() + nBackStepY);
                    }

                    //x方向结束行驶
                    bFlagX = false;
                }


            }
            else
            {
                if (bCornorFlag == true)
                {
                    bFlagX = false;
                    if (ndosteps == 1)
                    {
                        m_pLstAllPoints.append(pBegin);
                    }
                    else
                    {
                        pBegin.setX(pEnd.x());
                        pBegin.setY(pBegin.y());
                        m_pLstAllPoints.append(pBegin);

                    }
                }
                else
                {
                    pBegin.setX(pBegin.x()+nBackStepX);
                    pBegin.setY(pBegin.y());
                }
            }
        }
        else
        {

            QGraphicsItem *PCurrrentItem = NULL;
            //是否是拐点
            bool bCornorFlag = false;

            if (fabs(pBegin.y()-pEnd.y()) <= fabs(double(nYStep)))
            {
                //拐点
                PCurrrentItem = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pEnd.y(), QTransform()));
                bCornorFlag = true;
            }
            else
            {
                PCurrrentItem = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pBegin.y() + nYStep, QTransform()));
            }

            if (PCurrrentItem != NULL && PCurrrentItem->type() == OBJECTITEM)
            {
                m_pLstAllPoints.append(pBegin);
                pBegin.setY(pBegin.y());

                if (pBegin.x() == pEnd.x())
                {
                    pBegin.setX(pBegin.x() + nBackStepX);
                    QGraphicsItem *item0 =
                        qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pBegin.y() + nBackStepY, QTransform()));

                    int nItemofalog = 0;
                    while (item0 != NULL && item0->type() == OBJECTITEM)
                    {
                        nItemofalog++;
                        if (nItemofalog > 300)
                        {
                            break;
                        }
                        pBegin.setX(pBegin.x() + nBackStepX);
                        item0 = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pBegin.y() + nBackStepY, QTransform()));
                    }

                    bFlagX = false;
                    m_pLstAllPoints.append(pBegin);

                }
                else
                {

                    if (fabs(pBegin.x()-pEnd.x()) <= fabs(double(nYStep)))
                    {
                        pBegin.setX(pBegin.x());
                    }
                    else
                    {
                        pBegin.setX(pBegin.x()+nBackStepX);
                    }

                    //y方向结束行驶
                    bFlagX = true;
                }
            }
            else
            {
                if (bCornorFlag == true)
                {
                    bFlagX = true;

                    pBegin.setX(pBegin.x());
                    pBegin.setY(pEnd.y());
                    m_pLstAllPoints.append(pBegin);
                }
                else
                {
                    pBegin.setX(pBegin.x());
                    pBegin.setY(pBegin.y()+nBackStepY);
                }
            }
        }
         //保存上一个非0值
        if (nXStep != 0)
        {
            nBackStepX = nXStep;
        }

        if (nYStep != 0)
        {
            nBackStepY = nYStep;
        }
    }
}

void CConnectionRouter::DeleteLinesPoints()
{
    QList<QPointF> tLstPoints;

    if (m_pLstAllPoints.count() > 0)
    {
        tLstPoints.append(m_pLstAllPoints.at(0));

        for (int i=1; i<m_pLstAllPoints.count(); i++)
        {
            if (m_pLstAllPoints.at(i-1) != m_pLstAllPoints.at(i))
            {

                tLstPoints.append(m_pLstAllPoints.at(i));
            }
        }
    }

    m_pLstAllPoints = tLstPoints;

    tLstPoints.clear();

    for (int m=2; m<m_pLstAllPoints.count()-2; m++)
    {
        if ((m_pLstAllPoints.at(m-1).x()==m_pLstAllPoints.at(m).x() && m_pLstAllPoints.at(m).x()==m_pLstAllPoints.at(m+1).x())
                || (m_pLstAllPoints.at(m-1).y()==m_pLstAllPoints.at(m).y() && m_pLstAllPoints.at(m).y()==m_pLstAllPoints.at(m+1).y()))
        {
            m_pLstAllPoints.removeAt(m);
            m--;
        }
    }
}

void CConnectionRouter::InsertLineItemToVector(CConnectionLineItem *pBeginItem, CConnectionLineItem *pMidItem, CConnectionLineItem *pEndItem)
{
    if (m_pVectorLines.size() < 2)
    {
        return;
    }

    int nBeginIndex = m_pVectorLines.indexOf(pBeginItem);
    int nEndIndex = m_pVectorLines.indexOf(pEndItem);

    if (nBeginIndex > nEndIndex)
    {
        m_pVectorLines.insert(nBeginIndex,pMidItem);
    }
    else
    {
        m_pVectorLines.insert(nEndIndex,pMidItem);
    }


    for (int m=0; m<m_pVectorLines.count(); m++)
    {
        if (m_pVectorLines.count() > m+1)
        {
            m_pVectorLines.at(m)->SetLinkLineItem1(m_pVectorLines.at(m+1));
        }

        if (m > 0)
        {
            m_pVectorLines.at(m)->SetLinkLineItem2(m_pVectorLines.at(m-1));
        }
    }
}

void CConnectionRouter::SetEndInput(int nIndex)
{
    m_nEndEnterIndex = nIndex;
}

void CConnectionRouter::SetBegiOutput(int nIndex)
{
    m_nBeginEnterIndex = nIndex;
}


int CConnectionRouter::GetEndInput()
{
    return m_nEndEnterIndex;
}



void CConnectionRouter::DeleteLinkInfo()
{
    m_pStartItem->DeleteOneOutputItem(m_pEndItem,m_nBeginEnterIndex);
    m_pEndItem->DeleteOneInputItem(m_pStartItem, m_nEndEnterIndex);
    
    this->deleteLater();

    //更新编号
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));
    if (pView != nullptr)
    {
        pView->UpdateItemsExecNum();
    }
}

QList<QPointF> CConnectionRouter::GetAllPoints()
{
    return m_pLstAllPoints;
}

void CConnectionRouter::SetAllPoints(const QList<QPointF> &lstPoints)
{
    m_pLstAllPoints = lstPoints;
}

void CConnectionRouter::UpdateLinesFromLink()
{
    QList<QPointF> lstPoints;

    for (int i = 0; i < m_pVectorLines.count()-1; i++)
    {
        if (m_pVectorLines.at(i)->line().p1() == m_pVectorLines.at(i+1)->line().p1())
        {
            if (!lstPoints.contains(m_pVectorLines.at(i)->line().p2()))
            {
                lstPoints.append(m_pVectorLines.at(i)->line().p2());
            }
            if (!lstPoints.contains(m_pVectorLines.at(i)->line().p1()))
            {
                lstPoints.append(m_pVectorLines.at(i)->line().p1());
            }
            
        }
        else
        {
            if (!lstPoints.contains(m_pVectorLines.at(i)->line().p1()))
            {
                lstPoints.append(m_pVectorLines.at(i)->line().p1());
            }
            
            if (!lstPoints.contains(m_pVectorLines.at(i)->line().p2()))
            {
                lstPoints.append(m_pVectorLines.at(i)->line().p2());
            }
            
        }

        if (i == m_pVectorLines.count() - 2)
        {
            if (!lstPoints.contains(m_pVectorLines.at(i)->line().p1()))
            {
                lstPoints.append(m_pVectorLines.at(i - 1)->line().p1());
            }
            
            if (!lstPoints.contains(m_pVectorLines.at(i)->line().p2()))
            {
                lstPoints.append(m_pVectorLines.at(i - 1)->line().p2());
            }
        }
    }

    lstPoints.append(m_pLstAllPoints.last());
    SetAllPoints(lstPoints);
    //当有三个点在一条直线上是  去除中间的点
    for (int m = 0; m < lstPoints.count() - 2; m++)
    {
        if ((lstPoints.at(m).x() == lstPoints.at(m+1).x() && lstPoints.at(m+1).x() == lstPoints.at(m+2).x())
            || (lstPoints.at(m).y() == lstPoints.at(m + 1).y() && lstPoints.at(m + 1).y() == lstPoints.at(m + 2).y()))
        {
            //删除所有的直线
            CConnectionLineItem *item = nullptr;
            QVector<CConnectionLineItem*>::iterator lineItem;

            for (lineItem = m_pVectorLines.begin(); lineItem != m_pVectorLines.end(); lineItem++)
            {
                item = (CConnectionLineItem*)*lineItem;
                delete item;
            }
            m_pVectorLines.clear();

            lstPoints.removeAt(m+1);
            SetAllPoints(lstPoints);
            DrawLineFromPoints();
        }
    }

    //更新
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(m_pScene);

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

void CConnectionRouter::SetLineColor(QColor colorLine)
{
    m_LineColor = colorLine;
    for (int i = 0; i < m_pVectorLines.count(); i++)
    {
        m_pVectorLines[i]->SetLineColor(colorLine);
        m_pVectorLines[i]->update();
    }
}
