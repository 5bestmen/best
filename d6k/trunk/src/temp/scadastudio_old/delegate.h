#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include "define.h"

//利用QComboBox委托对输入进行限制 
class CComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec);
	~CComboBoxDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QVector<IDDESC> m_itemVec;
};



//利用QLineEdit委托和正则表达式对输入进行限制  
class CLineEditDelegate : public QItemDelegate  
{  
	Q_OBJECT  

public:  
	CLineEditDelegate(QObject *parent = 0, QString fieldType = 0, int precsion = 0, int scale = 0);

private:
	int m_nType;
	int m_nPrecsion;
	int m_nScale;

	QString m_fieldType;

	int m_fieldWidth;

public:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,  
		const QModelIndex &index) const;  
 
	void setEditorData(QWidget *editor, const QModelIndex &index) const;  

	void setModelData(QWidget *editor, QAbstractItemModel *model,  
		const QModelIndex &index) const ; 
 
	void updateEditorGeometry(QWidget *editor,  
		const QStyleOptionViewItem &option, const QModelIndex &index) const; 

};

class CCheckBoxDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	CCheckBoxDelegate(QObject *parent = 0) : QItemDelegate(parent) {}
	void CCheckBoxDelegate::paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	bool CCheckBoxDelegate::editorEvent(QEvent *event,
		QAbstractItemModel *model,
		const QStyleOptionViewItem &option,
		const QModelIndex &index);
};

//只读委托  
//这个方法我还真想不到，呵呵  
class CReadOnlyDelegate : public QItemDelegate  
{  
	Q_OBJECT  
public:  
	CReadOnlyDelegate(QObject *parent = 0): QItemDelegate(parent) { }  
	QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,  
		const QModelIndex &index) const  
	{  
		return NULL;  
	}  
};

class CIPDevicePortDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	CIPDevicePortDelegate(QObject *parent = 0): QItemDelegate(parent){}
};
#endif // DELEGATE_H
