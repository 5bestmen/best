#include "nodeservermodel.h"
#include "scadastudiodefine.h"
#include "delegate.h"
#include "nodeservertable.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "global.h"
#include "scadastudio/icore.h"
#include "scadastudio/imodule.h"

#include <QPushButton>

#define SERVER_MAX_COUNT 2

CNodeServerModel::CNodeServerModel(QObject *parent, IMainModuleInterface *pCore, Config::CNodeServer *pServerData
	, Config::CNodeConfigData *pNodeConfig)
	: QAbstractTableModel(parent), m_pServerData(nullptr), m_pNodeConfig(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pServerData);
	Q_ASSERT(pNodeConfig);
	if (!pCore)
	{
		return;
	}
	if (!pServerData)
	{
		return;
	}
	if (!pNodeConfig)
	{
		return;
	}

	m_pServerData = pServerData;
	m_pCore = pCore;
	m_pNodeConfig = pNodeConfig;

	InitPara();
}

CNodeServerModel::~CNodeServerModel()
{

}

int CNodeServerModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pServerData->m_arrNodes.size();
}

int CNodeServerModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return 0;
}

QVariant CNodeServerModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == TagName)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pServerData->m_arrNodes[index.row()]->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pServerData->m_arrNodes[index.row()]->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pServerData->m_arrNodes[index.row()]->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strConfig;
		}
		else if (index.column() == Service)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strService;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pServerData->m_arrNodes[index.row()]->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pServerData->m_arrNodes[index.row()]->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pServerData->m_arrNodes[index.row()]->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strConfig;
		}
		else if (index.column() == Service)
		{
			return m_pServerData->m_arrNodes[index.row()]->m_strService;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CNodeServerModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CNodeServerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	//int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == TagName)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_szTagName != value.toString())
				{
					if (m_pNodeConfig->CheckTagNameIsExist(value.toString()))
					{
						return false;
					}
					auto strTmp = value.toString();
					if (!CheckTagNameIsValid(strTmp, NODE_CONFIG_DESC))
					{
						return false;
					}

					//hash修改
					if (!m_pNodeConfig->ModifyItemHashMap(value.toString(), m_pServerData->m_arrNodes[index.row()]->m_szTagName))
					{
						return false;
					}

					memset(m_pServerData->m_arrNodes[index.row()]->m_szTagName, 0, sizeof(m_pServerData->m_arrNodes[index.row()]->m_szTagName));
					int nSize = strTmp.size();
					strncpy(m_pServerData->m_arrNodes[index.row()]->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				}
			}
			else if (index.column() == HostName)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_strHostName != value.toString())
				{
					auto strTmp = value.toString();
					
					if (!CheckDescIsValid(strTmp, NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pServerData->m_arrNodes[index.row()]->m_strHostName = strTmp;
				}
			}
			else if (index.column() == OccNo)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_nOccNo != value.toInt())
				{
					m_pServerData->m_arrNodes[index.row()]->m_nOccNo = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_nBlockNo != value.toInt())
				{
					m_pServerData->m_arrNodes[index.row()]->m_nBlockNo = value.toInt();
				}
			}
			else if (index.column() == Network_IP_A)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_A != value.toString())
				{
					m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_A = value.toString();
				}
			}
			else if (index.column() == Network_IP_B)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_B != value.toString())
				{
					m_pServerData->m_arrNodes[index.row()]->m_strNetwork_IP_B = value.toString();
				}
			}
			else if (index.column() == IsTimeSource)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_bIsTimeSource != value.toBool())
				{
					m_pServerData->m_arrNodes[index.row()]->m_bIsTimeSource = value.toBool();
				}
			}
			else if (index.column() == Program)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_strProgram != value.toString())
				{
					auto strTmp = value.toString();

					if (!CheckDescIsValid(strTmp, NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pServerData->m_arrNodes[index.row()]->m_strProgram = value.toString();
				}
			}
			else if (index.column() == Config)
			{
				if (m_pServerData->m_arrNodes[index.row()]->m_strProgram != value.toString())
				{
					auto strTmp = value.toString();

					if (!CheckTagNameIsValid(strTmp, NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pServerData->m_arrNodes[index.row()]->m_strConfig = value.toString();
				}
			}
			else if (index.column() == Service)
			{
				
			}
			//else if (index.column() == NodeType)
			//{
			//	if (m_pServerData->m_arrNodes[index.row()]->m_nNodeType != value.toInt())
			//	{
			//		m_pServerData->m_arrNodes[index.row()]->m_nNodeType = value.toInt();
			//	}
			//}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CNodeServerModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CNodeServerModel::InitPara()
{
	//m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("TagName");
	para.tagname = "TagName";
	//m_arrInfo.insert(ID, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("HostName");
	para.tagname = "HostName";
	//m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditHostName = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(HostName, pLineEditHostName);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(BlockNo, pReadOnlyDelegate);

	//para.desc = tr("NodeType");
	//para.tagname = "NodeType";
	////m_arrInfo.insert(Condition, para);
	//m_horizontal_header_list.append(para.desc);
	//QVector<IDDESC> vec;
	//vec.clear();
	//IDDESC tmp;
	//tmp.desc = tr("SERVER");
	//tmp.ID = QString("%1").arg(NODETYPE::SERVER);
	//vec.append(tmp);
	//tmp.desc = tr("FEP");
	//tmp.ID = QString("%1").arg(NODETYPE::FEP);
	//vec.append(tmp);
	//tmp.desc = tr("WORKSTAIOTN");
	//tmp.ID = QString("%1").arg(NODETYPE::WORKSTAIOTN);
	//vec.append(tmp);
	//tmp.desc = tr("HISTORYSERVER");
	//tmp.ID = QString("%1").arg(NODETYPE::HISTORYSERVER);
	//vec.append(tmp);
	//tmp.desc = tr("WEBSERVER");
	//tmp.ID = QString("%1").arg(NODETYPE::WEBSERVER);
	//vec.append(tmp);
	//CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, vec);
	//((CNodeServerTable *)parent())->setItemDelegateForColumn(NodeType, pCombo);

	para.desc = tr("Network_IP_A");
	para.tagname = "Network_IP_A";
	//m_arrInfo.insert(Category, para);
	m_horizontal_header_list.append(para.desc);
	auto pIPDelegate = new CLineEditDelegate(this, IPADDRESS);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(Network_IP_A, pIPDelegate);


	para.desc = tr("Network_IP_B");
	para.tagname = "Network_IP_B";
	//m_arrInfo.insert(DelayAlarm, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(Network_IP_B, pIPDelegate);

	para.desc = tr("IsTimeSource");
	para.tagname = "IsTimeSource";
	//m_arrInfo.insert(Priority, para);
	m_horizontal_header_list.append(para.desc);
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(IsTimeSource, pCheck);

	para.desc = tr("Program");
	para.tagname = "Program";
	//m_arrInfo.insert(SupportAck, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(Program, pLineEditHostName);

	para.desc = tr("Config");
	para.tagname = "Config";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	
	std::vector<QString> vecConfigNames;
	vecConfigNames.clear();
	m_pCore->GetModulesName(vecConfigNames);
	auto pScadaVariableModule = m_pCore->GetModule("scada_variable");
	std::vector<std::string> vecTmp;
	vecTmp.clear();
	pScadaVariableModule->GetScadaConfigName(vecTmp);
	QVector<IDDESC> vec;
	vec.clear();
	IDDESC tmp;
	vec.clear();
	for each (auto var in vecTmp)
	{
		tmp.ID = var.c_str();
		tmp.desc = var.c_str();

		vec.push_back(tmp);
	}
	auto pConfigComboxbox = new CComboBoxDelegate(this, vec, true);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(Config, pConfigComboxbox);

	para.desc = tr("Service");
	para.tagname = "Service";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeServerTable *)parent())->setItemDelegateForColumn(Service, pReadOnlyDelegate);
}

int CNodeServerModel::GetSize()
{
	return (int)m_pServerData->m_arrNodes.size();
}

bool CNodeServerModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	auto nCount = m_pServerData->m_arrNodes.size();

	if (nCount == SERVER_MAX_COUNT)
	{
		return true;
	}

	beginInsertRows(parent, position, position + rows - 1);

	for (int i = position; i < position + rows; i++)
	{
		auto pNode = m_pNodeConfig->AddServerNodeItem();
		Q_ASSERT(pNode);
		if (!pNode)
		{
			endInsertRows();

			return false;
		}

		pNode->m_nNodeType = NODETYPE::SERVER;
		pNode->m_strService = QObject::tr(ServiceConfig);

		m_pServerData->m_arrNodes.push_back(pNode);

		////pNode->m_strTagName = tr("node name%1").arg(i + 1);
		////pNode->m_strHostName = tr("host name%1").arg(i + 1);
		////pNode->m_nOccNo = 0;
		////pNode->m_nBlockNo = 0;
		////pNode->m_strNetwork_IP_A = "";
		////pNode->m_strNetwork_IP_B = "";
		////pNode->m_bIsTimeSource = false;
		////pNode->m_strProgram = "";

		//m_pServerData->m_arrNodes.push_back(pNode);
	}



	endInsertRows();

	return true;
}

