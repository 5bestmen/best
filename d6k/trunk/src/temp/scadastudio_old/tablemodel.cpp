/*! @file
<PRE>
********************************************************************************
ģ����       :	����model
�ļ���       :	tablemodel.cpp
�ļ�ʵ�ֹ��� :  ���model
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/

#include "tablemodel.h"
//#include "command.h"

int g_column = -1;

Qt::SortOrder g_sortOrder;

/*! \fn bool lessThan(QVector<QString> &l, QVector<QString> &r)
********************************************************************************************************* 
** \brief lessThan 
** \details 
** \param l 
** \param r 
** \return bool 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
bool lessThan(QVector<QString> &l, QVector<QString> &r)
{
	bool ok, ok1;
	double fltL = l[g_column].toDouble(&ok);

	double fltR = r[g_column].toDouble(&ok1);

	if(g_sortOrder == Qt::AscendingOrder)
	{
		if(ok & ok1)
		{
			return fltL < fltR;
		}
		else
		{	
			return l[g_column].localeAwareCompare(r[g_column]) < 0;
		}
	}
	else
	{
		if(ok & ok1)
		{
			return fltL > fltR;
		}
		else
		{	
			return l[g_column].localeAwareCompare(r[g_column]) > 0;
		}
	}
}

/*! \fn CTableModel::CTableModel(QObject *parent)
********************************************************************************************************* 
** \brief CTableModel::CTableModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
CTableModel::CTableModel(QObject *parent, QStringList headerList)
	: QAbstractTableModel(parent) /*,arr_row_list(NULL)*/
{
	m_stack = new QUndoStack(this);

	horizontal_header_list = headerList;
}

/*! \fn CTableModel::~CTableModel()
********************************************************************************************************* 
** \brief CTableModel::~CTableModel 
** \details 
** \return  
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
CTableModel::~CTableModel()
{
	arr_row_list.clear();
	
	this->deleteLater();
}

/*! \fn void CTableModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
********************************************************************************************************* 
** \brief CTableModel::SetHorizontalHeaderList 
** \details ˮƽ����
** \param horizontalHeaderList 
** \return void 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::SetHorizontalHeaderList(QStringList list)
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::SetVerticalHeaderList(QStringList verticalHeaderList)
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
int CTableModel::rowCount(const QModelIndex &parent) const
{
	if(vertical_header_list.size() > 0)
		return vertical_header_list.size();

	if(0 == arr_row_list.count())
		return 0;
	else
		return arr_row_list.count();
}

/*! \fn int CTableModel::columnCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CTableModel::columnCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
int CTableModel::columnCount(const QModelIndex &parent) const
{	
	if(horizontal_header_list.size() > 0)
		return horizontal_header_list.size();

	if(0 == arr_row_list.count())
		return 0;
	else if(arr_row_list.count() < 1)
		return 0;
	else
		return arr_row_list.at(0).size();
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
QVariant CTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if(0 == arr_row_list.count())
		return QVariant();

	if(arr_row_list.count() < 1)
		return QVariant();

	if (role == Qt::TextAlignmentRole) 
	{
		return int(Qt::AlignLeft | Qt::AlignVCenter);
	} 
	else if (role == Qt::DisplayRole) 
	{
		if(index.row() >= arr_row_list.count())
			return QVariant();
		if(index.column() >= arr_row_list.at(0).size())
			return QVariant();
		return arr_row_list.at(index.row()).at(index.column());
	}
	else if(role == Qt::EditRole)
	{	
		return arr_row_list.at(index.row()).at(index.column());
	}
	else if (role == Qt::TextColorRole && index.column() == 1)
	{
		//        double d = value.toDouble();
		//        if(d > 0)
		
		QColor color = QColor(Qt::blue);

		return QVariant::fromValue(QColor(Qt::blue));
	}

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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{	
	if(role==Qt::DisplayRole)  
	{  
		if(orientation==Qt::Horizontal) // ˮƽ��ͷ  
		{  
			if(horizontal_header_list.size() > section)
				return horizontal_header_list[section];
			else
				return QVariant(); 
		}  
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // ��ֱ��ͷ  
			//else
			//	return QVariant();
		
			return QString("Row %1").arg(section);
		}
	}  

	return QVariant(); 
}

/*! \fn Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
********************************************************************************************************* 
** \brief CTableModel::flags 
** \details 
** \param index 
** \return Qt::ItemFlags 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
Qt::ItemFlags CTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);  

	flag |= Qt::ItemIsEditable; // ���õ�Ԫ��ɱ༭,�˴�ע��,��Ԫ���޷����༭  
	return flag;  
}

/*! \fn void CTableModel::SetModalDatas(QList< QVector<QString> > &rowlist)
********************************************************************************************************* 
** \brief CTableModel::SetModalDatas 
** \details 
** \param rowlist 
** \return void 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::SetModalDatas(QList< QVector<QString> > &rowlist)
{
	arr_row_list = rowlist;
}

/*! \fn void CTableModel::RefrushModel()
********************************************************************************************************* 
** \brief CTableModel::RefrushModel 
** \details 
** \return void 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::RefrushModel()
{
	beginResetModel();
	endResetModel();

	//emit updateCount(this->rowCount(QModelIndex()));
}


#include <QFont>

/*! \fn bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
********************************************************************************************************* 
** \brief CTableModel::setData 
** \details 
** \param index 
** \param value 
** \param role 
** \return bool 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{	
	QFont font = value.value<QFont>();

	if (index.isValid() && role == Qt::EditRole) 
	{	
		if(arr_row_list[index.row()][index.column()].compare(value.toString()) != 0)
		{
			arr_row_list[index.row()][index.column()] = value.toString();
			
			//m_stack->push(new ChangeValueCommand(index, value, this));

			//emit dataChanged(index, index);
		}

		return true;
	}

	return false;
}

/*! \fn void CTableModel::sort ( int column, Qt::SortOrder order)
********************************************************************************************************* 
** \brief CTableModel::sort 
** \details 
** \param column 
** \param order 
** \return void 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::sort ( int column, Qt::SortOrder order/* = Qt::AscendingOrder */)
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
** \details �������Դ
** \return QList<QVector<QString>> & 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
QList<QVector<QString>> &CTableModel::GetDataList()
{
	return arr_row_list;
}

/*!
    Inserts a number of rows into the model at the specified position.
*/

/*! \fn bool CTableModel::InsertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CTableModel::InsertRows 
** \details ����һ��
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
bool CTableModel::InsertRows(int position, int rows, const QModelIndex &parent)
{
    int columns = columnCount();

    beginInsertRows(parent, position, position + rows - 1);

    for (int row = 0; row < rows; ++row) 
	{
        QVector<QString> items;
        
		for (int column = 0; column < columns; ++column)
            items.append("");
        
		arr_row_list.insert(position + 1, items);
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
** \details ɾ��
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
bool CTableModel::RemoveRows(int position, int rows, const QModelIndex &parent)
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::emitDataChanged(const QModelIndex &index)
{ 
	emit dataChanged(index, index);
}

/*! \fn QUndoStack *CTableModel::undoStackTest()
********************************************************************************************************* 
** \brief CTableModel::undoStackTest 
** \details 
** \return QUndoStack * 
** \author gw
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
QUndoStack *CTableModel::undoStackTest()
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::SaveDataModel(QString tableName)
{
	int nColumns = columnCount();
	
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::AddRowModel(QVector<QString> &rowList, int rowType)
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
void CTableModel::GetColumInfo(QMap<QString, QString> &desc2RealField)
{
	m_desc2RealField.clear();

	m_desc2RealField = desc2RealField;
}
