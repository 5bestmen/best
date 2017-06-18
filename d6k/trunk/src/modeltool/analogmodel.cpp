
#include "analogmodel.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>



CAnalogModel::CAnalogModel(QMap<int, CContentInfo>& mapContentInfo, QObject* parent) : m_mapAnalogInfo(mapContentInfo), QAbstractTableModel(parent)
{

}

CAnalogModel::~CAnalogModel()
{

}

/*virtual */
int CAnalogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_mapAnalogInfo.size();
}

/*virtual */
int CAnalogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_headList.size();
}

/*virtual */
QVariant CAnalogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int nSize = m_headList.size();

        if (nSize > 0)
        {
            return m_headList[section];
        }
    }

    return QVariant();
}

/*virtual */
QVariant CAnalogModel::data(const QModelIndex &index, int role) const
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
        return ShowAnalog(nCol, nRow);

        break;
    }
    case Qt::CheckStateRole:
    {
        if (nCol == ANALOG_READ_WRITE)
        {
            return m_mapAnalogInfo[nRow].s_bReadWrite ? Qt::Checked : Qt::Unchecked;
        }

        break;
    }
    case  Qt::EditRole:
    {
        if (nCol == ANALOG_TYPE)
        {
            return QVariant(m_mapAnalogInfo[nRow].s_nDataType);
        }

        break;
    }
    default:
        break;
    }
    return QVariant();
}

/*virtual */
Qt::ItemFlags CAnalogModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() == 0)
    {
        return QAbstractItemModel::flags(index);
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    int nCol = index.column();
    if (nCol == ANALOG_READ_WRITE)
    {
        flags |= Qt::ItemIsUserCheckable;
    }
    else
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

/*virtual */
bool CAnalogModel::setData(const QModelIndex &index, const QVariant &value, int role/* = Qt::EditRole*/)
{
    int nRow = index.row();
    int nCol = index.column();
    //if (index.isValid() && index.row() != index.column() && role == Qt::EditRole)
    if (!index.isValid() || index.column() == 0)
    {
        return false;
    }

    return ModifyAnalogData(index, value, role);
}

bool CAnalogModel::ModifyAnalogData(const QModelIndex& index, const QVariant& value, int& role)
{
    int nRow = index.row();
    int nCol = index.column();

    switch (role)
    {
    case Qt::DisplayRole://case Qt::EditRole:
    {
        break;
    }
    case Qt::CheckStateRole:
    {
        if (nCol == ANALOG_READ_WRITE)
        {
            if (nCol == ANALOG_READ_WRITE)
            {
                m_mapAnalogInfo[nRow].s_bReadWrite = (value.toInt() == Qt::Checked);

                emit dataChanged(index, index);
                return true;
            }
        }
        break;
    }
    case Qt::EditRole:
    {
        if (value.toString().isEmpty())
        {
            return false;
        }

        // 修改内容
        switch (nCol)
        {
            case ANALOG_DESCRIBE:
            {
                if (m_mapAnalogInfo[nRow].s_strDescribe != value.toString().toStdString())
                {
                    m_mapAnalogInfo[nRow].s_strDescribe = value.toString().toStdString();
                }

                break;
            }
            case ANALOG_TYPE:
            {
                if (m_mapAnalogInfo[nRow].s_nDataType != value.toInt())
                {
                    m_mapAnalogInfo[nRow].s_nDataType = value.toInt();
                }

                break;
            }
            case ANALOG_READ_WRITE:
            {
                if (m_mapAnalogInfo[nRow].s_bReadWrite != value.toBool())
                {
                    m_mapAnalogInfo[nRow].s_bReadWrite = value.toBool();
                }

                break;
            }
        }

        //交换对应项的模型索引  
        QModelIndex transposedIndex = createIndex(index.column(), index.row());

        //某项发生改变，发射信号( between topLeft and bottomRight inclusive)  
        emit dataChanged(index, index);
        emit dataChanged(transposedIndex, transposedIndex);

        break;

    }
    default:
        return false;

    }
    return true;
}

void CAnalogModel::SetHeadTitles(QStringList headList)
{
    m_headList = headList;
}

QVariant CAnalogModel::ShowAnalog(int nCol, int nRow) const
{
    QMap<int, CContentInfo>::const_iterator iter = m_mapAnalogInfo.find(nRow);
    if (iter == m_mapAnalogInfo.end())
    {
        return QVariant();
    }

    char buf[M_MAX_CONTENT_LEN] = { 0 };

    switch (nCol)
    {
    case ANALOG_ID:
    {
        sprintf(buf, "%d", nRow);
        break;
    }
    case ANALOG_DESCRIBE:
    {
        sprintf(buf, "%s", m_mapAnalogInfo[nRow].s_strDescribe.c_str());
        break;
    }
    default:
        break;
    }

    return QVariant(buf);
}
