#include "scadastudio/quoteItem.h"
#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/module_export.h"

#include "predict_module.h"
#include "predict_define.h"
#include "predict_tree.h"
#include "predict_datainfo.h"

#include <QDir>
#include <QDomNode>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStandardItem>



static CPowerPredictApi *s_ptrModule = nullptr;


extern "C" SCADA_EXPORT IModule *CreateModule()
{
	s_ptrModule = new CPowerPredictApi();
	return s_ptrModule;
	//	return  s_ptrModule.get();
}


extern "C" SCADA_EXPORT void DestroyModule()
{
	if (s_ptrModule)
	{
		delete s_ptrModule;
		s_ptrModule = nullptr;
	}
}

CPowerPredictApi * GetPredictModuleApi()
{
	return s_ptrModule;
}

IMainModuleInterface * GetPreIMainInterface()
{
	return s_ptrModule->GetIMainInterface();
}

int CPowerPredictApi::m_nTreeDepth = 0;
/*! \fn CPowerPredictApi::CPowerPredictApi()
************************************************************************
** \brief   CPowerPredictApi::CPowerPredictApi
** \details 预测模块构造
** \param
** \return
** \author  GuoHaijun
** \date    2017年3月29日
** \note
***********************************************************************/
CPowerPredictApi::CPowerPredictApi()
{
	m_pCore = nullptr;
	m_pUi   = nullptr;
	m_pPrdtMgr = nullptr;
}
/*! \fn CPowerPredictApi::CPowerPredictApi()
************************************************************************
** \brief   CPowerPredictApi::CPowerPredictApi
** \details 预测模块构造
** \param
** \return
** \author  GuoHaijun
** \date    2017年3月29日
** \note
***********************************************************************/
CPowerPredictApi::~CPowerPredictApi()
{
	//预测管理，释放内存
	if (m_pPrdtMgr != nullptr)
	{
		delete m_pPrdtMgr;
		m_pPrdtMgr = nullptr;
	}

	//预测左侧树结构指针，释放内存
	if (m_pPredictTree != nullptr)
	{
		delete m_pPredictTree;
		m_pPredictTree = nullptr;
	}
}
/*! \fn void CPowerPredictApi::Init(IMainModuleInterface *pCore)
************************************************************************
** \brief   CPowerPredictApi::Init
** \details 初始化
** \param   pCore  主模块指针
** \return  void
** \author  GuoHaijun
** \date    2017年3月29日
** \note
***********************************************************************/
void CPowerPredictApi::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (pCore == nullptr)
	{
		return;
	}

	//预测管理指针，分配内存
	m_pPrdtMgr = new CPredictMgr();

	m_pCore = pCore;

	m_pUi = pCore->GetUIMgr();

	m_pModel = m_pUi->GetLeftTreeModel();

	
	CTreeWidget* pTree = m_pUi->GetLeftTree();
	Q_ASSERT(pTree);
	//预测左侧树结构指针，分配内存
	m_pPredictTree = new CPredictTree(pTree, m_pUi, m_pCore);
	m_pPredictTree->InitTreeModel();

	connect(m_pPredictTree, SIGNAL(Signl_AddGroup(QModelIndex&)), this, SLOT(Slot_AddGroup(QModelIndex&)));
	connect(m_pPredictTree, SIGNAL(Signl_AddItem(QModelIndex&)), this, SLOT(Slot_AddItem(QModelIndex&)));
	connect(m_pPredictTree, SIGNAL(Signl_DeleteGroup(QModelIndex&)), this, SLOT(Slot_DelGroup(QModelIndex&)));
	connect(m_pPredictTree, SIGNAL(Signl_DeleteItem(QModelIndex&)), this, SLOT(Slot_DelItem(QModelIndex&)));
	connect(m_pPredictTree, SIGNAL(Signl_LoadItem(QModelIndex&)), this, SLOT(Slot_OnLoadItem(const CStationData*, const QModelIndex&)));
	//connect(m_pPredictTree, SIGNAL(Signl_ItemChanged(QStandardItem *)), this, SLOT(Slot_ItemChanged(QStandardItem *)));
	connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(Slot_ItemChanged(QStandardItem *)));
	connect(m_pPredictTree, SIGNAL(Signl_AddInterver(QModelIndex&)), this, SLOT(Slot_AddInverter(QModelIndex&)));
	connect(m_pPredictTree, SIGNAL(Signl_DeleteInverter(QModelIndex&)), this, SLOT(Slot_DelInverter(QModelIndex&)));

	return;
}
/*! \fn void CPowerPredictApi::UnInit()
************************************************************************
** \brief   CPowerPredictApi::UnInit
** \details 卸载
** \param   
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
***********************************************************************/
void CPowerPredictApi::UnInit()
{

	return;
}
/*! \fn void CPowerPredictApi::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot, CQuoteItem *pTopItem)
***************************************************************************************************************************************
** \brief   CPowerPredictApi::CreateNewProject
** \details 新建工程,在工程文件中添加预测节点,并在工程路径下创建预测文件夹powerpredict
** \param   pXml     xml文件指针
** \param   pRoot    xml文件根元素指针
** \param   sxRoot   新建工程路径
** \param   pTopItem item顶节点指针
** \return  void
** \author  GuoHaijun
** \date    2017年3月29日
** \note
***************************************************************************************************************************************/
void CPowerPredictApi::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot, CQuoteItem *pTopItem)
{
	Q_ASSERT(pXml);
	Q_ASSERT(pRoot);
	Q_ASSERT(pTopItem);
	if (pXml == nullptr || pRoot == nullptr || pTopItem == nullptr)
	{
		return;
	}

	//创建目录
	QDir dir;
	bool isExist = dir.mkdir(szRoot + "/" + "powerpredict");
	if (!isExist)
	{
		QString strError = QString(tr("%1/powerprediction folder mkdir failed!!!!!")).arg(szRoot);

		LogMsg(strError.toStdString().c_str(), PREDICT_LOG_LEVEL_0);
	}
	
	//创建预测元素,并插入节点
	QDomElement root = pXml->createElement("powerpredict");
	pRoot->appendChild(root);
	CQuoteItem *pNewItem = new CQuoteItem(QObject::tr("powerprecdict"));
	pNewItem->setData(POWER_PREDICT_ROOT_TYPE, PREDICT_ROOT_ROLE);
	pNewItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(m_pPrdtMgr->m_pRootPrdtGrp);
	pNewItem->setData(reinterpret_cast<long long>(m_pPrdtMgr->m_pRootPrdtGrp), POINTERDATA);
	pNewItem->setIcon(QIcon(POWER_PREDICT_PNG));
	pNewItem->setEditable(false);
	pTopItem->appendRow(pNewItem);

	return;
}
/*! \fn void CPowerPredictApi::LogMsg(const char* szLogTxt, int nLevel)
***********************************************************************
** \brief   CPowerPredictApi::LogMsg
** \details 日志输出
** \param   szlogTxt
** \param   nLevel
** \return  bool
** \author  GuoHaijun
** \date    2017年2月7日
** \note
************************************************************************/
bool CPowerPredictApi::LogMsg(const char* szLogTxt, int nLevel)
{
	Q_ASSERT(szLogTxt && strlen(szLogTxt) > 0);
	if (szLogTxt == nullptr || strlen(szLogTxt) == 0)
	{
		return false;
	}
	Q_ASSERT(m_pCore);
	if (m_pCore == nullptr)
	{
		return false;
	}

	//预测模块名：powerpredict
	return m_pCore->LogMsg("PowerPrediction", szLogTxt, nLevel);
}
/*! \fn void CPowerPredictApi::Slot_AddGroup(QModelIndex &index)
************************************************************************
** \brief    CPowerPredictApi::Slot_AddGroup
** \details  添加组
** \param    index
** \return   void
** \author   Guohaijun
** \date     2017年3月9日
** \note
************************************************************************/
void CPowerPredictApi::Slot_AddGroup(QModelIndex &index)
{
	Q_ASSERT(m_pPrdtMgr);
	if (!m_pPrdtMgr)
	{
		return;
	}

	CQuoteItem* pGetItem = (CQuoteItem*)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	int nType = pGetItem->data(PREDICT_ROOT_ROLE).toInt();

	if (nType == POWER_PREDICT_GROUP_TYPE)
	{
		//厂站组下添加组
		if (pGetItem->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
		{
			auto pPredictGroup = reinterpret_cast<CPredictGroup*>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pPredictGroup);
			if (!pPredictGroup)
			{
				return;
			}

			auto pPredictChildGroup = pPredictGroup->CreateGroup();
			Q_ASSERT(pPredictChildGroup);
			if (pPredictChildGroup == nullptr)
			{
				return;
			}

			CQuoteItem* newItem = new CQuoteItem(pPredictChildGroup->m_strGrpName);
			newItem->setData(pGetItem->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(reinterpret_cast<long long>(pPredictChildGroup), POINTERDATA);
			newItem->setData(POWER_PREDICT_GROUP_TYPE, PREDICT_ROOT_ROLE);
			newItem->setData(pPredictChildGroup->m_strGrpName, PREDICT_OLD_NAME_ROLE);
			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
			newItem->setEditable(true);
			pGetItem->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}
	else
	{
		//根节点下添加厂站组
		if (pGetItem->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
		{
			auto pPrdtGrp = m_pPrdtMgr->m_pRootPrdtGrp->CreateGroup();
			Q_ASSERT(pPrdtGrp);
			if (!pPrdtGrp)
			{
				return;
			}

			CQuoteItem* newItem = new CQuoteItem(pPrdtGrp->m_strGrpName);
			newItem->setData(pPrdtGrp->m_strGrpName, PREDICT_OLD_NAME_ROLE);
			newItem->setData(pGetItem->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(POWER_PREDICT_GROUP_TYPE, PREDICT_ROOT_ROLE);
			newItem->setData(reinterpret_cast<long long>(pPrdtGrp), POINTERDATA);
			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
			newItem->setEditable(true);
			pGetItem->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}

	return;
}
/*! \fn void CPowerPredictApi::GetPredictItemMap(const QAbstractItemModel* pModel, const QModelIndex& top, int nType, QVector<QModelIndex>& indexVec)
******************************************************************************************************************************************************
** \brief    CPowerPredictApi::GetPredictItemMap
** \details  获取预测项map
** \param    pModel
** \param    top
** \param    indexVec
** \return   void
** \author   GuoHaijun
** \date     2017年3月29日
** \note
******************************************************************************************************************************************************/
void CPowerPredictApi::GetPredictItemMap(const QAbstractItemModel* pModel, const QModelIndex& top, int nType, QVector<QModelIndex>& indexVec)
{
	QModelIndex itemIndex;
	if (!top.isValid())
	{
		return;
	}

	for (int row = 0; row < pModel->rowCount(top); row++)
	{
		for (int column = 0; column < pModel->columnCount(top); column++)
		{
			itemIndex = pModel->index(row, column, top);

			if (nType == itemIndex.data(PREDICT_ROOT_ROLE).toInt())
			{
				indexVec.append(itemIndex);
			}
		}

		for (int column = 0; column < pModel->columnCount(top); column++)
		{
			itemIndex = pModel->index(row, column, top);
			GetPredictItemMap(pModel, itemIndex, nType, indexVec);
		}
	}

	return;
}
/*! \fn void CPowerPredictApi::Slot_AddItem(QModelIndex& index)
*******************************************************************
** \brief   CPowerPredictModule::Slot_AddItem
** \details
** \param   index
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*******************************************************************/
void CPowerPredictApi::Slot_AddItem(QModelIndex& index)
{
	CQuoteItem *item = (CQuoteItem*)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	Q_ASSERT(m_pPrdtMgr);
	if (!m_pPrdtMgr)
	{
		return;
	}

	int nType = index.data(PREDICT_ROOT_ROLE).toInt();

	switch (nType)
	{
	case POWER_PREDICT_ROOT_TYPE:
	{	//根节点下,添加单个厂站item
		CStationData* pPredictItem = m_pPrdtMgr->CreateNewPredictItem(m_pPrdtMgr->m_pRootPrdtGrp);

		Q_ASSERT(pPredictItem);
		if (pPredictItem == nullptr)
		{
			return;
		}

		/*CPlantInfo* pStnDatas = pPredictItem->AddPlantData();
		CInverterGroup* pInvDatas = pPredictItem->AddInverterData();
		CWeatherData* pWeaDatas = pPredictItem->AddWeatherData();
		CPredictData* pPrdtDatas = pPredictItem->AddPredictData();*/

		//厂站item
		CQuoteItem* newItem = new CQuoteItem(pPredictItem->GetName());
		newItem->setData(reinterpret_cast<long long>(pPredictItem), POINTERDATA);
		newItem->setData(POWER_PREDICT_ITEM_TYPE, PREDICT_ROOT_ROLE);
		newItem->setData(pPredictItem->GetName(), PREDICT_OLD_NAME_ROLE);
		newItem->setData(pPredictItem->GetName(), STATION_NAME_ROLE);
		newItem->setIcon(QIcon(PREDICT_ITEM_PNG));
		newItem->setEditable(true);
		item->appendRow(newItem);

		//整站数据
		auto strStationData = tr("Plant Data");
		CQuoteItem* itemStationData = new CQuoteItem(strStationData);
		itemStationData->setData(strStationData, PREDICT_OLD_NAME_ROLE);
		itemStationData->setData(reinterpret_cast<long long>(pPredictItem->GetPlantInfo()), POINTERDATA);
		itemStationData->setData(POWER_PREDICT_STATION_TYPE, PREDICT_ROOT_ROLE);
		itemStationData->setData(pPredictItem->GetName(), STATION_NAME_ROLE);
		itemStationData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemStationData->setEditable(false);
		newItem->appendRow(itemStationData);

		//逆变器组数据
		auto strInverterData = tr("Inverter Groups");
		CQuoteItem* itemInverterData = new CQuoteItem(strInverterData);
		itemInverterData->setData(strInverterData, PREDICT_OLD_NAME_ROLE);
		itemInverterData->setData(reinterpret_cast<long long>(pPredictItem->GetInverterGrpInfo()), POINTERDATA);
		itemInverterData->setData(POWER_PREDICT_INVERTER_GROUP_TYPE, PREDICT_ROOT_ROLE);
		itemInverterData->setData(pPredictItem->GetName(), STATION_NAME_ROLE);
		itemInverterData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemInverterData->setEditable(false);
		newItem->appendRow(itemInverterData);

		//气象仪数据
		auto strWeatherData = tr("Weather Data");
		CQuoteItem* itemWeatherData = new CQuoteItem(strWeatherData);
		itemWeatherData->setData(strWeatherData, PREDICT_OLD_NAME_ROLE);
		itemWeatherData->setData(reinterpret_cast<long long>(pPredictItem->GetWeatherInfo()), POINTERDATA);
		itemWeatherData->setData(POWER_PREDICT_WEATHER_TYPE, PREDICT_ROOT_ROLE);
		itemWeatherData->setData(pPredictItem->GetName(), STATION_NAME_ROLE);
		itemWeatherData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemWeatherData->setEditable(false);
		newItem->appendRow(itemWeatherData);

		//预测数据
		auto strPredictData = tr("Predict Data");
		CQuoteItem* itemPredictData = new CQuoteItem(strPredictData);
		itemPredictData->setData(strPredictData, PREDICT_OLD_NAME_ROLE);
		itemPredictData->setData(reinterpret_cast<long long>(pPredictItem->GetPredictInfo()), POINTERDATA);
		itemPredictData->setData(POWER_PREDICT_PREDICT_TYPE, PREDICT_ROOT_ROLE);
		itemPredictData->setData(pPredictItem->GetName(), STATION_NAME_ROLE);
		itemPredictData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemPredictData->setEditable(false);
		newItem->appendRow(itemPredictData);

		m_pPredictTree->GetTree()->expand(index);

		break;
	}
	case POWER_PREDICT_GROUP_TYPE:
	{	//厂站组下,添加单个厂站item
		auto pGroup = reinterpret_cast<CPredictGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (pGroup == nullptr)
		{
			return;
		}

		auto ppItem = m_pPrdtMgr->CreateNewPredictItem(pGroup);

		/*CPlantInfo* pStnDatas = ppItem->AddPlantData();
		CInverterGroup* pInvDatas = ppItem->AddInverterData();
		CWeatherData* pWeaDatas = ppItem->AddWeatherData();
		CPredictData* pPrdtDatas = ppItem->AddPredictData();*/
 
		//厂站item
		CQuoteItem* newItem = new CQuoteItem(ppItem->GetName());
		newItem->setData(reinterpret_cast<long long>(ppItem), POINTERDATA);
		newItem->setData(POWER_PREDICT_ITEM_TYPE, PREDICT_ROOT_ROLE);
		newItem->setData(ppItem->GetName(), PREDICT_OLD_NAME_ROLE);
		newItem->setData(ppItem->GetName(), STATION_NAME_ROLE);
		newItem->setIcon(QIcon(PREDICT_ITEM_PNG));
		newItem->setEditable(true);
		item->appendRow(newItem);

		//整站数据
		QString strStationData = tr("Plant Data");
		CQuoteItem* itemStationData = new CQuoteItem(strStationData);
		itemStationData->setData(strStationData, PREDICT_OLD_NAME_ROLE);
		itemStationData->setData(reinterpret_cast<long long>(ppItem->GetPlantInfo()), POINTERDATA);
		itemStationData->setData(POWER_PREDICT_STATION_TYPE, PREDICT_ROOT_ROLE);
		itemStationData->setData(ppItem->GetName(), STATION_NAME_ROLE);
		itemStationData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemStationData->setEditable(false);
		newItem->appendRow(itemStationData);

		//逆变器组数据
		QString strInverterData = tr("Inverter Group");
		CQuoteItem* itemInverterData = new CQuoteItem(strInverterData);
		itemInverterData->setData(strInverterData, PREDICT_OLD_NAME_ROLE);
		itemInverterData->setData(reinterpret_cast<long long>(ppItem->GetInverterGrpInfo()), POINTERDATA);
		itemInverterData->setData(POWER_PREDICT_INVERTER_GROUP_TYPE, PREDICT_ROOT_ROLE);
		itemInverterData->setData(ppItem->GetName(), STATION_NAME_ROLE);
		itemInverterData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemInverterData->setEditable(false);
		newItem->appendRow(itemInverterData);

		//气象仪数据
		QString strWeatherData = tr("Weather Data");
		CQuoteItem* itemWeatherData = new CQuoteItem(strWeatherData);
		itemWeatherData->setData(strWeatherData, PREDICT_OLD_NAME_ROLE);
		itemWeatherData->setData(reinterpret_cast<long long>(ppItem->GetWeatherInfo()), POINTERDATA);
		itemWeatherData->setData(POWER_PREDICT_WEATHER_TYPE, PREDICT_ROOT_ROLE);
		itemWeatherData->setData(ppItem->GetName(), STATION_NAME_ROLE);
		itemWeatherData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemWeatherData->setEditable(false);
		newItem->appendRow(itemWeatherData);

		//预测数据
		QString strPredictData = tr("Predict Data");
		CQuoteItem* itemPredictData = new CQuoteItem(strPredictData);
		itemPredictData->setData(strPredictData, PREDICT_OLD_NAME_ROLE);
		itemPredictData->setData(reinterpret_cast<long long>(ppItem->GetPredictInfo()), POINTERDATA);
		itemPredictData->setData(POWER_PREDICT_PREDICT_TYPE, PREDICT_ROOT_ROLE);
		itemPredictData->setData(ppItem->GetName(), STATION_NAME_ROLE);
		itemPredictData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemPredictData->setEditable(false);
		newItem->appendRow(itemPredictData);

		m_pPredictTree->GetTree()->expand(index);

		break;
	}
	default:
		break;
	}

	return;
}
/*! \fn void CPowerPredictModule::Slot_DelGroup(QModelIndex& index)
*******************************************************************
** \brief   CPowerPredictModule::Slot_DelGroup
** \details
** \param   index
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*******************************************************************/
void CPowerPredictApi::Slot_DelGroup(QModelIndex& index)
{
	QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("Are you sure to delete station group?"));
	warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
	warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));

	if (warning.exec() == QMessageBox::Cancel)
	{
		return;
	}

	Q_ASSERT(m_pModel);
	if (!m_pModel)
	{
		return;
	}

	auto pItem = reinterpret_cast<CPredictGroup*>(index.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pItem);
	if (pItem == nullptr)
	{
		return;
	}

	auto pChildItem = reinterpret_cast<CPredictGroup*>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pChildItem);
	if (pChildItem == nullptr)
	{
		return;
	}

	QVector<QModelIndex> vecItem;
	vecItem.clear();
	GetPredictItemMap(m_pModel, index, POWER_PREDICT_ROOT_TYPE, vecItem);

	/*
	for each(auto var in vecItem)
	{
	//删除tabwidget
	DeletePredictApiWidgetInTabWidget(index);
	}*/

	for each(const auto vec in vecItem)
	{
		auto bFlag = m_pPrdtMgr->DeleteHashPredictItem(vec.data().toString());
		if (!bFlag)
		{
			auto strError = tr("Delete Predict Item failed!!!");

			LogMsg(strError.toStdString().c_str(), PREDICT_LOG_LEVEL_0);

			return;
		}
	}

	auto strTag = index.data(Qt::EditRole).toString();
	if (!pItem->DeleteGroup(pChildItem, strTag))
	{
		auto strError = tr("Delete Group failed!!!");

		LogMsg(strError.toStdString().c_str(), PREDICT_LOG_LEVEL_0);

		return;
	}

	auto pTop = m_pModel->itemFromIndex(index);

	pTop->parent()->removeRow(pTop->row());

	return;
}

/*! \fn void CPowerPredictApi::Slot_DelItem(QModelIndex& index)
*******************************************************************
** \brief   CPowerPredictModule::Slot_DelItem
** \details
** \param   index
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*******************************************************************/
void CPowerPredictApi::Slot_DelItem(QModelIndex& index)
{
	Q_ASSERT(m_pModel);
	if (!m_pModel)
	{
		return;
	}

	QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("Are you sure to delete station?"));
	warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
	warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));

	if (warning.exec() == QMessageBox::Cancel)
	{
		return;
	}

