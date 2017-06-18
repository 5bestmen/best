#include "nodeworkstationmodel.h"
#include "config_data.h"
#include "delegate.h"
#include "nodeworkstationtable.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/icore.h"
#include "global.h"
#include "scadastudio/imodule.h"

CNodeWorkstationModel::CNodeWorkstationModel(QObject *parent, IMainModuleInterface *pCore, Config::CNode *pWorkstationData
	, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig)
	: QAbstractTableModel(parent), m_pWorkstationData(nullptr), m_pCore(nullptr), m_pItem(nullptr), m_pNodeConfig(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pWorkstationData);
	Q_ASSERT(pItem);
	Q_ASSERT(pNodeConfig);

	if (!pCore)
	{
		return;
	}

	if (!pWorkstationData)
	{
		return;
	}

	if (!pItem)
	{
		return;
	}

	if (!pNodeConfig)
	{
		return;
	}


	m_pCore = pCore;
	m_pWorkstationData = pWorkstationData;
	m_pItem = pItem;
	m_pNodeConfig = pNodeConfig;


	InitPara();
}

CNodeWorkstationModel::~CNodeWorkstationModel()
{

}

int CNodeWorkstationModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CNodeWorkstationModel::columnCount(const QModelIndex &parent) const
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

QVariant CNodeWorkstationModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == TagName)
		{
			return m_pWorkstationData->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pWorkstationData->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pWorkstationData->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pWorkstationData->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pWorkstationData->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pWorkstationData->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pWorkstationData->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pWorkstationData->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pWorkstationData->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pWorkstationData->m_strConfig;
		}
		else if (index.column() == Service)
		{
			return m_pWorkstationData->m_strService;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pWorkstationData->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pWorkstationData->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pWorkstationData->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pWorkstationData->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pWorkstationData->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pWorkstationData->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pWorkstationData->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pWorkstationData->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pWorkstationData->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pWorkstationData->m_strConfig;
		}
		else if (index.column() == Service)
		{
			return m_pWorkstationData->m_strService;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CNodeWorkstationModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CNodeWorkstationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	//int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == TagName)
			{
				if (m_pWorkstationData->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}

					if (m_pNodeConfig->CheckTagNameIsExist(value.toString()))
					{
						return false;
					}
					
					//nodemodel里itemchanged处理
					m_pItem->setData(value, Qt::EditRole);
				}
			}
			else if (index.column() == HostName)
			{
				if (m_pWorkstationData->m_strHostName != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pWorkstationData->m_strHostName = value.toString();

					m_pItem->setData(value, TREE_TOOL_TIP_ROLE);
				}
			}
			else if (index.column() == OccNo)
			{
				if (m_pWorkstationData->m_nOccNo != value.toInt())
				{
					m_pWorkstationData->m_nOccNo = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pWorkstationData->m_nBlockNo != value.toInt())
				{
					m_pWorkstationData->m_nBlockNo = value.toInt();
				}
			}
			//else if (index.column() == NodeType)
			//{
			//	if (m_pWorkstationData->m_nNodeType != value.toInt())
			//	{
			//		m_pWorkstationData->m_nNodeType = value.toInt();
			//	}
			//}
			else if (index.column() == Network_IP_A)
			{
				if (m_pWorkstationData->m_strNetwork_IP_A != value.toString())
				{
					m_pWorkstationData->m_strNetwork_IP_A = value.toString();
				}

			}
			else if (index.column() == Network_IP_B)
			{
				if (m_pWorkstationData->m_strNetwork_IP_B != value.toString())
				{
					m_pWorkstationData->m_strNetwork_IP_B = value.toString();
				}
			}
			else if (index.column() == IsTimeSource)
			{
				if (m_pWorkstationData->m_bIsTimeSource != value.toBool())
				{
					m_pWorkstationData->m_bIsTimeSource = value.toInt();
				}
			}
			else if (index.column() == Program)
			{
				if (m_pWorkstationData->m_strProgram != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pWorkstationData->m_strProgram = value.toString();
				}
			}
			else if (index.column() == Config)
			{
				if (m_pWorkstationData->m_strConfig != value.toString())
				{
					m_pWorkstationData->m_strConfig = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CNodeWorkstationModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CNodeWorkstationModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

void CNodeWorkstationModel::InitPara()
{
	//m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("TagName");
	para.tagname = "TagName";
	//m_arrInfo.insert(ID, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("HostName");
	para.tagname = "HostName";
	//m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditName = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(HostName, pLineEditName);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(BlockNo, pReadOnlyDelegate);

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
	//((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(NodeType, pCombo);

	para.desc = tr("Network_IP_A");
	para.tagname = "Network_IP_A";
	//m_arrInfo.insert(Category, para);
	m_horizontal_header_list.append(para.desc);
	auto pIPDelegate = new CLineEditDelegate(this, IPADDRESS);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(Network_IP_A, pIPDelegate);


	para.desc = tr("Network_IP_B");
	para.tagname = "Network_IP_B";
	//m_arrInfo.insert(DelayAlarm, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(Network_IP_B, pIPDelegate);

	para.desc = tr("IsTimeSource");
	para.tagname = "IsTimeSource";
	//m_arrInfo.insert(Priority, para);
	m_horizontal_header_list.append(para.desc);
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(IsTimeSource, pCheck);

	para.desc = tr("Program");
	para.tagname = "Program";
	//m_arrInfo.insert(SupportAck, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(Program, pLineEditName);

	para.desc = tr("Config");
	para.tagname = "Config";
	//m_arrInfo.insert(SupportAck, para);
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
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(Config, pConfigComboxbox);


	para.desc = tr("Service");
	para.tagname = "Service";
	//m_arrInfo.insert(SupportDelete, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeWorkstationTable *)parent())->setItemDelegateForColumn(Service, pReadOnlyDelegate);
}