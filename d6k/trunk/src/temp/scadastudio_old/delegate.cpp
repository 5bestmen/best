/*! @file
<PRE>
********************************************************************************
模块名       :	代理
文件名       :	delegate.cpp
文件实现功能 :  代理
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/

#include <QComboBox>
#include <QApplication>
#include <QLineEdit>
#include <QKeyEvent>
#include "define.h"
#include "delegate.h"


/*! \fn CComboBoxDelegate::CComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec)
********************************************************************************************************* 
** \brief CComboBoxDelegate::CComboBoxDelegate 
** \details 
** \param parent 
** \param vec 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CComboBoxDelegate::CComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec)
	:QItemDelegate(parent)
{
	m_itemVec = vec;
}

/*! \fn CComboBoxDelegate::~CComboBoxDelegate()
********************************************************************************************************* 
** \brief CComboBoxDelegate::~CComboBoxDelegate 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CComboBoxDelegate::~CComboBoxDelegate()
{

}

/*! \fn QWidget * CComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
********************************************************************************************************* 
** \brief CComboBoxDelegate::createEditor 
** \details 
** \param parent 
** \param option 
** \param index 
** \return QWidget * 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
QWidget * CComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{	
	QComboBox* editor = new QComboBox(parent);
	for(unsigned int i = 0; i < m_itemVec.size(); ++i)
	{
		editor->addItem(m_itemVec[i].desc);
	}
	return editor;
}

/*! \fn void CComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
********************************************************************************************************* 
** \brief CComboBoxDelegate::setEditorData 
** \details 
** \param editor 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{	
	QString strValue = index.model()->data(index, Qt::EditRole).toString().toUpper();  
	QComboBox *pCombobox = static_cast<QComboBox*>(editor); 

	//根据value找key
	////////////////////////////////////////////////////////////////////////////
	////Modified by gw in 20141106
	////QString value = txtValueVec.value(text);

	QVector<IDDESC>::const_iterator ite = m_itemVec.constBegin();
	for(; ite != m_itemVec.constEnd(); ite++)
	{
		QString strID = ite->ID;
		if(strID.compare(strValue, Qt::CaseInsensitive) == 0)
		{
			strValue = ite->desc;

			pCombobox->setEditText(strValue);

			break;
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	//int tindex = comboBox->findText(value);

	pCombobox->setEditable(false);

	pCombobox->setCurrentIndex(pCombobox->findText(strValue));
}

/*! \fn void CComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
********************************************************************************************************* 
** \brief CComboBoxDelegate::setModelData 
** \details 
** \param editor 
** \param model 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *pComboBox = static_cast<QComboBox*>(editor);  
	QString strDescribe = pComboBox->currentText();

	if(strDescribe.isEmpty())
	{
		return;
	}

	QVector<IDDESC>::const_iterator ite = m_itemVec.constBegin();
	
	QString text;

	for(; ite != m_itemVec.constEnd(); ite++)
	{
		QString desc = ite->desc;
		if(desc.compare(strDescribe, Qt::CaseInsensitive) == 0)
		{
			text = ite->ID;

			break;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	model->setData(index, text, Qt::EditRole); 
}

/*! \fn void CComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
********************************************************************************************************* 
** \brief CComboBoxDelegate::updateEditorGeometry 
** \details 
** \param editor 
** \param option 
** \param & 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

/*! \fn void CComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
********************************************************************************************************* 
** \brief CComboBoxDelegate::paint 
** \details 
** \param painter 
** \param option 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 myOption = option;
	//QString text = Items[index.row()];

	QString strText = index.model()->data(index, Qt::EditRole).toString();


	//////////////////////////////////////////////////////////////////////////////////
	//Modified by gw in 20141106
	//if(txtValueVec.find(text) != txtValueVec.end())
	//{
	//	//text是key
	//	QString value = txtValueVec.value(text);
	//	myOption.text = value;
	//}
	//else
	//{
	//	myOption.text = text;
	//}

	myOption.text = strText;

	QVector<IDDESC>::const_iterator ite = m_itemVec.constBegin();
	for(; ite != m_itemVec.constEnd(); ite++)
	{
		QString ID = ite->ID;
		if(ID.compare(strText, Qt::CaseInsensitive) == 0)
		{
			myOption.text = ite->desc;

			break;
		}
	}


	///////////////////////////////////////////////////////////////////////////////////

	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}

/*! \fn CLineEditDelegate::CLineEditDelegate(QObject *parent, QString fieldType, int precsion, int scale)
********************************************************************************************************* 
** \brief CLineEditDelegate::CLineEditDelegate 
** \details 
** \param parent 
** \param fieldType 
** \param precsion 
** \param scale 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CLineEditDelegate::CLineEditDelegate(QObject *parent /* = 0 */, QString fieldType /* = 0 */, int precsion /* = 0 */, int scale /* = 0): QItemDelegate(parent */)
	:QItemDelegate(parent)
{
	m_fieldType = fieldType;

	m_fieldWidth = precsion;
}


