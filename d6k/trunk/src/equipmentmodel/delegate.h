
#ifndef _MODEL_TOOL_DELEGATE_H_
#define _MODEL_TOOL_DELEGATE_H_

#include <QComboBox>
#include <QItemDelegate>

#include "modeldefine.h"

class CComboxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    CComboxDelegate(QVector<ID_DESC>&vecIdDesc, QObject *parent = 0);
    ~CComboxDelegate();

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void destroyEditor(QWidget *editor, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QComboBox* m_pComboBox;
    QVector<ID_DESC>& m_vecAnalogCombo;
};

#endif