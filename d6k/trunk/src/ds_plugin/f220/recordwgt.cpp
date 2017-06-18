#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFile>
#include <QMenu>
#include <QDir>
#include <QProcess>
#include <QFileDialog>
#include "recordwgt.h"
#include "commthread.h"
#include "devicestudio/main_module.h"

CRecordWgt::CRecordWgt(CCommThread *pCommunicate, IMainModule *pCore, const QString &deviceName,QWidget *parent)
    : QWidget(parent),
    m_strDeviceName(deviceName)
{
    ui.setupUi(this);

    Q_ASSERT(pCommunicate);
    if (pCommunicate == nullptr)
    {
        return;
    }
    m_pMainModule = pCore;
    m_pCommunicate = pCommunicate;

    m_pRecordProcess = new QProcess(this);

    ui.tableWidget->setSortingEnabled(true);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->verticalHeader()->setHidden(true);
    //右击菜单
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
    //点击树item
    connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_ClickTreeItem(QTreeWidgetItem *, int)));
    //树结构增加右击菜单
    ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_treeContextMenuRequest(const QPoint &)));
    //暂时隐藏下载页面
    ui.tabWidget->removeTab(0);
    ui.tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //
    connect(ui.tableWidget_2, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_DoubleClickFileList(QTableWidgetItem *)));
    ui.tableWidget_2->setColumnWidth(0, 400);
    ui.tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_TableContexeMenuRequest(const QPoint &)));
}

CRecordWgt::~CRecordWgt()
{

}

bool CRecordWgt::AnalyseRecordFile(const QString &strFilename)
{
    if (strFilename.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("read record file is null,please check!"));
        return false;
    }
    //清空原有数据
    ui.treeWidget->clear();
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);

    QFile fPluginFile(strFilename);
    if (fPluginFile.open(QFile::ReadOnly | QFile::Text))
    {
        if (!AnalyseXmlData(&fPluginFile))
        {
            fPluginFile.close();
            return false;
        }
        
        fPluginFile.close();
    }
    else
    {
        //QMessageBox::warning(this, tr("warning"), tr("open file %1 error!").arg(strFilename));
        return false;
    }

    return true;
}

bool CRecordWgt::AnalyseXmlData(QIODevice *pDevice)
{
    QXmlStreamReader xmlReader(pDevice);

    int nTreeIndex = 0;
    while (!xmlReader.atEnd() && (!xmlReader.hasError()))
    {
        //遍历所有配置
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            //qDebug() << "xmlReader:"<<xmlReader.name().toString();
            if (xmlReader.name().toString() == "catalog")
            {
                int nIndex = xmlReader.attributes().value("index").toInt();
                QString strName = xmlReader.attributes().value("name").toString();

                InserDataToTree(nIndex, strName, nTreeIndex);
                nTreeIndex++;
                AnalyseItemData(xmlReader, nIndex, strName);
            }
        }
    }
    return true;
}

void CRecordWgt::AnalyseItemData(QXmlStreamReader &xmlReader,int nIndex,const QString &strPath)
{
    //组装数据
    RECORINFO recordInfo;
    recordInfo.nIndex = nIndex;
    recordInfo.strName = strPath;

    QList<RECORDITEM> lstItems;

    while (!xmlReader.atEnd() && (!xmlReader.hasError()))
    {
        //遍历
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "fileItem")
            {
                RECORDITEM itemInfo;
                itemInfo.nFileIndex = xmlReader.attributes().value("index").toInt();
                itemInfo.strFileName = xmlReader.attributes().value("fileName").toString();
                itemInfo.strDestriber = xmlReader.attributes().value("comment").toString();
                lstItems.append(itemInfo);
            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "catalog")
            {
                //一条完整的记录
                recordInfo.lstItems = lstItems;
                m_mapIndexFileInfo.insert(nIndex, recordInfo);
                return;

            }
        }
    }
}

void CRecordWgt::InserDataToTree(int nIndex, const QString &strCatalog, int nTreeIndex)
{
    QTreeWidgetItem *item0 = new QTreeWidgetItem;
    item0->setText(0, QString::number(nIndex));
    item0->setText(1, strCatalog);
    ui.treeWidget->insertTopLevelItem(nTreeIndex, item0);
}

