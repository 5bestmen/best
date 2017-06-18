/*! @file
<PRE>
********************************************************************************
模块名       :	排序model
文件名       :	tablemodel.cpp
文件实现功能 :  表格model
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/

#include <QFont>
//#include <qvector.h>
#include "channelmodel.h"
//#include "command.h"
#include "scadastudiodefine.h"
#include "channeltable.h"
#include "delegate.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "global.h"

extern int g_column;

extern Qt::SortOrder g_sortOrder;

extern bool lessThan(QVector<QString> &l, QVector<QString> &r);

//bool lessThan(QVector<QString> &l, QVector<QString> &r)
//{
//	bool ok, ok1;
//	double fltL = l[g_column].toDouble(&ok);
//
//	double fltR = r[g_column].toDouble(&ok1);
//
//	if(g_sortOrder == Qt::AscendingOrder)
//	{
//		if(ok & ok1)
//		{
//			return fltL < fltR;
//		}
//		else
//		{	
//			return l[g_column].localeAwareCompare(r[g_column]) < 0;
//		}
//	}
//	else
//	{
//		if(ok & ok1)
//		{
//			return fltL > fltR;
//		}
//		else
//		{	
//			return l[g_column].localeAwareCompare(r[g_column]) > 0;
//		}
//	}
//}

/*! \fn CTableModel::CTableModel(QObject *parent)
********************************************************************************************************* 
** \brief CTableModel::CTableModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CChannelModel::CChannelModel(QObject *parent, QStringList headerList, Config::CChannelData *pChannel, CQuoteItem *pItem, Config::CFesData *pfesData, IMainModuleInterface *pCore)
	: QAbstractTableModel(parent) /*,arr_row_list(NULL)*/, m_pCore(pCore)
{
	Q_ASSERT(pChannel);
	Q_ASSERT(pItem);
	Q_ASSERT(pfesData);
	Q_ASSERT(pCore);

	if (!pCore)
	{
		return;
	}

	if (!pfesData)
	{
		return;
	}

	if (!pItem)
	{
		return;
	}

	if (!pChannel)
	{
		return;
	}

	m_stack = new QUndoStack(this);

	horizontal_header_list = headerList;

	InitPara();

	//connect((CChannelTable *)parent, SIGNAL(clicked(const QModelIndex &)), this, SLOT(OnClickedView(const QModelIndex &)));

	Q_ASSERT(pChannel);

	m_pChannel = pChannel;

	m_pItem = pItem;

	m_pfesData = pfesData;

	m_pCore = pCore;
	//connect(this, SIGNAL(SendSelectedRows(QSet<int32s> &, int32s, CChannelModel *)), this, SLOT(SendSelectedRowsTest(QSet<int32s> &, int32s, CChannelModel *)));
}

