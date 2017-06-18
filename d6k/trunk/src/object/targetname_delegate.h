#ifndef TARGETNAME_DELEGATE_H
#define TARGETNAME_DELEGATE_H

#include <QItemDelegate>

class CTargetNameButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CTargetNameButtonDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

    public slots :

private:
    QMap<QModelIndex, QStyleOptionButton*> m_btns;

};

#endif // TARGETNAME_DELEGATE_H