/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  fdborderbaseitem.cpp
文件实现功能 :  逻辑块基类
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
#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "fdborderbaseitem.h"
#include "fdborderview.h"
#include "connectionrouter.h"
#include "fdborderscene.h"
#include "logiccontainter.h"
#include "logicmodel.h"

/*! \fn CFdbOrderBaseItem::CFdbOrderBaseItem()
*********************************************************************************************************
** \brief CFdbOrderBaseItem()
** \details 构造函数
** \param   
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFdbOrderBaseItem::CFdbOrderBaseItem()
{
    m_bChangeFlag = false;
    m_nInputIndex = 0;
    m_nOutputIndex = 0;
    m_bEnoFlag = false;
    m_nCurrentWidth = 0;
    m_nChangeSize = 0;
    m_bScanl = false;
    m_nItemStatus = 0;
    m_nCreateNum = 0;
    m_nIndexNumber = 0;
    m_nInsertMaxHeight = 0;
    m_nOutMaxNum = 0;
    m_nInputNums = 0;
    m_nItemMaxHeight = 0;
    m_nOutMaxHeight = 0;
    m_pCommentTextItem = NULL;
    m_nCurrentOutputNums = 0;
    m_nEnFlag = 1;
    m_nEnoFlag = 1;

    m_rect = QRectF(0, 0, 110, 120);
    m_cursor = new QCursor;
    
    m_ItemColor = QColor(255, 224, 192);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setAcceptHoverEvents(true);
    //item在最上层 注释第一层  直线第二层
    this->setZValue(3);
}

/*! \fn CFdbOrderBaseItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief AnalyseItemType(int nType)
** \details 返回说明标识
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QString  CFdbOrderBaseItem::AnalyseItemType(int nType)
{
    return QString::number(nType);
}

/*! \fn CFdbOrderBaseItem::ResetRect(QRectF rect)
*********************************************************************************************************
** \brief ResetRect(QRectF rect)
** \details 改变大小
** \param  rect:逻辑块数据
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::ResetRect(QRectF rect)
{
    m_rect = rect;
    update(boundingRect());
}

/*! \fn CFdbOrderBaseItem::boundingRect()
*********************************************************************************************************
** \brief boundingRect()
** \details item形状
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QRectF CFdbOrderBaseItem::boundingRect() const
{
    return m_rect;
}

/*! \fn CFdbOrderBaseItem::shape()
*********************************************************************************************************
** \brief shape()
** \details item shape
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QPainterPath CFdbOrderBaseItem::shape() const
{
    QPainterPath path;
    //shape大小大于实际item大小  防止两个item距离太近
    path.addRect(QRectF(-28, -28, m_rect.width() + 42, m_rect.height() + 42));
    return path;
}

/*! \fn CFdbOrderBaseItem::RemovePloyLine(CConnectionRouter *ployline)
*********************************************************************************************************
** \brief RemovePloyLine(CConnectionRouter *ployline)
** \details ployline:需要删除的连接线关系
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::RemovePloyLine(CConnectionRouter *ployline)
{
    //去除线
    int index = m_polyLines.indexOf(ployline);

    if (index != -1)
    {
        m_polyLines.removeAt(index);
    }
}

/*! \fn CFdbOrderBaseItem::RemovePloyLines()
*********************************************************************************************************
** \brief RemovePloyLines()
** \details 删除与改item相关的所有的连接线
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::RemovePloyLines()
{   
    foreach(CConnectionRouter *ployline, m_polyLines)
    {
        ployline->StartItem()->RemovePloyLine(ployline);
        ployline->EndItem()->RemovePloyLine(ployline);

        delete ployline;
    }
}

/*! \fn CFdbOrderBaseItem::AddPloyLine(CConnectionRouter *ployline)
*********************************************************************************************************
** \brief AddPloyLine(CConnectionRouter *ployline)
** \details 增加连接线
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AddPloyLine(CConnectionRouter *ployline)
{
    m_polyLines.append(ployline);
}

/*! \fn CFdbOrderBaseItem::AddPloyLine(CConnectionRouter *ployline)
*********************************************************************************************************
** \brief GetItemIOStatus()
** \details 获取输入输出信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::GetItemIOStatus()
{
    // 1:输入端  2:输出端
    return m_nItemStatus;
}

/*! \fn CFdbOrderBaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief mousePressEvent(QGraphicsSceneMouseEvent *event)
** \details 鼠标点击事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            if (pScene->GetMode() != CURSOR_LINE)
            {
                //在非连接状态下 可以在管脚输入内容
                AddTextBinInfo();
            }
         
            m_nMoveBge = event->scenePos();
            m_pfClickPoint = this->pos();
            m_bScanl = true;
           
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        //删除操作
        QMenu *pMenu = new QMenu;
        QAction *pDeleteAct = new QAction(QObject::tr("Delete  "),this);
        connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteAct()));

        pMenu->addAction(pDeleteAct);
        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();
    }

    QGraphicsItem::mousePressEvent(event);
    update();
}

/*! \fn CFdbOrderBaseItem::Slot_DeleteAct()
*********************************************************************************************************
** \brief Slot_DeleteAct()
** \details 删除item槽函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::Slot_DeleteAct()
{
    //删除链表前后关系
    QMap<int, QList<CFdbOrderBaseItem*> >::const_iterator itBeg = m_mapNextInfos.constBegin();

    while (itBeg != m_mapNextInfos.constEnd())
    {
        for each (CFdbOrderBaseItem* item in itBeg.value())
        {
            //删除与该item相关连的item信息
            item->DeleteInputItem(this);
        }

        itBeg++;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = m_mapPreInfos.constBegin();

    while (it != m_mapPreInfos.constEnd())
    {
        it.value()->DeleteOutputItem(this);
        ++it;
    }
    //
    this->deleteLater();
    //删除该item相关的连线信息
    RemovePloyLines();

    //更新编号
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(this->scene()->views().at(0));
    if (pView != nullptr)
    {
        pView->DeleteRemoveItem(this);
        pView->UpdateItemsExecNum();
    }

    this->scene()->removeItem(this);

    //更新
    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

/*! \fn CFdbOrderBaseItem::AnalyseSencePoint(const QPointF &pScene)
*********************************************************************************************************
** \brief AnalyseSencePoint(const QPointF &pScene)
** \details 判断给定的点在哪个端子上
** \param pScece 点位置
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AnalyseSencePoint(const QPointF &pScene)
{
    AnalysePointPosition(mapFromScene(pScene));
}

/*! \fn CFdbOrderBaseItem::GetLinePoint(const QPointF &pItem)
*********************************************************************************************************
** \brief GetLinePoint(const QPointF &pItem)
** \details 判断给定的点在哪个端子上
** \param pScece 点位置
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::GetLinePoint(const QPointF &pItem)
{
    AnalysePointPosition(pItem);
}

/*! \fn CFdbOrderBaseItem::GetPointWidth()
*********************************************************************************************************
** \brief GetPointWidth()
** \details 获取点相对与item的宽度
** \param pScece 点位置
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::GetPointWidth() const
{
    return m_nCurrentWidth;
}

/*! \fn CFdbOrderBaseItem::getPointHeight()
*********************************************************************************************************
** \brief getPointHeight()
** \details 获取点相对与item的高度
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::getPointHeight() const
{
    return m_nCurrentHeight;
}

/*! \fn CFdbOrderBaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief mouseMoveEvent(QGraphicsSceneMouseEvent *event)
** \details 移动事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bScanl)   //拉动改变大小
    {
        QPointF dis;
        m_nMoveEnd = event->scenePos();
        //移动的距离
        dis = m_nMoveEnd - m_nMoveBge;
        m_nMoveBge = m_nMoveEnd;

        if (m_nChangeSize == 1)
        {
            QRectF tem = QRectF(0, m_rect.y(), m_rect.width(), m_rect.height() + dis.y());
            //item最大值  最小值判断
            if (m_rect.height() + dis.y() < m_nItemMaxHeight && m_rect.height() + dis.y() > 80)
            {
                int nMaxInIndex = 0;

                //找到 输入和输出 已经连线的点  item在改变大小时 不能小于已经连线的端子处
                for each (int nInIndex  in m_mapPreInfos.keys())
                {
                    if (nMaxInIndex < nInIndex)
                    {
                        nMaxInIndex = nInIndex;
                    }
                }

                for each (int nOutIndex in m_mapNextInfos.keys())
                {
                    if (nMaxInIndex < nOutIndex)
                    {
                        nMaxInIndex = nOutIndex;
                    }
                }

                if (nMaxInIndex * 18 + 45 < m_rect.height() + dis.y())
                {
                    this->ResetRect(tem);
                    //item单位位移判断
                    update(boundingRect());
                    this->setPos(this->x(),this->y()+0.1);
                }
            }
        }
    }

    m_bChangeFlag = true;

    update();

    QGraphicsItem::mouseMoveEvent(event);
}

/*! \fn CFdbOrderBaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
** \details 鼠标释放槽函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //item释放的地方 不能和其他item重叠
    QList<QGraphicsItem*> lstGraphicsItem = this->collidingItems();

    if (!lstGraphicsItem.isEmpty())
    {
        for (int i = 0; i < lstGraphicsItem.count(); i++)
        {
            CFdbOrderBaseItem * pItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(lstGraphicsItem.at(i));
            if (pItem != nullptr && pItem->type() == OBJEC_TITEM)
            {
                //如果重叠  放回原位置
                this->setPos(m_pfClickPoint);
            }
        }
    }

    if (m_bChangeFlag)
    {
        //item位置移动后  需要更新连线的信息
        foreach(CConnectionRouter *plyLine, m_polyLines)
        {
            plyLine->UpdatePosition();
        }
    }

    m_nInputIndex = 0;
    m_nOutputIndex = 0;
    m_bScanl = false;

    QGraphicsItem::mouseReleaseEvent(event);
    update();
    m_bChangeFlag = false;
    //更新 数据模型
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

/*! \fn CFdbOrderBaseItem::hoverMoveEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief hoverMoveEvent(QGraphicsSceneMouseEvent *event)
** \details 鼠标hover触发事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene == nullptr)
    {
        return;
    }

    if (pScene->GetMode() == CURSOR_LINE)
    {
        //当处于连线状态时  修改鼠标样式
        QGraphicsView *pView = pScene->views().at(0);
        if (pView != nullptr)
        {
            QPixmap pixmap(":/images/pen.png");
            QCursor pCursor(pixmap, 0, pixmap.height());
            this->setCursor(pCursor);
        }
    }
    else
    {

        QPointF pos1 = event->scenePos();
        QPointF lt = this->scenePos() + QPointF(m_rect.x(), m_rect.y());
        QPointF lb = this->scenePos() + QPointF(m_rect.x(), m_rect.y() + m_rect.height());
        QPointF rt = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y());

        if ((pos1.y() >= lb.y() - 2) && (pos1.x() >= lt.x() && pos1.x() <= rt.x()))
        {
            //改变鼠标样式 拖动使用
            m_cursor->setShape(Qt::SizeVerCursor);
            m_nChangeSize = 1;
            setFlag(QGraphicsItem::ItemIsMovable, false);
        }
        else
        {
            m_nChangeSize = 0;
            m_bScanl = false;
            m_cursor->setShape(Qt::ArrowCursor);
            setFlag(QGraphicsItem::ItemIsMovable, true);
        }
        this->setCursor(*m_cursor);
    }
    //判断鼠标所在位置
    QPointF pointCurrent = event->pos();

    //分析鼠标点处于哪个端子处
    AnalysePointPosition(pointCurrent);

    QGraphicsItem::hoverMoveEvent(event);
    update();
}

/*! \fn CFdbOrderBaseItem::hoverLeaveEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief hoverLeaveEvent(QGraphicsSceneMouseEvent *event)
** \details 鼠标离开触发事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_nOutputIndex = 0;
    m_nInputIndex = 0;
    //设置鼠标正常状态
    m_cursor->setShape(Qt::ArrowCursor);
    m_bScanl = false;
    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void CFdbOrderBaseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

/*! \fn CFdbOrderBaseItem:~CFdbOrderBaseItem()
*********************************************************************************************************
** \brief ~CFdbOrderBaseItem()
** \details 鼠标离开触发事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFdbOrderBaseItem:: ~CFdbOrderBaseItem()
{
    delete m_cursor;
    //删除所有相关的连线信息
    RemovePloyLines();
    if (m_pCommentTextItem != nullptr)
    {
        m_pCommentTextItem->deleteLater();
        m_pCommentTextItem = nullptr;
    }
}

/*! \fn CFdbOrderBaseItem:SetIndexNumber(int nIndex)
*********************************************************************************************************
** \brief SetIndexNumber(int nIndex)
** \details 更新执行编号
** \param  nIndex执行编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetIndexNumber(int nIndex)
{
    m_nIndexNumber = nIndex;
    update();
}

int CFdbOrderBaseItem::GetInputIndex()
{
    return m_nInputIndex;
}

int CFdbOrderBaseItem::GetOutputIndex()
{
    return m_nOutputIndex;
}

/*! \fn CFdbOrderBaseItem:GetOutPutList()
*********************************************************************************************************
** \brief GetOutPutList()
** \details 获取输出信息
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QMap<int, QList<CFdbOrderBaseItem*> > CFdbOrderBaseItem::GetOutPutList()
{
    //每个输出点只能有一个输入
    return m_mapNextInfos;
}

/*! \fn CFdbOrderBaseItem:GetInputItems()
*********************************************************************************************************
** \brief GetInputItems()
** \details 获取输入信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QMap<int, CFdbOrderBaseItem*> CFdbOrderBaseItem::GetInputItems()
{
    //每个输入点 可以 给多个输出点
    return m_mapPreInfos;
}

/*! \fn CFdbOrderBaseItem:GetInputItems()
*********************************************************************************************************
** \brief GetInputNums()
** \details 获取输入端子个数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::GetInputNums()
{
    return m_mapPreInfos.count();
}

int CFdbOrderBaseItem::GetOutputNums()
{
    return m_mapNextInfos.count();
}

/*! \fn CFdbOrderBaseItem:InsertInputItem(int nIndex, CFdbOrderBaseItem* mapItem)
*********************************************************************************************************
** \brief InsertInputItem(int nIndex, CFdbOrderBaseItem* mapItem)
** \details 增加输入端子信息
** \param nIndex:端子编号  mapItem:连线对端item
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::InsertInputItem(int nIndex, CFdbOrderBaseItem* mapItem)
{
    Q_ASSERT(mapItem);
    if (mapItem == nullptr)
    {
        return;
    }

    m_mapPreInfos.insert(nIndex, mapItem);
}

/*! \fn CFdbOrderBaseItem:InsertOuputItem(int nIndex, CFdbOrderBaseItem* mapItem)
*********************************************************************************************************
** \brief InsertOuputItem(int nIndex, CFdbOrderBaseItem* mapItem)
** \details 增加输出端子信息
** \param nIndex:端子编号  mapItem:连线对端item
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::InsertOuputItem(int nIndex, CFdbOrderBaseItem* pItem)
{
    //m_mapNextInfos.insert(nIndex, pItem);

    m_mapNextInfos[nIndex].append(pItem);
}

/*! \fn CFdbOrderBaseItem:DeleteInputItem(CFdbOrderBaseItem* pItem)
*********************************************************************************************************
** \brief DeleteInputItem(CFdbOrderBaseItem* pItem)
** \details 删除item之间关系
** \param pItem:连线对端item
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::DeleteInputItem(CFdbOrderBaseItem* pItem)
{
    Q_ASSERT(pItem);

    if (pItem != nullptr)
    {
        QMap<int, CFdbOrderBaseItem*>::const_iterator it = m_mapPreInfos.constBegin();
        QList<int> nKeys;

        while (it != m_mapPreInfos.constEnd())
        {
            if (it.value() == pItem)
            {
                nKeys.append(it.key());
            }

            ++it;
        }
        //删除对应关系
        for (int i = 0; i < nKeys.count(); i++)
        {
            m_mapPreInfos.remove(nKeys.at(i));
        }

    }
}


/*! \fn CFdbOrderBaseItem:DeleteOutputItem(CFdbOrderBaseItem* pItem)
*********************************************************************************************************
** \brief DeleteOutputItem(CFdbOrderBaseItem* pItem)
** \details 删除item之间关系
** \param pItem:连线对端item
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::DeleteOutputItem(CFdbOrderBaseItem* pItem)
{
//     m_mapNextInfos.removeAll(pItem);
    Q_ASSERT(pItem);
    if (pItem != nullptr)
    {
        QMap<int, QList<CFdbOrderBaseItem*> >::const_iterator it = m_mapNextInfos.constBegin();

        QList<int> nKeys;

        while (it != m_mapNextInfos.constEnd())
        {
            if (it.value().contains(pItem))
            {
                m_mapNextInfos[it.key()].removeAll(pItem);
            }

            if (it.value().count() == 0)
            {
                nKeys.append(it.key());
            }

            ++it;
        }
        //删除item之间关系
        for (int i = 0; i < nKeys.count(); i++)
        {
            m_mapNextInfos.remove(nKeys.at(i));
        }
    }
}

/*! \fn CFdbOrderBaseItem:DeleteOneOutputItem(CFdbOrderBaseItem* pItem, int nIndex)
*********************************************************************************************************
** \brief DeleteOneOutputItem(CFdbOrderBaseItem* pItem, int nIndex)
** \details 删除指定端子的item的关联
** \param pItem: 连线对端item nIndex:端子编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::DeleteOneOutputItem(CFdbOrderBaseItem* pItem, int nIndex)
{
    Q_UNUSED(pItem);
    //m_mapNextInfos.remove(nIndex);
    m_mapNextInfos[nIndex].removeAll(pItem);

    if (!m_mapNextInfos[nIndex].isEmpty())
    {
        m_mapNextInfos.remove(nIndex);
    }
}

void CFdbOrderBaseItem::DeleteOneInputItem(CFdbOrderBaseItem* pItem, int nIndex)
{
    Q_UNUSED(pItem);
    m_mapPreInfos.remove(nIndex);
}

/*! \fn CFdbOrderBaseItem:SetItemType(int nItemType)
*********************************************************************************************************
** \brief SetItemType(int nItemType)
** \details 设置逻辑块类型
** \param nItemType:类型编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetItemType(int nItemType)
{
    m_nItemTypeNum = nItemType;
    m_strItemType = AnalyseItemType(nItemType);
}

int CFdbOrderBaseItem::GetItemTypNum()
{
    return m_nItemTypeNum;
}

/*! \fn CFdbOrderBaseItem:SetCreateNum(int nCreateNum)
*********************************************************************************************************
** \brief SetCreateNum(int nCreateNum)
** \details 设置创建编号
** \param nCreateNum:创建编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetCreateNum(int nCreateNum)
{
    m_nCreateNum = nCreateNum;
    //初始化创建编号和执行编号一致
    m_nIndexNumber = nCreateNum;
}

const QRectF& CFdbOrderBaseItem::GetItemRect()
{
    return m_rect;
}

int CFdbOrderBaseItem::GetCreateNum()
{
    return m_nCreateNum;
}

/*! \fn CFdbOrderBaseItem:GetIndexNum()
*********************************************************************************************************
** \brief GetIndexNum()
** \details 获取执行编号
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::GetIndexNum()
{
    return m_nIndexNumber;
}

const QColor & CFdbOrderBaseItem::GetItemColor()
{
    return m_ItemColor;
}

const QString& CFdbOrderBaseItem::GetItemType()
{
    return m_strItemType;
}

void CFdbOrderBaseItem::SetMaxInputHeight(int nMaxHeight)
{
    m_nInsertMaxHeight = nMaxHeight;
}

/*! \fn CFdbOrderBaseItem:GetInputLines(int nInputIndex)
*********************************************************************************************************
** \brief GetInputLines(int nInputIndex)
** \details 获取某个输入端子的连接线的点集
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QList<QPointF> CFdbOrderBaseItem::GetInputLines(int nInputIndex)
{
    QList<QPointF> linepoints;
    //遍历所有逻辑线
    foreach(CConnectionRouter* lineItem,m_polyLines)
    {
        if (lineItem->StartItem() != this)
        {
            //找到对应端子下的连接线
            if (lineItem->GetEndInput() == nInputIndex)
            {
                //返回该连接线的点号
                return lineItem->GetAllPoints();
            }
        }
    }

    return linepoints;
}

/*! \fn CFdbOrderBaseItem:GetSceneRectf()
*********************************************************************************************************
** \brief GetSceneRectf()
** \details 获取item相对应与scene的坐标位置
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QRectF CFdbOrderBaseItem::GetSceneRectf()
{
    QRectF sceneRect(this->scenePos().x(),this->scenePos().y(),m_rect.width(),m_rect.height());
    return sceneRect;
}

void CFdbOrderBaseItem::SetItemStatus(int nStatus)
{
    m_nItemStatus = nStatus;
}

/*! \fn CFdbOrderBaseItem:SetInputNum()
*********************************************************************************************************
** \brief SetInputNum()
** \details 更新输入端子数量
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInputNum()
{
    m_nInputNums = m_mapPreInfos.count();
}

/*! \fn CFdbOrderBaseItem:MinusInputNum()
*********************************************************************************************************
** \brief MinusInputNum()
** \details 入度
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::MinusInputNum()
{
    return --m_nInputNums;
}

/*! \fn CFdbOrderBaseItem:keyPressEvent(QKeyEvent *event)
*********************************************************************************************************
** \brief keyPressEvent(QKeyEvent *event)
** \details 键盘控制
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::keyPressEvent(QKeyEvent *event)
{
    m_pfKeyPoint = this->scenePos();
    switch (event->key())
    {
    case Qt::Key_Left:
    {
        //向左移动  加ctrl 时 移动速度加快
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(0, -5);
        }
        else
        {
            MoveItemFromKey(0, -1);
        }
        //更新模型数据
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }

        break;
    }
    case Qt::Key_Right:
    {
        //向右移动  加ctrl 时 移动速度加快
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(0, 5);
        }
        else
        {
            MoveItemFromKey(0, 1);
        }

        //更新数据模型
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    case Qt::Key_Up:
    {
        //向上移动  加ctrl 时 移动速度加快
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(-5, 0);
        }
        else
        {
            MoveItemFromKey(-1, 0);
        }

        //更新数据模型
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    case Qt::Key_Down:
    {
        //向下移动  加ctrl 时 移动速度加快
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(5, 0);
        }
        else
        {
            MoveItemFromKey(1, 0);
        }

        //更新数据模型
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    default:
        break;
    }
    QGraphicsItem::keyPressEvent(event);
}

/*! \fn CFdbOrderBaseItem:paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
*********************************************************************************************************
** \brief paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
** \details 重绘
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //小选中框
    AddHoverRect(painter);

    //选中状态改变样式
    if (option->state & QStyle::State_Selected)
    {
        //设置选中框
        painter->setPen(QPen(QColor(192, 0, 0), 1, Qt::SolidLine));
        painter->drawRect(this->GetItemRect());
        painter->setPen(QPen(Qt::black, 0));
    }
    
    //绘制编号
    QString strTitle = "F" + QString::number(GetCreateNum()) + "_(" + QString::number(GetIndexNum()) + ")";
    painter->drawText(QRect(38, 0, 100, 15), strTitle);

    //设置选中颜色 加深
    QColor fillColor = (option->state & QStyle::State_Selected) ? GetItemColor().dark(150) : GetItemColor();

    //设置点击颜色  再加深
    if (option->state & QStyle::State_MouseOver)
    {
        fillColor = fillColor.dark(100);
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
    {
        width += 2;
    }
    //QRectF(0, 0, 110, 330)
    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->setPen(QPen(m_colorStatus, 0));
    painter->drawRect(QRect(GetItemRect().x() + 14, GetItemRect().y() + 14, GetItemRect().width() - 31, GetItemRect().height() - 20));
    painter->setPen(QPen(Qt::black, 0));

    painter->setBrush(b);

    if (GetEnoFlag())
    {
        //如果存在eno信息  则显示
        painter->setPen(QPen(m_colorStatus, 0));
        painter->drawLine(QLine(5, GetItemRect().y() + 45 - 18, 13, GetItemRect().y() + 45 - 18));
        painter->setPen(QPen(Qt::black, 0));
        painter->drawText(QRect(20, GetItemRect().y() + 20, 30, 10), tr("EN"));

        painter->setPen(QPen(m_colorStatus, 0));
        painter->drawLine(QLine(94, GetItemRect().y() + 45 - 18, 102, GetItemRect().y() + 45 - 18));
        painter->setPen(QPen(Qt::black, 0));
        painter->drawText(QRect(70, GetItemRect().y() + 20, 30, 10), tr("ENO"));
    }

    // Draw lines
    QVarLengthArray<QLineF, 36> lines;
    m_nCurrentInputNums = 0;
    m_nCurrentOutputNums = 0;

    for (int i = 0; i < m_nInputMaxNum; i++)
    {
        //只显示当前高度下能显示的端子
        if (GetItemRect().y() + 40 + i * 18 > GetItemRect().height() - 20)
        {
            break;
        }

        m_nCurrentInputNums = i + 1;
        //存放端子位置信息
        lines.append(QLineF(5, GetItemRect().y() + 45 + i * 18, 13, GetItemRect().y() + 45 + i * 18));
    }

    for (int j = 0; j < m_nOutMaxNum; j++)
    {
        //只显示当前高度下能显示的输出端子
        if (GetItemRect().y() + 40 + j * 18 > GetItemRect().height() - 20)
        {
            break;
        }
        m_nCurrentOutputNums = j + 1;
        //存放端子位置信息
        lines.append(QLineF(94, GetItemRect().y() + 45 + j * 18, 102, GetItemRect().y() + 45 + j * 18));
    }

    painter->setPen(QPen(m_colorStatus, 0));
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(QPen(Qt::black, 0));

    //填写内容
    for (int i = 0; i < m_nInputMaxNum; i++)
    {
        if (GetItemRect().y() + 40 + i * 18 > GetItemRect().height() - 20)
        {
            //超出显示范围内不显示
            break;
        }

        this->SetMaxInputHeight(GetItemRect().y() + 40 + i * 18 + 8);
        //填写内容
        QString strIn = m_lstInputPin.at(i);
        painter->drawText(QRect(20, GetItemRect().y() + 40 + i * 18, 30, 10), strIn);
    }

    painter->drawText(QRect(46, GetItemRect().y() + 15, 36, 10), GetItemType());

    //填写输出显示内容
    for (int m = 0; m < m_nOutMaxNum; m++)
    {
        if (GetItemRect().y() + 40 + m * 18 > GetItemRect().height() - 20)
        {
            break;
        }

        m_nOutMaxHeight = GetItemRect().y() + 40 + m * 18 + 8;

        QString strOut = m_lstOutputPin.at(m);
        painter->drawText(QRect(70, GetItemRect().y() + 40 + m * 18, 30, 10), strOut);

    }

    //item的内部注释框  显示在逻辑块 下方
    if (GetCommentItem() != nullptr)
    {
        GetCommentItem()->setPos(10, GetItemRect().height());
    }

}

/*! \fn CFdbOrderBaseItem:MoveItemFromKey(double disHeight, double disWidth)
*********************************************************************************************************
** \brief MoveItemFromKey(double disHeight, double disWidth)
** \details 键盘控制 移动逻辑块
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::MoveItemFromKey(double disHeight, double disWidth)
{
    //移动的终点处item重叠的个数
    QList<QGraphicsItem*> lstGraphicsItem = this->collidingItems();

    if (!lstGraphicsItem.isEmpty())   //存在重叠的情况
    {
        //遍历所有重叠的item
        for (int i = 0; i < lstGraphicsItem.count(); i++)
        {
            //找出其中为逻辑块的item
            CFdbOrderBaseItem * pItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(lstGraphicsItem.at(i));

            if (pItem != nullptr && pItem->type() == OBJEC_TITEM)//判断是否为逻辑块
            {
                //移动的距离存在多种大小,先找出单位移动大小
                if (fabs(disHeight) > 1)
                {
                    disHeight = disHeight / fabs(disHeight);
                }

                if (fabs(disWidth) > 1)
                {
                    disWidth = disWidth / fabs(disWidth);
                }

                //当撞到item时 将会反弹回去  反弹大小为 28
                this->setPos(QPointF(m_pfKeyPoint.x() - disWidth*28, m_pfKeyPoint.y() - disHeight*28));

                //当item移动时,需要同时更新逻辑线的位置
                foreach(CConnectionRouter *plyLine, m_polyLines)
                {
                    plyLine->UpdatePosition();
                }

                m_nInputIndex = 0;
                m_nOutputIndex = 0;
                m_bScanl = false;

                return;
            }

        }

    }

    //当不存在障碍物时 正常移动
    this->setPos(QPointF(m_pfKeyPoint.x() + disWidth, m_pfKeyPoint.y() + disHeight));

    //更新逻辑线
    foreach(CConnectionRouter *plyLine, m_polyLines)
    {
        plyLine->UpdatePosition();
    }

    m_nInputIndex = 0;
    m_nOutputIndex = 0;
    m_bScanl = false;

    update();
}

/*! \fn CFdbOrderBaseItem:GetEnoFlag()
*********************************************************************************************************
** \brief GetEnoFlag()
** \details 获取eno是否显示标识
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
bool CFdbOrderBaseItem::GetEnoFlag()
{
    return m_bEnoFlag;
}

/*! \fn CFdbOrderBaseItem:GetCommentData()
*********************************************************************************************************
** \brief GetCommentData()
** \details 获取逻辑块中的注释内容
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
const QString& CFdbOrderBaseItem::GetCommentData()
{
    return m_strComment;
}

void CFdbOrderBaseItem::SetComment(const QString &strComment)
{
    m_strComment = strComment;
}

/*! \fn CFdbOrderBaseItem:Slot_UpdateComment(const QString &strComment)
*********************************************************************************************************
** \brief Slot_UpdateComment(const QString &strComment)
** \details 更新逻辑下方的注释
** \param strComment:注释内容
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::Slot_UpdateComment(const QString &strComment)
{
    if (strComment.isEmpty())
    {
        //当注释内容为空时  删除注释item
        if (m_pCommentTextItem != nullptr)
        {
            m_pCommentTextItem->deleteLater();
            m_pCommentTextItem = nullptr;
        }

        m_strComment.clear();
        return;
    }

    if (m_pCommentTextItem == nullptr)
    {
        m_pCommentTextItem = new QGraphicsTextItem(this);
    }
    
    //注释item  显示在逻辑块下方
    m_pCommentTextItem->setPlainText(strComment);
    m_pCommentTextItem->setDefaultTextColor(QColor(0, 128, 0));
    m_pCommentTextItem->setTextWidth(90);
    m_pCommentTextItem->setPos(10, GetItemRect().height());

    m_strComment = strComment;
}

/*! \fn CFdbOrderBaseItem:Slot_UpdateEnoInfo(int nFlag)
*********************************************************************************************************
** \brief Slot_UpdateEnoInfo(int nFlag)
** \details 更新eno标识
** \param strComment:注释内容
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::Slot_UpdateEnoInfo(int nFlag)
{
    if (nFlag == 0)
    {
        //关闭
        CloseEnoItems();
    }
    else if (nFlag == 1)
    {
        //开启
        ShowEnoItems();
    }

    //更新模型数据
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
    
}

void CFdbOrderBaseItem::Slot_updateBindValueInfo(QMap<int, QString>& mapBindValue)
{
	if (mapBindValue.count() == 0)
	{
		return;
	}
	QMap<int, QString>::const_iterator it = mapBindValue.constBegin();

	while (it != mapBindValue.constEnd())
	{
		if (m_mapPinIndexText.contains(it.key()))
		{
			m_mapPinIndexText[it.key()]->setPlainText(it.value());
		}
		else
		{
			m_nInputIndex = it.key();

			m_nCurrentWidth = 5;
			int nBegin = 45 - 8 - 18;
			m_nCurrentHeight = nBegin + 8 + (m_nInputIndex - 1) * 18;
			m_pfPinText = QPointF(m_nCurrentWidth, m_nCurrentHeight - 8);

			AddTextBinInfo();
			if (m_mapPinIndexText.contains(it.key()))
			{
				m_mapPinIndexText[it.key()]->setPlainText(it.value());
			}
		}
		it++;
	}

	UpdateCurrentItemInfo();
}

void CFdbOrderBaseItem::ShowEnoItems()
{
    m_bEnoFlag = true;
}

void CFdbOrderBaseItem::CloseEnoItems()
{
    m_bEnoFlag = false;
}

QGraphicsTextItem * CFdbOrderBaseItem::GetCommentItem()
{
    return m_pCommentTextItem;
}

/*! \fn CFdbOrderBaseItem:AddHoverRect(QPainter *painter)
*********************************************************************************************************
** \brief AddHoverRect(QPainter *painter)
** \details 根据位置信息 判断是否处于端子处 绘制颜色显示
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AddHoverRect(QPainter *painter)
{
    if (painter == NULL || (m_nInputIndex == 0 && m_nOutputIndex == 0))
    {
        return;
    }

    //设置选中框
    QBrush b = painter->brush();
    painter->setPen(QPen(QColor(0, 255, 0), 1, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::green));

    //起始地址 为eno所在的位置
    int nBegin = 45 - 18;

    //当index不为0的时候 需要绘制当前所在的位置
    if (m_nInputIndex != 0)
    {
        int nBgx = 5;
        int nBgy = nBegin + 18 * (m_nInputIndex-1) - 8;
        int nWidth = 8;
        int nHeight = 16;
        if ((m_nInputIndex == 1) && (GetEnoFlag() == false))
        {
            //非显示状态
        }
        else
        {
            //绘制rect
            painter->drawRect(nBgx, nBgy, nWidth, nHeight);
        }
    }

    //当index不为0的时候 需要绘制当前所在的位置
    if (m_nOutputIndex != 0)
    {
        int nBgx = 94;
        int nBgy = nBegin + 18 * (m_nOutputIndex-1) - 8;
        int nWidth = 8;
        int nHeight = 16;

        if ((m_nOutputIndex == 1) && (GetEnoFlag() == false))
        {
            //非显示状态
        }
        else
        {
            painter->drawRect(nBgx, nBgy, nWidth, nHeight);
        }
        
    }

    painter->setPen(QPen(Qt::black, 0));
    painter->setBrush(b);

    m_nInputIndex = 0;
    m_nOutputIndex = 0;
}


/*! \fn CFdbOrderBaseItem:AnalysePointPosition(const QPointF &pointCurrent)
*********************************************************************************************************
** \brief AnalysePointPosition(const QPointF &pointCurrent)
** \details 根据点判断  在哪个端子处
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AnalysePointPosition(const QPointF &pointCurrent)
{
    m_nItemStatus = 0;
    //起始端子位置
    int nBegin = 45 - 8 - 18;

    //输出
    if ((pointCurrent.x() >= 94) && (pointCurrent.x() <= 102))   //输出x范围
    {
        if ((pointCurrent.y() >= nBegin) && (pointCurrent.y() <= m_nOutMaxHeight))  //输出y范围
        {
            //解析数据判断在第几个
            if (pointCurrent.y() < nBegin + 8)
            {
                //首个单独判断
                m_nOutputIndex = 1;
            }
            else
            {
                //
                int nHeight = pointCurrent.y() - (nBegin + 8);
                //编号从1开始
                int nFloor = nHeight / 18 + 1;
                int nLength = nHeight % 18;

                if (nLength > 8 && nLength < 10)
                {
                    //中间空
                    m_nOutputIndex = 0;
                }
                else
                {
                    if (nLength <= 8)
                    {
                        //靠上
                        m_nOutputIndex = nFloor;
                    }
                    else
                    {
                        //靠下属于下一层
                        m_nOutputIndex = nFloor + 1;
                    }
                }
            }
        }
        else
        {
            m_nOutputIndex = 0;
        }
    }
    else
    {
        m_nOutputIndex = 0;
    }

    //输入
    //横向取值范围
    if ((pointCurrent.x() >= 5) && (pointCurrent.x() <= 13))
    {
        //纵向取值范围
        if ((pointCurrent.y() >= nBegin) && (pointCurrent.y() <= m_nInsertMaxHeight))
        {
            //解析数据判断在第几个
            if (pointCurrent.y() < nBegin + 8)
            {
                //首个单独判断
                m_nInputIndex = 1;
            }
            else
            {
                //
                int nHeight = pointCurrent.y() - (nBegin + 8);
                //编号从1开始
                int nFloor = nHeight / 18 + 1;
                int nLength = nHeight % 18;

                if (nLength > 8 && nLength < 10)
                {
                    //中间空
                    m_nInputIndex = 0;
                }
                else
                {
                    if (nLength <= 8)
                    {
                        //靠上
                        m_nInputIndex = nFloor;
                    }
                    else
                    {
                        //靠下属于下一层
                        m_nInputIndex = nFloor + 1;
                    }
                }
            }
        }
        else
        {
            m_nInputIndex = 0;
        }
    }
    else
    {
        m_nInputIndex = 0;
    }

    //记录输入出位置信息
    if (m_nInputIndex != 0)
    {
        //
        m_nCurrentWidth = 5;
        m_nCurrentHeight = nBegin + 8 + (m_nInputIndex-1) * 18;
        m_nItemStatus = IO_INPUT;
        m_pfPinText = QPointF(m_nCurrentWidth, m_nCurrentHeight-8);
    }

    //记录输出处位置信息
    if (m_nOutputIndex != 0)
    {
        m_nCurrentWidth = 102;
        m_nCurrentHeight = nBegin + 8 + (m_nOutputIndex-1) * 18;
        m_nItemStatus = IO_OUTPUT;
        m_pfPinText = QPointF(m_nCurrentWidth, m_nCurrentHeight - 8);
    }

}

/*! \fn CFdbOrderBaseItem:SetInPutIndex(int nInputIndex)
*********************************************************************************************************
** \brief SetInPutIndex(int nInputIndex)
** \details 设置输入端 位置信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInPutIndex(int nInputIndex)
{
    //y轴上起始
    int nBegin = 45 - 18;

    m_nCurrentWidth = 5;
    m_nCurrentHeight = nBegin + (nInputIndex-1) * 18;

    m_nInputIndex = nInputIndex;
}

/*! \fn CFdbOrderBaseItem:SetOutPutIndex(int nInputIndex)
*********************************************************************************************************
** \brief SetOutPutIndex(int nInputIndex)
** \details 设置输出端 位置信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutPutIndex(int nOutputIndex)
{
    //y轴起始
    int nBegin = 45 - 18;

    m_nCurrentWidth = 102;
    m_nCurrentHeight = nBegin + (nOutputIndex-1) * 18;
    m_nOutputIndex = nOutputIndex;
}

/*! \fn CFdbOrderBaseItem:SetInputMaxNum(int nIndex)
*********************************************************************************************************
** \brief SetInputMaxNum(int nIndex)
** \details 设置输入最大高度
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInputMaxNum(int nIndex)
{
    m_nInputMaxNum = nIndex;
    //比较输入和输出端子数大小 
    m_nItemMaxHeight = (m_nInputMaxNum > m_nOutMaxNum ? m_nInputMaxNum : m_nOutMaxNum) * 18 + 80;

    //最大高度不能超过120
    if (m_nItemMaxHeight < 120)
    {
        m_rect.setHeight(m_nItemMaxHeight);
    }
}

int CFdbOrderBaseItem::GetInputMaxNum()
{
    return m_nInputMaxNum;
}

/*! \fn CFdbOrderBaseItem:SetOutPutMaxNum(int nIndex)
*********************************************************************************************************
** \brief SetOutPutMaxNum(int nIndex)
** \details 设置输入最大高度
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutPutMaxNum(int nIndex)
{
    m_nOutMaxNum = nIndex;
    //比较输入和输出端子数大小 
    m_nItemMaxHeight = (m_nInputMaxNum > m_nOutMaxNum ? m_nInputMaxNum : m_nOutMaxNum) * 18 + 60;

    if (m_nItemMaxHeight < 120)
    {
        m_rect.setHeight(m_nItemMaxHeight);
    }
}

int CFdbOrderBaseItem::GetOutPutMaxNum()
{
    return m_nOutMaxNum;
}

/*! \fn CFdbOrderBaseItem:SetInputPinNames(const QStringList &lstNames)
*********************************************************************************************************
** \brief SetInputPinNames(const QStringList &lstNames)
** \details 设置端子名称
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInputPinNames(const QStringList &lstNames)
{
    m_lstInputPin = lstNames;
}

/*! \fn CFdbOrderBaseItem:SetOutputPinNames(const QStringList &lstNames)
*********************************************************************************************************
** \brief SetOutputPinNames(const QStringList &lstNames)
** \details 设置端子名称
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutputPinNames(const QStringList &lstNames)
{
    m_lstOutputPin = lstNames;
}

/*! \fn CFdbOrderBaseItem:InsertPins(int nEndIndex, int nBeginIndex)
*********************************************************************************************************
** \brief InsertPins(int nEndIndex, int nBeginIndex)
** \details 输入端子  和 其他输出端子  每个输入端子只能有一个输入端子
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::InsertPins(int nEndIndex, int nBeginIndex)
{
    m_mapPins.insert(nEndIndex, nBeginIndex);
}

QMap<int, int> CFdbOrderBaseItem::GetBinMapInfo()
{
    return m_mapPins;
}

void CFdbOrderBaseItem::RemoveOneInputPin(int nPinIndex)
{
    m_mapPins.remove(nPinIndex);
}

/*! \fn CFdbOrderBaseItem:AddTextBinInfo()
*********************************************************************************************************
** \brief AddTextBinInfo()
** \details 每个端子出可以手动输入内容
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AddTextBinInfo()
{
    if (m_nInputIndex != 0)
    {
        int nInputPinNum = m_nInputIndex;

        //输入
        if (m_mapPinIndexText.contains(nInputPinNum))
        {
            //可编辑状态
            m_mapPinIndexText[nInputPinNum]->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_mapPinIndexText[nInputPinNum]->setFocus();
        }
        else
        {
            //创建新文本对象
            QGraphicsTextItem *pinText = new QGraphicsTextItem(this);
            pinText->setPos(m_pfPinText.x()-200,m_pfPinText.y()-5);
            pinText->setTextWidth(200);
            pinText->document()->setDefaultTextOption(QTextOption(Qt::AlignRight));
            
            pinText->setTextInteractionFlags(Qt::TextEditorInteraction);
            pinText->setFocus();
            m_mapPinIndexText.insert(nInputPinNum, pinText);
        }
    }
    else if (m_nOutputIndex != 0)
    {
        //输出
        int nInputPinNum = m_nOutputIndex;

        if (m_mapOutPinIndexText.contains(nInputPinNum))
        {
            //已经存在的对象 可编辑
            m_mapOutPinIndexText[nInputPinNum]->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_mapOutPinIndexText[nInputPinNum]->setFocus();
        }
        else
        {
            //创建新的文本对象
            QGraphicsTextItem *pinText = new QGraphicsTextItem(this);
            pinText->setPos(m_pfPinText.x(), m_pfPinText.y() - 5);
            pinText->setTextWidth(130);
			pinText->document()->setDefaultTextOption(QTextOption(Qt::AlignLeft));

            pinText->setTextInteractionFlags(Qt::TextEditorInteraction);
            pinText->setFocus();
            m_mapOutPinIndexText.insert(nInputPinNum, pinText);
        }
    }
}

void CFdbOrderBaseItem::SetStatusColor(const QColor &statusColor)
{
    m_colorStatus = statusColor;
}

int CFdbOrderBaseItem::GetLogicValue()
{
    return 0;
}

void CFdbOrderBaseItem::AnalseBitInput()
{
    
}

/*! \fn CFdbOrderBaseItem:GetAllLines()
*********************************************************************************************************
** \brief GetAllLines()
** \details 获取所有关联的逻辑线
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QList<CConnectionRouter*> CFdbOrderBaseItem::GetAllLines()
{
    return m_polyLines;
}

/*! \fn CFdbOrderBaseItem:SetCurrentInputNum(int nInputNum)
*********************************************************************************************************
** \brief SetCurrentInputNum(int nInputNum)
** \details 设置当前输入高度
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetCurrentInputNum(int nInputNum)
{
    m_nCurrentInputNums = nInputNum;
    //设置高度
    m_rect.setHeight(45 + nInputNum * 18);
    update();
}

int CFdbOrderBaseItem::GetCurrentInpusNum()
{
    return m_nCurrentInputNums;
}

/*! \fn CFdbOrderBaseItem:UpdateCurrentItemInfo()
*********************************************************************************************************
** \brief UpdateCurrentItemInfo()
** \details 更新输入端子详细信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::UpdateCurrentItemInfo()
{
    //输入
    QStringList tLstInputs;
    tLstInputs.append("EN");
    tLstInputs.append(m_lstInputPin);

    m_vInputPros.clear();
    m_vOutputPros.clear();

    for (int i = 0; i < tLstInputs.count(); i++)
    {
        if (i < m_nCurrentInputNums + 1)
        {
            //组装端子信息
            std::shared_ptr<CBinProperty> pPinProperty = std::make_shared<CBinProperty>();
            
            pPinProperty->SetPinName(tLstInputs.at(i));

            pPinProperty->SetPinIndex(i + 1);

            if (m_mapPins.contains(i+1))
            {
                //output输入
                pPinProperty->SetPinType(1);

                if (m_mapPreInfos.contains(i+1))
                {
                    //读取端子信息
                    std::shared_ptr<CInPutProperty> tInpro = pPinProperty->CreateInModel();
                    tInpro->SetCreateNum(m_mapPreInfos[i + 1]->GetCreateNum());
                    tInpro->SetExecNum(m_mapPreInfos[i + 1]->GetIndexNum());
                    tInpro->SetOutPutIndex(m_mapPins[i + 1]);

                    QStringList tLstOutpus;
                    tLstOutpus.append("ENO");
                    tLstOutpus.append(m_mapPreInfos[i + 1]->GetOutputPins());

                    if (tLstOutpus.count() >= m_mapPins[i + 1])
                    {
                        tInpro->SetOutputPinName(tLstOutpus[m_mapPins[i + 1] - 1]);
                    }
                    
                }
            }
            else
            {
                
                if (m_mapPinIndexText.contains(i + 1))
                {
                    //元件输入
                    pPinProperty->SetPinType(2);
                    std::shared_ptr<CCellProperty> tCellpro = pPinProperty->CreateCellPro();
                    tCellpro->SetCellName(m_mapPinIndexText[i + 1]->toPlainText());

                }
                else
                {
                    //空 没有输入
					pPinProperty->SetPinType(3);
                }
            }
            
            m_vInputPros.emplace_back(pPinProperty);
        }
    }

    //输出
    QStringList tLstoutPuts;
    tLstoutPuts.append("ENO");
    tLstoutPuts.append(m_lstOutputPin);

    for (int i = 0; i < tLstoutPuts.count(); i++)
    {
        if (i < m_nCurrentOutputNums + 1)
        {
            //读取端子信息
            std::shared_ptr<CBinProperty> pPinOutProperty = std::make_shared<CBinProperty>();

            pPinOutProperty->SetPinName(tLstoutPuts.at(i));
            pPinOutProperty->SetPinIndex(i + 1);

            if (m_mapOutPinIndexText.contains(i + 1))
            {
                //元件输入
                pPinOutProperty->SetPinType(2);
                pPinOutProperty->CreateCellPro()->SetCellName(m_mapOutPinIndexText[i + 1]->toPlainText());

            }
            else
            {
                //空 没有输入
                pPinOutProperty->SetPinType(3);
            }

            m_vOutputPros.emplace_back(pPinOutProperty);
        }
    }
}

/*! \fn CFdbOrderBaseItem:GetOutputPins()
*********************************************************************************************************
** \brief GetOutputPins()
** \details 获取所有输出端子名称
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
const QStringList & CFdbOrderBaseItem::GetOutputPins()
{
    return m_lstOutputPin;
}

void CFdbOrderBaseItem::SetEnData(int nEnData)
{
    m_nEnFlag = nEnData;
}

int CFdbOrderBaseItem::GetEnData()
{
    return m_nEnFlag;
}

void CFdbOrderBaseItem::SetEnoData(int nEnoData)
{
    m_nEnoFlag = nEnoData;
}

int CFdbOrderBaseItem::GetEnoData()
{
    return m_nEnoFlag;
}

/*! \fn CFdbOrderBaseItem:SetOutputData(int nOutPinNum, int nValue)
*********************************************************************************************************
** \brief SetOutputData(int nOutPinNum, int nValue)
** \details 输出端子  和对应的编号
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutputData(int nOutPinNum, int nValue)
{
    m_mapOutput[nOutPinNum] = nValue;
}

QMap<int, int> CFdbOrderBaseItem::GetOutputData()
{
    return m_mapOutput;
}

/*! \fn CFdbOrderBaseItem:GetInputPros()
*********************************************************************************************************
** \brief GetInputPros()
** \details 获取输入端子信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
const std::vector<std::shared_ptr<CBinProperty>> CFdbOrderBaseItem::GetInputPros()
{
    return m_vInputPros;
}

/*! \fn CFdbOrderBaseItem:GetOutPutPros()
*********************************************************************************************************
** \brief GetOutPutPros()
** \details 获取输出端子信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
const std::vector<std::shared_ptr<CBinProperty>> CFdbOrderBaseItem::GetOutPutPros()
{
    return m_vOutputPros;
}