/*! \fn CTableModel::~CTableModel()
********************************************************************************************************* 
** \brief CTableModel::~CTableModel 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CChannelModel::~CChannelModel()
{
	arr_row_list.clear();
	
	this->deleteLater();
}

/*! \fn void CTableModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
********************************************************************************************************* 
** \brief CTableModel::SetHorizontalHeaderList 
** \details 水平标题
** \param horizontalHeaderList 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::SetHorizontalHeaderList(QStringList list)
{
	if(!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}
	
	horizontal_header_list = list;
}

/*! \fn void CTableModel::SetVerticalHeaderList(QStringList verticalHeaderList)
********************************************************************************************************* 
** \brief CTableModel::SetVerticalHeaderList 
** \details 
** \param verticalHeaderList 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	//vertical_header_list = verticalHeaderList;
}

/*! \fn int CTableModel::rowCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CTableModel::rowCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
int CChannelModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	//if(vertical_header_list.size() > 0)
	//	return vertical_header_list.size();

	return 1;
}

/*! \fn int CTableModel::columnCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CTableModel::columnCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
int CChannelModel::columnCount(const QModelIndex &parent) const
{	
	Q_UNUSED(parent);

	if(horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	//if(0 == arr_row_list.count())
	//	return 0;
	//else if(arr_row_list.count() < 1)
	//	return 0;
	//else
	//	return arr_row_list.at(0).size();

	return 0;
}

#include <QColor>

/*! \fn QVariant CTableModel::data(const QModelIndex &index, int role) const
********************************************************************************************************* 
** \brief CTableModel::data 
** \details 
** \param index 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
QVariant CChannelModel::data(const QModelIndex &index, int role) const
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
		Q_ASSERT(m_pChannel);
		
		if (index.column() == OccNo)
		{
			return uint(m_pChannel->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_pChannel->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pChannel->m_nID;
		}
		else if (index.column() == ChannelName)
		{
			return m_pChannel->m_strDescription;
		}
		else if (index.column() == DriverName)
		{
			return m_pChannel->m_CommPara.DriverName;
		}
		else if (index.column() == DriverPara)
		{
			return m_pChannel->m_CommPara.m_strDriverParam;
		}
		else if (index.column() == ChannelParameter)
		{
			return m_pChannel->m_strChannelPara;
		}
		//else if (index.column() == DriverType)
		//{
		//	return m_pChannel->m_CommPara.DriveType;
		//}
		//else if (index.column() == Medium)
		//{
		//	return m_pChannel->m_CommPara.Medium;
		//}
		//else if (index.column() == DeviceAddress)
		//{
		//	QString strTmp = QString("%1").arg(m_pChannel->m_CommPara.Port);
		//	
		//	return m_pChannel->m_CommPara.IP + ":" + strTmp;
		//}
		//else if (index.column() == COM)
		//{
		//	return m_pChannel->m_CommPara.Com;
		//}
		//else if (index.column() == Baud)
		//{
		//	return m_pChannel->m_CommPara.Baud;
		//}
		else if (index.column() == Extention)
		{
			return m_pChannel->m_strExtention;
		}
		else if (index.column() == GroupName)
		{
			return m_pChannel->m_strGroupName;
		}

		return 1;
	}
	else if(role == Qt::EditRole)
	{	
		Q_ASSERT(m_pChannel);

		if (index.column() == OccNo)
		{
			return uint(m_pChannel->GetOccNo());
		}
		else if (index.column() == TagName)
		{
			return m_pChannel->m_szTagName;
		}
		else if (index.column() == ID)
		{
			return m_pChannel->m_nID;
		}
		else if (index.column() == ChannelName)
		{
			return m_pChannel->m_strDescription;
		}
		else if (index.column() == DriverName)
		{
			return m_pChannel->m_CommPara.DriverName;
		}
		else if (index.column() == ChannelParameter)
		{
			return m_pChannel->m_strChannelPara;
		}
		else if (index.column() == DriverPara)
		{
			return m_pChannel->m_CommPara.m_strDriverParam;
		}
		//else if (index.column() == DriverType)
		//{
		//	return m_pChannel->m_CommPara.DriveType;
		//}
		//else if (index.column() == Medium)
		//{
		//	return m_pChannel->m_CommPara.Medium;
		//}
		//else if (index.column() == DeviceAddress)
		//{
		//	QString strTmp = QString("%1").arg(m_pChannel->m_CommPara.Port);

		//	return m_pChannel->m_CommPara.IP + ":" + strTmp;
		//}
		//else if (index.column() == COM)
		//{
		//	return m_pChannel->m_CommPara.Com;
		//}
		//else if (index.column() == Baud)
		//{
		//	return m_pChannel->m_CommPara.Baud;
		//}
		else if (index.column() == Extention)
		{
			return m_pChannel->m_strExtention;
		}
		else if (index.column() == GroupName)
		{
			return m_pChannel->m_strGroupName;
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

/*! \fn QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
********************************************************************************************************* 
** \brief CTableModel::headerData 
** \details 
** \param section 
** \param orientation 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
QVariant CChannelModel::headerData(int section, Qt::Orientation orientation, int role) const
{	
	if(role==Qt::DisplayRole)  
	{  
		if(orientation==Qt::Horizontal) // 水平表头  
		{  
			if(horizontal_header_list.size() > section)
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

/*! \fn Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
********************************************************************************************************* 
** \brief CTableModel::flags 
** \details 
** \param index 
** \return Qt::ItemFlags 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
Qt::ItemFlags CChannelModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);  

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;  
}

/*! \fn void CTableModel::SetModalDatas(QList< QVector<QString> > &rowlist)
********************************************************************************************************* 
** \brief CTableModel::SetModalDatas 
** \details 
** \param rowlist 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::SetModalDatas(QList< QVector<QString> > &rowlist)
{
	arr_row_list = rowlist;
}

/*! \fn void CTableModel::RefrushModel()
********************************************************************************************************* 
** \brief CTableModel::RefrushModel 
** \details 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::RefrushModel()
{
	beginResetModel();
	endResetModel();

	//emit updateCount(this->rowCount(QModelIndex()));
}

/*! \fn bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
********************************************************************************************************* 
** \brief CTableModel::setData 
** \details model设置值
** \param index 项
** \param value 值
** \param role 角色
** \return bool 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
bool CChannelModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{	
	QFont font = value.value<QFont>();

	QSet<int32s> set;

	set.insert(index.row());

	if (index.isValid() && role == Qt::EditRole) 
	{	
		//if(index.column() == 0)
		//{
		//	if (m_pChannel->m_nOccNo != value.toInt())
		//	{
		//		QSet<int32s> set;

		//		set.insert(index.row());
		//		
		//		//值改变,属性栏跟着变
		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//		
		//		m_pChannel->m_nOccNo = value.toInt();
		//	}
		//	


		//	//m_stack->push(new ChangeValueCommand(index, value, this));

		//	//emit dataChanged(index, index);
		//}
		if (index.column() == TagName)
		{
			if (m_pChannel->m_szTagName != value.toString())
			{

				auto bFlag = m_pfesData->CheckTagNameIsExist(value.toString());
				if (bFlag)
				{	
					auto strError = QString("%1 %2 is exist").arg(tr(" channel tag name:")).arg(value.toString());
					m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
					
					return true;
				}
				
				if (!CheckTagNameIsValid(value.toString(), FES_DESC))
				{
					return nullptr;
				}


				//TagName
				m_pItem->setData(value, Qt::EditRole);

				memset(m_pChannel->m_szTagName, 0, sizeof(m_pChannel->m_szTagName));
				int nSize = value.toString().size();
				strncpy(m_pChannel->m_szTagName, value.toString().toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

				emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
			

		}
		else if (index.column() == ID)
		{
			if (m_pChannel->m_nID != value.toInt())
			{
				m_pChannel->m_nID = value.toInt();

				emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ChannelName)
		{
			if (m_pChannel->m_strDescription.compare(value.toString()) != 0)
			{
				auto strDesc = value.toString();

				if (!CheckDescIsValid(strDesc, FES_DESC))
				{
					return false;
				}
				else
				{
					m_pChannel->m_strDescription = strDesc;
				}

				//修改树形分支通道名
				m_pItem->setData(value, TREE_TOOL_TIP_ROLE);

				emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == DriverName)
		{
			if (m_pChannel->m_CommPara.DriverName.compare(value.toString()) != 0)
			{
				m_pChannel->m_CommPara.DriverName = value.toString();
			
				emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == DriverPara)
		{
			if (m_pChannel->m_CommPara.m_strDriverParam != value.toString())
			{
				m_pChannel->m_CommPara.m_strDriverParam = value.toString();
			
				emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == ChannelParameter)
		{
			if (m_pChannel->m_strChannelPara.compare(value.toString()) != 0)
			{
				m_pChannel->m_strChannelPara = value.toString();
			}
		}
		//else if (index.column() == DriverType)
		//{
		//	if (m_pChannel->m_CommPara.DriveType != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.DriveType = value.toInt();
		//	
		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == Medium)
		//{
		//	if (m_pChannel->m_CommPara.Medium != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.Medium = value.toInt();
		//	
		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == DeviceAddress)
		//{
		//	QString strTmp = value.toString();

		//	QStringList list = strTmp.split(":");

		//	if (list.count() == 2)
		//	{
		//		if (m_pChannel->m_CommPara.IP.compare(list[0]) != 0 ||
		//			m_pChannel->m_CommPara.Port != list[1].toInt())
		//		{
		//			m_pChannel->m_CommPara.IP = list[0];

		//			m_pChannel->m_CommPara.Port = list[1].toInt();

		//			emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//		}
		//	}
		//	else
		//	{
		//		//输出错误
		//	}

		//	//QString strTmp = QString("%1").arg(m_pChannel->m_CommPara.Port);

		//	//m_pChannel->m_CommPara.IP + ":" + strTmp;
		//}
		//else if (index.column() == COM)
		//{
		//	if (m_pChannel->m_CommPara.Com.compare(value.toString()) != 0)
		//	{
		//		m_pChannel->m_CommPara.Com = value.toString();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		//else if (index.column() == Baud)
		//{
		//	if (m_pChannel->m_CommPara.Baud != value.toInt())
		//	{
		//		m_pChannel->m_CommPara.Baud = value.toInt();

		//		emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
		//	}
		//}
		else if (index.column() == Extention)
		{
			if (m_pChannel->m_strExtention.compare(value.toString()) != 0)
			{
				auto strDesc = value.toString();
				
				if (!CheckDescIsValid(strDesc, FES_DESC))
				{
					return false;
				}
				else
				{
					m_pChannel->m_strExtention = value.toString();
				}
				


				emit SendChangeChannelModelRows(set, FES_CHANNEL_CHILD_ITEM, this);
			}
		}
		else if (index.column() == GroupName)
		{
			m_pChannel->m_strGroupName = value.toString();
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

/*! \fn void CTableModel::sort ( int column, Qt::SortOrder order)
********************************************************************************************************* 
** \brief CTableModel::sort 
** \details 排序
** \param column 
** \param order 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder */)
{
	g_column = column;

	g_sortOrder = order;

	if(arr_row_list.count() == 0)
	{
		return;
	}

	qSort(arr_row_list.begin(), arr_row_list.end(), lessThan);

	for(int i = 0; i < arr_row_list.count(); i++)
	{
		QString strTmp;

		strTmp = arr_row_list[i][0];

		strTmp = "test";
	}

	RefrushModel();
}