// 	itemPredictData->setData(strPredictData, PREDICT_OLD_NAME_ROLE);
// 	itemPredictData->setData(reinterpret_cast<long long>(ppItem->GetPredictInfo()), POINTERDATA);
// 	itemPredictData->setData(POWER_PREDICT_PREDICT_TYPE, PREDICT_ROOT_ROLE);

	long long nType = static_cast<long long>(index.data(PREDICT_ROOT_ROLE).toLongLong());

	// 只有厂站才可以删除
	if (nType == POWER_PREDICT_ITEM_TYPE)
	{
		auto pChildItem = reinterpret_cast<CStationData*>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChildItem);
		if (pChildItem == nullptr)
			return;

		auto *pGrp = pChildItem->GetParentGroup();
		Q_ASSERT(pGrp);
		if (pGrp == nullptr)
			return;
		
		
		if (!m_pPrdtMgr->DeleteHashPredictItem(pChildItem->GetName()))
		{
			auto strError = tr("delete predict item fail!!!");
			LogMsg(strError.toStdString().c_str(), PREDICT_LOG_LEVEL_0);
			return;
		}


		bool bRet = pGrp->DeleteItem(pChildItem);

		Q_ASSERT(bRet);
		if (bRet == true)
		{// 如果删除成功，则清理界面
			auto pTop = m_pModel->itemFromIndex(index);
			pTop->parent()->removeRow(pTop->row());
		}
	}	 
