
#include <QTabWidget>
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QTimer>
#include <QDir>
#include "commthread.h"
#include "devicestudio/main_module.h"
#include "fixddeploy.h"

CFixdDeploy::CFixdDeploy(CCommThread *pCommunicate, IMainModule *pCore, const QString &deviceName, QWidget *parent)
	: QWidget(parent),
	m_pTabWgt(new QTabWidget(this)),
	m_pFixTimeOut(new QTimer(this)),
	m_pFixReadTimeOut(new QTimer(this)),
	m_strDeviceName(deviceName)
{
	m_nProtectDevBgAddr = 0;
	m_nBinaryDevBgAddr = 0;
	m_nBatteryDevBgAddr = 0;
	//m_nCurrentDevAddr = DZ_BG_NUM;

	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}
	m_pMainModule = pCore;
	m_pCommunicate = pCommunicate;
	connect(m_pCommunicate, SIGNAL(Signal_DevReadBack(QMap<int, short>)), this, SLOT(Slot_updateFixData(QMap<int, short>)));
	//定值设定
	connect(m_pCommunicate, SIGNAL(Signal_devWriteBack(int, int, int)), this, SLOT(Slot_WriteFeedBack(int, int, int)));
	//
	connect(m_pCommunicate,SIGNAL(Signal_ReadFixData(DEV_BASE &)),this,SLOT(Slot_ReadFixData(DEV_BASE&)));

	QVBoxLayout *pWgtLayout = new QVBoxLayout(this);
	pWgtLayout->addWidget(m_pTabWgt);
	//超时设置
	connect(m_pFixTimeOut, SIGNAL(timeout()), this, SLOT(Slot_FixWaitTimeOut()));
	//读取超时
	connect(m_pFixReadTimeOut, SIGNAL(timeout()), this, SLOT(Slot_FixReadTimeOut()));
	//
	m_mapTypeCode.insert(1,"Boolean");
	m_mapTypeCode.insert(43, "Tiny");
	m_mapTypeCode.insert(32, "UTiny");
	m_mapTypeCode.insert(33, "Short");
	m_mapTypeCode.insert(45, "UShort");
	m_mapTypeCode.insert(2, "Int");
	m_mapTypeCode.insert(35, "Uint");
	m_mapTypeCode.insert(36, "Long");
	m_mapTypeCode.insert(37, "Ulong");
	m_mapTypeCode.insert(38, "Float");
	m_mapTypeCode.insert(39, "Double");
	m_mapTypeCode.insert(4, "String");
}

//初始化界面
bool CFixdDeploy::InitWgt(const QString &strFileName)
{
	//清空所有数据  
	m_MAPTabindexToTable.clear();
	m_MapTableNameToIndex.clear();
	m_lstFixData.clear();
	m_lstBaseInfo.clear();
	m_pFixTimeOut->stop();
	m_pFixReadTimeOut->stop();

	m_nProtectDevBgAddr = 0;
	m_nBinaryDevBgAddr = 0;
	m_nBatteryDevBgAddr = 0;
	//m_nCurrentDevAddr = DZ_BG_NUM;

	m_pTabWgt->clear();

	if (strFileName.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Fix Value Conifg file is null!"));
		return false;
	}

	QFile fFixConfig(strFileName);
	if (fFixConfig.open(QFile::ReadOnly | QFile::Text))
	{
		AnalyseXmlData(&fFixConfig);
		fFixConfig.close();
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Open file %1 error!").arg(strFileName));
		return false;
	}

	SetConfigBinValue(m_strDevFilePath + "/dzcs.bin");

	return true;
}

//解析文件
void CFixdDeploy::AnalyseXmlData(QIODevice *pDevice)
{
	QXmlStreamReader xmlReader(pDevice);
	m_lstFixData.clear();

	//需要加载的tablewidget
	s_DevParam currentData;

	while (!xmlReader.atEnd() && (!xmlReader.hasError()))
	{
		//遍历所有配置
		xmlReader.readNext();
		if (xmlReader.isStartElement())
		{
			if (xmlReader.name().toString() == "DeviceTerminalConfig")
			{
				m_strConfigVersion = xmlReader.attributes().value("version").toString();
			}
			if (xmlReader.name().toString() == "DevParam")
			{
				//解析数据
				currentData.lstItems.clear();
				currentData.strDevParamName = xmlReader.attributes().value("name").toString();
				currentData.iDesplay = xmlReader.attributes().value("display").toString().toInt();
				currentData.iBeginAddress = GetBeginNumFromName(currentData.strDevParamName);
			}
			else if (xmlReader.name().toString() == "Item")
			{
				s_DevItems devItemData;

				devItemData.valueType = xmlReader.attributes().value("valueType").toString();
				devItemData.defalutVlaue = xmlReader.attributes().value("defValue").toString().toInt();
				devItemData.ItemName = xmlReader.attributes().value("name").toString();
				devItemData.ItemDesc = xmlReader.attributes().value("desc").toString();
				devItemData.rangeFt = xmlReader.attributes().value("range").toString();
				devItemData.TypeCode = xmlReader.attributes().value("tag").toString().toInt();
				devItemData.DataLength = xmlReader.attributes().value("length").toString().toInt();

				m_strConfigValueType = xmlReader.attributes().value("valueType").toString();

				currentData.lstItems.append(devItemData);
			}
		}
		if (xmlReader.isEndElement())
		{
			//
			if (xmlReader.name().toString() == "DevParam")
			{
				m_lstFixData.append(currentData);
			}
		}
	}
	//排序
	ChangeItemSort();
	//数据写入表格中
	UpdateDataToWgt(m_lstFixData);
}

//根据名称返回编号
int CFixdDeploy::GetBeginNumFromName(const QString &strName)
{
	if (strName == QString("固有参数"))
	{
		return DZ_GYCS_ADDR;
	}
	else if (strName == QString("装置参数"))
	{
		return DZ_DEVICE_ADDR;
	}
	else if (strName == QString("保护定值参数"))
	{
		return DZ_PROTECT_ADDR;
	}
	else
	{
		return 0;
	}
}

CFixdDeploy::~CFixdDeploy()
{

}


