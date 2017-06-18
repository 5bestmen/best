#include <QLabel>
#include <QDataStream>
#include <QPixmap>
#include <QMimeData>
#include <QPainter>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "treewidget.h"
#include "seqdefine.h"
#include "attrwgt.h"
#include "prjectatrrwgt.h"

CTreeWidget::CTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    m_pMastAttrWgt = new CAttrWgt;
    m_pMastAttrWgt->setWindowTitle(tr("Attr-MAST"));
    m_pMastAttrWgt->SetMastName();
    

    m_pFastAttrWgt = new CAttrWgt;
    m_pFastAttrWgt->setWindowTitle(tr("Attr-FAST"));
    m_pFastAttrWgt->SetFastName();

    m_pAuxAttrWgt = new CAttrWgt;
    m_pAuxAttrWgt->setWindowTitle(tr("Attr-AUX0"));
    m_pAuxAttrWgt->SetAuxName();

    m_pProjectWgt = new CPrjectAtrrWgt;
    m_pProjectWgt->setWindowTitle(tr("Attr-Project"));
    connect(m_pProjectWgt, SIGNAL(Signal_Conform()), this, SLOT(Slot_ProjectConform()));

    setColumnCount(1);
    setHeaderLabel(tr("Project"));
    //设置程序items
    CreateProjectItems();
    //增加右击菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

}

CTreeWidget::~CTreeWidget()
{
    delete m_pMastAttrWgt;
    delete m_pFastAttrWgt;
    delete m_pAuxAttrWgt;
    delete m_pProjectWgt;
}

void CTreeWidget::Slot_ContextMenuRequest(const QPoint &pPoint)
{
    if (this->itemAt(pPoint) == nullptr)
    {
        //创建菜单
        QMenu *pMenu = new QMenu(this);

        QAction *pCreateNewAct = new QAction(tr("Create New Project"), this);
        connect(pCreateNewAct, SIGNAL(triggered()),this,SLOT(Slot_CreateNewProject()));

        pMenu->addAction(pCreateNewAct);

        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();
    }
    else
    {
        //根据不同类型创建不同菜单
        QTreeWidgetItem *pCurrentItem = this->itemAt(pPoint);

        if (pCurrentItem->data(0, Qt::UserRole) == MASTSEG || pCurrentItem->data(0, Qt::UserRole) == MASTSRSEG
            || pCurrentItem->data(0, Qt::UserRole) == FASTSEG || pCurrentItem->data(0, Qt::UserRole) == FASTSRSEG
            || pCurrentItem->data(0, Qt::UserRole) == AUX0SEG || pCurrentItem->data(0, Qt::UserRole) == AUX0SRSEG
            || pCurrentItem->data(0, Qt::UserRole) == TIMEEVENT || pCurrentItem->data(0, Qt::UserRole) == IOEVENT)
        {
            //创建子程序
            QMenu *pMenu = new QMenu(this);

            QAction *pCreateNewSegAct = new QAction(tr("Create New %1").arg(pCurrentItem->text(0)), this);
            connect(pCreateNewSegAct, SIGNAL(triggered()), this, SLOT(Slot_CreateNewSeg()));

            pMenu->addAction(pCreateNewSegAct);

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();

        }
        else if (pCurrentItem->data(0, Qt::UserRole) == PROJECT)
        {
            //程序属性
            QMenu *pMenu = new QMenu(this);

            QAction *pAttrAct = new QAction(tr("attribute"), this);
            connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_ProjectAttr()));

            pMenu->addAction(pAttrAct);

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();

        }
        else if (pCurrentItem->data(0, Qt::UserRole) == MAST)
        {
            //
            //m_pMastAttrWgt->show();
            //创建子程序
            QMenu *pMenu = new QMenu(this);

            QAction *pAttrAct = new QAction(tr("attribute"), this);
            connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_MastAttr()));

            pMenu->addAction(pAttrAct);

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();

        }
        else if (pCurrentItem->data(0, Qt::UserRole) == FAST)
        {
            QMenu *pMenu = new QMenu(this);

            QAction *pAttrAct = new QAction(tr("attribute"), this);
            connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_FastAttr()));

            pMenu->addAction(pAttrAct);

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();

        }
        else if (pCurrentItem->data(0, Qt::UserRole) == AUX0)
        {
            QMenu *pMenu = new QMenu(this);

            QAction *pAttrAct = new QAction(tr("attribute"), this);
            connect(pAttrAct, SIGNAL(triggered()), this, SLOT(Slot_AuxAttr()));

            pMenu->addAction(pAttrAct);

            pMenu->exec(QCursor::pos());
            pMenu->deleteLater();

        }
        
    }
}

