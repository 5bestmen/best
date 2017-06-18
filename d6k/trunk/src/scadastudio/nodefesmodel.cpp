#include "nodefesmodel.h"
#include "config_data.h"
#include "scadastudio/define.h"
#include "delegate.h"
#include "nodefesgrouptable.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/icore.h"
#include "scadastudio/imodule.h"
#include "global.h"

#define FES_GROUP_NODE_COUNT 2

CNodeFesModel::CNodeFesModel(QObject *parent, IMainModuleInterface *pCore, Config::CNode *pData
	, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig)
	: QAbstractTableModel(parent), m_pData(nullptr), m_pCore(nullptr), m_pItem(nullptr), m_pNodeConfig(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pData);
	Q_ASSERT(pItem);
	Q_ASSERT(pNodeConfig);

	if (!pData)
	{
		return;
	}

	if (!pCore)
	{
		return;
	}

	if (!pNodeConfig)
	{
		return;
	}



	m_pData = pData;
	m_pCore = pCore;
	m_pItem = pItem;
	m_pNodeConfig = pNodeConfig;

	InitPara();
}

CNodeFesModel::~CNodeFesModel()
{

}

int CNodeFesModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CNodeFesModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_horizontal_header_list.size() > 0)
		return m_horizontal_header_list.size();

	return 0;
}

QVariant CNodeFesModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		//Q_ASSERT(m_pChannel);
		if (index.column() == TagName)
		{
			return m_pData->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pData->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pData->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pData->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pData->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pData->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pData->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pData->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pData->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pData->m_strConfig;
		}

		return QVariant();
	}
	else if (role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			return m_pData->m_szTagName;
		}
		else if (index.column() == HostName)
		{
			return m_pData->m_strHostName;
		}
		else if (index.column() == OccNo)
		{
			return (uint)m_pData->m_nOccNo;
		}
		else if (index.column() == BlockNo)
		{
			return (uint)m_pData->m_nBlockNo;
		}
		//else if (index.column() == NodeType)
		//{
		//	return (uint)m_pData->m_nNodeType;
		//}
		else if (index.column() == Network_IP_A)
		{
			return m_pData->m_strNetwork_IP_A;
		}
		else if (index.column() == Network_IP_B)
		{
			return m_pData->m_strNetwork_IP_B;
		}
		else if (index.column() == IsTimeSource)
		{
			return m_pData->m_bIsTimeSource;
		}
		else if (index.column() == Program)
		{
			return m_pData->m_strProgram;
		}
		else if (index.column() == Config)
		{
			return m_pData->m_strConfig;
		}

		return QVariant();
	}

	return QVariant();
}

QVariant CNodeFesModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CNodeFesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	//int nRow = index.row();

	if (index.isValid() && role == Qt::EditRole)
	{
		if (role == Qt::EditRole)
		{
			if (index.column() == TagName)
			{
				if (m_pData->m_szTagName != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					if (m_pNodeConfig->CheckTagNameIsExist(value.toString()))
					{
						auto strError = QObject::tr("Node tagname %1 is exist!!!");
						m_pCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
						
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
					pFesModule->ChangeTagNameNodify(value.toString(), QString(m_pData->m_szTagName), Module_ID::NODE_MODULE_FES);


					//nodemodel里itemchanged处理
					m_pItem->setData(value, Qt::EditRole);
				}
			}
			else if (index.column() == HostName)
			{
				if (m_pData->m_strHostName != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pData->m_strHostName = value.toString();

					m_pItem->setData(value.toString(), TREE_TOOL_TIP_ROLE);
				}
			}
			else if (index.column() == OccNo)
			{
				if (m_pData->m_nOccNo != value.toInt())
				{
					m_pData->m_nOccNo = value.toInt();
				}
			}
			else if (index.column() == BlockNo)
			{
				if (m_pData->m_nBlockNo != value.toInt())
				{
					m_pData->m_nBlockNo = value.toInt();
				}
			}
			//else if (index.column() == NodeType)
			//{
			//	if (m_pData->m_nNodeType != value.toInt())
			//	{
			//		m_pData->m_nNodeType = value.toInt();
			//	}
			//}
			else if (index.column() == Network_IP_A)
			{
				if (m_pData->m_strNetwork_IP_A != value.toString())
				{
					m_pData->m_strNetwork_IP_A = value.toString();
				}
			
			}
			else if (index.column() == Network_IP_B)
			{
				if (m_pData->m_strNetwork_IP_B != value.toString())
				{
					m_pData->m_strNetwork_IP_B = value.toString();
				}
			}
			else if (index.column() == IsTimeSource)
			{
				if (m_pData->m_bIsTimeSource != value.toBool())
				{
					m_pData->m_bIsTimeSource = value.toBool();
				}
			}
			else if (index.column() == Program)
			{
				if (m_pData->m_strProgram != value.toString())
				{
					if (!CheckDescIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					m_pData->m_strProgram = value.toString();
				}
			}
			else if (index.column() == Config)
			{
				if (m_pData->m_strConfig != value.toString())
				{
					if (!CheckTagNameIsValid(value.toString(), NODE_CONFIG_DESC))
					{
						return false;
					}
					
					////网络节点tagname修改，
					////如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
					//auto pFesModule = m_pCore->GetModule("fes");
					//Q_ASSERT(pFesModule);
					//if (!pFesModule)
					//{
					//	return false;
					//}
					//pFesModule->ChangeTagNameNodify(value.toString(), QString(m_pData->m_szTagName), Module_ID::NODE_MODULE_FES);

					bool bFlag = m_pNodeConfig->ChangeNodeFesTagNameNotifyFes(QString(""), QString(m_pData->m_szTagName));
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						return false;
					}

					m_pData->m_strConfig = value.toString();
				}
			}

			emit dataChanged(index, index);

			return true;
		}

		return false;
	}

	return false;
}

Qt::ItemFlags CNodeFesModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CNodeFesModel::InitPara()
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
	((CNodeFesGroupTable *)parent())->setItemDelegateForColumn(TagName, pLineEditName);


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

int CNodeFesModel::GetSize()
{
	return 1;
}

bool CNodeFesModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED(position);
	Q_UNUSED(rows);
	Q_UNUSED(parent);

	return 1;
}

bool CNodeFesModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
	Q_UNUSED(position);
	Q_UNUSED(rows);
	Q_UNUSED(parent);
	
	return true;
}

void CNodeFesModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}