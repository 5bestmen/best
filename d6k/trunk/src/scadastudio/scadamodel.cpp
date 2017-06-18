
#include "scadamodel.h"
#include "scadastudio/quoteItem.h"
#include "scadatable.h"
#include "delegate.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "scadavirable_data.h"

CScadaModel::CScadaModel(QObject *parent, Config::CScadaData *pData, CQuoteItem *pItem, IMainModuleInterface *pCore
	, Config::CScadaVariableConfigData *pConfig)
	: QAbstractTableModel(parent), m_pCore(nullptr), m_pItem(nullptr), m_pScadaConfig(nullptr), m_pScadaData(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pData);
	Q_ASSERT(pItem);
	Q_ASSERT(pConfig);
	
	if (!pCore)
	{
		return;
	}

	if (!pData)
	{
		return;
	}

	if (!pItem)
	{
		return;
	}

	if (!pConfig)
	{
		return;
	}

	m_pScadaData = pData;
	m_pCore = pCore;
	m_pItem = pItem;
	m_pScadaConfig = pConfig;

	InitPara();
}

CScadaModel::~CScadaModel()
{

}

void CScadaModel::SetHorizontalHeaderList(QStringList list)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = list;
}

void CScadaModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CScadaModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CScadaModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	return 0;
}

QVariant CScadaModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	//if(0 == arr_row_list.count())
	//	return QVariant();

	//if(arr_row_list.count() < 1)
	//	return QVariant();

	if (role == Qt::TextAlignmentRole)
	{
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	}
	else if (role == Qt::DisplayRole)
	{
		Q_ASSERT(m_pScadaData);

		if (index.column() == TagName)
		{
			return m_pScadaData->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pScadaData->m_nID;
		}
		else if (index.column() == Name)
		{
			return m_pScadaData->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pScadaData->m_strExtention;
		}
		else if (index.column() == GroupName)
		{
			return m_pScadaData->m_strGroupName;
		}

		return 1;
	}
	else if (role == Qt::EditRole)
	{
		Q_ASSERT(m_pScadaData);

		if (index.column() == TagName)
		{
			return m_pScadaData->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pScadaData->m_nID;
		}
		else if (index.column() == Name)
		{
			return m_pScadaData->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pScadaData->m_strExtention;
		}
		else if (index.column() == GroupName)
		{
			return m_pScadaData->m_strGroupName;
		}
	}

	return QVariant();
}

QVariant CScadaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (horizontal_header_list.size() > section)
				return horizontal_header_list[section];
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
	//else if (role == Qt::TextColorRole)
	//{
	//	//if (orientation == Qt::Vertical)
	//	//{
	//	//	
	//	//	if (m_rowStatus[section] == DELEROW)
	//	//	{
	//	//		//删除
	//	//		return QVariant(QColor(Qt::blue));
	//	//	}
	//	//	else if(m_rowStatus[section] == MODIFIEDROW)
	//	//	{
	//	//		//正常
	//	//		return QVariant(QColor(Qt::red));
	//	//	}
	//	//}
	//}

	return QVariant();
}

Qt::ItemFlags CScadaModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CScadaModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	QFont font = value.value<QFont>();

	QSet<int32s> set;

	set.insert(index.row());

	if (index.isValid() && role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			if (m_pScadaData->m_szTagName != value.toString())
			{
				//auto bFlag = m_pScadaData->CheckTagNameIsExist(value.toString());
				//auto bFlagFess = m_pScadaData->CheckModifyFesTagNameExist(value.toString());
				//if (bFlag | bFlagFess)
				//{
				//	auto strError = QString("%1 %2 is exist").arg(tr(" fes tag name:")).arg(value.toString());

				//	m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
				//	return false;
				//}

				//TagName
				m_pItem->setData(value, Qt::EditRole);

				m_pScadaData->SetModifyFlag(true);

				

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}


		}
		else if (index.column() == ID)
		{
			if (m_pScadaData->m_nID != value.toInt())
			{
				m_pScadaData->m_nID = value.toInt();

				m_pScadaData->SetModifyFlag(true);
				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == Name)
		{
			if (m_pScadaData->m_strDescription.compare(value.toString()) != 0)
			{
				m_pScadaData->m_strDescription = value.toString();

				//m_pCore->SetModifyFlag(FES, true);

				//修改前置名
				m_pItem->setData(value, TREE_TOOL_TIP_ROLE);

				////通道名
				//m_pItem->setData(value, Qt::EditRole);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == Extention)
		{
			if (m_pScadaData->m_strExtention.compare(value.toString()) != 0)
			{
				m_pScadaData->m_strExtention = value.toString();

				m_pScadaData->SetModifyFlag(true);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == GroupName)
		{
			m_pScadaData->m_strGroupName = value.toString();

			m_pScadaData->SetModifyFlag(true);
		}

		return true;
	}
	/*else if (index.isValid() && role == Qt::UserRole)
	{

	}
	else if (index.isValid() && role == TREE_TOOL_TIP_ROLE)
	{

	}*/

	return false;
}

//void CFesDataModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder */)
//{
//	//g_column = column;
//
//	//g_sortOrder = order;
//
//	//if (arr_row_list.count() == 0)
//	//{
//	//	return;
//	//}
//
//	//qSort(arr_row_list.begin(), arr_row_list.end(), lessThan);
//
//	//for (int i = 0; i < arr_row_list.count(); i++)
//	//{
//	//	QString strTmp;
//
//	//	strTmp = arr_row_list[i][0];
//
//	//	strTmp = "test";
//	//}
//
//	//RefrushModel();
//}

void CScadaModel::InitPara()
{
	m_arrInfo.clear();

	CONFIG_PARA para;

	para.desc = QStringLiteral("ID");
	para.tagname = "ID";
	m_arrInfo.insert(ID, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_arrInfo.insert(TagName, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CScadaTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("Name");
	para.tagname = "Name";
	m_arrInfo.insert(Name, para);
	horizontal_header_list.append(para.desc);
	auto pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CScadaTable *)parent())->setItemDelegateForColumn(Name, pLineEditDesc);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	horizontal_header_list.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CScadaTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	//
	para.desc = tr("Extention");
	para.tagname = "Extention";
	m_arrInfo.insert(Extention, para);
	horizontal_header_list.append(para.desc);
	((CScadaTable *)parent())->setItemDelegateForColumn(Extention, pLineEditDesc);

	//para.desc = QStringLiteral("分组名称");
	//para.tagname = "GroupName";
	//m_arrInfo.insert(GroupName, para);
	//horizontal_header_list.append(para.desc);
	//((CFesTable *)parent())->setItemDelegateForColumn(GroupName, pLineEdit);
}

void CScadaModel::RefrushModel()
{
	beginResetModel();
	endResetModel();

	//emit updateCount(this->rowCount(QModelIndex()));
}