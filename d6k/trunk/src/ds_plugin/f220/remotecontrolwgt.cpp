#include <QStandardItemModel>
#include <QDateTime>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QTimer>
#include "remoteponittableanalyse.h"
#include "remotecontrolwgt.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"



CRemoteControlWgt::CRemoteControlWgt(CCommThread *pCommunicate, QWidget *parent, CInfoConfigWgt *pConfgWgt)
	: QWidget(parent),
	m_pTableModel(new QStandardItemModel(this)),
	m_pTimeOut(new QTimer(this))
{
	ui.setupUi(this);
	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}

	m_pCommThread = pCommunicate;
    m_pConfgWgt = pConfgWgt;
	InitWgt();
	connect(ui.pbtConform,SIGNAL(clicked()),this,SLOT(Slot_ClickConform()));
	//超时
	connect(m_pTimeOut, SIGNAL(timeout()), this, SLOT(Slot_WaitForResponseTimeOut()));
    //
    connect(ui.comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(Slot_ComboboxItemChange(const QString &)));
}

void CRemoteControlWgt::InitWgt()
{
	
	QStringList lstLables;
	lstLables << tr("NUM.") << tr("DateTime") << tr("Control Type") << tr("Point Num")<<tr("Operate Type")<<tr("Status");
	m_pTableModel->setColumnCount(6);
	m_pTableModel->setHorizontalHeaderLabels(lstLables);

	ui.tableViewInfo->setModel(m_pTableModel);
	ui.tableViewInfo->verticalHeader()->setHidden(true);
	ui.tableViewInfo->setSortingEnabled(true);
	ui.tableViewInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableViewInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableViewInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//增加右击菜单
	m_pMTable = new QMenu(ui.tableViewInfo);
	ui.tableViewInfo->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableViewInfo, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

	QAction *pActClear = m_pMTable->addAction(tr("Clear"));
	connect(pActClear, SIGNAL(triggered()), this, SLOT(Slot_clearAction()));
}
//点击确定
void CRemoteControlWgt::Slot_ClickConform()
{
	//安全性检测
	int iPointNum = ui.lineEditPonitNum->text().toInt();
	if (iPointNum < 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Point Number input Error"));
		return;
	}

    m_pCommThread->SendRemoteControl(ui.comboControlType->currentIndex(), ui.lineEditPonitNum->text().toInt() + m_pConfgWgt->GetControlStart(), ui.comboOpType->currentIndex(),1);

    InsertDataToTable(ui.comboControlType->currentText(), QString::number(ui.lineEditPonitNum->text().toInt() + m_pConfgWgt->GetControlStart()), ui.comboOpType->currentText());
	//五秒没反馈认为超时
	m_pTimeOut->start(5000*30);
}

//table右击菜单
void CRemoteControlWgt::Slot_ContextMenuRequest(const QPoint &cPointCurrent)
{
	Q_UNUSED(cPointCurrent);
	m_pMTable->exec(QCursor::pos());
}

//clear action
void CRemoteControlWgt::Slot_clearAction()
{
	m_pTableModel->removeRows(0, m_pTableModel->rowCount());
}

//timeOut
void CRemoteControlWgt::Slot_WaitForResponseTimeOut()
{
	m_pTimeOut->stop();
    QMessageBox::warning(this, tr("Error"), tr("Control Operate Error,Wait For Respond Timeout!"));
}

//往表格中插入数据
void CRemoteControlWgt::InsertDataToTable(const QString &strContrTpe, const QString &strPointNum, const QString &operateType)
{
	int iCurrentRow = m_pTableModel->rowCount() + 1;
	m_pTableModel->insertRow(0);
	m_pTableModel->setItem(0, 0,new QStandardItem(QString::number(iCurrentRow)));
	m_pTableModel->setItem(0, 1, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
	m_pTableModel->setItem(0, 2, new QStandardItem(strContrTpe));
	m_pTableModel->setItem(0, 3, new QStandardItem(strPointNum));
	m_pTableModel->setItem(0, 4, new QStandardItem(operateType));
	m_pTableModel->setItem(0, 5, new QStandardItem(tr("Preset Request")));
	//改编样式
	m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(170, 255, 127)));
	m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(170, 255, 127)));
	m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(170, 255, 127)));
	m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(170, 255, 127)));
	m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(170, 255, 127)));
	m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(170, 255, 127)));
}