/*! \fn QList<QVector<QString>> &CTableModel::GetDataList()
********************************************************************************************************* 
** \brief CTableModel::GetDataList 
** \details 获得数据源
** \return QList<QVector<QString>> & 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
QList<QVector<QString>> &CChannelModel::GetDataList()
{
	return arr_row_list;
}

/*!
    Inserts a number of rows into the model at the specified position.
*/

/*! \fn bool CTableModel::InsertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CTableModel::InsertRows 
** \details 插入一行
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
bool CChannelModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
    int columns = columnCount();

    beginInsertRows(parent, position, position + rows - 1);

    for (int row = 0; row < rows; ++row) 
	{
        QVector<QString> items;
        
		for (int column = 0; column < columns; ++column)
		{
			items.append("");

			//项状态
			//m_itemStatus[row][column] = NORMALITEM;
		}  
        
		arr_row_list.insert(position + 1, items);

		//m_rowStatus.insert(position + 1, NORMALROW);
    }

    endInsertRows();

    return true;
}

/*!
    Removes a number of rows from the model at the specified position.
*/

/*! \fn bool CTableModel::RemoveRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CTableModel::RemoveRows 
** \details 删除
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
bool CChannelModel::RemoveRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(parent, position, position + rows - 1);

    for (int row = 0; row < rows; ++row) 
	{
        arr_row_list.removeAt(position);
    }

    endRemoveRows();
    
	return true;
}

/*! \fn void CTableModel::emitDataChanged(const QModelIndex &index)
********************************************************************************************************* 
** \brief CTableModel::emitDataChanged 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::emitDataChanged(const QModelIndex &index)
{ 
	emit dataChanged(index, index);
}

/*! \fn QUndoStack *CTableModel::undoStackTest()
********************************************************************************************************* 
** \brief CTableModel::undoStackTest 
** \details 
** \return QUndoStack * 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
QUndoStack *CChannelModel::undoStackTest()
{ 
	return m_stack;
}

/*! \fn void CTableModel::SaveDataModel(QString tableName)
********************************************************************************************************* 
** \brief CTableModel::SaveDataModel 
** \details 
** \param tableName 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::SaveDataModel(QString tableName)
{
	//int nColumns = columnCount();
	
	for(int i = 0; i < arr_row_list.count(); i++)
	{
		QString strSql = "";
		
		QString strTmp = horizontal_header_list[i];

		if(m_desc2RealField.contains(strTmp))
		{
		
		}
	}
}

/*! \fn void CTableModel::AddRowModel(QVector<QString> &rowList, int rowType)
********************************************************************************************************* 
** \brief CTableModel::AddRowModel 
** \details 
** \param rowList 
** \param rowType 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::AddRowModel(QVector<QString> &rowList, int rowType)
{
	arr_row_list.append(rowList);

	m_rowStatus.append(rowType);
}

/*! \fn void CTableModel::GetColumInfo(QMap<QString, QString> &desc2RealField)
********************************************************************************************************* 
** \brief CTableModel::GetColumInfo 
** \details 
** \param  
** \param desc2RealField 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CChannelModel::GetColumInfo(QMap<QString, QString> &desc2RealField)
{
	m_desc2RealField.clear();

	m_desc2RealField = desc2RealField;
}

/*! \fn void CTableModel::RemoveRowModel(QVector<QString> &rowList, int rowType)
********************************************************************************************************* 
** \brief CTableModel::RemoveRowModel 
** \details 行删除标志
** \param rowList 
** \param rowType 
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CChannelModel::RemoveRowModel(QVector<QString> &rowList, int rowType)
{
	Q_UNUSED(rowList);
	Q_UNUSED(rowType);
	
	//m_rowStatus[rowType] = DELEROW;
}

/*! \fn void CChannelModel::InitPara()
********************************************************************************************************* 
** \brief CChannelModel::InitPara 
** \details 初始化参数
** \return void 
** \author gw
** \date 2016年1月19日 
** \note 
********************************************************************************************************/

