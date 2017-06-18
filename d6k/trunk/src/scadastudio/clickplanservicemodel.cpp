#include "clickplanservicemodel.h"
#include "config_data.h"

CClickPlanServiceModel::CClickPlanServiceModel(QObject *parent, Config::CNodeStandardTemplateServiceRole *pData)
	: QAbstractTableModel(parent), m_pConfig(nullptr)
{
	Q_ASSERT(pData);
	if (!pData)
	{
		return;
	}
	
	m_pData = pData;

	InitPara();
}

CClickPlanServiceModel::~CClickPlanServiceModel()
{

}

int CClickPlanServiceModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return (int)m_pData->m_arrServiceRole.size();
}

int CClickPlanServiceModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return m_horizontal_header_list.size();
}

QVariant CClickPlanServiceModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();
	auto bFlag = nRow >= 0 && nRow < m_pData->m_arrServiceRole.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole)
	{
		if (index.column() == Name)
		{
			return m_pData->m_arrServiceRole[index.row()]->Name;
		}
		else if (index.column() == LoadType)
		{
			return m_pData->m_arrServiceRole[index.row()]->LoadType;
		}
		else if (index.column() == Argument)
		{
			return m_pData->m_arrServiceRole[index.row()]->Argument;
		}
		else if (index.column() == Description)
		{
			return m_pData->m_arrServiceRole[index.row()]->Description;
		}
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == Name)
		{
			return m_pData->m_arrServiceRole[index.row()]->Name;
		}
		else if (index.column() == LoadType)
		{
			return m_pData->m_arrServiceRole[index.row()]->LoadType;
		}
		else if (index.column() == Argument)
		{
			return m_pData->m_arrServiceRole[index.row()]->Argument;
		}
		else if (index.column() == Description)
		{
			return m_pData->m_arrServiceRole[index.row()]->Description;
		}
	}

	return QVariant();
}

void CClickPlanServiceModel::InitPara()
{
	m_horizontal_header_list.clear();

	m_horizontal_header_list.append(QObject::tr("Name"));
	m_horizontal_header_list.append(QObject::tr("LoadType"));
	m_horizontal_header_list.append(QObject::tr("Argument"));
	m_horizontal_header_list.append(QObject::tr("Description"));
}

QVariant CClickPlanServiceModel::headerData(int section, Qt::Orientation orientation, int role) const
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