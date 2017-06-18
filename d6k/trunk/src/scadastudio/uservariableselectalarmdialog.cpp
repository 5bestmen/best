/*! @file
<PRE>
********************************************************************************
模块名       :  选择告警
文件名       :  selectalarmdialog.cpp
文件实现功能 :  选择转换
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date	2016.6.13
*/

#include <QStandardItemModel>
#include "uservariableselectalarmdialog.h"
#include "config_data.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "fesmodule.h"
#include "scadastudio/define.h"
#include "scadastudio/icore.h"
#include "delegate.h"

#include <QMenu>

extern IMainModuleInterface *s_pGlobleCore;

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

CUserVariableSelectAlarmDialog::CUserVariableSelectAlarmDialog(QWidget *parent, Config::CFesData *pFes, Config::CUserVariable *pData, int nType)
	: QDialog(parent), m_pModel(nullptr)
{
	Q_ASSERT(s_pGlobleCore);
	
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}

	Q_ASSERT(pData);
	if (!pData)
	{
		return;
	}

	ui.setupUi(this);

	m_pModel = new QStandardItemModel(this);
	ui.treeView->setModel(m_pModel);

	m_pFesData = pFes;
	m_pData = pData;

	InitPara();

	InitAlarm(nType);

	//InitTable(pFes, pAI);
	if (nType == AIALARM)
	{
		auto pTmp = pData;
		m_nAlarmType = pTmp->m_nAlarmType;
		//m_strDesc = pTmp->m_strAlarmDesc;
		m_strTagName = pTmp->m_strAlarmTagName;
		DumpModel(m_pModel, m_pModel->indexFromItem(m_pModel->invisibleRootItem()), pTmp->m_strAlarmTagName);
	}
	else
	{
		auto pTmp = pData;
		m_nAlarmType = pTmp->m_nAlarmType;
		//m_strDesc = pTmp->m_strAlarmDesc;
		m_strTagName = pTmp->m_strAlarmTagName;

		DumpModel(m_pModel, m_pModel->indexFromItem(m_pModel->invisibleRootItem()), pTmp->m_strAlarmTagName);
	}

	ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Clicked(const QModelIndex &)));
	connect(ui.pOK, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(ui.pCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
}

CUserVariableSelectAlarmDialog::~CUserVariableSelectAlarmDialog()
{

}

/*! \fn void CSelectAlarmDialog::InitAlarm()
********************************************************************************************************* 
** \brief CSelectAlarmDialog::InitAlarm 
** \details 
** \return void 
** \author gw
** \date 2016年6月13日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::InitAlarm(int nType)
{
	//报警
	auto strAlarm = tr("alarm list");
	CQuoteItem *itemAlarm = new CQuoteItem(strAlarm);
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);

	m_pModel->appendRow(itemAlarm);

	if (nType == AIALARM)
	{
		//模拟量报警
		CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
		itemAnalogAlarm->setData(FES_ANALOG_ALARM_ITEM, Qt::UserRole);
		itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
		itemAnalogAlarm->setData(reinterpret_cast<long long>(m_pFesData->m_pAnalogGroup), POINTERDATA);
		itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
		itemAnalogAlarm->setEditable(false);
		itemAlarm->appendRow(itemAnalogAlarm);

		InitAIAlarmGroup(itemAnalogAlarm, m_pFesData->m_pAnalogGroup);
	}
	else
	{
		//开关量报警
		CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
		itemDigtalAlarm->setData(FES_DIGTAL_ALARM_ITEM, Qt::UserRole);
		itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
		itemDigtalAlarm->setData(reinterpret_cast<long long>(m_pFesData->m_pDigitalGroup), POINTERDATA);
		itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
		itemDigtalAlarm->setEditable(false);
		itemAlarm->appendRow(itemDigtalAlarm);

		InitDIAlarmGroup(itemDigtalAlarm, m_pFesData->m_pDigitalGroup);
	}

	ui.treeView->expandAll();
}


bool CUserVariableSelectAlarmDialog::InitAIAlarmGroup(CQuoteItem *pItem, Config::CAnalogAlarmGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}



	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{

		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(FES_TYPE_ANALOG_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		InitAIAlarmGroup(newItem, var);
	}

	auto &arrItem = pGroup->GetItem();
	for each (auto var in arrItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *pItemAlarm = new CQuoteItem(var->m_szTagName);
			pItemAlarm->setData(var->m_szTagName, LASTVALUE);
			pItemAlarm->setData(FES_TYPE_ALARM_ANALOG_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			pItemAlarm->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pItemAlarm->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
			pItemAlarm->setIcon(QIcon(SCALE_PNG));
			pItemAlarm->setEditable(false);

			//InitAIOfflimit(pItemAlarm, var->m_arrAlarmOfflimit);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}


bool CUserVariableSelectAlarmDialog::InitDIAlarmGroup(CQuoteItem *pItem, Config::CDigitalAlarmGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}



	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{

		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(FES_TYPE_DIGTAL_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		InitDIAlarmGroup(newItem, var);
	}

	auto &arrItem = pGroup->GetItem();
	for each (auto var in arrItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *pItemAlarm = new CQuoteItem(var->m_szTagName);
			pItemAlarm->setData(var->m_szTagName, LASTVALUE);
			pItemAlarm->setData(FES_TYPE_ALARM_DIGITAL_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			pItemAlarm->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pItemAlarm->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
			pItemAlarm->setIcon(QIcon(SCALE_PNG));
			pItemAlarm->setEditable(false);

			//InitDIOfflimit(pItemAlarm, var->m_arrDigtalOfflimit);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

///*! \fn void CSelectAlarmDialog::InitAIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData)
//********************************************************************************************************* 
//** \brief CSelectAlarmDialog::InitAIAlarm 
//** \details 
//** \param pItem 
//** \param  
//** \param pGroup 
//** \param pFesData 
//** \return void 
//** \author gw
//** \date 2016年6月13日 
//** \note 
//********************************************************************************************************/
//void CSelectAlarmDialog::InitAIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData)
//{
//	Q_ASSERT(pItem);
//
//	//for (int32u i = 0; i < pFesData->m_arrAIAlarms.size(); i++)
//	//{
//	//	//模拟量告警
//	//	//名字
//	//	CQuoteItem *pAIAlarm = new CQuoteItem(pFesData->m_arrAIAlarms[i]->m_strDescription);
//	//	pAIAlarm->setData(FES_TYPE_ALARM_ANALOG_ITEM, Qt::UserRole);
//	//	//Tagname
//	//	pAIAlarm->setData(pFesData->m_arrAIAlarms[i]->m_szTagName, Qt::EditRole);
//	//	pAIAlarm->setData(reinterpret_cast<long long>(pFesData->m_arrAIAlarms[i]), POINTERDATA);
//	//	pAIAlarm->setIcon(QIcon(ALARM_PNG));
//	//	pAIAlarm->setEditable(false);
//
//	//	//判断是否有组
//	//	if (pGroup.contains(pFesData->m_arrAIAlarms[i]->m_strGroupName))
//	//	{
//	//		pGroup[pFesData->m_arrAIAlarms[i]->m_strGroupName]->appendRow(pAIAlarm);
//	//	}
//	//	else
//	//	{
//	//		pItem->appendRow(pAIAlarm);
//	//	}
//
//	//	InitAIOfflimit(pAIAlarm, pFesData->m_arrAIAlarms[i]->m_arrAlarmOfflimit);
//	//}
//}

/*! \fn void CSelectAlarmDialog::InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::ALARM_OFFLIMIT_AI *> &m_arrAlarmOfflimit
********************************************************************************************************* 
** \brief CSelectAlarmDialog::InitAIOfflimit 
** \details 
** \param pItem 
** \param m_arrAlarmOfflimit 
** \return void 
** \author gw
** \date 2016年6月13日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::CAinAlarmLimit *> &m_arrAlarmOfflimit)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < m_arrAlarmOfflimit.size(); i++)
	{
		CQuoteItem *pAIAlarm = new CQuoteItem(m_arrAlarmOfflimit[i]->m_szTagName);
		
		pAIAlarm->setData(FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, Qt::UserRole);
		pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_szTagName, LASTVALUE);

		//Tagname
		//pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_szTagName, TAGNAME);
		pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_strDescription, TREE_TOOL_TIP_ROLE);
		pAIAlarm->setData(reinterpret_cast<long long>(m_arrAlarmOfflimit[i]), POINTERDATA);
		pAIAlarm->setIcon(QIcon(ALARM_PNG));
		pAIAlarm->setEditable(false);

		pItem->appendRow(pAIAlarm);
	}
}

/*! \fn void CSelectAlarmDialog::InitDIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CSelectAlarmDialog::InitDIAlarm 
** \details 
** \param pItem 
** \param  
** \param pGroup 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年6月13日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::InitDIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < pFesData->m_arrDIAlarms.size(); i++)
	{
		//非线性转换
		//名字
		CQuoteItem *pDIAlarm = new CQuoteItem(pFesData->m_arrDIAlarms[i]->m_strDescription);
		pDIAlarm->setData(FES_TYPE_ALARM_DIGITAL_ITEM, Qt::UserRole);
		//Tagname
		//pDIAlarm->setData(pFesData->m_arrDIAlarms[i]->m_szTagName, TAGNAME);
		pDIAlarm->setData(reinterpret_cast<long long>(pFesData->m_arrDIAlarms[i]), POINTERDATA);
		pDIAlarm->setIcon(QIcon(ALARM_PNG));
		pDIAlarm->setEditable(false);

		//判断是否有组
		if (pGroup.contains(pFesData->m_arrDIAlarms[i]->m_strGroupName))
		{
			pGroup[pFesData->m_arrDIAlarms[i]->m_strGroupName]->appendRow(pDIAlarm);
		}
		else
		{
			pItem->appendRow(pDIAlarm);
		}

		InitDIOfflimit(pDIAlarm, pFesData->m_arrDIAlarms[i]->m_arrDigtalOfflimit);
	}
}

/*! \fn void CSelectAlarmDialog::InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::ALARM_OFFLIMIT_DI *> &m_arrOfflimit)
********************************************************************************************************* 
** \brief CSelectAlarmDialog::InitDIOfflimit 
** \details 
** \param pItem 
** \param m_arrOfflimit 
** \return void 
** \author gw
** \date 2016年6月13日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::CDinAlarmLimit *> &m_arrOfflimit)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < m_arrOfflimit.size(); i++)
	{
		CQuoteItem *pDIAlarm = new CQuoteItem(m_arrOfflimit[i]->Description);
		pDIAlarm->setData(FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, Qt::UserRole);
		//Tagname
		pDIAlarm->setData(m_arrOfflimit[i]->TagName, Qt::EditRole);
		pDIAlarm->setData(reinterpret_cast<long long>(m_arrOfflimit[i]), POINTERDATA);
		pDIAlarm->setIcon(QIcon(ALARM_PNG));
		pDIAlarm->setEditable(false);

		pItem->appendRow(pDIAlarm);
	}
}

/*! \fn void CSelectAlarmDialog::InitAIAlarm(Config::CAnalogAlarm *pItem)
********************************************************************************************************* 
** \brief CSelectAlarmDialog::InitAIAlarm 
** \details 
** \param pItem 
** \return void 
** \author gw
** \date 2016年6月16日 
** \note 
********************************************************************************************************/
//void CSelectAlarmDialog::InitAIAlarm(Config::CAnalogAlarm *pAnalogAlarmItem)
//{
//	while (ui.m_pTableAI->rowCount() > 0)
//	{
//		ui.m_pTableAI->removeRow(0);
//	}
//
//	QStringList list;
//	list.clear();
//	list << tr("ID") << tr("Description") << tr("TagName") << tr("Enable") << tr("OnQualityGood") << tr("DeadArea")
//		<< tr("DeadType") << tr("AlarmType") << tr("DelayAlarm");
//
//	ui.m_pTableAI->setColumnCount(9);
//	ui.m_pTableAI->setHorizontalHeaderLabels(list);
//
//	ui.m_pTableAI->insertRow(ui.m_pTableAI->rowCount());
//
//	ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::ID, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmItem->m_nID)));
//
//	ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::Description, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmItem->m_strDescription)));
//
//	ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::TagName, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmItem->m_szTagName)));
//
//	QTableWidgetItem *pItem = new QTableWidgetItem();
//	if (pAnalogAlarmItem->m_bEnable == true)
//	{
//		pItem->setCheckState(Qt::Checked);
//	}
//	else
//	{
//		pItem->setCheckState(Qt::Unchecked);
//	}
//
//	ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::Enable, pItem);
//
//	//
//	pItem = new QTableWidgetItem();
//	if (pAnalogAlarmItem->m_bEnable == true)
//	{
//		pItem->setCheckState(Qt::Checked);
//	}
//	else
//	{
//		pItem->setCheckState(Qt::Unchecked);
//	}
//
//	ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::OnQualityGood, pItem);
//
//	if (pAnalogAlarmItem->m_nDeadType == ABS)
//	{
//		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::DeadType, new QTableWidgetItem(QString("%1").arg(tr("abs"))));
//	}
//	else
//	{
//		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::DeadType, new QTableWidgetItem(QString("%1").arg(tr("absrange"))));
//	}
//
//	ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::DeadArea, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmItem->m_dbDeadArea)));
//
//	if (pAnalogAlarmItem->m_nAlarmType == AIALARM)
//	{
//		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::AlarmType, new QTableWidgetItem(QString("%1").arg(tr("ai alarm"))));
//	}
//	else
//	{
//		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AIALARMCOLUMN::AlarmType, new QTableWidgetItem(QString("%1").arg(tr("di alarm"))));
//	}
//
//
//	while (ui.m_pTableAIOfflimit->rowCount() > 0)
//	{
//		ui.m_pTableAIOfflimit->removeRow(0);
//	}
//
//	list.clear();
//	list << tr("ID") << tr("Description") << tr("TagName") << tr("Condition") << tr("Value")
//		<< tr("LowValue") << tr("WriteLevel") << tr("ReadLevel") << tr("DelayAlarm") << tr("Level")
//		<< tr("Commands") << tr("SoundFile") << tr("PlaySoundTimes") << tr("SupportAck") << tr("SupportDelete") << tr("PushPic") << tr("Pdr")
//		<< tr("PlaySound") << tr("Blink") << tr("LogAlarmWindow") << tr("AccidentHandle") << tr("AccidentPrint") << tr("LightPlate");
//
//	ui.m_pTableAIOfflimit->setColumnCount(23);
//	ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);
//
//	for (auto const &tmp : pAnalogAlarmItem->m_arrAlarmOfflimit)
//	{
//		ui.m_pTableAIOfflimit->insertRow(ui.m_pTableAIOfflimit->rowCount());
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitID, new QTableWidgetItem(QString("%1").arg(tmp->m_nID)));
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitDescription, new QTableWidgetItem(QString("%1").arg(tmp->m_nID)));
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitTagName, new QTableWidgetItem(QString("%1").arg(tmp->m_nID)));
//
//		if (tmp->m_nCondition == GREATEROREQUAL)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("greaterorequal"))));
//		}
//		else if (tmp->m_nCondition == LESSOREQUAL)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("lessorequal"))));
//		}
//		else if (tmp->m_nCondition == EQUAL)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("equal"))));
//		}
//		else if (tmp->m_nCondition == RATEDOWN)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("ratedown"))));
//		}
//		else if (tmp->m_nCondition == RATEUP)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("rateup"))));
//		}
//		else if (tmp->m_nCondition == NOTEQUAL)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("notequal"))));
//		}
//		else if (tmp->m_nCondition == BETWEEN)
//		{
//			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCondition, new QTableWidgetItem(QString("%1").arg(tr("between"))));
//		}
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitValue, new QTableWidgetItem(QString("%1").arg(tmp->m_dbValue)));
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitLowValue, new QTableWidgetItem(QString("%1").arg(tmp->m_dbLowValue)));
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitDelayAlarm, new QTableWidgetItem(QString("%1").arg(tmp->m_nDelayAlarm)));
//
//		//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitLevel, new QTableWidgetItem(QString("%1").arg(tmp->m_nPriority)));
//
//		//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitCommands, new QTableWidgetItem(tmp->m_strCommands));
//
//		//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitSoundFile, new QTableWidgetItem(tmp->m_strSoundFile));
//
//		//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitPlaySoundTimes, new QTableWidgetItem(QString("%1").arg(tmp->m_nPlaySoundTimes)));
//
//		//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::AIOfflimitPlaySoundTimes, new QTableWidgetItem(QString("%1").arg(tmp->PlaySoundTimes)));
//
//		QTableWidgetItem *pItem = new QTableWidgetItem();
//		if (tmp->m_bSupportAck == true)
//		{
//			pItem->setCheckState(Qt::Checked);
//		}
//		else
//		{
//			pItem->setCheckState(Qt::Unchecked);
//		}
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitSupportAck, pItem);
//
//
//		//
//		pItem = new QTableWidgetItem();
//		if (tmp->m_bSupportDelete == true)
//		{
//			pItem->setCheckState(Qt::Checked);
//		}
//		else
//		{
//			pItem->setCheckState(Qt::Unchecked);
//		}
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitSupportDelete, pItem);
//
//		ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, AIOfflimit::LimitLightPlate, pItem);
//	}
//}