#if 0
	auto pItem = reinterpret_cast<CPredictGroup*>(index.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pItem);
	if (pItem == nullptr)
	{
		return;
	}

	auto pChildItem = reinterpret_cast<CStationData*>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pChildItem);
	if (pChildItem == nullptr)
	{
		return;
	}

	QString strPrdtTag = index.data(Qt::EditRole).toString();
	//删除预测管理hash
	if (!m_pPrdtMgr->DeleteHashPredictItem(strPrdtTag))
	{
		auto strError = tr("delete predict item fail!!!");
		LogMsg(strError.toStdString().c_str(), PREDICT_LOG_LEVEL_0);
		return;
	}

	//删除tabwidget
	//DeletePredictApiWidgetInTabWidget(index);

	//删除预测厂站组内存
	if (!pItem->DeleteItem(strPrdtTag))
	{
		auto strError = tr("delete predict item fail!!!");

		LogMsg(strError.toStdString().c_str(), PREDICT_LOG_LEVEL_0);

		return;
	}
	auto pTop = m_pModel->itemFromIndex(index);

	pTop->parent()->removeRow(pTop->row());

#endif

	return;
}

/*! \fn void CPowerPredictApi::Slot_AddInverter(QModelIndex &index)
************************************************************************
** \brief    CPowerPredictApi::Slot_AddInverter
** \details  添加逆变器
** \param    index
** \return   void
** \author   Guohaijun
** \date     2017年4月8日
** \note
************************************************************************/
void CPowerPredictApi::Slot_AddInverter(QModelIndex& index)
{
	CQuoteItem *item = (CQuoteItem*)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	auto pGroup = reinterpret_cast<CInverterGroup* >(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return;
	}

	CInverterInfo*  pInvter = pGroup->AddInverter();

	CQuoteItem* newItem = new CQuoteItem(pInvter->m_szName);
	newItem->setData(reinterpret_cast<long long>(pInvter), POINTERDATA);
	newItem->setData(POWER_PREDICT_INVERTER_TYPE, PREDICT_ROOT_ROLE);
	newItem->setData(pInvter->m_szName, PREDICT_OLD_NAME_ROLE);
	newItem->setIcon(QIcon(PREDICT_DEVICE_PNG));
	newItem->setEditable(true);
	item->appendRow(newItem);

	m_pPredictTree->GetTree()->expand(index);

	/*CTabWidget* pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);

	CPredictInverterWidget* pWidget = new CPredictInverterWidget(pTabWidget, m_pCore);

	pWidget->AddToTableCtrl(pInvter);

	m_pUi->GetTabWidget()->setCurrentWidget(pWidget);*/

}
/*! \fn void CPowerPredictApi::Slot_DelInverter(QModelIndex &index)
************************************************************************
** \brief    CPowerPredictApi::Slot_DelInverter
** \details  删除逆变器
** \param    index
** \return   void
** \author   Guohaijun
** \date     2017年5月16日
** \note
************************************************************************/
void CPowerPredictApi::Slot_DelInverter(QModelIndex& index)
{
	Q_ASSERT(m_pModel);
	if (!m_pModel)
	{
		return;
	}
	QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("Are you sure to delete inverter?"));
	warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
	warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));

	if (warning.exec() == QMessageBox::Cancel)
	{
		return;
	}

	long long nType = static_cast<long long>(index.data(PREDICT_ROOT_ROLE).toLongLong());

	if (nType == POWER_PREDICT_INVERTER_TYPE)
	{
		auto pChildItem = reinterpret_cast<CInverterInfo*>(index.data(POINTERDATA).toLongLong());

		Q_ASSERT(pChildItem);
		if (pChildItem == nullptr)
			return;

		auto pPItem = reinterpret_cast<CInverterGroup*>(index.parent().data(POINTERDATA).toLongLong());

		Q_ASSERT(pPItem);
		if (pPItem == nullptr)
		{
			return;
		}

		Q_ASSERT(pPItem);
		if (pPItem == nullptr)
		{
			return;
		}

		bool bRet = pPItem->DeleteInverter(pChildItem);

		Q_ASSERT(bRet);
		if (bRet == true)
		{// 如果删除成功，则清理界面
			auto pTop = m_pModel->itemFromIndex(index);
			pTop->parent()->removeRow(pTop->row());
		}
	}

	return;
}
/*! \fn void CPowerPredictApi::Slot_ItemChanged(QStandardItem* pItem)
*********************************************************************
** \brief   CPowerPredictApi::Slot_ItemChanged
** \details
** \param   pItem
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*********************************************************************/
void CPowerPredictApi::Slot_ItemChanged(QStandardItem* pItem)
{
	if (pItem == nullptr)
	{
		return;
	}

	auto strOldValue = pItem->data(PREDICT_OLD_NAME_ROLE).toString();
	auto strNewValue = pItem->data(Qt::EditRole).toString();

	if (strNewValue != strOldValue)
	{
		if (strNewValue.isEmpty())
		{
			pItem->setData(strOldValue, Qt::EditRole);

			return;
		}
	}
	else
	{
		return;
	}

	QModelIndex index = pItem->index();
	QVariant varCurrent = index.data(PREDICT_ROOT_ROLE);
	if (!varCurrent.isValid())
	{
		return;
	}

	int nType = varCurrent.toInt();

	switch (nType)
	{
		case POWER_PREDICT_GROUP_TYPE:
		{
			CPredictGroup* pFaGroup = reinterpret_cast<CPredictGroup *>(pItem->parent()->data(POINTERDATA).toLongLong());
			Q_ASSERT(pFaGroup);
			if (pFaGroup == nullptr)
			{
				return;
			}

			CPredictGroup* pChildGroup = reinterpret_cast<CPredictGroup *>(pItem->data(POINTERDATA).toLongLong());
			Q_ASSERT(pChildGroup);
			if (pChildGroup == nullptr)
			{
				return;
			}

			auto newIter = pFaGroup->m_mapStrGrps.find(strNewValue);
			if (newIter != pFaGroup->m_mapStrGrps.end())
			{
				pItem->setData(strOldValue, Qt::EditRole);
				return;
			}

			auto pValue = pFaGroup->m_mapStrGrps[strOldValue];
			auto oldIter = pFaGroup->m_mapStrGrps.find(strOldValue);
			pFaGroup->m_mapStrGrps.erase(oldIter);
			pFaGroup->m_mapStrGrps[strNewValue] = pValue;

			auto pRootValue = m_pPrdtMgr->m_mapRootGrpStrPrdtItem[strOldValue];
			auto oldRootIter = m_pPrdtMgr->m_mapRootGrpStrPrdtItem.find(strOldValue);
			m_pPrdtMgr->m_mapRootGrpStrPrdtItem.erase(oldRootIter);
			m_pPrdtMgr->m_mapRootGrpStrPrdtItem[strNewValue] = pRootValue;
		
			pItem->setData(strNewValue, PREDICT_OLD_NAME_ROLE);

			break;
		}
		case POWER_PREDICT_ITEM_TYPE:
		{
			CPredictGroup* pFaGroup = reinterpret_cast<CPredictGroup* >(pItem->parent()->data(POINTERDATA).toLongLong());
			Q_ASSERT(pFaGroup);
			if (pFaGroup == nullptr)
			{
				return;
			}

			CStationData* pChildItem = reinterpret_cast<CStationData* >(pItem->data(POINTERDATA).toLongLong());
			Q_ASSERT(pChildItem);
			if (pChildItem == nullptr)
			{
				return;
			}

			auto newIter = pFaGroup->m_mapStations.find(strNewValue);
			if (newIter != pFaGroup->m_mapStations.end())
			{
				pItem->setData(strOldValue, Qt::EditRole);
				return;
			}

			auto pValue = pFaGroup->m_mapStations[strOldValue];
			auto oldIter = pFaGroup->m_mapStations.find(strOldValue);
			pFaGroup->m_mapStations.erase(oldIter);
			pFaGroup->m_mapStations[strNewValue] = pValue;

			auto pRootValue = m_pPrdtMgr->m_mapRootGrpStrPrdtItem[strOldValue];
			auto oldRootIter = m_pPrdtMgr->m_mapRootGrpStrPrdtItem.find(strOldValue);
			m_pPrdtMgr->m_mapRootGrpStrPrdtItem.erase(oldRootIter);
			m_pPrdtMgr->m_mapRootGrpStrPrdtItem[strNewValue] = pRootValue;

			pItem->setData(strNewValue, PREDICT_OLD_NAME_ROLE);

			break;
		}
		default:
			break;
	}
}
/*! \fn bool CPowerPredictApi::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot)
*********************************************************************************************************
** \brief   CPowerPredictApi::SaveProject
** \details
** \param   pXml
** \param   pRoot
** \param   szRoot   xxx.proj工程文件绝对路径
** \return  bool
** \author  GuoHaijun
** \date    2017年5月2日
** \note
*********************************************************************************************************/
bool CPowerPredictApi::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot)
{
	Q_ASSERT(pXml);
	Q_ASSERT(pRoot);
	if (pXml == nullptr || pRoot == nullptr)
	{
		return false;
	}
	Q_ASSERT(szRoot.length() > 1);
	if (szRoot.length() <= 1)
	{
		return false;
	}
	QDomElement nRoot = pXml->documentElement();
	for (QDomNode dNode = nRoot.firstChild(); !dNode.isNull(); dNode = dNode.nextSibling())
	{
		QDomElement dElement = dNode.toElement();
		QString strRootName = dElement.tagName();
		if (strRootName.compare("powerpredict", Qt::CaseInsensitive) == 0)
		{
			while (dElement.hasChildNodes())
			{
				QDomNodeList nList = dElement.childNodes();
				for (int i = 0; i < nList.count(); ++i)
				{
					QDomNode& tmpDomNode = nList.at(i);
					dElement.removeChild(tmpDomNode);
				}
			}
			m_pPrdtMgr->SaveProjectPredictNode(pXml, dElement, szRoot);
		}
	}
	return true;
}

