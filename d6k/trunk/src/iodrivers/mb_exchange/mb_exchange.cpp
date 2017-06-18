#include <QTextEdit>
#include <string>
#include <QTreeWidget>
#include <QDateTime>
#include <QMenu>
#include <QContextMenuEvent>
#include "mb_exchange.h"

#include "socket_client.h"
#include "socketmgr.h"
#include "socket_client.h"
#include "handle_thread.h"

CMb_Exchange::CMb_Exchange(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pReadXML = nullptr;
	m_pSocketMgr = nullptr;
	m_nDeviceID = 0;

	//handlethread对象及界面初始化
	Init();	
	//界面显示信息初始化
    InitPointTableData();

}

CMb_Exchange::~CMb_Exchange()
{
	if (m_pSocketMgr != nullptr)
	{
		delete m_pSocketMgr;
		m_pSocketMgr = nullptr;
	}

	if (m_pReadXML != nullptr)
	{
		delete m_pReadXML;
		m_pReadXML = nullptr;
	}
}

//初始化handlethread对象
bool CMb_Exchange::Init()
{
	if (!LoadCfg())
	{
		return false;
	}
	Q_ASSERT(m_pReadXML);

	for each (auto i in m_pReadXML->m_mapDevice)
	{
		CHandleThread *pHandelThread = new CHandleThread(i.strIP, i.Port, i.Device_id);
		pHandelThread->start();
		connect(pHandelThread, SIGNAL(Signal_ShowDiValue(int, int, int, int, QString)), this, SLOT(Slot_UpdateRecvDiData(int, int, int, int, QString)));
		connect(pHandelThread, SIGNAL(Signal_ShowAiValue(int, int, int, int, QString)), this, SLOT(Slot_UpdateRecvAiData(int, int, int, int, QString)));
		connect(pHandelThread, SIGNAL(Signal_ShowKwhValue(int, int, int, int, QString)), this, SLOT(Slot_UpdateRecvKwhData(int, int, int, int, QString)));

		connect(pHandelThread, SIGNAL(Signal_ConnectSuccess(int, const QString &)), this, SLOT(Slot_ConnectSuccess(int, const QString &)));
		connect(pHandelThread, SIGNAL(Signal_SocketError(int, const QString &, const QString &)), this, SLOT(Slot_SocketError(int, const QString &, const QString &)));
		connect(this, SIGNAL(Signal_CloseSocket(int)), pHandelThread, SLOT(Slot_CloseSocket(int)));
		connect(this, SIGNAL(Signal_ReconSocket(int, QString, int)), pHandelThread, SLOT(Slot_ReconSocket(int, QString, int)));

		connect(this, SIGNAL(Signal_TransBinary(int, int, int, int)), pHandelThread, SLOT(Slot_TransBinary(int, int, int, int)));
		connect(this, SIGNAL(Signal_TransAnalog(int, int, int, int)), pHandelThread, SLOT(Slot_TransAnalog(int, int, int, int)));
	}

	ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuEvent(const QPoint &)));

	//创建根节点信息
	MakeTreeInfo();

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CMb_Exchange::InitPointTableData
** \details 初始化表格中显示的点信息
** \return void
** \author HuangYuqing
** \date   2016年9月24日
** \note 
********************************************************************************************************/
void CMb_Exchange::InitPointTableData()
{

	QMap<int, QVector<SDiInfo> > mapDiInfo = m_pReadXML->GetBinaryInfo();

	QMap<int, QVector<SDiInfo> >::const_iterator it = mapDiInfo.constBegin();
	QMap<int, BINARYSTRUCT> tMapSingleDIInfo;
	while (it != mapDiInfo.constEnd())
	{

		QVector<SDiInfo>::const_iterator diIt = it.value().constBegin();

		while (diIt != it.value().constEnd())
		{
			BINARYSTRUCT binaryData;
			binaryData.StationID = 1;
			binaryData.DeviceID = it.key();

			binaryData.DiID = diIt->nDiID;
			binaryData.DiName = diIt->strDiName;
			binaryData.realData = -1;

			if (m_mapBinaryDevice.contains(it.key()))
			{
				m_mapBinaryDevice[it.key()].insert(diIt->nDiID, binaryData);
			}
			else
			{			
				tMapSingleDIInfo.insert(diIt->nDiID, binaryData);
				m_mapBinaryDevice.insert(it.key(), tMapSingleDIInfo);
			}

			diIt++;
		}

		it++;
	}


}

