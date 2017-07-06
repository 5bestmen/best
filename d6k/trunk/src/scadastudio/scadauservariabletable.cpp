#include "scadauservariabletable.h"
#include "sortfiltermodel.h"
#include "scadauservariablemodel.h"
#include "scadastudiodefine.h"
#include "scadastudio/icore.h"
#include "scadauservariableselectscaledialog.h"
#include "scadavirable_data.h"
#include "setscadaalarmdialog.h"
#include "variant.h"

#include <QMenu>
#include <QMessageBox>

CScadaUserVariableTable::CScadaUserVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pScadaVirableConfig
	, Config::CScadaUserVariableGroup *pGroup, Config::CScadaData *pScada)
	: QTableView(parent), m_pModel(nullptr), m_pSort(nullptr), m_pCore(nullptr), m_pScadaVirableConfig(nullptr), m_pScada(nullptr)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;

	Q_ASSERT(pScadaVirableConfig);
	if (!pScadaVirableConfig)
	{
		return;
	}
	m_pScadaVirableConfig = pScadaVirableConfig;
	
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}
	m_pScada = pScada;

	m_pSort = new CSortFilterModel(this);
	m_pModel = new CScadaUserVariableModel(this, pCore, pScadaVirableConfig, pGroup, pScada);
	m_pSort->setSourceModel(m_pModel);
	setModel(m_pSort);

	sortByColumn(0, Qt::AscendingOrder);
	setSortingEnabled(true);

	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), SLOT(DoubleClicked(const QModelIndex &)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowMouseRightButton(const QPoint&)));
}

CScadaUserVariableTable::~CScadaUserVariableTable()
{
	if (m_pSort)
	{
		m_pSort->deleteLater();
	}

	if (m_pModel)
	{
		m_pModel->deleteLater();
	}
}

