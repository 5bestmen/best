#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "fdborderbaseitem.h"
#include "fdborderview.h"
#include "connectionrouter.h"
#include "fdborderscene.h"

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

   // m_colorStatus = QColor(255, 0, 0);

    m_rect = QRectF(0, 0, 110, 120);
    m_cursor = new QCursor;
    
    m_ItemColor = QColor(255, 224, 192);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setAcceptHoverEvents(true);
    this->setZValue(3);
    //test
}

QString  CFdbOrderBaseItem::AnalyseItemType(int nType)
{
    return QString::number(nType);
}


void CFdbOrderBaseItem::ResetRect(QRectF rect)
{
    m_rect = rect;
    update(boundingRect());
}

QRectF CFdbOrderBaseItem::boundingRect() const
{
    return m_rect;
}

QPainterPath CFdbOrderBaseItem::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(-28, -28, m_rect.width() + 42, m_rect.height() + 42));
    return path;
}

void CFdbOrderBaseItem::RemovePloyLine(CConnectionRouter *ployline)
{
    //去除线
    int index = m_polyLines.indexOf(ployline);

    if (index != -1)
    {
        m_polyLines.removeAt(index);
    }
}

void CFdbOrderBaseItem::RemovePloyLines()
{   
    foreach(CConnectionRouter *ployline, m_polyLines)
    {
        ployline->StartItem()->RemovePloyLine(ployline);
        ployline->EndItem()->RemovePloyLine(ployline);

        delete ployline;
    }
}

void CFdbOrderBaseItem::AddPloyLine(CConnectionRouter *ployline)
{
    m_polyLines.append(ployline);
}

int CFdbOrderBaseItem::GetItemIOStatus()
{
    return m_nItemStatus;
}

void CFdbOrderBaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            if (pScene->GetMode() != CURSORLINE)
            {
                AddTextBinInfo();
            }
           // else
            //{
                m_nMoveBge = event->scenePos();
                m_pfClickPoint = this->pos();
                m_bScanl = true;
            //}
        }

    }
    else if (event->button() == Qt::RightButton)
    {
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

void CFdbOrderBaseItem::Slot_DeleteAct()
{
    //删除链表前后关系
    QMap<int, QList<CFdbOrderBaseItem*> >::const_iterator itBeg = m_mapNextInfos.constBegin();

    while (itBeg != m_mapNextInfos.constEnd())
    {
        for each (CFdbOrderBaseItem* item in itBeg.value())
        {
            item->DeleteInputItem(this);
        }

        itBeg++;

//         if (itBeg.value() != nullptr)
//         {
//             itBeg.value()->DeleteInputItem(this);
//             
//         }
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = m_mapPreInfos.constBegin();

    while (it != m_mapPreInfos.constEnd())
    {
        it.value()->DeleteOutputItem(this);
        ++it;
    }
    //
    this->deleteLater();
    RemovePloyLines();
    //更新编号
    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(this->scene()->views().at(0));
    if (pView != nullptr)
    {
        pView->DeleteRemoveItem(this);
        pView->UpdateItemsExecNum();
    }
}

void CFdbOrderBaseItem::AnalyseSencePoint(const QPointF &pScene)
{
    AnalysePointPosition(mapFromScene(pScene));
}

void CFdbOrderBaseItem::GetLinePoint(const QPointF &pItem)
{
    AnalysePointPosition(pItem);
}

int CFdbOrderBaseItem::GetPointWidth() const
{
    return m_nCurrentWidth;
}

int CFdbOrderBaseItem::getPointHeight() const
{
    return m_nCurrentHeight;
}

void CFdbOrderBaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bScanl)
    {
        QPointF dis;
        m_nMoveEnd = event->scenePos();
        dis = m_nMoveEnd - m_nMoveBge;
        m_nMoveBge = m_nMoveEnd;

        if (m_nChangeSize == 1)
        {
            QRectF tem = QRectF(0, m_rect.y(), m_rect.width(), m_rect.height() + dis.y());
            //item最大值  最小值判断
            if (m_rect.height() + dis.y() < m_nItemMaxHeight && m_rect.height() + dis.y() > 80)
            {
                int nMaxInIndex = 0;

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

                    this->moveBy(0, -dis.y() / 2);
                }
            }
        }
    }

    m_bChangeFlag = true;

    update();

    QGraphicsItem::mouseMoveEvent(event);
}

void CFdbOrderBaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<QGraphicsItem*> lstGraphicsItem = this->collidingItems();

    if (!lstGraphicsItem.isEmpty())
    {
        for (int i = 0; i < lstGraphicsItem.count(); i++)
        {
            CFdbOrderBaseItem * pItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(lstGraphicsItem.at(i));
            if (pItem != nullptr && pItem->type() == OBJECTITEM)
            {
                this->setPos(m_pfClickPoint);
            }
        }
    }

    if (m_bChangeFlag)
    {
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
    //更新
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

void CFdbOrderBaseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene == nullptr)
    {
        return;
    }
    if (pScene->GetMode() == CURSORLINE)
    {
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

    AnalysePointPosition(pointCurrent);

    QGraphicsItem::hoverMoveEvent(event);
    update();
}

void CFdbOrderBaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_nOutputIndex = 0;
    m_nInputIndex = 0;

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


CFdbOrderBaseItem:: ~CFdbOrderBaseItem()
{
    delete m_cursor;
    RemovePloyLines();
}

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

QMap<int, QList<CFdbOrderBaseItem*> > CFdbOrderBaseItem::GetOutPutList()
{
    return m_mapNextInfos;
}

QMap<int, CFdbOrderBaseItem*> CFdbOrderBaseItem::GetInputItems()
{
    return m_mapPreInfos;
}

int CFdbOrderBaseItem::GetInputNums()
{
    return m_mapPreInfos.count();
}

int CFdbOrderBaseItem::GetOutputNums()
{
    return m_mapNextInfos.count();
}

void CFdbOrderBaseItem::InsertInputItem(int nIndex, CFdbOrderBaseItem* mapItem)
{
    m_mapPreInfos.insert(nIndex, mapItem);
}

void CFdbOrderBaseItem::InsertOuputItem(int nIndex, CFdbOrderBaseItem* pItem)
{
    //m_mapNextInfos.insert(nIndex, pItem);

    m_mapNextInfos[nIndex].append(pItem);
}

void CFdbOrderBaseItem::DeleteInputItem(CFdbOrderBaseItem* pItem)
{
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

        for (int i = 0; i < nKeys.count(); i++)
        {
            m_mapPreInfos.remove(nKeys.at(i));
        }

    }
}



void CFdbOrderBaseItem::DeleteOutputItem(CFdbOrderBaseItem* pItem)
{
//     m_mapNextInfos.removeAll(pItem);
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

        for (int i = 0; i < nKeys.count(); i++)
        {
            m_mapNextInfos.remove(nKeys.at(i));
        }
    }
}

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

void CFdbOrderBaseItem::SetItemType(int nItemType)
{
    m_nItemTypeNum = nItemType;
    m_strItemType = AnalyseItemType(nItemType);
}

int CFdbOrderBaseItem::GetItemTypNum()
{
    return m_nItemTypeNum;
}

void CFdbOrderBaseItem::SetCreateNum(int nCreateNum)
{
    m_nCreateNum = nCreateNum;
    //初始化创建编号和执行编号一致
    m_nIndexNumber = nCreateNum;
}

QRectF CFdbOrderBaseItem::GetItemRect()
{
    return m_rect;
}

int CFdbOrderBaseItem::GetCreateNum()
{
    return m_nCreateNum;
}

int CFdbOrderBaseItem::GetIndexNum()
{
    return m_nIndexNumber;
}