bool CMb_Exchange::LoadCfg()
{
	m_pReadXML = new CReadXML();

	m_pReadXML->ReadXML(CONFIG_NAME);

	return true;
}


void CMb_Exchange::MakeTreeInfo()
{
	QMap<int, SDeviceInfo> mapDeviceInfo = m_pReadXML->GetDeviceInfo();

	QMap<int, SDeviceInfo>::const_iterator it = mapDeviceInfo.constBegin();

	while (it != mapDeviceInfo.constEnd())
	{
		QTreeWidgetItem *item0 = new QTreeWidgetItem;
		item0->setText(0, it->Device_name);
		item0->setData(0, 256, it->Device_id);
		ui.treeWidget->addTopLevelItem(item0);
	
		it++;
	}

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(Slot_ClickTreeItem(QTreeWidgetItem *, int)));
}

void CMb_Exchange::Slot_ClickTreeItem(QTreeWidgetItem *pItem, int nColumn)
{
	m_nDeviceID = pItem->data(0, 256).toInt();//256保存设备名
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);

	ui.tableWidget_3->clearContents();
	ui.tableWidget_3->setRowCount(0);

	ui.tableWidget_4->clearContents();
	ui.tableWidget_4->setRowCount(0);

	m_mapDevDiRow.clear();
	m_mapDevAiRow.clear();
	m_mapDevKwhRow.clear();

}

//更新遥信点数据槽函数
void CMb_Exchange::Slot_UpdateRecvDiData(int StationID, int DeviceID, int DiID, int realData, QString DiName)
{
	if (m_nDeviceID == DeviceID)
	{
		if (m_mapDevDiRow.contains(DiID))
		{
            //条件检测
            QVector<SDiInfo> vDiInfo = m_pReadXML->GetBinaryInfo()[DeviceID];
            QVector<SDiInfo>::const_iterator itDi = vDiInfo.constBegin();

			while (itDi != vDiInfo.constEnd())
			{
				if (itDi->nDiID == DiID && itDi->nTransType != 0)
				{
					//检测
					//条件
					const QVector<SConditionInfo>& vCond = itDi->transInfo.s_vecCondition;
					//目标点
					const QVector<STargetInfo>& vTargetInfo = itDi->transInfo.s_vecTarget;

					//根据m_mapDevDiRow的值来判断是否需要转发
					QVector<SConditionInfo>::const_iterator itCond = vCond.constBegin();
					//判断有几个转发目标点
					QVector<STargetInfo>::const_iterator itTarget = vTargetInfo.constBegin();

					m_mapBinaryDevice[DeviceID][DiID].realData = realData;

					bool bCondFlag = false;
					while (itCond != vCond.end())
					{
						if (m_mapBinaryDevice[itCond->Condition_device_id][itCond->Condition_id].realData == itCond->Condition_flag)
						{
							bCondFlag = true;
						}
						else
						{
							bCondFlag = false;
							break;
						}
						itCond++;
					}

					if (!bCondFlag)
					{
						break;
					}

					if (itDi->nTransType == 1)
					{
						//转发遥控
						while (itTarget != vTargetInfo.end())
						{
							emit Signal_TransBinary(itTarget->Target_station_id, itTarget->Targert_device_id, itTarget->Target_id, realData);
							itTarget++;
						}


					}
					else if (itDi->nTransType == 2)
					{
						//转发遥调
						while (itTarget != vTargetInfo.end())
						{
							emit Signal_TransAnalog(itTarget->Target_station_id, itTarget->Targert_device_id, itTarget->Target_id, realData);

							itTarget++;
						}

					}
					else
					{
						break;
					}

					break;
				}
				itDi++;
			}

			ui.tableWidget->item(m_mapDevDiRow[DiID], 3)->setText(QString::number(realData));
		}
		else
		{
			int nRownum = ui.tableWidget->rowCount();
			m_mapDevDiRow.insert(DiID, nRownum);

            //要往m_mapBinaryDevice里插入数据
			BINARYSTRUCT binaryData;
			binaryData.StationID = StationID;
			binaryData.DeviceID = DeviceID;
			binaryData.DiID = DiID;
			binaryData.DiName = DiName;
			binaryData.realData = realData;
			if (m_mapBinaryDevice.contains(DeviceID))
			{
				m_mapBinaryDevice[DeviceID].insert(DiID, binaryData);
			}
			else
			{
				QMap<int, BINARYSTRUCT> tMapSingleDIInfo;
				tMapSingleDIInfo.insert(DiID, binaryData);
				m_mapBinaryDevice.insert(DeviceID, tMapSingleDIInfo);
			}


			ui.tableWidget->insertRow(nRownum);

			QTableWidgetItem *item0 = new QTableWidgetItem;
			item0->setText(QString::number(DiID));
			ui.tableWidget->setItem(nRownum, 0, item0);

			QTableWidgetItem *item1 = new QTableWidgetItem;
			item1->setText(QString::number(StationID));
			ui.tableWidget->setItem(nRownum, 1, item1);

			QTableWidgetItem *item2 = new QTableWidgetItem;
			item2->setText(QString::number(DeviceID));
			ui.tableWidget->setItem(nRownum, 2, item2);

			QTableWidgetItem *item3 = new QTableWidgetItem;
			item3->setText(QString::number(realData));
			ui.tableWidget->setItem(nRownum, 3, item3);

			QTableWidgetItem *item4 = new QTableWidgetItem;
			item4->setText(QString(DiName));
			ui.tableWidget->setItem(nRownum, 4, item4);

		}

	}
}