//往表中写入数据
void CFixdDeploy::UpdateDataToWgt(const QList<s_DevParam> & lstFixData)
{
	//检查数据
	if (lstFixData.isEmpty())
	{
		return;
	}

	int iCurrentTabIndex = 0;

	for (int i = 0; i < lstFixData.count(); i++)
	{
		if (lstFixData.at(i).iDesplay == DISPLAY && (lstFixData.at(i).strDevParamName == QString("装置参数")
			|| (lstFixData.at(i).strDevParamName == QString("保护定值参数"))
			|| (lstFixData.at(i).strDevParamName == QString("固有参数")) ))
		{
			//展示
			QWidget *pOneDeviceWgt = new QWidget(m_pTabWgt);
			m_pTabWgt->addTab(pOneDeviceWgt, lstFixData.at(i).strDevParamName);

			//构建表格
			QTableWidget *pOneDeviceTable = new QTableWidget(pOneDeviceWgt);
			QVBoxLayout *oneWgtLayout = new QVBoxLayout(pOneDeviceWgt);
			oneWgtLayout->addWidget(pOneDeviceTable);
			//记录数据
			m_MAPTabindexToTable[iCurrentTabIndex] = pOneDeviceTable;
			m_MapTableNameToIndex[lstFixData.at(i).strDevParamName] = iCurrentTabIndex;
			iCurrentTabIndex++;
// 			if (lstFixData.at(i).strDevParamName == QString("保护定值参数"))
// 			{
// 				m_nProtectDevBgAddr = lstFixData.at(i).iBeginAddress;;
// 			}
// 			else if (lstFixData.at(i).strDevParamName == QString("装置参数"))
// 			{
// 				m_nBinaryDevBgAddr = lstFixData.at(i).iBeginAddress;;;
// 			}
// 			else if (lstFixData.at(i).strDevParamName == QString("蓄电池管理"))
// 			{
// 				m_nBatteryDevBgAddr = m_nCurrentDevAddr;
// 			}

			//对表格进行处理
			SetTableAttr(pOneDeviceTable);

			//加载数据
			InitDataToTable(pOneDeviceTable, lstFixData.at(i));

		}
	}
}

//加载数据
void CFixdDeploy::InitDataToTable(QTableWidget* pCurrentTable, const s_DevParam &devParamDatas)
{
	for (int i = 0; i < devParamDatas.lstItems.count(); i++)
	{
		int iInsertRow = pCurrentTable->rowCount();

		pCurrentTable->insertRow(iInsertRow);
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(devParamDatas.lstItems.at(i).ItemName);
		item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 0, item0);

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(QString::number(devParamDatas.lstItems.at(i).defalutVlaue));
		item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 1, item1);

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setText(QString::number(devParamDatas.lstItems.at(i).defalutVlaue));
		item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 2, item2);

		QTableWidgetItem *item3 = new QTableWidgetItem;
		item3->setText(devParamDatas.lstItems.at(i).rangeFt);
		item3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 3, item3);

		QTableWidgetItem *item4 = new QTableWidgetItem;
		item4->setText(devParamDatas.lstItems.at(i).ItemDesc);
		item4->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 4, item4);

		QTableWidgetItem *item5 = new QTableWidgetItem;
		item5->setText(QString::number(devParamDatas.iBeginAddress + i));
		item5->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 5, item5);

		QTableWidgetItem *item6 = new QTableWidgetItem;
		int nType = devParamDatas.lstItems.at(i).TypeCode;
		item6->setText(m_mapTypeCode[nType]);
		item6->setData(Qt::UserRole + 1, nType);
		item6->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 6, item6);

		QTableWidgetItem *item7 = new QTableWidgetItem;
		int nDataLeg = devParamDatas.lstItems.at(i).DataLength;
		item7->setText(QString::number(nDataLeg));
		item7->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		pCurrentTable->setItem(iInsertRow, 7, item7);

	}

	// m_nCurrentDevAddr++;
}

//设置table属性
void CFixdDeploy::SetTableAttr(QTableWidget* pCurrentTable)
{
	pCurrentTable->setColumnCount(8);
	QStringList lstHeader;
	lstHeader << tr("Name") << tr("Value") << tr("Default Value") << tr("Range") << tr("describe") << tr("Begin")<<tr("DataTyp")<<tr("DataLength");
	pCurrentTable->setHorizontalHeaderLabels(lstHeader);
	pCurrentTable->setColumnWidth(0, 300);
	pCurrentTable->setColumnWidth(4, 500);
	pCurrentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	//pCurrentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pCurrentTable->horizontalHeader()->setStretchLastSection(true);
	//右击菜单
	pCurrentTable->setContextMenuPolicy(Qt::CustomContextMenu);
	//隐藏起始地址那一列
	//pCurrentTable->setColumnHidden(5, true);

	connect(pCurrentTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_DoubleClickTableItem(QTableWidgetItem *)));

	connect(pCurrentTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));
}

//双击表格
void CFixdDeploy::Slot_DoubleClickTableItem(QTableWidgetItem *item)
{
	QTableWidgetItem * pValueItem = item->tableWidget()->item(item->row(), 1);
	item->tableWidget()->editItem(pValueItem);
}


//右击菜单槽函数
void CFixdDeploy::Slot_ContextMenuRequest(const QPoint &pointCurrent)
{
	QTableWidget *pCurrentTable = qobject_cast <QTableWidget*>(sender());

	if (pCurrentTable == nullptr)
	{
		return;
	}

	if (pCurrentTable->itemAt(pointCurrent) == nullptr)
	{
		return;
	}

	QMenu *pLeftMenu = new QMenu(pCurrentTable);

	QAction *pUpdateAllAct = new QAction(tr("Update All"), pCurrentTable);
	QAction *pUpdateSelectedAct = new QAction(tr("Update Selected"), pCurrentTable);
	QAction *pSendAllAct = new QAction(tr("Send All"), pCurrentTable);
	QAction *pSendSelectAct = new QAction(tr("Send Selected"), pCurrentTable);
	//保存
	QAction *pSaveAct = new QAction(tr("Save File"), pCurrentTable);

	connect(pUpdateAllAct, SIGNAL(triggered()), this, SLOT(Slot_SendUpateAllPoints()));
	connect(pUpdateSelectedAct, SIGNAL(triggered()), this, SLOT(Slot_SendUpdatePoints()));
	connect(pSendAllAct, SIGNAL(triggered()), this, SLOT(Slot_SendCurrentAllPoints()));
	connect(pSendSelectAct, SIGNAL(triggered()), this, SLOT(Slot_SendCurrentPoints()));
	//保存
	connect(pSaveAct, SIGNAL(triggered()), this, SLOT(Slot_SaveFixConfigAct()));

	pLeftMenu->addAction(pUpdateAllAct);
	pLeftMenu->addAction(pUpdateSelectedAct);
	pLeftMenu->addAction(pSendAllAct);
	pLeftMenu->addAction(pSendSelectAct);
	pLeftMenu->addSeparator();
	pLeftMenu->addAction(pSaveAct);

	pLeftMenu->exec(QCursor::pos());
	pLeftMenu->deleteLater();
}