QColor CFdbOrderBaseItem::GetItemColor()
{
    return m_ItemColor;
}

QString CFdbOrderBaseItem::GetItemType()
{
    return m_strItemType;
}

void CFdbOrderBaseItem::SetMaxInputHeight(int nMaxHeight)
{
    m_nInsertMaxHeight = nMaxHeight;
}

QList<QPointF> CFdbOrderBaseItem::GetInputLines(int nInputIndex)
{
    QList<QPointF> linepoints;

    foreach(CConnectionRouter* lineItem,m_polyLines)
    {
        if (lineItem->StartItem() != this)
        {
            if (lineItem->GetEndInput() == nInputIndex)
            {
                return lineItem->GetAllPoints();
            }
        }
    }

    return linepoints;
}

QRectF CFdbOrderBaseItem::GetSceneRectf()
{
    QRectF sceneRect(this->scenePos().x(),this->scenePos().y(),m_rect.width(),m_rect.height());
    return sceneRect;
}

void CFdbOrderBaseItem::SetItemStatus(int nStatus)
{
    m_nItemStatus = nStatus;
}

void CFdbOrderBaseItem::SetInputNum()
{
    m_nInputNums = m_mapPreInfos.count();
}

int CFdbOrderBaseItem::MinusInputNum()
{
    return --m_nInputNums;
}

void CFdbOrderBaseItem::keyPressEvent(QKeyEvent *event)
{
    m_pfKeyPoint = this->scenePos();
    switch (event->key())
    {
    case Qt::Key_Left:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(0, -5);
        }
        else
        {
            MoveItemFromKey(0, -1);
        }
        //更新
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }

        break;
    }
    case Qt::Key_Right:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(0, 5);
        }
        else
        {
            MoveItemFromKey(0, 1);
        }

        //更新
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    case Qt::Key_Up:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(-5, 0);
        }
        else
        {
            MoveItemFromKey(-1, 0);
        }

        //更新
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    case Qt::Key_Down:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(5, 0);
        }
        else
        {
            MoveItemFromKey(1, 0);
        }

        //更新
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

void CFdbOrderBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //小选中框
    AddHoverRect(painter);
    if (GetEnoFlag())
    {
        AddEnoItems(painter);
    }
    if (option->state & QStyle::State_Selected)
    {
        //设置选中框
        painter->setPen(QPen(QColor(192, 0, 0), 1, Qt::SolidLine));
        painter->drawRect(this->GetItemRect());
        painter->setPen(QPen(Qt::black, 0));
    }

    
    //绘制编号
    //暂时
    QString strTitle = "F" + QString::number(GetCreateNum()) + "_(" + QString::number(GetIndexNum()) + ")";
    painter->drawText(QRect(38, 0, 100, 15), strTitle);

    //设置选中颜色
    QColor fillColor = (option->state & QStyle::State_Selected) ? GetItemColor().dark(150) : GetItemColor();

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
        if (GetItemRect().y() + 40 + i * 18 > GetItemRect().height() - 20)
        {
            break;
        }

//         if (i + 1 > GetInputMaxNum())
//         {
//             break;
//         }

        //test
        //         if (GetAllInputPro().count()<i+1)
        //         {
        //             LOGICITEMPRO tPro;
        //             tPro.nIndex = 1;
        //             m_lstInputPro.append(tPro);
        // 
        //         }
        //end
        m_nCurrentInputNums = i + 1;
        lines.append(QLineF(5, GetItemRect().y() + 45 + i * 18, 13, GetItemRect().y() + 45 + i * 18));
    }

    for (int j = 0; j < m_nOutMaxNum; j++)
    {
        if (GetItemRect().y() + 40 + j * 18 > GetItemRect().height() - 20)
        {
            break;
        }
        m_nCurrentOutputNums = j + 1;

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
            break;
        }

        this->SetMaxInputHeight(GetItemRect().y() + 40 + i * 18 + 8);

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

    //painter->drawText(QRect(75, GetItemRect().y() + 40, 20, 10), QObject::tr("Out"));

    if (GetCommentItem() != nullptr)
    {
        GetCommentItem()->setPos(10, GetItemRect().height());
    }

}

