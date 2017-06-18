#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QMenu>
#include "historywgt.h"
#include "infoconfigwgt.h"
#include "choosepointswgt.h"

CHistoryWgt::CHistoryWgt(CChoosePointsWgt *pPontWgt,QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    Q_ASSERT(pPontWgt);

    if (pPontWgt == nullptr)
    {
        return;
    }
    m_pChooseWgt = pPontWgt;
    ui.tableWidget->setColumnWidth(1,250);
    ui.tableWidget->verticalHeader()->setHidden(true);
    ui.listWidget->setMaximumWidth(200);
    InitFile();
    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Slot_ClickeListItem(QListWidgetItem *)));
    //树结构增加右击菜单
    ui.listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.listWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_listContextMenuRequest(const QPoint &)));

}

CHistoryWgt::~CHistoryWgt()
{

}

void CHistoryWgt::InitFile()
{
    //清空所有数据
    ui.listWidget->clear();
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);
    m_mapFileName.clear();

    QString strRunPath = qApp->applicationDirPath();

    QDir tDir(strRunPath + HISTORYTABLE);

    QStringList lstFiles = tDir.entryList(QDir::Files);

    for (int i = 0; i < lstFiles.count(); i++)
    {
        QString strAbsFile = strRunPath + HISTORYTABLE + lstFiles.at(i);
        m_mapFileName.insert(lstFiles.at(i), strAbsFile);
        ui.listWidget->insertItem(i, lstFiles.at(i));
    }

}

void CHistoryWgt::Slot_ClickeListItem(QListWidgetItem * item)
{
    //
    //清除表格内容
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);
    AnalyseHistoryFile(m_mapFileName[item->text()]);
}

void CHistoryWgt::AnalyseHistoryFile(const QString strFileName)
{
    //解析文件
    QFile tFile(strFileName);

    if (tFile.open(QIODevice::ReadOnly))
    {
        AnalyseData(tFile.readAll());
        tFile.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("file :%1 dose not exist").arg(strFileName));
    }
}

void CHistoryWgt::AnalyseData(const QByteArray byData)
{
    //解析数据 
    if (byData.length() % sizeof(HISTORYALLINFO) != 0 || byData.isEmpty())
    {
        QMessageBox::warning(0, tr("Warning"), tr("Can not Analyse history bin file,file has error"));
    }

    QMap<int, QString> mapinfo = m_pChooseWgt->GetRemoteToDesc();
    for (int i = 0; i < byData.count(); i = i + sizeof(HISTORYALLINFO))
    {
        //
        HISTORYALLINFO historyInfo;
        memcpy((char *)&historyInfo, byData.data() + i, sizeof(HISTORYALLINFO));
        //写入数据
        InsertDataToTable(mapinfo[historyInfo.xxdz.GetAddr()],historyInfo.xxdz.GetAddr(),historyInfo.val,historyInfo.tTime.Dump());
    }
}

void CHistoryWgt::InsertDataToTable(const QString &desctrber, int nAddress, int nValue, const QString &strTime)
{
    int nRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRow);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(QString::number(nRow + 1));

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(desctrber);

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(QString::number(nAddress));

    QTableWidgetItem *item3 = new QTableWidgetItem;
    item3->setText(QString::number(nValue));

    QTableWidgetItem *item4 = new QTableWidgetItem;
    item4->setText(strTime);

    ui.tableWidget->setItem(nRow, 0, item0);
    ui.tableWidget->setItem(nRow, 1, item1);
    ui.tableWidget->setItem(nRow, 2, item2);
    ui.tableWidget->setItem(nRow, 3, item3);
    ui.tableWidget->setItem(nRow, 4, item4);
}

void CHistoryWgt::Slot_listContextMenuRequest(const QPoint &point)
{
    Q_UNUSED(point);
    QMenu *pMenu = new QMenu(ui.listWidget);

    QAction *pUpdateAct = new QAction(tr("Update Conifg"), ui.tableWidget);

    pMenu->addAction(pUpdateAct);

    connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateFileAct()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CHistoryWgt::Slot_UpdateFileAct()
{
    InitFile();
}

