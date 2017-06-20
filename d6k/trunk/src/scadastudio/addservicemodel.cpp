#include "addservicemodel.h"
#include "config_data.h"

CAddServiceModel::CAddServiceModel(QObject *parent, Config::CNodeConfigData *pConfig)
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

CAddServiceModel::~CAddServiceModel()
{

}

int CAddServiceModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return (int)m_pConfig->m_arrServerService.size();
}

int CAddServiceModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return m_horizontal_header_list.size();
}

QVariant CAddServiceModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();
	auto bFlag = nRow >= 0 && nRow < m_pConfig->m_arrServerService.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole)
	{
		if (index.column() == Name)
		{
			return m_pConfig->m_arrServerService[index.row()]->Name;
		}
		else if (index.column() == LoadType)
		{
			return m_pConfig->m_arrServerService[index.row()]->LoadType;
		}
		else if (index.column() == Argument)
		{
			return m_pConfig->m_arrServerService[index.row()]->Argument;
		}
		else if (index.column() == Description)
		{
			return m_pConfig->m_arrServerService[index.row()]->Description;
		}
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == Name)
		{
			return m_pConfig->m_arrServerService[index.row()]->Name;
		}
		else if (index.column() == LoadType)
		{
			return m_pConfig->m_arrServerService[index.row()]->LoadType;
		}
		else if (index.column() == Argument)
		{
			return m_pConfig->m_arrServerService[index.row()]->Argument;
		}
		else if (index.column() == Description)
		{
			return m_pConfig->m_arrServerService[index.row()]->Description;
		}
	}

	return QVariant();
}

void CAddServiceModel::InitPara()
{
	m_horizontal_header_list.clear();

	//LoadType nLoadType;    //! 调用类型：dll形式 ，exe形式
	//AppType  nAppType;	   //! 应用类型：内置应用，扩展服务


	m_horizontal_header_list.append(QObject::tr("ProgramName"));
	m_horizontal_header_list.append(QObject::tr("AppType"));
	m_horizontal_header_list.append(QObject::tr("LoadType"));
	m_horizontal_header_list.append(QObject::tr("LoadType"));

	m_horizontal_header_list.append(QObject::tr("Argument"));
	m_horizontal_header_list.append(QObject::tr("Description"));
}

QVariant CAddServiceModel::headerData(int section, Qt::Orientation orientation, int role) const
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