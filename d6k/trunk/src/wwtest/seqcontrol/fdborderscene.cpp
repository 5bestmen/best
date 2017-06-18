#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QCursor>
#include <QMenu>
#include <QMessageBox>
#include <QBitmap>
#include <QFileDialog>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include "fdborderscene.h"
#include "fdborderbaseitem.h"
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "commentitem.h"
#include "funblockdiagramitem.h"
#include "logicxmlwriter.h"
#include "logicxmlreader.h"
#include "dataioanalyse.h"

CFdbOrderScene::CFdbOrderScene(QObject *parent)
    : QGraphicsScene(parent)
{
    m_nMode = 0;
    m_nCpyCutFlag = 0;
    m_pLineItem = nullptr;
    m_pChoseItem = nullptr;
    //
    m_nControlNum = -1;
    this->setSceneRect(0, 0, 500, 500);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Slot_TimeOut()));
}

CFdbOrderScene::~CFdbOrderScene()
{
//     for each (CConnectionRouter* item in m_lstRouters)
//     {
// //         if (item != nullptr)
// //         {
//             item->deleteLater();
//       //  }
//         
//     }
}

void CFdbOrderScene::SetMode(int nMode)
{
    m_nMode = nMode;
}

//检测是否为合法终点
void CFdbOrderScene::UpdateItemPoint(QPointF pfPoint)
{
    CFdbOrderBaseItem *pEndItem = qgraphicsitem_cast<CFdbOrderBaseItem *>(itemAt(pfPoint, QTransform()));

    if ((pEndItem != NULL && pEndItem->type() == OBJECTITEM))
    {
        pEndItem->AnalysePointPosition(pEndItem->mapFromScene(pfPoint));
        pEndItem->update();
    }
}


void CFdbOrderScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    qDebug() << mouseEvent->scenePos();
    if (mouseEvent->button() == Qt::LeftButton)
    {
        if (m_nMode == CURSORLINE)
        {
            m_pfClickBeg.setX(mouseEvent->scenePos().x());
            m_pfClickBeg.setY(mouseEvent->scenePos().y());
            m_pLineItem = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            m_pLineItem->setPen(QPen(Qt::DotLine));
            m_pLineItem->setZValue(2);
            addItem(m_pLineItem);
        }
        else
        {
            if (items(QRect(mouseEvent->scenePos().x() - 2, mouseEvent->scenePos().y()-2,4,4)).count() == 0)
            {
                m_pfChooseBeg.setX(mouseEvent->scenePos().x());
                m_pfChooseBeg.setY(mouseEvent->scenePos().y());

                m_pChoseItem = new QGraphicsRectItem;
                //m_pChoseItem->setPen(QPen(Qt::DotLine));
                m_pChoseItem->setZValue(2);
                addItem(m_pChoseItem);
            }
        }

        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        if (this->itemAt(mouseEvent->scenePos(), QTransform()) != nullptr)
        {
            QGraphicsScene::mousePressEvent(mouseEvent);
            return;
        }
        m_pfRightPoint = mouseEvent->scenePos();

        m_pfCurrentPoint.setX(mouseEvent->scenePos().x());
        m_pfCurrentPoint.setY(mouseEvent->scenePos().y());

        m_nMode = CURSORARROW;

        QMenu *pMenu = new QMenu;

        QAction *pLineAct = new QAction(QObject::tr("Draw Line  "),this);
        connect(pLineAct, SIGNAL(triggered()), this, SLOT(Slot_OnLineAct()));
        //注释框
        QAction *pCommentAct = new QAction(tr("Add Comment"),this);
        connect(pCommentAct, SIGNAL(triggered()), this, SLOT(Slot_AddCommentAct()));

        //保存框
        QAction *pSaveAct = new QAction(tr("Save as..."),this);
        connect(pSaveAct, SIGNAL(triggered()), this, SLOT(Slot_SaveAsAct()));

        //打开框
        QAction *pOpenAct = new QAction(tr("Open"),this);
        connect(pOpenAct, SIGNAL(triggered()), this, SLOT(Slot_OpenAct()));

        //撤销
        QAction *pBackAct = new QAction(tr("Undo"),this);
        connect(pBackAct, SIGNAL(triggered()), this, SLOT(Slot_UndoAct()));
        //重做
        QAction *pReDoAct = new QAction(tr("Redo"),this);
        connect(pReDoAct, SIGNAL(triggered()), this, SLOT(Slot_RedoAct()));
        //清空
        QAction *pClearAllAct = new QAction(tr("Clear All"),this);
        connect(pClearAllAct,SIGNAL(triggered()),this,SLOT(Slot_ClearAllAct()));
        //复制
        QAction *pCopyAct = new QAction(tr("Copy"),this);
        connect(pCopyAct, SIGNAL(triggered()), this, SLOT(Slot_CopyAct()));
        //剪切
        QAction *pCutAct = new QAction(tr("Cut"),this);
        connect(pCutAct, SIGNAL(triggered()), this, SLOT(Slot_CutAct()));
        //粘贴
        QAction *pPasteAct = new QAction(tr("Paste"),this);
        connect(pPasteAct, SIGNAL(triggered()), this, SLOT(Slot_PasteAct()));
        //仿真
        QAction *pEmulation = new QAction(tr("Emulation"),this);
        connect(pEmulation, SIGNAL(triggered()), this, SLOT(Slot_Emulation()));

        if (m_nControlNum < 0)
        {
            pBackAct->setEnabled(false);
        }
        else
        {
            pBackAct->setEnabled(true);
        }

        if (m_nControlNum + 1 > m_lstGraphicsData.count()-1 || m_nControlNum >= 49)
        {
            pReDoAct->setEnabled(false);
        }
        else
        {
            pReDoAct->setEnabled(true);
        }

        if ((m_CpyData.commentData.count() == 0 && m_CpyData.itemData.count() == 0 && m_CpyData.lineData.count() == 0)
            && (m_CutData.commentData.count() == 0 && m_CutData.itemData.count() == 0 && m_CutData.lineData.count() == 0))
        {
            pPasteAct->setEnabled(false);
        }

        pMenu->addAction(pLineAct);
        pMenu->addAction(pCommentAct);
        pMenu->addSeparator();
        pMenu->addAction(pSaveAct);
        pMenu->addAction(pOpenAct);
        pMenu->addAction(pClearAllAct);
        pMenu->addSeparator();
        pMenu->addAction(pCopyAct);
        pMenu->addAction(pCutAct);
        pMenu->addAction(pPasteAct);
        pMenu->addSeparator();
        pMenu->addAction(pBackAct);
        pMenu->addAction(pReDoAct);
        pMenu->addSeparator();
        pMenu->addAction(pEmulation);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }
}

void CFdbOrderScene::Slot_OnLineAct()
{
    this->SetMode(CURSORLINE);

    QGraphicsView *pView = this->views().at(0);
    if (pView != nullptr)
    {
        QPixmap pixmap(":/images/pen.png");
        QCursor pCursor(pixmap, 0, pixmap.height());
        pView->viewport()->setCursor(pCursor);
    }
}

void CFdbOrderScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //
    UpdateItemPoint(mouseEvent->scenePos());

    if (m_nMode == CURSORLINE && m_pLineItem != NULL)
    {
        QLineF newLine(m_pLineItem->line().p1(), mouseEvent->scenePos());
        m_pLineItem->setLine(newLine);
    }
    else
    {
        if (m_pChoseItem != nullptr)
        {
            int nxLegth = mouseEvent->scenePos().x() - m_pfChooseBeg.x();
            int nylegth = mouseEvent->scenePos().y() - m_pfChooseBeg.y();
            if (nxLegth >=0 && nylegth >=0)
            {
                m_pChoseItem->setRect(m_pfChooseBeg.x(), m_pfChooseBeg.y(), nxLegth, nylegth);
            }
            else if (nxLegth <0 && nylegth>=0)
            {
                m_pChoseItem->setRect(m_pfChooseBeg.x()+nxLegth, m_pfChooseBeg.y(), -nxLegth, nylegth);
            }
            else if (nxLegth >=0 && nylegth <0)
            {
                m_pChoseItem->setRect(m_pfChooseBeg.x(), m_pfChooseBeg.y()+nylegth, nxLegth, -nylegth);
            }
            else if (nxLegth<0 && nylegth <0)
            {
                m_pChoseItem->setRect(m_pfChooseBeg.x()+nxLegth, m_pfChooseBeg.y()+nylegth, -nxLegth, -nylegth);
            }

            QList<QGraphicsItem *> lstItems = m_pChoseItem->collidingItems();

            for each (QGraphicsItem *item in lstItems)
            {
                item->setSelected(true);
            }
            
        }

        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void CFdbOrderScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if (m_pLineItem != NULL && m_nMode == CURSORLINE)
    {
        QList<QGraphicsItem *> startItems = items(m_pLineItem->line().p1());

        if (startItems.count() && startItems.first() == m_pLineItem)
        {
            startItems.removeFirst();
        }

        QList<QGraphicsItem *> endItems = items(m_pLineItem->line().p2());

        if (endItems.count() && endItems.first() == m_pLineItem)
        {
            endItems.removeFirst();
        }

        removeItem(m_pLineItem);
        delete m_pLineItem;
        m_pLineItem = nullptr;

        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first() != endItems.first())
        {
            CFdbOrderBaseItem *startItem = qgraphicsitem_cast<CFdbOrderBaseItem *>(startItems.first());
            CFdbOrderBaseItem *endItem = qgraphicsitem_cast<CFdbOrderBaseItem *>(endItems.first());

            startItem->AnalyseSencePoint(m_pfClickBeg);
            endItem->AnalyseSencePoint(mouseEvent->scenePos());
            //判断起点和终点是否合法
            
            if ((startItem!=NULL) && (endItem!=NULL) && (startItem->type()==OBJECTITEM) && (endItem->type()==OBJECTITEM))
            {
                if (startItem->GetItemIOStatus()*endItem->GetItemIOStatus() == 2)
                {
                    //赋值分析
                    CConnectionRouter *plyLine = nullptr;
                    if (startItem->GetItemIOStatus() == 1)
                    {
                        plyLine = new CConnectionRouter(endItem, startItem, this);
                        plyLine->SetEndInput(startItem->GetInputIndex());
                        plyLine->SetBegiOutput(endItem->GetOutputIndex());
                        //增加关联
                        endItem->InsertOuputItem(endItem->GetOutputIndex(),startItem);
                        startItem->InsertInputItem(startItem->GetInputIndex(), endItem);
                        //增加记录
                        startItem->InsertPins(startItem->GetInputIndex(), endItem->GetOutputIndex());
                    }
                    else
                    {
                        plyLine = new CConnectionRouter(startItem, endItem, this);
                        plyLine->SetEndInput(endItem->GetInputIndex());
                        plyLine->SetBegiOutput(startItem->GetOutputIndex());
                        //增加关联
                        startItem->InsertOuputItem(startItem->GetOutputIndex(),endItem);
                        endItem->InsertInputItem(endItem->GetInputIndex(), startItem);
                        //增加pin对应关系
                        endItem->InsertPins(endItem->GetInputIndex(), startItem->GetOutputIndex());
                    }
                    
                    //m_lstRouters.append(plyLine);
                    plyLine->UpdatePosition();

                    startItem->AddPloyLine(plyLine);
                    endItem->AddPloyLine(plyLine);

                    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));
                    if (pView != nullptr)
                    {
                        pView->UpdateItemsExecNum();
                    }
                }
            }
        }
    }
    else if (m_pChoseItem != nullptr)
    {
        m_pfChooseAreaStart = QPointF(m_pChoseItem->rect().x(), m_pChoseItem->rect().y());
        removeItem(m_pChoseItem);
        delete m_pChoseItem;
        m_pChoseItem = nullptr;
        this->update();

    }

    m_pLineItem = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

