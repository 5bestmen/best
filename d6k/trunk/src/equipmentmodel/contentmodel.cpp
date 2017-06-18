
#include "contentmodel.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>

bool g_bModifyFlag = false;

CModelToolModel::CModelToolModel(CModelInfo& modelInfo, QObject* parent) : m_modelInfo(modelInfo), QAbstractTableModel(parent)
{

}

CModelToolModel::~CModelToolModel()
{

}

/*virtual */
int CModelToolModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_modelInfo.m_mapContentInfo.size();
}

/*virtual */
int CModelToolModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_headList.size();
}

/*virtual */
QVariant CModelToolModel::headerData(int section, Qt::Orientation orientation, int role) const
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
QVariant CModelToolModel::data(const QModelIndex &index, int role) const
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
        return ShowContent(nCol, nRow);

        break;
    }
    case Qt::CheckStateRole:
    {
        if (nCol == READ_WRITE)
        {
            return m_modelInfo.m_mapContentInfo[nRow].m_bReadWrite ? Qt::Checked : Qt::Unchecked;
        }

        break;
    }
    case  Qt::EditRole:
    {
        if (nCol == TYPE)
        {
            return QVariant(m_modelInfo.m_mapContentInfo[nRow].m_nDataType);
        }

        break;
    }
    default:
        break;
    }
    return QVariant();
}

/*virtual */
Qt::ItemFlags CModelToolModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() == ID)
    {
        return QAbstractItemModel::flags(index);
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    int nCol = index.column();
    if (nCol == READ_WRITE)
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
bool CModelToolModel::setData(const QModelIndex &index, const QVariant &value, int role/* = Qt::EditRole*/)
{
    int nRow = index.row();
    int nCol = index.column();
    //if (index.isValid() && index.row() != index.column() && role == Qt::EditRole)
    if (!index.isValid() || index.column() == 0)
    {
        return false;
    }

    return ModifyContentData(index, value, role);
}

bool CModelToolModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
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
        if (nCol == READ_WRITE)
        {
            m_modelInfo.m_mapContentInfo[nRow].m_bReadWrite = (value.toInt() == Qt::Checked);

            emit dataChanged(index, index);

            g_bModifyFlag = true;
            return true;
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
            case NAME:
            {
                if (m_modelInfo.m_mapContentInfo[nRow].m_strName != value.toString().toStdString())
                {
                    m_modelInfo.m_mapContentInfo[nRow].m_strName = value.toString().toStdString();
                    g_bModifyFlag = true;
                }

                break;
            }
            case DESCRIBE:
            {
                if (m_modelInfo.m_mapContentInfo[nRow].m_strDescribe != value.toString().toStdString())
                {
                    m_modelInfo.m_mapContentInfo[nRow].m_strDescribe = value.toString().toStdString();
                    g_bModifyFlag = true;
                }

                break;
            }
            case TYPE:
            {
                if (m_modelInfo.m_mapContentInfo[nRow].m_nDataType != value.toInt())
                {
                    m_modelInfo.m_mapContentInfo[nRow].m_nDataType = value.toInt();
                    g_bModifyFlag = true;
                }

                break;
            }
            case READ_WRITE:
            {
                if (m_modelInfo.m_mapContentInfo[nRow].m_bReadWrite != value.toBool())
                {
                    m_modelInfo.m_mapContentInfo[nRow].m_bReadWrite = value.toBool();
                    g_bModifyFlag = true;
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

void CModelToolModel::SetHeadTitles(QStringList headList)
{
    m_headList = headList;
}

QVariant CModelToolModel::ShowContent(int nCol, int nRow) const
{
    QMap<int, CContentInfo>::const_iterator iter = m_modelInfo.m_mapContentInfo.find(nRow);
    if (iter == m_modelInfo.m_mapContentInfo.end())
    {
        return QVariant();
    }

    char buf[M_MAX_CONTENT_LEN] = { 0 };

    switch (nCol)
    {
        case ID:
        {
            sprintf(buf, "%d", nRow);
            break;
        }
        case DESCRIBE:
        {
            sprintf(buf, "%s", m_modelInfo.m_mapContentInfo[nRow].m_strDescribe.c_str());
            break;
        }
        case NAME:
        {
            sprintf(buf, "%s", m_modelInfo.m_mapContentInfo[nRow].m_strName.c_str());
            break;
        }
        default:
            break;
    }

    return QVariant(buf);
}
