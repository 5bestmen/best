#include "confignodeservicetablemodel.h"
#include "config_data.h"

CConfigNodeServiceTableModel::CConfigNodeServiceTableModel(QObject *parent, Config::CNodeConfigData *pConfig, Config::CNode *pNode)
	: QAbstractTableModel(parent), m_pConfig(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	} 
	m_pConfig = pConfig;

	Q_ASSERT(pNode);
	if (!pNode)
	{
		return;
	}

	m_arrVec.clear();

	if (pNode->m_arrNodeServiceRole.empty())
	{
		//节点服务为空 选用模板的服务
		for each (auto var in m_pConfig->m_arrStandTemplateService)
		{
			if (var->m_strName.compare(NORMAL_SERVER, Qt::CaseInsensitive) == 0)
			{
				m_arrVec = var->m_arrServiceRole;
				pNode->m_arrNodeServiceRole = var->m_arrServiceRole;
			}
		}
	}
	else
	{
		//节点本身包含服务
		m_arrVec = pNode->m_arrNodeServiceRole;
	}
}

CConfigNodeServiceTableModel::~CConfigNodeServiceTableModel()
{

}

int CConfigNodeServiceTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return (int)m_arrVec.size();
}

int CConfigNodeServiceTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	return 1;
}

QVariant CConfigNodeServiceTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int nRow = index.row();
	auto bFlag = nRow >= 0 && nRow < m_arrVec.size();
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole)
	{
		return m_arrVec[index.row()]->Name;
	}
	else if (role == Qt::EditRole)
	{
		return m_arrVec[index.row()]->Name;
	}

	return QVariant();
}

void CConfigNodeServiceTableModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

void CConfigNodeServiceTableModel::appendData(std::vector<Config::NODE_SERVICE_ROLE *> &arrVec)
{
	int newRow = (int)m_arrVec.size();
	int nCount = (int)arrVec.size();
	if (nCount == 0)
	{
		return;
	}
	beginInsertRows(QModelIndex(), newRow, newRow + nCount - 1);
	for each (auto var in arrVec)
	{
		m_arrVec.push_back(var);
	}

	endInsertRows();
}

void CConfigNodeServiceTableModel::deleteData(Config::NODE_SERVICE_ROLE *pData)
{
	beginResetModel();
	auto it = std::find(m_arrVec.begin(), m_arrVec.end(), pData);
	if (it != m_arrVec.end())
	{
		m_arrVec.erase(it);
	}

	endResetModel();
}