void CRecordWgt::Slot_ClickTreeItem(QTreeWidgetItem *item, int nColumn)
{
    Q_UNUSED(nColumn);
    //清空表格
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);

    //插入数据
    for (int i = 0; i < m_mapIndexFileInfo[item->text(0).toInt()].lstItems.count(); i++)
    {
        ui.tableWidget->insertRow(i);
        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText(QString::number(m_mapIndexFileInfo[item->text(0).toInt()].lstItems.at(i).nFileIndex));
        ui.tableWidget->setItem(i, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setText(m_mapIndexFileInfo[item->text(0).toInt()].lstItems.at(i).strFileName);
        ui.tableWidget->setItem(i, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setText(m_mapIndexFileInfo[item->text(0).toInt()].lstItems.at(i).strDestriber);
        ui.tableWidget->setItem(i, 2, item2);
    }
}

void CRecordWgt::Slot_ContextMenuRequest(const QPoint &point)
{
    if (ui.tableWidget->itemAt(point) == nullptr)
    {
        return;
    }

    QMenu *pMenu = new QMenu(ui.tableWidget);

    QAction *pDownAct = new QAction(tr("DownFile"), ui.tableWidget);

    pMenu->addAction(pDownAct);

    connect(pDownAct, SIGNAL(triggered()), this, SLOT(Slot_DownFileAct()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CRecordWgt::Slot_DownFileAct()
{
    //发送下载请求
    QList<LB_DATA> lstLbData;
    
    QList<QTableWidgetSelectionRange>ranges = ui.tableWidget->selectedRanges();

    int count = ranges.count();

    for (int i = 0; i < count; i++)
    {
        int topRow = ranges.at(i).topRow();

        int bottomRow = ranges.at(i).bottomRow();

        for (int j = topRow; j <= bottomRow; j++)
        {
            LB_DATA lbItem;
            lbItem.m_bValidFlag = true;
            lbItem.m_nCatalogIndex = ui.treeWidget->currentItem()->text(0).toInt();
            lbItem.m_nFilenameIndex = ui.tableWidget->item(j, 0)->text().toInt();
            lbItem.m_strFileName = ui.tableWidget->item(j, 1)->text();
            lstLbData.append(lbItem);
        }
    }

    m_pCommunicate->SendRecordOrder(lstLbData);
}

void CRecordWgt::Slot_treeContextMenuRequest(const QPoint &point)
{
    Q_UNUSED(point);
    QMenu *pMenu = new QMenu(ui.treeWidget);

    QAction *pUpdateAct = new QAction(tr("Update Conifg"), ui.tableWidget);

    pMenu->addAction(pUpdateAct);

    connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateFileAct()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();

}

void CRecordWgt::Slot_UpdateFileAct()
{
    m_mapIndexFileInfo.clear();
    AnalyseRecordFile(m_strRecordConfigFile);

}

void CRecordWgt::SetRecoredConifgName(const QString &strFileName)
{
    m_strRecordConfigFile = strFileName;
}

void CRecordWgt::SetRecordPath(const QString &strPath)
{
    m_strRecordFilePath = strPath;
    UpdateRecordFileList();
}

void CRecordWgt::UpdateRecordFileList()
{
    ui.tableWidget_2->clearContents();
    ui.tableWidget_2->setRowCount(0);
    //遍历文件 目录
    //查找所有cfg文件
    QDir tDir(m_strRecordFilePath);
    QFileInfoList lstFiles = tDir.entryInfoList(QDir::Files);

    for (int i = 0; i < lstFiles.count(); i++)
    {
        if (lstFiles.at(i).fileName().endsWith(".cfg"))
        {
            QTableWidgetItem *item0 = new QTableWidgetItem(lstFiles.at(i).fileName());
            QTableWidgetItem *item1 = new QTableWidgetItem(lstFiles.at(i).created().toString("yyyy-MM-dd hh:mm:ss"));
            ui.tableWidget_2->insertRow(0);
            ui.tableWidget_2->setItem(0, 0, item0);
            ui.tableWidget_2->setItem(0, 1, item1);
        }
    }
}

void CRecordWgt::Slot_DoubleClickFileList(QTableWidgetItem *pItem)
{
    QFileDialog pFileDialog(this);

    pFileDialog.setWindowModality(Qt::WindowModal);
    pFileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (pFileDialog.exec() != QDialog::Accepted)
    {
        return;
    }
    QString strFileName = pFileDialog.selectedFiles().first();

    QString strRecordFile = m_strRecordFilePath + "/" + ui.tableWidget_2->item(pItem->row(), 0)->text();

    m_pRecordProcess->start(strFileName, QStringList(strRecordFile));
}

void CRecordWgt::Slot_TableContexeMenuRequest(const QPoint &point)
{
    Q_UNUSED(point);
    QMenu *pMenu = new QMenu(ui.tableWidget_2);

    QAction *pUpdateAct = new QAction(tr("Update File"), ui.tableWidget_2);

    pMenu->addAction(pUpdateAct);

    connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateRecordFileAct()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CRecordWgt::Slot_UpdateRecordFileAct()
{
    UpdateRecordFileList();
}