int CFdbOrderScene::GetMode() const
{
    return m_nMode;
}

void CFdbOrderScene::Slot_AddCommentAct()
{
    CCommentItem *pComment = new CCommentItem;
    pComment->setPos(m_pfCurrentPoint);
    this->addItem(pComment);
    //m_lstComments.append(pComment);

    //更新

    UpdateOperate();
    
}

void CFdbOrderScene::Slot_SaveAsAct()
{
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save File"), "./", "Text files (*.xml)");

    if (!fileName.isEmpty())
    {
        MakeSaveData(fileName);
    }
}

void CFdbOrderScene::Slot_OpenAct()
{
    QString filename = QFileDialog::getOpenFileName(0, tr("Save File"), "./", "Text files (*.xml)");

    if (!filename.isEmpty())
    {
        ClearAllItems();

        CLogicXmlReader reader;
        reader.OpenXmlFile(filename);

        CDataIOAnalyse dataAnalyse(this);
        dataAnalyse.MakeLogicItem(reader.GetItemInfo());
        dataAnalyse.MaieLineItem(reader.GetLineInfo());
        dataAnalyse.MakeCommentItem(reader.GetCommentInfo());

        UpdateOperate();
    }

    
}

void CFdbOrderScene::MakeSaveData(const QString &filename)
{
    //获取数据
    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView == nullptr)
    {
        QMessageBox::warning(0, tr("Warning"),tr("save file error!"));
        return;
    }

    //item 列表
    QList<ITEMINFO> lstItemInfos;
    //line列表
    QList<LINEROUNTER> lstLineRounter;
    //注释
    QList<COMMENTDATA> lstComment;

    //循环遍历所有item  
    foreach(QGraphicsItem *item, this->items())
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJECTITEM)
        {
            ITEMINFO itempro;
            itempro.itemRect = pBlockItem->GetSceneRectf();
            itempro.strItemComment = pBlockItem->GetCommentData();
            itempro.nCreateNum = pBlockItem->GetCreateNum();
            itempro.nExecNum = pBlockItem->GetIndexNum();
            itempro.lstInInfo = pBlockItem->GetAllInputPro();
            itempro.nItemType = pBlockItem->GetItemTypNum();
            itempro.bEx = pBlockItem->GetEnoFlag();

//             pBlockItem->UpdateCurrentItemInfo();
//             itempro.lstInInfo = pBlockItem->GetAllInputPro();

            lstItemInfos.append(itempro);

            //找出所有item的输出线
            LINEROUNTER itemLine;
            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                itemLine.strEndItem = QString::number(pBlockItem->GetCreateNum());
                itemLine.strBgeItem = QString::number(it.value()->GetCreateNum());
                itemLine.strEndIndex = QString::number(it.key());
                itemLine.strBeginIndex = QString::number(pBlockItem->GetBinMapInfo()[it.key()]);
                itemLine.lstAllPoints = pBlockItem->GetInputLines(it.key());
                lstLineRounter.append(itemLine);

                it++;
            }
        }

        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENTITEM && pcommitem->GetTextType() == GRAPHICSCOMMENT)
        {
            COMMENTDATA commentData;
            commentData.pfPos = pcommitem->scenePos();
            commentData.rfRect = pcommitem->GetItemRect();
            commentData.strComment = pcommitem->toPlainText().simplified();
            lstComment.append(commentData);

        }
    }

    CLogicXmlWriter xmlWriter;

    xmlWriter.UpdateXmlFile(filename, lstItemInfos, lstLineRounter, lstComment);
}

