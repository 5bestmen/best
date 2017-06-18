
#include "delegate.h"

#include "modeldefine.h"

#include <QApplication>

CComboxDelegate::CComboxDelegate(QVector<IDDESC>& vecIdDesc, QObject *parent /*= 0*/) : m_vecAnalogCombo(vecIdDesc), QItemDelegate(parent)
{
    
}

CComboxDelegate::~CComboxDelegate()
{

}

/* virtual */
void CComboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString strValue = index.model()->data(index, Qt::EditRole).toString().toUpper();
    QComboBox* pCombobox = static_cast<QComboBox*>(editor);

    QVector<IDDESC>::const_iterator ite = m_vecAnalogCombo.constBegin();
    for (; ite != m_vecAnalogCombo.constEnd(); ite++)
    {
        QString strID = ite->ID;
        if (strID.compare(strValue, Qt::CaseInsensitive) == 0)
        {
            strValue = ite->desc;

            pCombobox->setEditText(strValue);

            break;
        }
    }

    pCombobox->setEditable(false);

    pCombobox->setCurrentIndex(pCombobox->findText(strValue));
}

/* virtual */
void CComboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString strDescribe = comboBox->currentText();

    if (strDescribe.isEmpty())
    {
        return;
    }

    QVector<IDDESC>::const_iterator ite = m_vecAnalogCombo.constBegin();

    QString text;

    for (; ite != m_vecAnalogCombo.constEnd(); ite++)
    {
        QString desc = ite->desc;
        if (desc.compare(strDescribe, Qt::CaseInsensitive) == 0)
        {
            text = ite->ID;

            break;
        }
    }

    model->setData(index, text, Qt::EditRole);
}

/* virtual */
QWidget* CComboxDelegate::createEditor(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QComboBox *pComboBox = static_cast<QComboBox*>(editor);
    QComboBox* pComboBox = new QComboBox(editor);
    for (unsigned int i = 0; i < m_vecAnalogCombo.size(); ++i)
    {
        pComboBox->addItem(m_vecAnalogCombo[i].desc);
    }

    pComboBox->installEventFilter(const_cast<CComboxDelegate*>(this));
    return pComboBox;
}

/* virtual */
void CComboxDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{

    /*QComboBox* pComboBox = static_cast<QComboBox*>(editor);
    if (nullptr != pComboBox)
    {
    delete pComboBox;
    pComboBox = nullptr;
    }*/
}

/* virtual */
void CComboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

/* virtual */
void CComboxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 myOption = option;
    //QString text = Items[index.row()];

    QString strText = index.model()->data(index, Qt::EditRole).toString();

    myOption.text = strText;

    QVector<IDDESC>::const_iterator ite = m_vecAnalogCombo.constBegin();
    for (; ite != m_vecAnalogCombo.constEnd(); ite++)
    {
        QString ID = ite->ID;
        if (ID.compare(strText, Qt::CaseInsensitive) == 0)
        {
            myOption.text = ite->desc;

            break;
        }
    }

    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
