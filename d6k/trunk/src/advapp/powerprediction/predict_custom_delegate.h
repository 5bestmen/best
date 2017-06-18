#ifndef  PREDICT_CUSTOM_DELEGATE_H
#define  PREDICT_CUSTOM_DELEGATE_H

#include "predict_define.h"

#include <QItemDelegate>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QApplication>
#include <QDialog>


class CPredictSelctPointDelegate : public QItemDelegate
{
	Q_OBJECT

public:

    CPredictSelctPointDelegate(QObject* parent = 0) : QItemDelegate(parent){}

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		Q_UNUSED(option);
		Q_UNUSED(index);

		QLineEdit *editor = new QLineEdit(parent);
		QRegExp regExp("[0-9]{0,10}");
		editor->setValidator(new QRegExpValidator(regExp,parent));
		return editor;

	}

	void setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		Q_UNUSED(index);

		QString text = index.model()->data(index, Qt::EditRole).toString();
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		lineEdit->setText(text);
	}

	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{ 
		Q_UNUSED(index);
		QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
		QString text = lineEdit->text();
		model->setData(index, text, Qt::EditRole);
	}

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		Q_UNUSED(index);
		editor->setGeometry(option.rect);
	}
};



class ReadOnlyDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	ReadOnlyDelegate(QObject* parent = 0) : QItemDelegate(parent){}

	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		Q_UNUSED(parent); 

		Q_UNUSED(option);
		Q_UNUSED(index);

		return nullptr;
	}

};


#endif   //PREDICT_CUSTOM_DELEGATE_H