void CFdbOrderScene::Slot_UndoAct()
{
    ClearAllItems();
    m_nControlNum--;
    MakeNewGraphic();
}

void CFdbOrderScene::MakeNewGraphic()
{
    if (m_nControlNum <0 || m_nControlNum > 49 || m_nControlNum>m_lstGraphicsData.count()-1)
    {
        return;
    }


    CDataIOAnalyse dataAnalyse(this); 
    dataAnalyse.MakeLogicItem(m_lstGraphicsData.at(m_nControlNum).itemData);
    dataAnalyse.MaieLineItem(m_lstGraphicsData.at(m_nControlNum).lineData);
    dataAnalyse.MakeCommentItem(m_lstGraphicsData.at(m_nControlNum).commentData);

}


void CFdbOrderScene::Slot_RedoAct()
{
    ClearAllItems();
    m_nControlNum++;
    MakeNewGraphic();
}

void CFdbOrderScene::UpdateOperate()
{
    //获取数据
    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    //item 列表
    QList<ITEMINFO> lstItemInfos;
    //line列表
    QList<LINEROUNTER> lstLineRounter;
    //注释
    QList<COMMENTDATA> lstComment;

    //循环遍历所有item  
    foreach(QGraphicsItem *item, this->items())
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJECTITEM)
        {
            ITEMINFO itempro;
            itempro.itemRect = pBlockItem->GetSceneRectf();
            itempro.strItemComment = pBlockItem->GetCommentData();
            itempro.nCreateNum = pBlockItem->GetCreateNum();
            itempro.nExecNum = pBlockItem->GetIndexNum();
            itempro.lstInInfo = pBlockItem->GetAllInputPro();
            itempro.nItemType = pBlockItem->GetItemTypNum();
            itempro.bEx = pBlockItem->GetEnoFlag();

            lstItemInfos.append(itempro);

            //找出所有item的输出线
            LINEROUNTER itemLine;
            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                itemLine.strEndItem = QString::number(pBlockItem->GetCreateNum());
                itemLine.strBgeItem = QString::number(it.value()->GetCreateNum());
                itemLine.strEndIndex = QString::number(it.key());
                itemLine.strBeginIndex = QString::number(pBlockItem->GetBinMapInfo()[it.key()]);

                itemLine.lstAllPoints = pBlockItem->GetInputLines(it.key());
                lstLineRounter.append(itemLine);

                it++;
            }
        }

        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENTITEM && pcommitem->GetTextType() == GRAPHICSCOMMENT)
        {
            COMMENTDATA commentData;
            commentData.pfPos = pcommitem->scenePos();
            commentData.rfRect = pcommitem->GetItemRect();
            commentData.strComment = pcommitem->toPlainText().simplified();
            lstComment.append(commentData);

        }

    }

    GRAHPICDATA tGrahpicsData; 
    tGrahpicsData.itemData = lstItemInfos;
    tGrahpicsData.commentData = lstComment;
    tGrahpicsData.lineData = lstLineRounter;


    if (m_lstGraphicsData.count() < 50)
    {
        m_lstGraphicsData.append(tGrahpicsData);
    }
    else
    {
        m_lstGraphicsData.removeFirst();
        m_lstGraphicsData.append(tGrahpicsData);
    }

    //list编号
    m_nControlNum = m_lstGraphicsData.count()-1;

}

void CFdbOrderScene::ClearAllItems()
{
    QList<QGraphicsItem *>lstpItem = this->items();

//     for each (QGraphicsItem* item in lstpItem)
//     {
//           delete item;
//     }


    while (!lstpItem.isEmpty())
    {
        delete lstpItem.at(0);
        lstpItem = this->items();
    }

    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView != nullptr)
    {
        pView->ClearCreateNum();
        pView->ClearItems();
        return;
    }

}

void CFdbOrderScene::Slot_ClearAllAct()
{
    ClearAllItems();
}

void CFdbOrderScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Z:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            if (m_nControlNum >= 0)
            {
                Slot_UndoAct();
            }
        }

        break;
    }
    case Qt::Key_Y:
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            if (m_nControlNum + 1 > m_lstGraphicsData.count() - 1 || m_nControlNum >= 49)
            {
                
            }
            else
            {
                Slot_RedoAct();
            }
            
        }
        break;
    }
    case Qt::Key_X:
    {
        //剪切
        Slot_CutAct();
        break;
    }
    case Qt::Key_C:
    {
        //复制
        Slot_CopyAct();
        break;
    }
    case Qt::Key_V:
    {
        //粘贴
        Slot_PasteAct();
        break;
    }

    default:
        break;
    }


    QGraphicsScene::keyPressEvent(event);
}

void CFdbOrderScene::Slot_CopyAct()
{
    m_nCpyCutFlag = COPYONLY;

    m_pfCpyCutPoint = m_pfChooseAreaStart;
    QList<QGraphicsItem*> lstSelectedItems = this->selectedItems();

    //创建
    //获取数据
    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    //item 列表
    QList<ITEMINFO> lstItemInfos;
    //line列表
    QList<LINEROUNTER> lstLineRounter;
    //注释
    QList<COMMENTDATA> lstComment;

    //循环遍历所有item  
    foreach(QGraphicsItem *item, lstSelectedItems)
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJECTITEM)
        {
            ITEMINFO itempro;
            itempro.itemRect = pBlockItem->GetSceneRectf();
            itempro.strItemComment = pBlockItem->GetCommentData();
            itempro.nCreateNum = pBlockItem->GetCreateNum();
            itempro.nExecNum = pBlockItem->GetIndexNum();
            itempro.lstInInfo = pBlockItem->GetAllInputPro();
            itempro.nItemType = pBlockItem->GetItemTypNum();
            itempro.bEx = pBlockItem->GetEnoFlag();

//             pBlockItem->UpdateCurrentItemInfo();
// 
//             itempro.lstInInfo = pBlockItem->GetAllInputPro();

            lstItemInfos.append(itempro);

            //找出所有item的输出线
            LINEROUNTER itemLine;
            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                itemLine.strEndItem = QString::number(pBlockItem->GetCreateNum());
                itemLine.strBgeItem = QString::number(it.value()->GetCreateNum());
                itemLine.strEndIndex = QString::number(it.key());
                itemLine.strBeginIndex = QString::number(pBlockItem->GetBinMapInfo()[it.key()]);
                itemLine.lstAllPoints = pBlockItem->GetInputLines(it.key());
                lstLineRounter.append(itemLine);

                it++;
            }
        }

        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENTITEM && pcommitem->GetTextType() == GRAPHICSCOMMENT)
        {
            COMMENTDATA commentData;
            commentData.pfPos = pcommitem->scenePos();
            commentData.rfRect = pcommitem->GetItemRect();
            commentData.strComment = pcommitem->toPlainText().simplified();
            lstComment.append(commentData);

        }
    }

    m_CpyData.itemData = lstItemInfos;
    m_CpyData.commentData = lstComment;
    m_CpyData.lineData = lstLineRounter;
}

void CFdbOrderScene::Slot_PasteAct()
{
    if ((m_CpyData.commentData.count() == 0 && m_CpyData.itemData.count() == 0 && m_CpyData.lineData.count() == 0)
        && (m_CutData.commentData.count() == 0 && m_CutData.itemData.count() == 0 && m_CutData.lineData.count() == 0))
    {
        return;
    }

    if (m_nCpyCutFlag == COPYONLY)
    {
        CDataIOAnalyse dataAnalyse(this);
        dataAnalyse.SetStartPoint(m_pfCpyCutPoint);
        dataAnalyse.SetEndPoint(m_pfRightPoint);
        dataAnalyse.CopyLogicItem(m_CpyData.itemData);
        dataAnalyse.CopyLineItem(m_CpyData.lineData);
        dataAnalyse.CopyCommentItem(m_CpyData.commentData);

    }
    else if (m_nCpyCutFlag == CUTONLY)
    {
        CDataIOAnalyse dataAnalyse(this);
        dataAnalyse.SetStartPoint(m_pfCpyCutPoint);
        dataAnalyse.SetEndPoint(m_pfRightPoint);
        dataAnalyse.CutLoginItem(m_CutData.itemData);
        dataAnalyse.CutLineItem(m_CutData.lineData);
        dataAnalyse.CutCommentItem(m_CutData.commentData);
        m_CutData.commentData.clear();
        m_CutData.itemData.clear();
        m_CutData.lineData.clear();
    }
}

