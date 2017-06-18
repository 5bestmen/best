#ifndef _BINARY_MODEL_H_
#define _BINARY_MODEL_H_

#include <vector>

#include <QDockWidget>
#include <QGridLayout>
#include <QAbstractTableModel>

#include "modeldefine.h"
#include "equipmentmodel.h"

enum
{
    BINARY_ID,
    BINARY_DESCRIBE,
    BINARY_TYPE,
    BINARY_READ_WRITE,
};

class CBinaryModel : public QAbstractTableModel
{
public:
    explicit CBinaryModel(QMap<int, CContentInfo>& mapFileInfo, QObject* parent = 0);
    ~CBinaryModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:

    bool ModifyBinaryData(const QModelIndex& index, const QVariant& value, int& role);

    void SetHeadTitles(QStringList headList);

    int offsetOf(int row, int column) const
    {
        if (row < column)
            qSwap(row, column);
        return (row * (row - 1) / 2) + column;
    }

    void RefrushModel()
    {
        beginResetModel();
        endResetModel();
    }

private:

    // 显示遥信数据
    QVariant ShowBinary(int nCol, int nRow) const;

public:
    // 数据
    std::vector<int> m_vecDataID;

    // 遥信信息
    QMap<int, CContentInfo>& m_mapBinaryInfo;

private:
    // 表头
    QStringList m_headList;
};

#endif // _CONTENT_H_
