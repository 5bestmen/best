#include "scalenonlearmodel.h"

//CScaleNonlearModel::CScaleNonlearModel(QObject *parent, Config::CFesData *pFesData, CQuoteItem *pItem)
//	: QAbstractTableModel(parent), m_pFesData(NULL)
//{
//	m_pFesData = pFesData;
//}
//
//CScaleNonlearModel::~CScaleNonlearModel()
//{
//
//}
//
//int CScaleNonlearModel::rowCount(const QModelIndex &parent) const
//{
//	//if(vertical_header_list.size() > 0)
//	//	return vertical_header_list.size();
//
//	return /*m_pFesData->m_arrNonLinears.size()*/ 1;
//}
//
//int CScaleNonlearModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
//{
//	return 0;
//}
//
//QVariant CScaleNonlearModel::data(const QModelIndex &index, int role) const
//{
//	if (!index.isValid())
//		return QVariant();
//
//	return QVariant();
//}
//
//QVariant CScaleNonlearModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
//{
//	if (role == Qt::Horizontal)
//	{
//		if (orientation == Qt::Horizontal) // 水平表头  
//		{
//			if (horizontal_header_list.size() > section)
//				return horizontal_header_list[section];
//			else
//				return QVariant();
//		}
//		else
//		{
//			//if(vertical_header_list.size() > section)
//			//	return vertical_header_list[section]; // 垂直表头  
//			//else
//			//return QVariant();
//
//			return QString(" ");
//		}
//	}
//
//	return QVariant();
//}
//
//Qt::ItemFlags CScaleNonlearModel::flags(const QModelIndex &index) const
//{
//	if (!index.isValid())
//		return Qt::NoItemFlags;
//
//	Qt::ItemFlags flag = QAbstractItemModel::flags(index);
//
//	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
//	return flag;
//}
//
//bool CScaleNonlearModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
//{
//	//QFont font = value.value<QFont>();
//
//	//QSet<int32s> set;
//
//	//set.insert(index.row());
//
//	//if (index.isValid() && role == Qt::EditRole)
//	//{
//	//	if (index.column() == TagName)
//	//	{
//	//		if (m_pFesData->m_szTagName.compare(value.toString()) != 0)
//	//		{
//
//	//			m_pFesData->m_szTagName = value.toString();
//
//	//			m_pFesData->m_bModifiedFlag = true;
//
//	//			//TagName
//	//			//m_pItem->setData(value, TAGNAME);
//
//	//			//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
//	//		}
//
//
//	//	}
//	//	else if (index.column() == ID)
//	//	{
//	//		if (m_pFesData->m_nID != value.toInt())
//	//		{
//	//			m_pFesData->m_nID = value.toInt();
//
//	//			m_pFesData->m_bModifiedFlag = true;
//	//			//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
//	//		}
//	//	}
//	//	else if (index.column() == FesName)
//	//	{
//	//		if (m_pFesData->m_strDescription.compare(value.toString()) != 0)
//	//		{
//	//			m_pFesData->m_strDescription = value.toString();
//
//	//			m_pFesData->m_bModifiedFlag = true;
//
//	//			//通道名
//	//			//m_pItem->setData(value, Qt::EditRole);
//
//	//			//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
//	//		}
//	//	}
//	//	else if (index.column() == Extention)
//	//	{
//	//		if (m_pFesData->m_strExtention.compare(value.toString()) != 0)
//	//		{
//	//			m_pFesData->m_strExtention = value.toString();
//
//	//			m_pFesData->m_bModifiedFlag = true;
//
//	//			//emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
//	//		}
//	//	}
//	//	else if (index.column() == GroupName)
//	//	{
//	//		m_pFesData->m_strGroupName = value.toString();
//
//	//		m_pFesData->m_bModifiedFlag = true;
//	//	}
//
//	//	return true;
//	//}
//	///*else if (index.isValid() && role == Qt::UserRole)
//	//{
//
//	//}
//	//else if (index.isValid() && role == TREE_TOOL_TIP_ROLE)
//	//{
//
//	//}*/
//
//	return false;
//}
//
//void CScaleNonlearModel::sort(int column, Qt::SortOrder order /* = Qt::AscendingOrder */)
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