/*! \fn QWidget  *CLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,  
const QModelIndex &index) const
********************************************************************************************************* 
** \brief CLineEditDelegate::createEditor 
** \details 
** \param parent 
** \param option 
** \param index 
** \return QWidget * 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
QWidget  *CLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,  
	const QModelIndex &index) const  
{
	QLineEdit *editor = new QLineEdit(parent);  
	QRegExp regExp/*("[0-9]{0,10}")*/;

	QString strPattern;

	//bool bTmp = m_fieldType.contains(UNINT);
	
	if(m_fieldType.contains(UNCHAR))
	{
		strPattern = QString("^([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$");
	}
	else if(m_fieldType.contains(UNSHORT))
	{
		strPattern = QString("^([0-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1-9][0-9][0-9][0-9]|([1-5][0-9][0-9][0-9][0-9])|(6[0-4][0-9][0-9][0-9])|(65[0-4][0-9][0-9])|(655[0-2][0-9])|(6553[0-5]))$");
	}
	else if(m_fieldType.contains(UNINT))
	{
		strPattern = QString("^([0-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1-9][0-9][0-9][0-9]|[1-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
[1-3][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
(4[0-1][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9])|\
(42[0-8][0-9][0-9][0-9][0-9][0-9][0-9][0-9])|\
(429[0-3][0-9][0-9][0-9][0-9][0-9][0-9])|\
(4294[0-8][0-9][0-9][0-9][0-9][0-9])|\
(42949[0-5][0-9][0-9][0-9][0-9])|\
(429496[0-6][0-9][0-9][0-9])|\
(4294967[0-1][0-9][0-9])|\
(42949672[0-8][0-9])|\
(429496729[0-5]))$");
	}
	else if(m_fieldType.contains(MYSTRING))
	{
		strPattern = QString("^[\\s\\S]{0,%1}$").arg(m_fieldWidth);
	}
	else if(m_fieldType.compare(MYFLOAT) == COMPARE_TRUE)
	{
		//strPattern = QString("^[+-]?\\d{1,7}(?:\\.\\d{1,3}$|$)");
	
		strPattern = QString("[+-]?\\d{1,7}\\.\\d{0,3}|\\d{1,7}");
	}
	else if(m_fieldType.compare(MYIPPORT) == COMPARE_TRUE)
	{
		strPattern = QString(MYIPPORTREG);
	}
	else if(m_fieldType.compare(MYCOM) == COMPARE_TRUE)
	{
		strPattern = QString(MYCOMREG);
	}

	regExp.setPattern(strPattern);

	editor->setValidator(new QRegExpValidator(regExp, parent));  

	QString strTmp = index.data().toString();

	return editor;  
}