/*! \fn bool CPowerPredictApi::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot)
*********************************************************************************************************
** \brief   CPowerPredictApi::LoadProject
** \details
** \param   pXml
** \param   pRoot
** \param   szRoot   xxx.proj工程文件绝对路径
** \return  bool
** \author  GuoHaijun
** \date    2017年5月15日
** \note
*********************************************************************************************************/
bool CPowerPredictApi::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString &szRoot)
{
	Q_ASSERT(pXml);
	Q_ASSERT(pRoot);
	if (pXml == nullptr || pRoot == nullptr || szRoot.isEmpty())
	{
		return false;
	}
	SetPredictPath(szRoot);
	QDir dir;
	if (!dir.exists(m_strPredictPath))
	{
		bool bFlag = dir.mkdir(m_strPredictPath);
		if (!bFlag)
		{
			return false;
		}
	}
	//获取根节点root
	QDomElement root = pXml->documentElement();
	if (root.tagName() != PROJECT_ROOT_NODE_NAME)
	{
		return false;
	}
	//获取根节点下第一个子节点nChild
	QDomNode nChild = pRoot->firstChild();
	for ( ;!nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement domElm = nChild.toElement();
		if (domElm.nodeName() == POWERPREDICT_NODE_NAME)
		{
			//LoadPredictNode(domElm, pTopItem, m_pPrdtMgr->m_pRootPrdtGrp, szRoot);
			m_pPrdtMgr->LoadPredictNode(domElm, szRoot);

			break;
		}
	}

	//左侧树加载
	CQuoteItem* pItem = m_pUi->GetExtendServiceTopItem();
	if (pItem == nullptr)
	{
		return false;
	}
	CQuoteItem* pTopItem = new CQuoteItem(POWERPREDICT_NODE_NAME);
	pTopItem->setData(POWER_PREDICT_ROOT_TYPE, PREDICT_ROOT_ROLE);
	pTopItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(m_pPrdtMgr->m_pRootPrdtGrp);
	pTopItem->setData(reinterpret_cast<long long>(m_pPrdtMgr->m_pRootPrdtGrp), POINTERDATA);
	pTopItem->setIcon(QIcon(POWER_PREDICT_PNG));
	pTopItem->setEditable(false);
	pItem->appendRow(pTopItem);

	CreatePredictGroupUi(m_pPrdtMgr->m_pRootPrdtGrp, pTopItem);

	return true;
}