//更新遥测点数据槽函数
void CMb_Exchange::Slot_UpdateRecvAiData(int StationID, int DeviceID, int AiID, int realData, QString AiName)
{
	if (m_nDeviceID == DeviceID)
	{
		if (m_mapDevAiRow.contains(AiID))
		{
			//条件检测
			QVector<SAiInfo> vAiInfo = m_pReadXML->GetAnalogInfo()[DeviceID];
			QVector<SAiInfo>::const_iterator itAi = vAiInfo.constBegin();

			while (itAi != vAiInfo.constEnd())
			{
				if (itAi->nAiID == AiID && itAi->nTransType != 0)
				{
					//检测
					//条件
					QVector<SConditionInfo> vCond = itAi->transInfo.s_vecCondition;
					//目标点
					QVector<STargetInfo> vTargetInfo = itAi->transInfo.s_vecTarget;

					//根据m_mapDevDiRow的值来判断是否需要转发
					QVector<SConditionInfo>::const_iterator itCond = vCond.constBegin();
					//判断有几个转发目标点
					QVector<STargetInfo>::const_iterator itTarget = vTargetInfo.constBegin();

					m_mapBinaryDevice[DeviceID][AiID].realData = realData;

					bool bCondFlag = false;
					while (itCond != vCond.end())
					{
						if (m_mapBinaryDevice[itCond->Condition_device_id][itCond->Condition_id].realData == itCond->Condition_flag)
						{
							bCondFlag = true;
						}
						else
						{
							bCondFlag = false;
							break;
						}
						itCond++;
					}

					if (!bCondFlag)
					{
						break;
					}

					if (itAi->nTransType == 1)
					{
						//转发遥控
						while (itTarget != vTargetInfo.end())
						{
							emit Signal_TransBinary(itTarget->Target_station_id, itTarget->Targert_device_id, itTarget->Target_id, realData);
							itTarget++;
						}


					}
					else if (itAi->nTransType == 2)
					{
						//转发遥调
						while (itTarget != vTargetInfo.end())
						{
							emit Signal_TransAnalog(itTarget->Target_station_id, itTarget->Targert_device_id, itTarget->Target_id, realData);

							itTarget++;
						}

					}
					else
					{
						break;
					}

					break;
				}
				itAi++;
			}

			ui.tableWidget_3->item(m_mapDevAiRow[AiID], 3)->setText(QString::number(realData));
		}
		else
		{
			int nRownum = ui.tableWidget_3->rowCount();
			m_mapDevAiRow.insert(AiID, nRownum);

			//要往m_mapAnalogDevice里插入数据
			ANALOGSTRUCT analogData;
			analogData.StationID = StationID;
			analogData.DeviceID = DeviceID;
			analogData.AiID = AiID;
			analogData.AiName = AiName;
			analogData.realData = realData;
			if (m_mapAnalogDevice.contains(DeviceID))
			{
				m_mapAnalogDevice[DeviceID].insert(AiID, analogData);
			}
			else
			{
				QMap<int, ANALOGSTRUCT> tMapSingleAIInfo;
				tMapSingleAIInfo.insert(AiID, analogData);
				m_mapAnalogDevice.insert(DeviceID, tMapSingleAIInfo);
			}

			ui.tableWidget_3->insertRow(nRownum);

			QTableWidgetItem *item0 = new QTableWidgetItem;
			item0->setText(QString::number(AiID));
			ui.tableWidget_3->setItem(nRownum, 0, item0);

			QTableWidgetItem *item1 = new QTableWidgetItem;
			item1->setText(QString::number(StationID));
			ui.tableWidget_3->setItem(nRownum, 1, item1);

			QTableWidgetItem *item2 = new QTableWidgetItem;
			item2->setText(QString::number(DeviceID));
			ui.tableWidget_3->setItem(nRownum, 2, item2);

			QTableWidgetItem *item3 = new QTableWidgetItem;
			item3->setText(QString::number(realData));
			ui.tableWidget_3->setItem(nRownum, 3, item3);

			QTableWidgetItem *item4 = new QTableWidgetItem;
			item4->setText(QString(AiName));
			ui.tableWidget_3->setItem(nRownum, 4, item4);

		}

	}
}