/*! \fn bool CNodeServerModel::RemoveRows(int position, int rows, const QModelIndex &parent)
*********************************************************************************************************
** \brief Config::CUserVariableGroup::CreateGroup
** \details
** \return CUserVariableGroup *
** \author gw
** \date 2016年11月29日
** \note
********************************************************************************************************/
bool CNodeServerModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);
	//auto nCount = rows;

	auto &arr = m_pServerData->m_arrNodes;
	if (arr.size() == 0)
	{
		return true;
	}
	
	bool bFlag = true;

	if (position >= 0 && position < m_pServerData->m_arrNodes.size())
	{
		
		auto pNode = arr[position];

		if (!m_pNodeConfig->DeleteHashItem(pNode->m_szTagName))
		{
			bFlag = false;
			endRemoveRows();

			return bFlag;
		}

		auto it = std::find(arr.begin(), arr.end(), pNode);
		if (it != arr.end())
		{
			arr.erase(it);
		}
		else
		{
			bFlag = false;
			endRemoveRows();

			return bFlag;
		}

		delete pNode;
	}
	else
	{	
		bFlag = false;
	}

	endRemoveRows();

	if (m_pServerData->m_arrNodes.size() == 0)
	{
		beginResetModel();
		endResetModel();
	}

	return bFlag;
}