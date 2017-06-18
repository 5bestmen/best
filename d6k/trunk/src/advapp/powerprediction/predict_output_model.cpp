#include "predict_output_model.h"
#include "predict_define.h"

#include <QObject>
#include <QStandardItem>

/*! \fn CPredictOutputModel::CPredictOutputModel(QObject* parent, QVector<CStationData* > &vecStnData, IMainModuleInterface* pCore)
*************************************************************************************************************************************
** \brief   CPredictOutputModel::CPredictOutputModel
** \details ���캯��
** \return
** \author  GuoHaijun
** \date    2017��3��30��
** \note
************************************************************************************************************************************/
CPredictOutputModel::CPredictOutputModel(QVector<CPPPointInfo >& vecWeaItems, QObject* parnt)
	:QAbstractTableModel(parnt), m_vecWeaItems(vecWeaItems)
{

	m_strListHorHead.clear();
	m_mapOutputStr.clear();

	m_mapOutputStr.insert(ID, "ID");
	m_mapOutputStr.insert(Name, "Name");
	m_mapOutputStr.insert(Type, "Type");
	m_mapOutputStr.insert(SelectPoint, "SelectPoint");

	m_strListHorHead.append("ID");
	m_strListHorHead.append("Name");
	m_strListHorHead.append("SelectPoint");
	m_strListHorHead.append("SelectPoint");
	CPredictSelctPointDelegate *delegate = new CPredictSelctPointDelegate(this);
	((CPredictOutputTable *)parent())->setItemDelegateForColumn(SelectPoint, delegate);


}
/*! \fn CPredictOutputModel::~CPredictOutputModel()
********************************************************
** \brief   CPredictOutputModel::~CPredictOutputModel
** \details ��������
** \return
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************/
CPredictOutputModel::~CPredictOutputModel()
{


}
/*! \fn void CPredictOutputModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
********************************************************************************************
** \brief   CPredictOutputModel::SetHorizontalHeaderList
** \details ����ˮƽ��ͷ
** \return  strListHorHead
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
void CPredictOutputModel::SetHorizontalHeaderList(QStringList strListHorHead)
{
	if (!m_strListHorHead.isEmpty())
	{
		m_strListHorHead.clear();
	}

	m_strListHorHead = strListHorHead;
}
/*! \fn void CPredictOutputModel::SetVerticalHeaderList(QStringList strListVerHead)
********************************************************************************************
** \brief   CPredictOutputModel::SetVerticalHeaderList
** \details ���ñ�ͷ
** \return  strListVerHead
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
void CPredictOutputModel::SetVerticalHeaderList(QStringList strListVerHead)
{

	return;
}
/*! \fn int CPredictOutputModel::rowCount(const QModelIndex &parent) const
********************************************************************************************
** \brief   CPredictOutputModel::rowCount
** \details ����
** \return  parent
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
int CPredictOutputModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);

	int nCount = 2;

	return nCount;
}
/*! \fn int CPredictOutputModel::columnCount(const QModelIndex &parent) const
********************************************************************************************
** \brief   CPredictOutputModel::columnCount
** \details ����
** \return  parent
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
int CPredictOutputModel::columnCount(const QModelIndex &parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);

	return m_strListHorHead.size();
}
/*! \fn QVariant CPredictOutputModel::data(const QModelIndex &index, int role) const
********************************************************************************************
** \brief   CPredictOutputModel::data
** \details ����
** \return  index
** \return  role
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************************************************/
QVariant CPredictOutputModel::data(const QModelIndex &index, int role/*= Qt::DisplayRole*/) const
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
/*! \fn QVariant CPredictOutputModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
*****************************************************************************************************************************
** \brief   CPredictOutputModel::headerData
** \details ��ͷ
** \return  section
** \return  orientation
** \return  role
** \author  GuoHaijun
** \date    2017��3��31��
** \note
*****************************************************************************************************************************/
QVariant CPredictOutputModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
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
/*! \fn bool CPredictOutputModel::setData(const QModelIndex &index, const QVariant &value, int role)
*****************************************************************************************************
** \brief   CPredictOutputModel::setData
** \details ���ñ�����
** \param   index
** \param   value
** \param   role
** \return  bool
** \author  GuoHaijun
** \date    2017��4��1��
** \note
******************************************************************************************************/
bool CPredictOutputModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	Q_UNUSED(value);
	Q_UNUSED(role);
	if (!index.isValid())
	{
		return false;
	}

	return true;
}
/*! \fn Qt::ItemFlags CPredictOutputModel::flags(const QModelIndex &index) const
*********************************************************************************
** \brief   CPredictOutputModel::flags
** \details ���
** \param   index
** \return  Qt::ItemFlags
** \author  GuoHaijun
** \date    2017��4��1��
** \note
**********************************************************************************/
Qt::ItemFlags CPredictOutputModel::flags(const QModelIndex &index) const
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
/*! \fn void CPredictOutputModel::RefrushModel()
*************************************************
** \brief   CPredictOutputModel::RefrushModel
** \details ˢ��
** \return
** \author  GuoHaijun
** \date    2017��3��31��
** \note
*************************************************/
void CPredictOutputModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}
/*! \fn bool CPredictOutputModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
****************************************************************************************************************
** \brief   CPredictOutputModel::ModifyContentData
** \details �޸ı�����
** \param   index
** \param   value
** \param   role
** \return  void
** \author  GuoHaijun
** \date    2017��4��1��
** \note
****************************************************************************************************************/
bool CPredictOutputModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
{
	Q_UNUSED(index);
	Q_UNUSED(value);
	Q_UNUSED(role);
	return true;
}
/*! \fn void CPredictOutputModel::SetColumnCount(int columns)
**************************************************************
** \brief   CPredictOutputModel::SetColumnCount
** \details ��������
** \param   columns
** \return  void
** \author  GuoHaijun
** \date    2017��4��1��
** \note
**************************************************************/
void CPredictOutputModel::SetColumnCount(int columns)
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
/*! \fn void CPredictOutputModel::SetRowCount(int rows)
********************************************************
** \brief   CPredictOutputModel::SetRowCount
** \details ��������
** \param   rows
** \return  void
** \author  GuoHaijun
** \date    2017��3��31��
** \note
********************************************************/
void CPredictOutputModel::SetRowCount(int rows)
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
/*! \fn QVariant CPredictOutputModel::ShowData(int nRow, int nCol) const
*************************************************************************
** \brief   CPredictOutputModel::ShowData
** \details ������ʾ
** \param   nRow
** \param   nCol
** \return  QVariant
** \author  GuoHaijun
** \date    2017��4��1��
** \note
*************************************************************************/
QVariant CPredictOutputModel::ShowData(int nRow, int nCol) const
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

