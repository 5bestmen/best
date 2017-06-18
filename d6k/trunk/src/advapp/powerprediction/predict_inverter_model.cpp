#include "predict_inverter_model.h"
#include "predict_define.h"

#include <QObject>
#include <QStandardItem>

/*! \fn CPredictInverterModel::CPredictInverterModel(QObject* parent, QVector<CStationData* > &vecStnData, IMainModuleInterface* pCore)
*************************************************************************************************************************************
** \brief   CPredictInverterModel::CPredictInverterModel
** \details 构造函数
** \return
** \author  GuoHaijun
** \date    2017年3月30日
** \note
************************************************************************************************************************************/
CPredictInverterModel::CPredictInverterModel(QVector<CPPPointInfo >& vecInvterItems, QObject* parnt)
	:QAbstractTableModel(parnt), m_vecInvterItems(vecInvterItems)
{
	m_strListHorHead.clear();
	m_mapIntStr.clear();

	m_mapIntStr.insert(ID, "ID");
	m_mapIntStr.insert(Name, "Name");
	m_mapIntStr.insert(Type, "Type");
	m_mapIntStr.insert(SelectPoint, "SelectPoint");

	m_strListHorHead.append("ID");
	m_strListHorHead.append("Name");
	m_strListHorHead.append("Type");
	m_strListHorHead.append("SelectPoint");
	CPredictSelctPointDelegate *delegate = new CPredictSelctPointDelegate(this);
	((CPredictStationTable *)parent())->setItemDelegateForColumn(SelectPoint, delegate);
}


/*! \fn CPredictInverterModel::~CPredictInverterModel()
********************************************************
** \brief   CPredictInverterModel::~CPredictInverterModel
** \details 析构函数
** \return
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************/
CPredictInverterModel::~CPredictInverterModel()
{


}


/*! \fn void CPredictInverterModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
********************************************************************************************
** \brief   CPredictInverterModel::SetHorizontalHeaderList
** \details 设置水平表头
** \return  strListHorHead
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************************************************/
void CPredictInverterModel::SetHorizontalHeaderList(QStringList strListHorHead)
{
	if (!m_strListHorHead.isEmpty())
	{
		m_strListHorHead.clear();
	}

	m_strListHorHead = strListHorHead;
}



/*! \fn void CPredictInverterModel::SetVerticalHeaderList(QStringList strListVerHead)
********************************************************************************************
** \brief   CPredictInverterModel::SetVerticalHeaderList
** \details 设置表头
** \return  strListVerHead
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************************************************/
void CPredictInverterModel::SetVerticalHeaderList(QStringList strListVerHead)
{

	return;
}



/*! \fn int CPredictInverterModel::rowCount(const QModelIndex &parent) const
********************************************************************************************
** \brief   CPredictInverterModel::rowCount
** \details 行数
** \return  parent
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************************************************/
int CPredictInverterModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);

	int nCount = 5;

	return nCount;
}



/*! \fn int CPredictInverterModel::columnCount(const QModelIndex &parent) const
********************************************************************************************
** \brief   CPredictInverterModel::columnCount
** \details 列数
** \return  parent
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************************************************/
int CPredictInverterModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);

	return m_strListHorHead.size();
}



/*! \fn QVariant CPredictInverterModel::data(const QModelIndex &index, int role) const
********************************************************************************************
** \brief   CPredictInverterModel::data
** \details 数据
** \return  index
** \return  role
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************************************************/
QVariant CPredictInverterModel::data(const QModelIndex &index, int role/*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	int nRow = index.row();
	int nCol = index.column();

	switch (role)
	{
	case Qt::TextAlignmentRole:
	{
		return int(Qt::AlignCenter);// Qt::AlignVCenter);
	}
	case Qt::DisplayRole:
	{
		return ShowData(nRow, nCol);

		break;
	}
	default:
		break;
	}
	return QVariant();
}



/*! \fn QVariant CPredictInverterModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
*****************************************************************************************************************************
** \brief   CPredictInverterModel::headerData
** \details 表头
** \return  section
** \return  orientation
** \return  role
** \author  GuoHaijun
** \date    2017年3月31日
** \note
*****************************************************************************************************************************/
QVariant CPredictInverterModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			int nSize = m_strListHorHead.size();

			if (nSize > 0)
			{
				return m_strListHorHead[section];
			}
		}
		else
		{
			return QString(" ");
		}
	}

	return QVariant();
}