/*! \fn void CPowerPredictApi::SetPredictPath(const QString& path)
*********************************************************************************************************
** \brief   CPowerPredictApi::SetPredictPath
** \details       设置预测文件保存路径
** \param   path  工程文件路径
** \return  void
** \author  GuoHaijun
** \date    2017年5月15日
** \note
*********************************************************************************************************/
void CPowerPredictApi::SetPredictPath(const QString& path)
{

	m_strPredictPath = path + "/" + POWERPREDICT_SAVE_FOLDER_NAME + "/";
}

/*! \fn bool CPowerPredictApi::LoadPredictNode(QDomElement& domElm, CQuoteItem* pTtem, CPredictGroup* pPrdtGrp,const QString& szRoot)
*********************************************************************************************************
** \brief   CPowerPredictApi::LoadPredictNode
** \details
** \param   domElm   powerpredict子节点
** \param   pTtem
** \param   pPrdtGrp
** \return  bool
** \author  GuoHaijun
** \date    2017年5月12日
** \note
*********************************************************************************************************/
bool CPowerPredictApi::LoadPredictNode(QDomElement& domElm, CQuoteItem* pTtem, CPredictGroup* pPrdtGrp,const QString& szRoot)
{
	if (domElm.isNull() || pTtem == nullptr || pPrdtGrp == nullptr)
	{
		return false;
	}

	CPredictGroup* pPredictGrp = pPrdtGrp->CreateGroup(POWERPREDICT_NODE_NAME);
	pPrdtGrp->m_mapStrGrps[POWERPREDICT_NODE_NAME] = pPredictGrp;

	QDomNode& nNode = domElm.firstChild();
	QDomElement elmData;
	QString tagName;
	for (; !nNode.isNull(); nNode = nNode.nextSibling())
	{
		elmData = nNode.toElement();
		tagName = elmData.tagName();
		if (tagName == POWERPREDICT_GROUP_NODE_NAME)
		{
			m_nTreeDepth = 0;
			LoadGroupNode(elmData, pTtem, pPredictGrp);
		}
		else if (tagName == POWERPREDICT_FILE_NAME)
		{
			QString strItemName = elmData.attribute("name");
			pPredictGrp->m_mapStations[strItemName] = nullptr;

			CStationData* pStn = new CStationData();
			pStn->m_strStationName = strItemName;
			//项添加至父组
			pPredictGrp->m_mapStations.insert(strItemName, pStn);

			CQuoteItem* pNewItem = new CQuoteItem(strItemName);
			pNewItem->setData(POWER_PREDICT_ROOT_TYPE, PREDICT_ROOT_ROLE);
			pNewItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
			Q_ASSERT(m_pPrdtMgr->m_pRootPrdtGrp);
			pNewItem->setData(reinterpret_cast<long long>(m_pPrdtMgr->m_pRootPrdtGrp), POINTERDATA);
			pNewItem->setIcon(QIcon(POWER_PREDICT_PNG));
			pNewItem->setEditable(false);	
			pTtem->appendRow(pNewItem);		

			LoadPredictFile(pStn, pStn->m_strStationName, szRoot);
		}
	}
	return true;
}


