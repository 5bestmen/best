#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include "scadastudiodefine.h"

//利用QComboBox委托对输入进行限制 
class CComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec, bool bEdit = false);
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
	bool m_bEdit;
};

//利用QComboBox委托对输入进行限制 
class CDriveParaComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CDriveParaComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec101, 
		QVector<IDDESC> &vecModbus, QVector<IDDESC> &vec104);
	~CDriveParaComboBoxDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QVector<IDDESC> m_itemModbusVec;
	QVector<IDDESC> m_item101Vec;
	QVector<IDDESC> m_item104Vec;

	mutable QVector<IDDESC>	m_itemVec;
};


//利用QLineEdit委托和正则表达式对输入进行限制  
class CLineEditDelegate : public QItemDelegate  
{  
	Q_OBJECT  

public:  
	CLineEditDelegate(QObject *parent = 0, uint nType = 0, uint nCount = 0);

private:
	uint m_nCount;

	uint m_nType;

public:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,  
		const QModelIndex &index) const;  
 
	void setEditorData(QWidget *editor, const QModelIndex &index) const;  

	void setModelData(QWidget *editor, QAbstractItemModel *model,  
		const QModelIndex &index) const ; 
 
	void updateEditorGeometry(QWidget *editor,  
		const QStyleOptionViewItem &option, const QModelIndex &index) const; 

};

#include <QToolButton>
#include <QPainter>
#include <qstyle>
class CButtonEdit;
class CButtonEditDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CButtonEditDelegate(QObject *parent = 0);
	~CButtonEditDelegate();

private:


public:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;

	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;

	//void paint(QPainter *painter, const QStyleOptionViewItem &option,
	//	const QModelIndex &index) const;

	//bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

	CButtonEdit *m_btn;

	QStyle::StateFlag _state;
};

class CCheckBoxDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	CCheckBoxDelegate(QObject *parent = 0, bool bEdit = true) : QItemDelegate(parent) 
	{
		m_bEdit = bEdit;
	}
	void CCheckBoxDelegate::paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	bool CCheckBoxDelegate::editorEvent(QEvent *event,
		QAbstractItemModel *model,
		const QStyleOptionViewItem &option,
		const QModelIndex &index);
private:
	bool m_bEdit;
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
		Q_UNUSED(parent);
		Q_UNUSED(option);
		Q_UNUSED(index);

		return NULL;  
	}  
};

class CIPDevicePortDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	CIPDevicePortDelegate(QObject *parent = 0): QItemDelegate(parent){}
};

//利用QLineEdit委托和正则表达式对输入进行限制  
class CInputEditDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CInputEditDelegate(QObject *parent, const QString &strReg);

private:
	QString m_strReg;

public:
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;

	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;

};
#endif // DELEGATE_H
