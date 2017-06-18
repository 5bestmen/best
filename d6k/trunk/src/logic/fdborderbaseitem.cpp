/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  fdborderbaseitem.cpp
�ļ�ʵ�ֹ��� :  �߼������
����         :  ww
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> �߼��༭
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \details ���캯��
** \param   
** \return
** \author ww
** \date 2017��1��4��
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
    //item�����ϲ� ע�͵�һ��  ֱ�ߵڶ���
    this->setZValue(3);
}

/*! \fn CFdbOrderBaseItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief AnalyseItemType(int nType)
** \details ����˵����ʶ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QString  CFdbOrderBaseItem::AnalyseItemType(int nType)
{
    return QString::number(nType);
}

/*! \fn CFdbOrderBaseItem::ResetRect(QRectF rect)
*********************************************************************************************************
** \brief ResetRect(QRectF rect)
** \details �ı��С
** \param  rect:�߼�������
** \return
** \author ww
** \date 2017��1��4��
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
** \details item��״
** \param  
** \return
** \author ww
** \date 2017��1��4��
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
** \date 2017��1��4��
** \note
********************************************************************************************************/
QPainterPath CFdbOrderBaseItem::shape() const
{
    QPainterPath path;
    //shape��С����ʵ��item��С  ��ֹ����item����̫��
    path.addRect(QRectF(-28, -28, m_rect.width() + 42, m_rect.height() + 42));
    return path;
}