//更新数据
void CFixdDeploy::Slot_updateFixData(QMap<int, short> mapAddrData)
{
	//收到数据  关闭定时器
	m_pFixReadTimeOut->stop();
	//m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Receive Param Read Suceess Respond").toLocal8Bit().data(), 1);
	//更新数据
	QMap<int, short>::const_iterator it = mapAddrData.constBegin();

	while (it != mapAddrData.constEnd())
	{
		int iKey = it.key();
		if (iKey >= DZ_GYCS_ADDR && iKey < DZ_DEVICE_ADDR)
		{
			//装置参数起始地址
			if (m_MapTableNameToIndex.contains(QString("固有参数")))
			{
				//处理装置参数
				int iTabIndex = m_MapTableNameToIndex[QString("固有参数")];
				if (m_MAPTabindexToTable.contains(iTabIndex))
				{
					//UpdateDevData(m_MAPTabindexToTable[iTabIndex], it.key() - DZ_READ_BG_NUM + DZ_BG_NUM, it.value());
				}
			}
		}
		else if (iKey >= DZ_DEVICE_ADDR && iKey < DZ_PROTECT_ADDR)
		{
			//保护定值参数起始地址
			if (m_MapTableNameToIndex.contains(QString("装置参数")))
			{
				//处理装置参数
				int iTabIndex = m_MapTableNameToIndex[QString("装置参数")];
				if (m_MAPTabindexToTable.contains(iTabIndex))
				{
					//UpdateDevData(m_MAPTabindexToTable[iTabIndex], it.key() - DZ_READ_BG_NUM + DZ_BG_NUM, it.value());
				}
			}
		}
		else if (iKey >= DZ_PROTECT_ADDR)
		{
			//遥信参数起始地址
			if (m_MapTableNameToIndex.contains(QString("保护定值参数")))
			{
				//处理装置参数
				int iTabIndex = m_MapTableNameToIndex[QString("保护定值参数")];
				if (m_MAPTabindexToTable.contains(iTabIndex))
				{
					//UpdateDevData(m_MAPTabindexToTable[iTabIndex], it.key() - DZ_READ_BG_NUM + DZ_BG_NUM, it.value());
				}
			}
		}

		else
		{
			//除了以上定值  其他值暂不做处理
		}
		it++;

	}

	//QMessageBox::information(this, tr("Success"),tr("Update Data Success"));
}


void CFixdDeploy::Slot_ReadFixData(DEV_BASE &devRead)
{
	//收到数据  关闭定时器
	m_pFixReadTimeOut->stop();

	for (auto item : devRead.m_lstData)
	{
		int iKey = item.nAddress;
		if (iKey >= DZ_GYCS_ADDR && iKey < DZ_DEVICE_ADDR)
		{
			//装置参数起始地址
			if (m_MapTableNameToIndex.contains(QString("固有参数")))
			{
				//处理装置参数
				int iTabIndex = m_MapTableNameToIndex[QString("固有参数")];
				if (m_MAPTabindexToTable.contains(iTabIndex))
				{
					UpdateDevData(m_MAPTabindexToTable[iTabIndex], iKey, item.strValue);
				}
			}
		}
		else if (iKey >= DZ_DEVICE_ADDR && iKey < DZ_PROTECT_ADDR)
		{
			//保护定值参数起始地址
			if (m_MapTableNameToIndex.contains(QString("装置参数")))
			{
				//处理装置参数
				int iTabIndex = m_MapTableNameToIndex[QString("装置参数")];
				if (m_MAPTabindexToTable.contains(iTabIndex))
				{
					UpdateDevData(m_MAPTabindexToTable[iTabIndex], iKey, item.strValue);
				}
			}
		}
		else if (iKey >= DZ_PROTECT_ADDR)
		{
			//遥信参数起始地址
			if (m_MapTableNameToIndex.contains(QString("保护定值参数")))
			{
				//处理装置参数
				int iTabIndex = m_MapTableNameToIndex[QString("保护定值参数")];
				if (m_MAPTabindexToTable.contains(iTabIndex))
				{
					UpdateDevData(m_MAPTabindexToTable[iTabIndex], iKey, item.strValue);
				}
			}
		}
		else
		{
			//除了以上定值  其他值暂不做处理
		}

	}
}

//更新定值参数
void CFixdDeploy::UpdateDevData(QTableWidget *pCurrentTable, int iNumber, const QString &strValue)
{
	//更新装置参数信息
	int iRowNumber = 0;

	if (iNumber >= DZ_GYCS_ADDR && iNumber < DZ_DEVICE_ADDR)
	{
		iRowNumber = iNumber - DZ_GYCS_ADDR;
	}
	else if (iNumber >= DZ_DEVICE_ADDR && iNumber < DZ_PROTECT_ADDR)
	{
		iRowNumber = iNumber - DZ_DEVICE_ADDR;
	}
	else if (iNumber >= m_nBinaryDevBgAddr && iNumber < m_nBatteryDevBgAddr)
	{
		iRowNumber = iNumber - m_nBinaryDevBgAddr;
	}
	else if (iNumber >= DZ_PROTECT_ADDR)
	{
		iRowNumber = iNumber - DZ_PROTECT_ADDR;
	}

	if (pCurrentTable->rowCount() > iRowNumber)
	{
		pCurrentTable->item(iRowNumber, 1)->setText(strValue);
	}
	else
	{
		pCurrentTable->item(pCurrentTable->currentItem()->row(),1)->setText(strValue);
	}
	//pCurrentTable->item(iRowNumber, 1)->setText(strValue);
}