/*! \fn void CSelectAlarmDialog::InitDIAlarm(Config::CDigtalAlarm *pDigtalAlarm)
********************************************************************************************************* 
** \brief CSelectAlarmDialog::InitDIAlarm 
** \details 
** \param pDigtalAlarm 
** \return void 
** \author gw
** \date 2016年6月16日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::InitDIAlarm(Config::CDigitalAlarm *pDigtalAlarm)
{
	Q_UNUSED(pDigtalAlarm);
	
	while (ui.m_pTableAI->rowCount() > 0)
	{
		ui.m_pTableAI->removeRow(0);
	}

	QStringList list;
	list.clear();
	list << tr("ID") << tr("Description") << tr("TagName") << tr("Enable") << tr("OnQualityGood") << tr("AlarmType") << tr("DelayAlarm");

	ui.m_pTableAI->setColumnCount(7);
	ui.m_pTableAI->setHorizontalHeaderLabels(list);
}

/*! \fn void CSelectAlarmDialog::Clicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CSelectAlarmDialog::Clicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年7月15日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::Clicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();
	QString strTagname = index.data(Qt::EditRole).toString();


	if (nType == FES_TYPE_ALARM_ANALOG_ITEM)
	{	
		while (ui.m_pTableAI->rowCount() > 0)
		{
			ui.m_pTableAI->removeRow(0);
		}

		ui.m_pTableAI->clear();

		while (ui.m_pTableAIOfflimit->rowCount() > 0)
		{
			ui.m_pTableAIOfflimit->removeRow(0);
		}

		ui.m_pTableAIOfflimit->clear();
		
		
		auto pAnalogAlarm = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalogAlarm);
		if (!pAnalogAlarm)
		{
			return;
		}
		
		QStringList list;
		list.clear();
		list << tr("TagName") << tr("Description") << tr("Enable") << tr("OnQualityGood") << tr("DeadType") << tr("DeadArea")
			<< tr("AlarmType");
		ui.m_pTableAI->setColumnCount(7);
		ui.m_pTableAI->setHorizontalHeaderLabels(list);

		ui.m_pTableAI->insertRow(ui.m_pTableAI->rowCount());
		QTableWidgetItem *pItem = nullptr;
		pItem = new QTableWidgetItem(pAnalogAlarm->m_szTagName);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, TagName, pItem);
		pItem = new QTableWidgetItem(pAnalogAlarm->m_strDescription);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Description, pItem);
		//enable
		pItem = new QTableWidgetItem();

		if (pAnalogAlarm->m_bEnable)
		{
			pItem->setCheckState(Qt::Checked);
		}
		else
		{
			pItem->setCheckState(Qt::Unchecked);
		}
		pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Enable, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_bOnQualityGood));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, OnQualityGood, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_dbDeadArea));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, DeadType, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_nDeadType));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, DeadArea, pItem);
		//pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_nAlarmType));
		//pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		//ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, 6, pItem);
		
		list.clear();
		list.clear();
		list << tr("TagName") << tr("Description") << tr("ID") << tr("Condition") << tr("Category") << tr("Value")
			<< tr("LowValue") << tr("DelayAlarm") << tr("Priority") << tr("SupportAck") << tr("SupportDelete") << tr("AckType") << tr("Log");

		ui.m_pTableAIOfflimit->setColumnCount(12);
		ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);
		for each (auto var in pAnalogAlarm->m_arrAlarmOfflimit)
		{
			Q_ASSERT(var);
			if (!var)
			{
				return;
			}

			auto pAnalogAlarmLimit = var;
			if (!pAnalogAlarmLimit)
			{
				ui.m_pTableAIOfflimit;
			}



			ui.m_pTableAIOfflimit->insertRow(ui.m_pTableAIOfflimit->rowCount());

			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_szTagName);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitTagName, pItem);

			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_strDescription);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDescription, pItem);

			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_nID));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitID, pItem);

			//condition
			auto bFlag = m_arrAnalogAlarmConditon.contains(pAnalogAlarmLimit->m_nCondition);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(m_arrAnalogAlarmConditon[pAnalogAlarmLimit->m_nCondition]);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCondition, pItem);
			//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, 3, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_nCondition)));
			
			//category
			bFlag = m_arrAlarmCategory.contains(pAnalogAlarmLimit->m_nCategory);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(m_arrAnalogAlarmConditon[pAnalogAlarmLimit->m_nCategory]);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCategory, pItem);

			//value
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_dbValue));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitValue, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_dbLowValue));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitLowValue, pItem);

			//
			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_nDelayAlarm);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDelayAlarm, pItem);

			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_nPriority);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitPriority, pItem);
			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_bSupportAck);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportAck, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_bSupportDelete));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportDelete, pItem);

			//AckType
			bFlag = m_arrAlarmCategory.contains(pAnalogAlarmLimit->m_nAckType);
			pItem = new QTableWidgetItem(QString("%1").arg(m_arrAlarmCategory[pAnalogAlarmLimit->m_nAckType]));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitAckType, pItem);
		}

		m_strTagName = pAnalogAlarm->m_szTagName;
		//m_strDesc = pAnalogAlarm->m_strDescription;
		m_nAlarmType = /*pAnalogAlarm->m_nAlarmType*/AIALARM;
	}
	else if (nType == FES_TYPE_ALARM_DIGITAL_ITEM)
	{
		while (ui.m_pTableAI->rowCount() > 0)
		{
			ui.m_pTableAI->removeRow(0);
		}

		ui.m_pTableAI->clear();

		while (ui.m_pTableAIOfflimit->rowCount() > 0)
		{
			ui.m_pTableAIOfflimit->removeRow(0);
		}

		ui.m_pTableAIOfflimit->clear();

		auto pDigitalAlarm = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarm);
		if (!pDigitalAlarm)
		{
			return;
		}

		QStringList list;
		list.clear();
		list << tr("TagName") << tr("Description") << tr("Enable") << tr("OnQualityGood");
		ui.m_pTableAI->setColumnCount(4);
		ui.m_pTableAI->setHorizontalHeaderLabels(list);

		ui.m_pTableAI->insertRow(ui.m_pTableAI->rowCount());
		QTableWidgetItem *pItem = nullptr;
		pItem = new QTableWidgetItem(pDigitalAlarm->m_szTagName);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, TagName, pItem);
		pItem = new QTableWidgetItem(pDigitalAlarm->m_strDescription);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Description, pItem);
		//enable
		pItem = new QTableWidgetItem();
		if (pDigitalAlarm->m_bEnable)
		{
			pItem->setCheckState(Qt::Checked);
		}
		else
		{
			pItem->setCheckState(Qt::Unchecked);
		}
		pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Enable, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pDigitalAlarm->m_bOnQualityGood));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, OnQualityGood, pItem);
		

		list.clear();
		list << tr("TagName") << tr("Description") << tr("ID") << tr("Condition") << tr("Category") << tr("Value")
			<< tr("LowValue") << tr("DelayAlarm") << tr("Priority") << tr("SupportAck") << tr("SupportDelete") << tr("AckType") << tr("Log");
		ui.m_pTableAIOfflimit->setColumnCount(12);
		ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);
		for each (auto var in pDigitalAlarm->m_arrDigtalOfflimit)
		{
			Q_ASSERT(var);
			if (!var)
			{
				return;
			}

			auto pDigitalAlarmLimit = var;
			if (!pDigitalAlarmLimit)
			{
				return;
			}

			ui.m_pTableAIOfflimit->insertRow(ui.m_pTableAIOfflimit->rowCount());

			pItem = new QTableWidgetItem(pDigitalAlarmLimit->TagName);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitTagName, pItem);

			pItem = new QTableWidgetItem(pDigitalAlarmLimit->Description);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDescription, pItem);

			pItem = new QTableWidgetItem(QString("%1").arg(pDigitalAlarmLimit->ID));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitID, pItem);

			//condition
			auto bFlag = m_arrAnalogAlarmConditon.contains(pDigitalAlarmLimit->Condition);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(m_arrAnalogAlarmConditon[pDigitalAlarmLimit->Condition]);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCondition, pItem);
			//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, 3, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_nCondition)));

			//category
			bFlag = m_arrAlarmCategory.contains(pDigitalAlarmLimit->Category);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(m_arrAnalogAlarmConditon[pDigitalAlarmLimit->Category]);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCategory, pItem);

			//value
			if (pDigitalAlarmLimit->Condition == DIAlarmContion::ZEROTOONE)
			{
				pItem = new QTableWidgetItem(QString("%1").arg(tr("ZEROTOONE")));
				pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
				ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitValue, pItem);
			}
			else
			{
				pItem = new QTableWidgetItem(QString("%1").arg(tr("ONETOZERO")));
				pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
				ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitValue, pItem);
			}

			pItem = new QTableWidgetItem(QString("%1").arg(""));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitLowValue, pItem);

			//
			pItem = new QTableWidgetItem(pDigitalAlarmLimit->DelayAlarm);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDelayAlarm, pItem);

			pItem = new QTableWidgetItem(pDigitalAlarmLimit->Priority);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitPriority, pItem);
			pItem = new QTableWidgetItem(pDigitalAlarmLimit->SupportAck);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportAck, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pDigitalAlarmLimit->SupportDelete));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportDelete, pItem);

			//AckType
			bFlag = m_arrAlarmCategory.contains(pDigitalAlarmLimit->AckType);
			pItem = new QTableWidgetItem(QString("%1").arg(m_arrAlarmCategory[pDigitalAlarmLimit->AckType]));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitAckType, pItem);
		}

		m_strTagName = pDigitalAlarm->m_szTagName;
		//m_strDesc = pDigitalAlarm->m_strDescription;
		m_nAlarmType = /*pDigitalAlarm->m_nAlarmType*/DIALARM;
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CSelectAlarmDialog::DumpModel 
** \details 
** \param pModel 
** \param top 
** \param s 
** \param nType 
** \return void 
** \author gw
** \date 2016年7月15日 
** \note 
********************************************************************************************************/
void CUserVariableSelectAlarmDialog::DumpModel(const QAbstractItemModel *pModel, const QModelIndex & top, const QString &s)
{
	QModelIndex Item;

	if (!top.isValid()) {

	}

	for (int r = 0; r < pModel->rowCount(top); r++)
	{
		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);

			const QString strTmp = Item.data(Qt::EditRole).toString();

			auto nType = Item.data(Qt::UserRole).toInt();
			if (s.compare(strTmp) == 0 && 
				(nType == FES_TYPE_ALARM_DIGITAL_ITEM || nType == FES_TYPE_ALARM_ANALOG_ITEM))
			{
				ui.treeView->setCurrentIndex(Item);

				//qDebug() << s;
			}

			//qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			DumpModel(pModel, Item, s);
		}
	}
}

