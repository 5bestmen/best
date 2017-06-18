
#include "targetname_delegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QComboBox>

#include "objectdefine.h"
#include "choosetargetname.h"

CTargetNameButtonDelegate::CTargetNameButtonDelegate(QObject *parent) : QItemDelegate(parent)
{
}

void CTargetNameButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton* button = m_btns.value(index);
    if (!button)
    {
        button = new QStyleOptionButton();
        button->rect = option.rect.adjusted(3, 3, -3, -3);
        button->text = TARGETNAME_CHOOSE;
        button->state |= QStyle::State_Enabled;

        (const_cast<CTargetNameButtonDelegate*>(this))->m_btns.insert(index, button);
    }
    painter->save();

    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());

    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);


}

bool CTargetNameButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {

        QMouseEvent* e = (QMouseEvent*)event;

        if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index))
        {
            m_btns.value(index)->state |= QStyle::State_Sunken;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e = (QMouseEvent*)event;

        if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index))
        {
            m_btns.value(index)->state &= (~QStyle::State_Sunken);

            /*QDialog *d = new QDialog();

            QComboBox* pTargetNameCB = new QComboBox();

            d->setGeometry(0, 0, 200, 200);
            d->move(QApplication::desktop()->screenGeometry().center() - d->rect().center());

            d->show();*/

            CChooseTargetName targetNameDlg;

            int nExecRes = targetNameDlg.exec();
            if (nExecRes == QDialog::Rejected)
            {
                return true;
            }
        }
    }

    return true;
}
