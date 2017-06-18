/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出model
文件名       :  outputmessagerserver.cpp
文件实现功能 :  系统输出model
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
*  @date    2015.11.30
*/

#include <QColor>
#include "systemoutputmodel.h"


/*! \fn CSystemOutputModel::CSystemOutputModel(QObject *parent)
********************************************************************************************************* 
** \brief CSystemOutputModel::CSystemOutputModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
CSystemOutputModel::CSystemOutputModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

/*! \fn CSystemOutputModel::~CSystemOutputModel()
********************************************************************************************************* 
** \brief CSystemOutputModel::~CSystemOutputModel 
** \details 
** \return  
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
CSystemOutputModel::~CSystemOutputModel()
{

}

/*! \fn int CSystemOutputModel::rowCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CSystemOutputModel::rowCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
int CSystemOutputModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	if(0 == arr_row_list.count())
		return 0;
	else
		return arr_row_list.count();
}

/*! \fn int CSystemOutputModel::columnCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CSystemOutputModel::columnCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
int CSystemOutputModel::columnCount(const QModelIndex &parent) const
{
	if(0 == arr_row_list.count())
		return 0;
	else
	{
		int nTmp = arr_row_list.at(0).size();
		
		return arr_row_list.at(0).size();
	}

}

/*! \fn QVariant CSystemOutputModel::data(const QModelIndex &index, int role) const
********************************************************************************************************* 
** \brief CSystemOutputModel::data 
** \details 
** \param index 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
QVariant CSystemOutputModel::data(const QModelIndex &index, int role) const
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
		QColor color = QColor(Qt::blue);

		return QVariant::fromValue(QColor(Qt::blue));
	}

	return QVariant();
}

/*! \fn void CSystemOutputModel::addRow(QVector<QString> rowVec)
********************************************************************************************************* 
** \brief CSystemOutputModel::addRow 
** \details 
** \param rowVec 
** \return void 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
void CSystemOutputModel::addRow(QVector<QString> rowVec)
{	
	arr_row_list.push_back(rowVec);

	int nRow = arr_row_list.size();

	//insertRows(nRow, 1);

	beginResetModel();

	endResetModel();
}

/*! \fn bool CSystemOutputModel::insertRows(int position, int rows, const QModelIndex &parent)
********************************************************************************************************* 
** \brief CSystemOutputModel::insertRows 
** \details 
** \param position 
** \param rows 
** \param parent 
** \return bool 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
bool CSystemOutputModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(parent, position, position + rows - 1);

	QString strTmp = "test";

	QVector<QString> vec;

	vec.push_back(strTmp);

	arr_row_list.push_back(vec);

	endInsertRows();

	return true;
}

/*! \fn bool CSystemOutputModel::setData(const QModelIndex &index, const QVariant &value, int role)
********************************************************************************************************* 
** \brief CSystemOutputModel::setData 
** \details 
** \param index 
** \param value 
** \param role 
** \return bool 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
bool CSystemOutputModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{	
	if (index.isValid() && role == Qt::EditRole) 
	{	
		//QVariant var = index.data(Qt::TextColorRole);

		//QColor color = var.value<QColor>();

		//	var = index.data(Qt::EditRole);

		//color = value.value<QColor>();

		if(arr_row_list[index.row()][index.column()].compare(value.toString()) != 0)
		{
			arr_row_list[index.row()][columnCount() - 1] = "1";

			arr_row_list[index.row()][index.column()] = value.toString();

			emit dataChanged(index, index);
		}

		return true;
	}

	return false;
}

/*! \fn Qt::ItemFlags CSystemOutputModel::flags(const QModelIndex &index) const
********************************************************************************************************* 
** \brief CSystemOutputModel::flags 
** \details 
** \param index 
** \return Qt::ItemFlags 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
Qt::ItemFlags CSystemOutputModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);  

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;  
}