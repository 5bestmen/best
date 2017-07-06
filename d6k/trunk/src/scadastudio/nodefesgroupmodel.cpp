#include "nodefesgroupmodel.h"
#include "config_data.h"
#include "scadastudio/define.h"
#include "delegate.h"
#include "nodefesgrouptable.h"
#include "scadastudio/icore.h"
#include "scadastudio/imodule.h"
#include "global.h"

#define FES_GROUP_NODE_COUNT 2

CNodeFesGroupModel::CNodeFesGroupModel(QObject *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pFesGroupData
	, Config::CNodeConfigData *pNodeConfig)
	: QAbstractTableModel(parent), m_pFesGroupData(nullptr), m_pCore(nullptr), m_pNodeConfig(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pFesGroupData);
	Q_ASSERT(pNodeConfig);

	if (!pCore)
	{
		return;
	}

	if (!pFesGroupData)
	{
		return;
	}

	if (!pNodeConfig)
	{
		return;
	}

	m_pFesGroupData = pFesGroupData;
	m_pCore = pCore;
	m_pNodeConfig = pNodeConfig;

	InitPara();
}

CNodeFesGroupModel::~CNodeFesGroupModel()
{

}

int CNodeFesGroupModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return (int)m_pFesGroupData->m_arrItem.size();
}

int CNodeFesGroupModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CNodeFesGroupModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == TagName)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pFesGroupData->m_arrItem[index.row()]->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pFesGroupData->m_arrItem[index.row()]->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pFesGroupData->m_arrItem[index.row()]->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strConfig;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pFesGroupData->m_arrItem[index.row()]->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pFesGroupData->m_arrItem[index.row()]->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pFesGroupData->m_arrItem[index.row()]->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pFesGroupData->m_arrItem[index.row()]->m_strConfig;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CNodeFesGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CNodeFesGroupModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	//int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == TagName)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					if (m_pNodeConfig->CheckTagNameIsExist(value.toString()))
					{
						return false;
					}

					//网络节点tagname修改，
					//如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
					auto pFesModule = m_pCore->GetModule("fes");
					Q_ASSERT(pFesModule);
					if (!pFesModule)
					{
						return false;
					}
					QString strDelete = "";
					pFesModule->ChangeTagNameNodify(value.toString(), QString(m_pFesGroupData->m_arrItem[index.row()]->m_szTagName), Module_ID::NODE_MODULE_FES);

					//hash修改
					if (m_pNodeConfig->ModifyItemHashMap(value.toString(), m_pFesGroupData->m_arrItem[index.row()]->m_szTagName))
					{
						memset(m_pFesGroupData->m_arrItem[index.row()]->m_szTagName, 0, sizeof(m_pFesGroupData->m_arrItem[index.row()]->m_szTagName));
						int nSize = value.toString().size();
						strncpy(m_pFesGroupData->m_arrItem[index.row()]->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
					}
				}
			}
			else if (index.column() == HostName)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_strHostName != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pFesGroupData->m_arrItem[index.row()]->m_strHostName = value.toString();
				}
			}
			else if (index.column() == OccNo)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_nOccNo != value.toInt())
				{
					m_pFesGroupData->m_arrItem[index.row()]->m_nOccNo = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_nBlockNo != value.toInt())
				{
					m_pFesGroupData->m_arrItem[index.row()]->m_nBlockNo = value.toInt();
				}
			}
			//else if (index.column() == NodeType)
			//{
			//	if (m_pFesGroupData->m_arrItem[index.row()]->m_nNodeType != value.toInt())
			//	{
			//		m_pFesGroupData->m_arrItem[index.row()]->m_nNodeType = value.toInt();
			//	}
			//}
			else if (index.column() == Network_IP_A)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_A != value.toString())
				{
					m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_A = value.toString();
				}
			
			}
			else if (index.column() == Network_IP_B)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_B != value.toString())
				{
					m_pFesGroupData->m_arrItem[index.row()]->m_strNetwork_IP_B = value.toString();
				}
			}
			else if (index.column() == IsTimeSource)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_bIsTimeSource != value.toBool())
				{
					m_pFesGroupData->m_arrItem[index.row()]->m_bIsTimeSource = value.toBool();
				}
			}
			else if (index.column() == Program)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_strProgram != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pFesGroupData->m_arrItem[index.row()]->m_strProgram = value.toString();
				}
			}
			else if (index.column() == Config)
			{
				if (m_pFesGroupData->m_arrItem[index.row()]->m_strConfig != value.toString())
				{
					bool bFlag = m_pNodeConfig->ChangeNodeFesTagNameNotifyFes(QString(""), QString(m_pFesGroupData->m_arrItem[index.row()]->m_szTagName));
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						return false;
					}
					
					m_pFesGroupData->m_arrItem[index.row()]->m_strConfig = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CNodeFesGroupModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CNodeFesGroupModel::InitPara()
{
	//m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = tr("TagName");
	para.tagname = "TagName";
	//m_arrInfo.insert(ID, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("HostName");
	para.tagname = "HostName";
	//m_arrInfo.insert(TagName, para);
	m_horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEditName = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(HostName, pLineEditName);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	//m_arrInfo.insert(Description, para);
	m_horizontal_header_list.append(para.desc);
	auto pDelegateRead = new CReadOnlyDelegate(this);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(OccNo, pDelegateRead);

	para.desc = tr("BlockNo");
	para.tagname = "BlockNo";
	//m_arrInfo.insert(OccNo, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(BlockNo, pDelegateRead);

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
	//((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(NodeType, pCombo);

	para.desc = tr("Network_IP_A");
	para.tagname = "Network_IP_A";
	//m_arrInfo.insert(Category, para);
	m_horizontal_header_list.append(para.desc);
	auto pIPDelegate = new CLineEditDelegate(this, IPADDRESS);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(Network_IP_A, pIPDelegate);

	para.desc = tr("Network_IP_B");
	para.tagname = "Network_IP_B";
	//m_arrInfo.insert(DelayAlarm, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(Network_IP_B, pIPDelegate);

	para.desc = tr("IsTimeSource");
	para.tagname = "IsTimeSource";
	//m_arrInfo.insert(Priority, para);
	m_horizontal_header_list.append(para.desc);
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(IsTimeSource, pCheck);

	para.desc = tr("Program");
	para.tagname = "Program";
	//m_arrInfo.insert(SupportAck, para);
	m_horizontal_header_list.append(para.desc);
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(Program, pLineEditName);

	para.desc = tr("Config");
	para.tagname = "Config";
	//m_arrInfo.insert(SupportDelete, para);
	auto pFesModule = m_pCore->GetModule("FES");
	std::vector<std::string> vecTmp;
	pFesModule->GetFesConfigName(vecTmp);
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
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(Config, pConfigComboxbox);

	m_horizontal_header_list.append(para.desc);
}

int CNodeFesGroupModel::GetSize()
{
	return (int)m_pFesGroupData->m_arrItem.size();
}

bool CNodeFesGroupModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	auto nCount = m_pFesGroupData->m_arrItem.size();

	if (nCount == FES_GROUP_NODE_COUNT)
	{
		return true;
	}

	beginInsertRows(parent, position, position + rows - 1);

	for (int i = position; i < position + rows; i++)
	{
		auto pNode = m_pNodeConfig->AddNodeItem(m_pFesGroupData);
		Q_ASSERT(pNode);
		if (!pNode)
		{
			endInsertRows();

			return false;
		}

		pNode->m_nNodeType = NODETYPE::FEP;

	}

	endInsertRows();

	return true;
}

bool CNodeFesGroupModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(parent, rows, rows);

	auto bFlag = true;

	if (position >= 0 && position < m_pFesGroupData->m_arrItem.size())
	{
		auto pNode = m_pFesGroupData->m_arrItem[position];
		Q_ASSERT(pNode);
		if (!pNode)
		{
			bFlag = false;
		}


		if (!m_pNodeConfig->DeleteHashItem(pNode->m_szTagName))
		{
			bFlag = false;
		}

		if (!m_pFesGroupData->DeleteItem(pNode))
		{
			bFlag = false;
		}
	}

	endRemoveRows();

	if (m_pFesGroupData->m_arrItem.size() == 0)
	{
		beginResetModel();
		endResetModel();
	}

	return true;
}

void CNodeFesGroupModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}