void CScadaUserVariableTable::DoubleClicked(const QModelIndex & index)
{
	int nCol = index.column();
	auto indexTmp = m_pSort->mapToSource(index);
	if (nCol == CScadaUserVariableModel::COLUMN::SourceTagName)
	{
		auto pUserVariableGroup = m_pModel->GetUserVariableGroup();
		Q_ASSERT(pUserVariableGroup);
		if (!pUserVariableGroup)
		{
			return;
		}

		std::string strNodeTagName, strTagName, strFileName;

		std::vector<int> vec;
		vec.clear();

		//ϵͳ������ʱ������
		m_pCore->SelectTag(vec, true, strNodeTagName, strTagName, strFileName);

		//�����ַ���Ϊ�ղ���ֵ
		if (strNodeTagName.empty() || strTagName.empty() || strFileName.empty())
		{
			return;
		}

		//ƴ��SourceTagName nodeTagName.dataTagName.AttrTagName
		//node
		QString strSourceTagName = QString("%1.%2.%3").arg(strNodeTagName.c_str()).arg(strTagName.c_str()).arg(strFileName.c_str());

		if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName == strSourceTagName)
		{
			//���û���޸�
			return;
		}

		if (pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName.isEmpty())
		{
			//strSourceTagNameΪ�գ���ӹ�����ϵ
			//note
			//Ϊ�˷������,hash tagname��node tagname + . + ��tagname�����Ժ��޸Ļ���ɾ����node tagname���ߵ�tagname�Ϳ���ֱ���ҵ�������tagname���޸�
			
			auto strTagNameCombine = QString("%1.%2").arg(strNodeTagName.c_str()).arg(strTagName.c_str());
			if (!m_pScada->AddUserVariableRelationSourceTagNameArr(strTagNameCombine.toStdString(), pUserVariableGroup->m_arrItem[indexTmp.row()]))
			{
				auto strError = QObject::tr("Add Scada varaible UserVariable Relation SourceTagName %1 modify error!").arg(strSourceTagName);
				m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}
		else
		{
			//strSourceTagName��Ϊ�գ��޸Ĺ�����ϵ

			auto list = pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName.split(".");
			//3���ֶ�Ϊ�Ϸ� ��ʱ�ж�����
			if (list.count() == 3)
			{
				QString strTagNameCombine = QString("%1.%2").arg(strNodeTagName.c_str()).arg(strTagName.c_str());
				QString strLastTagNameCombine = QString("%1.%2").arg(list[0]).arg(list[1]);
				if (!m_pScada->ModifyUserVariableRelationSourceTagNameArr(strTagNameCombine.toStdString(), strLastTagNameCombine.toStdString(), pUserVariableGroup->m_arrItem[indexTmp.row()]))
				{
					auto strError = QObject::tr("Modify Scada varaible UserVariable Relation SourceTagName %1 modify error!").arg(strSourceTagName);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

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

		pUserVariableGroup->m_arrItem[indexTmp.row()]->m_strSourceTagName = strSourceTagName;

	}
	else if (nCol == CScadaUserVariableModel::COLUMN::Scale)
	{
		//ѡ��ת��
		auto arr = m_pModel->GetArr();
		auto nRow = indexTmp.row();
		auto bFlag = nRow < 0 || nRow >= arr.size();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strTagName = arr[nRow]->m_strScaleTagName;

		CScadaUserVariableSelectScaleDialog dialog(nullptr, m_pScadaVirableConfig, strTagName, m_pScada);
		dialog.setWindowModality(Qt::ApplicationModal);
		if (dialog.exec() == QDialog::Accepted)
		{
			if (arr[indexTmp.row()]->m_strScaleTagName == dialog.GetTagName())
			{
				return;
			}
			
			if (arr[indexTmp.row()]->m_strScaleTagName.isEmpty())
			{
				//���

				//���ת��tagname����variable
				auto bFlag = m_pScada->AddScadaVariableRelationScaleArr(dialog.GetTagName().toStdString(), arr[indexTmp.row()]);
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					QMessageBox box;
					box.setText(tr("add hash fail!!!"));

					return;
				}

				arr[indexTmp.row()]->m_nScaleType = dialog.GetType();
				arr[indexTmp.row()]->m_strScaleTagName = dialog.GetTagName();
			}
			else
			{
				//���

				//���ת��tagname����variable
				auto bFlag = m_pScada->ModifyUserVariableRelationScaleArr(arr[indexTmp.row()], dialog.GetTagName().toStdString(), arr[indexTmp.row()]->m_strScaleTagName.toStdString()
					, dialog.GetType());
				Q_ASSERT(bFlag);
				if (!bFlag)
				{
					QMessageBox box;
					box.setText(tr("add hash fail!!!"));

					return;
				}

				arr[indexTmp.row()]->m_nScaleType = dialog.GetType();
				arr[indexTmp.row()]->m_strScaleTagName = dialog.GetTagName();
			}



		}
	}
	else if (nCol == CScadaUserVariableModel::COLUMN::Alarm)
	{
		auto arr = m_pModel->GetArr();
		auto nRow = indexTmp.row();
		auto bFlag = nRow < 0 || nRow >= arr.size();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{

		}

		auto pData = arr[nRow];

		auto strTagName = arr[nRow]->m_strAlarmTagName;
		 
		if (pData->m_nType == DT_BOOLEAN)
		{
			CSetScadaAlarmDialog dialog(nullptr, m_pScadaVirableConfig, strTagName, m_pScada, DIALARM);
			dialog.setWindowModality(Qt::ApplicationModal);

			if (dialog.exec() == QDialog::Accepted)
			{
				if (pData->m_strAlarmTagName == dialog.GetTagName())
				{
					return;
				}

				if (arr[indexTmp.row()]->m_strAlarmTagName.isEmpty())
				{
					pData->m_strAlarmTagName = dialog.GetTagName();
					pData->m_strDescription = dialog.GetDesc();
					pData->m_nAlarmType = dialog.GetType();

					m_pScada->AddScadaVariableRelationAlarmArr(pData->m_strAlarmTagName.toStdString(), pData);
				}
				else
				{
					auto bFlag = m_pScada->ModifyUserVariableRelationAlarmArr(pData, dialog.GetTagName().toStdString(), pData->m_strAlarmTagName.toStdString());
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						QMessageBox box;
						box.setText(QObject::tr("Set related alarm fail!!!"));
						box.exec();
					}

					pData->m_strAlarmTagName = dialog.GetTagName();
					pData->m_strDescription = dialog.GetDesc();
					pData->m_nAlarmType = dialog.GetType();
				}
			}
		}
		else
		{
			CSetScadaAlarmDialog dialog(nullptr, m_pScadaVirableConfig, strTagName, m_pScada, AIALARM);
			dialog.setWindowModality(Qt::ApplicationModal);

			if (dialog.exec() == QDialog::Accepted)
			{
				if (pData->m_strAlarmTagName == dialog.GetTagName())
				{
					return;
				}

				if (arr[indexTmp.row()]->m_strAlarmTagName.isEmpty())
				{
					pData->m_strAlarmTagName = dialog.GetTagName();
					pData->m_strDescription = dialog.GetDesc();
					pData->m_nAlarmType = dialog.GetType();

					auto bFlag = m_pScada->AddScadaVariableRelationAlarmArr(pData->m_strAlarmTagName.toStdString(), pData);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						QMessageBox box;
						box.setText(QObject::tr("Add related alarm fail!!!"));
						box.exec();

						return;
					}
				}
				else
				{
					auto bFlag = m_pScada->ModifyUserVariableRelationAlarmArr(pData, dialog.GetTagName().toStdString(), pData->m_strAlarmTagName.toStdString());
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						QMessageBox box;
						box.setText(QObject::tr("Set related alarm fail!!!"));
						box.exec();

						return;
					}

					pData->m_strAlarmTagName = dialog.GetTagName();
					pData->m_strDescription = dialog.GetDesc();
					pData->m_nAlarmType = dialog.GetType();
				}
			}
		}
	}
}