void CUserVariableSelectAlarmDialog::OnOK()
{
	if (ui.m_pTableAI->rowCount() > 0)
	{
		accept();
	}
	else
	{
		rejected();
		this->close();
	}
}

void CUserVariableSelectAlarmDialog::OnCancel()
{
	rejected();

	this->close();
}

void CUserVariableSelectAlarmDialog::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = ui.treeView->indexAt(point);

	QMenu *pMenu = new QMenu(NULL);

	QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
	pMenu->addAction(pActionExpand);

	QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
	pMenu->addAction(pActionCollapse);

	QAction* selectedItem = pMenu->exec(QCursor::pos());
	if (selectedItem == pActionExpand)
	{
		ExpandChildren(indexSelect, ui.treeView);
	}
	else if (selectedItem == pActionCollapse)
	{
		CollapseChildren(indexSelect, ui.treeView);
	}

	pMenu->deleteLater();
}

void CUserVariableSelectAlarmDialog::InitPara()
{
	QStringList list;
	list.clear();
	list << tr("TagName") << tr("Description") << tr("Enable") << tr("OnQualityGood") << tr("DeadType") << tr("DeadArea")
		<< tr("AlarmType");
	ui.m_pTableAI->setColumnCount(7);
	ui.m_pTableAI->setHorizontalHeaderLabels(list);

	CONFIG_PARA para;

	//false 不可以修改checkbox
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this, false);
	ui.m_pTableAI->setItemDelegateForColumn(Enable, pCheck);

	ui.m_pTableAI->setItemDelegateForColumn(OnQualityGood, pCheck);

	auto delegate = new CLineEditDelegate(this, DOUBLETYPE, SIX_COUNT);
	ui.m_pTableAI->setItemDelegateForColumn(DeadArea, delegate);

	QVector<IDDESC> arrDeadType;
	arrDeadType.clear();

	IDDESC tmp;
	tmp.desc = tr("value");
	tmp.ID = QString("%1").arg(AnalogAlarmDeadArea::VALUE);
	arrDeadType.append(tmp);
	arrDeadType.append(tmp);

	tmp.desc = tr("percent");
	tmp.ID = QString("%1").arg(AnalogAlarmDeadArea::PERCENT);
	arrDeadType.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, arrDeadType);
	ui.m_pTableAI->setItemDelegateForColumn(DeadType, pCombo);


	list.clear();
	list << tr("TagName") << tr("Description") << tr("ID") << tr("Condition") << tr("Category") << tr("Value")
		<< tr("LowValue") << tr("DelayAlarm") << tr("Priority") << tr("SupportAck") << tr("SupportDelete") << tr("AckType") << tr("Log");
	ui.m_pTableAIOfflimit->setColumnCount(12);
	ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);


	//limit contidon
	m_arrAnalogAlarmConditon.clear();
	m_arrAnalogAlarmConditon.insert(AlarmContion::GREATEREQUAL, QObject::tr("GREATEREQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::LESSEQUAL, QObject::tr("LESSEQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::EQUAL, QObject::tr("EQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::NOTEQUAL, QObject::tr("NOTEQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::BWTEEN, QObject::tr("BWTEEN"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::RATEREDUCE, QObject::tr("RATEREDUCE"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::RATEREDUCE, QObject::tr("RATEINCREASE"));

	m_arrDigitalAlarmConditon.clear();
	m_arrDigitalAlarmConditon.insert(DIAlarmContion::ZEROTOONE, QObject::tr("ZEROTOONE"));
	m_arrDigitalAlarmConditon.insert(DIAlarmContion::ONETOZERO, QObject::tr("ONETOZERO"));

	m_arrAlarmCategory.clear();
	m_arrAlarmCategory.insert(AlarmCategory::ACCIDENT, QObject::tr("ACCIDENT"));
	m_arrAlarmCategory.insert(AlarmCategory::FAULT, QObject::tr("FAULT"));
	m_arrAlarmCategory.insert(AlarmCategory::COMMON, QObject::tr("COMMON"));

	//限值
	ui.m_pTableAIOfflimit->setItemDelegateForColumn(LimitSupportAck, pCheck);

	ui.m_pTableAIOfflimit->setItemDelegateForColumn(LimitSupportDelete, pCheck);

	//AckType
	m_arrAlarmAckType.clear();
	m_arrAlarmAckType.insert(AlarmAckType::DELETE, QObject::tr("DELETE"));
	m_arrAlarmAckType.insert(AlarmAckType::KEEP, QObject::tr("KEEP"));
	m_arrAlarmAckType.insert(AlarmAckType::CONFORM, QObject::tr("CONFORM"));
}