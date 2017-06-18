
#include "fesdatamodel.h"
#include "scadastudio/quoteItem.h"
#include "festable.h"
#include "delegate.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "config_data.h"
#include "global.h"
#include "scadastudio/imodule.h"

CFesDataModel::CFesDataModel(QObject *parent, Config::CFesData *pFesData, CQuoteItem *pItem, IMainModuleInterface *pCore
	, Config::CFesConfigData *pFesConfig)
	: QAbstractTableModel(parent), m_pCore(nullptr), m_pItem(nullptr), m_pFesConfig(nullptr)
{
	Q_ASSERT(pCore);
	Q_ASSERT(pFesData);
	Q_ASSERT(pItem);
	Q_ASSERT(pFesConfig);
	
	if (!pCore)
	{
		return;
	}

	if (!pFesData)
	{
		return;
	}

	if (!pItem)
	{
		return;
	}

	if (!pFesConfig)
	{
		return;
	}

	m_pFesData = pFesData;
	
	InitPara();

	m_pCore = pCore;

	m_pItem = pItem;

	m_pFesConfig = pFesConfig;
}

CFesDataModel::~CFesDataModel()
{

}

void CFesDataModel::SetHorizontalHeaderList(QStringList list)
{
	if (!m_lstHorizontalHeader.isEmpty())
	{
		m_lstHorizontalHeader.clear();
	}

	m_lstHorizontalHeader = list;
}

void CFesDataModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

int CFesDataModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

int CFesDataModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	
	if (m_lstHorizontalHeader.size() > 0)
		return m_lstHorizontalHeader.size();

	return 0;
}

QVariant CFesDataModel::data(const QModelIndex &index, int role) const
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
		Q_ASSERT(m_pFesData);

		if (index.column() == TagName)
		{
			return m_pFesData->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pFesData->m_nID;
		}
		else if (index.column() == FesName)
		{
			return m_pFesData->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pFesData->m_strExtention;
		}
		else if (index.column() == GroupName)
		{
			return m_pFesData->m_strGroupName;
		}

		return 1;
	}
	else if (role == Qt::EditRole)
	{
		Q_ASSERT(m_pFesData);

		if (index.column() == TagName)
		{
			return m_pFesData->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pFesData->m_nID;
		}
		else if (index.column() == FesName)
		{
			return m_pFesData->m_strDescription;
		}
		else if (index.column() == Extention)
		{
			return m_pFesData->m_strExtention;
		}
		else if (index.column() == GroupName)
		{
			return m_pFesData->m_strGroupName;
		}
	}
	//else if (role == Qt::TextColorRole && index.column() == 0)
	//{
	//	////        double d = value.toDouble();
	//	////        if(d > 0)
	//	//
	//	//if(m_rowStatus[index.row()] == MODIFIEDROW)
	//	//{
	//	//	//修改颜色
	//	//	return QVariant::fromValue(MODIFYROWCOLOR);
	//	//}
	//	//else if(m_rowStatus[index.row()] == DELEROW)
	//	//{
	//	//	//删除颜色
	//	//	return QVariant::fromValue(DELETEROWCOLOR);
	//	//}
	//	//else if(m_rowStatus[index.row()] == INSERTROW)
	//	//{
	//	//	//插入颜色
	//	//	return QVariant::fromValue(DELETEROWCOLOR);
	//	//}
	//	//else if(m_rowStatus[index.row()] == NORMALROW)
	//	//{
	//	//	return QVariant();
	//	//}

	//	return QVariant();
	//}

	return QVariant();
}

QVariant CFesDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_lstHorizontalHeader.size() > section)
				return m_lstHorizontalHeader[section];
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

Qt::ItemFlags CFesDataModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