//发送需要更新的点  请求数据
void CFixdDeploy::Slot_SendUpdatePoints()
{
	//
	QAction *pClickAct = qobject_cast<QAction*>(sender());

	if (pClickAct == nullptr)
	{
		return;
	}

	if (pClickAct->parent() != nullptr)
	{
		QTableWidget *pCurrentTableWgt = qobject_cast<QTableWidget*>(pClickAct->parent());

		if (pCurrentTableWgt != NULL)
		{
			//处理当前选中的行
			DEV_BASE devBaseData;
			devBaseData.m_nAsduID = 1;


			devBaseData.m_nCommandType = D_FIX_READ;

			devBaseData.m_nCto = 6;

			devBaseData.m_nQos = 0;

			QList<int> lstRows = GetSelectRows(pCurrentTableWgt);

			for (int i = 0; i < lstRows.count(); i++)
			{
				DEV_DATA singleData;
				//定值信息体地址
				singleData.nAddress = pCurrentTableWgt->item(lstRows.at(i), 5)->text().toInt();
				//value 由于不知道类型  用字符串标识
				singleData.strValue = pCurrentTableWgt->item(lstRows.at(i), 1)->text();
				//数据类型
				singleData.nTagType = pCurrentTableWgt->item(lstRows.at(i), 6)->data(Qt::UserRole).toInt();
				//数据长度
				singleData.nLength = pCurrentTableWgt->item(lstRows.at(i), 7)->text().toInt();

				devBaseData.m_lstData.append(singleData);
			}

			//发送数据
			m_pCommunicate->SendDevOrder(devBaseData);

			//启动定时器
			m_pFixReadTimeOut->start(5000);

			m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Param Read Request").toLocal8Bit().data(), 1);
		}
	}
}
//发送所有的点      请求数据
void CFixdDeploy::Slot_SendUpateAllPoints()
{
	//
	QAction *pClickAct = qobject_cast<QAction*>(sender());

	if (pClickAct == nullptr)
	{
		return;
	}

	if (pClickAct->parent() != nullptr)
	{
		QTableWidget *pCurrentTableWgt = qobject_cast<QTableWidget*>(pClickAct->parent());

		if (pCurrentTableWgt != NULL)
		{
			//处理当前选中的行
			DEV_BASE devBaseData;
			devBaseData.m_nAsduID = 1;
			// 			if (GetSelectRows(pCurrentTableWgt).count() == 1)
			// 			{
			// 				devBaseData.m_nCommandType = C_RD_NA_1;
			// 			}
			// 			else
			// 			{
			devBaseData.m_nCommandType = D_FIX_READ;
			//			}

			devBaseData.m_nCto = 6;

			devBaseData.m_nQos = 0;

// 			for (int i = 0; i < pCurrentTableWgt->rowCount(); i++)
// 			{
// 				DEV_DATA singleData;
// 				singleData.nAddress = pCurrentTableWgt->item(i, 5)->text().toInt() - DZ_BG_NUM + DZ_READ_BG_NUM;;
// 				singleData.nValue = pCurrentTableWgt->item(i, 1)->text().toInt();
// 				devBaseData.m_lstData.append(singleData);
// 			}
			//发送数据
			m_pCommunicate->SendDevOrder(devBaseData);
			//启动定时器
			m_pFixReadTimeOut->start(5000);
			m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Param Read Request").toLocal8Bit().data(), 1);
		}
	}
}
//发送现有的数据    发送数据
void CFixdDeploy::Slot_SendCurrentPoints()
{
	QAction *pClickAct = qobject_cast<QAction*>(sender());

	if (pClickAct == nullptr)
	{
		return;
	}

	if (pClickAct->parent() != nullptr)
	{
		QTableWidget *pCurrentTableWgt = qobject_cast<QTableWidget*>(pClickAct->parent());

		if (pCurrentTableWgt != NULL)
		{
			//处理当前选中的行
			DEV_BASE devBaseData;
			devBaseData.m_nAsduID = 1;

			//定值写入
		    devBaseData.m_nCommandType = D_FIX_WRITE;

			devBaseData.m_nCto = COT_ACT;

			devBaseData.m_nQos = 0;

			QList<int> lstRows = GetSelectRows(pCurrentTableWgt);
			for (int i = 0; i < lstRows.count(); i++)
			{
				DEV_DATA singleData;
				singleData.nAddress = pCurrentTableWgt->item(lstRows.at(i), 5)->text().toInt();
				//所有数据都用字符串传送
				singleData.strValue = pCurrentTableWgt->item(lstRows.at(i), 1)->text();
				//数据类型
				singleData.nTagType = pCurrentTableWgt->item(lstRows.at(i), 6)->data(Qt::UserRole+1).toInt();
				//数据长度
				singleData.nLength = pCurrentTableWgt->item(lstRows.at(i), 7)->text().toInt();

				devBaseData.m_lstData.append(singleData);
			}

			//发送数据
			m_pCommunicate->SendDevOrder(devBaseData);
			m_lstBaseInfo.append(devBaseData);
			//启动定时器
			m_pFixTimeOut->start(5000);
			m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Param Write  Request").toLocal8Bit().data(), 1);
		}
	}
}
//发送所有的数据
void CFixdDeploy::Slot_SendCurrentAllPoints()
{
	QAction *pClickAct = qobject_cast<QAction*>(sender());

	if (pClickAct == nullptr)
	{
		return;
	}

	if (pClickAct->parent() != nullptr)
	{
		QTableWidget *pCurrentTableWgt = qobject_cast<QTableWidget*>(pClickAct->parent());

		if (pCurrentTableWgt != NULL)
		{
			//处理当前选中的行
			DEV_BASE devBaseData;
			devBaseData.m_nAsduID = 1;

			devBaseData.m_nCommandType = D_FIX_WRITE;

			devBaseData.m_nCto = COT_ACT;

			devBaseData.m_nQos = 0;

			for (int i = 0; i < pCurrentTableWgt->rowCount(); i++)
			{
				DEV_DATA singleData;
				singleData.nAddress = pCurrentTableWgt->item(i, 5)->text().toInt();
				//所有数据都用字符串传送
				singleData.strValue = pCurrentTableWgt->item(i, 1)->text();
				//数据类型
				singleData.nTagType = pCurrentTableWgt->item(i, 6)->data(Qt::UserRole).toInt();
				//数据长度
				singleData.nLength = pCurrentTableWgt->item(i, 7)->text().toInt();

				devBaseData.m_lstData.append(singleData);
			}

			//发送数据
			m_pCommunicate->SendDevOrder(devBaseData);
			m_lstBaseInfo.append(devBaseData);
			//启动定时器
			m_pFixTimeOut->start(5000);
			m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Param Write Request").toLocal8Bit().data(), 1);
		}
	}
}

