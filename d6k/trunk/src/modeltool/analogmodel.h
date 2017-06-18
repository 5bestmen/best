#ifndef _ANALOG_MODEL_H_
#define _ANALOG_MODEL_H_

#include <vector>

#include <QDockWidget>
#include <QGridLayout>
#include <QAbstractTableModel>

#include "modeldefine.h"
#include "equipmentmodel.h"

enum
{
    ANALOG_ID,
    ANALOG_DESCRIBE,
    ANALOG_TYPE,
    ANALOG_READ_WRITE,
};

class CAnalogModel : public QAbstractTableModel
{
public:
    explicit CAnalogModel(QMap<int, CContentInfo>& mapFileInfo, QObject* parent = 0);
    ~CAnalogModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:

    bool ModifyAnalogData(const QModelIndex& index, const QVariant& value, int& role);

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

    // 显示遥测数据
    QVariant ShowAnalog(int nCol, int nRow) const;

public:

    // 遥测信息
    QMap<int, CContentInfo>& m_mapAnalogInfo;

private:
    // 表头
    QStringList m_headList;
};

#endif // _CONTENT_H_