bool CFesDataModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	QFont font = value.value<QFont>();

	QSet<int32s> set;

	set.insert(index.row());

	if (index.isValid() && role == Qt::EditRole)
	{
		if (index.column() == TagName)
		{
			if (m_pFesData->m_szTagName != value.toString())
			{
				if (!CheckTagNameIsValid(value.toString(), FES_DESC))
				{
					return false;
				}

				QString strFesTagName = value.toString();
				QString strLastTagName = m_pFesData->m_szTagName;

				//int nCount = value.toString().size();

				auto bFlag = m_pFesData->CheckTagNameIsExist(value.toString());
				auto bFlagFess = m_pFesConfig->CheckModifyFesTagNameExist(value.toString());
				if (bFlag | bFlagFess)
				{
					auto strError = QString("%1 %2 is exist").arg(tr(" fes tag name:")).arg(value.toString());

					m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
					return false;
				}

				//TagName
				m_pItem->setData(value, Qt::EditRole);

				//sprintf_s(m_pFesData->m_szTagName, TAGNAME_SIZE + 1, "%s", value.toString().toStdString().c_str());
				memset(m_pFesData->m_szTagName, 0, sizeof(m_pFesData->m_szTagName));
				auto nSize = value.toString().size();
				strncpy(m_pFesData->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

				//m_pCore->SetModifyFlag(FES, true);

				m_pFesData->SetModifyFlag(true);


				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}


		}
		else if (index.column() == ID)
		{
			if (m_pFesData->m_nID != value.toInt())
			{
				m_pFesData->m_nID = value.toInt();

				m_pFesData->SetModifyFlag(true);
				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == FesName)
		{
			if (m_pFesData->m_strDescription.compare(value.toString()) != 0)
			{
				auto strTmp = value.toString();
				int nSize = strTmp.toLocal8Bit().length();
				//中文一个字符占两个字节
				if (!CheckDescIsValid(strTmp, FES_DESC))
				{
					//m_pFesData->m_strDescription = strTmp.toLocal8Bit().data();
					return false;
				}
				else
				{
					m_pFesData->m_strDescription = strTmp;
				}
				
				

				////通道名
				//m_pItem->setData(value, Qt::EditRole);

				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
				
			}
		}
		else if (index.column() == Extention)
		{
			if (m_pFesData->m_strExtention.compare(value.toString()) != 0)
			{
				auto strTmp = value.toString();
				//中文一个字符占两个字节
				if (!CheckDescIsValid(strTmp, FES_DESC))
				{
					//m_pFesData->m_strDescription = strTmp.toLocal8Bit().data();
					return false;
				}
				else
				{
					m_pFesData->m_strExtention = value.toString();

					m_pFesData->SetModifyFlag(true);
				}



				//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == GroupName)
		{
			m_pFesData->m_strGroupName = value.toString();

			m_pFesData->SetModifyFlag(true);
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

void CFesDataModel::InitPara()
{
	m_mapInfo.clear();

	CONFIG_PARA para;

	para.desc = QStringLiteral("ID");
	para.tagname = "ID";
	m_mapInfo.insert(ID, para);
	m_lstHorizontalHeader.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_mapInfo.insert(TagName, para);
	m_lstHorizontalHeader.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CFesTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);

	para.desc = tr("FesName");
	para.tagname = "FesName";
	m_mapInfo.insert(FesName, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CFesTable *)parent())->setItemDelegateForColumn(FesName, pLineEditDesc);


	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_mapInfo.insert(OccNo, para);
	m_lstHorizontalHeader.append(para.desc);
	auto pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CFesTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);

	//
	para.desc = tr("Extention");
	para.tagname = "Extention";
	m_mapInfo.insert(Extention, para);
	m_lstHorizontalHeader.append(para.desc);
	((CFesTable *)parent())->setItemDelegateForColumn(Extention, pLineEditDesc);

	//para.desc = QStringLiteral("分组名称");
	//para.tagname = "GroupName";
	//m_arrInfo.insert(GroupName, para);
	//horizontal_header_list.append(para.desc);
	//((CFesTable *)parent())->setItemDelegateForColumn(GroupName, pLineEdit);
}

void CFesDataModel::RefrushModel()
{
	beginResetModel();
	endResetModel();

	//emit updateCount(this->rowCount(QModelIndex()));
}

///*! \fn bool CFesDataModel::IsModifyFesTagNameExist(QString &strTagName)
//********************************************************************************************************* 
//** \brief CFesDataModel::IsModifyFesTagNameExist 
//** \details 
//** \param strTagName 
//** \return bool 
//** \author gw
//** \date 2016年7月29日 
//** \note 
//********************************************************************************************************/
//bool CFesDataModel::IsModifyFesTagNameExist(QString &strTagName)
//{
//	//for (auto const &tmp : m_fesDataVec)
//	//{
//	//	if (tmp->m_szTagName == strTagName)
//	//	{
//	//		return true;
//	//	}
//	//}
//
//	return false;
//}