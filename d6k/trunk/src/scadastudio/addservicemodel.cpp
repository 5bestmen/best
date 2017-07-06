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
		else if (index.column() == AppType)
		{
			return m_appTypeMap[m_pConfig->m_arrServerService[index.row()]->AppType];
		}
		else if (index.column() == LoadType)
		{
			return m_loadTypeMap[m_pConfig->m_arrServerService[index.row()]->LoadType];
		}
		else if (index.column() == Argument)
		{
			return m_pConfig->m_arrServerService[index.row()]->Argument;
		}
		else if (index.column() == Description)
		{
			return m_pConfig->m_arrServerService[index.row()]->Description;
		}
		else if (index.column() == Prority)
		{
			return m_pConfig->m_arrServerService[index.row()]->Prority;
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

	m_appTypeMap.clear();
	m_loadTypeMap.clear();

	m_appTypeMap.insert(Inn_APP, QObject::tr("Inn_APP"));
	m_appTypeMap.insert(Ext_App, QObject::tr("Ext_App"));

	m_loadTypeMap.insert(Load_Dll, QObject::tr("Load_Dll"));
	m_loadTypeMap.insert(Load_Exe, QObject::tr("Load_Exe"));

	//LoadType nLoadType;    //! 调用类型：dll形式 ，exe形式
	//AppType  nAppType;	   //! 应用类型：内置应用，扩展服务
	
	//Name, AppType, LoadType, Argument, Description, Prority

	m_horizontal_header_list.append(QObject::tr("ProgramName"));
	m_horizontal_header_list.append(QObject::tr("AppType"));
	m_horizontal_header_list.append(QObject::tr("LoadType"));
	m_horizontal_header_list.append(QObject::tr("Argument"));
	m_horizontal_header_list.append(QObject::tr("Description"));
	m_horizontal_header_list.append(QObject::tr("Prority"));
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