void CFdbOrderBaseItem::MoveItemFromKey(double disHeight, double disWidth)
{
    QList<QGraphicsItem*> lstGraphicsItem = this->collidingItems();

    if (!lstGraphicsItem.isEmpty())
    {

        for (int i = 0; i < lstGraphicsItem.count(); i++)
        {
            CFdbOrderBaseItem * pItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(lstGraphicsItem.at(i));
            if (pItem != nullptr && pItem->type() == OBJECTITEM)
            {
                if (fabs(disHeight) > 1)
                {
                    disHeight = disHeight / fabs(disHeight);
                }

                if (fabs(disWidth) > 1)
                {
                    disWidth = disWidth / fabs(disWidth);
                }
                this->setPos(QPointF(m_pfKeyPoint.x() - disWidth*28, m_pfKeyPoint.y() - disHeight*28));

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


    this->setPos(QPointF(m_pfKeyPoint.x() + disWidth, m_pfKeyPoint.y() + disHeight));


    foreach(CConnectionRouter *plyLine, m_polyLines)
    {
        plyLine->UpdatePosition();
    }

    m_nInputIndex = 0;
    m_nOutputIndex = 0;
    m_bScanl = false;

    update();
}

bool CFdbOrderBaseItem::GetEnoFlag()
{
    return m_bEnoFlag;
}

QString CFdbOrderBaseItem::GetCommentData()
{
    return m_strComment;
}

void CFdbOrderBaseItem::Slot_UpdateComment(const QString &strComment)
{
    if (strComment.isEmpty())
    {
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
    
    m_pCommentTextItem->setPlainText(strComment);
    m_pCommentTextItem->setDefaultTextColor(QColor(0, 128, 0));
    m_pCommentTextItem->setTextWidth(90);
    m_pCommentTextItem->setPos(10, GetItemRect().height());
//     m_pCommentTextItem->setTextInteractionFlags(Qt::TextEditorInteraction);
//     m_pCommentTextItem->setFocus();

    m_strComment = strComment;
}

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
}

void CFdbOrderBaseItem::ShowEnoItems()
{
    m_bEnoFlag = true;
}

void CFdbOrderBaseItem::CloseEnoItems()
{
    m_bEnoFlag = false;
}

void CFdbOrderBaseItem::AddEnoItems(QPainter *painter)
{
    //绘制
}

QGraphicsTextItem * CFdbOrderBaseItem::GetCommentItem()
{
    return m_pCommentTextItem;
}

QList<LOGICITEMPRO> CFdbOrderBaseItem::GetAllInputPro()
{
    return m_mapInputPro.values();
}

QList<LOGICITEMPRO> CFdbOrderBaseItem::GetOutputPro()
{
    return m_mapOutPutPro.values();
}

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

    int nBegin = 45 - 18;
//     if (GetEnoFlag() == true)
//     {
//         nBegin = 45 - 18;
//     }
//     else
//     {
//         nBegin = 45;
//     }

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
            painter->drawRect(nBgx, nBgy, nWidth, nHeight);
        }
    }

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
}



