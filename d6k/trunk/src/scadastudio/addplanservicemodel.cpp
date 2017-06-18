#include "addplanservicemodel.h"
#include "config_data.h"

CAddPlanServiceModel::CAddPlanServiceModel(QObject *parent, Config::CNodeConfigData *pConfig)
	: QAbstractTableModel(parent), m_pConfig(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	
	m_pConfig = pConfig;

	InitPara();
}

CAddPlanServiceModel::~CAddPlanServiceModel()
{

}

int CAddPlanServiceModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return (int)m_pConfig->m_arrStandTemplateService.size();
}

int CAddPlanServiceModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return m_horizontal_header_list.size();
}

QVariant CAddPlanServiceModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();
	auto bFlag = nRow >= 0 && nRow < m_pConfig->m_arrStandTemplateService.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole)
	{
		if (index.column() == PlanName)
		{
			return m_pConfig->m_arrStandTemplateService[index.row()]->m_strName;
		}
		else if (index.column() == Description)
		{
			return m_pConfig->m_arrStandTemplateService[index.row()]->m_strDesc;
		}
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == PlanName)
		{
			return m_pConfig->m_arrStandTemplateService[index.row()]->m_strName;
		}
		else if (index.column() == Description)
		{
			return m_pConfig->m_arrStandTemplateService[index.row()]->m_strDesc;
		}
	}

	return QVariant();
}

void CAddPlanServiceModel::InitPara()
{
	m_horizontal_header_list.clear();

	m_horizontal_header_list.append(QObject::tr("PlanName"));
	m_horizontal_header_list.append(QObject::tr("Description"));
}

QVariant CAddPlanServiceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section && section > -1)
				return m_horizontal_header_list[section];
			else
				return QVariant();
		}
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // 垂直表头  
			//else
			//return QVariant();

			return QString(" ");
		}
	}

	return QVariant();
}