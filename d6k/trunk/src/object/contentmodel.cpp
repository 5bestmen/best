
#include "contentmodel.h"

#include "objectdefine.h"


CContentModel::CContentModel(CObjectItem& modelInfo, QObject* parent /*= 0*/) : m_modelInfo(modelInfo), QAbstractTableModel(parent)
{

}

CContentModel::~CContentModel()
{

}

/*virtual */
int CContentModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    Q_UNUSED(parent);

    return m_headList.size();
}

/*virtual */
int CContentModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    Q_UNUSED(parent);

    return m_modelInfo.m_vecTableInfo.size();
}

/*virtual */
QVariant CContentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) // 水平表头  
        {
            int nSize = m_headList.size();

            if (nSize > 0)
            {
                return m_headList[section];
            }
        }
        else
        {
            return QString(" ");
        }
    }

    return QVariant();
}

/*virtual */
QVariant CContentModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
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
    default:
        break;
    }
    return QVariant();
}

/*virtual */
Qt::ItemFlags CContentModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() <= READ_WRITE)
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
bool CContentModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
    return true;
}

bool CContentModel::ModifyContentData(const QModelIndex& index, const QVariant& value, int& role)
{
    return true;
}

void CContentModel::SetHeadTitles(QStringList headList)
{
    m_headList = headList;
}

#define M_MAX_CONTENT_LEN   256 // 内容最大长度
QVariant CContentModel::ShowContent(int nCol, int nRow) const
{
    char buf[M_MAX_CONTENT_LEN] = { 0 };

    switch (nCol)
    {
    case ID:
    {
        sprintf(buf, "%d", nRow);
        break;
    }
    case NAME:
    {
        sprintf(buf, "%s", m_modelInfo.m_vecTableInfo[nRow].m_strName.toStdString().c_str());
        break;
    }
    case DESCRIBE:
    {
        sprintf(buf, "%s", m_modelInfo.m_vecTableInfo[nRow].m_strDiscribe.toStdString().c_str());
        break;
    }
    case TYPE:
    {
        sprintf(buf, "%d", m_modelInfo.m_vecTableInfo[nRow].m_nType);
        break;
    }
    case READ_WRITE:
    {
        return m_modelInfo.m_vecTableInfo[nRow].m_bRWAttr ? READ_ATTRIBUTE : WRITE_ATTRIBUTE;
        break;
    }
    case RELATION_POINT:
    {
        sprintf(buf, "%s", m_modelInfo.m_vecTableInfo[nRow].m_strRelationPoint.toStdString().c_str());
        break;
    }
    default:
        break;
    }

    return QVariant(buf);
}


