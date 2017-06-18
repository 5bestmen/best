#include "predictmodel.h"

//������󳤶�
#define MAX_CONTENT_LENGTH  256  


CPredictModel::CPredictModel(CPredictItem& itemInfo, QObject* parent /* = 0 */) : m_predictItemInfo(itemInfo), QAbstractTableModel(parent)
{

}


CPredictModel::~CPredictModel()
{

}


/*! \fn int CPredictModel::columnCount(const QModelIndex& parent) const
*********************************************************************************************
** \brief CPredictModel::columnCount
** \details  ��ȡ������
** \param parent
** \return int
** \author guohaijun
** \date 2017��2��13��
** \note
*********************************************************************************************/
int CPredictModel::columnCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);

	return m_strHeadList.size();
}


/*! \fn int CPredictModel::rowCount(const QModelIndex& parent) const
*********************************************************************************************
** \brief CPredictModel::rowCount
** \details  ��ȡ������
** \param parent
** \return int
** \author guohaijun
** \date 2017��2��13��
** \note
*********************************************************************************************/
int CPredictModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
	Q_UNUSED(parent);

	return m_predictItemInfo.m_vecTableInfo.size();
}


/*! \fn QVariant CPredictModel::headerData(int section, Qt::Orientation orientation, int role) const
****************************************************************************************************
** \brief CPredictModel::headerData
** \details  ��ȡ��ͷ
** \param section
** \param orientation
** \param role
** \return QVariant
** \author ghj
** \date 2017��2��13��
** \note
****************************************************************************************************/
QVariant CPredictModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		//ˮƽ��ͷ
		if (orientation == Qt::Horizontal)
		{
			int nSize = m_strHeadList.size();

			if (nSize > 0)
			{
				return m_strHeadList[section];
			}
		} 
		else
		{
			return QString(" ");
		}
	}

	return QVariant();
}


/*! \fn QVariant CPredictModel::data(const QModelIndex& index, int role) const
*******************************************************************************
** \brief CPredictModel::data
** \details  ��ȡ������
** \param index
** \param role
** \return QVariant
** \author ghj
** \date 2017��2��13��
** \note
*******************************************************************************/
QVariant CPredictModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	int nRow = index.row();
	int nCol = index.column(); 

	switch(role)
	{
		case Qt::TextAlignmentRole:
		{
			//���ж���
			return int(Qt::AlignCenter);
		}
		case Qt::DisplayRole:
		{
			return ShowContent(nCol, nRow);
		}
		default:
			break;
	}

	return QVariant();
}


/*! \fn Qt::ItemFlags CPredictModel::Flags(const QModelIndex &index)
**********************************************************************************************
** \brief CPredictModel::flags
** \details  ��ȡitem���
** \param index
** \return Qt::ItemFlags
** \author ghj
** \date 2017��2��13��
** \note
**********************************************************************************************/
Qt::ItemFlags CPredictModel::flags(const QModelIndex &index) const
{
	return QAbstractItemModel::flags(index);
}


/*! \fn bool CPredictModel::SetData(const QModelIndex& index, const QVariant &value, int role)
**********************************************************************************************
** \brief CPredictModel::SetData
** \details  ���ñ�����
** \param index
** \param value
** \param role
** \return bool
** \author ghj
** \date 2017��2��13��
** \note
**********************************************************************************************/
bool CPredictModel::setData(const QModelIndex& index, const QVariant &value, int role /* = Qt::EditRole */)
{
	Q_UNUSED(index);
	Q_UNUSED(value);
	Q_UNUSED(role);
	return true;
}


/*! \fn bool CPredictModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
*********************************************************************************************************
** \brief CPredictModel::ModifyContentData
** \details  �޸ı�����
** \param index
** \param value
** \param role
** \return bool
** \author ghj
** \date 2017��2��13��
** \note
********************************************************************************************************/
bool CPredictModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
{
	Q_UNUSED(index);
	Q_UNUSED(value);
	Q_UNUSED(role);
	return true;
}


/*! \fn void CPredictModel::SetHeadTitles(QStringList headList)
*********************************************************************************************
** \brief CPredictModel::SetHeadTitles
** \details  ���ñ�ͷ
** \param headList
** \return void
** \author ghj
** \date 2017��2��13��
** \note
*********************************************************************************************/
void CPredictModel::SetHeadTitles(QStringList headList)
{
	m_strHeadList = headList;
}


/*! \fn QVariant CPredictModel::ShowContent(int nCol, int nRow) const
*********************************************************************************************
** \brief CPredictModel::ShowContent
** \details  ��ʾYC����
** \param nCol
** \param nRow
** \return QVariant
** \author ghj
** \date 2017��2��13��
** \note
*********************************************************************************************/
QVariant CPredictModel::ShowContent(int nCol, int nRow) const
{
	char buf[MAX_CONTENT_LENGTH] = { 0 };

	switch (nCol)
	{
	case ID:
	{
		sprintf(buf, "%d", nRow);
		break;
	}
	case TagName:
	{
		sprintf(buf, "%s", m_predictItemInfo.m_vecTableInfo[nRow].m_strTagName.toStdString().c_str());
		break;
	}
	case Describe:
	{
		sprintf(buf, "%s", m_predictItemInfo.m_vecTableInfo[nRow].m_strDesc.toStdString().c_str());
		break;
	}
	case OccNo:
	{
		sprintf(buf, "%d", m_predictItemInfo.m_vecTableInfo[nRow].m_OccNo);
		break;
	}
	default:
		break;			
	}

	return QVariant(buf);
}