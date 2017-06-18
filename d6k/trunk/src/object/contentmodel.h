
#ifndef CONTENT_MODEL_H
#define CONTENT_MODEL_H

#include <QAbstractTableModel>

#include "datainfo.h"


//QString m_strName;                  // 名称
//QString m_strDiscribe;              // 描述
//int m_nType;                        // 类型
//bool m_bRWAttr;                     // 读写属性
//QString m_strRelationPoint;         // 关联测点

enum
{
    ID,
    NAME,
    DESCRIBE,
    TYPE,
    READ_WRITE,
    RELATION_POINT,
};

class CContentModel : public QAbstractTableModel
{
public:
    explicit CContentModel(CObjectItem& modelInfo, QObject* parent = 0);
    ~CContentModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

public:

    bool ModifyContentData(const QModelIndex& index, const QVariant& value, int& role);

    void SetHeadTitles(QStringList headList);

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
    CObjectItem& m_modelInfo;

private:
    // 表头
    QStringList m_headList;
};

#endif // CONTENT_MODEL_H