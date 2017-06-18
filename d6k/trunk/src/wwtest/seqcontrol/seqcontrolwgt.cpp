#include <QVBoxLayout>
#include <QSplitter>
#include <QApplication>
#include <QDesktopWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QDockWidget>
#include "seqcontrolwgt.h"
#include "treewidget.h"
#include "fdborderscene.h"
#include "fdborderview.h"
#include "seqdefine.h"
#include "fdbitemchoosewgt.h"

CSeqControlWgt::CSeqControlWgt(QWidget *parent)
    : QWidget(parent),
      m_pTreeInfo(new CTreeWidget),
      //m_pGraphicScene(new CPlcOrderScene(this)),
      m_pCursorToolBar(new QToolBar(this)),
      m_pFdbItemChooseWgt(new CFdbItemChooseWgt(this)),
      m_pTabWgt(new QTabWidget)
{
    resize(1900,1100);
   // m_pGraphicsView = new CPlcOrderView(m_pGraphicScene);

    CreateToolBar();

    QSplitter *pSplitter = new QSplitter(this);
    pSplitter->addWidget(m_pTreeInfo);
    m_pTreeInfo->setMinimumWidth(300);
    //
    m_pTabWgt->setTabsClosable(true);
    m_pTabWgt->setTabPosition(QTabWidget::South);
    pSplitter->addWidget(m_pTabWgt);

    QDockWidget *pdockfpditems = new QDockWidget(this);
    pdockfpditems->setWindowTitle(tr("FBD ITEM"));
    pdockfpditems->setWidget(m_pFdbItemChooseWgt);

    pSplitter->addWidget(pdockfpditems);
    
    pSplitter->setStretchFactor(1,1);


    QVBoxLayout *pLayout = new QVBoxLayout(this);
    //pLayout->addWidget(m_pCursorToolBar);
    pLayout->addWidget(pSplitter);
    connect(m_pTreeInfo, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_DoubleClickTreeItem(QTreeWidgetItem *, int)));
    connect(m_pTabWgt, SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_CloseTabWidget(int)));

    m_pTreeInfo->expandAll();
}

void CSeqControlWgt::CreateToolBar()
{
//     QAction *pArrowAct = new QAction(tr("Choose"), this);
//     pArrowAct->setStatusTip(tr("Choose Object"));
//     connect(pArrowAct, SIGNAL(triggered()), this, SLOT(Slot_ChooseCursor()));
// 
//     QAction *pLineAct = new QAction(tr("Line"), this);
//     pLineAct->setStatusTip(tr("line"));
//     connect(pLineAct, SIGNAL(triggered()), this, SLOT(Slot_LineCursor()));
// 
//     m_pCursorToolBar->addAction(pArrowAct);
//     m_pCursorToolBar->addAction(pLineAct);
//     m_pCursorToolBar->addSeparator();

}

void CSeqControlWgt::Slot_ChooseCursor()
{
    //m_pGraphicScene->SetMode(CURSORARROW);
//   //  this->setCursor(Qt::ArrowCursor);
   // m_pGraphicsView->setCursor(Qt::ArrowCursor);
}

void CSeqControlWgt::Slot_LineCursor()
{
   // m_pGraphicScene->SetMode(CURSORLINE);
    
   // QCursor *xx = new QCursor;
   // xx->setShape(Qt::CrossCursor);
   // this->setCursor(Qt::CrossCursor);
    //m_pGraphicsView->setCursor(*xx);
}


CSeqControlWgt::~CSeqControlWgt()
{
}

void CSeqControlWgt::Slot_DoubleClickTreeItem(QTreeWidgetItem *pItem, int nColumn)
{
    if (nColumn != 0)
    {
        return;
    }

    if (pItem->data(0, Qt::UserRole) != EDITITEM)
    {
        return;
    }

    if (m_mapItemWgt.contains(pItem))
    {
        m_pTabWgt->setCurrentWidget(m_mapItemWgt[pItem]);
        m_pTabWgt->setTabText(m_pTabWgt->currentIndex(),pItem->text(0));
    }
    else
    {
        //插入新数据
        CFdbOrderScene *pScene = new CFdbOrderScene(this);
        CFdbOrderView *pView = new CFdbOrderView(pScene);

        m_mapItemWgt.insert(pItem, pView);
        m_pTabWgt->addTab(pView, pItem->text(0));
        m_pTabWgt->setCurrentWidget(pView);
    }
}

void CSeqControlWgt::Slot_CloseTabWidget(int nIndex)
{
    //
    QWidget *pCloseWgt = m_pTabWgt->widget(nIndex);

    if (pCloseWgt != NULL)
    {
        m_mapItemWgt.remove(m_mapItemWgt.key(pCloseWgt));
        m_pTabWgt->removeTab(nIndex);
    }
}
