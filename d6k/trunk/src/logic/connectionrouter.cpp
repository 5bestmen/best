/*! @file
<PRE>
********************************************************************************
模块名       :  连线管理对象
文件名       :  connectionrouter.cpp
文件实现功能 :  一组连接线
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
#include <QGraphicsScene>
#include <QDebug>
#include "connectionrouter.h"
#include "funblockdiagramitem.h"
#include "fdborderbaseitem.h"
#include "connectionlineitem.h"
#include "fdborderscene.h"
#include "fdborderview.h"

/*! \fn CConnectionRouter::CConnectionRouter(CFdbOrderBaseItem *startItem, CFdbOrderBaseItem *endItem,QGraphicsScene *scene)
*********************************************************************************************************
** \brief CConnectionRouter::CConnectionRouter(CFdbOrderBaseItem *startItem, CFdbOrderBaseItem *endItem,QGraphicsScene *scene)
** \details 构造函数
** \param   startItem起始逻辑块   endItem终止逻辑块  scene场景
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CConnectionRouter::~CConnectionRouter()
*********************************************************************************************************
** \brief CConnectionRouter::~CConnectionRouter()
** \details 函数
** \param   
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CConnectionRouter::~CConnectionRouter()
{
    //逻辑线消除时  要删除所有的直线对象
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

/*! \fn CConnectionRouter::UpdatePosition()
*********************************************************************************************************
** \brief CConnectionRouter::UpdatePosition()
** \details 更新所有的点位置
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CConnectionRouter::UpdateAllLines()
*********************************************************************************************************
** \brief CConnectionRouter::UpdateAllLines()
** \details 更新所有的点位置
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::UpdateAllLines()
{
    //根据起点和重点位置  寻找拐点
    //清除原有点左边信息
    m_pLstAllPoints.clear();
    //起点位置
    m_pLstAllPoints.append(QPointF(m_SPoints.top_x,m_SPoints.top_y));

    //第二点的位置  为起点伸长14 
    int nNodeLegth = 14;

    //根据是否有item 判断 向左还是向右 连线
    CFdbOrderBaseItem *pBgenGraphicItem = dynamic_cast<CFdbOrderBaseItem*>(m_pScene->itemAt(m_SPoints.top_x + nNodeLegth, m_SPoints.top_y, QTransform()));

    if (pBgenGraphicItem != NULL)
    {
        m_pfBeginPoint = QPointF(m_SPoints.top_x-nNodeLegth,m_SPoints.top_y);
    }
    else
    {
        m_pfBeginPoint = QPointF(m_SPoints.top_x+nNodeLegth,m_SPoints.top_y);
    }

    //根据是否有item 判断终点移动
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
    
    m_pLstAllPoints.append(QPointF(m_SPoints.buttom_x,m_SPoints.buttom_y));

    int nCountIndex = m_pLstAllPoints.count() - 1;

    //当点多于三个时  去掉中间重复的点
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

/*! \fn CConnectionRouter::DrawLineFromPoints()
*********************************************************************************************************
** \brief CConnectionRouter::DrawLineFromPoints()
** \details 根据点集 创建直线对象集
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::DrawLineFromPoints()
{
    for (int i=1; i<m_pLstAllPoints.count(); i++)
    {
        //创建直线对象
        CConnectionLineItem *item = new CConnectionLineItem(m_pLstAllPoints.at(i-1).x(),m_pLstAllPoints.at(i-1).y(),
                                                        m_pLstAllPoints.at(i).x(),m_pLstAllPoints.at(i).y());
        //所有的直线对象都是由 路由对象管理
        item->SetOwnRounter(this);

        m_pScene->addItem(item);

        m_pVectorLines.push_back(item);
    }

    //设置直线对象之间的连接关系
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

/*! \fn CConnectionRouter::StepToEnd()
*********************************************************************************************************
** \brief CConnectionRouter::StepToEnd()
** \details 寻找路径方法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::StepToEnd()
{
    //起始点
    QPointF pBegin = m_pfBeginPoint;
    //终点
    QPointF pEnd = m_pfEndPoint;
    //走x为true
    bool bFlagX = true;
    //步长x
    int nBackStepX = 14;
    //步长y
    int nBackStepY = 14;

    //步数
    int ndosteps = 0;

    //寻找路径  直到起点 移动到 终点
    while (pBegin != pEnd)
    {
        ndosteps++;

        if (ndosteps >300)
        {
            break;
        }

        int nXStep = 0;
        //单位位移 正负性判断
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
        //单位位移 正负性判断
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


        if (bFlagX)    //在x轴方向移动
        {
            //移动的地方是否有逻辑块
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

            if (PCurrrentItem != NULL && PCurrrentItem->type() == OBJEC_TITEM)   //单位位移后  存在逻辑块
            {
                //保存上一个点 作为拐点
                m_pLstAllPoints.append(pBegin);
                pBegin.setX(pBegin.x());

                if (pBegin.y() == pEnd.y())   
                {
                    //换个方向移动
                    pBegin.setY(pBegin.y() + nBackStepY);

                    QGraphicsItem *item0 =
                        qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x() + nBackStepX, pBegin.y(), QTransform()));

                    int nItemoflag = 0;
                    while (item0 != NULL && item0->type() == OBJEC_TITEM)  //是否存在逻辑块
                    {
                        nItemoflag++;
                        if (nItemoflag > 300)
                        {
                            break;
                        }
                        pBegin.setY(pBegin.y() + nBackStepY);
                        item0 = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x() + nBackStepX, pBegin.y(), QTransform()));
                    }
                    //该方向移动结束
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
                    //拐点  记录改点 并且位移方向改变
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
            //单位位移后是否存在逻辑块
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
                //非拐点
                PCurrrentItem = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pBegin.y() + nYStep, QTransform()));
            }

            if (PCurrrentItem != NULL && PCurrrentItem->type() == OBJEC_TITEM)   //单位位移后存在逻辑块
            {
                //记录位移前一点
                m_pLstAllPoints.append(pBegin);
                pBegin.setY(pBegin.y());

                if (pBegin.x() == pEnd.x())
                {
                    //换个方向继续位移
                    pBegin.setX(pBegin.x() + nBackStepX);
                    QGraphicsItem *item0 =
                        qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pBegin.y() + nBackStepY, QTransform()));

                    int nItemofalog = 0;
                    while (item0 != NULL && item0->type() == OBJEC_TITEM)   //单位位移后存在逻辑块
                    {
                        nItemofalog++;
                        if (nItemofalog > 300)
                        {
                            break;
                        }
                        pBegin.setX(pBegin.x() + nBackStepX);
                        item0 = qgraphicsitem_cast<QGraphicsItem*>(m_pScene->itemAt(pBegin.x(), pBegin.y() + nBackStepY, QTransform()));
                    }

                    //记录拐点
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

/*! \fn CConnectionRouter::DeleteLinesPoints()
*********************************************************************************************************
** \brief CConnectionRouter::DeleteLinesPoints()
** \details  去除重复的点  一条直线上只需要两个点
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::DeleteLinesPoints()
{
    //点集合
    QList<QPointF> tLstPoints;

    if (m_pLstAllPoints.count() > 0)
    {
        //去除重复的点
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
        //去除三个点在一条线上的多于的点
        if ((m_pLstAllPoints.at(m-1).x()==m_pLstAllPoints.at(m).x() && m_pLstAllPoints.at(m).x()==m_pLstAllPoints.at(m+1).x())
                || (m_pLstAllPoints.at(m-1).y()==m_pLstAllPoints.at(m).y() && m_pLstAllPoints.at(m).y()==m_pLstAllPoints.at(m+1).y()))
        {
            m_pLstAllPoints.removeAt(m);
            m--;
        }
    }
}

/*! \fn CConnectionRouter::InsertLineItemToVector(CConnectionLineItem *pBeginItem, CConnectionLineItem *pMidItem, CConnectionLineItem *pEndItem)
*********************************************************************************************************
** \brief CConnectionRouter::InsertLineItemToVector(CConnectionLineItem *pBeginItem, CConnectionLineItem *pMidItem, CConnectionLineItem *pEndItem)
** \details  插入新的直线对象  更新直线集合
** \param  pBeginItem:起始直线对象  pMidItem:中间直线对象  pEndItem:终止直线对象
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::InsertLineItemToVector(CConnectionLineItem *pBeginItem, CConnectionLineItem *pMidItem, CConnectionLineItem *pEndItem)
{
    Q_ASSERT(pBeginItem);
    Q_ASSERT(pMidItem);
    Q_ASSERT(pEndItem);

    if (pBeginItem == nullptr || pMidItem == nullptr || pEndItem == nullptr)
    {
        return;
    }

    //只有起点线 和 终点线 将不做处理
    if (m_pVectorLines.size() < 2)
    {
        return;
    }

    //起始线编号
    int nBeginIndex = m_pVectorLines.indexOf(pBeginItem);
    //终止线编号
    int nEndIndex = m_pVectorLines.indexOf(pEndItem);

    //在两条线中间插入一条新创建的线
    if (nBeginIndex > nEndIndex)
    {
        m_pVectorLines.insert(nBeginIndex,pMidItem);
    }
    else
    {
        m_pVectorLines.insert(nEndIndex,pMidItem);
    }

    //插入线后  更新他们之间的连接关系
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

/*! \fn CConnectionRouter::SetEndInput(int nIndex)
*********************************************************************************************************
** \brief CConnectionRouter::SetEndInput(int nIndex)
** \details
** \param  结束点编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::SetEndInput(int nIndex)
{
    m_nEndEnterIndex = nIndex;
}

/*! \fn CConnectionRouter::SetBegiOutput(int nIndex)
*********************************************************************************************************
** \brief CConnectionRouter::SetBegiOutput(int nIndex)
** \details
** \param  起始点编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::SetBegiOutput(int nIndex)
{
    m_nBeginEnterIndex = nIndex;
}


int CConnectionRouter::GetEndInput()
{
    return m_nEndEnterIndex;
}


/*! \fn CConnectionRouter::DeleteLinkInfo()
*********************************************************************************************************
** \brief CConnectionRouter::DeleteLinkInfo()
** \details  
** \param  删除对象前  先删除线和逻辑块的连接关系
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::DeleteLinkInfo()
{
    //删除起始逻辑块和逻辑线的关系
    m_pStartItem->DeleteOneOutputItem(m_pEndItem,m_nBeginEnterIndex);
    //删除结束逻辑块和逻辑线的关系
    m_pEndItem->DeleteOneInputItem(m_pStartItem, m_nEndEnterIndex);
    
    this->deleteLater();

    //更新编号
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(m_pScene->views().at(0));
    if (pView != nullptr)
    {
        pView->UpdateItemsExecNum();
    }

    //更新数据模型数据
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(m_pScene);

     if (pScene != nullptr)
     {
         pScene->UpdateOperate();
     }
}

/*! \fn CConnectionRouter::GetAllPoints()
*********************************************************************************************************
** \brief CConnectionRouter::GetAllPoints()
** \details
** \param  获取所有的点
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
const QList<QPointF>& CConnectionRouter::GetAllPoints()
{
    return m_pLstAllPoints;
}

/*! \fn CConnectionRouter::SetAllPoints(const QList<QPointF> &lstPoints)
*********************************************************************************************************
** \brief CConnectionRouter::SetAllPoints(const QList<QPointF> &lstPoints)
** \details
** \param  设置所有的点
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::SetAllPoints(const QList<QPointF> &lstPoints)
{
    m_pLstAllPoints = lstPoints;
}

/*! \fn CConnectionRouter::UpdateLinesFromLink()
*********************************************************************************************************
** \brief CConnectionRouter::UpdateLinesFromLink()
** \details
** \param  设置所有的点
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::UpdateLinesFromLink()
{
    //点集合
    QList<QPointF> lstPoints;

    //去除所有直线对象的点  重复的点不要
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
            //SetAllPoints(lstPoints);
            DrawLineFromPoints();
        }
    }

    SetAllPoints(lstPoints);

    //更新
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(m_pScene);

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

/*! \fn CConnectionRouter::SetLineColor(const QColor &colorLine)
*********************************************************************************************************
** \brief CConnectionRouter::SetLineColor(const QColor &colorLine)
** \details
** \param  设置逻辑线颜色
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionRouter::SetLineColor(const QColor &colorLine)
{
    m_LineColor = colorLine;
    for (int i = 0; i < m_pVectorLines.count(); i++)
    {
        m_pVectorLines[i]->SetLineColor(colorLine);
        m_pVectorLines[i]->update();
    }
}
