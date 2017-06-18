#include <QHBoxLayout>
#include <QTableWidget>
#include <QDateTime>
#include <QMenu>
#include <QAction>
#include "lcddisplaywgt.h"
#include "lcdoperatorwgt.h"
#include "lcdcontrolwgt.h"
#include "maintecancewgt.h"

CLcdoperatorWgt::CLcdoperatorWgt(QWidget *parent)
    : QWidget(parent)
{
    m_pDiswgt = new CLcdDisplayWgt;
    //m_pDiswgt->setFixedSize(815, 653);
    m_pDiswgt->setFixedWidth(815);

    m_pControlWgt = new CLcdControlWgt;
    
    m_pTableInfo = new QTableWidget;
    m_pTableInfo->setColumnCount(2);
    QStringList lstTableHeader;
    lstTableHeader << tr("Time") << tr("Content");
    m_pTableInfo->setHorizontalHeaderLabels(lstTableHeader);
    m_pTableInfo->setColumnWidth(0, 300);

    m_pTableInfo->horizontalHeader()->setStretchLastSection(true);

    //布局
    QHBoxLayout *pHboxLayout = new QHBoxLayout;
    pHboxLayout->addStretch();
    pHboxLayout->addWidget(m_pControlWgt);
    pHboxLayout->addStretch();

    QVBoxLayout*pTotalLayout = new QVBoxLayout;
    pTotalLayout->addWidget(m_pDiswgt);
    pTotalLayout->addLayout(pHboxLayout);
    pTotalLayout->addStretch();
    pTotalLayout->setStretch(0, 3);
    pTotalLayout->setStretch(1, 1);

    QHBoxLayout *pWgtLayout = new QHBoxLayout(this);
    pWgtLayout->addLayout(pTotalLayout);
    pWgtLayout->addWidget(m_pTableInfo);

    m_pTableInfo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTableInfo, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

    //AnalyseLcdRecvInfo("fdsafdsafdas\njfiaoljfdlas\nfjdafjasjldsaf");

}

CLcdoperatorWgt::~CLcdoperatorWgt()
{

}


void CLcdoperatorWgt::WriteTableLog(const QString &strContext)
{
    int nRow = m_pTableInfo->rowCount();
    m_pTableInfo->insertRow(nRow);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    m_pTableInfo->setItem(nRow, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(strContext);
    m_pTableInfo->setItem(nRow, 1, item1);

    if (nRow % 2 != 0)
    {
        item0->setBackgroundColor(QColor(230, 231, 232));
        item1->setBackgroundColor(QColor(230, 231, 232));
    }

    m_pTableInfo->scrollToBottom();
}

void CLcdoperatorWgt::Slot_ContextMenuRequest(const QPoint &tPoint)
{
    Q_UNUSED(tPoint);

    QMenu *pMenu = new QMenu(m_pTableInfo);

    QAction *pClearAct = new QAction(tr("Clear"),m_pTableInfo);
    connect(pClearAct, SIGNAL(triggered()), this, SLOT(Slot_ClearAct()));

    pMenu->addAction(pClearAct);

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CLcdoperatorWgt::Slot_ClearAct()
{
    m_pTableInfo->clearContents();
    m_pTableInfo->setRowCount(0);
}

void CLcdoperatorWgt::AnalyseLcdRecvInfo(const QByteArray & btData)
{
    LCDCONTROLINFO lcdTotalInfo;
    if (btData.count() != sizeof(LCDCONTROLINFO))
    {
        return;
    }

    memcpy((char *)&lcdTotalInfo, btData.data(), sizeof(LCDCONTROLINFO));
    //更新日志
    //QString strData = QString::fromLocal8Bit((char *)lcdTotalInfo.ledData, 240);
    m_pDiswgt->UpdateLcdDisplayData(QByteArray((char *)lcdTotalInfo.ledData, 240), lcdTotalInfo.cursorRow, lcdTotalInfo.cursorColumn);
}

CLcdControlWgt * CLcdoperatorWgt::GetControl()
{
    return m_pControlWgt;
}

void CLcdoperatorWgt::AnalyseLEDRecvInfo(const QByteArray &BtData)
{
//  
//      memcpy((char *)&ledTotalInfo, BtData.data(), sizeof(LEDLIGHTINFO));

    if (BtData.length() != 90)
    {
        return;
    }
    unsigned char m_ucLedBuffer[256];
    memcpy(m_ucLedBuffer, BtData.data() + 4, BtData.length() - 4);
 
     //根据液晶数据显示LED
     //面板灯ID号为0~11，表示12个LED灯
     int i = 0;

     for (i = 0; i < 8; i++)
     {
         SedLEDLight(i, (m_ucLedBuffer[0] >> i) & 0x01);
     }

     for (i = 0; i < 8; i++)
     {
         SedLEDLight(i + 8, (m_ucLedBuffer[1] >> i) & 0x01);
     }

//      //灯数据
//      if (ledTotalInfo.FirstLights.ONE == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(0,4);
//      }
//      else if (ledTotalInfo.FirstLights.TWO == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(1, 4);
//      }
//      else if (ledTotalInfo.FirstLights.THREE == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(2, 4);
//      }
//      else if (ledTotalInfo.FirstLights.FOUR == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(3, 4);
//      }
//      else if (ledTotalInfo.FirstLights.FIVE == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(4, 4);
//      }
//      else if (ledTotalInfo.FirstLights.SIX == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(5, 4);
//      }
//      else if (ledTotalInfo.FirstLights.SEVEN == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(6, 4);
//      }
//      else if (ledTotalInfo.FirstLights.EIGHT == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(7, 4);
//      }
//  
//  
//      if (ledTotalInfo.SecondLights.ONE == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(8, 4);
//      }
//      else if (ledTotalInfo.SecondLights.TWO == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(9, 4);
//      }
//      else if (ledTotalInfo.SecondLights.THREE == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(10, 4);
//      }
//      else if (ledTotalInfo.SecondLights.FOUR == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(11, 4);
//      }
//      else if (ledTotalInfo.SecondLights.FIVE == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(12, 4);
//      }
//      else if (ledTotalInfo.SecondLights.SIX == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(13, 4);
//      }
//      else if (ledTotalInfo.SecondLights.SEVEN == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(14, 4);
//      }
//      else if (ledTotalInfo.SecondLights.EIGHT == 1)
//      {
//          m_pDiswgt->UpdateLEDLight(15, 4);
//      }
 
     m_pDiswgt->update();

}

void CLcdoperatorWgt::SedLEDLight(int nLedID, int nOnOff)
{
    if (nOnOff == 0)
    {
        m_pDiswgt->UpdateLEDLight(nLedID, 1);
    }
    else
    {
        m_pDiswgt->UpdateLEDLight(nLedID, 4);
    }
    
}