void CFdbOrderBaseItem::AnalysePointPosition(const QPointF &pointCurrent)
{
    m_nItemStatus = 0;

    int nBegin = 45 - 8 - 18;
//    int nEnd = 45 + 8 - 18;
//     if (GetEnoFlag() == true)
//     {
//         nBegin = 45 - 8 - 18;
//         nEnd = 45 + 8 - 18;
//     }
//     else
//     {
//         nBegin = 45 - 8;
//         nEnd = 45 + 8;
//     }
    //输出
    if ((pointCurrent.x() >= 94) && (pointCurrent.x() <= 102))
    {
        if ((pointCurrent.y() >= nBegin) && (pointCurrent.y() <= m_nOutMaxHeight))
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

//     if (m_nOutputIndex > 2)
//     {
//         m_nOutputIndex = 0;
//     }

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

    if (m_nInputIndex != 0)
    {
        //
        m_nCurrentWidth = 5;
        m_nCurrentHeight = nBegin + 8 + (m_nInputIndex-1) * 18;
        m_nItemStatus = IOINPUT;
        m_pfPinText = QPointF(m_nCurrentWidth, m_nCurrentHeight-8);
    }

    if (m_nOutputIndex != 0)
    {
        m_nCurrentWidth = 102;
        m_nCurrentHeight = nBegin + 8 + (m_nOutputIndex-1) * 18;
        m_nItemStatus = IOOUTPUT;
        m_pfPinText = QPointF(m_nCurrentWidth, m_nCurrentHeight - 8);
    }

}

void CFdbOrderBaseItem::SetInPutIndex(int nInputIndex)
{
    int nBegin = 45 - 18;
//     if (GetEnoFlag() == true)
//     {
//         nBegin = 45 - 18;
//     }
//     else
//     {
//         nBegin = 45;
//     }

    m_nCurrentWidth = 5;
    m_nCurrentHeight = nBegin + (nInputIndex-1) * 18;

    m_nInputIndex = nInputIndex;
}

void CFdbOrderBaseItem::SetOutPutIndex(int nOutputIndex)
{
    int nBegin = 45 - 18;
//     if (GetEnoFlag() == true)
//     {
//         nBegin = 45 - 18;
//     }
//     else
//     {
//         nBegin = 45;
//     }

    m_nCurrentWidth = 102;
    m_nCurrentHeight = nBegin + (nOutputIndex-1) * 18;
    m_nOutputIndex = nOutputIndex;
}

void CFdbOrderBaseItem::SetInputMaxNum(int nIndex)
{
    m_nInputMaxNum = nIndex;

    m_nItemMaxHeight = (m_nInputMaxNum > m_nOutMaxNum ? m_nInputMaxNum : m_nOutMaxNum) * 18 + 80;

    if (m_nItemMaxHeight < 120)
    {
        m_rect.setHeight(m_nItemMaxHeight);
    }
}

int CFdbOrderBaseItem::GetInputMaxNum()
{
    return m_nInputMaxNum;
}

void CFdbOrderBaseItem::SetOutPutMaxNum(int nIndex)
{
    m_nOutMaxNum = nIndex;

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

void CFdbOrderBaseItem::SetInputPinNames(const QStringList &lstNames)
{
    m_lstInputPin = lstNames;
}

void CFdbOrderBaseItem::SetOutputPinNames(const QStringList &lstNames)
{
    m_lstOutputPin = lstNames;
}

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

void CFdbOrderBaseItem::AddTextBinInfo()
{
    if (m_nInputIndex != 0)
    {
        int nInputPinNum = m_nInputIndex;

//         if (GetEnoFlag())
//         {
//             nInputPinNum = m_nInputIndex - 1;
//         }
//         else
//         {
//             nInputPinNum = m_nInputIndex;
//         }

        //输入
        if (m_mapPinIndexText.contains(nInputPinNum))
        {
            m_mapPinIndexText[nInputPinNum]->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_mapPinIndexText[nInputPinNum]->setFocus();
        }
        else
        {
            QGraphicsTextItem *pinText = new QGraphicsTextItem(this);
            pinText->setPos(m_pfPinText.x()-80,m_pfPinText.y()-5);
            pinText->setTextWidth(80);
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

//         if (GetEnoFlag())
//         {
//             nInputPinNum = m_nOutputIndex - 1;
//         }
//         else
//         {
//             nInputPinNum = m_nOutputIndex;
//         }

        if (m_mapOutPinIndexText.contains(nInputPinNum))
        {
            m_mapOutPinIndexText[nInputPinNum]->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_mapOutPinIndexText[nInputPinNum]->setFocus();
        }
        else
        {
            QGraphicsTextItem *pinText = new QGraphicsTextItem(this);
            pinText->setPos(m_pfPinText.x(), m_pfPinText.y() - 5);
            pinText->setTextWidth(80);
            //pinText->document()->setDefaultTextOption(QTextOption(Qt::AlignRight));

            pinText->setTextInteractionFlags(Qt::TextEditorInteraction);
            pinText->setFocus();
            m_mapOutPinIndexText.insert(nInputPinNum, pinText);
        }
    }
}

void CFdbOrderBaseItem::SetStatusColor(const QColor statusColor)
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

QList<CConnectionRouter*> CFdbOrderBaseItem::GetAllLines()
{
    return m_polyLines;
}

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

void CFdbOrderBaseItem::UpdateCurrentItemInfo()
{
    //输入
    QStringList tLstInputs;
    tLstInputs.append("EN");
    tLstInputs.append(m_lstInputPin);

    for (int i = 0; i < tLstInputs.count(); i++)
    {
        if (i < m_nCurrentInputNums + 1)
        {
            LOGICITEMPRO tPinInfo;

            tPinInfo.bEnFlag = GetEnoFlag();

            tPinInfo.strPinName = tLstInputs.at(i);
            tPinInfo.nIndex = i + 1;

            if (m_mapPins.contains(i+1))
            {
                //output输入
                tPinInfo.nPinType = 1;

                if (m_mapPreInfos.contains(i+1))
                {
                    tPinInfo.outPinInfo.nCreateNum = m_mapPreInfos[i+1]->GetCreateNum();
                    tPinInfo.outPinInfo.nExecNum = m_mapPreInfos[i + 1]->GetIndexNum();
                    tPinInfo.outPinInfo.nOutPutIndex = m_mapPins[i+1];

                    QStringList tLstOutpus;
                    tLstOutpus.append("ENO");
                    tLstOutpus.append(m_mapPreInfos[i + 1]->GetOutputPins());

                    if (tLstOutpus.count() >= tPinInfo.outPinInfo.nOutPutIndex)
                    {
                        tPinInfo.outPinInfo.strOutPutPinName = tLstOutpus[tPinInfo.outPinInfo.nOutPutIndex - 1];
                    }
                    
                }
            }
            else
            {
                
                if (m_mapPinIndexText.contains(i + 1))
                {
                    //元件输入
                    tPinInfo.nPinType = 2;
                    tPinInfo.sCellInfo.strCell = m_mapPinIndexText[i+1]->toPlainText();
                }
                else
                {
                    //空 没有输入
                    tPinInfo.nPinType = 3;
                }

            }

            m_mapInputPro.insert(i + 1, tPinInfo);

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
            LOGICITEMPRO tPinInfo;

            tPinInfo.bEnFlag = GetEnoFlag();

            tPinInfo.strPinName = tLstoutPuts.at(i);
            tPinInfo.nIndex = i + 1;

            if (m_mapOutPinIndexText.contains(i + 1))
            {
                //元件输入
                tPinInfo.nPinType = 2;
                tPinInfo.sCellInfo.strCell = m_mapOutPinIndexText[i + 1]->toPlainText();
            }
            else
            {
                //空 没有输入
                tPinInfo.nPinType = 3;
            }

            m_mapOutPutPro.insert(i + 1, tPinInfo);
        }
    }
}

QStringList CFdbOrderBaseItem::GetOutputPins()
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

void CFdbOrderBaseItem::SetOutputData(int nOutPinNum, int nValue)
{
    m_mapOutput[nOutPinNum] = nValue;
}

QMap<int, int> CFdbOrderBaseItem::GetOutputData()
{
    return m_mapOutput;
}