#include <string.h>

#include <qdialog.h>

void CChannelModel::InitPara()
{
	m_arrInfo.clear();

	CONFIG_PARA para;
	
	para.desc = tr("ID");
	para.tagname = "ID";
	m_arrInfo.insert(ID, para);
	horizontal_header_list.append(para.desc);

	para.desc = tr("OccNo");
	para.tagname = "OccNo";
	m_arrInfo.insert(OccNo, para);
	CReadOnlyDelegate *pReadOnlyDelegate = new CReadOnlyDelegate(this);
	((CChannelTable *)parent())->setItemDelegateForColumn(OccNo, pReadOnlyDelegate);


	horizontal_header_list.append(para.desc);

	para.desc = tr("TagName");
	para.tagname = "TagName";
	m_arrInfo.insert(TagName, para);
	horizontal_header_list.append(para.desc);
	CLineEditDelegate *pLineEdit = new CLineEditDelegate(this, TAGNAMELINE, MAX_TAGNAME_LEN_SCADASTUDIO);
	((CChannelTable *)parent())->setItemDelegateForColumn(TagName, pLineEdit);



	para.desc = tr("ChannelName");
	para.tagname = para.desc;
	m_arrInfo.insert(ChannelName, para);
	horizontal_header_list.append(para.desc);
	auto pLineEditDesc = new CLineEditDelegate(this, LINESTRING, MAX_NAME_LENGTH_SCADASTUDIO);
	((CChannelTable *)parent())->setItemDelegateForColumn(ChannelName, pLineEditDesc);

	para.desc = tr("DriverName");
	para.tagname = "DriverName";
	m_arrInfo.insert(DriverName, para);
	horizontal_header_list.append(para.desc);

	QVector<IDDESC> arrVec;
	m_arrDriverName.clear();

	IDDESC tmp;
	tmp.desc = "modbus";
	tmp.ID = "modbus";
	arrVec.append(tmp);
	m_arrDriverName.append(tmp);
	tmp.desc = "101";
	tmp.ID = "101";
	arrVec.append(tmp);
	m_arrDriverName.append(tmp);
	tmp.desc = "104";
	tmp.ID = "104";
	arrVec.append(tmp);
	m_arrDriverName.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, m_arrDriverName);
	((CChannelTable *)parent())->setItemDelegateForColumn(DriverName, pCombo);

	//驱动参数
	para.desc = tr("DriverPara");
	para.tagname = "DriverPara";
	m_arrInfo.insert(DriverPara, para);
	horizontal_header_list.append(para.desc);

	QVector<IDDESC> arrVecModebus, arrVec101, arrVec104;
	tmp.desc = "modbus";
	tmp.ID = "modbus";
	//arrVecModebus.append(tmp);
	m_arrDriverParaModbus.append(tmp);
	tmp.desc = "101";
	tmp.ID = "101";
	//arrVec101.append(tmp);
	m_arrDriverPara101.append(tmp);
	tmp.desc = "104";
	tmp.ID = "104";
	//arrVec104.append(tmp);
	m_arrDriverPara104.append(tmp);
	CDriveParaComboBoxDelegate *pComboDrivePara = new CDriveParaComboBoxDelegate(this, m_arrDriverPara101, m_arrDriverParaModbus, m_arrDriverPara104);
	((CChannelTable *)parent())->setItemDelegateForColumn(DriverPara, pComboDrivePara);


	para.desc = tr("ChannelParameter");
	para.tagname = para.desc;
	m_arrInfo.insert(ChannelParameter, para);
	horizontal_header_list.append(para.desc);
	CButtonEditDelegate *pButtonDelegate = new CButtonEditDelegate(this);
	((CChannelTable *)parent())->setItemDelegateForColumn(ChannelParameter, pButtonDelegate);

	//
	//para.desc = QStringLiteral("驱动类型");
	//para.tagname = "DriverType";
	//m_arrInfo.insert(6, para);
	//horizontal_header_list.append(para.desc);
	//
	//QVector<IDDESC> arrVecDriverType;
	//tmp.desc = QStringLiteral("进程外驱动");
	//tmp.ID = QString("%1").arg(PROCEEDOUT);
	//arrVecDriverType.append(tmp);

	//tmp.desc = QStringLiteral("进程内驱动");
	//tmp.ID = QString("%1").arg(PROCEEDIN);
	//arrVecDriverType.append(tmp);
	//pCombo = new CComboBoxDelegate(this, arrVecDriverType);
	//((CChannelTable *)parent())->setItemDelegateForColumn(DriverType, pCombo);


	////
	//QVector<IDDESC> arrVecCommMode;
	//para.desc = QStringLiteral("通讯方式");
	//para.tagname = "Medium";
	//m_arrInfo.insert(Medium, para);
	//horizontal_header_list.append(para.desc);
	//
	//tmp.desc = QStringLiteral("以太网");
	//tmp.ID = "0";
	//m_arrMedium.append(tmp);

	//tmp.desc = QStringLiteral("串口");
	//tmp.ID = "1";
	//m_arrMedium.append(tmp);
	//pCombo = new CComboBoxDelegate(this, m_arrMedium);
	//((CChannelTable *)parent())->setItemDelegateForColumn(Medium, pCombo);


	////
	//para.desc = QStringLiteral("装置地址");
	//para.tagname = "DeviceAddress";
	//m_arrInfo.insert(DeviceAddress, para);
	//horizontal_header_list.append(para.desc);
	//pLineEdit = new CLineEditDelegate(this, DEVICEADDRESS);
	//((CChannelTable *)parent())->setItemDelegateForColumn(DeviceAddress, pLineEdit);


	////
	//QVector<IDDESC> arrVecCom;
	//para.desc = QStringLiteral("串口");
	//para.tagname = "COM";
	//m_arrInfo.insert(COM, para);
	//horizontal_header_list.append(para.desc);
	//for (int32s i = 0; i < 36; i++)
	//{
	//	QString strTmp = QString("COM%1").arg(i);

	//	IDDESC tmp;

	//	tmp.desc = strTmp;
	//	tmp.ID = strTmp;

	//	m_arrCOM.append(tmp);
	//}
	//pCombo = new CComboBoxDelegate(this, m_arrCOM);
	//((CChannelTable *)parent())->setItemDelegateForColumn(COM, pCombo);

	////
	//QVector<IDDESC> arrVecBaud;
	//para.desc = QStringLiteral("波特率");
	//para.tagname = "Baud";
	//m_arrInfo.insert(Baud, para);
	//horizontal_header_list.append(para.desc);

	//tmp.desc = "300";
	//tmp.ID = "300";
	//m_arrBaud.append(tmp);

	//tmp.desc = "600";
	//tmp.ID = "600";
	//m_arrBaud.append(tmp);

	//tmp.desc = "1200";
	//tmp.ID = "1200";
	//m_arrBaud.append(tmp);

	//tmp.desc = "2400";
	//tmp.ID = "2400";
	//m_arrBaud.append(tmp);

	//tmp.desc = "2400";
	//tmp.ID = "2400";
	//m_arrBaud.append(tmp);

	//tmp.desc = "4800";
	//tmp.ID = "4800";
	//m_arrBaud.append(tmp);

	//tmp.desc = "9600";
	//tmp.ID = "9600";
	//m_arrBaud.append(tmp);

	//tmp.desc = "19200";
	//tmp.ID = "19200";
	//m_arrBaud.append(tmp);

	//pCombo = new CComboBoxDelegate(this, m_arrBaud);
	//((CChannelTable *)parent())->setItemDelegateForColumn(Baud, pCombo);


	//
	para.desc = tr("Extention");
	para.tagname = "Extention";
	m_arrInfo.insert(Extention, para);
	horizontal_header_list.append(para.desc);
	((CChannelTable *)parent())->setItemDelegateForColumn(Extention, pLineEditDesc);

	//para.desc = tr("GroupName");
	//para.tagname = "GroupName";
	//m_arrInfo.insert(GroupName, para);
	//horizontal_header_list.append(para.desc);
}

