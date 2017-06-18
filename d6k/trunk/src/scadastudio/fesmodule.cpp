/*! @file
<PRE>
********************************************************************************
模块名       :  前置模块
文件名       :  fesmodel.cpp
文件实现功能 :  前置模块
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
*  @date    2015.12.8
*/

#include "fesmodule.h"
#include "scadastudio/uimgr.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "channel.h"
#include "alarm.h"
#include "scale.h"
#include "variable.h"
#include "hissave.h"
#include "pdr.h"
#include "forwarding.h"
#include "scadastudio/treewidget.h"
#include "xmlreader.h"
#include "datatypes.h"
#include "config_data.h"
//#include "uimodule/outputmessagerclient.h"
#include "fesview.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "scadastudio/iprojmgr.h"
#include "scadastudio/define.h"
#include "selectpointdialog.h"
#include "global.h"


#include <QMenu>
#include <QFile>
#include <QDebug>
#include <QMenubar>
#include <QVariant>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDir>
#include <QMessageBox>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CFesModel::CFesModel(ICoreui *pUi)
********************************************************************************************************* 
** \brief CFesModel::CFesModel 
** \details 创建前置模块
** \param pUi 
** \return  
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
CFesModule::CFesModule(IMainModuleInterface *pCore)
	:m_pTopFesItem(NULL), m_pTopProjectItem(NULL), m_pChannel(NULL), m_pAlarm(NULL), m_pScale(NULL), m_pVariable(NULL), m_pHisSave(NULL),
	m_pPdr(nullptr), m_pForwarding(nullptr), m_pCore(pCore), m_pStringPoolVec(nullptr), m_pHash(nullptr), m_pDescStringPoolOccNo(nullptr)
{	
 	if (m_pChannel == nullptr)
	{
		m_pChannel = new CChannel(m_pCore);
	}
	else
	{
		m_pChannel->DisConnect();
		delete m_pChannel;
		m_pChannel = nullptr;

		m_pChannel = new CChannel(m_pCore);
	}


	if (m_pAlarm == nullptr)
	{
		m_pAlarm = new CAlarm(m_pCore);
	}
	else
	{
		m_pAlarm->Disconnect();
		delete m_pAlarm;
		m_pAlarm = nullptr;

		m_pAlarm = new CAlarm(m_pCore);
	}

	if (m_pScale == nullptr)
	{
		m_pScale = new CScale(m_pCore);
	}
	else
	{
		m_pScale->Disconnect();
		delete m_pScale;
		m_pScale = nullptr;

		m_pScale = new CScale(m_pCore);
	}

	if (m_pVariable == nullptr)
	{
		m_pVariable = new CVariable(m_pCore);
	}
	else
	{
		m_pVariable->Disconnect();
		delete m_pVariable;
		m_pVariable = nullptr;

		m_pVariable = new CVariable(m_pCore);
	}

	if (m_pHisSave == nullptr)
	{
		m_pHisSave = new CHisSave(m_pCore->GetUIMgr());
	}
	else
	{
		//Disconnect signal slot
		m_pHisSave->Disconnect();
		delete m_pHisSave;
		m_pHisSave = nullptr;

		m_pHisSave = new CHisSave(m_pCore->GetUIMgr());
	}

	if (m_pPdr)
	{
		m_pPdr = new CPdr(m_pCore->GetUIMgr());
	}
	else
	{
		//Disconnect signal slot
		m_pPdr->Disconnect();
		delete m_pPdr;
		m_pPdr = nullptr;

		m_pPdr = new CPdr(m_pCore->GetUIMgr());
	}

	if (m_pForwarding == nullptr)
	{
		m_pForwarding = new CForwarding(m_pCore);
	}
	else
	{
		m_pForwarding->Disconnect();
		delete m_pForwarding;
		m_pForwarding = nullptr;

		m_pForwarding = new CForwarding(m_pCore);
	}

	m_pUi = m_pCore->GetUIMgr();
	
	
	//m_pChannel = new CChannel(pUi);

	//m_pAlarm = new CAlarm(pUi);

	//m_pScale = new CSacle(pUi);

	//m_pVariable = new CVariable(pUi);

	//m_pHisSave = new CHisSave(pUi);

	//m_pPdr = new CPdr(pUi);

//	setParent(pCore);

	m_pCore = pCore;

	m_pUi = pCore->GetUIMgr();

	m_pUi->GetAtrri();

	m_pModel = m_pUi->GetLeftTreeModel();

	connect(m_pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
	//QAction *pAct = new QAction("test", m_pUi->menuBar());
	//m_pUi->menuBar()->addAction(pAct);

	//connect(pAct, SIGNAL(triggered()), this, SLOT(Save()));

	//connect((QObject *)pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	////connect(pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	//connect((QObject *)pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));

	connect((QObject *)m_pCore->GetUIMgr()->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	//connect(pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	connect((QObject *)m_pCore->GetUIMgr()->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
}

/*! \fn CFesModel::~CFesModel()
********************************************************************************************************* 
** \brief CFesModel::~CFesModel 
** \details 
** \return  
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
CFesModule::~CFesModule()
{	
	//for (size_t i = 0; i < m_arrFes.size(); i++)
	//{
	//	delete m_arrFes[i];
	//}
	
	delete m_pConfig;
	m_pConfig = nullptr;

	//if (m_pChannel)
	//{
	//	m_pChannel->deleteLater();
	//}

	//if (m_pAlarm)
	//{
	//	m_pAlarm->deleteLater();
	//}

	//if (m_pScale)
	//{
	//	m_pScale->deleteLater();
	//}

	//if (m_pVariable)
	//{
	//	m_pVariable->deleteLater();
	//}

	//this->deleteLater();
}


/*! \fn void CFesModel::Init(ICoreui *pUi, CXmlReader *pXmlReader)
********************************************************************************************************* 
** \brief CFesModel::Init 
** \details 前置模块初始化
** \param pUi 
** \param pXmlReader 
** \return void 
** \author gw
** \date 2015年12月24日 
** \note 
********************************************************************************************************/
void CFesModule::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pConfig = new Config::CFesConfigData;

	////Q_ASSERT(pXmlReader);

	//QVector<QString> fesVec = pXmlReader->GetFesVec();

	//QXmlStreamReader xml;

	////前置组态
	//CQuoteItem *pTop = pUi->GetTopItem();

	//m_pTopFesItem = new CQuoteItem(QStringLiteral("前置配置"));
	//m_pTopFesItem->setData(FES_CONFIG, Qt::UserRole);
	//m_pTopFesItem->setIcon(QIcon(FES_PNG));
	//m_pTopFesItem->setEditable(false);

	//pTop->appendRow(m_pTopFesItem);

	//QMap<QString, CQuoteItem *> groupItem;

	//for (int i = 0; i < fesVec.size(); i++)
	//{
	//	QString strPath = "../project/test/scada/";

	//	strPath += fesVec[i];
	//
	//	QFile file(strPath);

	//	if (!file.open(QFile::ReadOnly | QFile::Text))
	//	{
	//		qDebug() << strPath << file.errorString();

	//		break;

	//		return;
	//	}

	//	xml.setDevice(&file);

	//	////前置组态
	//	//CQuoteItem *pTop = pUi->GetTopItem();

	//	//m_pTopFesItem = new CQuoteItem(QStringLiteral("前置配置"));
	//	//m_pTopFesItem->setData(FES_CONFIG, Qt::UserRole);
	//	//m_pTopFesItem->setIcon(QIcon(FES_PNG));
	//	//m_pTopFesItem->setEditable(false);

	//	//pTop->appendRow(m_pTopFesItem);

	//	if (xml.readNextStartElement())
	//	{
	//		QString strTmp = xml.name().toString();

	//		QString strTmp1 = xml.attributes().value("version").toString();

	//		if (xml.name() == "fes" && xml.attributes().value("version") == "1.0")
	//		{
	//			Config::CFesData *pFesData = new Config::CFesData();
	//			
	//			QString strTagName = xml.attributes().value("TagName").toString();
	//			pFesData->m_szTagName = strTagName;

	//			QString strDesc = xml.attributes().value("Description").toString();
	//			pFesData->m_strDescription = strDesc;

	//			QString strGroup = xml.attributes().value("GroupName").toString();
	//			pFesData->m_strGroupName = strGroup;

	//			int nID = xml.attributes().value("ID").toInt();
	//			pFesData->m_nID = nID;

	//			CQuoteItem *pTmp = m_pTopFesItem;

	//			//分组
	//			QStringList list = strGroup.split(".");

	//			if (strGroup.isEmpty())
	//			{
	//				//组空
	//			}
	//			else if (groupItem.contains(strGroup))
	//			{
	//				//已建立组
	//				pTmp = groupItem[strGroup];
	//			}
	//			else
	//			{
	//				QString strTmp;
	//				CQuoteItem *newItem;

	//				for (int32s i = 0; i < list.count(); i++)
	//				{
	//					strTmp += list[i];

	//					QString strTagName = list[i];

	//					newItem = new CQuoteItem(list[i]);
	//					newItem->setData(FES_GROUP_NAME, Qt::UserRole);
	//					newItem->setData(strTagName, TAGNAME);
	//					newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

	//					pTmp->appendRow(newItem);

	//					if (!groupItem.contains(strTmp))
	//					{
	//						groupItem.insert(strTmp, newItem);
	//					}

	//					pTmp = newItem;

	//					strTmp += ".";
	//				}
	//			}


	//			//添加前置节点
	//			CQuoteItem *pFesItem = new CQuoteItem(strDesc);
	//			pFesItem->setData(FES_ITEM, Qt::UserRole);
	//			pFesItem->setData(strTagName, TAGNAME);
	//			pFesItem->setIcon(QIcon(FES_PNG));
	//			pFesItem->setEditable(true);

	//			pTmp->appendRow(pFesItem);
	//			
	//			ReadFes(xml, pFesItem, pFesData);

	//			m_arrFes.push_back(pFesData);
	//		}
	//	}
	//}


	//m_pChannel = new CChannel(pUi, m_arrFes);

	//m_pAlarm = new CAlarm(pUi);

	//m_pScale = new CScale(pUi, m_arrFes);

	//m_pVariable = new CVariable(pUi);

	//m_pHisSave = new CHisSave(pUi);

	//m_pPdr = new CPdr(pUi);

	//m_pUi = pUi;

	//connect((QObject *)pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	////connect(pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	//connect((QObject *)pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));

	////CQuoteItem *pTop = pUi->GetTopItem();

	////m_pTopFesItem = new CQuoteItem(QStringLiteral("前置配置"));
	////m_pTopFesItem->setData(FES_CONFIG, Qt::UserRole);
	////m_pTopFesItem->setIcon(QIcon(FES_PNG));
	////m_pTopFesItem->setEditable(false);

	////pTop->appendRow(m_pTopFesItem);

	////AddSubFesItem(m_pTopFesItem);

	//return;


	////通道
	//CQuoteItem *itemChannel = new CQuoteItem(QStringLiteral("通道"));
	//itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	//itemChannel->setIcon(QIcon(CHANNEL_PNG));
	//itemChannel->setEditable(false);

	//pFesItem->appendRow(itemChannel);

	////报警
	//CQuoteItem *itemAlarm = new CQuoteItem(QStringLiteral("报警"));
	//itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	//itemAlarm->setIcon(QIcon(ALARM_PNG));
	//itemAlarm->setEditable(false);

	//pFesItem->appendRow(itemAlarm);

	////转换关系
	//CQuoteItem *itemScale = new CQuoteItem(QStringLiteral("转换"));
	//itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	//itemScale->setIcon(QIcon(SCALE_PNG));
	//itemScale->setEditable(false);

	//pFesItem->appendRow(itemScale);

	////变量
	//CQuoteItem *itemVariableList = new CQuoteItem(QStringLiteral("变量"));
	//itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	//itemVariableList->setIcon(QIcon(VARIABLE_PNG));
	//itemVariableList->setEditable(false);

	//pFesItem->appendRow(itemVariableList);

	////系统变量
	//CQuoteItem *itemSysVariableList = new CQuoteItem(QStringLiteral("系统变量"));
	//itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	//itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	//itemSysVariableList->setEditable(false);

	//itemVariableList->appendRow(itemSysVariableList);

	////用户变量
	//CQuoteItem *itemUserVariableList = new CQuoteItem(QStringLiteral("用户变量"));
	//itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	//itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	//itemUserVariableList->setEditable(false);

	////用户root
	//CQuoteItem *itemUserVariableRoot = new CQuoteItem(QStringLiteral("root"));
	//itemUserVariableRoot->setData(FES_USER_VARIBALE_CHILD_ITEM, Qt::UserRole);
	//itemUserVariableRoot->setIcon(QIcon(VARIABLE_USER_PNG));
	//itemUserVariableRoot->setEditable(false);

	//itemUserVariableList->appendRow(itemUserVariableRoot);

	//itemVariableList->appendRow(itemUserVariableList);

	////历史存储
	//CQuoteItem *itemHis = new CQuoteItem(QStringLiteral("历史存储"));
	//itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	//itemHis->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHis->setEditable(false);

	/////////////////////////////////////////////////////////////////////
	//CQuoteItem *itemHisTmp = new CQuoteItem(QStringLiteral("模拟量报表数据保存"));
	//itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHis->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	//itemHisTmp = new CQuoteItem(QStringLiteral("模拟量曲线数据保存"));
	//itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHisTmp->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	//itemHisTmp = new CQuoteItem(QStringLiteral("模拟量最值统计"));
	//itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHisTmp->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	//itemHisTmp = new CQuoteItem(QStringLiteral("模拟量越限值统计"));
	//itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHisTmp->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	//itemHisTmp = new CQuoteItem(QStringLiteral("模拟量累计值统计"));
	//itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHisTmp->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	//itemHisTmp = new CQuoteItem(QStringLiteral("开关量累计值统计"));
	//itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHis->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	//itemHisTmp = new CQuoteItem(QStringLiteral("电度量累计值统计"));
	//itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	//itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	//itemHisTmp->setEditable(false);

	//itemHis->appendRow(itemHisTmp);

	/////////////////////////////////////////////////////////////////////

	//pFesItem->appendRow(itemHis);

	////事故追忆
	//CQuoteItem *itemPdr = new CQuoteItem(QStringLiteral("事故追忆"));
	//itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	//itemPdr->setIcon(QIcon(PDR_PNG));
	//itemPdr->setEditable(false);

	//pFesItem->appendRow(itemPdr);
}

/*! \fn void CFesModel::UnInit()
********************************************************************************************************* 
** \brief CFesModel::UnInit 
** \details 模块卸载
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void CFesModule::UnInit()
{

}

#include <qdebug.h>
/*! \fn void CFesModule::AddSubFesItem(CQuoteItem *pItem)
********************************************************************************************************* 
** \brief CFesModule::AddSubFesItem 
** \details 
** \param pItem 
** \return void 
** \author gw
** \date 2016年9月6日 
** \note 
********************************************************************************************************/
void CFesModule::AddSubFesItem(CQuoteItem *pItem)
{
	using namespace Config;
	
	Q_ASSERT(m_pConfig);
	if (!m_pConfig)
	{
		auto strError = tr("add sub fes item config pointer is nullptr!!!");
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	Config::CFesData *pFes = nullptr;

	if (pItem->data(Qt::UserRole).toInt() == FES_CONFIG)
	{
		//FES ROOT
		pFes = m_pConfig->AddNewFesData(m_pConfig->m_pFesGroup);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
	}
	else if (pItem->data(Qt::UserRole).toInt() == FES_ITEM_GROUP)
	{
		auto pFesGroup = reinterpret_cast<Config::CFesGroup *>(pItem->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesGroup);
		if (!pFesGroup)
		{
			return;
		}
		
		pFes = m_pConfig->AddNewFesData(pFesGroup);
	}

	Q_ASSERT(pFes);
	if (!pFes)
	{
		auto strError = tr("add sub fes item pointer is nullptr!!!");
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}


	CQuoteItem *pFesItem = new CQuoteItem(pFes->m_szTagName);
	pFesItem->setData(pFes->m_szTagName, LASTVALUE);
	pFesItem->setData(FES_ITEM, Qt::UserRole);
	//pFesItem->setData(pFes->m_szTagName, TAGNAME);
	pFesItem->setData(reinterpret_cast<long long>(pFes), POINTERDATA);
	pFesItem->setIcon(QIcon(FES_PNG));
	pFesItem->setData(pFes->m_strDescription, TREE_TOOL_TIP_ROLE);
	pFesItem->setEditable(true);

	pItem->appendRow(pFesItem);
	QString strGroupName = CFesModule::GetGroupName(m_pUi->GetLeftTreeModel()->indexFromItem(pFesItem),
		FES_CONFIG);
	pFes->m_strGroupName = strGroupName;

	//通道
	auto strChannel = tr("channel list");
	CQuoteItem *itemChannel = new CQuoteItem(strChannel);
	itemChannel->setData(strChannel, LASTVALUE);
	//通道root节点 pFes->m_pChannelGroup 组
	itemChannel->setData(reinterpret_cast<long long>(pFes->m_pChannelGroup), POINTERDATA);
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(true);
	pFesItem->appendRow(itemChannel);

	//报警
	auto strAlarm = tr("alarm list");
	CQuoteItem *itemAlarm = new CQuoteItem(strAlarm);
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);

	pFesItem->appendRow(itemAlarm);

	//模拟量报警
	CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
	itemAnalogAlarm->setData(FES_ANALOG_ALARM_ITEM, Qt::UserRole);
	itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemAnalogAlarm->setData(reinterpret_cast<long long>(pFes->m_pAnalogGroup), POINTERDATA);
	itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
	itemAnalogAlarm->setEditable(false);
	itemAlarm->appendRow(itemAnalogAlarm);

	//开关量报警
	CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
	itemDigtalAlarm->setData(FES_DIGTAL_ALARM_ITEM, Qt::UserRole);
	itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemDigtalAlarm->setData(reinterpret_cast<long long>(pFes->m_pDigitalGroup), POINTERDATA);
	itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
	itemDigtalAlarm->setEditable(false);
	itemAlarm->appendRow(itemDigtalAlarm);

	//转换关系
	CQuoteItem *itemScale = new CQuoteItem(tr("scale list"));
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	itemScale->setIcon(QIcon(SCALE_PNG));
	itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemScale->setData(reinterpret_cast<long long>(pFes), POINTERDATA);
	itemScale->setEditable(false);

	pFesItem->appendRow(itemScale);

	//线性转换 非线性转换
	auto pLinear = new CQuoteItem(tr("linear node"));
	pLinear->setData(FES_SCALE_ITEM_LINEAR, Qt::UserRole);
	pLinear->setIcon(QIcon(SCALE_PNG));
	pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pLinear->setData(reinterpret_cast<long long>(pFes->m_pLinearTransformGroup), POINTERDATA);
	pLinear->setEditable(false);
	itemScale->appendRow(pLinear);
	auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	pNonLinear->setData(FES_SCALE_ITEM_NONLINEAR, Qt::UserRole);
	pNonLinear->setIcon(QIcon(SCALE_PNG));
	pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pNonLinear->setData(reinterpret_cast<long long>(pFes->m_pNonLinearTransformGroup), POINTERDATA);
	pNonLinear->setEditable(false);
	itemScale->appendRow(pNonLinear);


	//内存变量
	CQuoteItem *itemVariableList = new CQuoteItem(tr("memory variable"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setData(reinterpret_cast<long long>(pFes), POINTERDATA);
	itemVariableList->setIcon(QIcon(VARIABLE_PNG));
	itemVariableList->setEditable(false);

	pFesItem->appendRow(itemVariableList);

	//系统变量
	auto strTmp = tr("system variable");
	CQuoteItem *itemSysVariableList = new CQuoteItem(strTmp);
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setData(reinterpret_cast<long long>(pFes), POINTERDATA);
	itemSysVariableList->setData(strTmp, TREE_TOOL_TIP_ROLE);
	itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	itemSysVariableList->setEditable(false);


	itemVariableList->appendRow(itemSysVariableList);


	//用户变量
	strTmp = tr("user variable");
	CQuoteItem *itemUserVariableList = new CQuoteItem(strTmp);
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setData(reinterpret_cast<long long>(pFes->m_pUserVariableGroup), POINTERDATA);
	itemUserVariableList->setData(strTmp, TREE_TOOL_TIP_ROLE);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	//用户root
	strTmp = tr("root");
	CQuoteItem *itemUserVariableRoot = new CQuoteItem(strTmp);

	itemUserVariableRoot->setData(FES_USER_VARIBALE_GROUP_ITEM, Qt::UserRole);
	auto pGroup = new CUserVariableGroup;
	pGroup->m_strGroupName = "root";
	itemUserVariableRoot->setData(pGroup->m_strGroupName, LASTVALUE);
	pFes->m_pUserVariableGroup->AddUserVariableGroup(pGroup);
	itemUserVariableRoot->setData(reinterpret_cast<long long>(pGroup), POINTERDATA);
	itemUserVariableRoot->setData(strTmp, TREE_TOOL_TIP_ROLE);
	itemUserVariableRoot->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableRoot->setEditable(true);

	itemUserVariableList->appendRow(itemUserVariableRoot);

	itemVariableList->appendRow(itemUserVariableList);

	//历史存储
	CQuoteItem *itemHis = new CQuoteItem(tr("HistoricalDataStorage"));
	itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	itemHis->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	///////////////////////////////////////////////////////////////////
	//模拟量报表数据保存
	CQuoteItem *itemHisTmp = new CQuoteItem(tr("AiReportStorage"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量曲线数据保存
	itemHisTmp = new CQuoteItem(tr("AnalogCurveStorage"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量最值统计
	itemHisTmp = new CQuoteItem(tr("AnalogMaxminStatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量越限值统计
	itemHisTmp = new CQuoteItem(tr("AnalogLimitStatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量累计值统计
	itemHisTmp = new CQuoteItem(tr("AnalogCumulativeStatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//开关量累计值统计
	itemHisTmp = new CQuoteItem(tr("DigitalCumulativeStatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//电度量累计值统计
	itemHisTmp = new CQuoteItem(tr("KwhCumulativeStatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	///////////////////////////////////////////////////////////////////

	pFesItem->appendRow(itemHis);

	//事故追忆
	CQuoteItem *itemPdr = new CQuoteItem(tr("Pdr"));
	itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	itemPdr->setIcon(QIcon(PDR_PNG));
	itemPdr->setEditable(false);
	pFesItem->appendRow(itemPdr);

	//前置转发
	CQuoteItem *itemForwarding = new CQuoteItem(tr("Forwarding channel list"));
	itemForwarding->setData(FES_FORWARDING_CHANNEL_LIST, Qt::UserRole);
	Q_ASSERT(pFes->m_pForwardingChannelGroup);
	if (!pFes->m_pForwardingChannelGroup)
	{
		return;
	}
	itemForwarding->setData(reinterpret_cast<long long>(pFes->m_pForwardingChannelGroup), POINTERDATA);
	itemForwarding->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemForwarding->setIcon(QIcon(PDR_PNG));
	itemForwarding->setEditable(false);
	pFesItem->appendRow(itemForwarding);
}

/*! \fn void CFesModel::showMouseRightButton(const QPoint &point)
********************************************************************************************************* 
** \brief CFesModel::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2015年12月18日 
** \note 
********************************************************************************************************/
void CFesModule::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_CONFIG || nType == FES_ITEM_GROUP || nType == FES_ITEM)
	{
		ShowMenu(indexSelect);
	}

}

/*! \fn void CFesModel::itemChanged(QStandardItem *pItem)
********************************************************************************************************* 
** \brief CFesModel::itemChanged 
** \details 修改操作
** \param pItem 
** \return void 
** \author gw
** \date 2015年12月18日 
** \note 
********************************************************************************************************/
void CFesModule::itemChanged(QStandardItem *pItem)
{
	int nType = pItem->data(Qt::UserRole).toInt();

	if (nType == FES_ITEM)
	{
		auto strLastTagValue = pItem->data(LASTVALUE).toString();
		auto strTagValue = pItem->data(Qt::EditRole).toString();

		if (!CheckTagNameIsValid(strTagValue, FES_DESC) || !CheckTagNameIsValid(strLastTagValue, FES_DESC))
		{
			return;
		}

		if (strLastTagValue == strTagValue)
		{
			return;
		}

		//fes config判断修改的前置tagname是否存在
		if (m_pConfig->CheckModifyFesTagNameExist(strTagValue))
		{
			auto strError = tr("fes tag name %1 is exist!!!").arg(strTagValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			pItem->setData(strLastTagValue, Qt::EditRole);

			return;
		}

		//auto strTagName = pItem->data(TAGNAME).toString();
		auto pFes = reinterpret_cast<Config::CFesData *>(pItem->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (pFes == nullptr)
		{
			m_pCore->LogMsg(FES_DESC, LOG_STRING("can not find fes!!!"), LEVEL_1);

			return;
		}
		
		//前置单元内判断tagname是否存在
		if (pFes->CheckTagNameIsExist(strTagValue))
		{
			auto strError = tr("fes tag name %1 is exist!!!").arg(strTagValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			pItem->setData(strLastTagValue, Qt::EditRole);

			return;
		}

		if (!pFes->ModifyFesHashMap(HASHID::FESHASHID, strTagValue, strLastTagValue))
		{
			auto strError = tr("fes item modify fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			pItem->setData(strLastTagValue, Qt::EditRole);

			return;
		}

		//修改前置tagname
		if (!m_pConfig->m_pFesGroup->ModifyItem(strTagValue, strLastTagValue))
		{
			auto strError = tr("fes item modify fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			pItem->setData(strLastTagValue, Qt::EditRole);

			return;
		}
		pItem->setData(strTagValue, LASTVALUE);


		//if (strTagValue.compare(pFes->m_szTagName) != 0)
		{
			//memset(pFes->m_szTagName, 0, sizeof(pFes->m_szTagName));
			//auto nSize = strTagValue.size();
			//strncpy(pFes->m_szTagName, strTagValue.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));

			//前置管理hash tagname key修改
			if (!m_pConfig->ModifyFesHashMap(strTagValue, strLastTagValue))
			{
				auto strError = tr("fes item modify fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				pItem->setData(strLastTagValue, Qt::EditRole);

				return;
			}

			pFes->SetModifyFlag(true);

			//判断是否存在该widget
			//strLastTagValue fes tagname
			TabKeyStruct newKeyInfo;
			TabKeyStruct lastKeyInfo;
			newKeyInfo.BigTagName = strTagValue;
			newKeyInfo.TagName = strTagValue;
			newKeyInfo.Type = WIDGET_TYPE::FES_WIDGET_TYPE;
			lastKeyInfo.BigTagName = strLastTagValue;
			lastKeyInfo.TagName = strLastTagValue;
			lastKeyInfo.Type = WIDGET_TYPE::FES_WIDGET_TYPE;

			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagValue, WIDGET_TYPE::FES_WIDGET_TYPE, strLastTagValue);
			if (bFlag)
			{
				m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}

			//////////////////////////////////前置修改，和前置关联的模块也跟着修改/////////////////////////////////////////////////////
			std::vector<QString> vec;
			vec.clear();
			m_pCore->GetModulesName(vec);
			auto pNodeModule = m_pCore->GetModule("node_config");
			Q_ASSERT(pNodeModule);
			if (!pNodeModule)
			{
				return;
			}
			//前置tagname修改,节点模块关联的前置tagname也修改
			pNodeModule->ChangeTagNameNodify(strTagValue, strLastTagValue, Module_ID::NODE_MODULE_FES);


			//前置修改 tabwidget里和前置相关的map修改
			//通道 装置
			m_pUi->GetTabWidget()->ModifyFesBigTagName(strTagValue, strLastTagValue);

			///////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	else if (nType == FES_ITEM_GROUP)
	{
		auto strLastValue = pItem->data(LASTVALUE).toString();
		auto strValue = pItem->data(Qt::EditRole).toString();
		if(strLastValue == strValue)
		{
			return;
		}
		
		auto pFesGroupParent = reinterpret_cast<Config::CFesGroup *>(pItem->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesGroupParent);
		if (!pFesGroupParent)
		{
			auto strError = tr("group parent pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pFesGroup = reinterpret_cast<Config::CFesGroup *>(pItem->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesGroup);
		if (!pFesGroup)
		{
			auto strError = tr("group pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		if (pFesGroupParent->CheckModifyGroupNameExist(pFesGroup, strValue))
		{
			auto strError = tr("group name %1 is exist!!!").arg(strValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			pItem->setData(strLastValue, Qt::EditRole);
			
			return;
		}

		if (!pFesGroupParent->ModifyGroup(strValue, strLastValue))
		{
			auto strError = tr("group name %1 is modify fail!!!").arg(strValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			pItem->setData(strLastValue, Qt::EditRole);

			return;
		}


		pFesGroup->m_strGroup = strValue;
		pItem->setData(strValue, LASTVALUE);

		//修改分组,view的数据跟着修改
		QStandardItemModel *pModel = m_pCore->GetUIMgr()->GetLeftTreeModel();

		//auto index = m_pModel->indexFromItem(pItem);
		//if (!index.isValid())
		//{
		//	return;
		//}

		//auto indexParent = index.parent();
		//if (!indexParent.isValid())
		//{
		//	return;
		//}

		//if (CheckSlbingGroupExist(index))
		//{
		//	auto strError = QString(tr("group name is exist!!!"));
		//	m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//	pItem->setData(strLastValue, Qt::EditRole);

		//	return;
		//}

		QVector<QModelIndex> vec;

		//前置组组修改,遍历查找前置项
		CFesModule::GetItemMap(pModel, pModel->indexFromItem(pItem), FES_ITEM, vec);
		for (auto &tmp : vec)
		{
			auto pTmp = reinterpret_cast<Config::CFesData *>(tmp.data(POINTERDATA).toLongLong());
			Q_ASSERT(pTmp);
			if(pTmp == nullptr)
			{
				m_pCore->LogMsg(FES_DESC, LOG_STRING("can not find fes!!!"), LEVEL_1);
			
				return;
			}

			QString strGroupName = CFesModule::GetGroupName(tmp, FES_CONFIG);
			pTmp->m_strGroupName = strGroupName;

			pItem->setData(strValue, LASTVALUE);

			//判断是否存在该widget
			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(tmp.data(Qt::EditRole).toString(), WIDGET_TYPE::FES_WIDGET_TYPE);
			if (bFlag)
			{
				//m_pUi->GetTabWidget()->SetTabName(strTagName, FES_WIDGET_TYPE, strGroupName);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}
		}

		//m_pCore->GetUIMgr()->GetTabWidget()->
	}
}

void CFesModule::forEach(QMap<QString, QStandardItem *> map, QAbstractItemModel* model, QModelIndex parent) 
{
	for (int r = 0; r < model->rowCount(parent); ++r) 
	{
		QModelIndex index = model->index(r, 0, parent);
		QVariant name = index.data(Qt::EditRole);
		qDebug() << name;

		//遍历通道
		if (index.data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
		{
			m_arrIteChannelMap.insert(name.toString(), m_pModel->itemFromIndex(index));
		}

		// here is your applicable code
		if (model->hasChildren(index)) {
			forEach(map, model, index);
		}
	}
}

#include "scadastudio/base.h"

void CFesModule::Save()
{
	QMap<QString, QStandardItem *>::const_iterator ite = m_arrChannelMap.constBegin();

	//遍历通道
	forEach(m_arrChannelMap, m_pModel);

	//for (size_t i = 0; i < m_vecChannel.size(); i++)
	//{
	//	QString strTagName = m_vecChannel[i]->m_szTagName;

	//	if (m_arrIteChannelMap.contains(strTagName))
	//	{
	//		QString strGroup = FindChannelGroup(m_arrIteChannelMap[strTagName]);

	//		int nIndex = strGroup.indexOf(".");

	//		strGroup = strGroup.mid(nIndex + 1);

	//		m_vecChannel[i]->m_strGroupName = strGroup;
	//	}
	//	else
	//	{

	//	}
	//}

	for (int i = 0; i < m_pUi->GetTabWidget()->count(); i++)
	{
		IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->widget(i));

		pBase->Save();
	}



}

QString CFesModule::FindChannelGroup(QStandardItem *pItem)
{
	QString strTmp;
	
	//int nRow = m_pModel->rowCount();

	QObjectList list = m_pModel->children();

	//int nCos = m_pModel->columnCount();

	if (pItem->parent() && pItem->parent()->data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
	{
		strTmp = pItem->parent()->data(Qt::EditRole).toString();
		strTmp = FindChannelGroup(pItem->parent()) + "." + strTmp;
	}
	else
	{
		return "";
	}

	return strTmp;
}
#include <QHeaderView>
void CFesModule::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);

	//int nType = index.data(Qt::UserRole).toInt();

	if (index.data(Qt::UserRole).toInt() == FES_CONFIG)
	{
		QAction *pActionGroup = new QAction(GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		QAction *pAction = new QAction(FES_NAME, pMenu);

		pAction->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pAction);


		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pAction)
		{
			//添加前置
			AddFes(index);
		}
		else if (selectedItem == pActionGroup)
		{
			//添加前置组
			AddFesGroup(index);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_ITEM_GROUP)
	{
		QAction *pAction = new QAction(FES_NAME, pMenu);

		pAction->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pAction);

		QAction *pActionGroup = new QAction(GROUP_ITEM, pMenu);
		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionGroup);

		QAction *pGroupModifyAction = new QAction(RENAME_ITEM, pMenu);
		pGroupModifyAction->setIcon(QIcon(CHANNEL_PNG));
		pMenu->addAction(pGroupModifyAction);


		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);


		//删除
		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionGroup)
		{
			AddFesGroup(index);
		}
		else if (selectedItem == pAction)
		{
			//添加前置
			AddFes(index);
		}
		else if (selectedItem == pGroupModifyAction)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionDelete)
		{
			DeleteFesGroup(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_ITEM)
	{
		//配置前置
		QAction *pAction = new QAction(RENAME_ITEM, pMenu);

		//pAction->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pAction);


		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		//删除
		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);


		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pAction)
		{
			//ConfigFes(index);
			m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionDelete)
		{
			DeleteFesItem(index);
		}
	}

	pMenu->deleteLater();
}

void CFesModule::AddFes(QModelIndex &index)
{
	//添加通道
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	AddSubFesItem(item);

	m_pUi->GetLeftTree()->expand(index);
}

void CFesModule::AddFesGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	int nType = item->data(Qt::UserRole).toInt();

	if (nType == FES_ITEM_GROUP)
	{
		//auto nTest = item->data(GROUP_NUMBER_ROLE).toInt();
		//前置组
		if (item->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
		{
			auto pFesGroup = reinterpret_cast<Config::CFesGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pFesGroup);
			if (!pFesGroup)
			{
				return;
			}

			auto pFesGroupChild = pFesGroup->CreateGroup();
			Q_ASSERT(pFesGroupChild);
			if (!pFesGroupChild)
			{
				return;
			}


			QString strName = pFesGroupChild->m_strGroup;

			CQuoteItem *newItem = new CQuoteItem(strName);
			newItem->setData(strName, LASTVALUE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(FES_ITEM_GROUP, Qt::UserRole);
			newItem->setData(reinterpret_cast<long long>(pFesGroupChild), POINTERDATA);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}
	else
	{
		//前置根节点
		//根节点的m_pConfig->m_pFesGroup
		if (item->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
		{
			auto pFesGroup = m_pConfig->m_pFesGroup->CreateGroup();
			Q_ASSERT(pFesGroup);
			if (!pFesGroup)
			{
				return;
			}

			QString strName = pFesGroup->m_strGroup;

			CQuoteItem *newItem = new CQuoteItem(strName);
			newItem->setData(strName, LASTVALUE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(FES_ITEM_GROUP, Qt::UserRole);
			newItem->setData(reinterpret_cast<long long>(pFesGroup), POINTERDATA);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}
}

/*! \fn void CFesModule::DeleteFesGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::DeleteFesGroup 
** \details 删除前置组
** \param index 
** \return void 
** \author gw
** \date 2016年10月17日 
** \note 
********************************************************************************************************/
void CFesModule::DeleteFesGroup(QModelIndex &index)
{

	
	QMessageBox msgBox;
	msgBox.setText("If u delete the group, its childs will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the group?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret) 
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		Q_ASSERT(m_pModel);
		if (!m_pModel)
		{
			return;
		}

		auto pFesGroupParent = reinterpret_cast<Config::CFesGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesGroupParent);
		if (!pFesGroupParent)
		{
			return;
		}

		auto pFesGroup = reinterpret_cast<Config::CFesGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesGroup);
		if (!pFesGroup)
		{
			return;
		}

		QVector<QModelIndex> vec;
		vec.clear();
		GetItemMap(m_pModel, index, FES_ITEM, vec);
		for each (auto var in vec)
		{
			//
			DeleteFesModuleWidgetInTabWidget(var);
		}

		for each (auto var in vec)
		{
			
			auto bFlag = m_pConfig->DeleteHashFesItem(var.data().toString().toStdString());
			if (!bFlag)
			{
				auto strError = tr("delete fes item fail!!!");

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!pFesGroupParent->DeletGroup(pFesGroup, strTag))
		{
			auto strError = tr("delete group fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		}

		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pModel->itemFromIndex(index);
		//while (pTop->rowCount())
		//{
		//	itemsToBeDeleted << pTop->takeRow(0);
		//	//if the row removed has children:
		//	if (itemsToBeDeleted.at(0)->hasChildren())
		//	{
		//		carryItems << pTop;                 //put on the stack the current loopItem
		//		pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
		//	}
		//	//if current loopItem has no more rows but carryItems list is not empty:
		//	if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

		//}
		//qDeleteAll(itemsToBeDeleted);

		pTop->parent()->removeRow(pTop->row());


		break;
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn void CFesModule::DeleteFesItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::DeleteFesItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年10月19日 
** \note 
********************************************************************************************************/
void CFesModule::DeleteFesItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the group, its childs will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		Q_ASSERT(m_pModel);
		if (!m_pModel)
		{
			return;
		}

		auto pFesGroupParent = reinterpret_cast<Config::CFesGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesGroupParent);
		if (!pFesGroupParent)
		{
			return;
		}

		auto pFesItem = reinterpret_cast<Config::CFesData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFesItem);
		if (!pFesItem)
		{
			return;
		}

		auto szFesTag = index.data(Qt::EditRole).toString();
		//删除前置hash
		if (!m_pConfig->DeleteHashFesItem(szFesTag.toStdString()))
		{
			auto strError = tr("delete fes fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		DeleteFesModuleWidgetInTabWidget(index);
		

		//组内的前置项删除 删除前置内存
		if (!pFesGroupParent->DeletItem(pFesItem, szFesTag))
		{
			auto strError = tr("delete fes fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pNodeModule = m_pCore->GetModule("node_config");
		Q_ASSERT(pNodeModule);
		if (!pNodeModule)
		{
			return;
		}
		QString strDelete = "";
		pNodeModule->ChangeTagNameNodify(strDelete, szFesTag, Module_ID::FES);


		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pModel->itemFromIndex(index);
		//while (pTop->rowCount())
		//{
		//	itemsToBeDeleted << pTop->takeRow(0);
		//	//if the row removed has children:
		//	if (itemsToBeDeleted.at(0)->hasChildren())
		//	{
		//		carryItems << pTop;                 //put on the stack the current loopItem
		//		pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
		//	}
		//	//if current loopItem has no more rows but carryItems list is not empty:
		//	if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

		//}
		//qDeleteAll(itemsToBeDeleted);

		pTop->parent()->removeRow(pTop->row());

		//QVector<QModelIndex> vec;
		//vec.clear();
		//GetItemMap(m_pModel, index, FES_ITEM, vec);
		break;
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn bool CFesModule::CheckSlbingGroupExist(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::CheckSlbingGroupExist 
** \details 检查修改的组与兄弟节点是否有重复
** \param index 
** \return bool 
** \author gw
** \date 2016年10月17日 
** \note 
********************************************************************************************************/
bool CFesModule::CheckSlbingGroupExist(QModelIndex &index)
{
	QVector<QModelIndex> vec;

	int nRows = m_pModel->rowCount(index.parent());

	for (int i = 0; i < nRows; i++)
	{
		auto &indexTmp = index.parent().child(i, 0);
		if (index != indexTmp && indexTmp.data(Qt::UserRole).toInt() == FES_ITEM_GROUP)
		{
			if (indexTmp.data(Qt::EditRole).toString() == index.data(Qt::EditRole).toString())
			{
				return true;
			}
		}
	}
	
	return false;
}

///*! \fn void CFesModel::ReadFes(QXmlReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadFes 
//** \details 解析前置
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月24日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadFes(QXmlStreamReader &reader, CQuoteItem *pFesItem, Config::CFesData *pFesData)
//{
//	QString strTagName = reader.attributes().value("TagName").toString();
//
//	QString strDesc = reader.attributes().value("Description").toString();
//
//	while(!reader.atEnd())
//	{
//		reader.readNext();
//
//		if(reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if(strTmp == "channel")
//			{
//				ReadChannel(reader, pFesData);
//			}
//			else if (strTmp == "device")
//			{
//				ReadDevice(reader, pFesData);
//			}
//			else if(strTmp == "data")
//			{
// 				ReadData(reader, pFesData);
//			}
//			else if(strTmp == "transform")
//			{
//				ReadTransform(reader, pFesData);
//			}
//			else if(strTmp == "alarm")
//			{
//				ReadAlarm(reader, pFesData);
//			}
//		}
//	}
//
////	//初始化树分支
////	InitChannel(pFesItem, pFesData);
////
////	InitTransform(pFesItem, pFesData);
////
////	InitAlarm(pFesItem, pFesData);
//}

///*! \fn void CFesModel::ReadChannel(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadChannel 
//** \details 通道
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月24日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadChannel(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "c")
//			{
//				ReadC(reader, pFesData);
//			}
//		}
//		
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadC(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadC 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月24日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadC(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	while (!reader.atEnd())
//	{
//		if (reader.isStartElement())
//		{
//			int32u nOccNo = reader.attributes().value("OccNo").toInt();
//			Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//			if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//			{// LOG
//				m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//				reader.readNext();
//
//				continue;
//			}
//
//			QString strTagName = reader.attributes().value("TagName").toString();
//			Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//			if (strTagName.isEmpty() == true)
//			{
//				m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//				reader.readNext();
//
//				continue;
//			}
//
//			CChannelData *pChannel = new CChannelData(m_pCore);
//			
//			pChannel->SetOccNo(nOccNo);
//			//sprintf_s(pChannel->m_szTagName, TAGNAME_SIZE + 1, "%s", reader.attributes().value("TagName").toString().toStdString().c_str());
//			//pChannel->m_szTagName = reader.attributes().value("TagName").toString();
//			memset(pChannel->m_szTagName, 0, sizeof(pChannel->m_szTagName));
//			int nSize = strTagName.size();
//			strncpy(pChannel->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, nSize));
//
//			int32u nID = reader.attributes().value("ID").toUInt();
//			pChannel->m_nID = reader.attributes().value("ID").toUInt();
//
//			QString strDescription = reader.attributes().value("ChannelName").toString();
//			pChannel->m_strDescription = reader.attributes().value("ChannelName").toString();
//			
//			QString strDriverName = reader.attributes().value("DriverName").toString();
//			pChannel->m_CommPara.DriverName = reader.attributes().value("DriverName").toString();
//			
//			int32s nDriverPara = reader.attributes().value("DriverPara").toInt();
//			pChannel->m_CommPara.DriverParam = nDriverPara;
//
//			int32u nDriverType = reader.attributes().value("DriverType").toInt();
//			pChannel->m_CommPara.DriveType = reader.attributes().value("DriverType").toInt();
//
//			int8u nMedium = reader.attributes().value("Medium").toInt();
//			pChannel->m_CommPara.Medium = reader.attributes().value("Medium").toInt();
//
//			//QString strIP = reader.attributes().value("IP").toString();
//			//pChannel->m_CommPara.IP = reader.attributes().value("IP").toString();
//
//			//int32u nPort = reader.attributes().value("Port").toInt();
//			//pChannel->m_CommPara.Port = reader.attributes().value("Port").toInt();
//			//
//			//QString strCom = reader.attributes().value("COM").toString();
//			//pChannel->m_CommPara.Com = reader.attributes().value("COM").toString();
//			//
//			//int32u nBaud = reader.attributes().value("Baud").toInt();
//			//pChannel->m_CommPara.Baud = reader.attributes().value("Baud").toInt();
//
//			QString strExtend = reader.attributes().value("Extention").toString();
//			pChannel->m_strExtention = reader.attributes().value("Extention").toString();
//			
//			QString strGroupName = reader.attributes().value("GroupName").toString();
//			pChannel->m_strGroupName = reader.attributes().value("GroupName").toString();
//
//			//m_vecChannel.push_back(pChannel);
//
//			auto bTest = CHECK_LESS_ZERO_INT(nID);
//			if (!bTest)
//			{
//				auto strFesError = QString("ID %1 of channel %2 is invalid < 0").arg(nID).arg(strTagName);
//				m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//			}
//			if (strTagName.isEmpty())
//			{
//				auto strFesError = QString("channel %1 is empty").arg(strTagName);
//				m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//			}
//			else if (strTagName.count() > TAGNAME_SIZE)
//			{
//				auto strFesError = QString(tr("channel %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//				m_pCore->LogMsg(CHANNEL_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//			}
//
//			pFesData->m_arrChannels.push_back(pChannel);
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadDevice(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadDevice 
//** \details 装置
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月25日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadDevice(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "d")
//			{
//				ReadD(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadD(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadD 
//** \details 装置
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月25日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadD(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	while (!reader.atEnd())
//	{
//		if (reader.isStartElement())
//		{
//			int32u nOccNo = reader.attributes().value("OccNo").toInt();
//			Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//			if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//			{// LOG
//				m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//				reader.readNext();
//
//				continue;
//			}
//
//			QString strTagName = reader.attributes().value("TagName").toString();
//			Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//			if (strTagName.isEmpty() == true)
//			{
//				m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//				reader.readNext();
//
//				continue;
//			}
//			
//			CDevice *pDevice = new CDevice(m_pCore);
//			pDevice->SetOccNo(nOccNo);
//			//pDevice->m_szTagName = strTagName;
//			strncpy(pDevice->m_szTagName, strTagName.toStdString().c_str(), TAGNAME_SIZE);
//			pDevice->m_szTagName[TAGNAME_SIZE] = 0;
//			
//			int32u nID = reader.attributes().value("ID").toUInt();
//			pDevice->m_nID = nID;
//			
//			QString strDescription = reader.attributes().value("DeviceName").toString();
//			pDevice->m_strDescription = strDescription;
//			
//			int8u nType = reader.attributes().value("Type").toInt();
//			pDevice->m_nType = nType;
//
//			QString strDeviceAddr = reader.attributes().value("DeviceAddr").toString();
//			pDevice->m_strDeviceAddr = strDeviceAddr;
//			
//			int32u nPriority = reader.attributes().value("Priority").toInt();
//			pDevice->m_nPriority = nPriority;
//			
//			QString strExtent = reader.attributes().value("ExtentionParam").toInt();
//			pDevice->m_strExtentionParam = strExtent;
//
//			auto bFlag = CHECK_LESS_ZERO_INT(nID);
//			if (!bFlag)
//			{
//				auto strFesError = QString(tr("ID %1 of device %2 is invalid < 0")).arg(nID).arg(strTagName);
//				m_pCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//			}
//			if (strTagName.isEmpty())
//			{
//				auto strFesError = QString(tr("device %1 is empty")).arg(strTagName);
//				m_pCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//			}
//			else if (strTagName.count() > TAGNAME_SIZE)
//			{
//				auto strFesError = QString(tr("device %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//				m_pCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//			}
//
//			std::vector<CChannelData *>::iterator ite;
//			
//			bFlag = false;
//
//			////通道挂装置
//			//for (ite = pFesData->m_arrChannels.begin(); ite != pFesData->m_arrChannels.end(); ite++)
//			//{
//			//	if ((*ite)->GetOccNo() == pDevice->m_nChannelOccNo)
//			//	{
//			//		(*ite)->PushDevice(pDevice);
//
//			//		bFlag = true;
//
//			//		break;
//			//	}
//			//}
//
//			//if (!bFlag)
//			//{
//			//	delete pDevice;
//			//}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadData(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadData 
//** \details data
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月25日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadData(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//		
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "AIN")
//			{
//				ReadAIN(reader, pFesData);
//			}
//			else if (strTmp == "DIN")
//			{
//				ReadDIN(reader, pFesData);
//			}
//			else if (strTmp == "AOUT")
//			{
//				ReadAOUT(reader, pFesData);
//			}
//			else if (strTmp == "DOUT")
//			{
//				ReadDOUT(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAIN(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAIN 
//** \details 模拟量输入
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月25日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAIN(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "ai")
//			{
//				ReadAi(reader, pFesData);
//			}
//		}
//		else if(reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAi(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAi 
//** \details ai
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月25日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAi(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "ai")
//			{
//					
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO) ;
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				CAnalogInput *pAnalog = new CAnalogInput(nOccNo);
//				memset(pAnalog->m_szTagName, 0, sizeof(pAnalog->m_szTagName));
//				strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));
//				pAnalog->SetOccNo(nOccNo);
//		//		pAnalog->m_nOccNo = nOccNo;
//
//				
//				
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pAnalog->m_nID = nID;
//				
//				QString strDescription = reader.attributes().value("Description").toString();
//				pAnalog->m_strDescription = strDescription;
//				
//				int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
//				pAnalog->m_nChannelOccNo = nChannelOccNo;
//				
//				int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
//				pAnalog->m_nDeviceOccNo = nDeviceOccNo;
//				
//				int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
//				pAnalog->m_nBlockOccNo = nBlockOccNo;
//
//				fp64 dbInitValue = reader.attributes().value("InitValue").toDouble();
//				pAnalog->m_dbInitValue = dbInitValue;
//				
//				int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
//				pAnalog->m_nInitialQua = nInitialQua;
//				
//				int8u nDataSource = reader.attributes().value("DataSource").toInt();
//				pAnalog->m_nDataSource = nDataSource;
//
//				pAnalog->m_strScaleTagName = reader.attributes().value("TransformTagName").toString();
//				
//				pAnalog->m_strScaleDesc = reader.attributes().value("TransformDesc").toString();
//
//				pAnalog->m_nScaleType = reader.attributes().value("TransformType").toInt();
//
//				
//				//int32u nTransformOccNo = reader.attributes().value("TransformOccNo").toUInt();
//				//pAnalog->m_nScaleOccNo = nTransformOccNo;
//				
//				pAnalog->m_strAlarmTagName = reader.attributes().value("AlarmTagName").toString();
//				//pAnalog->m_nAlarmType = reader.attributes().value("AlarmType").toUInt();
//
//				pAnalog->m_strAlarmDesc = reader.attributes().value("AlarmDesc").toString();
//
//				
//				fp64 dbRangeL = reader.attributes().value("RangeL").toDouble();
//				pAnalog->m_dblRangeL = dbRangeL;
//				
//				fp64 dbRangeH = reader.attributes().value("RangeH").toDouble();
//				pAnalog->m_dblRangeH = dbRangeH;
//				
//				fp64 dbHighQua = reader.attributes().value("HighQua").toDouble();
//				pAnalog->m_dblHighQua = dbHighQua;
//				
//				fp64 dbLowQua = reader.attributes().value("LowQua").toDouble();
//				pAnalog->m_dblLowQua = dbLowQua;
//				
//				pAnalog->m_strExpress = reader.attributes().value("ExpressOccNo").toString();
//				
//				pAnalog->m_strUnit = reader.attributes().value("Unit").toString();
//
//				QString strPinLabel = reader.attributes().value("PinLabel").toString();
//				pAnalog->m_strPinLabel = strPinLabel;
//				
//				int8u nDataType = reader.attributes().value("DataType").toInt();
//				pAnalog->m_nDataType = nDataType;
//
//				int8u nSaveDisk = reader.attributes().value("SaveDisk").toUInt();
//				pAnalog->m_bSaveDisk = nSaveDisk;
//				
//				int32u nSaveDiskPeriod = reader.attributes().value("SaveDiskPeriod").toUInt();
//				pAnalog->m_nSaveDiskPeriod = nSaveDiskPeriod;
//				
//				int8u nSaveDB = reader.attributes().value("SaveDB").toUInt();
//				pAnalog->m_bSaveDB = nSaveDB;
//				
//				int32u nSaveDBPeriod = reader.attributes().value("SaveDBPeriod").toUInt();
//				pAnalog->m_nSaveDBPeriod = nSaveDBPeriod;
//				
//				fp64 dbSensitivity = reader.attributes().value("Sensitivity").toDouble();
//				pAnalog->m_dblSensitivity = dbSensitivity;
//				
//				int32u nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
//				pAnalog->m_nReferenceCount = nReferenceCount;
//				
//			
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of ai %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(AI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("ai %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(AI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("ai %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(AI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				//装置挂模拟量输入	
//				std::vector<CChannelData *>::iterator ite;
//
//				bool bTest = false;
//
//				//通道挂装置
//				for (ite = pFesData->m_arrChannels.begin(); ite != pFesData->m_arrChannels.end(); ite++)
//				{
//					//通道大排行号
//					if ((*ite)->GetOccNo() == pAnalog->m_nChannelOccNo)
//					{
//						CChannelData *pChannel = (*ite);
//						
//						std::vector<CDevice *>::iterator iteDevice;
//						std::vector <CDevice *> &tmp = pChannel->m_arrDevices;
//						
//						for (iteDevice = tmp.begin(); iteDevice != tmp.end(); iteDevice++)
//						{
//							CDevice *pDevice = (*iteDevice);
//							
//							//装置大排行号
//							if ((*iteDevice)->GetOccNo() == pAnalog->m_nDeviceOccNo)
//							{
//								//Push to device
//								(*iteDevice)->AddAnalogInput(pAnalog);
//								
//								bTest = true;
//
//								break;
//							}
//						}
//					
//						break;
//					}
//				}
//
//				if (!bTest)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("测试"), 1);
//				}
//
//			}
//
//			reader.readNext();
//		}
//		else if(reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void void  CFesModel::ReadDIN(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadDIN 
//** \details 数字量输入
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadDIN(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "di")
//			{
//				ReadDi(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadDi(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadDi 
//** \details 数字量输入
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadDi(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "di")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CDigitalInput *pDigtalInput = new CDigitalInput();
//				pDigtalInput->SetOccNo(nOccNo);
//				strncpy(pDigtalInput->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pDigtalInput->m_nID = nID;
//				
//				QString strDIName = reader.attributes().value("Description").toString();
//				pDigtalInput->m_strDescription = strDIName;
//
//				int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
//				pDigtalInput->m_nChannelOccNo = nChannelOccNo;
//				
//				int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
//				pDigtalInput->m_nDeviceOccNo = nDeviceOccNo;
//				
//				int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
//				pDigtalInput->m_nBlockOccNo = nBlockOccNo;
//
//				bool bInitValue = reader.attributes().value("InitValue").toInt();
//				pDigtalInput->m_bInitValue = bInitValue;
//				
//				int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
//				pDigtalInput->m_nInitialQua = nInitialQua;
//				
//				int8u nDataSource = reader.attributes().value("DataSource").toInt();
//				pDigtalInput->m_nDataSource = nDataSource;
//
//				pDigtalInput->m_nAlarmType = reader.attributes().value("AlarmType").toUInt();
//
//				pDigtalInput->m_nAlarmOccNo = reader.attributes().value("AlarmOccNo").toUInt();
//				
//				//fp64 dbRangeL = reader.attributes().value("RangeL").toDouble();
//				//fp64 dbRangeH = reader.attributes().value("RangeH").toDouble();
//				//fp64 dbHighQua = reader.attributes().value("HighQua").toDouble();
//				//fp64 dbLowQua = reader.attributes().value("LowQua").toDouble();
//				
//				int32u nExpressOccNo = reader.attributes().value("ExpressOccNo").toUInt();
//				pDigtalInput->m_nExpressOccNo = nExpressOccNo;
//				
//				//int32u nUnitNameOccNo = reader.attributes().value("UnitNameOccNo").toUInt();
//				//pDigtalInput->m_nUnitNameOccNo = nUnitNameOccNo;
//
//				QString strPinLabel = reader.attributes().value("PinLabel").toString();
//				pDigtalInput->m_strPinLabel = strPinLabel;
//				
//				int8u nDataType = reader.attributes().value("DataType").toInt();
//				pDigtalInput->m_nDataType = nDataType;
//
//				int8u nSaveDisk = reader.attributes().value("SaveDisk").toUInt();
//				pDigtalInput->m_bSaveDisk = nSaveDisk;
//				
//				int32u nSaveDiskPeriod = reader.attributes().value("SaveDiskPeriod").toUInt();
//				pDigtalInput->m_nSaveDiskPeriod = nSaveDiskPeriod;
//				
//				int8u nSaveDB = reader.attributes().value("SaveDB").toUInt();
//				pDigtalInput->m_bSaveDB = nSaveDB;
//				
//				int32u nSaveDBPeriod = reader.attributes().value("SaveDBPeriod").toUInt();
//				pDigtalInput->m_nSaveDBPeriod;
//				
//				//fp64 dbSensitivity = reader.attributes().value("Sensitivity").toDouble();
//				//pDigtalInput->m_dbSensitivity = dbSensitivity;
//				
//				int nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
//				pDigtalInput->m_nReferenceCount = nReferenceCount;
//				
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pDigtalInput->m_strGroupName = strGroupName;
//
//				int8u nBitString = reader.attributes().value("BitString").toUInt();
//				
//				
//				QString strBitCloseString = reader.attributes().value("BitCloseString").toString();
//				QString strBitOpenString = reader.attributes().value("BitOpenString").toString();
//			
//				QString strSOE = reader.attributes().value("SOE").toString();
//			
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of di %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(DI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("di %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(DI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("di %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(DI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				//装置挂数字量输入	
//				std::vector<CChannelData *>::iterator ite;
//
//				//通道挂装置
//				for (ite = pFesData->m_arrChannels.begin(); ite != pFesData->m_arrChannels.end(); ite++)
//				{
//					//通道大排行号
//					if ((*ite)->GetOccNo() == pDigtalInput->m_nChannelOccNo)
//					{
//						CChannelData *pChannel = (*ite);
//
//						std::vector<CDevice *>::iterator iteDevice;
//						std::vector <CDevice *> &tmp = pChannel->m_arrDevices;
//
//						for (iteDevice = tmp.begin(); iteDevice != tmp.end(); iteDevice++)
//						{
//							CDevice *pDevice = (*iteDevice);
//
//							//装置大排行号
//							if ((*iteDevice)->GetOccNo() == pDigtalInput->m_nDeviceOccNo)
//							{
//								//Push to device
//								(*iteDevice)->AddDigitalInput(pDigtalInput);
//
//								break;
//							}
//						}
//
//						break;
//					}
//				}
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAOUT(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAOUT 
//** \details 模拟量输出
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAOUT(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "ao")
//			{
//				ReadAo(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAo(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAo 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAo(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "ao")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CAnalogOutput *pAnalogOutput = new CAnalogOutput();
//				
//				pAnalogOutput->SetOccNo(nOccNo);
//				//pAnalogOutput->m_szTagName = strTagName;
//				strncpy(pAnalogOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pAnalogOutput->m_nID = nID;
//				
//				QString strDescription = reader.attributes().value("Description").toString();
//				pAnalogOutput->m_strDescription = strDescription;
//				
//				int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
//				pAnalogOutput->m_nChannelOccNo = nChannelOccNo;
//				
//				int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
//				pAnalogOutput->m_nDeviceOccNo = nDeviceOccNo;
//				
//				int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
//				pAnalogOutput->m_nBlockOccNo = nBlockOccNo;
//
//				fp64 dbInitValue = reader.attributes().value("InitValue").toDouble();
//				pAnalogOutput->m_dbInitValue = dbInitValue;
//				
//				int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
//				pAnalogOutput->m_nInitialQua = nInitialQua;
//				
//				//int8u nDataSource = reader.attributes().value("DataSource").toInt();
//				//pAnalogOutput->m_nDataSource = nDataSource;
//
//				int32u nTransformType = reader.attributes().value("TransformType").toInt();
//				pAnalogOutput->m_nScaleType = nTransformType;
//				
//				int32u nTransformOccNo = reader.attributes().value("TransformOccNo").toUInt();
//				pAnalogOutput->m_nScaleOccNo = nTransformOccNo;
//				
//				int32u nAlarmOccNo = reader.attributes().value("AlarmOccNo").toUInt();
//				pAnalogOutput->m_nAlarmOccNo = nAlarmOccNo;
//				
//				fp64 dbRangeL = reader.attributes().value("RangeL").toDouble();
//				pAnalogOutput->m_dbRangeL = dbRangeL;
//				
//				fp64 dbRangeH = reader.attributes().value("RangeH").toDouble();
//				pAnalogOutput->m_dbRangeH = dbRangeH;
//				
//				fp64 dbHighQua = reader.attributes().value("HighQua").toDouble();
//				pAnalogOutput->m_dbHighQua = dbHighQua;
//				
//				fp64 dbLowQua = reader.attributes().value("LowQua").toDouble();
//				pAnalogOutput->m_dbLowQua = dbLowQua;
//				
//				int32u nExpressOccNo = reader.attributes().value("ExpressOccNo").toUInt();
//				pAnalogOutput->m_nExpressOccNo = nExpressOccNo;
//				
//				int32u nUnitNameOccNo = reader.attributes().value("UnitNameOccNo").toUInt();
//				pAnalogOutput->m_nUnitNameOccNo = nUnitNameOccNo;
//
//				QString strPinLabel = reader.attributes().value("PinLabel").toString();
//				pAnalogOutput->m_strPinLabel = strPinLabel;
//				
//				int8u nDataType = reader.attributes().value("DataType").toInt();
//				pAnalogOutput->m_nDataType = nDataType;
//				
//				//QString strAOName = reader.attributes().value("AOName").toInt();
//				//pAnalogOutput->m_strAOName = strAOName;
//
//				int8u nSaveDisk = reader.attributes().value("SaveDisk").toUInt();
//				pAnalogOutput->m_bSaveDisk = nSaveDisk;
//				
//				int32u nSaveDiskPeriod = reader.attributes().value("SaveDiskPeriod").toUInt();
//				pAnalogOutput->m_bSaveDiskPeriod = nSaveDiskPeriod;
//				
//				int8u nSaveDB = reader.attributes().value("SaveDB").toUInt();
//				pAnalogOutput->m_bSaveDB = nSaveDB;
//				
//				int32u nSaveDBPeriod = reader.attributes().value("SaveDBPeriod").toUInt();
//				pAnalogOutput->m_nSaveDBPeriod;
//				
//				fp64 dbSensitivity = reader.attributes().value("Sensitivity").toDouble();
//				pAnalogOutput->m_dbSensitivity = dbSensitivity;
//				
//				int nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
//				pAnalogOutput->m_nReferenceCount = nReferenceCount;
//				
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pAnalogOutput->m_strGroupName = strGroupName;
//			
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of ao %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(AO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("ao %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(AO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("ao %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(AO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				
//
//				//装置挂模拟量输出	
//				std::vector<CChannelData *>::iterator ite;
//
//				//通道挂装置
//				for (ite = pFesData->m_arrChannels.begin(); ite != pFesData->m_arrChannels.end(); ite++)
//				{
//					//通道大排行号
//					if ((*ite)->GetOccNo() == pAnalogOutput->m_nChannelOccNo)
//					{
//						CChannelData *pChannel = (*ite);
//
//						std::vector<CDevice *>::iterator iteDevice;
//						std::vector <CDevice *> &tmp = pChannel->m_arrDevices;
//
//						for (iteDevice = tmp.begin(); iteDevice != tmp.end(); iteDevice++)
//						{
//							CDevice *pDevice = (*iteDevice);
//
//							//装置大排行号
//							if ((*iteDevice)->GetOccNo() == pAnalogOutput->m_nDeviceOccNo)
//							{
//								//Push to device
//								(*iteDevice)->AddAnalogOutput(pAnalogOutput);
//
//								break;
//							}
//						}
//
//						break;
//					}
//				}
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadDOUT(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadDOUT 
//** \details 数字量输出
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadDOUT(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "do")
//			{
//				ReadDo(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadDo(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadDo 
//** \details 数字量输出
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadDo(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "do")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CDigtalOutput *pDigitalOutput = new CDigtalOutput();
//
//				pDigitalOutput->SetOccNo(nOccNo);
//				//pDigitalOutput->m_szTagName = strTagName;
//				strncpy(pDigitalOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));
//				pDigitalOutput->m_szTagName[TAGNAME_SIZE] = 0;
//				
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pDigitalOutput->m_nID = nID;
//				
//				QString strDescription = reader.attributes().value("Description").toString();
//				pDigitalOutput->m_strDescription = strDescription;
//				
//				int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
//				pDigitalOutput->m_nChannelOccNo = nChannelOccNo;
//				
//				int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
//				pDigitalOutput->m_nDeviceOccNo = nDeviceOccNo;
//				
//				int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
//				pDigitalOutput->m_nBlockOccNo = nBlockOccNo;
//
//				fp64 dbInitValue = reader.attributes().value("InitValue").toDouble();
//				pDigitalOutput->m_dbInitValue = dbInitValue;
//				
//				int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
//				pDigitalOutput->m_nInitialQua = nInitialQua;
//				
//				int8u nDataSource = reader.attributes().value("DataSource").toInt();
//				pDigitalOutput->m_nDataSource = nDataSource;
//
//				int32u nAlarmOccNo = reader.attributes().value("AlarmOccNo").toUInt();
//				pDigitalOutput->m_nAlarmOccNo = nAlarmOccNo;
//				
//				int8u nIsSBO = reader.attributes().value("IsSBO").toInt();
//				pDigitalOutput->m_bIsSBO = nIsSBO;
//	
//				int32u nTelCtlWaitTime = reader.attributes().value("TelCtlWaitTime").toUInt();
//				pDigitalOutput->m_dbTelCtlWaitTime = nTelCtlWaitTime;
//				
//				int32u nTelCtlSendTime = reader.attributes().value("TelCtlSendTime").toUInt();
//				pDigitalOutput->m_dbTelCtlSendTime = nTelCtlSendTime;
//				
//				int32u nTelCtlExecTime = reader.attributes().value("TelCtlExecTime").toUInt();
//				pDigitalOutput->m_dbTelCtlExecTime = nTelCtlExecTime;
//				
//				int32u nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
//				pDigitalOutput->m_nReferenceCount = nReferenceCount;
//				
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pDigitalOutput->m_strGroupName = strGroupName;
//				
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of do %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(DO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("do %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(DO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("do %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(DO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				//装置数字量输出	
//				std::vector<CChannelData *>::iterator ite;
//
//				//通道挂装置
//				for (ite = pFesData->m_arrChannels.begin(); ite != pFesData->m_arrChannels.end(); ite++)
//				{
//					//通道大排行号
//					if ((*ite)->GetOccNo() == pDigitalOutput->m_nChannelOccNo)
//					{
//						CChannelData *pChannel = (*ite);
//
//						std::vector<CDevice *>::iterator iteDevice;
//						std::vector <CDevice *> &tmp = pChannel->m_arrDevices;
//
//						for (iteDevice = tmp.begin(); iteDevice != tmp.end(); iteDevice++)
//						{
//							CDevice *pDevice = (*iteDevice);
//
//							//装置大排行号
//							if ((*iteDevice)->GetOccNo() == pDigitalOutput->m_nDeviceOccNo)
//							{
//								//Push to device
//								(*iteDevice)->AddDigitalOutput(pDigitalOutput);
//
//								break;
//							}
//						}
//
//						break;
//					}
//				}
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadTransform(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadTransform 
//** \details 转换关系
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadTransform(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "linear")
//			{
//				ReadLinear(reader, pFesData);
//			}
//			else if(strTmp == "nonlinear")
//			{
//				ReadNonlinear(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadLinear(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadLinear 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadLinear(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "l")
//			{
//				ReadL(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadNonlinear(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadNonlinear 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadNonlinear(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "nl")
//			{
//				ReadNl(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadL(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadL 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadL(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "l")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CLinearTransform *pLinearTransform = new CLinearTransform();
//
//				pLinearTransform->SetOccNo(nOccNo);
//				memset(pLinearTransform->m_szTagName, 0, sizeof(pLinearTransform->m_szTagName));
//				strncpy(pLinearTransform->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pLinearTransform->m_nID = nID;
//				
//				QString strDescription = reader.attributes().value("Description").toString();
//				pLinearTransform->m_strDescription = strDescription;
//				
//				fp64 dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
//				pLinearTransform->m_dbMaxRaw = dbMaxRaw;
//				
//				fp64 dbMinRaw = reader.attributes().value("MinRaw").toDouble();
//				pLinearTransform->m_dbMinRaw = dbMinRaw;
//				
//				fp64 dbMaxScale = reader.attributes().value("MaxScale").toDouble();
//				pLinearTransform->m_dbMaxScale = dbMaxScale;
//				
//				fp64 dbMinScale = reader.attributes().value("MinScale").toDouble();
//				pLinearTransform->m_dbMinScale = dbMinScale;
//				
//				int8u nInverse = reader.attributes().value("Inverse").toInt();
//				pLinearTransform->m_bInverseFactor = nInverse;
//				
//				int8u nEnaleFactor = reader.attributes().value("EnaleFactor").toInt();
//				pLinearTransform->m_bEnableFactor = nEnaleFactor;
//
//				fp64 dbGain = reader.attributes().value("Gain").toDouble();
//				pLinearTransform->m_dbGain = dbGain;
//
//				fp64 dbOffset = reader.attributes().value("Offset").toDouble();
//				pLinearTransform->m_dbOffset = dbOffset;
//				
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pLinearTransform->m_strGroupName = strGroupName;
//			
//				pLinearTransform->m_nType = reader.attributes().value("ScaleType").toInt();
//
//
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of linear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("linear scale %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("linear scale %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(LINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				CTransform *pTmp = new CTransform;
//
//				pTmp->m_nType = pLinearTransform->m_nType;
//				memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
//				auto nSize = strlen(pLinearTransform->m_szTagName);
//				strncpy(pTmp->m_szTagName, pLinearTransform->m_szTagName, qMin(TAGNAME_SIZE, (int)nSize));
//
//				pTmp->SetLinear(pLinearTransform);
//
//				pFesData->m_arrTransforms.push_back(pTmp);
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadNl(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadNl 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadNl(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "nl")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CNonlinearTransform *pNonliear = new CNonlinearTransform();
//				
//				pNonliear->SetOccNo(nOccNo);
//				memset(pNonliear->m_szTagName, 0, sizeof(pNonliear->m_szTagName));
//				int nSize = strTagName.size();
//				strncpy(pNonliear->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, nSize));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pNonliear->m_nID = nID;
//				
//				QString strDescription = reader.attributes().value("Description").toString();
//				pNonliear->m_strDescription = strDescription;
//
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pNonliear->m_strGroupName = strGroupName;
//
//				pNonliear->m_nType = reader.attributes().value("ScaleType").toInt();
//
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of nonlinear scale %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(NONLINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("nonlinear scale %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(NONLINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("nonlinear scale %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(NONLINEAR_SCALE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				Readnl(reader, pNonliear);
//
//				CTransform *pTmp = new CTransform;
//				
//				pTmp->m_nType = pNonliear->m_nType;
//				memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
//				nSize = sizeof(pNonliear->m_szTagName);
//				strncpy(pTmp->m_szTagName, pNonliear->m_szTagName, qMin(TAGNAME_SIZE, nSize));
//
//				pTmp->SetNonLinear(pNonliear);
//
//				pFesData->m_arrTransforms.push_back(pTmp);
//			}
//
//			strTmp = reader.name().toString();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//		else
//		{
//			reader.readNext();
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadP(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadP 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadP(QXmlStreamReader &reader, Config::CNonlinearTransform *pData)
//{
//	using namespace Config;
//	
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "p")
//			{
//				NONLINEAR_SCALE *pNonScale = new NONLINEAR_SCALE;
//				
//				fp64 dbMaxRaw = reader.attributes().value("RawValue").toDouble();
//				pNonScale->RawValue = dbMaxRaw;
//				
//				fp64 dbMaxScale = reader.attributes().value("ScaleValue").toDouble();
//				pNonScale->ScaleValue = dbMaxScale;
//
//				pData->m_arrPNonliear.push_back(pNonScale);
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//		else
//		{
//			reader.readNext();
//		}
//	}
//}
//
///*! \fn void CFesModel::Readnl(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::Readnl 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月28日 
//** \note 
//********************************************************************************************************/
//void CFesModule::Readnl(QXmlStreamReader &reader, Config::CNonlinearTransform *pData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "p")
//			{
//				ReadP(reader, pData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			//reader.skipCurrentElement();
//
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAlarm(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAlarm 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月29日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAlarm(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//		
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "ain_alarm")
//			{
//				ReadAINAlarm(reader, pFesData);
//			}
//			else if (strTmp == "din_alarm")
//			{
//				ReadDINAlarm(reader, pFesData);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAINAlarm(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAINAlarm 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月29日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAINAlarm(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement() && reader.name() == "ain_alarm_offlimit")
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "ain_alarm_offlimit")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CAnalogAlarm *pAlarm = new CAnalogAlarm;
//				//模拟量报警
//				pAlarm->SetOccNo(nOccNo);
//				memset(pAlarm->m_szTagName, 0, sizeof(pAlarm->m_szTagName));
//				int nSize = strTagName.size();
//				strncpy(pAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, nSize));
//				
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pAlarm->m_nID = nID;
//				
//				QString strDescription = reader.attributes().value("Description").toString();
//				pAlarm->m_strDescription = strDescription;
//
//				int8u nEnable = reader.attributes().value("Enable").toInt();
//				pAlarm->m_bEnable = nEnable;
//
//				int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
//				pAlarm->m_bOnQualityGood = nOnQualityGood;
//				
//				int8u nDeadType = reader.attributes().value("DeadType").toUInt();
//				pAlarm->m_nDeadType = nDeadType;
//				
//				fp64 dbDeadArea = reader.attributes().value("DeadArea").toDouble();
//				pAlarm->m_dbDeadArea = dbDeadArea;
//
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pAlarm->m_strGroupName = strGroupName;
//
//				auto nAlarmType = reader.attributes().value("AlarmType").toInt();
//				pAlarm->m_nAlarmType = nAlarmType;
//
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of ai alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("ai alarm %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("ai alarm %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(AI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				ReadAa(reader, pAlarm);
//
//				pFesData->m_arrAIAlarms.push_back(pAlarm);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
//void CFesModule::ReadDINAlarm(QXmlStreamReader &reader, Config::CFesData *pFesData)
//{
//	using namespace Config;
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement() && reader.name() == "din_alarm_offlimit")
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "din_alarm_offlimit")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CDigtalAlarm *pDigitalAlarm = new CDigtalAlarm;
//
//				//数字量报警
//				pDigitalAlarm->SetOccNo(nOccNo);
//				strncpy(pDigitalAlarm->m_szTagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, strTagName.size()));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pDigitalAlarm->m_nID = nID;
//
//				QString strDescription = reader.attributes().value("Description").toString();
//				pDigitalAlarm->m_strDescription = strDescription;
//
//				int8u nEnable = reader.attributes().value("Enable").toInt();
//				pDigitalAlarm->m_bEnable = nEnable;
//
//				int8u nOnQualityGood = reader.attributes().value("OnQualityGood").toUInt();
//				pDigitalAlarm->m_bOnQualityGood = nOnQualityGood;
//
//				QString strGroupName = reader.attributes().value("GroupName").toString();
//				pDigitalAlarm->m_strGroupName = strGroupName;
//
//				auto nAlarmType = reader.attributes().value("AlarmType").toInt();
//				pDigitalAlarm->m_nAlarmType = nAlarmType;
//
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of di alarm %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("di alarm %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("di alarm %1 is more than %2")).arg(strTagName);
//					m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				ReadDa(reader, pDigitalAlarm);
//
//				pFesData->m_arrDIAlarms.push_back(pDigitalAlarm);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
///*! \fn void CFesModel::ReadAa(QXmlStreamReader &reader)
//********************************************************************************************************* 
//** \brief CFesModel::ReadAa 
//** \details 
//** \param reader 
//** \return void 
//** \author gw
//** \date 2015年12月29日 
//** \note 
//********************************************************************************************************/
//void CFesModule::ReadAa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm)
//{
//	while(!reader.atEnd())
//	{
//		reader.readNext();
//		
//		if (reader.isStartElement() && reader.name() == "aa")
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "aa")
//			{
//				Readaa(reader, pAlarm);
//			}
//		}
//		else if(reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
//void CFesModule::ReadDa(QXmlStreamReader &reader, Config::CDigtalAlarm *pDigital)
//{
//	while (!reader.atEnd())
//	{
//		reader.readNext();
//
//		if (reader.isStartElement() && reader.name() == "da")
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "da")
//			{
//				Readda(reader, pDigital);
//			}
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//	}
//}
//
//void CFesModule::Readaa(QXmlStreamReader &reader, Config::CAnalogAlarm *pAlarm)
//{
//	using namespace Config;
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "aa")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				CAinAlarmLimit *pAlarmOfflimit = new CAinAlarmLimit;
//				pAlarmOfflimit->OccNo = nOccNo;
//				memset(pAlarmOfflimit->TagName, 0, sizeof(pAlarmOfflimit->TagName));
//				int nSize = strTagName.size();
//				strncpy(pAlarmOfflimit->TagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, nSize));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pAlarmOfflimit->ID = nID;
//
//				QString strDescription = reader.attributes().value("Description").toString();
//				pAlarmOfflimit->Description = strDescription;
//
//				//QString strTimeFormat = reader.attributes().value("TimeFormat").toString();
//				//pAlarmOfflimit->TimeFormat = strTimeFormat;
//				
//				int8u nCondition = reader.attributes().value("Condition").toUInt();
//				pAlarmOfflimit->Condition = nCondition;
//				
//				fp64 dbValue = reader.attributes().value("Value").toDouble();
//				pAlarmOfflimit->Value = dbValue;
//				
//				fp64 dbLowValue = reader.attributes().value("LowValue").toDouble();
//				pAlarmOfflimit->LowValue = dbLowValue;
//
//				int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
//				pAlarmOfflimit->DelayAlarm = nDelayAlarm;
//				
//				pAlarmOfflimit->WriteAccessLevel = reader.attributes().value("WriteLevel").toInt();
//
//				pAlarmOfflimit->ReadAccessLevel = reader.attributes().value("ReadLevel").toInt();
//
//				int32u nLevel = reader.attributes().value("Level").toUInt();
//				pAlarmOfflimit->Level = nLevel;
//				
//				//告警方式
//				int32u nAlarmStyle = reader.attributes().value("AlarmStyle").toUInt();
//				pAlarmOfflimit->AlarmStyle = nAlarmStyle;
//				
//				int8u nSupportAck = reader.attributes().value("SupportAck").toUInt();
//				pAlarmOfflimit->SupportAck = nSupportAck;
//
//				int8u nSupportDelete = reader.attributes().value("SupportDelete").toUInt();
//				pAlarmOfflimit->SupportDelete = nSupportDelete;
//				
//				QString strSoundFile = reader.attributes().value("SoundFile").toString();
//				pAlarmOfflimit->SoundFile = strSoundFile;
//				
//				int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
//				pAlarmOfflimit->PlaySoundTimes = nPlaySoundTimes;
//				
//				QString strCommands = reader.attributes().value("Commands").toString();
//				pAlarmOfflimit->Commands = strCommands;
//				
//				QString strCommandsOn = reader.attributes().value("CommandsOn").toString();
//				pAlarmOfflimit->CommandsOn = strCommandsOn;
//				
//				QString strCommandsAck = reader.attributes().value("CommandsAck").toString();
//				pAlarmOfflimit->CommandsAck = strCommandsAck;
//				
//				QString strCommandsReset = reader.attributes().value("CommandsReset").toString();
//				pAlarmOfflimit->CommandsReset = strCommandsReset;
//				
//				QString strCommandsOff = reader.attributes().value("CommandsOff").toString();
//				pAlarmOfflimit->CommandsOff = strCommandsOff;
//
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of ai alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(AI_ALARM_LIMIT_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty() || strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("ai alarm limit %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(AI_ALARM_LIMIT_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				else if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("ai alarm limit %1 size is more than %2 ")).arg(strTagName).arg(TAGNAME_SIZE);
//					m_pCore->LogMsg(AI_ALARM_LIMIT_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				pAlarm->m_arrAlarmOfflimit.push_back(pAlarmOfflimit);
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//		else
//		{
//			reader.readNext();
//		}
//	}
//}
//
//void CFesModule::Readda(QXmlStreamReader &reader, Config::CDigtalAlarm *pDigital)
//{
//	using namespace Config;
//	while (!reader.atEnd())
//	{
//		QString strTmp = reader.name().toString();
//
//		if (reader.isStartElement())
//		{
//			QString strTmp = reader.name().toString();
//
//			if (strTmp == "da")
//			{
//				int32u nOccNo = reader.attributes().value("OccNo").toInt();
//				Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
//				if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
//				{// LOG
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("OccNo is invalid!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//
//				QString strTagName = reader.attributes().value("TagName").toString();
//				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
//				if (strTagName.isEmpty() == true)
//				{
//					m_pCore->LogMsg(FES_DESC, LOG_STRING("Tag name is empty!!!"), LEVEL_1);
//
//					reader.readNext();
//
//					continue;
//				}
//				
//				ALARM_OFFLIMIT_DI *pDigitalOfflimit = new ALARM_OFFLIMIT_DI;
//				pDigitalOfflimit->OccNo = nOccNo;
//				memset(pDigitalOfflimit->TagName, 0, sizeof(pDigitalOfflimit->TagName));
//				int nSize = strTagName.size();
//				strncpy(pDigitalOfflimit->TagName, strTagName.toStdString().c_str(), qMin(TAGNAME_SIZE, nSize));
//
//				int32u nID = reader.attributes().value("ID").toUInt();
//				pDigitalOfflimit->ID = nID;
//
//				QString strDescription = reader.attributes().value("Description").toString();
//				pDigitalOfflimit->Description = strDescription;
//
//				QString strTimeFormat = reader.attributes().value("TimeFormat").toString();
//				pDigitalOfflimit->TimeFormat = strTimeFormat;
//
//				int8u nCondition = reader.attributes().value("Condition").toUInt();
//				pDigitalOfflimit->Condition = nCondition;
//
//				int8u nValue = reader.attributes().value("Value").toDouble();
//				pDigitalOfflimit->Value = nValue;
//
//
//				int32u nDelayAlarm = reader.attributes().value("DelayAlarm").toInt();
//				pDigitalOfflimit->DelayAlarm = nDelayAlarm;
//
//				int32u nLevel = reader.attributes().value("Level").toUInt();
//				pDigitalOfflimit->Level = nLevel;
//
//				//告警方式
//				int32u nAlarmStyle0_1 = reader.attributes().value("AlarmStyle0_1").toUInt();
//				pDigitalOfflimit->AlarmStyle0_1 = nAlarmStyle0_1;
//
//				int32u nAlarmStyle1_0 = reader.attributes().value("AlarmStyle1_0").toUInt();
//				pDigitalOfflimit->AlarmStyle1_0 = nAlarmStyle1_0;
//
//				int8u nSupportAck = reader.attributes().value("SupportAck").toUInt();
//				pDigitalOfflimit->SupportAck = nSupportAck;
//
//				int8u nSupportDelete = reader.attributes().value("SupportDelete").toUInt();
//				pDigitalOfflimit->SupportDelete = nSupportDelete;
//
//				QString strSoundFile = reader.attributes().value("SoundFile").toString();
//				pDigitalOfflimit->SoundFile = strSoundFile;
//
//				int32u nPlaySoundTimes = reader.attributes().value("PlaySoundTimes").toUInt();
//				pDigitalOfflimit->PlaySoundTimes = nPlaySoundTimes;
//
//				QString strCommands = reader.attributes().value("Commands").toString();
//				pDigitalOfflimit->Commands = strCommands;
//
//				QString strCommandsOn = reader.attributes().value("CommandsOn").toString();
//				pDigitalOfflimit->CommandsOn = strCommandsOn;
//
//				QString strCommandsAck = reader.attributes().value("CommandsAck").toString();
//				pDigitalOfflimit->CommandsAck = strCommandsAck;
//
//				QString strCommandsReset = reader.attributes().value("CommandsReset").toString();
//				pDigitalOfflimit->CommandsReset = strCommandsReset;
//
//				QString strCommandsOff = reader.attributes().value("CommandsOff").toString();
//				pDigitalOfflimit->CommandsOff = strCommandsOff;
//
//				auto bFlag = CHECK_LESS_ZERO_INT(nID);
//				if (!bFlag)
//				{
//					auto strFesError = QString(tr("ID %1 of di alarm limit %2 is invalid < 0")).arg(nID).arg(strTagName);
//					m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.isEmpty())
//				{
//					auto strFesError = QString(tr("di alarm limit %1 is empty")).arg(strTagName);
//					m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//				if (strTagName.count() > TAGNAME_SIZE)
//				{
//					auto strFesError = QString(tr("di alarm limit %1 is more than %2")).arg(strTagName);
//					m_pCore->LogMsg(DI_ALARM_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
//				}
//
//				pDigital->m_arrDigtalOfflimit.push_back(pDigitalOfflimit);
//			}
//
//			reader.readNext();
//		}
//		else if (reader.isEndElement())
//		{
//			break;
//		}
//		else
//		{
//			reader.readNext();
//		}
//	}
//}

/*! \fn void CFesModel::AddChannel(CQuoteItem *pItem)
********************************************************************************************************* 
** \brief CFesModel::AddChannel 
** \details 
** \param pItem 
** \return void 
** \author gw
** \date 2016年1月11日 
** \note 
********************************************************************************************************/
void CFesModule::InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		auto strError = tr("fes is nullptr!!!");
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto strChannel = "channel list";
	//通道
	CQuoteItem *itemChannel = new CQuoteItem(strChannel);
	itemChannel->setData(strChannel, LASTVALUE);
	//channel node root group
	itemChannel->setData(reinterpret_cast<long long>(pFesData->m_pChannelGroup), POINTERDATA);
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(false);

	pItem->appendRow(itemChannel);

	InitChannelGroup(itemChannel, pFesData->m_pChannelGroup);
}


bool CFesModule::InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pGroup->m_arrChannelGroup.begin();
	for (; it != pGroup->m_arrChannelGroup.end(); it++)
	{
		auto strGroupName = it->second->m_strGroup;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(FES_CHANNEL_GROUP_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(it->second), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitChannelGroup(newItem, it->second);
	}

	auto ite = pGroup->m_arrChannelItem.begin();
	for (; ite != pGroup->m_arrChannelItem.end(); ++ite)
	{
		//添加前置节点
		auto pChannel = ite->second;
		CQuoteItem *pChannelItem = new CQuoteItem(pChannel->m_szTagName);
		pChannelItem->setData(pChannel->m_szTagName, LASTVALUE);
		pChannelItem->setData(FES_CHANNEL_CHILD_ITEM, Qt::UserRole);
		//pFesItem->setData(tmp->m_szTagName, TAGNAME);
		pChannelItem->setData((reinterpret_cast<std::size_t>(pChannel)), POINTERDATA);
		pChannelItem->setData(pChannel->m_strDescription, TREE_TOOL_TIP_ROLE);
		pChannelItem->setIcon(QIcon(FES_PNG));
		pChannelItem->setEditable(true);

		pItem->appendRow(pChannelItem);

		InitDevice(pChannelItem, pChannel->m_arrDevices);
	}

	return true;
}

/*! \fn void CFesModule::InitForwardingChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CFesModule::InitForwardingChannel 
** \details 
** \param pItem 
** \param pFesData 
** \return void 
** \author xingzhibing
** \date 2017年4月19日 
** \note 
********************************************************************************************************/
void CFesModule::InitForwardingChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		auto strError = tr("fes is nullptr!!!");
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto strChannel = QObject::tr("Forwarding channel list");
	//通道
	CQuoteItem *itemChannel = new CQuoteItem(strChannel);
	itemChannel->setData(strChannel, LASTVALUE);
	//channel node root group
	itemChannel->setData(reinterpret_cast<long long>(pFesData->m_pForwardingChannelGroup), POINTERDATA);
	itemChannel->setData(FES_FORWARDING_CHANNEL_LIST, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(false);

	pItem->appendRow(itemChannel);

	InitForwardingChannelGroup(itemChannel, pFesData->m_pForwardingChannelGroup);
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesModule::InitForwardingChannelGroup 
** \details 
** \param pItem 
** \param pGroup 
** \return bool 
** \author xingzhibing
** \date 2017年4月19日 
** \note 
********************************************************************************************************/
bool CFesModule::InitForwardingChannelGroup(CQuoteItem *pItem, Config::CForwardingChannelGroup *pGroup)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pGroup->m_arrGroup.begin();
	for (; it != pGroup->m_arrGroup.end(); it++)
	{
		auto strGroupName = it->second->m_strGroupName;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(FES_FORWARDING_CHANNEL_GROUP_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(it->second), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitForwardingChannelGroup(newItem, it->second);
	}

	auto ite = pGroup->m_arrItem.begin();
	for (; ite != pGroup->m_arrItem.end(); ++ite)
	{
		//添加通道节点
		auto pChannel = ite->second;
		CQuoteItem *pChannelItem = new CQuoteItem(pChannel->m_szTagName);
		pChannelItem->setData(pChannel->m_szTagName, LASTVALUE);
		pChannelItem->setData(FES_FORWARDING_CHANNEL_ITEM, Qt::UserRole);
		//pFesItem->setData(tmp->m_szTagName, TAGNAME);
		pChannelItem->setData((reinterpret_cast<std::size_t>(pChannel)), POINTERDATA);
		pChannelItem->setData(pChannel->m_strDescription, TREE_TOOL_TIP_ROLE);
		pChannelItem->setIcon(QIcon(FES_PNG));
		pChannelItem->setEditable(true);

		pItem->appendRow(pChannelItem);

		//遥测
		auto strTagNameAI = QString("%1_AI").arg(pChannel->m_szTagName);
		auto pAIItem = new CQuoteItem(strTagNameAI);
		pAIItem->setData(FES_FORWARDING_CHANNEL_AI_ITEM, Qt::UserRole);
		pAIItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		pAIItem->setData(strTagNameAI, LASTVALUE);
		pAIItem->setIcon(QIcon(CHANNEL_PNG));
		pChannelItem->appendRow(pAIItem);

		//遥信
		auto strTagNameDI = QString("%1_DI").arg(pChannel->m_szTagName);
		auto pDIItem = new CQuoteItem(strTagNameDI);
		pDIItem->setData(FES_FORWARDING_CHANNEL_DI_ITEM, Qt::UserRole);
		pDIItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		pDIItem->setData(strTagNameDI, LASTVALUE);
		pDIItem->setIcon(QIcon(CHANNEL_PNG));
		pChannelItem->appendRow(pDIItem);

		//遥控
		auto strTagNameDO = QString("%1_DO").arg(pChannel->m_szTagName);
		auto pDOItem = new CQuoteItem(strTagNameDO);
		pDOItem->setData(FES_FORWARDING_CHANNEL_DO_ITEM, Qt::UserRole);
		pDOItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		pDOItem->setData(strTagNameDO, LASTVALUE);
		pDOItem->setIcon(QIcon(CHANNEL_PNG));
		pChannelItem->appendRow(pDOItem);

		//遥调
		auto strTagNameAO = QString("%1_AO").arg(pChannel->m_szTagName);
		auto pAOItem = new CQuoteItem(strTagNameAO);
		pAOItem->setData(FES_FORWARDING_CHANNEL_AO_ITEM, Qt::UserRole);
		pAOItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		pAOItem->setData(strTagNameAO, LASTVALUE);
		pAOItem->setIcon(QIcon(CHANNEL_PNG));
		pChannelItem->appendRow(pAOItem);

		//调档
		auto strTagNameControlGear = QString("%1_CG").arg(pChannel->m_szTagName);
		auto pCGItem = new CQuoteItem(strTagNameControlGear);
		pCGItem->setData(FES_FORWARDING_CHANNEL_CG_ITEM, Qt::UserRole);
		pCGItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		pCGItem->setData(strTagNameControlGear, LASTVALUE);
		pCGItem->setIcon(QIcon(CHANNEL_PNG));
		pChannelItem->appendRow(pCGItem);

		//KWH
		auto strTagNameKWH = QString("%1_KWH").arg(pChannel->m_szTagName);
		auto pKWHItem = new CQuoteItem(strTagNameKWH);
		pKWHItem->setData(FES_FORWARDING_CHANNEL_KWH_ITEM, Qt::UserRole);
		pKWHItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		pKWHItem->setData(strTagNameKWH, LASTVALUE);
		pKWHItem->setIcon(QIcon(CHANNEL_PNG));
		pChannelItem->appendRow(pKWHItem);
	}

	return true;
}

/*! \fn void CFesModel::InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp)
********************************************************************************************************* 
** \brief CFesModel::InitDevice 
** \details 初始化装置
** \param pItem 
** \param pTmp 
** \return void 
** \author gw
** \date 2016年1月13日 
** \note 
********************************************************************************************************/
void CFesModule::InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	for (int32u i = 0; i < pTmp.size(); i++)
	{
		Q_ASSERT(pTmp[i]);
		if (!pTmp[i])
		{
			auto strError = tr("init device  device is nullptr!!!");
			m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//装置
		//装置名字
		CQuoteItem *newItem = new CQuoteItem(pTmp[i]->m_szTagName);

		newItem->setData(pTmp[i]->m_szTagName, LASTVALUE);
		newItem->setData(FES__CHANNEL_CHILD_DEVICE_ITEM, Qt::UserRole);
		//Tagname
		//newItem->setData(pTmp[i]->m_szTagName, TAGNAME);
		newItem->setData((reinterpret_cast<long long>(pTmp[i])), POINTERDATA);
		newItem->setData(pTmp[i]->m_strDescription, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(DEVICE_PNG));

		////////////////////////////////////////////////////////////////////////////
		QString strDesc = "ai";
		QString strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		strDesc = tr("%1").arg(strAITag);
		auto *pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		strDesc = "di";
		strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		strDesc = tr("%1").arg(strAITag);
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_DI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		strDesc = "ao";
		strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		strDesc = tr("%1").arg(strAITag);
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AO, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		strDesc = "do";
		strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_DO, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);
		////////////////////////////////////////////////////////////////////////////

		pItem->appendRow(newItem);
	}
}

void CFesModule::InitTransform(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}

	//转换关系
	auto strScale = tr("scale list");
	CQuoteItem *itemScale = new CQuoteItem(strScale);
	itemScale->setData(strScale, LASTVALUE);
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	//int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();
	itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemScale->setData(reinterpret_cast<long long>(pFesData), POINTERDATA);
	itemScale->setIcon(QIcon(SCALE_PNG));
	itemScale->setEditable(false);
	
	pItem->appendRow(itemScale);

	//线性转换 非线性转换
	auto pLinear = new CQuoteItem(tr("linear node"));
	pLinear->setData(FES_SCALE_ITEM_LINEAR, Qt::UserRole);
	pLinear->setIcon(QIcon(SCALE_PNG));
	pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pLinear->setData(reinterpret_cast<long long>(pFesData->m_pLinearTransformGroup), POINTERDATA);
	pLinear->setEditable(false);
	itemScale->appendRow(pLinear);
	auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	pNonLinear->setData(FES_SCALE_ITEM_NONLINEAR, Qt::UserRole);
	pNonLinear->setIcon(QIcon(SCALE_PNG));
	pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pNonLinear->setData(reinterpret_cast<long long>(pFesData->m_pNonLinearTransformGroup), POINTERDATA);
	pNonLinear->setEditable(false);
	itemScale->appendRow(pNonLinear);

	QMap<QString, CQuoteItem *> groupMap;
	QString strGroup;
	strGroup.clear();
	InitTransformGroup(pLinear, pFesData->m_pLinearTransformGroup);
	strGroup.clear();
	InitNonTransformGroup(pNonLinear, pFesData->m_pNonLinearTransformGroup);
}

bool CFesModule::InitTransformGroup(CQuoteItem *pItem, Config::CTransformGroup *pGroup)
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
		newItem->setData(FES_TYPE_SCALE_LINEAR_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		


		InitTransformGroup(newItem, var);
	}

	auto &arrLinearItem = pGroup->GetItem();
	for each (auto var in arrLinearItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *itemLinear = new CQuoteItem(var->m_pLinearScale->m_szTagName);
			itemLinear->setData(var->m_pLinearScale->m_szTagName, LASTVALUE);
			itemLinear->setData(FES_TYPE_SCALE_LINEAR_CHILD_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			itemLinear->setData(reinterpret_cast<long long>(var), POINTERDATA);
			itemLinear->setData(var->m_pLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);
			itemLinear->setIcon(QIcon(SCALE_PNG));
			itemLinear->setEditable(true);

			pItem->appendRow(itemLinear);
		}
	}

	return true;
}


bool CFesModule::InitNonTransformGroup(CQuoteItem *pItem, Config::CTransformGroup *pGroup)
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
		newItem->setData(FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitNonTransformGroup(newItem, var);
	}

	auto &arrNonLinearItem = pGroup->GetItem();
	for each (auto var in arrNonLinearItem)
	{
		{
			Q_ASSERT(var);
			Q_ASSERT(var->m_pNonLinearScale);
			//线性转换
			//名字
			CQuoteItem *itemLinear = new CQuoteItem(var->m_pNonLinearScale->m_szTagName);
			itemLinear->setData(var->m_pNonLinearScale->m_szTagName, LASTVALUE);
			itemLinear->setData(FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			itemLinear->setData(reinterpret_cast<long long>(var), POINTERDATA);
			itemLinear->setData(var->m_pNonLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);
			itemLinear->setIcon(QIcon(SCALE_PNG));
			itemLinear->setEditable(true);

			pItem->appendRow(itemLinear);
		}
	}

	return true;
}

void CFesModule::InitAlarm(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);
	Q_ASSERT(pFesData);

	//报警
	auto strAlarm = tr("alarm list");
	CQuoteItem *itemAlarm = new CQuoteItem(strAlarm);
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);

	pItem->appendRow(itemAlarm);

	//模拟量报警
	CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
	itemAnalogAlarm->setData(FES_ANALOG_ALARM_ITEM, Qt::UserRole);
	itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemAnalogAlarm->setData(reinterpret_cast<long long>(pFesData->m_pAnalogGroup), POINTERDATA);
	itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
	itemAnalogAlarm->setEditable(false);
	itemAlarm->appendRow(itemAnalogAlarm);

	//开关量报警
	CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
	itemDigtalAlarm->setData(FES_DIGTAL_ALARM_ITEM, Qt::UserRole);
	itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemDigtalAlarm->setData(reinterpret_cast<long long>(pFesData->m_pDigitalGroup), POINTERDATA);
	itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
	itemDigtalAlarm->setEditable(false);
	itemAlarm->appendRow(itemDigtalAlarm);

	InitAIAlarmGroup(itemAnalogAlarm, pFesData->m_pAnalogGroup);

	InitDIAlarmGroup(itemDigtalAlarm, pFesData->m_pDigitalGroup);
}


/*! \fn bool CFesModule::InitAIAlarmGroup(CQuoteItem *pItem, Config::CAnalogGroup *pGroup)
********************************************************************************************************* 
** \brief CFesModule::InitAIAlarmGroup 
** \details 初始化模拟量告警组
** \param pItem 
** \param pGroup 
** \return bool 
** \author gw
** \date 2016年11月10日 
** \note 
********************************************************************************************************/
bool CFesModule::InitAIAlarmGroup(CQuoteItem *pItem, Config::CAnalogAlarmGroup *pGroup)
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
			pItemAlarm->setEditable(true);

			//限值
			auto strLimitTagName = QString("%1_limit").arg(var->m_szTagName);
			auto strLimitDesc = QString(tr("%1_limit")).arg(var->m_szTagName);
			CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
			pLimitItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pLimitItem->setData(FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, Qt::UserRole);
			pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
			pLimitItem->setIcon(QIcon(ALARM_PNG));
			pItemAlarm->appendRow(pLimitItem);

			//InitAIOfflimit(pItemAlarm, var->m_arrAlarmOfflimit);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

void CFesModule::InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::CAinAlarmLimit *> &m_arrAlarmOfflimit)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < m_arrAlarmOfflimit.size(); i++)
	{
		CQuoteItem *pAIAlarm = new CQuoteItem(m_arrAlarmOfflimit[i]->m_szTagName);
		pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_szTagName, LASTVALUE);
		pAIAlarm->setData(FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, Qt::UserRole);
		//Tagname
		//pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_szTagName, TAGNAME);
		pAIAlarm->setData(reinterpret_cast<long long>(m_arrAlarmOfflimit[i]), POINTERDATA);
		pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_strDescription, TREE_TOOL_TIP_ROLE);
		pAIAlarm->setIcon(QIcon(ALARM_PNG));
		pAIAlarm->setEditable(true);

		pItem->appendRow(pAIAlarm);
	}
}

void CFesModule::InitDIAlarm(CQuoteItem *pItem, QMap<QString, CQuoteItem *> &pGroup, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < pFesData->m_arrDIAlarms.size(); i++)
	{
		//非线性转换
		//名字
		CQuoteItem *pDIAlarm = new CQuoteItem(pFesData->m_arrDIAlarms[i]->m_szTagName);
		pDIAlarm->setData(pFesData->m_arrDIAlarms[i]->m_szTagName, LASTVALUE);
		pDIAlarm->setData(FES_TYPE_ALARM_DIGITAL_ITEM, Qt::UserRole);
		//Tagname
		//pDIAlarm->setData(pFesData->m_arrDIAlarms[i]->m_szTagName, TAGNAME);
		pDIAlarm->setData(reinterpret_cast<long long>(pFesData->m_arrDIAlarms[i]), POINTERDATA);
		pDIAlarm->setData(pFesData->m_arrDIAlarms[i]->m_strDescription, TREE_TOOL_TIP_ROLE);
		pDIAlarm->setIcon(QIcon(ALARM_PNG));
		pDIAlarm->setEditable(true);

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


/*! \fn bool CFesModule::InitDIAlarmGroup(CQuoteItem *pItem, Config::CDigitalGroup *pGroup)
********************************************************************************************************* 
** \brief CFesModule::InitDIAlarmGroup 
** \details 初始化数字量告警组
** \param pItem 
** \param pGroup 
** \return bool 
** \author gw
** \date 2016年11月10日 
** \note 
********************************************************************************************************/
bool CFesModule::InitDIAlarmGroup(CQuoteItem *pItem, Config::CDigitalAlarmGroup *pGroup)
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
			pItemAlarm->setEditable(true);

			auto strLimitTagName = QString("%1_limit").arg(var->m_szTagName);
			auto strLimitDesc = QString(tr("%1_limit")).arg(var->m_szTagName);
			CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
			pLimitItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pLimitItem->setData(FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, Qt::UserRole);
			pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
			pLimitItem->setIcon(QIcon(ALARM_PNG));
			pItemAlarm->appendRow(pLimitItem);

			//InitDIOfflimit(pItemAlarm, var->m_arrDigtalOfflimit);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

void CFesModule::InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::CDinAlarmLimit *> &m_arrOfflimit)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < m_arrOfflimit.size(); i++)
	{
		CQuoteItem *pDIAlarm = new CQuoteItem(m_arrOfflimit[i]->TagName);
		pDIAlarm->setData(m_arrOfflimit[i]->TagName, LASTVALUE);
		pDIAlarm->setData(FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, Qt::UserRole);
		//Tagname
		//pDIAlarm->setData(m_arrOfflimit[i]->TagName, TAGNAME);
		pDIAlarm->setData(reinterpret_cast<long long>(m_arrOfflimit[i]), POINTERDATA);
		pDIAlarm->setData(m_arrOfflimit[i]->Description, TREE_TOOL_TIP_ROLE);
		pDIAlarm->setIcon(QIcon(ALARM_PNG));
		pDIAlarm->setEditable(true);

		pItem->appendRow(pDIAlarm);
	}
}


/*! \fn void CFesModule::InitVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CFesModule::InitVariable 
** \details 
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CFesModule::InitVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	//内存变量
	CQuoteItem *itemVariableList = new CQuoteItem(tr("memory variable"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(VARIABLE_PNG));
	itemVariableList->setEditable(false);

	pItem->appendRow(itemVariableList);


	InitSystemVariable(itemVariableList, pFesData);

	InitUserVariable(itemVariableList, pFesData);
}


/*! \fn void CFesModule::InitSystemVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CFesModule::InitSystemVariable 
** \details 
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CFesModule::InitSystemVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	//系统变量
	auto strTmp = tr("system variable");
	CQuoteItem *itemSysVariableList = new CQuoteItem(strTmp);
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setData(reinterpret_cast<long long>(pFesData), POINTERDATA);
	itemSysVariableList->setData(strTmp, TREE_TOOL_TIP_ROLE);
	itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	itemSysVariableList->setEditable(false);


	pItem->appendRow(itemSysVariableList);
}


/*! \fn void CFesModule::InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CFesModule::InitUserVariable 
** \details 
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CFesModule::InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}
	
	Q_ASSERT(pFesData->m_pUserVariableGroup);
	if (!pFesData->m_pUserVariableGroup)
	{
		return;
	}

	//用户变量
	CQuoteItem *itemUserVariableList = new CQuoteItem(tr("user variable"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setData(reinterpret_cast<long long>(pFesData->m_pUserVariableGroup), POINTERDATA);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	pItem->appendRow(itemUserVariableList);
	
	InitUserVariableGroup(itemUserVariableList, pFesData->m_pUserVariableGroup);
}


bool CFesModule::InitUserVariableGroup(CQuoteItem *pItem, Config::CUserVariableGroup *pGroup)
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
		newItem->setData(FES_USER_VARIBALE_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitUserVariableGroup(newItem, var);
	}

	//auto &arrUserVariableItem = pGroup->GetItem();
	//for each (auto var in arrUserVariableItem)
	//{
	//	CQuoteItem *itemUserVariable = new CQuoteItem(var->m_szTagName);
	//	itemUserVariable->setData(FES_USER_VARIBALE_CHILD_ITEM, Qt::UserRole);
	//	itemUserVariable->setData(reinterpret_cast<long long>(var), POINTERDATA);
	//	itemUserVariable->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
	//	itemUserVariable->setData(var->m_szTagName, LASTVALUE);

	//	itemUserVariable->setIcon(QIcon(VARIABLE_USER_PNG));
	//	itemUserVariable->setEditable(true);

	//	pItem->appendRow(itemUserVariable);
	//}

	return true;
}

//void CFesModel::GetOut(COutputMessagerClient *pClient)
//{
//	Q_ASSERT(pClient);
//
//	m_pClient = pClient;
//}

/*! \fn void CFesModel::ConfigFes(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModel::ConfigFes 
** \details 配置前置
** \param index 
** \return void 
** \author gw
** \date 2016年2月14日 
** \note 
********************************************************************************************************/
void CFesModule::ConfigFes(QModelIndex &index)
{
	//auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	m_pCore->GetUIMgr()->GetLeftTree()->edit(index);

	//int nType = pItem->data(Qt::UserRole).toInt();
	//m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
	//if (nType == FES_ITEM)
	//{
	//	auto strLastValue = pItem->data(LASTVALUE).toString();
	//	auto strValue = pItem->data(Qt::EditRole).toString();

	//	if (strLastValue == strValue)
	//	{
	//		return;
	//	}

	//	auto strTagName = pItem->data(TAGNAME).toString();
	//	auto pFes = reinterpret_cast<Config::CFesData *>(pItem->data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pFes);
	//	if (pFes == nullptr)
	//	{
	//		m_pCore->LogMsg(FES_DESC, LOG_STRING("can not find fes!!!"), LEVEL_1);

	//		return;
	//	}

	//	if (pFes->m_strDescription.compare(pItem->data(Qt::EditRole).toString()) != 0)
	//	{
	//		pFes->m_strDescription = pItem->data(Qt::EditRole).toString();

	//		pFes->SetModifyFlag(true);

	//		//判断是否存在该widget
	//		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_WIDGET_TYPE);
	//		if (bFlag)
	//		{
	//			m_pUi->GetTabWidget()->SetTabName(strTagName, FES_WIDGET_TYPE, pFes->m_strDescription);

	//			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
	//			Q_ASSERT(pBase);
	//			if (!pBase)
	//			{
	//				return;
	//			}

	//			pBase->Refresh();
	//		}
	//	}
	//}
}

//Config::CFesData *CFesModel::GetFesData(std::vector<Config::CFesData *> &arrFes, QString strTag, int nType)
//{
//	if (nType == FES_CHANNEL_CHILD_ITEM)
//	{
//		//通道所属前置
//		for (int32u i = 0; i < arrFes.size(); i++)
//		{
//			for (size_t j = 0; j < arrFes[i]->m_arrChannels.size(); j++)
//			{
//				if (arrFes[i]->m_arrChannels[j]->m_szTagName.compare(strTag) == 0)
//				{
//					return arrFes[i];
//				}
//			}
//		}
//	}
//	else if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
//	{
//		//转换所属前置
//		for (int32u i = 0; i < arrFes.size(); i++)
//		{
//			for (size_t j = 0; j < arrFes[i]->m_arrTransforms.size(); j++)
//			{
//				if (arrFes[i]->m_arrTransforms[j]->m_nType == ScaleType::LINEAR)
//				{
//					if (arrFes[i]->m_arrTransforms[j]->m_pLinearScale->m_szTagName.compare(strTag) == 0)
//					{
//						return arrFes[i];
//					}
//				}
//			}
//		}
//	}
//	else if (nType == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
//	{
//		//转换所属前置
//		for (int32u i = 0; i < arrFes.size(); i++)
//		{
//			for (size_t j = 0; j < arrFes[i]->m_arrTransforms.size(); j++)
//			{
//				if (arrFes[i]->m_arrTransforms[j]->m_nType == ScaleType::NONLINEAR)
//				{
//					if (arrFes[i]->m_arrTransforms[j]->m_pNonLinearScale->m_szTagName.compare(strTag) == 0)
//					{
//						return arrFes[i];
//					}
//				}
//			}
//		}
//	}
//	else if (nType == FES_ITEM)
//	{
//		for (int32u i = 0; i < arrFes.size(); i++)
//		{
//			if (arrFes[i]->m_szTagName.compare(strTag) == 0)
//			{
//				return arrFes[i];
//			}
//		}
//	}
//	else if (nType == FES__CHANNEL_CHILD_DEVICE_ITEM)
//	{
//		for (auto const &pFes : arrFes)
//		{
//			for(auto const &pChannel : pFes->m_arrChannels)
//			{
//				for (auto const &pDevice : pChannel->m_arrDevices)
//				{
//					if (pDevice->m_szTagName == strTag)
//					{
//						return pFes;
//					}
//				}
//			}
//		}
//	}
//
//	return NULL;
//}

///*! \fn QString CFesModel::GetFesTagname(QModelIndex &index)
//********************************************************************************************************* 
//** \brief CFesModel::GetFesTagname 
//** \details 查找所属fes tagname
//** \param index 
//** \return QT_NAMESPACE::QString 
//** \author gw
//** \date 2016年6月27日 
//** \note 
//********************************************************************************************************/
//QString CFesModule::GetTagname(const QModelIndex &index, const int nType)
//{
//	QModelIndex indexTmp = index;
//	
//	while (indexTmp.parent().isValid())
//	{
//		if (indexTmp.parent().data(Qt::UserRole) == nType)
//		{
//			return indexTmp.parent().data(TAGNAME).toString();
//		}
//
//		indexTmp = indexTmp.parent();
//	}
//
//	return "";
//}

///*! \fn Config::CFesData *CFesModel::GetFes(std::vector<Config::CFesData *> &arrFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetFes 
//** \details fes
//** \param arrFes 
//** \param strTag 
//** \return Config::CFesData * 
//** \author gw
//** \date 2016年6月27日 
//** \note 
//********************************************************************************************************/
//Config::CFesData *CFesModule::GetFes(std::vector<Config::CFesData *> &arrFes, QString strTag)
//{
//	for (int32u i = 0; i < arrFes.size(); i++)
//	{
//		if (arrFes[i]->m_szTagName == strTag)
//		{
//			return arrFes[i];
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::CLinearTransform *CFesModel::GetFesLinear(Config::CFesData *pFes, QString &strTag, int nType)
//********************************************************************************************************* 
//** \brief CFesModel::GetFesLinear 
//** \details 获得线性转换
//** \param pFes 
//** \param strTag 
//** \param nType 
//** \return Config::CLinearTransform * 
//** \author gw
//** \date 2016年4月19日 
//** \note 
//********************************************************************************************************/
//Config::CLinearTransform *CFesModule::GetFesLinear(Config::CFesData *pFes, QString &strTag, int nType)
//{
//	Q_ASSERT(pFes);
//	if (!pFes)
//	{
//		return nullptr;
//	}
//
//	if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
//	{
//		for (size_t i = 0; i < pFes->m_arrTransforms.size(); i++)
//		{
//			if (pFes->m_arrTransforms[i]->m_nType == ScaleType::LINEAR)
//			{
//				if (strTag.compare(pFes->m_arrTransforms[i]->m_pLinearScale->m_szTagName) == 0)
//				{
//					return pFes->m_arrTransforms[i]->m_pLinearScale;
//				}
//			}
//		}
//	}
//	
//	return NULL;
//}

///*! \fn Config::CNonlinearTransform *CFesModel::GetFesNonLinear(Config::CFesData *pFes, QString &strTag, int nType)
//********************************************************************************************************* 
//** \brief CFesModel::GetFesNonLinear 
//** \details 获得非线性转换
//** \param pFes 
//** \param strTag 
//** \param nType 
//** \return Config::CNonlinearTransform * 
//** \author gw
//** \date 2016年4月19日 
//** \note 
//********************************************************************************************************/
//Config::CNonlinearTransform *CFesModule::GetFesNonLinear(Config::CFesData *pFes, QString &strTag, int nType)
//{
//	Q_ASSERT(pFes);
//	if (!pFes)
//	{
//		return nullptr;
//	}
//	
//	if (nType == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
//	{
//		for (size_t i = 0; i < pFes->m_arrTransforms.size(); i++)
//		{
//			if (pFes->m_arrTransforms[i]->m_nType == ScaleType::NONLINEAR)
//			{
//				Q_ASSERT(pFes->m_arrTransforms[i]->m_pNonLinearScale);
//				if (!pFes->m_arrTransforms[i]->m_pNonLinearScale)
//				{
//					return nullptr;
//				}
//
//				if (strTag.compare(pFes->m_arrTransforms[i]->m_pNonLinearScale->m_szTagName) == 0)
//				{
//					return pFes->m_arrTransforms[i]->m_pNonLinearScale;
//				}
//			}
//		}
//	}
//
//	return nullptr;
//}

/*! \fn bool CFesModel::SaveProject(QDomDocument *pXml)
********************************************************************************************************* 
** \brief CFesModel::SaveProject 
** \details 保存工程文件
** \param pXml 
** \return bool 
** \author gw
** \date 2016年4月22日 
** \note 
********************************************************************************************************/
bool CFesModule::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	//if (m_pCore->GetModifyFlag(FES) == false)
	//{
	//	return false;
	//}
	
	//return false;

	QDomDocument *pDom = m_pCore->GetProjMgr()->GetDocument();

	if (!pDom)
	{
		m_pCore->LogMsg(FES_DESC, (char *)QStringLiteral("打开文件失败!!!").toStdString().c_str(), 1);

		return false;
	}

	//QDomNode n = pDom->documentElement().firstChild();
	//QDomNode n = pXml->documentElement();

	QDomNode n = pRoot->firstChild();

	{
		//if (m_arrFes[i]->GetModifyFlag())
		{
			while (!n.isNull())
			{
				QDomElement e = n.toElement(); // try to convert the node to an element.
				if (!e.isNull())
				{
					QString strTmp = e.tagName();// the node really is an element.


					if (strTmp.compare("fes") == 0)
					{
						if (e.hasChildNodes())
						{
							QDomNodeList list = e.childNodes();
							//auto nSize = list.size();

							while (!list.isEmpty())
							{
								QDomNode node = list.at(0);
								node.parentNode().removeChild(node);
							}
						}

						//保存前置工程文件
						m_pConfig->SaveProjectFesNode(pXml, e, szRoot, m_pHash, m_pStringPoolVec, m_pDescStringPoolOccNo);

						//m_pConfig->SaveFesFiles(pXml, e, szRoot, m_pCore);
					}
				}

				n = n.nextSibling();
			}
		}
	}

	return true;
}

/*! \fn bool CFesModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CFesModule::LoadProject 
** \details 前置模块加载工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author gw
** \date 2017年4月17日 
** \note 
********************************************************************************************************/
bool CFesModule::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_ASSERT(pXml);
	if (!pXml)
	{
		auto strError = QString(tr("load fes project failed!!!"));
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	Q_ASSERT(pRoot);
	if (!pRoot)
	{
		auto strError = QString(tr("load fes project failed!!!"));
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	//读取前置数据
	m_pCore->GetProjMgr()->GetDocument();

	QString strTmp = m_pCore->GetProjMgr()->GetDocument()->documentElement().tagName();

	QDomNode n = m_pCore->GetProjMgr()->GetDocument()->documentElement().firstChild();

	//QDomNode pTmp = pRoot->firstChild();

	QVector<QString> vec;
	vec.clear();
	Q_ASSERT(m_pConfig);
	if (!m_pConfig)
	{
		auto strError = QString("Config pointer is nullptr!!!");
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	/////////////////////////////delete tab widget/////////////////////////////////////////////////////////
	//auto &mapWidgetInfo = m_pCore->GetUIMgr()->GetTabWidget()->GetWidgetInfo();
	auto pTabWidget = m_pCore->GetUIMgr()->GetTabWidget();
	for (int i = pTabWidget->count(); i > 0; --i)
	{
		//pTabWidget->setCurrentIndex(i - 1);
		//if (mapWidgetInfo.contains(pTabWidget->widget(i - 1)))
		//{
		//	IBase *pBase = dynamic_cast<IBase *>(pTabWidget->widget(i - 1));
		//	Q_ASSERT(pBase);
		//	if (!pBase)
		//	{
		//		auto strError = tr("tab widget close wrong!!!");
		//		m_pCore->LogMsg("tab widget", strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}

		//	pTabWidget->removeTab(i - 1);

		//	pBase->Delete();

		emit pTabWidget->tabCloseRequested(i - 1);
		//}
	}
	pTabWidget->ClearMap();
	/////////////////////////////////////////////////////////////////////////////////////////

	//清理数据
	m_pConfig->Clear();
	m_pConfig->Create();

	while (!n.isNull())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if (!e.isNull())
		{
			QString strTmp = e.tagName();// the node really is an element.

			if (strTmp.compare("fes") == 0)
			{
				m_pConfig->LoadProjectFesNode(e, szRoot);
			}
		}

		n = n.nextSibling();
	}

	QString strPath = m_pCore->GetProjMgr()->GetFile()->fileName();

	auto nIndex = strPath.lastIndexOf("/");
	strPath = strPath.mid(0, nIndex);
	nIndex = strPath.lastIndexOf("/");
	strPath = strPath.mid(0, nIndex + 1);
	strPath += "scadastudio/scada/";

	//m_pConfig->LoadFesFiles(vec, m_pCore);

	CreateFesUi(m_pConfig);

	return true;
}

/*! \fn void CFesModel::CreateNewProject(QDomDocument *pXml, const QString & szRoot)
********************************************************************************************************* 
** \brief CFesModel::CreateNewProject 
** \details 新建工程
** \param pXml 
** \param szRoot 
** \return void 
** \author gw
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
void CFesModule::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{	
	QDir dir;
	bool ok = dir.mkdir(szRoot + "/" + "scada");
	if (!ok)
	{
		auto strError = QString(tr("%1/scada folder mkdir failed!!!")).arg(szRoot);

		m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
	}
	
	
	/////////////////////////////delete tab widget/////////////////////////////////////////////////////////
	auto &mapWidgetInfo = m_pCore->GetUIMgr()->GetTabWidget()->GetWidgetInfo();
	auto pTabWidget = m_pCore->GetUIMgr()->GetTabWidget();
	for (int i = pTabWidget->count(); i > 0; --i)
	{
		auto pWidget = pTabWidget->widget(i - 1);

		//pTabWidget->setCurrentIndex(i - 1);
		if (mapWidgetInfo.contains(pWidget))
		{
			IBase *pBase = dynamic_cast<IBase *>(pWidget);
			Q_ASSERT(pBase);
			if (!pBase)
			{
				auto strError = tr("tab widget close wrong!!!");
				m_pCore->LogMsg("tab widget", strError.toStdString().c_str(), LEVEL_1);

				return;
			}

		
			pBase->Delete();

			pTabWidget->removeTab(i - 1);

			delete pWidget;
			pWidget = nullptr;

		}
	}
	pTabWidget->ClearMap();
	/////////////////////////////////////////////////////////////////////////////////////////

	//清理数据
	m_pConfig->Clear();
	m_pConfig->Create();

	QDomElement root = pXml->createElement("fes");
	pRoot->appendChild(root);

	CQuoteItem *pFesItem = new CQuoteItem(tr("fes config"));
	pFesItem->setData(FES_CONFIG, Qt::UserRole);
	pFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(m_pConfig->m_pFesGroup);
	pFesItem->setData(reinterpret_cast<long long>(m_pConfig->m_pFesGroup), POINTERDATA);
	pFesItem->setIcon(QIcon(FES_PNG));
	pFesItem->setEditable(false);
	m_pTopFesItem = pFesItem;

	pTopItem->appendRow(pFesItem);

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(pTopItem));
}

/*! \fn void CFesModel::CloseProject()
********************************************************************************************************* 
** \brief CFesModel::CloseProject 
** \details 关闭工程
** \return void 
** \author gw
** \date 2016年8月16日 
** \note 
********************************************************************************************************/
void CFesModule::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(bSaveFlag);
	Q_UNUSED(pXml);
	//Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);

	QDomNode n = *pRoot;
	
	while (!n.isNull())
	{
		//auto nTe = n.toElement();

		//auto strTmp = nTe.tagName();

		n = n.nextSibling();
	}

	if (GetModifyFlag())
	{
		//CloseProject(pXml, pRoot, szRoot, bSaveFlag);
	}
}

void CFesModule::CreateFesUi(Config::CFesConfigData *pConfig)
{
	Q_UNUSED(pConfig);
	
	//auto &arrVec = pConfig->GetFecArr();
	
	//前置组态
	QStandardItem *pTop = m_pCore->GetUIMgr()->GetTopItem();

	////delete all children of parent;
	////QStandardItem * loopItem = parent; //main loop item
	//QList<QStandardItem *> carryItems; //Last In First Out stack of items
	//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
	//while (pTop->rowCount())
	//{
	//	itemsToBeDeleted << pTop->takeRow(0);
	//	//if the row removed has children:
	//	if (itemsToBeDeleted.at(0)->hasChildren())
	//	{
	//		carryItems << pTop;                 //put on the stack the current loopItem
	//		pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
	//	}
	//	//if current loopItem has no more rows but carryItems list is not empty:
	//	if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

	//}

	//qDeleteAll(itemsToBeDeleted);


	m_pTopFesItem = new CQuoteItem(tr("fes config"));
	m_pTopFesItem->setData(FES_CONFIG, Qt::UserRole);
	m_pTopFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	m_pTopFesItem->setData(reinterpret_cast<long long>(m_pConfig->m_pFesGroup), POINTERDATA);
	m_pTopFesItem->setIcon(QIcon(FES_PNG));
	m_pTopFesItem->setEditable(false);

	pTop->appendRow(m_pTopFesItem);

	CreateFesGroupUi(m_pConfig->m_pFesGroup, m_pTopFesItem);

	if (m_pChannel == nullptr)
	{
		m_pChannel = new CChannel(m_pCore);
	}
	//else
	//{
	//	m_pChannel->DisConnect();
	//	delete m_pChannel;
	//	m_pChannel = nullptr;

	//	m_pChannel = new CChannel(m_pCore, m_arrFes);
	//}


	if (m_pAlarm == nullptr)
	{
		m_pAlarm = new CAlarm(m_pCore);
	}
	//else
	//{
	//	m_pAlarm->Disconnect();
	//	delete m_pAlarm;
	//	m_pAlarm = nullptr;

	//	m_pAlarm = new CAlarm(m_pCore, m_arrFes);
	//}

	if (m_pScale == nullptr)
	{
		m_pScale = new CScale(m_pCore);
	}
	//else
	//{
	//	m_pScale->Disconnect();
	//	delete m_pScale;
	//	m_pScale = nullptr;

	//	m_pScale = new CScale(m_pCore, m_arrFes);
	//}

	if (m_pVariable == nullptr)
	{
		m_pVariable = new CVariable(m_pCore);
	}
	//else
	//{
	//	m_pVariable->Disconnect();
	//	delete m_pVariable;
	//	m_pVariable = nullptr;

	//	m_pVariable = new CVariable(m_pCore->GetUIMgr());
	//}

	if (m_pHisSave == nullptr)
	{
		m_pHisSave = new CHisSave(m_pCore->GetUIMgr());
	}

	if (m_pPdr == nullptr)
	{
		m_pPdr = new CPdr(m_pCore->GetUIMgr());
	}

	if (m_pForwarding == nullptr)
	{
		m_pForwarding = new CForwarding(m_pCore);
	}
	

	m_pUi = m_pCore->GetUIMgr();
}

/*! \fn void CFesModule::CreateFesGroupUi(Config::CFesGroup *pFesGroup)
********************************************************************************************************* 
** \brief CFesModule::CreateFesGroupUi 
** \details 
** \param pFesGroup 
** \return void 
** \author gw
** \date 2016年10月18日 
** \note 
********************************************************************************************************/
void CFesModule::CreateFesGroupUi(Config::CFesGroup *pFesGroup, CQuoteItem *pItem)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pFesGroup->m_arrFesGroup.begin();
	for (; it != pFesGroup->m_arrFesGroup.end(); it++)
	{
		auto strGroupName = it->second->m_strGroup;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(FES_ITEM_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(it->second), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		CreateFesGroupUi(it->second, newItem);
	}

	auto ite = pFesGroup->m_arrFesItem.begin();
	for (; ite != pFesGroup->m_arrFesItem.end(); ++ite)
	{
		//添加前置节点
		auto pFes = ite->second;
		CQuoteItem *pFesItem = new CQuoteItem(pFes->m_szTagName);
		pFesItem->setData(pFes->m_szTagName, LASTVALUE);
		pFesItem->setData(FES_ITEM, Qt::UserRole);
		//pFesItem->setData(tmp->m_szTagName, TAGNAME);
		pFesItem->setData((reinterpret_cast<std::size_t>(pFes)), POINTERDATA);
		pFesItem->setData(pFes->m_strDescription, TREE_TOOL_TIP_ROLE);
		pFesItem->setIcon(QIcon(FES_PNG));
		pFesItem->setEditable(true);

		pItem->appendRow(pFesItem);

		//初始化树分支
		InitChannel(pFesItem, pFes);

		InitTransform(pFesItem, pFes);

		InitAlarm(pFesItem, pFes);

		InitVariable(pFesItem, pFes);

		InitForwardingChannel(pFesItem, pFes);
	}
}


/*! \fn void CFesModule::DeleteFesModuleWidgetInTabWidget(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::DeleteFesModuleWidgetInTabWidget 
** \details 窗口包括前置窗口、通道窗口、装置窗口、装置AI窗口、装置DI窗口、装置AO窗口和装置DO窗口
** \param index 
** \return void 
** \author gw
** \date 2016年10月26日 
** \note 
********************************************************************************************************/
void CFesModule::DeleteFesModuleWidgetInTabWidget(QModelIndex &index)
{
	//前置窗口
	auto strFesTagName = index.data(Qt::EditRole).toString();
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strFesTagName, WIDGET_TYPE::FES_WIDGET_TYPE, strFesTagName))
	{
		//删除打开的前置窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIbase);
		pIbase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	//通道窗口
	QVector<QModelIndex> vec;
	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_CHANNEL_CHILD_ITEM, vec);
	for each (auto channel in vec)
	{
		//通道窗口
		auto strTag = channel.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::CHANNEL_WIDGET_TYPE, strFesTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIbase);
			pIbase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		QVector<QModelIndex> vecDevice;
		vecDevice.clear();
		CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES__CHANNEL_CHILD_DEVICE_ITEM, vecDevice);

		for each (auto device in vecDevice)
		{
			{
				//装置窗口
				auto strTag = device.data(Qt::EditRole).toString();
				if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_WIDGET_TYPE, strFesTagName))
				{
					//删除打开的前置窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIBase);
					if (!pIBase)
					{
						return;
					}

					pIBase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
				}
			}

			QVector<QModelIndex> vecDeviceChild;
			vecDeviceChild.clear();
			//装置AI窗口
			CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), device, FES_DEVICE_AI, vecDeviceChild);
			for each (auto var in vecDeviceChild)
			{
				//
				auto strTag = var.data(Qt::EditRole).toString();
				if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE, strFesTagName))
				{
					//删除打开的前置窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIBase);
					if (!pIBase)
					{
						return;
					}

					pIBase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
				}
			}

			vecDeviceChild.clear();
			//装置DI窗口
			CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), device, FES_DEVICE_DI, vecDeviceChild);
			for each (auto var in vecDeviceChild)
			{
				//
				auto strTag = var.data(Qt::EditRole).toString();
				if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE, strFesTagName))
				{
					//删除打开的前置窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIBase);
					if (!pIBase)
					{
						return;
					}

					pIBase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
				}
			}

			vecDeviceChild.clear();
			//装置AO窗口
			CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), device, FES_DEVICE_AO, vecDeviceChild);
			for each (auto var in vecDeviceChild)
			{
				//
				auto strTag = var.data(Qt::EditRole).toString();
				if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE, strFesTagName))
				{
					//删除打开的前置窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIBase);
					if (!pIBase)
					{
						return;
					}

					pIBase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
				}
			}

			vecDeviceChild.clear();
			//装置DO窗口
			CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), device, FES_DEVICE_DO, vecDeviceChild);
			for each (auto var in vecDeviceChild)
			{
				//
				auto strTag = var.data(Qt::EditRole).toString();
				if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE, strFesTagName))
				{
					//删除打开的前置窗口
					int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
					//model view detach
					auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
					Q_ASSERT(pIBase);
					if (!pIBase)
					{
						return;
					}

					pIBase->Delete();

					m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		 		}
			}
		}
	}

	//Delete alarm
	Delete_FesAlarm_Scale_Varaible_Widget_InTabWidget(index);

	//Delete forwarding channel
	Delete_FesForwardingChannelWidget_InTabWidget(index);
}

/*! \fn bool CFesModule::Delete_FesForwardingWidget_InTabWidget(QModelIndex & index)
********************************************************************************************************* 
** \brief CFesModule::Delete_FesForwardingWidget_InTabWidget 
** \details 
** \param index 
** \return bool 
** \author gw
** \date 2017年4月11日 
** \note 
********************************************************************************************************/
bool CFesModule::Delete_FesForwardingChannelWidget_InTabWidget(QModelIndex & index)
{
	auto pFes = reinterpret_cast<Config::CFesData *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	int nSize = strlen(pFes->m_szTagName);
	bool bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	//删除转发通道窗口
	QVector<QModelIndex> vec;
	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_ITEM, vec);
	for each (auto var in vec)
	{
		auto strChannelTagName = var.data(Qt::EditRole).toString();
		bool bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}


		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(var.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return false;
		}
		nSize = strlen(pChannel->m_szTagName);
		bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strChannelTagName, WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}

		QVector<QModelIndex> vec;
		vec.clear();
		//AI
		GetItemMap(m_pUi->GetLeftTreeModel(), var, FES_FORWARDING_CHANNEL_AI_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTagName = var.data(Qt::EditRole).toString();
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return false;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}

		//DI
		vec.clear();
		GetItemMap(m_pUi->GetLeftTreeModel(), var, FES_FORWARDING_CHANNEL_DI_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTagName = var.data(Qt::EditRole).toString();
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return false;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}

		//AO
		vec.clear();
		GetItemMap(m_pUi->GetLeftTreeModel(), var, FES_FORWARDING_CHANNEL_AO_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTagName = var.data(Qt::EditRole).toString();
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return false;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}

		//DO
		vec.clear();
		GetItemMap(m_pUi->GetLeftTreeModel(), var, FES_FORWARDING_CHANNEL_DO_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTagName = var.data(Qt::EditRole).toString();
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return false;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}

		//KWH
		vec.clear();
		GetItemMap(m_pUi->GetLeftTreeModel(), var, FES_FORWARDING_CHANNEL_KWH_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTagName = var.data(Qt::EditRole).toString();
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return false;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}

		//CG
		vec.clear();
		GetItemMap(m_pUi->GetLeftTreeModel(), var, FES_FORWARDING_CHANNEL_CG_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTagName = var.data(Qt::EditRole).toString();
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的前置窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return false;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}
		}


		////删除hash管理 ais
		//for each (auto var in pChannel->m_arrAIs)
		//{
		//	if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGAIHASHID, var->m_szTagName))
		//	{
		//		auto strError = tr("delete forwarding aie hash fail!!!");
		//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}
		//}

		//for each (auto var in pChannel->m_arrDIs)
		//{
		//	if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGDIHASHID, var->m_szTagName))
		//	{
		//		auto strError = tr("delete forwarding di hash fail!!!");
		//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}
		//}

		//for each (auto var in pChannel->m_arrAOs)
		//{
		//	if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGAOHASHID, var->m_szTagName))
		//	{
		//		auto strError = tr("delete forwarding ao hash fail!!!");
		//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}
		//}

		//for each (auto var in pChannel->m_arrDOs)
		//{
		//	if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGDOHASHID, var->m_szTagName))
		//	{
		//		auto strError = tr("delete forwarding do hash fail!!!");
		//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}
		//}

		//for each (auto var in pChannel->m_arrKWHs)
		//{
		//	if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGKWHHASHID, var->m_szTagName))
		//	{
		//		auto strError = tr("delete forwarding kwh hash fail!!!");
		//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}
		//}

		//for each (auto var in pChannel->m_arrCGs)
		//{
		//	if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGCGHASHID, var->m_szTagName))
		//	{
		//		auto strError = tr("delete forwarding cg hash fail!!!");
		//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		//		return false;
		//	}
		//}
	}

	return true;
}

/*! \fn bool CFesModule::Delete_FesAlarmWidget_InTabWidget(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::Delete_FesAlarmWidget_InTabWidget 
** \details 删除前置告警tabwidget
** \param index 
** \return bool 
** \author gw
** \date 2017年4月20日 
** \note 
********************************************************************************************************/
bool CFesModule::Delete_FesAlarm_Scale_Varaible_Widget_InTabWidget(QModelIndex &index)
{
	auto pFes = reinterpret_cast<Config::CFesData *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	int nSize = strlen(pFes->m_szTagName);
	bool bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	QVector<QModelIndex> vec;
	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_ALARM_ANALOG_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_ANALOG_ALARM_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}


	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_ALARM_DIGITAL_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_SCALE_LINEAR_CHILD_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCALE_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::SCALE_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//vec.clear();
	//CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, SCADAVARIABLE_ITEM_VARIABLE_SYSTEM, vec);
	//for each (auto var in vec)
	//{
	//	auto strTagName = var.data(Qt::EditRole).toString();

	//	//auto strLastGroupTagName = GetLastGroupName(varLastTmp);

	//	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::VARIABLE_SYSTEM_TYPE, pFes->m_szTagName))
	//	{
	//		//删除打开的前置窗口
	//		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
	//		//model view detach
	//		auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
	//		Q_ASSERT(pIBase);
	//		if (!pIBase)
	//		{
	//			return false;
	//		}

	//		pIBase->Delete();

	//		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	//	}
	//}

	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_SYSTEM_VARIABLE_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::VARIABLE_SYSTEM_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	vec.clear();
	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_USER_VARIBALE_GROUP_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();

		auto varTmp = var, varLastTmp = var;
		auto strGroupName = GetUserVaraibleGroupName(varTmp);

		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strGroupName, WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	return true;
}

///*! \fn Config::CTransform *CFesModel::GetFetTransform(Config::CFesData *pFes, QString &strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetFetTransform 
//** \details 转换
//** \param pFes 
//** \param strTag 
//** \return Config::CTransform * 
//** \author gw
//** \date 2016年4月20日 
//** \note 
//********************************************************************************************************/
//Config::CTransform *CFesModule::GetFetTransform(Config::CFesData *pFes, QString &strTag)
//{
//	Q_ASSERT(pFes);
//	if (pFes == nullptr)
//		return nullptr;
//
//	Q_ASSERT(strTag.isEmpty() == false);
//	if (strTag.isEmpty())
//		return nullptr;
//
//	for (size_t i = 0; i < pFes->m_arrTransforms.size(); i++)
//	{
//		if (strTag.compare(pFes->m_arrTransforms[i]->m_szTagName) == 0)
//		{
//			return pFes->m_arrTransforms[i];
//		}
//	}
//
//	return NULL;
//}

///*! \fn Config::CAnalogAlarm *CFesModel::GetAnalogAlarm(std::vector<Config::CFesData *> &arrFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetAnalogAlarm 
//** \details 查找模拟量告警
//** \param arrFes 
//** \param strTag 
//** \return Config::CAnalogAlarm * 
//** \author gw
//** \date 2016年5月6日 
//** \note 
//********************************************************************************************************/
//Config::CAnalogAlarm *CFesModel::GetAnalogAlarm(std::vector<Config::CFesData *> &arrFes, QString strTag)
//{
//	for (auto const &pFes : arrFes)
//	{
//		for (auto const &pAIAlarm : pFes->m_arrAIAlarms)
//		{
//			if (pAIAlarm->m_szTagName.compare(strTag) == 0)
//			{
//				return pAIAlarm;
//			}
//		}
//	}
//
//	return nullptr;
//}

//Config::CAnalogAlarm *CFesModule::GetAnalogAlarm(Config::CFesData *pFes, QString& strTag)
//{
//	Q_ASSERT(pFes);
//	if (pFes == nullptr)
//		return nullptr;
//
//	Q_ASSERT(strTag.isEmpty() == false);
//	if (strTag.isEmpty())
//		return nullptr;
//
//	//for (auto const &pFes : arrFes)
//	//{
//		for (auto const &pAIAlarm : pFes->m_arrAIAlarms)
//		{
//			if (strTag.compare(pAIAlarm->m_szTagName) == 0)
//			{
//				return pAIAlarm;
//			}
//		}
//	//}
//
//	return nullptr;
//}

///*! \fn Config::CAinAlarmLimit *CFesModel::GetAnalogAlarmLimit(std::vector<Config::CFesData *> &arrFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetAnalogAlarmLimit 
//** \details 模拟量限值
//** \param arrFes 
//** \param strTag 
//** \return Config::CAinAlarmLimit * 
//** \author gw
//** \date 2016年5月11日 
//** \note 
//********************************************************************************************************/
//Config::CAinAlarmLimit *CFesModel::GetAnalogAlarmLimit(std::vector<Config::CFesData *> &arrFes, QString strTag)
//{
//	for (auto const &pFes : arrFes)
//	{
//		for (auto const &pAIAlarm : pFes->m_arrAIAlarms)
//		{
//			for (auto const &tmp : pAIAlarm->m_arrAlarmOfflimit)
//			{
//				if (tmp->TagName.compare(strTag) == 0)
//				{
//					return tmp;
//				}
//			}
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::CAinAlarmLimit *CFesModel::GetAnalogAlarmLimit(Config::CFesData * pFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetAnalogAlarmLimit 
//** \details 在前置下面查找模拟量限值
//** \param pFes 
//** \param strTag 
//** \return Config::CAinAlarmLimit * 
//** \author gw
//** \date 2016年7月22日 
//** \note 
//********************************************************************************************************/
//Config::CAinAlarmLimit *CFesModule::GetAnalogAlarmLimit(Config::CAnalogAlarm *pAnalogAlarm, QString& strTag)
//{
//	Q_ASSERT(pAnalogAlarm);
//	if (pAnalogAlarm == nullptr)
//		return nullptr;
//
//	Q_ASSERT(strTag.isEmpty() == false);
//	if (strTag.isEmpty())
//		return nullptr;
//
//	for (auto const &tmp : pAnalogAlarm->m_arrAlarmOfflimit)
//	{
//		if (strTag.compare(tmp->TagName) == 0)
//		{
//			return tmp;
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::CDevice *CFesModel::GetDevice(std::vector<Config::CFesData *> &arrFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetDevice 
//** \details 查找装置
//** \param arrFes 
//** \param strTag 
//** \return Config::CDevice * 
//** \author gw
//** \date 2016年5月10日 
//** \note 
//********************************************************************************************************/
//Config::CDevice *CFesModule::GetDevice(Config::CChannelData *pChannel, QString &strTag)
//{
//	Q_ASSERT(pChannel);
//	if (pChannel == nullptr)
//		return nullptr;
//
//	Q_ASSERT(strTag.isEmpty() == false);
//	if (strTag.isEmpty())
//		return nullptr;
//
//	for (auto const &pDevice : pChannel->m_arrDevices)
//	{
//		if (pDevice->m_szTagName == strTag)
//		{
//			return pDevice;
//		}
//	}
//
//	return nullptr;
//}

/*! \fn Config::CDigtalAlarm *CFesModel::GetDigtitalAlarm(std::vector<Config::CFesData *> &arrFes, QString strTag)
********************************************************************************************************* 
** \brief CFesModel::GetDigtitalAlarm 
** \details 
** \param arrFes 
** \param strTag 
** \return Config::CDigtalAlarm * 
** \author gw
** \date 2016年6月16日 
** \note 
********************************************************************************************************/
//Config::CDigtalAlarm *CFesModel::GetDigtitalAlarm(std::vector<Config::CFesData *> &arrFes, QString strTag)
//{
//	for (auto const &pFes : arrFes)
//	{
//		for (auto const &pDIAlarm : pFes->m_arrDIAlarms)
//		{
//			if (pDIAlarm->m_szTagName.compare(strTag) == 0)
//			{
//				return pDIAlarm;
//			}
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::CDigtalAlarm *CFesModel::GetDigtitalAlarm(Config::CFesData *pFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetDigtitalAlarm 
//** \details 
//** \param pFes 
//** \param strTag 
//** \return Config::CDigtalAlarm * 
//** \author gw
//** \date 2016年6月16日 
//** \note 
//********************************************************************************************************/
//Config::CDigtalAlarm *CFesModule::GetDigtitalAlarm(Config::CFesData *pFes, QString &strTag)
//{
//	Q_ASSERT(pFes);
//	if (pFes == nullptr)
//		return nullptr;
//
//	Q_ASSERT(strTag.isEmpty() == false);
//	if (strTag.isEmpty())
//		return nullptr;
//
//	for (auto const &pDIAlarm : pFes->m_arrDIAlarms)
//	{
//		if (pDIAlarm->m_szTagName == strTag)
//		{
//			return pDIAlarm;
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::ALARM_OFFLIMIT_DI *CFesModel::GetDigtitalAlarmLimit(std::vector<Config::CFesData *> &arrFes, QString strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetDigtitalAlarmLimit 
//** \details 在前置下面查找所属开关量限值
//** \param arrFes 
//** \param strTag 
//** \return Config::ALARM_OFFLIMIT_DI * 
//** \author gw
//** \date 2016年6月22日 
//** \note 
//********************************************************************************************************/
//Config::ALARM_OFFLIMIT_DI *CFesModel::GetDigtitalAlarmLimit(std::vector<Config::CFesData *> &arrFes, QString strTag)
//{
//	for (auto const &pFes : arrFes)
//	{
//		for (auto const &pADAlarm : pFes->m_arrDIAlarms)
//		{
//			for (auto const &tmp : pADAlarm->m_arrDigtalOfflimit)
//			{
//				if (tmp->TagName.compare(strTag) == 0)
//				{
//					return tmp;
//				}
//			}
//
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::ALARM_OFFLIMIT_DI *CFesModel::GetDigtitalAlarmLimit(Config::CDigtalAlarm *pDigtalAlarm, QString strTag)
//********************************************************************************************************* 
//** \brief GetDigtitalAlarmLimit 
//** \details 
//** \param pFes 
//** \param strTag 
//** \return Config::ALARM_OFFLIMIT_DI * 
//** \author gw
//** \date 2016年7月22日 
//** \note 
//********************************************************************************************************/
//Config::ALARM_OFFLIMIT_DI *CFesModule::GetDigtitalAlarmLimit(Config::CDigtalAlarm *pDigtalAlarm, QString &strTag)
//{
//	Q_ASSERT(pDigtalAlarm);
//	if (pDigtalAlarm == nullptr)
//		return nullptr;
//
//	Q_ASSERT(strTag.isEmpty() == false);
//	if (strTag.isEmpty())
//		return nullptr;
//
//	for (auto const &tmp : pDigtalAlarm->m_arrDigtalOfflimit)
//	{
//		if (strTag.compare(tmp->TagName) == 0)
//		{
//			return tmp;
//		}
//	}
//
//	return nullptr;
//}

///*! \fn Config::CFesData *CFesModel::GetFesFromAlarmTagname(std::vector<Config::CFesData *> &arrFes, QString strTag, int nType)
//********************************************************************************************************* 
//** \brief CFesModel::GetFesFromAlarmTagname 
//** \details 
//** \param arrFes 
//** \param strTag 
//** \param nType 
//** \return Config::CFesData * 
//** \author gw
//** \date 2016年6月22日 
//** \note 
//********************************************************************************************************/
//Config::CFesData *CFesModel::GetFesFromAlarmTagname(std::vector<Config::CFesData *> &arrFes, QString strTag, int nType)
//{
//	for (auto const &pFes : arrFes)
//	{
//		if (pFes->m_szTagName.compare(strTag) == 0)
//		{
//			return pFes;
//		}
//	}
//
//	return nullptr;
//}

///*! \fn bool CFesModel::SaveFesFile(QString &strFileName, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveFesFile 
//** \details 保存前置文件
//** \param strFileName 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年4月27日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveFesFile(QString &strFileName, Config::CFesData *pFes, const QString & szRoot)
//{
//	Q_ASSERT(pFes);
//	if (!pFes)
//	{
//		return false;
//	}
//
//	QString strPath = szRoot + "/scada/";
//
//	strPath = strPath + strFileName;
//
//	QFile file(strPath);
//
//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//	{
//		QString strTmp = QString("%1%2%3").arg(LOG_STRING("打开文件")).arg(strFileName).arg(LOG_STRING("失败!!!"));
//
//		m_pCore->LogMsg(FES_DESC, (char *)file.errorString().toStdString().c_str(), 1);
//		
//		
//
//		return false;
//	}
//
//	QXmlStreamWriter writer(&file);
//	writer.setAutoFormatting(true);
//	writer.writeStartDocument();
//
//
//	writer.writeStartElement("fes");
//	writer.writeAttribute("Description", pFes->m_strDescription);
//	writer.writeAttribute("TagName", pFes->m_szTagName);
//	writer.writeAttribute("version", VERSION);
//	writer.writeAttribute("ID", QString("%1").arg(pFes->m_nID));
//	writer.writeAttribute("Extention", pFes->m_strExtention);
//	writer.writeAttribute("GroupName", pFes->m_strGroupName);
//
//
//	SaveChannel(writer, pFes);
//
//	Savetransform(writer, pFes);
//
//	SaveAlarm(writer, pFes);
//
//	writer.writeEndElement();
//
//	writer.writeEndDocument();
//
//	//file.close();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveChannel(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveChannel 
//** \details 保存通道数据
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年4月27日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveChannel(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("channel");
//
//	int nCounts = pFes->m_arrChannels.size();
//
//	for (int32u i = 0; i < nCounts; i++)
//	{
//		//pFes->m_arrChannels[i]->Save();
//		
//		writer.writeStartElement("c");
//
//		writer.writeAttribute("OccNo", QString("%1").arg(pFes->m_arrChannels[i]->GetOccNo()));
//		writer.writeAttribute("TagName", QString("%1").arg(pFes->m_arrChannels[i]->m_szTagName));
//		writer.writeAttribute("ID", QString("%1").arg(pFes->m_arrChannels[i]->m_nID));
//		writer.writeAttribute("Description", QString("%1").arg(pFes->m_arrChannels[i]->m_strDescription));
//		writer.writeAttribute("DriverName", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.DriverName));
//		writer.writeAttribute("DriverPara", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.DriverName));
//		writer.writeAttribute("Medium", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Medium));
//		/*writer.writeAttribute("IP", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.IP));
//		writer.writeAttribute("Port", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Port));
//		writer.writeAttribute("COM", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Com));
//		writer.writeAttribute("Baud", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Baud));*/
//		writer.writeAttribute("ChannelPara", QString("%1").arg(pFes->m_arrChannels[i]->m_strChannelPara));
//		writer.writeAttribute("Extention", QString("%1").arg(pFes->m_arrChannels[i]->m_strExtention));
//		writer.writeAttribute("GroupName", QString("%1").arg(pFes->m_arrChannels[i]->m_strGroupName));
//
//		SaveDevice(writer, pFes);
//
//		writer.writeEndElement();
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveDevice(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveDevice 
//** \details 保存装置
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月3日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveDevice(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	int nChannels = pFes->m_arrChannels.size();
//
//	writer.writeStartElement("device");
//
//	for (size_t i = 0; i < nChannels; i++)
//	{
//		int nDevices = pFes->m_arrChannels[i]->m_arrDevices.size();
//		for (size_t j = 0; j < nDevices; j++)
//		{
//			Config::CDevice *pTmp = pFes->m_arrChannels[i]->m_arrDevices[j];
//
//			writer.writeStartElement("d");
//
//			writer.writeAttribute("OccNo", QString("%1").arg(pTmp->GetOccNo()));
//			writer.writeAttribute("TagName", QString("%1").arg(pTmp->m_szTagName));
//			writer.writeAttribute("ID", QString("%1").arg(pTmp->m_nID));
//			writer.writeAttribute("Description", QString("%1").arg(pTmp->m_strDescription));
//			writer.writeAttribute("DeviceModelID", QString("%1").arg(pTmp->m_nDeviceModelID));
//			writer.writeAttribute("Type", QString("%1").arg(pTmp->m_nType));
//			writer.writeAttribute("DeviceAddr", QString("%1").arg(pTmp->m_szTagName));
//			writer.writeAttribute("Priority", QString("%1").arg(pTmp->m_nID));
//			writer.writeAttribute("ExtentionParam", QString("%1").arg(pTmp->m_strExtentionParam));
//
//			//////////////////////////data////////////////////////////////////////
//			SaveData(writer, pFes);
//			//////////////////////////////////////////////////////////////////
//
//			writer.writeEndElement();
//		}
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveData(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveData 
//** \details 保存点
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月3日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveData(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("data");
//	
//	SaveAI(writer, pFes);
//
//	SaveDI(writer, pFes);
//
//	SaveAO(writer, pFes);
//
//	SaveDO(writer, pFes);
//
//
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveAI(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveAI 
//** \details 保存AI
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月3日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveAI(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("AIN");
//
//	auto &arrChannel = pFes->m_arrChannels;
//
//	for (auto const &channel : arrChannel)
//	{
//		for (auto const &device : channel->m_arrDevices)
//		{
//			for (auto const &ai : device->m_arrAIs)
//			{
//				writer.writeStartElement("ai");
//
//				writer.writeAttribute("OccNo", QString("%1").arg(ai->GetOccNo()));
//				writer.writeAttribute("TagName", QString("%1").arg(ai->m_szTagName));
//				writer.writeAttribute("ID", QString("%1").arg(ai->m_nID));
//				writer.writeAttribute("BlockOccNo", QString("%1").arg(ai->m_nBlockOccNo));
//				writer.writeAttribute("DataType", QString("%1").arg(ai->m_nDataType));
//				writer.writeAttribute("Description", QString("%1").arg(ai->m_strDescription));
//				writer.writeAttribute("InitialQua", QString("%1").arg(ai->m_nInitialQua));
//				writer.writeAttribute("Address", QString("%1").arg(ai->m_strAddress));
//				writer.writeAttribute("PinLabel", QString("%1").arg(ai->m_strPinLabel));
//				//
//				writer.writeAttribute("InitValue", QString("%1").arg(ai->m_dbInitValue));
//				writer.writeAttribute("Format", QString("%1").arg(ai->m_strFormat));
//				writer.writeAttribute("Unit", QString("%1").arg(ai->m_strUnit));
//				writer.writeAttribute("Enable", QString("%1").arg(ai->m_bEnable));
//				writer.writeAttribute("MaxRaw", QString("%1").arg(ai->m_dbMaxRaw));
//				writer.writeAttribute("MinRaw", QString("%1").arg(ai->m_dbMinRaw));
//				writer.writeAttribute("MaxConvert", QString("%1").arg(ai->m_dbMaxConvert));
//				writer.writeAttribute("MinConvert", QString("%1").arg(ai->m_dbMinConvert));
//				writer.writeAttribute("ScaleTagName", QString("%1").arg(ai->m_strScaleTagName));
//				writer.writeAttribute("ScaleType", QString("%1").arg(ai->m_strScaleDesc));
//				writer.writeAttribute("ScaleDesc", QString("%1").arg(ai->m_nScaleType));
//
//				writer.writeAttribute("RangeL", QString("%1").arg(ai->m_dblRangeL));
//				writer.writeAttribute("RangeH", QString("%1").arg(ai->m_dblRangeH));
//				writer.writeAttribute("HighQua", QString("%1").arg(ai->m_dblHighQua));
//				writer.writeAttribute("LowQua", QString("%1").arg(ai->m_dblLowQua));
//
//				writer.writeAttribute("SaveDisk", QString("%1").arg(ai->m_bSaveDisk));
//				writer.writeAttribute("SaveDiskPeriod", QString("%1").arg(ai->m_nSaveDiskPeriod));
//				writer.writeAttribute("SaveDB", QString("%1").arg(ai->m_bSaveDB));
//				writer.writeAttribute("SaveDBPeriod", QString("%1").arg(ai->m_nSaveDBPeriod));
//
//				writer.writeAttribute("Sensitivity", QString("%1").arg(ai->m_dblSensitivity));
//				writer.writeAttribute("SensitivityType", QString("%1").arg(ai->m_nSensitivityType));
//				
//				writer.writeAttribute("AlarmTagName", QString("%1").arg(ai->m_strAlarmTagName));
//				writer.writeAttribute("AlarmDesc", QString("%1").arg(ai->m_strAlarmDesc));
//
//
//				writer.writeAttribute("ChannelOccNo", QString("%1").arg(ai->m_nChannelOccNo));
//				writer.writeAttribute("DeviceOccNo", QString("%1").arg(ai->m_nDeviceOccNo));
//				writer.writeAttribute("ReferenceCount", QString("%1").arg(ai->m_nReferenceCount));
//				writer.writeAttribute("DataSource", QString("%1").arg(ai->m_nDataSource));
//				writer.writeAttribute("Express", QString("%1").arg(ai->m_strExpress));
//
//				writer.writeEndElement();
//			}
//		}
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveDI(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveDI 
//** \details 保存DI
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月4日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveDI(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("DIN");
//
//	auto &arrChannel = pFes->m_arrChannels;
//
//	//std::for_each(arrChannel.begin(), arrChannel.end(), [](Config::CChannelData *& e)
//	//{
//	//	auto &arrDevice = e->m_arrDevices;
//
//	//	std::for_each(arrDevice.begin(), arrDevice.end(), [](Config::CDevice *& d)
//	//	{
//	//		auto  &arrAI = d->m_arrAIs;
//
//	//		std::for_each(arrAI.begin(), arrAI.end(), [](Config::CAnalogInput *& ai)
//	//		{
//
//	//		});
//	//	});
//	//});
//
//	for (auto const &channel : arrChannel)
//	{
//		for (auto const &device : channel->m_arrDevices)
//		{
//			for (auto const &di : device->m_arrDIs)
//			{
//				writer.writeStartElement("di");
//
//				writer.writeAttribute("OccNo", QString("%1").arg(di->GetOccNo()));
//				writer.writeAttribute("TagName", QString("%1").arg(di->m_szTagName));
//				writer.writeAttribute("ID", QString("%1").arg(di->m_nID));
//				writer.writeAttribute("Description", QString("%1").arg(di->m_strDescription));
//				writer.writeAttribute("ChannelOccNo", QString("%1").arg(di->m_nChannelOccNo));
//				writer.writeAttribute("DeviceOccNo", QString("%1").arg(di->m_nDeviceOccNo));
//				writer.writeAttribute("BlockOccNo", QString("%1").arg(di->m_nBlockOccNo));
//				writer.writeAttribute("Address", QString("%1").arg(di->m_strAddress));
//				writer.writeAttribute("InitValue", QString("%1").arg(di->m_bInitValue));
//				writer.writeAttribute("InitialQua", QString("%1").arg(di->m_nInitialQua));
//				writer.writeAttribute("DataSource", QString("%1").arg(di->m_nDataSource));
//				writer.writeAttribute("AlarmType", QString("%1").arg(di->m_nAlarmType));
//				writer.writeAttribute("AlarmOccNo", QString("%1").arg(di->m_nAlarmOccNo));
//				writer.writeAttribute("Express", QString("%1").arg(di->m_strExpress));
//				writer.writeAttribute("PinLabel", QString("%1").arg(di->m_strPinLabel));
//				writer.writeAttribute("DataType", QString("%1").arg(di->m_nDataType));
//				writer.writeAttribute("SaveDisk", QString("%1").arg(di->m_bSaveDisk));
//				writer.writeAttribute("SaveDiskPeriod", QString("%1").arg(di->m_nSaveDiskPeriod));
//				writer.writeAttribute("SaveDB", QString("%1").arg(di->m_bSaveDB));
//				writer.writeAttribute("SaveDBPeriod", QString("%1").arg(di->m_nSaveDBPeriod));
//				//writer.writeAttribute("Sensitivity", QString("%1").arg(di->m_dbSensitivity));
//				writer.writeAttribute("ReferenceCount", QString("%1").arg(di->m_nReferenceCount));
//				writer.writeAttribute("BitString", QString("%1").arg(di->m_bBitFlag));
//				writer.writeAttribute("BitOpenString", QString("%1").arg(di->m_strBitOpenString));
//				writer.writeAttribute("BitCloseString", QString("%1").arg(di->m_strBitCloseString));
//				writer.writeAttribute("SOE", QString("%1").arg(di->m_bSOEFlag));
//
//				writer.writeEndElement();
//			}
//		}
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveAO(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveAO 
//** \details 保存AO
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月4日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveAO(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("AOUT");
//
//	auto &arrChannel = pFes->m_arrChannels;
//
//	//std::for_each(arrChannel.begin(), arrChannel.end(), [](Config::CChannelData *& e)
//	//{
//	//	auto &arrDevice = e->m_arrDevices;
//
//	//	std::for_each(arrDevice.begin(), arrDevice.end(), [](Config::CDevice *& d)
//	//	{
//	//		auto  &arrAI = d->m_arrAIs;
//
//	//		std::for_each(arrAI.begin(), arrAI.end(), [](Config::CAnalogInput *& ai)
//	//		{
//
//	//		});
//	//	});
//	//});
//
//	for (auto const &channel : arrChannel)
//	{
//		for (auto const &device : channel->m_arrDevices)
//		{
//			for (auto const &ao : device->m_arrAOs)
//			{
//				writer.writeStartElement("ao");
//
//				writer.writeAttribute("OccNo", QString("%1").arg(ao->GetOccNo()));
//				writer.writeAttribute("TagName", QString("%1").arg(ao->m_szTagName));
//				writer.writeAttribute("ID", QString("%1").arg(ao->m_nID));
//				writer.writeAttribute("Description", QString("%1").arg(ao->m_strDescription));
//				writer.writeAttribute("ChannelOccNo", QString("%1").arg(ao->m_nChannelOccNo));
//				writer.writeAttribute("DeviceOccNo", QString("%1").arg(ao->m_nDeviceOccNo));
//				writer.writeAttribute("BlockOccNo", QString("%1").arg(ao->m_nBlockOccNo));
//				writer.writeAttribute("Address", QString("%1").arg(ao->m_strAddress));
//				writer.writeAttribute("InitValue", QString("%1").arg(ao->m_dbInitValue));
//				writer.writeAttribute("InitialQua", QString("%1").arg(ao->m_nInitialQua));
//				//writer.writeAttribute("DataSource", QString("%1").arg(ao->m_nDataSource));
//				writer.writeAttribute("TransformType", QString("%1").arg(ao->m_nScaleType));
//				writer.writeAttribute("TransformOccNo", QString("%1").arg(ao->m_nScaleOccNo));
//				writer.writeAttribute("AlarmOccNo", QString("%1").arg(ao->m_nAlarmOccNo));
//				writer.writeAttribute("RangeL", QString("%1").arg(ao->m_dbRangeL));
//				writer.writeAttribute("RangeH", QString("%1").arg(ao->m_dbRangeH));
//				writer.writeAttribute("HighQua", QString("%1").arg(ao->m_dbHighQua));
//				writer.writeAttribute("LowQua", QString("%1").arg(ao->m_dbLowQua));
//				writer.writeAttribute("UnitName", QString("%1").arg(ao->m_strUnitName));
//				writer.writeAttribute("Express", QString("%1").arg(ao->m_strExpress));
//				writer.writeAttribute("PinLabel", QString("%1").arg(ao->m_strPinLabel));
//				writer.writeAttribute("DataType", QString("%1").arg(ao->m_nDataType));
//				writer.writeAttribute("SaveDisk", QString("%1").arg(ao->m_bSaveDisk));
//				writer.writeAttribute("SaveDiskPeriod", QString("%1").arg(ao->m_bSaveDiskPeriod));
//				writer.writeAttribute("SaveDB", QString("%1").arg(ao->m_bSaveDB));
//				writer.writeAttribute("SaveDBPeriod", QString("%1").arg(ao->m_nSaveDBPeriod));
//				writer.writeAttribute("Sensitivity", QString("%1").arg(ao->m_dbSensitivity));
//				writer.writeAttribute("ReferenceCount", QString("%1").arg(ao->m_nReferenceCount));
//
//				writer.writeEndElement();
//			}
//		}
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveDO(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveDO 
//** \details 保存DO
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月4日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveDO(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("DOUT");
//
//	auto &arrChannel = pFes->m_arrChannels;
//
//	//std::for_each(arrChannel.begin(), arrChannel.end(), [](Config::CChannelData *& e)
//	//{
//	//	auto &arrDevice = e->m_arrDevices;
//
//	//	std::for_each(arrDevice.begin(), arrDevice.end(), [](Config::CDevice *& d)
//	//	{
//	//		auto  &arrAI = d->m_arrAIs;
//
//	//		std::for_each(arrAI.begin(), arrAI.end(), [](Config::CAnalogInput *& ai)
//	//		{
//
//	//		});
//	//	});
//	//});
//
//	for (auto const &channel : arrChannel)
//	{
//		for (auto const &device : channel->m_arrDevices)
//		{
//			for (auto const &dout : device->m_arrDOs)
//			{
//				writer.writeStartElement("do");
//
//				writer.writeAttribute("OccNo", QString("%1").arg(dout->GetOccNo()));
//				writer.writeAttribute("TagName", QString("%1").arg(dout->m_szTagName));
//				writer.writeAttribute("ID", QString("%1").arg(dout->m_nID));
//				writer.writeAttribute("Description", QString("%1").arg(dout->m_strDescription));
//				writer.writeAttribute("ChannelOccNo", QString("%1").arg(dout->m_nChannelOccNo));
//				writer.writeAttribute("DeviceOccNo", QString("%1").arg(dout->m_nDeviceOccNo));
//				writer.writeAttribute("Address", QString("%1").arg(dout->m_strAddress));
//				writer.writeAttribute("InitValue", QString("%1").arg(dout->m_dbInitValue));
//				writer.writeAttribute("InitialQua", QString("%1").arg(dout->m_nInitialQua));
//				writer.writeAttribute("DataSource", QString("%1").arg(dout->m_nDataSource));
//				writer.writeAttribute("AlarmOccNo", QString("%1").arg(dout->m_nAlarmOccNo));;
//				writer.writeAttribute("Express", QString("%1").arg(dout->m_strExpress));
//				writer.writeAttribute("PinLabel", QString("%1").arg(dout->m_strPinLabel));
//				writer.writeAttribute("IsSBO", QString("%1").arg(dout->m_bIsSBO));
//				writer.writeAttribute("TelCtlWaitTime", QString("%1").arg(dout->m_dbTelCtlWaitTime));
//				writer.writeAttribute("TelCtlSendTime", QString("%1").arg(dout->m_dbTelCtlSendTime));
//				writer.writeAttribute("TelCtlExecTime", QString("%1").arg(dout->m_dbTelCtlExecTime));
//				writer.writeAttribute("ReferenceCount", QString("%1").arg(dout->m_nReferenceCount));
//
//				writer.writeEndElement();
//			}
//		}
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::Savetransform(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::Savetransform 
//** \details 保存transform
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月4日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::Savetransform(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("transform");
//	
//	SaveLinear(writer, pFes);
//
//	SaveNonlinear(writer, pFes);
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveLinear(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveLinear 
//** \details 保存linear
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月4日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveLinear(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	auto &arrTransform = pFes->m_arrTransforms;
//
//	std::vector<Config::CLinearTransform *> arrLinear;
//
//	for (auto const &transform : arrTransform)
//	{
//		auto type = -1;
//		transform->GetType(type);
//		if (type == ScaleType::LINEAR)
//		{
//			auto pTmp = transform->GetLinear();
//			Q_ASSERT(pTmp);
//			if (pTmp)
//			{
//				arrLinear.push_back(pTmp);
//			}
//		}
//	}
//
//	writer.writeStartElement("linear");
//
//	for (auto const &linear : arrLinear)
//	{
//		writer.writeStartElement("l");
//
//		writer.writeAttribute("OccNo", QString("%1").arg(linear->GetOccNo()));
//		writer.writeAttribute("TagName", QString("%1").arg(linear->m_szTagName));
//		writer.writeAttribute("ID", QString("%1").arg(linear->m_nID));
//		writer.writeAttribute("Description", QString("%1").arg(linear->m_strDescription));
//		writer.writeAttribute("ScaleType", QString("%1").arg(linear->m_nType));
//		writer.writeAttribute("MaxRaw", QString("%1").arg(linear->m_dbMaxRaw));
//		writer.writeAttribute("MinRaw", QString("%1").arg(linear->m_dbMinRaw));
//		writer.writeAttribute("MaxScale", QString("%1").arg(linear->m_dbMaxScale));
//		writer.writeAttribute("MinScale", QString("%1").arg(linear->m_dbMinScale));
//		writer.writeAttribute("Inverse", QString("%1").arg(linear->m_bInverseFactor));;
//		writer.writeAttribute("EnaleFactor", QString("%1").arg(linear->m_bEnableFactor));
//		writer.writeAttribute("Gain", QString("%1").arg(linear->m_dbGain));
//		writer.writeAttribute("Offset", QString("%1").arg(linear->m_dbOffset));
//		writer.writeAttribute("GroupName", QString("%1").arg(linear->m_strGroupName));
//
//		writer.writeEndElement();
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveNonlinear(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveNonlinear 
//** \details 保存nonlinear
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月5日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveNonlinear(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("nonlinear");
//
//	auto &arrTransform = pFes->m_arrTransforms;
//
//	std::vector<Config::CNonlinearTransform *> arrNonlinear;
//
//	for (auto const &transform : arrTransform)
//	{
//		auto type = -1;
//		transform->GetType(type);
//		if (type == ScaleType::NONLINEAR)
//		{
//			auto pTmp = transform->GetNonLinear();
//			Q_ASSERT(pTmp);
//			if (pTmp)
//			{
//				arrNonlinear.push_back(pTmp);
//			}
//		}
//	}
//
//
//	for (auto const &nonlinear : arrNonlinear)
//	{
//		writer.writeStartElement("nl");
//
//		writer.writeAttribute("OccNo", QString("%1").arg(nonlinear->GetOccNo()));
//		writer.writeAttribute("TagName", QString("%1").arg(nonlinear->m_szTagName));
//		writer.writeAttribute("ID", QString("%1").arg(nonlinear->m_nID));
//		writer.writeAttribute("Description", QString("%1").arg(nonlinear->m_strDescription));
//		writer.writeAttribute("ScaleType", QString("%1").arg(nonlinear->m_nType));
//		writer.writeAttribute("Count", QString("%1").arg(nonlinear->m_arrPNonliear.size()));
//		writer.writeAttribute("GroupName", QString("%1").arg(nonlinear->m_strGroupName));
//
//
//		for (auto const &p : nonlinear->m_arrPNonliear)
//		{
//			writer.writeStartElement("p");
//
//			writer.writeAttribute("RawValue", QString("%1").arg(p->RawValue));
//			writer.writeAttribute("ScaleValue", QString("%1").arg(p->ScaleValue));
//
//			writer.writeEndElement();
//		}
//
//
//		writer.writeEndElement();
//	}
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveAlarm 
//** \details 保存alarm
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月5日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	writer.writeStartElement("alarm");
//
//	SaveAnalogAlarm(writer, pFes);
//
//	SaveDigtalAlarm(writer, pFes);
//
//	writer.writeEndElement();
//
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveAnalogAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveAnalogAlarm 
//** \details 保存analogalarm
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月5日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveAnalogAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	auto const &analogAlarm = pFes->m_arrAIAlarms;
//	
//	writer.writeStartElement("ain_alarm");
//
//	writer.writeStartElement("ain_alarm_offlimit");
//
//	for (auto const &alarm : analogAlarm)
//	{
//		writer.writeAttribute("OccNo", QString("%1").arg(alarm->GetOccNo()));
//		writer.writeAttribute("TagName", QString("%1").arg(alarm->m_szTagName));
//		writer.writeAttribute("ID", QString("%1").arg(alarm->m_nID));
//		writer.writeAttribute("Description", QString("%1").arg(alarm->m_strDescription));
//		writer.writeAttribute("AlarmType", QString("%1").arg(alarm->m_nAlarmType));
//		writer.writeAttribute("Enable", QString("%1").arg(alarm->m_bEnable));
//		writer.writeAttribute("OnQualityGood", QString("%1").arg(alarm->m_bOnQualityGood));
//		writer.writeAttribute("DeadType", QString("%1").arg(alarm->m_nDeadType));
//		writer.writeAttribute("DeadArea", QString("%1").arg(alarm->m_dbDeadArea));
//		writer.writeAttribute("GroupName", QString("%1").arg(alarm->m_strGroupName));
//
//		auto const &analogOfflimit = alarm->m_arrAlarmOfflimit;
//		for (auto const &tmp : analogOfflimit)
//		{
//			writer.writeStartElement("aa");
//
//			writer.writeAttribute("OccNo", QString("%1").arg(tmp->OccNo));
//			writer.writeAttribute("TagName", QString("%1").arg(tmp->TagName));
//			writer.writeAttribute("ID", QString("%1").arg(tmp->ID));
//			writer.writeAttribute("Description", QString("%1").arg(tmp->Description));
//			//writer.writeAttribute("TimeFormat", QString("%1").arg(tmp->TimeFormat));
//			writer.writeAttribute("Condition", QString("%1").arg(tmp->Condition));
//			writer.writeAttribute("Value", QString("%1").arg(tmp->Value));
//			writer.writeAttribute("LowValue", QString("%1").arg(tmp->LowValue));
//			writer.writeAttribute("WriteLevel", QString("%1").arg(tmp->LowValue));
//			writer.writeAttribute("ReadLevel", QString("%1").arg(tmp->LowValue));
//			writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->DelayAlarm));
//			writer.writeAttribute("Level", QString("%1").arg(tmp->Level));
//			writer.writeAttribute("AlarmStyle", QString("%1").arg(tmp->AlarmStyle));
//			writer.writeAttribute("SupportAck", QString("%1").arg(tmp->SupportAck));
//			writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->SupportDelete));
//			writer.writeAttribute("SoundFile", QString("%1").arg(tmp->SoundFile));
//			writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->PlaySoundTimes));
//			writer.writeAttribute("Commands", QString("%1").arg(tmp->Commands));
//
//			writer.writeEndElement();
//		}
//	}
//
//	writer.writeEndElement();
//
//	writer.writeEndElement();
//
//	return true;
//}
//
///*! \fn bool CFesModel::SaveDigtalAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes)
//********************************************************************************************************* 
//** \brief CFesModel::SaveDigtalAlarm 
//** \details 保存digtalalarm
//** \param writer 
//** \param pFes 
//** \return bool 
//** \author gw
//** \date 2016年5月5日 
//** \note 
//********************************************************************************************************/
//bool CFesModule::SaveDigtalAlarm(QXmlStreamWriter &writer, Config::CFesData *pFes)
//{
//	auto const &digtalAlarm = pFes->m_arrDIAlarms;
//
//	writer.writeStartElement("din_alarm");
//
//	writer.writeStartElement("din_alarm_offlimit");
//
//	for (auto const &alarm : digtalAlarm)
//	{
//		writer.writeAttribute("OccNo", QString("%1").arg(alarm->GetOccNo()));
//		writer.writeAttribute("TagName", QString("%1").arg(alarm->m_szTagName));
//		writer.writeAttribute("ID", QString("%1").arg(alarm->m_nID));
//		writer.writeAttribute("Description", QString("%1").arg(alarm->m_strDescription));
//		writer.writeAttribute("AlarmType", QString("%1").arg(alarm->m_nAlarmType));
//		writer.writeAttribute("Enable", QString("%1").arg(alarm->m_bEnable));
//		writer.writeAttribute("OnQualityGood", QString("%1").arg(alarm->m_bOnQualityGood));
//		writer.writeAttribute("GroupName", QString("%1").arg(alarm->m_strGroupName));
//
//		auto const &digtalOfflimit = alarm->m_arrDigtalOfflimit;
//		for (auto const &tmp : digtalOfflimit)
//		{
//			writer.writeStartElement("da");
//
//			writer.writeAttribute("OccNo", QString("%1").arg(tmp->OccNo));
//			writer.writeAttribute("TagName", QString("%1").arg(tmp->TagName));
//			writer.writeAttribute("ID", QString("%1").arg(tmp->ID));
//			writer.writeAttribute("Description", QString("%1").arg(tmp->Description));
//			writer.writeAttribute("TimeFormat", QString("%1").arg(tmp->TimeFormat));
//			writer.writeAttribute("Condition", QString("%1").arg(tmp->Condition));
//			writer.writeAttribute("Value", QString("%1").arg(tmp->Value));
//			writer.writeAttribute("DelayAlarm", QString("%1").arg(tmp->DelayAlarm));
//			writer.writeAttribute("Level", QString("%1").arg(tmp->Level));
//			writer.writeAttribute("AlarmStyle0_1", QString("%1").arg(tmp->AlarmStyle0_1));
//			writer.writeAttribute("AlarmStyle1_0", QString("%1").arg(tmp->AlarmStyle1_0));
//			writer.writeAttribute("SupportAck", QString("%1").arg(tmp->SupportAck));
//			writer.writeAttribute("SupportDelete", QString("%1").arg(tmp->SupportDelete));
//			writer.writeAttribute("SoundFile", QString("%1").arg(tmp->SoundFile));
//			writer.writeAttribute("PlaySoundTimes", QString("%1").arg(tmp->PlaySoundTimes));
//			writer.writeAttribute("Commands", QString("%1").arg(tmp->Commands));
//
//			writer.writeEndElement();
//		}
//	}
//
//	writer.writeEndElement();
//
//	writer.writeEndElement();
//
//	return true;
//}

/*! \fn void CFesModel::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int &nType, QVector<QModelIndex *> &indexVec)
********************************************************************************************************* 
** \brief CFesModel::GetItemMap 
** \details 根据当前项,向下遍历nType对应的子项
** \param pModel 
** \param top 
** \param nType 
** \param indexVec 
** \return void 
** \author gw
** \date 2016年7月26日 
** \note 
********************************************************************************************************/
void CFesModule::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
{
	QModelIndex Item;

	if (!top.isValid()) 
	{

	}

	for (int r = 0; r < pModel->rowCount(top); r++)
	{
		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);

			const QString strTmp = Item.data(Qt::EditRole).toString();

			if (nType == Item.data(Qt::UserRole).toInt())
			{
				indexVec.append(Item);

				//qDebug() << s;
			}

			//qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			GetItemMap(pModel, Item, nType, indexVec);
		}
	}
}

//字符串描述内存池
/*! \fn bool CFesModule::GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
********************************************************************************************************* 
** \brief CFesModule::GetDesPoolArr 
** \details 字符串内存池arr
** \param vec 
** \param nOccNo 
** \return bool 
** \author gw
** \date 2017年3月2日 
** \note 
********************************************************************************************************/
bool CFesModule::GetDesPoolArr(std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
{
	m_pHash = pHash;
	m_pStringPoolVec = pStringPoolVec;
	m_pDescStringPoolOccNo = pDescStringPoolOccNo;

	return true;
}

/*! \fn QString CFesModel::GetGroupName(QModelIndex *pIndex, int nType)
********************************************************************************************************* 
** \brief CFesModel::GetGroupName 
** \details 遍历分组
** \param pIndex 
** \param nType 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2016年7月26日 
** \note 
********************************************************************************************************/
QString CFesModule::GetGroupName(QModelIndex &pIndex, int nType)
{
	QModelIndex indexTmp = pIndex;
	
	QVector<QString> vec;
	vec.clear();

	while (indexTmp.parent().data(Qt::UserRole).toInt() != nType)
	{
		vec.append(indexTmp.parent().data(Qt::EditRole).toString());

		indexTmp = indexTmp.parent();
	}

	

	QString strGroup = "";

	for (auto p = vec.rbegin(); p != vec.rend(); p++)
	{
		strGroup += *p + ".";
	}

	int nIndex = strGroup.lastIndexOf(".");

	strGroup = strGroup.mid(0, nIndex);

	return strGroup;
}

/*! \fn QString CFesModule::GetUserGroupName(const QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::GetUserGroupName 
** \details 用户变量组名 group1.group2.group3itemitemchanged
** \param index 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2017年4月25日 
** \note 
********************************************************************************************************/
QString CFesModule::GetUserVaraibleGroupName(QModelIndex &index)
{
	QStringList list;
	list.clear();
	auto strGroupTmp = index.data(Qt::EditRole).toString();
	list.append(strGroupTmp);
	while (index.parent().data(Qt::UserRole).toInt() == FES_USER_VARIBALE_GROUP_ITEM)
	{
		index = index.parent();

		list.append(index.data(Qt::EditRole).toString());
	}

	auto it = list.rbegin();
	strGroupTmp.clear();
	for (; it != list.rend(); ++it)
	{
		strGroupTmp += *it + ".";
	}

	if (strGroupTmp.contains("."))
	{
		//删除最后一个.
		strGroupTmp.chop(1);
	}

	return strGroupTmp;
}

/*! \fn QString CFesModule::GetLastVaraibleUserGroupName(QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::GetLastVaraibleUserGroupName 
** \details 
** \param index 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2017年4月25日 
** \note 
********************************************************************************************************/
QString CFesModule::GetLastVaraibleUserGroupName(QModelIndex &index)
{
	QStringList list;
	list.clear();
	auto strGroupTmp = index.data(LASTVALUE).toString();
	list.append(strGroupTmp);
	while (index.parent().data(Qt::UserRole).toInt() == FES_USER_VARIBALE_GROUP_ITEM)
	{
		index = index.parent();

		list.append(index.data(LASTVALUE).toString());
	}

	auto it = list.rbegin();
	strGroupTmp.clear();
	for (; it != list.rend(); ++it)
	{
		strGroupTmp += *it + ".";
	}

	if (strGroupTmp.contains("."))
	{
		//删除最后一个.
		strGroupTmp.chop(1);
	}

	return strGroupTmp;
}

///*! \fn Config::CChannelData *CFesModel::GetChannel(Config::CFesData *pFes, QString &strTag)
//********************************************************************************************************* 
//** \brief CFesModel::GetChannel 
//** \details 查找fes下的通道
//** \param pFes 
//** \param strTag 
//** \return Config::CChannelData * 
//** \author gw
//** \date 2016年7月27日 
//** \note 
//********************************************************************************************************/
//Config::CChannelData *CFesModule::GetChannel(Config::CFesData *pFes, QString &strTag)
//{
//	for (auto const &pChannel : pFes->m_arrChannels)
//	{
//		if (strTag.compare(pChannel->m_szTagName, Qt::CaseSensitive) == 0)
//		{
//			return pChannel;
//		}
//	}
//
//	return nullptr;
//}

/*! \fn void CFesModel::expanded(const QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModel::expanded 
** \details 展开组
** \param index 
** \return void 
** \author gw
** \date 2016年7月29日 
** \note 
********************************************************************************************************/
void CFesModule::expanded(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_ITEM_GROUP)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		item->setIcon(QIcon(OPEN_GROUP_PNG));
	}
}

/*! \fn void CFesModel::collapsed(const QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModel::collapsed 
** \details 折叠组
** \param index 
** \return void 
** \author gw
** \date 2016年7月29日 
** \note 
********************************************************************************************************/
void CFesModule::collapsed(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_ITEM_GROUP)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		item->setIcon(QIcon(CLOSE_GROUP_PNG));
	}
}

/*! \fn void CFesModule::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CFesModule::doubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年9月18日 
** \note 
********************************************************************************************************/
void CFesModule::doubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	IModule *pModule = m_pCore->GetModule("fes");
	
	std::vector<std::string> arr;
	pModule->GetFesConfigName(arr);

	if (nType == FES_ITEM)
	{
		//配置通道
		QString strChannelDesc = index.data().toString();

		//int nType = index.data(Qt::UserRole).toInt();
		QString strTagName = index.data(Qt::EditRole).toString();

		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_WIDGET_TYPE, strTagName);
		if (bFlag)
		{
			return;
		}

		Config::CFesData *pTmp = nullptr;

		//bool bAddChannel = true;

		//int nID = 0;

		long long llRet = index.data(POINTERDATA).toLongLong();

		pTmp = reinterpret_cast<Config::CFesData *>(llRet);
		Q_ASSERT(pTmp);
		if (pTmp == NULL)
		{

		}

		Q_ASSERT(m_pConfig);
		if (!m_pConfig)
		{
			return;
		}

		CFesView *pDialog = new CFesView(m_pCore, pTmp, (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index), m_pConfig);

		//connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), pDialog, SLOT(ItemChanged(QStandardItem *)));

		QString strTab = /*pTmp->m_strGroupName + "." + */index.data(Qt::EditRole).toString();

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct info;
		info.BigTagName = strTagName;
		info.TagName = strTagName;
		info.Type = WIDGET_TYPE::FES_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pDialog, strTab, info, icon);
		m_pUi->GetTabWidget()->setCurrentWidget(pDialog);
	}
}

QWidget* CFesModule::SelectTagDialog(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName)
{
	CSelectPointDialog *pDialog = new CSelectPointDialog(nullptr, m_pConfig, arrAllowDataTypes, bFilterEnWrite, szNodeTagName, szTagName, szFiledName);
	
	return pDialog;
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesModule::ChangeTagNameNodify 
** \details 
** \param tagName 
** \param lastTagName 
** \param nType 
** \return bool 
** \author gw
** \date 2017年6月11日 
** \note 
********************************************************************************************************/
bool CFesModule::ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType)
{
	//前置关联的node tagname修改
	if (Module_ID::NODE_MODULE_FES == nType)
	{
		auto &arr = m_pConfig->GetFecArr();
		for each (auto var in arr)
		{
			auto pFes = var.second;
			pFes->ChangeNodeTagNameNodifyFesUserVaraible(tagName.toStdString(), lastTagName.toStdString());
		}
	}


	return true;
}