//反馈信息插入表格
void CRemoteControlWgt::InsertDataFromFeedBack(int iControlType, int iPointNum, int iOperateType, const QString &strStatus)
{
	int iCurrentRow = m_pTableModel->rowCount() + 1;
	m_pTableModel->insertRow(0);
	m_pTableModel->setItem(0, 0, new QStandardItem(QString::number(iCurrentRow)));
	m_pTableModel->setItem(0, 1, new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")));
	if (iControlType == 0)
	{
		m_pTableModel->setItem(0, 2, new QStandardItem(tr("Single")));
	}
	else if (iControlType == 1)
	{
		m_pTableModel->setItem(0, 2, new QStandardItem(tr("Double")));
	}
	else
	{
		m_pTableModel->setItem(0, 2, new QStandardItem(tr("UnKnown")));
	}

	
	m_pTableModel->setItem(0, 3, new QStandardItem(QString::number(iPointNum)));

	if (iOperateType == 0)
	{
		m_pTableModel->setItem(0, 4, new QStandardItem(tr("On")));
	}
	else if (iOperateType == 1)
	{
		m_pTableModel->setItem(0, 4, new QStandardItem(tr("Off")));
	}
	else
	{
		m_pTableModel->setItem(0, 4, new QStandardItem(tr("Unknown")));
	}
	
	

	if (strStatus == "Preset Response")
	{
		m_pTableModel->setItem(0, 5, new QStandardItem(tr("Preset Success")));
		//改编样式
		m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(170, 255, 255)));
        m_pTimeOut->stop();
        int messReturn = QMessageBox::information(this, tr("Success"), tr("Control Preset Success,,Go on Exec Command ?"),QMessageBox::Yes,QMessageBox::No);
        if (messReturn == QMessageBox::Yes)
        {
            m_pTimeOut->start();
//             m_pTableModel->setItem(0, 5, new QStandardItem(tr("Exec Request")));
//             //改编样式
//             m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(170, 255, 127)));
//             m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(170, 255, 127)));
//             m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(170, 255, 127)));
//             m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(170, 255, 127)));
//             m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(170, 255, 127)));
//             m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(170, 255, 127)));

            //发送执行
            m_pCommThread->SendRemoteControl(ui.comboControlType->currentIndex(), ui.lineEditPonitNum->text().toInt() + m_pConfgWgt->GetControlStart(), ui.comboOpType->currentIndex(),2);
        }
	}
	else if (strStatus == "Exec Request")
	{
		m_pTableModel->setItem(0, 5, new QStandardItem(tr("Exec Request")));
		//改编样式
		m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(170, 255, 127)));
		m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(170, 255, 127)));
		m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(170, 255, 127)));
		m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(170, 255, 127)));
		m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(170, 255, 127)));
		m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(170, 255, 127)));

	}
	else if (strStatus == "Exec Response")
	{
		m_pTableModel->setItem(0, 5, new QStandardItem(tr("Exec Success!")));
		//改编样式
		m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(170, 255, 255)));
		m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(170, 255, 255)));
		//当收到执行成功响应后认为 遥控成功
		m_pTimeOut->stop();
		
	}

    else
	{
        m_pTimeOut->stop();

        QString strErrorInfo = tr("operator error");

        if (strStatus == "10")
        {
            strErrorInfo = tr("Activation is terminated!");

            m_pTableModel->setItem(0, 5, new QStandardItem(strErrorInfo));
            //改编样式
            m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(170, 255, 255)));
            m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(170, 255, 255)));
            m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(170, 255, 255)));
            m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(170, 255, 255)));
            m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(170, 255, 255)));
            m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(170, 255, 255)));

        }
        else
        {
            if (strStatus == "44")
            {
                strErrorInfo = tr("Unknown type identification!");
            }
            else if (strStatus == "45")
            {
                strErrorInfo = tr("Unknown drive reason!");
            }
            else if (strStatus == "46")
            {
                strErrorInfo = tr("Unknown application service data unit public address!");
            }
            else if (strStatus == "47")
            {
                strErrorInfo = tr("Unknown information object address!");
            }

            m_pTableModel->setItem(0, 5, new QStandardItem(strErrorInfo));
            //改编样式
            m_pTableModel->item(0, 0)->setBackground(QBrush(QColor(255, 0, 0)));
            m_pTableModel->item(0, 1)->setBackground(QBrush(QColor(255, 0, 0)));
            m_pTableModel->item(0, 2)->setBackground(QBrush(QColor(255, 0, 0)));
            m_pTableModel->item(0, 3)->setBackground(QBrush(QColor(255, 0, 0)));
            m_pTableModel->item(0, 4)->setBackground(QBrush(QColor(255, 0, 0)));
            m_pTableModel->item(0, 5)->setBackground(QBrush(QColor(255, 0, 0)));

            QMessageBox::warning(this, tr("Warning"), strErrorInfo);
        }

	}
}

CRemoteControlWgt::~CRemoteControlWgt()
{

}

void CRemoteControlWgt::SetRemoteTableInfo(CPointInfo *pRemoteAnalyse)
{
    m_mapDescPoint.clear();
    ui.comboBox->clear();
    ui.lineEditPonitNum->clear();

    m_pRemoteAnalyse = pRemoteAnalyse;

    //填写数据
    QMap<unsigned int, RPT> mapControls = m_pRemoteAnalyse->GetControlMap();

    QMap<unsigned int, RPT>::const_iterator it = mapControls.constBegin();

    while (it != mapControls.constEnd())
    {
        m_mapDescPoint.insert(it.value().Destriber, it.key());

        ui.comboBox->addItem(it.value().Destriber);

        it++;
    }

//     for each (RPT control in lstControls)
//     {
//         //
//         QString strItem = control.Destriber;
//         ui.comboBox->addItem(strItem);
//         //记录数据
//         m_mapDescPoint.insert(strItem,control.NUM2);
//     }

    ui.lineEditPonitNum->setText(QString::number(m_mapDescPoint[ui.comboBox->currentText()]));
}

void CRemoteControlWgt::Slot_ComboboxItemChange(const QString &text)
{
    ui.lineEditPonitNum->setText(QString::number(m_mapDescPoint[text]));
}