/*! \fn bool CPowerPredictApi::LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot)
*********************************************************************************************************
** \brief   CPowerPredictApi::LoadPredictFile
** \details
** \param   pStnData
** \param   fileName
** \param   szRoot
** \return  bool
** \author  GuoHaijun
** \date    2017年5月15日
** \note
*********************************************************************************************************/
bool CPowerPredictApi::LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot)
{
	QXmlStreamReader xml;
	QString strRoot = szRoot;
	//auto nIndex = szRoot.lastIndexOf("/");
	//QString sRoot = strRoot.mid(0, nIndex);
	strRoot += "/powerpredict";
	QString strTmp = strRoot + "/" + fileName + ".xml";
	QFile file(strTmp);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		//qDebug() << file.errorString();
		return false;
	}
	xml.setDevice(&file);

	if (xml.readNextStartElement())
	{
		QString strTmp = xml.name().toString();

		//QString strTmp1 = xml.attributes().value("version").toString();
		if (xml.name() == "plant" /*&& xml.attributes().value("version") == "1.0"*/)
		{
			QString strDesc = xml.attributes().value("Description").toString();
			pStnData->m_strStationName = strDesc;
			//QString strGroup = xml.attributes().value("Group").toString();
			//pStnData->m_pParent->m_strGrpName = strGroup;
			pStnData->LoadStationData(xml);
		}
	}
	file.close();

	return true;
}
/*! \fn void CPowerPredictApi::CreatePredictGroupUi(CPredictGroup* pPtdtGroup, CQuoteItem* pItem)
*********************************************************************************************************
** \brief   CPowerPredictApi::CreatePredictGroupUi
** \details
** \param   pPtdtGroup
** \param   pItem
** \return  void
** \author  GuoHaijun
** \date    2017年5月15日
** \note
*********************************************************************************************************/
void CPowerPredictApi::CreatePredictGroupUi(CPredictGroup* pPtdtGroup, CQuoteItem* pItem)
{
	QString strTmp;
	CQuoteItem* newItem;
	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();
	nDeepths++;
	auto it = pPtdtGroup->m_mapStrGrps.begin();
	for (; it != pPtdtGroup->m_mapStrGrps.end(); it++)
	{
		auto strGroupName = it.value()->m_strGrpName;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(reinterpret_cast<long long>(it.value()), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		pItem->appendRow(newItem);
		CreatePredictGroupUi(it.value(), newItem);
	}

	auto ite = pPtdtGroup->m_mapStations.begin();
	for (; ite != pPtdtGroup->m_mapStations.end(); ++ite)
	{
		//厂站item
		CStationData* pPrdt = ite.value();
		Q_ASSERT(pPrdt);
		if (pPrdt == nullptr)
		{
			continue;
		}
		Q_ASSERT(pPrdt->m_strStationName.isEmpty() == false);
		if (pPrdt->m_strStationName.isEmpty() == true)
		{
			continue;
		}
		CQuoteItem* pPrdtItem = new CQuoteItem(pPrdt->m_strStationName);
		pPrdtItem->setData(pPrdt->m_strStationName, LASTVALUE);
		pPrdtItem->setData(POWER_PREDICT_ITEM_TYPE, PREDICT_ROOT_ROLE);
		pPrdtItem->setData(reinterpret_cast<long long>(pPrdt), POINTERDATA);
		pPrdtItem->setData(pPrdt->GetName(), STATION_NAME_ROLE);
		pPrdtItem->setIcon(QIcon(PREDICT_ITEM_PNG));
		pPrdtItem->setEditable(true);
		pItem->appendRow(pPrdtItem);

		//整站数据
		QString strStationData = pPrdt->GetPlantInfo()->GetName();
		Q_ASSERT(strStationData.isEmpty() == false);
		if (strStationData.isEmpty() == true)
		{
			continue;
		}
		CQuoteItem* itemStationData = new CQuoteItem(strStationData);
		itemStationData->setData(strStationData, LASTVALUE);
		itemStationData->setData(POWER_PREDICT_STATION_TYPE, PREDICT_ROOT_ROLE);
		itemStationData->setData(reinterpret_cast<long long>(pPrdt->GetPlantInfo()), POINTERDATA);
		itemStationData->setData(pPrdt->GetName(), STATION_NAME_ROLE);
		itemStationData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemStationData->setEditable(false);
		pPrdtItem->appendRow(itemStationData);

		//气象仪数据
		auto strWeatherData = pPrdt->GetWeatherInfo()->GetName();
		Q_ASSERT(strWeatherData.isEmpty() == false);
		if (strWeatherData.isEmpty() == true)
		{
			continue;
		}
		CQuoteItem* itemWeatherData = new CQuoteItem(strWeatherData);
		itemWeatherData->setData(strWeatherData, LASTVALUE);
		itemWeatherData->setData(POWER_PREDICT_WEATHER_TYPE, PREDICT_ROOT_ROLE);
		itemWeatherData->setData(reinterpret_cast<long long>(pPrdt->GetWeatherInfo()), POINTERDATA);
		itemWeatherData->setData(pPrdt->GetName(), STATION_NAME_ROLE);
		itemWeatherData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemWeatherData->setEditable(false);
		pPrdtItem->appendRow(itemWeatherData);

		//预测数据
		auto strPredictData = pPrdt->GetPredictInfo()->GetName();
		Q_ASSERT(strPredictData.isEmpty() == false);
		if (strPredictData.isEmpty() == true)
		{
			continue;
		}
		CQuoteItem* itemPredictData = new CQuoteItem(strPredictData);
		itemPredictData->setData(strPredictData, LASTVALUE);
		itemPredictData->setData(POWER_PREDICT_PREDICT_TYPE, PREDICT_ROOT_ROLE);
		itemPredictData->setData(reinterpret_cast<long long>(pPrdt->GetPlantInfo()), POINTERDATA);
		itemPredictData->setData(pPrdt->GetName(), STATION_NAME_ROLE);
		itemPredictData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemPredictData->setEditable(false);
		pPrdtItem->appendRow(itemPredictData);

		//逆变器组数据
		auto strInverterData = pPrdt->GetInverterGrpInfo()->GetName();
		Q_ASSERT(strInverterData.isEmpty() == false);
		if (strInverterData.isEmpty() == true)
		{
			continue;
		}
		CQuoteItem* itemInverterData = new CQuoteItem(strInverterData);
		itemInverterData->setData(strInverterData, LASTVALUE);
		itemInverterData->setData(POWER_PREDICT_INVERTER_GROUP_TYPE, PREDICT_ROOT_ROLE);
		itemInverterData->setData(reinterpret_cast<long long>(pPrdt->GetInverterGrpInfo()), POINTERDATA);
		itemInverterData->setData(pPrdt->GetName(), STATION_NAME_ROLE);
		itemInverterData->setIcon(QIcon(PREDICT_DEVICE_PNG));
		itemInverterData->setEditable(false);
		pPrdtItem->appendRow(itemInverterData);

		auto pGroup = reinterpret_cast<CInverterGroup*>(itemInverterData->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}
		int nCount = pGroup->m_nCount;
		for (int i = 0; i < nCount; i++)
		{
			//auto strInverter = pGroup->m_arrInverters[i].m_szName;
			//CInverterInfo*  pInvter = pGroup->AddInverter();
			CInverterInfo*  pInvter = &(pGroup->m_arrInverters[i]);
			CQuoteItem* itemIvtData = new CQuoteItem(pInvter->m_szName);
			itemIvtData->setData(pInvter->m_szName, LASTVALUE);
			itemIvtData->setData(POWER_PREDICT_INVERTER_TYPE, PREDICT_ROOT_ROLE);
			itemIvtData->setData(reinterpret_cast<long long>(pInvter), POINTERDATA);
			itemIvtData->setData(pPrdt->GetName(), STATION_NAME_ROLE);
			itemIvtData->setIcon(QIcon(PREDICT_DEVICE_PNG));
			itemIvtData->setEditable(true);
			itemInverterData->appendRow(itemIvtData);

			
		}
	
	}
}
/*! \fn bool CPowerPredictApi::LoadGroupNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGroup)
*********************************************************************************************************
** \brief   CPowerPredictApi::LoadGroupNode
** \details
** \param   domElm
** \param   pTtem
** \param   pPrdtGroup
** \return  bool
** \author  GuoHaijun
** \date    2017年5月12日
** \note
*********************************************************************************************************/
bool CPowerPredictApi::LoadGroupNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGroup)
{
	if (domElm.isNull() || pItem == nullptr || pPrdtGroup == nullptr)
	{
		return false;
	}

	QString strGrpName = domElm.attribute("name");
	CPredictGroup* pNewPrdtGrp = pPrdtGroup->CreateGroup(POWERPREDICT_NODE_NAME);
	pPrdtGroup->m_mapStrGrps[strGrpName] = pNewPrdtGrp;

	CQuoteItem* pNewItem = new CQuoteItem(strGrpName);
	m_nTreeDepth++;
	pNewItem->setData(pItem->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
	pNewItem->setData(m_nTreeDepth, Qt::WhatsThisRole);
	pNewItem->setData(reinterpret_cast<long long>(pNewPrdtGrp), POINTERDATA);
	pNewItem->setData(POWER_PREDICT_GROUP_TYPE, PREDICT_ROOT_ROLE);
	pNewItem->setData(pNewPrdtGrp->m_strGrpName, PREDICT_OLD_NAME_ROLE);
	pNewItem->setEditable(true);
	pItem->appendRow(pNewItem);

	QDomNode pNode;
	for (pNode = domElm.firstChild(); !pNode.isNull(); pNode = pNode.nextSibling())
	{
		QDomElement childDomELm = pNode.toElement();
		QString& name = childDomELm.tagName();
		if (name.isEmpty())
		{
			return false;
		}
		if (name == POWERPREDICT_GROUP_NODE_NAME)
		{
			LoadGroupNode(childDomELm, pNewItem, pNewPrdtGrp);
		}
		else if (name == POWERPREDICT_FILE_NAME)
		{
			LoadItemNode(childDomELm, pNewItem, pNewPrdtGrp);
		}
	}

	return true;
}

/*! \fn bool CPowerPredictApi::LoadItemNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGrp)
*********************************************************************************************************
** \brief   CPowerPredictApi::LoadItemNode
** \details
** \param   domElm
** \param   pItem
** \param   pPrdtGrp
** \return  bool
** \author  GuoHaijun
** \date    2017年5月12日
** \note
*********************************************************************************************************/
bool CPowerPredictApi::LoadItemNode(QDomElement& domElm, CQuoteItem* pItem, CPredictGroup* pPrdtGrp)
{
	if (domElm.isNull() || pItem == nullptr || pPrdtGrp == nullptr)
	{
		return false;
	}

	QString strItemName = domElm.attribute("name");
	CQuoteItem* pNewItem = new CQuoteItem(strItemName);
	pNewItem->setData(POWER_PREDICT_ROOT_TYPE, PREDICT_ROOT_ROLE);
	pNewItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	Q_ASSERT(pPrdtGrp);
	pNewItem->setData(reinterpret_cast<long long>(pPrdtGrp), POINTERDATA);
	pNewItem->setIcon(QIcon(POWER_PREDICT_PNG));
	pNewItem->setEditable(false);
	pItem->appendRow(pNewItem);

	return true;
}