void CTreeWidget::Slot_CreateNewProject()
{
    //创建新工程

}

void CTreeWidget::CreateProjectItems()
{
    //程序
    QTreeWidgetItem *item0 = new QTreeWidgetItem(this,QStringList(tr("Project")));
    item0->setData(0, Qt::UserRole, PROJECT);
    //任务
    QTreeWidgetItem *item1 = new QTreeWidgetItem(item0, QStringList(tr("Task")));
    item1->setData(0, Qt::UserRole, TASK);
    //mast
    QTreeWidgetItem *item2 = new QTreeWidgetItem(item1, QStringList(tr("MAST")));
    item2->setData(0, Qt::UserRole, MAST);
    //段
    QTreeWidgetItem *item3 = new QTreeWidgetItem(item2, QStringList(tr("Seg")));
    item3->setData(0, Qt::UserRole, MASTSEG);
    //SR 段
    QTreeWidgetItem *item4 = new QTreeWidgetItem(item2, QStringList(tr("SR Seg")));
    item4->setData(0, Qt::UserRole, MASTSRSEG);

    //fast
    QTreeWidgetItem *item5 = new QTreeWidgetItem(item1, QStringList(tr("FAST")));
    item5->setData(0, Qt::UserRole, FAST);
    //段
    QTreeWidgetItem *item6 = new QTreeWidgetItem(item5, QStringList(tr("Seg")));
    item6->setData(0, Qt::UserRole, FASTSEG);
    //SR 段
    QTreeWidgetItem *item7 = new QTreeWidgetItem(item5, QStringList(tr("SR Seg")));
    item7->setData(0, Qt::UserRole, FASTSRSEG);
    //event
    QTreeWidgetItem *item8 = new QTreeWidgetItem(item0, QStringList(tr("Event")));
    item8->setData(0, Qt::UserRole, EVENT);
    //定时器事件
    QTreeWidgetItem *item9 = new QTreeWidgetItem(item8, QStringList(tr("Timer Event")));
    item9->setData(0, Qt::UserRole, TIMEEVENT);
    //I/o事件
    QTreeWidgetItem *item10 = new QTreeWidgetItem(item8, QStringList(tr("I/O Event")));
    item10->setData(0, Qt::UserRole, IOEVENT);
    //aux辅助任务
    QTreeWidgetItem *item11 = new QTreeWidgetItem(item1, QStringList(tr("Aux0")));
    item11->setData(0, Qt::UserRole, AUX0);
    //段
    QTreeWidgetItem *item12 = new QTreeWidgetItem(item11, QStringList(tr("Seg")));
    item12->setData(0, Qt::UserRole, AUX0SEG);
    //SR 段
    QTreeWidgetItem *item13 = new QTreeWidgetItem(item11, QStringList(tr("SR Seg")));
    item13->setData(0, Qt::UserRole, AUX0SRSEG);


}

void CTreeWidget::Slot_CreateNewSeg()
{
    //创建新段
    int nCount = currentItem()->childCount();
    QTreeWidgetItem *item0 = new QTreeWidgetItem(currentItem(), QStringList(currentItem()->parent()->text(0) + currentItem()->text(0).remove(" ") +  QString::number(nCount)));
    item0->setData(0, Qt::UserRole, EDITITEM);
    item0->setFlags(item0->flags()|Qt::ItemIsEditable);
}

void CTreeWidget::Slot_MastAttr()
{
    m_pMastAttrWgt->show();
}

void CTreeWidget::Slot_FastAttr()
{
    m_pFastAttrWgt->show();
}

void CTreeWidget::Slot_AuxAttr()
{
    m_pAuxAttrWgt->show();
}

void CTreeWidget::Slot_ProjectAttr()
{
    m_pProjectWgt->SetprojectName(currentItem()->text(0));
    m_pProjectWgt->exec();
}

void CTreeWidget::Slot_ProjectConform()
{
    currentItem()->setText(0, m_pProjectWgt->GetProjectName());
}