void CChannelModel::OnClickedView(const QModelIndex &index)
{
	Q_UNUSED(index);
}

QMap<int32s, CONFIG_PARA> & CChannelModel::GetColumnInfo()
{
	return m_arrInfo;
}

Config::CChannelData *CChannelModel::GetChannelData()
{
	Q_ASSERT(m_pChannel);

	return m_pChannel;
}

QVector<IDDESC>& CChannelModel::GetDriverName()
{
	return m_arrDriverName;
}

QVector<IDDESC>& CChannelModel::GetDriverPara101()
{
	return m_arrDriverPara101;
}

QVector<IDDESC>& CChannelModel::GetDriverPara104()
{
	return m_arrDriverPara104;
}

QVector<IDDESC>& CChannelModel::GetDriverParaModbus()
{
	return m_arrDriverParaModbus;
}

QVector<IDDESC> &CChannelModel::GetMedium()
{
	return m_arrMedium;
}

QVector<IDDESC> &CChannelModel::GetCOM()
{
	return m_arrCOM;
}

QVector<IDDESC> &CChannelModel::GetBaud()
{
	return m_arrBaud;
}

CQuoteItem *CChannelModel::GetCurrentTreeItem()
{
	Q_ASSERT(m_pItem);
	
	return m_pItem;
}