//获取选中的行号
QList<int> CFixdDeploy::GetSelectRows(QTableWidget *pTableWidget)
{
	QList<int> lstSelectRows;


	QList<QTableWidgetSelectionRange>ranges = pTableWidget->selectedRanges();

	int count = ranges.count();

	for (int i = 0; i < count; i++)

	{

		int topRow = ranges.at(i).topRow();

		int bottomRow = ranges.at(i).bottomRow();

		for (int j = topRow; j <= bottomRow; j++)

		{
			lstSelectRows.append(j);
		}
	}

	return lstSelectRows;
}

//定值设定返回信息 48/136    6/7   1/0  
void CFixdDeploy::Slot_WriteFeedBack(int iControlType, int iCot, int iQos)
{
	Q_UNUSED(iQos);
	Q_UNUSED(iControlType);
	//发送定值
	// 	if ((iControlType == 48 || iControlType == 136) && (iCot == 7) && (iQos == 1))
	// 	{
	// 		//预置确认   不要预置,直接执行
	// // 		if (!m_lstBaseInfo.isEmpty())
	// // 		{
	// // 			DEV_BASE baseInfo = m_lstBaseInfo.at(0);
	// //             baseInfo.m_nCto = COT_ACT;
	// // 			baseInfo.m_nQos = 0;
	// // 			m_pCommunicate->SendDevOrder(baseInfo);
	// // 			m_lstBaseInfo.removeFirst();
	// // 			m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Receive Param Write Preset Respond").toLocal8Bit().data(), 1);
	// // 
	// // 			m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Send Param Write Active Request").toLocal8Bit().data(), 1);
	// // 		}
	// 		
	// 	}
	// 	else if ((iControlType == 48 || iControlType == 136) && (iCot == 7) )
	// 	{
	// 		//定值设置成功
	// 		m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Receive Param Write Active Success Respond").toLocal8Bit().data(), 1);
	// 		m_pFixTimeOut->stop();
	// 		QMessageBox::information(this, tr("Success"), tr("Parameter Set Success!"));
	// 	}

	if (iCot == 7)
	{
		//定值设置成功
		m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Receive Param Write Active Success Respond").toLocal8Bit().data(), 1);
		m_pFixTimeOut->stop();
		QMessageBox::information(this, tr("Success"), tr("Parameter Set Success!"));

	}

	//m_pFixTimeOut->stop();
}

//超时
void CFixdDeploy::Slot_FixWaitTimeOut()
{
	m_pFixTimeOut->stop();
	//清空数据
	m_lstBaseInfo.clear();

	QByteArray byDestr = tr("Receive Param Write  Respond Timeout").toLocal8Bit();
	m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), byDestr.data(), 1);
	QMessageBox::warning(this, tr("Warning"), tr("Waiting for Respond Timeout,Update Error"));
}

//读取超时
void CFixdDeploy::Slot_FixReadTimeOut()
{
	m_pMainModule->LogString(m_strDeviceName.toLocal8Bit().data(), tr("Waiting for Respond Timeout").toLocal8Bit().data(), 1);
	m_pFixReadTimeOut->stop();
	QMessageBox::warning(this, tr("Warning"), tr("Waiting for Respond Timeout,Update Error"));
}

void CFixdDeploy::ChangeItemSort()
{
	//s_DevParam
	QList<s_DevParam> lstTmpData;

	s_DevParam tTmpDev;
	lstTmpData.append(tTmpDev);
	lstTmpData.append(tTmpDev);
	lstTmpData.append(tTmpDev);
	//lstTmpData.append(tTmpDev);

	for each (s_DevParam devItem in m_lstFixData)
	{
		if (devItem.strDevParamName == QString("固有参数"))
		{
			lstTmpData[0] = devItem;
		}
		else if (devItem.strDevParamName == QString("装置参数"))
		{
			lstTmpData[1] = devItem;
		}
		else if (devItem.strDevParamName == QString("保护定值参数"))
		{
			lstTmpData[2] = devItem;
		}
	}

	m_lstFixData.clear();
	m_lstFixData = lstTmpData;
}

void CFixdDeploy::SetDeviceName(const QString strDeviceName)
{
	m_strDeviceName = strDeviceName;
}

void CFixdDeploy::Slot_SaveFixConfigAct()
{
	//保存文件
	//组装数据
	tDownloadParam downDevParam;
	//获取table里所有数据

	QMap<int, QTableWidget*>::const_iterator it = m_MAPTabindexToTable.constBegin();

	while (it != m_MAPTabindexToTable.constEnd())
	{
		QString strDevParamName = m_pTabWgt->tabText(it.key());

		QTableWidget *pTable = it.value();

		if (strDevParamName == QString("装置参数"))
		{
			if (pTable->rowCount() >= 70)
			{
				//填充数据
				UpdateDeviceParam(pTable, downDevParam);
			}
		}
		else if (strDevParamName == QString("保护定值参数"))
		{
			if (pTable->rowCount() >= 104)
			{
				//填充数据
				UpdateProtectParam(pTable, downDevParam);
			}
		}
		else if (strDevParamName == QString("遥信参数"))
		{
			//填充数据
			UpdateBinaryParam(pTable, downDevParam);
		}
		else if (strDevParamName == QString("蓄电池管理"))
		{
			if (pTable->rowCount() == 8)
			{
				//填充数据
				//UpdateElecParam(pTable, downDevParam);
			}
		}

		it++;
	}

	//UpdateDevConfigFile(downDevParam);
	//     qDebug() <<"Recvvvvvv:"<< (char *)&downDevParam;
	UpdateDevConfigFile(QByteArray((char *)&downDevParam, sizeof(tDownloadParam)));
}

