#ifndef _CONTENT_MODEL_H_
#define _CONTENT_MODEL_H_

#include <vector>

#include <QDockWidget>
#include <QGridLayout>
#include <QAbstractTableModel>

#include "modeldefine.h"
#include "modelinfo.h"

enum
{
    ID,
    NAME,
    DESCRIBE,
    TYPE,
    READ_WRITE,
};

class CModelToolModel : public QAbstractTableModel
{
public:
    explicit CModelToolModel(CModelInfo& modelInfo, QObject* parent = 0);
    ~CModelToolModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:

    bool ModifyContentData(const QModelIndex& index, const QVariant& value, int& role);

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
    QVariant ShowContent(int nCol, int nRow) const;

public:

    // 遥测信息
    CModelInfo& m_modelInfo;

private:
    // 表头
    QStringList m_headList;
};

#endif // _CONTENT_MODEL_H_
