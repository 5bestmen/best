#include "predictparammodel.h"

CPredictParamModel::CPredictParamModel(QObject* parent, std::vector <CParamInput *> &vecAi, IMainModuleInterface* pCore)
	:QAbstractTableModel(parent), m_vecParam(vecAi), m_pCore(pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	InitPara();

}


CPredictParamModel::~CPredictParamModel()
{


}


void CPredictParamModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = horizontalHeaderList;
}


void CPredictParamModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	Q_UNUSED(verticalHeaderList);

	return;
}

int CPredictParamModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return (int)m_vecParam.size();
}

int CPredictParamModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	if (horizontal_header_list.size() > 0)
	{
		return horizontal_header_list.size();
	}

	return 0;
}

QVariant CPredictParamModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	int nRow = index.row();

	if (role == Qt::DisplayRole)
	{
		int nIndex = index.column();

		switch (nIndex)
		{
		case ID:
			return static_cast<uint>(m_vecParam[nRow]->m_nID);
		case TagName:
			return m_vecParam[nRow]->m_szTagName;
		case Desc:
			return m_vecParam[nRow]->m_strDesc;
		case OccNo:
			return static_cast<uint>(m_vecParam[nRow]->m_nOccNo);
		default:
			return QVariant();
		}
	}
	else if (role == Qt::EditRole)
	{
		int nIndex = index.column();

		switch (nIndex)
		{
		case ID:
			return static_cast<uint>(m_vecParam[nRow]->m_nID);
		case TagName:
			return m_vecParam[nRow]->m_szTagName;
		case Desc:
			return m_vecParam[nRow]->m_strDesc;
		case OccNo:
			return (uint)m_vecParam[nRow]->m_nOccNo;
		default:
			return QVariant();
		}
	}

	return QVariant();
}


QVariant CPredictParamModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			if (horizontal_header_list.size() > section && section >= 0)
			{
				return horizontal_header_list[section];
			}
			else
			{
				return QVariant();
			}
		}
		else
		{
			return QString(" ");
		}
	}

	return QVariant();
}


Qt::ItemFlags CPredictParamModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::NoItemFlags;
	}

	Qt::ItemFlags flag = QAbstractTableModel::flags(index);

	flag |= Qt::ItemIsEditable;
	return flag;
}

void CPredictParamModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CPredictParamModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	int nRow = index.row();

	if (!index.isValid())
	{
		return false;
	}

	if (role == Qt::EditRole)
	{
		if (index.column() == ID)
		{
			if (m_vecParam[nRow]->m_nID != value.toInt())
			{
				m_vecParam[nRow]->m_nID = value.toInt();
			}

		}
		else if (index.column() == TagName)
		{
			if (m_vecParam[nRow]->m_szTagName != value.toString())
			{
				//待实现
				return true;
			}
		}
		else if (index.column() == Desc)
		{
			if (m_vecParam[nRow]->m_strDesc.compare(value.toString()) != 0)
			{
				m_vecParam[nRow]->m_strDesc = value.toString();
			}
		}
		else if (index.column() == OccNo)
		{
			if (m_vecParam[nRow]->m_nOccNo != value.toInt())
			{
				m_vecParam[nRow]->m_nOccNo = value.toInt();
			}
		}

		return true;
	}

	return false;
}


bool CPredictParamModel::InsertRows(int position, int rows, const QModelIndex &parent /* = QModelIndex() */)
{
	beginInsertRows(parent, position, position + rows - 1);

	//待实现

	endInsertRows();

	return true;
}


bool CPredictParamModel::RemoveRows(int position, int rows, const QModelIndex &parent /* = QModelIndex() */)
{
	beginRemoveRows(parent, rows, rows);

	//待实现

	endRemoveRows();

	if (m_vecParam.size() == 0)
	{
		RefrushModel();;
	}

	return true;
}

void CPredictParamModel::InitPara()
{
	horizontal_header_list.clear();

	Predict_Attributes attributePara;

	attributePara.desc = tr("ID");
	attributePara.tagName = "ID";
	m_mapInfo.insert(ID, attributePara);
	horizontal_header_list.append(attributePara.desc);

	attributePara.desc = tr("TagName");
	attributePara.tagName = "TagName";
	m_mapInfo.insert(TagName, attributePara);
	horizontal_header_list.append(attributePara.desc);

	attributePara.desc = tr("Desc");
	attributePara.tagName = "Desc";
	m_mapInfo.insert(Desc, attributePara);
	horizontal_header_list.append(attributePara.desc);

	attributePara.desc = tr("OccNo");
	attributePara.tagName = "OccNo";
	m_mapInfo.insert(OccNo, attributePara);
	horizontal_header_list.append(attributePara.desc);

	attributePara.desc = tr("Value");
	attributePara.tagName = "Value";
	m_mapInfo.insert(Value, attributePara);
	horizontal_header_list.append(attributePara.desc);

	SetHorizontalHeaderList(horizontal_header_list);
}