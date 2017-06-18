#include "uservariabletable.h"
#include "sortfiltermodel.h"
#include "uservariablemodle.h"
#include "scadastudio/icore.h"
#include "selectscaledialog.h"
#include "variable_data.h"
#include "config_data.h"
#include "variant.h"
#include "uservariableselectalarmdialog.h"
#include "global.h"

#include <QMenu>
#include <QModelIndex>
#include <QMessageBox>

CUserVariableTable::CUserVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData, Config::CUserVariableGroup *pVariable)
	: QTableView(parent), m_pSort(nullptr), m_pModel(nullptr), m_pCore(nullptr), m_pFesData(nullptr)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;
	
	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}
	m_pFesData = pFesData;

	m_pSort = new CSortFilterModel(this);
	m_pModel = new CUserVariableModel(this, pCore, pFesData, pVariable);

	m_pSort->setSourceModel(m_pModel);

	setModel(m_pSort);

	setContextMenuPolicy(Qt::CustomContextMenu);
	
	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(DoubleClicked(const QModelIndex &)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
}

CUserVariableTable::~CUserVariableTable()
{

}

/*! \fn void CUserVariableTable::DoubleClicked(const QModelIndex & index)
********************************************************************************************************* 
** \brief CUserVariableTable::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年11月30日 
** \note 
********************************************************************************************************/
void CUserVariableTable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();
	auto indexTmp = m_pSort->mapToSource(index);
	if (nCol == CUserVariableModel::COLUMN::SourceTagName)
	{
		auto pUserVariableGroup = m_pModel->GetUserVariableGroup();
		Q_ASSERT(pUserVariableGroup);
		if (!pUserVariableGroup)
		{
			return;
		}
		
		std::string strNodeTagName, strTagName, strFileName;

		auto &nDataType = pUserVariableGroup->m_arrItem[indexTmp.row()]->m_nType;
		std::vector<int> vec;
		vec.clear();
		vec.push_back(nDataType);
		
		//系统变量暂时不考虑
		m_pCore->SelectTag(vec, true, strNodeTagName, strTagName, strFileName);

		//返回字符串为空不赋值
		if (strNodeTagName.empty() || strTagName.empty() || strFileName.empty())
		{
			return;
		}

		if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strFesTagName == strNodeTagName.c_str() &&
			pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strDataTagName == strTagName.c_str() &&
			pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strAttrTagName == strFileName.c_str())
		{
			//绑点没有修改
			return;
		}


		//node.data.attr
		pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strFesTagName = strNodeTagName.c_str();
		pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strDataTagName = strTagName.c_str();
		pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strAttrTagName = strFileName.c_str();

		auto strSourceTagName = QString("%1.%2").arg(strNodeTagName.c_str()).arg(strTagName.c_str());
		if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName.isEmpty())
		{
			//note
			//为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
			if (!m_pFesData->AddUserVariableRelationSourceTagNameArr(strSourceTagName.toStdString(), pUserVariableGroup->m_arrItem[indexTmp.row()]))
			{
				auto strError = QObject::tr("Add UserVariable Relation SourceTagName %1 modify error!").arg(pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}
		else
		{
			//note
			//为了方便查找,hash tagname是node tagname + . + 点tagname，在以后修改或者删除的node tagname或者点tagname就可以直接找到，属性tagname不修改
			auto list = pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName.split(".");
			//3个字段为合法 暂时判定方法
			if (list.count() == 3)
			{
				auto strLastDataTagName = list[1];
				if (!CheckTagNameIsValid(strTagName.c_str(), FES_DESC) || !CheckTagNameIsValid(strLastDataTagName,FES_DESC))
				{
					return;
				}

				QString strLasTagName = QString("%1.%2").arg(list[0]).arg(list[1]);
				if (!m_pFesData->ModifyUserVariableRelationSourceTagNameArr(pUserVariableGroup->m_arrItem[indexTmp.row()], strSourceTagName.toStdString(), strLasTagName.toStdString()))
				{
					auto strError = QObject::tr("SourceTagName %1 modify error!").arg(pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
					
					return;
				}
			}
			else
			{
				auto strError = QObject::tr("SourceTagName %1 is error!").arg(pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName);
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		QString strValue = QString("%1.%2.%3").arg(strNodeTagName.c_str()).arg(strTagName.c_str()).arg(strFileName.c_str());
		m_pModel->setData(indexTmp, strValue, Qt::EditRole);
	}
	else if (nCol == CUserVariableModel::COLUMN::Scale)
	{		
		auto pUserVariableGroup = m_pModel->GetUserVariableGroup();
		Q_ASSERT(pUserVariableGroup);
		if (!pUserVariableGroup)
		{
			return;
		}
		//变量类型
		if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_nType == DT_BOOLEAN)
		{
			//BOOLEAN类型变量没有转换
			pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName = "";

			QMessageBox box;
			box.setText(QObject::tr("boolean type variable has no scale!!!"));
			box.exec();

			return;
		}


		CSelectScaleDialog *pDialog = new CSelectScaleDialog(NULL, m_pFesData, pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName,
			pUserVariableGroup->m_arrItem[indexTmp.row()]->m_nScaleType, pUserVariableGroup->m_arrItem[indexTmp.row()]->m_bNewScalePoint, AISELECTSCALE);
		//
		pDialog->setWindowModality(Qt::ApplicationModal);

		if (pDialog->exec() == QDialog::Accepted)
		{
			//tagname不变
			if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName == pDialog->GetTagName())
			{
				return;
			}
			
			//新添加
			if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName.isEmpty())
			{
				auto bFlag = m_pFesData->AddUserVariableRelationScaleArr(pDialog->GetTagName().toStdString()
					, pUserVariableGroup->m_arrItem[indexTmp.row()]);
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					QMessageBox box;
					box.setText(tr("add hash fail!!!"));

					return;
				}

				pUserVariableGroup->m_arrItem[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
				//pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
				pUserVariableGroup->m_arrItem[indexTmp.row()]->m_bNewScalePoint = false;
			}
			else
			{
				//添加转换tagname关联variable
				auto bFlag = m_pFesData->ModifyUserVariableRelationScaleArr(pUserVariableGroup->m_arrItem[indexTmp.row()], pDialog->GetTagName().toStdString(), pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName.toStdString()
					, pDialog->GetType());
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					QMessageBox box;
					box.setText(tr("add hash fail!!!"));

					return;
				}

				pUserVariableGroup->m_arrItem[indexTmp.row()]->m_nScaleType = pDialog->GetType();
				pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleTagName = pDialog->GetTagName();
				//pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strScaleDesc = pDialog->GetDesc();
				pUserVariableGroup->m_arrItem[indexTmp.row()]->m_bNewScalePoint = false;
			}


		}

		pDialog->deleteLater();
	}
	else if (nCol == CUserVariableModel::COLUMN::Alarm)
	{
		auto pUserVariableGroup = m_pModel->GetUserVariableGroup();
		Q_ASSERT(pUserVariableGroup);
		if (!pUserVariableGroup)
		{
			return;
		}
		
		if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_nType == DT_BOOLEAN)
		{
			CUserVariableSelectAlarmDialog dialog(nullptr, m_pFesData, pUserVariableGroup->m_arrItem[indexTmp.row()], AlarmType::DIALARM);
			if (dialog.exec() == QDialog::Accepted)
			{
				auto pData = pUserVariableGroup->m_arrItem[indexTmp.row()];
				
				//tagname不变
				if (pData->m_strAlarmTagName == dialog.GetTagName())
				{
					return;
				}

				if (pData->m_strAlarmTagName.isEmpty())
				{
					pData->m_strAlarmTagName = dialog.GetTagName();
					pData->m_nAlarmType = dialog.GetType();
					//pData->m_strAlarmDesc = dialog.GetDesc();

					m_pFesData->AddUserVariableRelationAlarmArr(pData->m_strAlarmTagName.toStdString(), pData);
				}
				else
				{
					if (!m_pFesData->ModifyUserVariableRelationAlarmArr(pData, dialog.GetTagName().toStdString(), pData->m_strAlarmTagName.toStdString()))
					{
						auto strError = QObject::tr("User varaible relation alarm %1 modify fail!!!").arg(pData->m_strAlarmTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return;
					}

					pData->m_strAlarmTagName = dialog.GetTagName();
					//pData->m_strAlarmDesc = dialog.GetDesc();
					pData->m_nAlarmType = dialog.GetType();
				}
			}
		}
		else
		{
			CUserVariableSelectAlarmDialog dialog(nullptr, m_pFesData, pUserVariableGroup->m_arrItem[indexTmp.row()], AlarmType::AIALARM);
			if (dialog.exec() == QDialog::Accepted)
			{
				auto pData = pUserVariableGroup->m_arrItem[indexTmp.row()];

				//tagname不变
				if (pData->m_strAlarmTagName == dialog.GetTagName())
				{
					return;
				}

				if (pData->m_strAlarmTagName.isEmpty())
				{
					pData->m_strAlarmTagName = dialog.GetTagName();
					pData->m_nAlarmType = dialog.GetType();
					//pData->m_strAlarmDesc = dialog.GetDesc();

					if (!m_pFesData->AddUserVariableRelationAlarmArr(pData->m_strAlarmTagName.toStdString(), pData))
					{
						auto strError = QObject::tr("Add UserVariable RelationAlarm %1 modify fail!!!").arg(pData->m_strAlarmTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return;
					}
				}
				else
				{
					if (!m_pFesData->ModifyUserVariableRelationAlarmArr(pData, dialog.GetTagName().toStdString(), pData->m_strAlarmTagName.toStdString()))
					{
						auto strError = QObject::tr("Modify UserVariable Relation Alarm  %1 modify fail!!!").arg(pData->m_strAlarmTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						return;
					}

					pData->m_strAlarmTagName = dialog.GetTagName();
					//pData->m_strAlarmDesc = dialog.GetDesc();
					pData->m_nAlarmType = dialog.GetType();
				}
			}
		}

	}
}

/*! \fn void CUserVariableTable::ShowMouseRightButton(const QPoint& point)
*********************************************************************************************************
** \brief CSystemVariableView::ShowMouseRightButton
** \details
** \param point
** \return void
** \author gw
** \date 2016年12月5日
** \note
********************************************************************************************************/
void CUserVariableTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pSort->mapToSource(this->indexAt(point));

	QMenu *pMenu = new QMenu(NULL);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);

	pAddPoint->setIcon(QIcon(CHANNEL_PNG));

	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);

	pMenu->addAction(pDeletePoint);

	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));

	pMenu->addAction(pDeletePoint);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//添加用户变量点
		AddUserVariablePoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//删除模拟量点
		DeleteUserVariablePoint(indexSelect);
	}

	pMenu->deleteLater();
}

/*! \fn void CUserVariableTable::AddUserVariablePoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CUserVariableTable::AddUserVariablePoint 
** \details 添加用户变量
** \param index 
** \return void 
** \author gw
** \date 2016年12月5日 
** \note 
********************************************************************************************************/
void CUserVariableTable::AddUserVariablePoint(QModelIndex &index)
{
	int nRow = index.row();
	auto test = m_pModel->GetArr();
	nRow = (int)test.size();
	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CUserVariableTable::DeleteUserVariablePoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CUserVariableTable::DeleteUserVariablePoint 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月5日 
** \note 
********************************************************************************************************/
void CUserVariableTable::DeleteUserVariablePoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	int nRow = index.row();

	m_pModel->RemoveRows(nRow, 1);
}

