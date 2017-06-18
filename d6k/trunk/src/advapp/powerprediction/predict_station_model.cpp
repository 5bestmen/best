#include "predict_station_model.h"
#include "predict_define.h"

#include <QObject>
#include <QStandardItem>

/*! \fn CPredictStationModel::CPredictStationModel(QObject* parent, QVector<CStationData* > &vecStnData, IMainModuleInterface* pCore)
*************************************************************************************************************************************
** \brief   CPredictStationModel::CPredictStationModel
** \details ���캯��
** \return
** \author  GuoHaijun
** \date    2017��3��30��
** \note
************************************************************************************************************************************/
CPredictStationModel::CPredictStationModel(QVector<CPPPointInfo >&vecStnItems, QObject* parnt)
	:QAbstractTableModel(parnt), m_vecStnItems(vecStnItems)
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
	((CPredictStationTable *) parent())->setItemDelegateForColumn(SelectPoint, delegate);
}


/*! \fn CPredictStationModel::~CPredictStationModel()
********************************************************
** \brief   CPredictStationModel::~CPredictStationModel
** \details ��������
** \return
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************/
CPredictStationModel::~CPredictStationModel()
{


}


/*! \fn void CPredictStationModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
********************************************************************************************
** \brief   CPredictStationModel::SetHorizontalHeaderList
** \details ����ˮƽ��ͷ
** \return  strListHorHead
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
void CPredictStationModel::SetHorizontalHeaderList(QStringList strListHorHead)
{
	if (!m_strListHorHead.isEmpty())
	{
		m_strListHorHead.clear();
	}
	m_strListHorHead = strListHorHead;
}



/*! \fn void CPredictStationModel::SetVerticalHeaderList(QStringList strListVerHead)
********************************************************************************************
** \brief   CPredictStationModel::SetVerticalHeaderList
** \details ���ñ�ͷ
** \return  strListVerHead
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
void CPredictStationModel::SetVerticalHeaderList(QStringList strListVerHead)
{
	return;
}



/*! \fn int CPredictStationModel::rowCount(const QModelIndex &parent) const
********************************************************************************************
** \brief   CPredictStationModel::rowCount
** \details ����
** \return  parent
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
int CPredictStationModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);
	int nCount = 2;
	return nCount;
}



/*! \fn int CPredictStationModel::columnCount(const QModelIndex &parent) const
********************************************************************************************
** \brief   CPredictStationModel::columnCount
** \details ����
** \return  parent
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
int CPredictStationModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);
	return m_strListHorHead.size();
}



/*! \fn QVariant CPredictStationModel::data(const QModelIndex &index, int role) const
********************************************************************************************
** \brief   CPredictStationModel::data
** \details ����
** \return  index
** \return  role
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
QVariant CPredictStationModel::data(const QModelIndex &index, int role/*= Qt::DisplayRole*/) const
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



/*! \fn QVariant CPredictStationModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
*****************************************************************************************************************************
** \brief   CPredictStationModel::headerData
** \details ��ͷ
** \return  section
** \return  orientation
** \return  role
** \author  GuoHaijun
** \date    2017��3��31��
** \note
*****************************************************************************************************************************/
QVariant CPredictStationModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
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



/*! \fn bool CPredictStationModel::setData(const QModelIndex &index, const QVariant &value, int role)
*****************************************************************************************************
** \brief   CPredictStationModel::setData
** \details ���ñ�����
** \param   index
** \param   value
** \param   role
** \return  bool
** \author  GuoHaijun
** \date    2017��4��1��
** \note
******************************************************************************************************/
bool CPredictStationModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	Q_UNUSED(value);
	Q_UNUSED(role);
	if (!index.isValid())
	{
		return false;
	}

	return true;
}



/*! \fn Qt::ItemFlags CPredictStationModel::flags(const QModelIndex &index) const
*********************************************************************************
** \brief   CPredictStationModel::flags
** \details ���
** \param   index
** \return  Qt::ItemFlags
** \author  GuoHaijun
** \date    2017��4��1��
** \note
**********************************************************************************/
Qt::ItemFlags CPredictStationModel::flags(const QModelIndex &index) const
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



/*! \fn void CPredictStationModel::RefrushModel()
*************************************************
** \brief   CPredictStationModel::RefrushModel
** \details ˢ��
** \return  
** \author  GuoHaijun
** \date    2017��3��31��
** \note
*************************************************/
void CPredictStationModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}



/*! \fn bool CPredictStationModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
****************************************************************************************************************
** \brief   CPredictStationModel::ModifyContentData
** \details �޸ı�����
** \param   index
** \param   value
** \param   role
** \return  void
** \author  GuoHaijun
** \date    2017��4��1��
** \note
****************************************************************************************************************/
bool CPredictStationModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
{
	Q_UNUSED(index);
	Q_UNUSED(value);
	Q_UNUSED(role);
	return true;
}



/*! \fn void CPredictStationModel::SetColumnCount(int columns)
**************************************************************
** \brief   CPredictStationModel::SetColumnCount
** \details ��������
** \param   columns
** \return  void
** \author  GuoHaijun
** \date    2017��4��1��
** \note
**************************************************************/
void CPredictStationModel::SetColumnCount(int columns)
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



/*! \fn void CPredictStationModel::SetRowCount(int rows)
********************************************************
** \brief   CPredictStationModel::SetRowCount
** \details ��������
** \param   rows
** \return  void
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************/
void CPredictStationModel::SetRowCount(int rows)
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



/*! \fn QVariant CPredictStationModel::ShowData(int nRow, int nCol) const
*************************************************************************
** \brief   CPredictStationModel::ShowData
** \details ������ʾ
** \param   nRow
** \param   nCol
** \return  QVariant
** \author  GuoHaijun
** \date    2017��4��1��
** \note
*************************************************************************/
QVariant CPredictStationModel::ShowData(int nRow, int nCol) const
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
	case SelectPoint:
	{
		//sprintf(buf, "%s", m_StnData.m_vecTableInfo[nRow].m_szSeletPoint.toStdString().c_str());
	}
	default:
		break;
	}

	return QVariant(buf);

}