void CFixdDeploy::UpdateDevConfigFile(QByteArray btArray)
{
	QDir directory;

	directory.setPath(m_strDevFilePath);
	if (!directory.exists())
	{
		directory.mkdir(m_strDevFilePath);
	}

	QString strFilename = m_strDevFilePath + "/dzcs.bin";

	QFile binFile(strFilename);

	if (binFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		binFile.write(btArray);
		binFile.close();
		QMessageBox::information(this, tr("Success"), tr("Dev File Save Success"));
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Save Dev File Error!"));
	}

}

void CFixdDeploy::UpdateDeviceParam(QTableWidget *pTable, tDownloadParam &DeviceParam)
{
	DeviceParam.devParm.zzcs.param.line2I = pTable->item(0, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.cycTimer = pTable->item(1, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.ykTime = pTable->item(2, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.u0BjTr = pTable->item(3, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.protocolType = pTable->item(4, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.addr = pTable->item(5, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.meter = pTable->item(6, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.ipA1 = pTable->item(7, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.ipA2 = pTable->item(8, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.ipB1 = pTable->item(9, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.ipB2 = pTable->item(10, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.lx_current_delt = pTable->item(11, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.bus1A_volt_delt = pTable->item(12, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.line1_volt_delt = pTable->item(13, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.line1A_current_delt = pTable->item(14, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.wattpower_delt = pTable->item(15, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.wattlesspower_delt = pTable->item(16, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.fifthxb_delt = pTable->item(17, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tlxuyx = pTable->item(18, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.fifthxbdz = pTable->item(19, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tfifthxb = pTable->item(20, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.i1_ycyxdz = pTable->item(21, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i1_ycyxdz1 = pTable->item(22, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx1 = pTable->item(23, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i2_ycyxdz = pTable->item(24, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i2_ycyxdz1 = pTable->item(25, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx2 = pTable->item(26, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i3_ycyxdz = pTable->item(27, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.i3_ycyxdz1 = pTable->item(28, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx3 = pTable->item(29, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i4_ycyxdz = pTable->item(30, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i4_ycyxdz1 = pTable->item(31, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx4 = pTable->item(32, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i5_ycyxdz = pTable->item(33, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i5_ycyxdz1 = pTable->item(34, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.tycyx5 = pTable->item(35, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i6_ycyxdz = pTable->item(36, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i6_ycyxdz1 = pTable->item(37, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx6 = pTable->item(38, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i7_ycyxdz = pTable->item(39, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i7_ycyxdz1 = pTable->item(40, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx7 = pTable->item(41, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.i8_ycyxdz = pTable->item(42, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i8_ycyxdz1 = pTable->item(43, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx8 = pTable->item(44, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i9_ycyxdz = pTable->item(45, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i9_ycyxdz1 = pTable->item(46, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx9 = pTable->item(47, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i10_ycyxdz = pTable->item(48, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.i10_ycyxdz1 = pTable->item(49, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx10 = pTable->item(50, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i11_ycyxdz = pTable->item(51, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i11_ycyxdz1 = pTable->item(52, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx11 = pTable->item(53, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i12_ycyxdz = pTable->item(54, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i12_ycyxdz1 = pTable->item(55, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.tycyx12 = pTable->item(56, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i13_ycyxdz = pTable->item(57, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i13_ycyxdz1 = pTable->item(58, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx13 = pTable->item(59, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i14_ycyxdz = pTable->item(60, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i14_ycyxdz1 = pTable->item(61, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx14 = pTable->item(62, 1)->text().toUShort();

	DeviceParam.devParm.zzcs.param.i15_ycyxdz = pTable->item(63, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i15_ycyxdz1 = pTable->item(64, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx15 = pTable->item(65, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i16_ycyxdz = pTable->item(66, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.i16_ycyxdz1 = pTable->item(67, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.tycyx16 = pTable->item(68, 1)->text().toUShort();
	DeviceParam.devParm.zzcs.param.getfile_enable = pTable->item(69, 1)->text().toUShort();

}

void CFixdDeploy::UpdateProtectParam(QTableWidget *pTable, tDownloadParam &DeviceParam)
{
	DeviceParam.devParm.ykpz.param.izds1 = pTable->item(0, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd1 = pTable->item(1, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds1 = pTable->item(2, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd1 = pTable->item(3, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds1 = pTable->item(4, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd1 = pTable->item(5, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds2 = pTable->item(6, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd2 = pTable->item(7, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds2 = pTable->item(8, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd2 = pTable->item(9, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds2 = pTable->item(10, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd2 = pTable->item(11, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds3 = pTable->item(12, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd3 = pTable->item(13, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds3 = pTable->item(14, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd3 = pTable->item(15, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds3 = pTable->item(16, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd3 = pTable->item(17, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds4 = pTable->item(18, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd4 = pTable->item(19, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds4 = pTable->item(20, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd4 = pTable->item(21, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds4 = pTable->item(22, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd4 = pTable->item(23, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds5 = pTable->item(24, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd5 = pTable->item(25, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds5 = pTable->item(26, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd5 = pTable->item(27, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds5 = pTable->item(28, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd5 = pTable->item(29, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds6 = pTable->item(30, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd6 = pTable->item(31, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds6 = pTable->item(32, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd6 = pTable->item(33, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds6 = pTable->item(34, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd6 = pTable->item(35, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds7 = pTable->item(36, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd7 = pTable->item(37, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds7 = pTable->item(38, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd7 = pTable->item(39, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds7 = pTable->item(40, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd7 = pTable->item(41, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds8 = pTable->item(42, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd8 = pTable->item(43, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds8 = pTable->item(44, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd8 = pTable->item(45, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds8 = pTable->item(46, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd8 = pTable->item(47, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds9 = pTable->item(48, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd9 = pTable->item(49, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds9 = pTable->item(50, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd9 = pTable->item(51, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds9 = pTable->item(52, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd9 = pTable->item(53, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds10 = pTable->item(54, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd10 = pTable->item(55, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds10 = pTable->item(56, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd10 = pTable->item(57, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds10 = pTable->item(58, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd10 = pTable->item(59, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds11 = pTable->item(60, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd11 = pTable->item(61, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds11 = pTable->item(62, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd11 = pTable->item(63, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds11 = pTable->item(64, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd11 = pTable->item(65, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds12 = pTable->item(66, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd12 = pTable->item(67, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds12 = pTable->item(68, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd12 = pTable->item(69, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds12 = pTable->item(70, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd12 = pTable->item(71, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds13 = pTable->item(72, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd13 = pTable->item(73, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds13 = pTable->item(74, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds13 = pTable->item(75, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds13 = pTable->item(76, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd13 = pTable->item(77, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds14 = pTable->item(78, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd14 = pTable->item(79, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds14 = pTable->item(80, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd14 = pTable->item(81, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds14 = pTable->item(82, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd14 = pTable->item(83, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds15 = pTable->item(84, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd15 = pTable->item(85, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds15 = pTable->item(86, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd15 = pTable->item(87, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds15 = pTable->item(88, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd15 = pTable->item(89, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.izds16 = pTable->item(90, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tglzd16 = pTable->item(91, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.igfhzds16 = pTable->item(92, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgfhzd16 = pTable->item(93, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.i0zds16 = pTable->item(94, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tgl0zd16 = pTable->item(95, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.tbhdzfg = pTable->item(96, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.tchz = pTable->item(97, 1)->text().toUShort();

	DeviceParam.devParm.ykpz.param.bhMode1 = pTable->item(98, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.bhMode2 = pTable->item(99, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.bhMode3 = pTable->item(100, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.bhMode4 = pTable->item(101, 1)->text().toUShort();
	DeviceParam.devParm.ykpz.param.bhMode5 = pTable->item(102, 1)->text().toUShort();

}

void CFixdDeploy::UpdateBinaryParam(QTableWidget *pTable, tDownloadParam &DeviceParam)
{
	for (int i = 0; i < pTable->rowCount() && i < 199; i++)
	{
		DeviceParam.devParm.yxcs.yx_time[i] = pTable->item(i, 1)->text().toUShort();
	}
}

void CFixdDeploy::UpdateElecParam(QTableWidget *pTable, tDownloadParam &DeviceParam)
{
	Q_UNUSED(pTable);
	Q_UNUSED(DeviceParam);
}

void CFixdDeploy::SetPorjectName(const QString &strPrjName)
{
	m_strDevFilePath = strPrjName;
}

void CFixdDeploy::SetConfigBinValue(const QString &strFilename)
{
	//读取文件  填充

	QFile tFile(strFilename);

	if (tFile.exists())
	{
		if (tFile.open(QIODevice::ReadOnly))
		{
			tDownloadParam downDevParam;

			QByteArray tBtData = tFile.readAll();

			memcpy((char *)&downDevParam, tBtData.data(), sizeof(tDownloadParam));

			InputBinDataToTable(downDevParam);
			tFile.close();
		}
	}

}

void CFixdDeploy::InputBinDataToTable(const tDownloadParam &tParam)
{
	//保存文件
	//获取table里所有数据

	QMap<int, QTableWidget*>::const_iterator it = m_MAPTabindexToTable.constBegin();

	while (it != m_MAPTabindexToTable.constEnd())
	{
		QString strDevParamName = m_pTabWgt->tabText(it.key());

		QTableWidget *pTable = it.value();

		if (strDevParamName == QString("装置参数"))
		{
			if (pTable->rowCount() >= 70)
			{
				//填充数据
				InputDeviceToTable(pTable, tParam);
			}
		}
		else if (strDevParamName == QString("保护定值参数"))
		{
			if (pTable->rowCount() >= 104)
			{
				//填充数据
				InputProtectToTable(pTable, tParam);
			}
		}
		else if (strDevParamName == QString("遥信参数"))
		{
			//填充数据
			InputBinaryToTable(pTable, tParam);
		}
		else if (strDevParamName == QString("蓄电池管理"))
		{
			if (pTable->rowCount() == 8)
			{
				//填充数据
				//UpdateElecParam(pTable, downDevParam);
			}
		}

		it++;
	}
}

void CFixdDeploy::InputDeviceToTable(QTableWidget *pTable, const tDownloadParam &DeviceParam)
{
	pTable->item(0, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.line2I));


	pTable->item(1, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.cycTimer));
	pTable->item(2, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.ykTime));
	pTable->item(3, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.u0BjTr));
	pTable->item(4, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.protocolType));
	pTable->item(5, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.addr));
	pTable->item(6, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.meter));

	pTable->item(7, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.ipA1));
	pTable->item(8, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.ipA2));
	pTable->item(9, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.ipB1));
	pTable->item(10, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.ipB2));
	pTable->item(11, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.lx_current_delt));
	pTable->item(12, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.bus1A_volt_delt));
	pTable->item(13, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.line1_volt_delt));

	pTable->item(14, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.line1A_current_delt));
	pTable->item(15, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.wattpower_delt));
	pTable->item(16, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.wattlesspower_delt));
	pTable->item(17, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.fifthxb_delt));
	pTable->item(18, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tlxuyx));
	pTable->item(19, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.fifthxbdz));
	pTable->item(20, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tfifthxb));

	pTable->item(21, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i1_ycyxdz));
	pTable->item(22, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i1_ycyxdz1));
	pTable->item(23, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx1));
	pTable->item(24, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i2_ycyxdz));
	pTable->item(25, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i2_ycyxdz1));
	pTable->item(26, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx2));
	pTable->item(27, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i3_ycyxdz));

	pTable->item(28, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i3_ycyxdz1));
	pTable->item(29, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx3));
	pTable->item(30, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i4_ycyxdz));
	pTable->item(31, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i4_ycyxdz1));
	pTable->item(32, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx4));
	pTable->item(33, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i5_ycyxdz));
	pTable->item(34, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i5_ycyxdz1));

	pTable->item(35, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx5));
	pTable->item(36, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i6_ycyxdz));
	pTable->item(37, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i6_ycyxdz1));
	pTable->item(38, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx6));
	pTable->item(39, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i7_ycyxdz));
	pTable->item(40, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i7_ycyxdz1));
	pTable->item(41, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx7));

	pTable->item(42, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i8_ycyxdz));
	pTable->item(43, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i8_ycyxdz1));
	pTable->item(44, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx8));
	pTable->item(45, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i9_ycyxdz));
	pTable->item(46, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i9_ycyxdz1));
	pTable->item(47, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx9));
	pTable->item(48, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i10_ycyxdz));

	pTable->item(49, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i10_ycyxdz1));
	pTable->item(50, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx10));
	pTable->item(51, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i11_ycyxdz));
	pTable->item(52, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i11_ycyxdz1));
	pTable->item(53, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx11));
	pTable->item(54, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i12_ycyxdz));
	pTable->item(55, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i12_ycyxdz1));

	pTable->item(56, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx12));
	pTable->item(57, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i13_ycyxdz));
	pTable->item(58, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i13_ycyxdz1));
	pTable->item(59, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx13));
	pTable->item(60, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i14_ycyxdz));
	pTable->item(61, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i14_ycyxdz1));
	pTable->item(62, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx14));

	pTable->item(63, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i15_ycyxdz));
	pTable->item(64, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i15_ycyxdz1));
	pTable->item(65, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx15));
	pTable->item(66, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i16_ycyxdz));
	pTable->item(67, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.i16_ycyxdz1));
	pTable->item(68, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.tycyx16));
	pTable->item(69, 1)->setText(QString::number(DeviceParam.devParm.zzcs.param.getfile_enable));
}

void CFixdDeploy::InputProtectToTable(QTableWidget *pTable, const tDownloadParam &DeviceParam)
{
	pTable->item(0, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds1));
	pTable->item(1, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd1));
	pTable->item(2, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds1));
	pTable->item(3, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd1));
	pTable->item(4, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds1));
	pTable->item(5, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd1));

	pTable->item(6, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds2));
	pTable->item(7, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd2));
	pTable->item(8, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds2));
	pTable->item(9, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd2));
	pTable->item(10, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds2));
	pTable->item(11, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd2));

	pTable->item(12, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds3));
	pTable->item(13, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd3));
	pTable->item(14, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds3));
	pTable->item(15, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd3));
	pTable->item(16, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds3));
	pTable->item(17, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd3));

	pTable->item(18, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds4));
	pTable->item(19, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd4));
	pTable->item(20, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds4));
	pTable->item(21, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd4));
	pTable->item(22, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds4));
	pTable->item(23, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd4));

	pTable->item(24, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds5));
	pTable->item(25, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd5));
	pTable->item(26, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds5));
	pTable->item(27, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd5));
	pTable->item(28, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds5));
	pTable->item(29, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd5));

	pTable->item(30, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds6));
	pTable->item(31, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd6));
	pTable->item(32, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds6));
	pTable->item(33, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd6));
	pTable->item(34, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds6));
	pTable->item(35, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd6));

	pTable->item(36, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds7));
	pTable->item(37, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd7));
	pTable->item(38, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds7));
	pTable->item(39, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd7));
	pTable->item(40, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds7));
	pTable->item(41, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd7));

	pTable->item(42, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds8));
	pTable->item(43, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd8));
	pTable->item(44, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds8));
	pTable->item(45, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd8));
	pTable->item(46, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds8));
	pTable->item(47, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd8));

	pTable->item(48, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds9));
	pTable->item(49, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd9));
	pTable->item(50, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds9));
	pTable->item(51, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd9));
	pTable->item(52, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds9));
	pTable->item(53, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd9));

	pTable->item(54, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds10));
	pTable->item(55, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd10));
	pTable->item(56, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds10));
	pTable->item(57, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd10));
	pTable->item(58, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds10));
	pTable->item(59, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd10));

	pTable->item(60, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds11));
	pTable->item(61, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd11));
	pTable->item(62, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds11));
	pTable->item(63, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd11));
	pTable->item(64, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds11));
	pTable->item(65, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd11));

	pTable->item(66, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds12));
	pTable->item(67, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd12));
	pTable->item(68, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds12));
	pTable->item(69, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd12));
	pTable->item(70, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds12));
	pTable->item(71, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd12));

	pTable->item(72, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds13));
	pTable->item(73, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd13));
	pTable->item(74, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds13));
	pTable->item(75, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds13));
	pTable->item(76, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds13));
	pTable->item(77, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd13));

	pTable->item(78, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds14));
	pTable->item(79, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd14));
	pTable->item(80, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds14));
	pTable->item(81, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd14));
	pTable->item(82, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds14));
	pTable->item(83, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd14));

	pTable->item(84, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds15));
	pTable->item(85, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd15));
	pTable->item(86, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds15));
	pTable->item(87, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd15));
	pTable->item(88, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds15));
	pTable->item(89, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd15));

	pTable->item(90, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.izds16));
	pTable->item(91, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tglzd16));
	pTable->item(92, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.igfhzds16));
	pTable->item(93, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgfhzd16));
	pTable->item(94, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.i0zds16));
	pTable->item(95, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tgl0zd16));

	pTable->item(96, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tbhdzfg));
	pTable->item(97, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.tchz));

	pTable->item(98, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.bhMode1));
	pTable->item(99, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.bhMode2));
	pTable->item(100, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.bhMode3));
	pTable->item(101, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.bhMode4));
	pTable->item(102, 1)->setText(QString::number(DeviceParam.devParm.ykpz.param.bhMode5));
}

void CFixdDeploy::InputBinaryToTable(QTableWidget *pTable, const tDownloadParam &DeviceParam)
{
	for (int i = 0; i < pTable->rowCount() && i < 199; i++)
	{
		pTable->item(i, 1)->setText(QString::number(DeviceParam.devParm.yxcs.yx_time[i]));
	}
}
