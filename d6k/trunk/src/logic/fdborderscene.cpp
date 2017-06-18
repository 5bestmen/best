/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  fdborderscene.cpp
文件实现功能 :  逻辑块场景
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
#include "logiccontainter.h"
#include "logicmodel.h"
#include "logicitemfactory.h"

/*! \fn CFdbOrderScene(QObject *parent, std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CFdbOrderScene(QObject *parent, std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details 构造函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFdbOrderScene::CFdbOrderScene(QObject *parent, std::shared_ptr<CLogicObjectModel> pObjectModel)
    : QGraphicsScene(parent)
{
    m_nMode = 0;
    m_nCpyCutFlag = 0;
    m_pLineItem = nullptr;
    m_pChoseItem = nullptr;
    m_pCpyData = nullptr;
    m_pCutData = nullptr;
    m_pItemFactory = nullptr;

    Q_ASSERT(pObjectModel);
    m_pSecenModel = pObjectModel;
    
    if (m_pSecenModel == nullptr)
    {
        return;
    }
        //
    m_nControlNum = -1;
    this->setSceneRect(0, 0, 500, 500);

    //模拟仿真使用定时器
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Slot_TimeOut()));
}

CFdbOrderScene::~CFdbOrderScene()
{
    delete m_pCpyData;
    delete  m_pCutData;
    delete m_pItemFactory;
}

//
/*! \fn SetMode(int nMode)
*********************************************************************************************************
** \brief SetMode(int nMode)
** \details 分为划线状态和编辑状态
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::SetMode(int nMode)
{
    m_nMode = nMode;
}

//
/*! \fn CFdbOrderScene(QObject *parent, std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief UpdateItemPoint(QPointF pfPoint)
** \details 判断鼠标移动到那个逻辑块的端子处
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::UpdateItemPoint(const QPointF &pfPoint)
{
    CFdbOrderBaseItem *pEndItem = qgraphicsitem_cast<CFdbOrderBaseItem *>(itemAt(pfPoint, QTransform()));

    if ((pEndItem != NULL && pEndItem->type() == OBJEC_TITEM))
    {
        //当鼠标移动到逻辑块上时  判断是否是端子
        pEndItem->AnalysePointPosition(pEndItem->mapFromScene(pfPoint));
        pEndItem->update();
    }
}

/*! \fn mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
*********************************************************************************************************
** \brief mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
** \details 鼠标点击事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        if (m_nMode == CURSOR_LINE)   //连线状态
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
            //选择区域
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

        if (m_pCpyData == nullptr)
        {
            //复制的数据
            m_pCpyData = new CLogicObjectModel;
        }

        if (m_pCutData == nullptr)
        {
            //剪切的数据
            m_pCutData = new CLogicObjectModel;
        }

        m_pfRightPoint = mouseEvent->scenePos();

        m_pfCurrentPoint.setX(mouseEvent->scenePos().x());
        m_pfCurrentPoint.setY(mouseEvent->scenePos().y());

        m_nMode = CURSOR_ARROW;

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

        //设置redo  undo  的编辑状态
        if (m_nControlNum < 0)
        {
            pBackAct->setEnabled(false);
        }
        else
        {
            pBackAct->setEnabled(true);
        }

        if (m_nControlNum + 1 > m_lstHistoryData.count()-1 || m_nControlNum >= 49)
        {
            pReDoAct->setEnabled(false);
        }
        else
        {
            pReDoAct->setEnabled(true);
        }

        if ((m_pCpyData->GetCommentProperty().empty() && m_pCpyData->GetBlockProperty().empty() && m_pCpyData->GetLineProperty().empty())
            && (m_pCutData->GetCommentProperty().empty() && m_pCutData->GetBlockProperty().empty() && m_pCutData->GetLineProperty().empty()))
        {
            //没有数据不能粘贴
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

/*! \fn Slot_OnLineAct()
*********************************************************************************************************
** \brief Slot_OnLineAct()
** \details 划线状态
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_OnLineAct()
{
    //连线状态
    this->SetMode(CURSOR_LINE);

    QGraphicsView *pView = this->views().at(0);
    if (pView != nullptr)
    {
        //改变鼠标样式
        QPixmap pixmap(":/images/pen.png");
        QCursor pCursor(pixmap, 0, pixmap.height());
        pView->viewport()->setCursor(pCursor);
    }
}

/*! \fn CFdbOrderScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
*********************************************************************************************************
** \brief CFdbOrderScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
** \details 鼠标移动
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //鼠标移动时 检测是否到端子处
    UpdateItemPoint(mouseEvent->scenePos());

    if (m_nMode == CURSOR_LINE && m_pLineItem != NULL)
    {
        //划中间线
        QLineF newLine(m_pLineItem->line().p1(), mouseEvent->scenePos());
        m_pLineItem->setLine(newLine);
    }
    else
    {
        //改变选中框大小
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
            //在选中框区域内的item全部 变为选中状态
            QList<QGraphicsItem *> lstItems = m_pChoseItem->collidingItems();

            for each (QGraphicsItem *item in lstItems)
            {
                item->setSelected(true);
            }
            
        }

        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

/*! \fn CFdbOrderScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
*********************************************************************************************************
** \brief CFdbOrderScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
** \details 鼠标释放事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if (m_pLineItem != NULL && m_nMode == CURSOR_LINE)
    {
        //划线状态下的处理
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
            
            if ((startItem != NULL) && (endItem != NULL) && (startItem->type() == OBJEC_TITEM) && (endItem->type() == OBJEC_TITEM))
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
        //选中框的处理
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

/*! \fn CFdbOrderScene::Slot_AddCommentAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_AddCommentAct()
** \details 增加注释
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_AddCommentAct()
{
    CCommentItem *pComment = new CCommentItem;
    pComment->setPos(m_pfCurrentPoint);
    this->addItem(pComment);
    //更新数据模型
    UpdateOperate();
}

/*! \fn CFdbOrderScene::Slot_SaveAsAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_SaveAsAct()
** \details 保存
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_SaveAsAct()
{
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save File"), "./", "Text files (*.xml)");

    if (!fileName.isEmpty())
    {
        MakeSaveData(fileName);
    }
}

/*! \fn CFdbOrderScene::Slot_OpenAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_OpenAct()
** \details 打开
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_OpenAct()
{
    QString filename = QFileDialog::getOpenFileName(0, tr("Save File"), "./", "Text files (*.xml)");
    //清空原有显示内容
    m_pSecenModel->ClearAllProperties();

    if (!filename.isEmpty())
    {
        ClearAllItems();

        CLogicXmlReader reader;
        reader.OpenLogicXmlFile(filename, m_pSecenModel);

        CDataIOAnalyse dataAnalyse(this);
        dataAnalyse.CreateLogic();
    }

}

/*! \fn CFdbOrderScene::MakeSaveData(const QString &filename)
*********************************************************************************************************
** \brief CFdbOrderScene::MakeSaveData(const QString &filename)
** \details 保存数据
** \param   保存文件名称
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::MakeSaveData(const QString &filename)
{
    //获取数据
    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView == nullptr)
    {
        QMessageBox::warning(0, tr("Warning"),tr("save file error!"));
        return;
    }
    //清除原有数据
    m_pSecenModel->ClearAllProperties();

    //循环遍历所有item  
    for each(QGraphicsItem *item in this->items())
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJEC_TITEM)
        {
            //add by wangwei
            std::shared_ptr<CBlockProperty> tBlockPro = m_pSecenModel->CreateBlockProperty(pBlockItem->GetCreateNum());
            Q_ASSERT(tBlockPro);
            if (tBlockPro != nullptr)
            {
                tBlockPro->SetBlockRect(pBlockItem->GetSceneRectf());
                tBlockPro->SetComment(pBlockItem->GetCommentData());
                tBlockPro->SetExecNum(pBlockItem->GetIndexNum());
                tBlockPro->SetItemType(pBlockItem->GetItemTypNum());
                tBlockPro->SetEnoShowFlag(pBlockItem->GetEnoFlag());
                tBlockPro->CreatePinFromOther(pBlockItem->GetInputPros());

            }
            //add by wangwei end

            //找出所有item的输出线
//             LINEROUNTER itemLine;
            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                //add by wangwei
                std::shared_ptr<CLineProperty> tLinePro = m_pSecenModel->CreateLineProperty();
                Q_ASSERT(tLinePro);
                if (tLinePro != nullptr)
                {
                    tLinePro->SetEndItemIndex(pBlockItem->GetCreateNum());
                    tLinePro->SetBeginItemIndex(it.value()->GetCreateNum());
                    tLinePro->SetEndPinIndex(it.key());
                    tLinePro->SetBeginPinIndex(pBlockItem->GetBinMapInfo()[it.key()]);

                    for (int i = 0; i < pBlockItem->GetInputLines(it.key()).count(); i++)
                    {
                        tLinePro->SetPoints(pBlockItem->GetInputLines(it.key()).at(i).x(), pBlockItem->GetInputLines(it.key()).at(i).y());
                    }

                }
                //add by wangwei end

                it++;
            }
        }

        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);
        //保存注释信息
        if (pcommitem != nullptr && pcommitem->type() == COMMENT_ITEM && pcommitem->GetTextType() == GRAPHICS_COMMENT)
        {
            std::shared_ptr<CCommentProperty> tCommentPro = m_pSecenModel->CreateCommentProperty();
            tCommentPro->SetCommentPos(pcommitem->scenePos());
            tCommentPro->SetRect(pcommitem->GetItemRect());
            tCommentPro->SetComment(pcommitem->toPlainText().simplified());
        }
    }

    CLogicXmlWriter xmlWriter;

    xmlWriter.SaveDataToXml(filename, m_pSecenModel);
}

/*! \fn CFdbOrderScene::Slot_UndoAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_UndoAct()
** \details undo
** \param   
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_UndoAct()
{
    //清除原有的item
    ClearAllItems();
    m_nControlNum--;
    //创建新的item
    MakeNewGraphic();
}

/*! \fn CFdbOrderScene::MakeNewGraphic()
*********************************************************************************************************
** \brief CFdbOrderScene::MakeNewGraphic()
** \details 创建新的item对象
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::MakeNewGraphic()
{
    //回退只能50步
    if (m_nControlNum <0 || m_nControlNum > 49 || m_nControlNum>m_lstHistoryData.count()-1)
    {
        return;
    }

    //创建界面
    CDataIOAnalyse dataAnalyse(this); 
    dataAnalyse.CreateLogicItem(m_lstHistoryData.at(m_nControlNum)->GetBlockProperty());
    dataAnalyse.CreateLogicLine(m_lstHistoryData.at(m_nControlNum)->GetLineProperty());
    dataAnalyse.CreateLogicComment(m_lstHistoryData.at(m_nControlNum)->GetCommentProperty());

    //清空模型数据
    m_pSecenModel->ClearAllProperties();
    //模型数据保存
    SaveOperateData(m_pSecenModel);
}

/*! \fn CFdbOrderScene::Slot_RedoAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_RedoAct()
** \details redo
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_RedoAct()
{
    //清空现有item
    ClearAllItems();
    m_nControlNum++;
    //创建新界面
    MakeNewGraphic();
}

/*! \fn CFdbOrderScene::UpdateOperate()
*********************************************************************************************************
** \brief CFdbOrderScene::UpdateOperate()
** \details 更加数据模型
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::UpdateOperate()
{
    //创建新对象  供undo  redo  使用
    std::shared_ptr<CLogicObjectModel> pObjectModel = std::make_shared<CLogicObjectModel>(m_pSecenModel->GetName());
    //清空数据
    m_pSecenModel->ClearAllProperties();

    //循环数据保存
    SaveOperateData(pObjectModel);
    //模型数据保存
    SaveOperateData(m_pSecenModel);
    
    //undo  rodo  共使用50步
    if (m_lstHistoryData.count() < 50)
    {
        m_lstHistoryData.append(pObjectModel);
    }
    else
    {
        m_lstHistoryData.removeFirst();
        m_lstHistoryData.append(pObjectModel);
    }

    //list编号
    m_nControlNum = m_lstHistoryData.count() - 1;
}

/*! \fn CFdbOrderScene::SaveOperateData(std::shared_ptr<CLogicObjectModel> pObjectModel)
*********************************************************************************************************
** \brief CFdbOrderScene::SaveOperateData(std::shared_ptr<CLogicObjectModel> pObjectModel)
** \details 保存界面数据至模型
** \param  模型
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::SaveOperateData(std::shared_ptr<CLogicObjectModel> pObjectModel)
{
    //遍历所有item
    for each(QGraphicsItem *item in this->items())
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJEC_TITEM)  //找出逻辑块
        {
            std::shared_ptr<CBlockProperty> tBlockPro = pObjectModel->CreateBlockProperty(pBlockItem->GetCreateNum());
            Q_ASSERT(tBlockPro);
            if (tBlockPro != nullptr)
            {
                //读取item数据
                tBlockPro->SetBlockRect(pBlockItem->GetSceneRectf());
                tBlockPro->SetComment(pBlockItem->GetCommentData());
                tBlockPro->SetExecNum(pBlockItem->GetIndexNum());
                tBlockPro->SetItemType(pBlockItem->GetItemTypNum());
                tBlockPro->SetEnoShowFlag(pBlockItem->GetEnoFlag());
                tBlockPro->CreatePinFromOther(pBlockItem->GetInputPros());
            }

            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                //读取逻辑线信息
                std::shared_ptr<CLineProperty> tLinePro = pObjectModel->CreateLineProperty();
                Q_ASSERT(tLinePro);
                if (tLinePro != nullptr)
                {
                    tLinePro->SetEndItemIndex(pBlockItem->GetCreateNum());
                    tLinePro->SetBeginItemIndex(it.value()->GetCreateNum());
                    tLinePro->SetEndPinIndex(it.key());
                    tLinePro->SetBeginPinIndex(pBlockItem->GetBinMapInfo()[it.key()]);

                    for (int i = 0; i < pBlockItem->GetInputLines(it.key()).count(); i++)
                    {
                        tLinePro->SetPoints(pBlockItem->GetInputLines(it.key()).at(i).x(), pBlockItem->GetInputLines(it.key()).at(i).y());
                    }

                }

                it++;
            }
        }
        //读取注释信息
        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENT_ITEM && pcommitem->GetTextType() == GRAPHICS_COMMENT)
        {
            std::shared_ptr<CCommentProperty> tCommentPro = pObjectModel->CreateCommentProperty();
            tCommentPro->SetCommentPos(pcommitem->scenePos());
            tCommentPro->SetRect(pcommitem->GetItemRect());
            tCommentPro->SetComment(pcommitem->toPlainText().simplified());
        }
    }
}

/*! \fn CFdbOrderScene::ClearAllItems()
*********************************************************************************************************
** \brief CFdbOrderScene::ClearAllItems()
** \details 清空所有界面数据
** \param  模型
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::ClearAllItems()
{
    QList<QGraphicsItem *>lstpItem = this->items();
    //遍历  清空
    while (!lstpItem.isEmpty())
    {
        delete lstpItem.at(0);
        lstpItem = this->items();
    }

    CFdbOrderView* pView = qobject_cast<CFdbOrderView*>(this->views().at(0));

    if (pView != nullptr)
    {
        //数据清空
        pView->ClearCreateNum();
        pView->ClearItems();
        return;
    }

}

/*! \fn CFdbOrderScene::Slot_ClearAllAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_ClearAllAct()
** \details 清空所有界面数据
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_ClearAllAct()
{
    ClearAllItems();
}

/*! \fn CFdbOrderScene::keyPressEvent(QKeyEvent *event)
*********************************************************************************************************
** \brief CFdbOrderScene::keyPressEvent(QKeyEvent *event)
** \details 键盘操作
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Z:
    {
        if (event->modifiers() == Qt::ControlModifier)    //ctrl + z = undo
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
        if (event->modifiers() == Qt::ControlModifier)   //ctrl _y  = redo
        {
            //50步
            if (m_nControlNum + 1 > m_lstHistoryData.count() - 1 || m_nControlNum >= 49)
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

/*! \fn CFdbOrderScene::Slot_CopyAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_CopyAct()
** \details 复制槽函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_CopyAct()
{
    m_nCpyCutFlag = COPY_ONLY;

    if (m_pCpyData == nullptr)
    {
        //复制保存的的数据
        m_pCpyData = new CLogicObjectModel;
    }

    m_pCpyData->ClearAllProperties();

    m_pfCpyCutPoint = m_pfChooseAreaStart;
    QList<QGraphicsItem*> lstSelectedItems = this->selectedItems();

    //循环遍历所有item  
    for each(QGraphicsItem *item in lstSelectedItems)
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);

        if (pBlockItem != nullptr && pBlockItem->type() == OBJEC_TITEM)
        {
            //逻辑块  数据保存
            std::shared_ptr<CBlockProperty> tBlockPro = m_pCpyData->CreateBlockProperty(pBlockItem->GetCreateNum());

            Q_ASSERT(tBlockPro);
            if (tBlockPro != nullptr)
            {
                tBlockPro->SetBlockRect(pBlockItem->GetSceneRectf());
                tBlockPro->SetComment(pBlockItem->GetCommentData());
                tBlockPro->SetExecNum(pBlockItem->GetIndexNum());
                tBlockPro->SetItemType(pBlockItem->GetItemTypNum());
                tBlockPro->SetEnoShowFlag(pBlockItem->GetEnoFlag());
                tBlockPro->CreatePinFromOther(pBlockItem->GetInputPros());

            }

            //找出所有item的输出线
            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                //直线对象
                std::shared_ptr<CLineProperty> tLinePro = m_pCpyData->CreateLineProperty();
                Q_ASSERT(tLinePro);
                if (tLinePro != nullptr)
                {
                    tLinePro->SetEndItemIndex(pBlockItem->GetCreateNum());
                    tLinePro->SetBeginItemIndex(it.value()->GetCreateNum());
                    tLinePro->SetEndPinIndex(it.key());
                    tLinePro->SetBeginPinIndex(pBlockItem->GetBinMapInfo()[it.key()]);

                    for (int i = 0; i < pBlockItem->GetInputLines(it.key()).count(); i++)
                    {
                        tLinePro->SetPoints(pBlockItem->GetInputLines(it.key()).at(i).x(), pBlockItem->GetInputLines(it.key()).at(i).y());
                    }

                }

                it++;
            }
        }

        //保存注释
        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENT_ITEM && pcommitem->GetTextType() == GRAPHICS_COMMENT)
        {
            std::shared_ptr<CCommentProperty> tCommentPro = m_pCpyData->CreateCommentProperty();
            tCommentPro->SetCommentPos(pcommitem->scenePos());
            tCommentPro->SetRect(pcommitem->GetItemRect());
            tCommentPro->SetComment(pcommitem->toPlainText().simplified());
        }
    }
}

/*! \fn CFdbOrderScene::Slot_PasteAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_PasteAct()
** \details past
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_PasteAct()
{
    //不存在数据则返回
    if ((m_pCpyData->GetCommentProperty().empty() && m_pCpyData->GetBlockProperty().empty() && m_pCpyData->GetLineProperty().empty())
        && (m_pCutData->GetCommentProperty().empty() && m_pCutData->GetBlockProperty().empty() && m_pCutData->GetLineProperty().empty()))
    {
        return;
    }

    if (m_nCpyCutFlag == COPY_ONLY)
    {
        //复制对象
        CDataIOAnalyse dataAnalyse(this);
        dataAnalyse.SetStartPoint(m_pfCpyCutPoint);
        dataAnalyse.SetEndPoint(m_pfRightPoint);
        dataAnalyse.CopyLogicItem(m_pCpyData->GetBlockProperty());
        dataAnalyse.CopyLineItem(m_pCpyData->GetLineProperty());
        dataAnalyse.CopyCommentItem(m_pCpyData->GetCommentProperty());
    }
    else if (m_nCpyCutFlag == CUT_ONLY)
    {
        //剪切对象
        CDataIOAnalyse dataAnalyse(this);
        dataAnalyse.SetStartPoint(m_pfCpyCutPoint);
        dataAnalyse.SetEndPoint(m_pfRightPoint);
        dataAnalyse.CutLogicItem(m_pCutData->GetBlockProperty());
        dataAnalyse.CutLineItem(m_pCutData->GetLineProperty());
        dataAnalyse.CutCommentItem(m_pCutData->GetCommentProperty());

        m_pCutData->ClearAllProperties();
    }
}

/*! \fn CFdbOrderScene::Slot_CutAct()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_CutAct()
** \details 剪切
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_CutAct()
{
    m_nCpyCutFlag = CUT_ONLY;

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

     //实体litem
     QList<QGraphicsItem*> lstItems;

     m_pCutData->ClearAllProperties();

    //循环遍历所有item  
    for each(QGraphicsItem *item in lstSelectedItems)
    {
        //当前只有逻辑运算一种类型  不做区分
        CFdbOrderBaseItem *pBlockItem = qgraphicsitem_cast<CFdbOrderBaseItem*>(item);
        if (pBlockItem != nullptr && pBlockItem->type() == OBJEC_TITEM)
        {
            //逻辑块数据
            std::shared_ptr<CBlockProperty> tBlockPro = m_pCutData->CreateBlockProperty(pBlockItem->GetCreateNum());
            Q_ASSERT(tBlockPro);
            if (tBlockPro != nullptr)
            {
                tBlockPro->SetBlockRect(pBlockItem->GetSceneRectf());
                tBlockPro->SetComment(pBlockItem->GetCommentData());
                tBlockPro->SetExecNum(pBlockItem->GetIndexNum());
                tBlockPro->SetItemType(pBlockItem->GetItemTypNum());
                tBlockPro->SetEnoShowFlag(pBlockItem->GetEnoFlag());
                tBlockPro->CreatePinFromOther(pBlockItem->GetInputPros());

            }

            //获取前向数据
            QMap<int, CFdbOrderBaseItem*> mapInfo = pBlockItem->GetInputItems();

            QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapInfo.begin();
            while (it != mapInfo.end())
            {
                //直线数据
                std::shared_ptr<CLineProperty> tLinePro = m_pCutData->CreateLineProperty();
                Q_ASSERT(tLinePro);
                if (tLinePro != nullptr)
                {
                    tLinePro->SetEndItemIndex(pBlockItem->GetCreateNum());
                    tLinePro->SetBeginItemIndex(it.value()->GetCreateNum());
                    tLinePro->SetEndPinIndex(it.key());
                    tLinePro->SetBeginPinIndex(pBlockItem->GetBinMapInfo()[it.key()]);

                    for (int i = 0; i < pBlockItem->GetInputLines(it.key()).count(); i++)
                    {
                        tLinePro->SetPoints(pBlockItem->GetInputLines(it.key()).at(i).x(), pBlockItem->GetInputLines(it.key()).at(i).y());
                    }

                }

                it++;
            }

            lstItems.append(pBlockItem);
            pView->DeleteRemoveItem(pBlockItem);
        }

        //注释
        CCommentItem *pcommitem = qgraphicsitem_cast<CCommentItem*>(item);

        if (pcommitem != nullptr && pcommitem->type() == COMMENT_ITEM && pcommitem->GetTextType() == GRAPHICS_COMMENT)
        {
            std::shared_ptr<CCommentProperty> tCommentPro = m_pCutData->CreateCommentProperty();
            tCommentPro->SetCommentPos(pcommitem->scenePos());
            tCommentPro->SetRect(pcommitem->GetItemRect());
            tCommentPro->SetComment(pcommitem->toPlainText().simplified());
        }
    }

    //清除原有数据
    for each (QGraphicsItem *item in lstItems)
    {
        removeItem(item);
        delete item;
    }
    
}

/*! \fn CFdbOrderScene::Slot_Emulation()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_Emulation()
** \details 仿真
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_Emulation()
{
    CFdbOrderView *pView = (CFdbOrderView*)this->views().at(0);
    if (pView != nullptr)
    {
        //顺序执行
        pView->UpdateItemList();
    }
    m_pTimer->setInterval(2000);
    m_pTimer->start();
}

/*! \fn CFdbOrderScene::Slot_TimeOut()
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_TimeOut()
** \details 仿真计时
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

std::shared_ptr<CLogicObjectModel> CFdbOrderScene::GetObjectModel()
{
    return m_pSecenModel;
}

/*! \fn CFdbOrderScene::UpdateModelData()
*********************************************************************************************************
** \brief CFdbOrderScene::UpdateModelData()
** \details 根据模型创建数据
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::UpdateModelData()
{
    //加载数据
    CDataIOAnalyse dataAnalyse(this);
    dataAnalyse.CreateLogic();
}

/*! \fn CFdbOrderScene::CreateItemObj(int nChangeType, int nIndex)
*********************************************************************************************************
** \brief CFdbOrderScene::CreateItemObj(int nChangeType, int nIndex)
** \details 创建对象
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFdbOrderBaseItem * CFdbOrderScene::CreateItemObj(int nChangeType, int nIndex)
{
    if (m_pItemFactory == nullptr)
    {
        m_pItemFactory = new CLogicItemFactory;
    }

    return m_pItemFactory->CreateLogicItemProduct(nChangeType, nIndex);
}

/*! \fn CFdbOrderScene::Slot_DeleteComment
*********************************************************************************************************
** \brief CFdbOrderScene::Slot_DeleteComment
** \details 删除注释item 更新模型
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderScene::Slot_DeleteComment()
{
    sender()->deleteLater();

    this->UpdateOperate();
}