void CFdbOrderScene::Slot_CutAct()
{
    m_nCpyCutFlag = CUTONLY;

    m_pfCpyCutPoint = m_pfChooseAreaStart;
    //剪切
    QList<QGraphicsItem*> lstSelectedItems = this->selectedItems();

    //创建
    //获取数据
    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView == nullptr)
    {
        return;
    }

    //item 列表
    QList<ITEMINFO> lstItemInfos;
    //line列表
    QList<LINEROUNTER> lstLineRounter;
    //注释
    QList<COMMENTDATA> lstComment;
    //实体litem
    QList<QGraphicsItem*> lstItems;


    //循环遍历所有item  
    foreach(QGraphicsItem *item, lstSelectedItems)
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJECTITEM)
        {
            ITEMINFO itempro;
            itempro.itemRect = pBlockItem->GetSceneRectf();
            itempro.strItemComment = pBlockItem->GetCommentData();
            itempro.nCreateNum = pBlockItem->GetCreateNum();
            itempro.nExecNum = pBlockItem->GetIndexNum();
            itempro.lstInInfo = pBlockItem->GetAllInputPro();
            itempro.nItemType = pBlockItem->GetItemTypNum();
            itempro.bEx = pBlockItem->GetEnoFlag();

            lstItemInfos.append(itempro);

            //找出所有item的输出线
            LINEROUNTER itemLine;
            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                itemLine.strEndItem = QString::number(pBlockItem->GetCreateNum());
                itemLine.strBgeItem = QString::number(it.value()->GetCreateNum());
                itemLine.strEndIndex = QString::number(it.key());
                itemLine.strBeginIndex = QString::number(pBlockItem->GetBinMapInfo()[it.key()]);
                itemLine.lstAllPoints = pBlockItem->GetInputLines(it.key());
                lstLineRounter.append(itemLine);

                it++;
            }
            lstItems.append(pBlockItem);
            pView->DeleteRemoveItem(pBlockItem);
        }

        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENTITEM && pcommitem->GetTextType() == GRAPHICSCOMMENT)
        {
            COMMENTDATA commentData;
            commentData.pfPos = pcommitem->scenePos();
            commentData.rfRect = pcommitem->GetItemRect();
            commentData.strComment = pcommitem->toPlainText().simplified();
            lstComment.append(commentData);
            lstItems.append(pcommitem);
        }
    }


    for each (QGraphicsItem *item in lstItems)
    {
        removeItem(item);
        delete item;
    }
    
    m_CutData.itemData = lstItemInfos;
    m_CutData.commentData = lstComment;
    m_CutData.lineData = lstLineRounter;
}

void CFdbOrderScene::Slot_Emulation()
{
    CFdbOrderView *pView = (CFdbOrderView*)this->views().at(0);
    if (pView != nullptr)
    {
        pView->UpdateItemList();
    }
    m_pTimer->setInterval(2000);
    m_pTimer->start();
}

void CFdbOrderScene::Slot_TimeOut()
{
    CFdbOrderView *pView = (CFdbOrderView*)this->views().at(0);
    if (pView != nullptr)
    {
        for each (CFdbOrderBaseItem* item in pView->GetAllItems())
        {
            item->AnalseBitInput();
        }
    }
}

