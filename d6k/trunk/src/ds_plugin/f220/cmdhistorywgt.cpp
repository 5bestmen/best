#include <QDir>
#include <QFile>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QTextStream>
#include "cmdhistorywgt.h"
#include "infoconfigwgt.h"

CCmdHistoryWgt::CCmdHistoryWgt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.textEdit->setReadOnly(true);
    ui.splitter->setStretchFactor(0, 20);
    ui.splitter->setStretchFactor(1, 80);

    InitHistory();

    connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Slot_ClickeListItem(QListWidgetItem *)));
    //树结构增加右击菜单
    ui.listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.listWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_listContextMenuRequest(const QPoint &)));

}

CCmdHistoryWgt::~CCmdHistoryWgt()
{

}

void CCmdHistoryWgt::InitHistory()
{
    ui.textEdit->clear();
    ui.listWidget->clear();
    m_mapFileName.clear();

    QString strRunPath = qApp->applicationDirPath();

    QDir tDir(strRunPath + CMDHISTORYPATH);

    QStringList lstFiles = tDir.entryList(QDir::Files);

    for (int i = 0; i < lstFiles.count(); i++)
    {
        QString strAbsFile = strRunPath + CMDHISTORYPATH + lstFiles.at(i);
        m_mapFileName.insert(lstFiles.at(i), strAbsFile);
        ui.listWidget->insertItem(i, lstFiles.at(i));
    }
}

void CCmdHistoryWgt::Slot_ClickeListItem(QListWidgetItem *lstItem)
{
    ui.textEdit->clear();

    //显示文件
    QString strFileName = m_mapFileName[lstItem->text()];
    QFile tFile(strFileName);
    if (tFile.open(QIODevice::ReadOnly | QFile::Text))
    {
        QTextStream in(&tFile);
        ui.textEdit->setPlainText(in.readAll());
        tFile.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("file :%1 dose not exist").arg(strFileName));
    }
}

void CCmdHistoryWgt::Slot_listContextMenuRequest(const QPoint &pfPoint)
{
    Q_UNUSED(pfPoint);

    QMenu *pMenu = new QMenu(ui.listWidget);

    QAction *pUpdateAct = new QAction(tr("Update Conifg"), ui.listWidget);

    pMenu->addAction(pUpdateAct);

    connect(pUpdateAct, SIGNAL(triggered()), this, SLOT(Slot_UpdateFileAct()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CCmdHistoryWgt::Slot_UpdateFileAct()
{
    InitHistory();
}