//更新电度量数据槽函数
void CMb_Exchange::Slot_UpdateRecvKwhData(int StationID, int DeviceID, int KwhID, int realData, QString KwhName)
{
	if (m_nDeviceID == DeviceID)
	{
		if (m_mapDevKwhRow.contains(KwhID))
		{
			ui.tableWidget_4->item(m_mapDevKwhRow[KwhID], 3)->setText(QString::number(realData));
		}
		else
		{
			int nRownum = ui.tableWidget_4->rowCount();
			m_mapDevKwhRow.insert(KwhID, nRownum);

			ui.tableWidget_4->insertRow(nRownum);

			QTableWidgetItem *item0 = new QTableWidgetItem;
			item0->setText(QString::number(KwhID));
			ui.tableWidget_4->setItem(nRownum, 0, item0);

			QTableWidgetItem *item1 = new QTableWidgetItem;
			item1->setText(QString::number(StationID));
			ui.tableWidget_4->setItem(nRownum, 1, item1);

			QTableWidgetItem *item2 = new QTableWidgetItem;
			item2->setText(QString::number(DeviceID));
			ui.tableWidget_4->setItem(nRownum, 2, item2);

			QTableWidgetItem *item3 = new QTableWidgetItem;
			item3->setText(QString::number(realData));
			ui.tableWidget_4->setItem(nRownum, 3, item3);

			QTableWidgetItem *item4 = new QTableWidgetItem;
			item4->setText(QString(KwhName));
			ui.tableWidget_4->setItem(nRownum, 4, item4);

		}

	}
}

//显示socket错误槽函数
void CMb_Exchange::Slot_SocketError(int nDeviceID, const QString &strDeviceName, const QString &strError)
{
	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	ui.tableWidget_2->insertRow(0);

	QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setText(strdate);
	ui.tableWidget_2->setItem(0, 0, item0);

	QTableWidgetItem *item1 = new QTableWidgetItem;
	item1->setText(strTime);
	ui.tableWidget_2->setItem(0, 1, item1);

	QTableWidgetItem *item2 = new QTableWidgetItem;
	item2->setText(QString::number(nDeviceID));
	ui.tableWidget_2->setItem(0, 2, item2);

	QTableWidgetItem *item3 = new QTableWidgetItem;
	item3->setText(strDeviceName);
	ui.tableWidget_2->setItem(0, 3, item3);

	QTableWidgetItem *item4 = new QTableWidgetItem;
	item4->setText(strError);
	ui.tableWidget_2->setItem(0, 4, item4);

}