/*! \fn bool CPredictInverterModel::setData(const QModelIndex &index, const QVariant &value, int role)
*****************************************************************************************************
** \brief   CPredictInverterModel::setData
** \details 设置表数据
** \param   index
** \param   value
** \param   role
** \return  bool
** \author  GuoHaijun
** \date    2017年4月1日
** \note
******************************************************************************************************/
bool CPredictInverterModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	Q_UNUSED(value);
	Q_UNUSED(role);
	if (!index.isValid())
	{
		return false;
	}

	return true;
}



/*! \fn Qt::ItemFlags CPredictInverterModel::flags(const QModelIndex &index) const
*********************************************************************************
** \brief   CPredictInverterModel::flags
** \details 标记
** \param   index
** \return  Qt::ItemFlags
** \author  GuoHaijun
** \date    2017年4月1日
** \note
**********************************************************************************/
Qt::ItemFlags CPredictInverterModel::flags(const QModelIndex &index) const
{
	if (!index.isValid() || index.column() <= SelectPoint)
	{
		return QAbstractItemModel::flags(index);
	}

	Qt::ItemFlags flags = QAbstractItemModel::flags(index);

	int nCol = index.column();
	if (nCol == SelectPoint)
	{
		flags |= Qt::ItemIsUserCheckable;
	}
	else
	{
		flags |= Qt::ItemIsEditable;
	}

	return flags;
}



/*! \fn void CPredictInverterModel::RefrushModel()
*************************************************
** \brief   CPredictInverterModel::RefrushModel
** \details 刷新
** \return
** \author  GuoHaijun
** \date    2017年3月31日
** \note
*************************************************/
void CPredictInverterModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}



/*! \fn bool CPredictInverterModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
****************************************************************************************************************
** \brief   CPredictInverterModel::ModifyContentData
** \details 修改表数据
** \param   index
** \param   value
** \param   role
** \return  void
** \author  GuoHaijun
** \date    2017年4月1日
** \note
****************************************************************************************************************/
bool CPredictInverterModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
{
	Q_UNUSED(index);
	Q_UNUSED(value);
	Q_UNUSED(role);
	return true;
}



/*! \fn void CPredictInverterModel::SetColumnCount(int columns)
**************************************************************
** \brief   CPredictInverterModel::SetColumnCount
** \details 设置列数
** \param   columns
** \return  void
** \author  GuoHaijun
** \date    2017年4月1日
** \note
**************************************************************/
void CPredictInverterModel::SetColumnCount(int columns)
{
	int nCount = columnCount();
	if (nCount == columns)
	{
		return;
	}

	if (nCount < columns)
	{
		insertColumns(qMax(nCount, 0), columns - nCount);
	}
	else
	{
		removeColumns(qMax(columns, 0), nCount - columns);
	}
}



/*! \fn void CPredictInverterModel::SetRowCount(int rows)
********************************************************
** \brief   CPredictInverterModel::SetRowCount
** \details 设置行数
** \param   rows
** \return  void
** \author  GuoHaijun
** \date    2017年3月31日
** \note
********************************************************/
void CPredictInverterModel::SetRowCount(int rows)
{
	int nCount = rowCount();
	if (nCount == rows)
	{
		return;
	}

	if (nCount < rows)
	{
		insertRows(qMax(nCount, 0), rows - nCount);
	}
	else
	{
		removeRows(qMax(rows, 0), nCount - rows);
	}
}



/*! \fn QVariant CPredictInverterModel::ShowData(int nRow, int nCol) const
*************************************************************************
** \brief   CPredictInverterModel::ShowData
** \details 数据显示
** \param   nRow
** \param   nCol
** \return  QVariant
** \author  GuoHaijun
** \date    2017年4月1日
** \note
*************************************************************************/
QVariant CPredictInverterModel::ShowData(int nRow, int nCol) const
{
	char buf[M_MAX_CONTENT_LEN] = { 0 };

	switch (nCol)
	{
	case ID:
	{
		sprintf(buf, "%d", nRow);
	}
	case Name:
	{
		//sprintf(buf, "%s", m_StnData.m_vecTableInfo[nRow].m_szName.toStdString().c_str());
	}
	case Type:
	{
		//sprintf(buf, "%s", m_StnData.m_vecTableInfo[nRow].m_szName.toStdString().c_str());
	}
	case SelectPoint:
	{
		//sprintf(buf, "%s", m_StnData.m_vecTableInfo[nRow].m_szSeletPoint.toStdString().c_str());
	}
	default:
		break;
	}

	return QVariant(buf);

}