void CScadaUserVariableTable::ShowMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = /*m_pSort->mapToSource(*/this->indexAt(point)/*)*/;

	QMenu *pMenu = new QMenu(NULL);

	QAction *pAddPoint = new QAction(tr("add point"), pMenu);
	pAddPoint->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pAddPoint);

	QAction *pDeletePoint = new QAction(tr("delete point"), pMenu);
	pDeletePoint->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pDeletePoint);

	QAction *pClearRelationPoint = nullptr;
	if (indexSelect.column() == CScadaUserVariableModel::COLUMN::SourceTagName ||
		 indexSelect.column() == CScadaUserVariableModel::COLUMN::Alarm ||
		indexSelect.column() == CScadaUserVariableModel::COLUMN::Scale)
	{
		pClearRelationPoint = new QAction(QObject::tr("Clear Relation"), pMenu);
		pClearRelationPoint->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pClearRelationPoint);
	}

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pAddPoint)
	{
		//����û�������
		AddUserVariablePoint(indexSelect);
	}
	else if (selectedItem == pDeletePoint)
	{
		//ɾ��ģ������
		DeleteUserVariablePoint(indexSelect);
	}
	else if (selectedItem == pClearRelationPoint && pClearRelationPoint != nullptr)
	{
		//��չ���
		m_pModel->setData(indexSelect, Qt::EditRole);
	}

	pMenu->deleteLater();
}

/*! \fn void CScadaUserVariableTable::AddUserVariablePoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaUserVariableTable::AddUserVariablePoint 
** \details ����û�����
** \param index 
** \return void 
** \author gw
** \date 2016��12��29�� 
** \note 
********************************************************************************************************/
void CScadaUserVariableTable::AddUserVariablePoint(QModelIndex &index)
{
	int nRow;
	auto test = m_pModel->GetArr();
	nRow = (int)test.size();
	m_pModel->InsertRows(nRow, 1);
}

/*! \fn void CScadaUserVariableTable::DeleteUserVariablePoint(QModelIndex &index)
********************************************************************************************************* 
** \brief CScadaUserVariableTable::DeleteUserVariablePoint 
** \details ɾ���û�����
** \param index 
** \return void 
** \author gw
** \date 2016��12��29�� 
** \note 
********************************************************************************************************/
void CScadaUserVariableTable::DeleteUserVariablePoint(QModelIndex &index)
{
	if (!index.isValid())
	{
		return;
	}
	
	auto indexTmp = m_pSort->mapToSource(index);
	
	int nRow = indexTmp.row();

	m_pModel->RemoveRows(nRow, 1);
}

void CScadaUserVariableTable::Refresh()
{
	m_pModel->RefrushModel();
}