//显示socket连接成功槽函数
void CMb_Exchange::Slot_ConnectSuccess(int nDeviceID, const QString &strDeviceName)
{
	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	ui.tableWidget_2->insertRow(0);

	QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setText(strdate);
	ui.tableWidget_2->setItem(0, 0, item0);

	QTableWidgetItem *item1 = new QTableWidgetItem;
	item1->setText(strTime);
	ui.tableWidget_2->setItem(0, 1, item1);

	QTableWidgetItem *item2 = new QTableWidgetItem;
	item2->setText(QString::number(nDeviceID));
	ui.tableWidget_2->setItem(0, 2, item2);

	QTableWidgetItem *item3 = new QTableWidgetItem;
	item3->setText(strDeviceName);
	ui.tableWidget_2->setItem(0, 3, item3);

	QTableWidgetItem *item4 = new QTableWidgetItem;
	item4->setText(tr("connect success!"));
	ui.tableWidget_2->setItem(0, 4, item4);
}

//通道重连断开右键菜单槽函数
void CMb_Exchange::Slot_ContextMenuEvent(const QPoint & cPoint)
{
	Q_UNUSED(cPoint);
	QMenu *pMenu = new QMenu(ui.treeWidget);
	QAction *pReconnect = new QAction(QStringLiteral("重连"), ui.treeWidget);
	QAction *pDisconnect = new QAction(QStringLiteral("断开"), ui.treeWidget);

	pMenu->addAction(pReconnect);
	pMenu->addAction(pDisconnect);

	connect(pReconnect, SIGNAL(triggered()), this, SLOT(Slot_Reconnect()));
	connect(pDisconnect, SIGNAL(triggered()), this, SLOT(Slot_Disconnect()));

	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}

//手动重连菜单槽函数
void CMb_Exchange::Slot_Reconnect()
{
	int nDeviceID = ui.treeWidget->currentItem()->data(0, 256).toInt();
	QString strDeviceName = m_pReadXML->m_mapDevice[nDeviceID].Device_name;
	QString strIP = m_pReadXML->m_mapDevice[nDeviceID].strIP;
	int nPort = m_pReadXML->m_mapDevice[nDeviceID].Port;

	emit Signal_ReconSocket(nDeviceID, strIP, nPort);

	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	ui.tableWidget_2->insertRow(0);

	QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setText(strdate);
	ui.tableWidget_2->setItem(0, 0, item0);

	QTableWidgetItem *item1 = new QTableWidgetItem;
	item1->setText(strTime);
	ui.tableWidget_2->setItem(0, 1, item1);

	QTableWidgetItem *item2 = new QTableWidgetItem;
	item2->setText(QString::number(nDeviceID));
	ui.tableWidget_2->setItem(0, 2, item2);

	QTableWidgetItem *item3 = new QTableWidgetItem;
	item3->setText(strDeviceName);
	ui.tableWidget_2->setItem(0, 3, item3);

	QTableWidgetItem *item4 = new QTableWidgetItem;
	item4->setText(QStringLiteral("重连中..."));
	ui.tableWidget_2->setItem(0, 4, item4);
}

//手动断开连接菜单槽函数
void CMb_Exchange::Slot_Disconnect()
{
	int nDeviceID = ui.treeWidget->currentItem()->data(0, 256).toInt();
	QString strDeviceName = m_pReadXML->m_mapDevice[nDeviceID].Device_name;

	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	ui.tableWidget_2->insertRow(0);

	QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setText(strdate);
	ui.tableWidget_2->setItem(0, 0, item0);

	QTableWidgetItem *item1 = new QTableWidgetItem;
	item1->setText(strTime);
	ui.tableWidget_2->setItem(0, 1, item1);

	QTableWidgetItem *item2 = new QTableWidgetItem;
	item2->setText(QString::number(nDeviceID));
	ui.tableWidget_2->setItem(0, 2, item2);

	QTableWidgetItem *item3 = new QTableWidgetItem;
	item3->setText(strDeviceName);
	ui.tableWidget_2->setItem(0, 3, item3);

	QTableWidgetItem *item4 = new QTableWidgetItem;
	item4->setText(QStringLiteral("断开连接..."));
	ui.tableWidget_2->setItem(0, 4, item4);
	emit Signal_CloseSocket(nDeviceID);
}




