#include "predictaimodel.h"

CPredictAiModel::CPredictAiModel(QObject* parent, std::vector <CAiInput *> &vecAi, IMainModuleInterface* pCore)
	:QAbstractTableModel(parent), m_vecAI(vecAi), m_pCore(pCore)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;

	InitPara();

}


CPredictAiModel::~CPredictAiModel()
{


}

void CPredictAiModel::SetHorizontalHeaderList(QStringList horizontalHeaderList)
{
	if (!horizontal_header_list.isEmpty())
	{
		horizontal_header_list.clear();
	}

	horizontal_header_list = horizontalHeaderList;
}


void CPredictAiModel::SetVerticalHeaderList(QStringList verticalHeaderList)
{
	Q_UNUSED(verticalHeaderList);

	return;
}

int CPredictAiModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return (int)m_vecAI.size();
}

int CPredictAiModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	if (horizontal_header_list.size() > 0)
	{
		return horizontal_header_list.size();
	}
	
	return 0;
}

QVariant CPredictAiModel::data(const QModelIndex &index, int role) const
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
			return static_cast<uint>(m_vecAI[nRow]->m_nID);
		case TagName:
			return m_vecAI[nRow]->m_szTagName;
		case Desc:
			return m_vecAI[nRow]->m_strDesc;
		case OccNo:
			return static_cast<uint>(m_vecAI[nRow]->m_nOccNo);
		default:
			return QVariant();
		}
	}
	else if(role == Qt::EditRole)
	{
		int nIndex = index.column();

		switch (nIndex)
		{
		case ID:
			return static_cast<uint>(m_vecAI[nRow]->m_nID);
		case TagName:
			return m_vecAI[nRow]->m_szTagName;
		case Desc:
			return m_vecAI[nRow]->m_strDesc;
		case OccNo:
			return (uint)m_vecAI[nRow]->m_nOccNo;
		default:
			return QVariant();
		}
	}

	return QVariant();
}


QVariant CPredictAiModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const
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


Qt::ItemFlags CPredictAiModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return Qt::NoItemFlags;
	}

	Qt::ItemFlags flag = QAbstractTableModel::flags(index);

	flag |= Qt::ItemIsEditable;
	return flag;
}

void CPredictAiModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CPredictAiModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
			if (m_vecAI[nRow]->m_nID != value.toInt())
			{
				m_vecAI[nRow]->m_nID = value.toInt();
			}

		}
		else if (index.column() == TagName)
		{
			if (m_vecAI[nRow]->m_szTagName != value.toString())
			{
				//待实现
				return true;
			}
		}
		else if (index.column() == Desc)
		{
			if (m_vecAI[nRow]->m_strDesc.compare(value.toString()) != 0)
			{
				m_vecAI[nRow]->m_strDesc = value.toString();
			}
		}
		else if (index.column() == OccNo)
		{
			if (m_vecAI[nRow]->m_nOccNo != value.toInt())
			{
				m_vecAI[nRow]->m_nOccNo = value.toInt();
			}
		}

		return true;
	}

	return false;
}


bool CPredictAiModel::InsertRows(int position, int rows, const QModelIndex &parent /* = QModelIndex() */)
{
	beginInsertRows(parent, position, position + rows - 1);

    //待实现

	endInsertRows();

	return true;
}


bool CPredictAiModel::RemoveRows(int position, int rows, const QModelIndex &parent /* = QModelIndex() */)
{
	beginRemoveRows(parent, rows, rows);

	//待实现

	endRemoveRows();

	if (m_vecAI.size() == 0)
	{
		RefrushModel();;
	}

	return true;
}

void CPredictAiModel::InitPara()
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

	SetHorizontalHeaderList(horizontal_header_list);
}