/*! \fn CFdbOrderBaseItem::RemovePloyLine(CConnectionRouter *ployline)
*********************************************************************************************************
** \brief RemovePloyLine(CConnectionRouter *ployline)
** \details ployline:��Ҫɾ���������߹�ϵ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::RemovePloyLine(CConnectionRouter *ployline)
{
    //ȥ����
    int index = m_polyLines.indexOf(ployline);

    if (index != -1)
    {
        m_polyLines.removeAt(index);
    }
}

/*! \fn CFdbOrderBaseItem::RemovePloyLines()
*********************************************************************************************************
** \brief RemovePloyLines()
** \details ɾ�����item��ص����е�������
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ����������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AddPloyLine(CConnectionRouter *ployline)
{
    m_polyLines.append(ployline);
}

/*! \fn CFdbOrderBaseItem::AddPloyLine(CConnectionRouter *ployline)
*********************************************************************************************************
** \brief GetItemIOStatus()
** \details ��ȡ���������Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::GetItemIOStatus()
{
    // 1:�����  2:�����
    return m_nItemStatus;
}

/*! \fn CFdbOrderBaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief mousePressEvent(QGraphicsSceneMouseEvent *event)
** \details ������¼�
** \param
** \return
** \author ww
** \date 2017��1��4��
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
                //�ڷ�����״̬�� �����ڹܽ���������
                AddTextBinInfo();
            }
         
            m_nMoveBge = event->scenePos();
            m_pfClickPoint = this->pos();
            m_bScanl = true;
           
        }

    }
    else if (event->button() == Qt::RightButton)
    {
        //ɾ������
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
** \details ɾ��item�ۺ���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::Slot_DeleteAct()
{
    //ɾ������ǰ���ϵ
    QMap<int, QList<CFdbOrderBaseItem*> >::const_iterator itBeg = m_mapNextInfos.constBegin();

    while (itBeg != m_mapNextInfos.constEnd())
    {
        for each (CFdbOrderBaseItem* item in itBeg.value())
        {
            //ɾ�����item�������item��Ϣ
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
    //ɾ����item��ص�������Ϣ
    RemovePloyLines();

    //���±��
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    CFdbOrderView *pView = qobject_cast<CFdbOrderView*>(this->scene()->views().at(0));
    if (pView != nullptr)
    {
        pView->DeleteRemoveItem(this);
        pView->UpdateItemsExecNum();
    }

    this->scene()->removeItem(this);

    //����
    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

/*! \fn CFdbOrderBaseItem::AnalyseSencePoint(const QPointF &pScene)
*********************************************************************************************************
** \brief AnalyseSencePoint(const QPointF &pScene)
** \details �жϸ����ĵ����ĸ�������
** \param pScece ��λ��
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AnalyseSencePoint(const QPointF &pScene)
{
    AnalysePointPosition(mapFromScene(pScene));
}

/*! \fn CFdbOrderBaseItem::GetLinePoint(const QPointF &pItem)
*********************************************************************************************************
** \brief GetLinePoint(const QPointF &pItem)
** \details �жϸ����ĵ����ĸ�������
** \param pScece ��λ��
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::GetLinePoint(const QPointF &pItem)
{
    AnalysePointPosition(pItem);
}

/*! \fn CFdbOrderBaseItem::GetPointWidth()
*********************************************************************************************************
** \brief GetPointWidth()
** \details ��ȡ�������item�Ŀ��
** \param pScece ��λ��
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::GetPointWidth() const
{
    return m_nCurrentWidth;
}

/*! \fn CFdbOrderBaseItem::getPointHeight()
*********************************************************************************************************
** \brief getPointHeight()
** \details ��ȡ�������item�ĸ߶�
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::getPointHeight() const
{
    return m_nCurrentHeight;
}

/*! \fn CFdbOrderBaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief mouseMoveEvent(QGraphicsSceneMouseEvent *event)
** \details �ƶ��¼�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bScanl)   //�����ı��С
    {
        QPointF dis;
        m_nMoveEnd = event->scenePos();
        //�ƶ��ľ���
        dis = m_nMoveEnd - m_nMoveBge;
        m_nMoveBge = m_nMoveEnd;

        if (m_nChangeSize == 1)
        {
            QRectF tem = QRectF(0, m_rect.y(), m_rect.width(), m_rect.height() + dis.y());
            //item���ֵ  ��Сֵ�ж�
            if (m_rect.height() + dis.y() < m_nItemMaxHeight && m_rect.height() + dis.y() > 80)
            {
                int nMaxInIndex = 0;

                //�ҵ� �������� �Ѿ����ߵĵ�  item�ڸı��Сʱ ����С���Ѿ����ߵĶ��Ӵ�
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
                    //item��λλ���ж�
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
** \details ����ͷŲۺ���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //item�ͷŵĵط� ���ܺ�����item�ص�
    QList<QGraphicsItem*> lstGraphicsItem = this->collidingItems();

    if (!lstGraphicsItem.isEmpty())
    {
        for (int i = 0; i < lstGraphicsItem.count(); i++)
        {
            CFdbOrderBaseItem * pItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(lstGraphicsItem.at(i));
            if (pItem != nullptr && pItem->type() == OBJEC_TITEM)
            {
                //����ص�  �Ż�ԭλ��
                this->setPos(m_pfClickPoint);
            }
        }
    }

    if (m_bChangeFlag)
    {
        //itemλ���ƶ���  ��Ҫ�������ߵ���Ϣ
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
    //���� ����ģ��
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

/*! \fn CFdbOrderBaseItem::hoverMoveEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief hoverMoveEvent(QGraphicsSceneMouseEvent *event)
** \details ���hover�����¼�
** \param
** \return
** \author ww
** \date 2017��1��4��
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
        //����������״̬ʱ  �޸������ʽ
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
            //�ı������ʽ �϶�ʹ��
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
    //�ж��������λ��
    QPointF pointCurrent = event->pos();

    //�������㴦���ĸ����Ӵ�
    AnalysePointPosition(pointCurrent);

    QGraphicsItem::hoverMoveEvent(event);
    update();
}

/*! \fn CFdbOrderBaseItem::hoverLeaveEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief hoverLeaveEvent(QGraphicsSceneMouseEvent *event)
** \details ����뿪�����¼�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_nOutputIndex = 0;
    m_nInputIndex = 0;
    //�����������״̬
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
** \details ����뿪�����¼�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CFdbOrderBaseItem:: ~CFdbOrderBaseItem()
{
    delete m_cursor;
    //ɾ��������ص�������Ϣ
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
** \details ����ִ�б��
** \param  nIndexִ�б��
** \return
** \author ww
** \date 2017��1��4��
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
** \details ��ȡ�����Ϣ
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QMap<int, QList<CFdbOrderBaseItem*> > CFdbOrderBaseItem::GetOutPutList()
{
    //ÿ�������ֻ����һ������
    return m_mapNextInfos;
}

/*! \fn CFdbOrderBaseItem:GetInputItems()
*********************************************************************************************************
** \brief GetInputItems()
** \details ��ȡ������Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QMap<int, CFdbOrderBaseItem*> CFdbOrderBaseItem::GetInputItems()
{
    //ÿ������� ���� ����������
    return m_mapPreInfos;
}

/*! \fn CFdbOrderBaseItem:GetInputItems()
*********************************************************************************************************
** \brief GetInputNums()
** \details ��ȡ������Ӹ���
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details �������������Ϣ
** \param nIndex:���ӱ��  mapItem:���߶Զ�item
** \return
** \author ww
** \date 2017��1��4��
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
** \details �������������Ϣ
** \param nIndex:���ӱ��  mapItem:���߶Զ�item
** \return
** \author ww
** \date 2017��1��4��
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
** \details ɾ��item֮���ϵ
** \param pItem:���߶Զ�item
** \return
** \author ww
** \date 2017��1��4��
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
        //ɾ����Ӧ��ϵ
        for (int i = 0; i < nKeys.count(); i++)
        {
            m_mapPreInfos.remove(nKeys.at(i));
        }

    }
}


/*! \fn CFdbOrderBaseItem:DeleteOutputItem(CFdbOrderBaseItem* pItem)
*********************************************************************************************************
** \brief DeleteOutputItem(CFdbOrderBaseItem* pItem)
** \details ɾ��item֮���ϵ
** \param pItem:���߶Զ�item
** \return
** \author ww
** \date 2017��1��4��
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
        //ɾ��item֮���ϵ
        for (int i = 0; i < nKeys.count(); i++)
        {
            m_mapNextInfos.remove(nKeys.at(i));
        }
    }
}

/*! \fn CFdbOrderBaseItem:DeleteOneOutputItem(CFdbOrderBaseItem* pItem, int nIndex)
*********************************************************************************************************
** \brief DeleteOneOutputItem(CFdbOrderBaseItem* pItem, int nIndex)
** \details ɾ��ָ�����ӵ�item�Ĺ���
** \param pItem: ���߶Զ�item nIndex:���ӱ��
** \return
** \author ww
** \date 2017��1��4��
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
** \details �����߼�������
** \param nItemType:���ͱ��
** \return
** \author ww
** \date 2017��1��4��
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
** \details ���ô������
** \param nCreateNum:�������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetCreateNum(int nCreateNum)
{
    m_nCreateNum = nCreateNum;
    //��ʼ��������ź�ִ�б��һ��
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
** \details ��ȡִ�б��
** \param 
** \return
** \author ww
** \date 2017��1��4��
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
** \details ��ȡĳ��������ӵ������ߵĵ㼯
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QList<QPointF> CFdbOrderBaseItem::GetInputLines(int nInputIndex)
{
    QList<QPointF> linepoints;
    //���������߼���
    foreach(CConnectionRouter* lineItem,m_polyLines)
    {
        if (lineItem->StartItem() != this)
        {
            //�ҵ���Ӧ�����µ�������
            if (lineItem->GetEndInput() == nInputIndex)
            {
                //���ظ������ߵĵ��
                return lineItem->GetAllPoints();
            }
        }
    }

    return linepoints;
}

/*! \fn CFdbOrderBaseItem:GetSceneRectf()
*********************************************************************************************************
** \brief GetSceneRectf()
** \details ��ȡitem���Ӧ��scene������λ��
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ���������������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInputNum()
{
    m_nInputNums = m_mapPreInfos.count();
}

/*! \fn CFdbOrderBaseItem:MinusInputNum()
*********************************************************************************************************
** \brief MinusInputNum()
** \details ���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
int CFdbOrderBaseItem::MinusInputNum()
{
    return --m_nInputNums;
}

/*! \fn CFdbOrderBaseItem:keyPressEvent(QKeyEvent *event)
*********************************************************************************************************
** \brief keyPressEvent(QKeyEvent *event)
** \details ���̿���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::keyPressEvent(QKeyEvent *event)
{
    m_pfKeyPoint = this->scenePos();
    switch (event->key())
    {
    case Qt::Key_Left:
    {
        //�����ƶ�  ��ctrl ʱ �ƶ��ٶȼӿ�
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(0, -5);
        }
        else
        {
            MoveItemFromKey(0, -1);
        }
        //����ģ������
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }

        break;
    }
    case Qt::Key_Right:
    {
        //�����ƶ�  ��ctrl ʱ �ƶ��ٶȼӿ�
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(0, 5);
        }
        else
        {
            MoveItemFromKey(0, 1);
        }

        //��������ģ��
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    case Qt::Key_Up:
    {
        //�����ƶ�  ��ctrl ʱ �ƶ��ٶȼӿ�
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(-5, 0);
        }
        else
        {
            MoveItemFromKey(-1, 0);
        }

        //��������ģ��
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
        
        break;
    }
    case Qt::Key_Down:
    {
        //�����ƶ�  ��ctrl ʱ �ƶ��ٶȼӿ�
        if (event->modifiers() == Qt::ControlModifier)
        {
            MoveItemFromKey(5, 0);
        }
        else
        {
            MoveItemFromKey(1, 0);
        }

        //��������ģ��
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
** \details �ػ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    //Сѡ�п�
    AddHoverRect(painter);

    //ѡ��״̬�ı���ʽ
    if (option->state & QStyle::State_Selected)
    {
        //����ѡ�п�
        painter->setPen(QPen(QColor(192, 0, 0), 1, Qt::SolidLine));
        painter->drawRect(this->GetItemRect());
        painter->setPen(QPen(Qt::black, 0));
    }
    
    //���Ʊ��
    QString strTitle = "F" + QString::number(GetCreateNum()) + "_(" + QString::number(GetIndexNum()) + ")";
    painter->drawText(QRect(38, 0, 100, 15), strTitle);

    //����ѡ����ɫ ����
    QColor fillColor = (option->state & QStyle::State_Selected) ? GetItemColor().dark(150) : GetItemColor();

    //���õ����ɫ  �ټ���
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
        //�������eno��Ϣ  ����ʾ
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
        //ֻ��ʾ��ǰ�߶�������ʾ�Ķ���
        if (GetItemRect().y() + 40 + i * 18 > GetItemRect().height() - 20)
        {
            break;
        }

        m_nCurrentInputNums = i + 1;
        //��Ŷ���λ����Ϣ
        lines.append(QLineF(5, GetItemRect().y() + 45 + i * 18, 13, GetItemRect().y() + 45 + i * 18));
    }

    for (int j = 0; j < m_nOutMaxNum; j++)
    {
        //ֻ��ʾ��ǰ�߶�������ʾ���������
        if (GetItemRect().y() + 40 + j * 18 > GetItemRect().height() - 20)
        {
            break;
        }
        m_nCurrentOutputNums = j + 1;
        //��Ŷ���λ����Ϣ
        lines.append(QLineF(94, GetItemRect().y() + 45 + j * 18, 102, GetItemRect().y() + 45 + j * 18));
    }

    painter->setPen(QPen(m_colorStatus, 0));
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(QPen(Qt::black, 0));

    //��д����
    for (int i = 0; i < m_nInputMaxNum; i++)
    {
        if (GetItemRect().y() + 40 + i * 18 > GetItemRect().height() - 20)
        {
            //������ʾ��Χ�ڲ���ʾ
            break;
        }

        this->SetMaxInputHeight(GetItemRect().y() + 40 + i * 18 + 8);
        //��д����
        QString strIn = m_lstInputPin.at(i);
        painter->drawText(QRect(20, GetItemRect().y() + 40 + i * 18, 30, 10), strIn);
    }

    painter->drawText(QRect(46, GetItemRect().y() + 15, 36, 10), GetItemType());

    //��д�����ʾ����
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

    //item���ڲ�ע�Ϳ�  ��ʾ���߼��� �·�
    if (GetCommentItem() != nullptr)
    {
        GetCommentItem()->setPos(10, GetItemRect().height());
    }

}

/*! \fn CFdbOrderBaseItem:MoveItemFromKey(double disHeight, double disWidth)
*********************************************************************************************************
** \brief MoveItemFromKey(double disHeight, double disWidth)
** \details ���̿��� �ƶ��߼���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::MoveItemFromKey(double disHeight, double disWidth)
{
    //�ƶ����յ㴦item�ص��ĸ���
    QList<QGraphicsItem*> lstGraphicsItem = this->collidingItems();

    if (!lstGraphicsItem.isEmpty())   //�����ص������
    {
        //���������ص���item
        for (int i = 0; i < lstGraphicsItem.count(); i++)
        {
            //�ҳ�����Ϊ�߼����item
            CFdbOrderBaseItem * pItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(lstGraphicsItem.at(i));

            if (pItem != nullptr && pItem->type() == OBJEC_TITEM)//�ж��Ƿ�Ϊ�߼���
            {
                //�ƶ��ľ�����ڶ��ִ�С,���ҳ���λ�ƶ���С
                if (fabs(disHeight) > 1)
                {
                    disHeight = disHeight / fabs(disHeight);
                }

                if (fabs(disWidth) > 1)
                {
                    disWidth = disWidth / fabs(disWidth);
                }

                //��ײ��itemʱ ���ᷴ����ȥ  ������СΪ 28
                this->setPos(QPointF(m_pfKeyPoint.x() - disWidth*28, m_pfKeyPoint.y() - disHeight*28));

                //��item�ƶ�ʱ,��Ҫͬʱ�����߼��ߵ�λ��
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

    //���������ϰ���ʱ �����ƶ�
    this->setPos(QPointF(m_pfKeyPoint.x() + disWidth, m_pfKeyPoint.y() + disHeight));

    //�����߼���
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
** \details ��ȡeno�Ƿ���ʾ��ʶ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
bool CFdbOrderBaseItem::GetEnoFlag()
{
    return m_bEnoFlag;
}

/*! \fn CFdbOrderBaseItem:GetCommentData()
*********************************************************************************************************
** \brief GetCommentData()
** \details ��ȡ�߼����е�ע������
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details �����߼��·���ע��
** \param strComment:ע������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::Slot_UpdateComment(const QString &strComment)
{
    if (strComment.isEmpty())
    {
        //��ע������Ϊ��ʱ  ɾ��ע��item
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
    
    //ע��item  ��ʾ���߼����·�
    m_pCommentTextItem->setPlainText(strComment);
    m_pCommentTextItem->setDefaultTextColor(QColor(0, 128, 0));
    m_pCommentTextItem->setTextWidth(90);
    m_pCommentTextItem->setPos(10, GetItemRect().height());

    m_strComment = strComment;
}

/*! \fn CFdbOrderBaseItem:Slot_UpdateEnoInfo(int nFlag)
*********************************************************************************************************
** \brief Slot_UpdateEnoInfo(int nFlag)
** \details ����eno��ʶ
** \param strComment:ע������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::Slot_UpdateEnoInfo(int nFlag)
{
    if (nFlag == 0)
    {
        //�ر�
        CloseEnoItems();
    }
    else if (nFlag == 1)
    {
        //����
        ShowEnoItems();
    }

    //����ģ������
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
** \details ����λ����Ϣ �ж��Ƿ��ڶ��Ӵ� ������ɫ��ʾ
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AddHoverRect(QPainter *painter)
{
    if (painter == NULL || (m_nInputIndex == 0 && m_nOutputIndex == 0))
    {
        return;
    }

    //����ѡ�п�
    QBrush b = painter->brush();
    painter->setPen(QPen(QColor(0, 255, 0), 1, Qt::SolidLine));
    painter->setBrush(QBrush(Qt::green));

    //��ʼ��ַ Ϊeno���ڵ�λ��
    int nBegin = 45 - 18;

    //��index��Ϊ0��ʱ�� ��Ҫ���Ƶ�ǰ���ڵ�λ��
    if (m_nInputIndex != 0)
    {
        int nBgx = 5;
        int nBgy = nBegin + 18 * (m_nInputIndex-1) - 8;
        int nWidth = 8;
        int nHeight = 16;
        if ((m_nInputIndex == 1) && (GetEnoFlag() == false))
        {
            //����ʾ״̬
        }
        else
        {
            //����rect
            painter->drawRect(nBgx, nBgy, nWidth, nHeight);
        }
    }

    //��index��Ϊ0��ʱ�� ��Ҫ���Ƶ�ǰ���ڵ�λ��
    if (m_nOutputIndex != 0)
    {
        int nBgx = 94;
        int nBgy = nBegin + 18 * (m_nOutputIndex-1) - 8;
        int nWidth = 8;
        int nHeight = 16;

        if ((m_nOutputIndex == 1) && (GetEnoFlag() == false))
        {
            //����ʾ״̬
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
** \details ���ݵ��ж�  ���ĸ����Ӵ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AnalysePointPosition(const QPointF &pointCurrent)
{
    m_nItemStatus = 0;
    //��ʼ����λ��
    int nBegin = 45 - 8 - 18;

    //���
    if ((pointCurrent.x() >= 94) && (pointCurrent.x() <= 102))   //���x��Χ
    {
        if ((pointCurrent.y() >= nBegin) && (pointCurrent.y() <= m_nOutMaxHeight))  //���y��Χ
        {
            //���������ж��ڵڼ���
            if (pointCurrent.y() < nBegin + 8)
            {
                //�׸������ж�
                m_nOutputIndex = 1;
            }
            else
            {
                //
                int nHeight = pointCurrent.y() - (nBegin + 8);
                //��Ŵ�1��ʼ
                int nFloor = nHeight / 18 + 1;
                int nLength = nHeight % 18;

                if (nLength > 8 && nLength < 10)
                {
                    //�м��
                    m_nOutputIndex = 0;
                }
                else
                {
                    if (nLength <= 8)
                    {
                        //����
                        m_nOutputIndex = nFloor;
                    }
                    else
                    {
                        //����������һ��
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

    //����
    //����ȡֵ��Χ
    if ((pointCurrent.x() >= 5) && (pointCurrent.x() <= 13))
    {
        //����ȡֵ��Χ
        if ((pointCurrent.y() >= nBegin) && (pointCurrent.y() <= m_nInsertMaxHeight))
        {
            //���������ж��ڵڼ���
            if (pointCurrent.y() < nBegin + 8)
            {
                //�׸������ж�
                m_nInputIndex = 1;
            }
            else
            {
                //
                int nHeight = pointCurrent.y() - (nBegin + 8);
                //��Ŵ�1��ʼ
                int nFloor = nHeight / 18 + 1;
                int nLength = nHeight % 18;

                if (nLength > 8 && nLength < 10)
                {
                    //�м��
                    m_nInputIndex = 0;
                }
                else
                {
                    if (nLength <= 8)
                    {
                        //����
                        m_nInputIndex = nFloor;
                    }
                    else
                    {
                        //����������һ��
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

    //��¼�����λ����Ϣ
    if (m_nInputIndex != 0)
    {
        //
        m_nCurrentWidth = 5;
        m_nCurrentHeight = nBegin + 8 + (m_nInputIndex-1) * 18;
        m_nItemStatus = IO_INPUT;
        m_pfPinText = QPointF(m_nCurrentWidth, m_nCurrentHeight-8);
    }

    //��¼�����λ����Ϣ
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
** \details ��������� λ����Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInPutIndex(int nInputIndex)
{
    //y������ʼ
    int nBegin = 45 - 18;

    m_nCurrentWidth = 5;
    m_nCurrentHeight = nBegin + (nInputIndex-1) * 18;

    m_nInputIndex = nInputIndex;
}

/*! \fn CFdbOrderBaseItem:SetOutPutIndex(int nInputIndex)
*********************************************************************************************************
** \brief SetOutPutIndex(int nInputIndex)
** \details ��������� λ����Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutPutIndex(int nOutputIndex)
{
    //y����ʼ
    int nBegin = 45 - 18;

    m_nCurrentWidth = 102;
    m_nCurrentHeight = nBegin + (nOutputIndex-1) * 18;
    m_nOutputIndex = nOutputIndex;
}

/*! \fn CFdbOrderBaseItem:SetInputMaxNum(int nIndex)
*********************************************************************************************************
** \brief SetInputMaxNum(int nIndex)
** \details �����������߶�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInputMaxNum(int nIndex)
{
    m_nInputMaxNum = nIndex;
    //�Ƚ�����������������С 
    m_nItemMaxHeight = (m_nInputMaxNum > m_nOutMaxNum ? m_nInputMaxNum : m_nOutMaxNum) * 18 + 80;

    //���߶Ȳ��ܳ���120
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
** \details �����������߶�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutPutMaxNum(int nIndex)
{
    m_nOutMaxNum = nIndex;
    //�Ƚ�����������������С 
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
** \details ���ö�������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetInputPinNames(const QStringList &lstNames)
{
    m_lstInputPin = lstNames;
}

/*! \fn CFdbOrderBaseItem:SetOutputPinNames(const QStringList &lstNames)
*********************************************************************************************************
** \brief SetOutputPinNames(const QStringList &lstNames)
** \details ���ö�������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetOutputPinNames(const QStringList &lstNames)
{
    m_lstOutputPin = lstNames;
}

/*! \fn CFdbOrderBaseItem:InsertPins(int nEndIndex, int nBeginIndex)
*********************************************************************************************************
** \brief InsertPins(int nEndIndex, int nBeginIndex)
** \details �������  �� �����������  ÿ���������ֻ����һ���������
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ÿ�����ӳ������ֶ���������
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::AddTextBinInfo()
{
    if (m_nInputIndex != 0)
    {
        int nInputPinNum = m_nInputIndex;

        //����
        if (m_mapPinIndexText.contains(nInputPinNum))
        {
            //�ɱ༭״̬
            m_mapPinIndexText[nInputPinNum]->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_mapPinIndexText[nInputPinNum]->setFocus();
        }
        else
        {
            //�������ı�����
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
        //���
        int nInputPinNum = m_nOutputIndex;

        if (m_mapOutPinIndexText.contains(nInputPinNum))
        {
            //�Ѿ����ڵĶ��� �ɱ༭
            m_mapOutPinIndexText[nInputPinNum]->setTextInteractionFlags(Qt::TextEditorInteraction);
            m_mapOutPinIndexText[nInputPinNum]->setFocus();
        }
        else
        {
            //�����µ��ı�����
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
** \details ��ȡ���й������߼���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QList<CConnectionRouter*> CFdbOrderBaseItem::GetAllLines()
{
    return m_polyLines;
}

/*! \fn CFdbOrderBaseItem:SetCurrentInputNum(int nInputNum)
*********************************************************************************************************
** \brief SetCurrentInputNum(int nInputNum)
** \details ���õ�ǰ����߶�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::SetCurrentInputNum(int nInputNum)
{
    m_nCurrentInputNums = nInputNum;
    //���ø߶�
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
** \details �������������ϸ��Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbOrderBaseItem::UpdateCurrentItemInfo()
{
    //����
    QStringList tLstInputs;
    tLstInputs.append("EN");
    tLstInputs.append(m_lstInputPin);

    m_vInputPros.clear();
    m_vOutputPros.clear();

    for (int i = 0; i < tLstInputs.count(); i++)
    {
        if (i < m_nCurrentInputNums + 1)
        {
            //��װ������Ϣ
            std::shared_ptr<CBinProperty> pPinProperty = std::make_shared<CBinProperty>();
            
            pPinProperty->SetPinName(tLstInputs.at(i));

            pPinProperty->SetPinIndex(i + 1);

            if (m_mapPins.contains(i+1))
            {
                //output����
                pPinProperty->SetPinType(1);

                if (m_mapPreInfos.contains(i+1))
                {
                    //��ȡ������Ϣ
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
                    //Ԫ������
                    pPinProperty->SetPinType(2);
                    std::shared_ptr<CCellProperty> tCellpro = pPinProperty->CreateCellPro();
                    tCellpro->SetCellName(m_mapPinIndexText[i + 1]->toPlainText());

                }
                else
                {
                    //�� û������
					pPinProperty->SetPinType(3);
                }
            }
            
            m_vInputPros.emplace_back(pPinProperty);
        }
    }

    //���
    QStringList tLstoutPuts;
    tLstoutPuts.append("ENO");
    tLstoutPuts.append(m_lstOutputPin);

    for (int i = 0; i < tLstoutPuts.count(); i++)
    {
        if (i < m_nCurrentOutputNums + 1)
        {
            //��ȡ������Ϣ
            std::shared_ptr<CBinProperty> pPinOutProperty = std::make_shared<CBinProperty>();

            pPinOutProperty->SetPinName(tLstoutPuts.at(i));
            pPinOutProperty->SetPinIndex(i + 1);

            if (m_mapOutPinIndexText.contains(i + 1))
            {
                //Ԫ������
                pPinOutProperty->SetPinType(2);
                pPinOutProperty->CreateCellPro()->SetCellName(m_mapOutPinIndexText[i + 1]->toPlainText());

            }
            else
            {
                //�� û������
                pPinOutProperty->SetPinType(3);
            }

            m_vOutputPros.emplace_back(pPinOutProperty);
        }
    }
}

/*! \fn CFdbOrderBaseItem:GetOutputPins()
*********************************************************************************************************
** \brief GetOutputPins()
** \details ��ȡ���������������
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details �������  �Ͷ�Ӧ�ı��
** \param
** \return
** \author ww
** \date 2017��1��4��
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
** \details ��ȡ���������Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
const std::vector<std::shared_ptr<CBinProperty>> CFdbOrderBaseItem::GetInputPros()
{
    return m_vInputPros;
}

/*! \fn CFdbOrderBaseItem:GetOutPutPros()
*********************************************************************************************************
** \brief GetOutPutPros()
** \details ��ȡ���������Ϣ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
const std::vector<std::shared_ptr<CBinProperty>> CFdbOrderBaseItem::GetOutPutPros()
{
    return m_vOutputPros;
}