/*! \fn void CLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
********************************************************************************************************* 
** \brief CLineEditDelegate::setEditorData 
** \details 
** \param editor 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const 
{
	QString text = index.model()->data(index, Qt::EditRole).toString();

	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);  
	
	lineEdit->setText(text);
}

/*! \fn void CLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,  
const QModelIndex &index) const
********************************************************************************************************* 
** \brief CLineEditDelegate::setModelData 
** \details 
** \param editor 
** \param model 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,  
	const QModelIndex &index) const  
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);  
	
	QString text = lineEdit->text();

	model->setData(index, text, Qt::EditRole);  
}

/*! \fn void CLineEditDelegate::updateEditorGeometry(QWidget *editor,  
const QStyleOptionViewItem &option, const QModelIndex &index) const
********************************************************************************************************* 
** \brief CLineEditDelegate::updateEditorGeometry 
** \details 
** \param editor 
** \param option 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CLineEditDelegate::updateEditorGeometry(QWidget *editor,  
	const QStyleOptionViewItem &option, const QModelIndex &index) const  
{  
	editor->setGeometry(option.rect);
}

////////////////////////////////////////////////////////  //////////////////////////////////////////////////////////////
/*! \fn static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options)
********************************************************************************************************* 
** \brief CheckBoxRect 
** \details 
** \param view_item_style_options 
** \return QT_NAMESPACE::QRect 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options) 
{
	QStyleOptionButton check_box_style_option;
	QRect check_box_rect = QApplication::style()->subElementRect(
		QStyle::SE_CheckBoxIndicator,
		&check_box_style_option);
	QPoint check_box_point(view_item_style_options.rect.x() +
		view_item_style_options.rect.width() / 2 -
		check_box_rect.width() / 2,
		view_item_style_options.rect.y() +
		view_item_style_options.rect.height() / 2 -
		check_box_rect.height() / 2);
	return QRect(check_box_point, check_box_rect.size());
}

/*! \fn void CCheckBoxDelegate::paint(QPainter *painter,
const QStyleOptionViewItem &option,
const QModelIndex &index) const
********************************************************************************************************* 
** \brief CCheckBoxDelegate::paint 
** \details 
** \param painter 
** \param option 
** \param index 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CCheckBoxDelegate::paint(QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const 
{
	int checked = index.model()->data(index, Qt::DisplayRole).toInt();
	QString str = index.model()->data(index, Qt::EditRole).toString();
	QStyleOptionButton check_box_style_option;
	check_box_style_option.state |= QStyle::State_Enabled;
	if (checked) {
		check_box_style_option.state |= QStyle::State_On;
	} else {
		check_box_style_option.state |= QStyle::State_Off;
	}
	check_box_style_option.rect = CheckBoxRect(option);

	QApplication::style()->drawControl(QStyle::CE_CheckBox,
		&check_box_style_option,
		painter);
}

/*! \fn bool CCheckBoxDelegate::editorEvent(QEvent *event,
QAbstractItemModel *model,
const QStyleOptionViewItem &option,
const QModelIndex &index)
********************************************************************************************************* 
** \brief CCheckBoxDelegate::editorEvent 
** \details 
** \param event 
** \param model 
** \param option 
** \param index 
** \return bool 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
bool CCheckBoxDelegate::editorEvent(QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) 
{
		if ((event->type() == QEvent::MouseButtonRelease) ||
			(event->type() == QEvent::MouseButtonDblClick)) {
				QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
				if (mouse_event->button() != Qt::LeftButton ||
					!CheckBoxRect(option).contains(mouse_event->pos())) {
						return false;
				}
				if (event->type() == QEvent::MouseButtonDblClick) {
					return true;
				}
		} 
		else if (event->type() == QEvent::KeyPress)
		{
			if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
				static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select) 
			{
					return false;
			}
		}
		else
		{
			return false;
		}

		int checked = index.model()->data(index, Qt::DisplayRole).toInt();
		if(checked)
		{
			return model->setData(index, "0", Qt::EditRole);
		}
		else
		{
			return model->setData(index, "1", Qt::EditRole);
		}
}