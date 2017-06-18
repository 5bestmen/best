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

#include "scadastudiodefine.h"
#include "delegate.h"
#include "channelmodel.h"
#include "scadastudio/define.h"
#include "button_edit.h"

#include <QComboBox>
#include <QApplication>
#include <QLineEdit>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QTableView>






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
CComboBoxDelegate::CComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec, bool bEdit)
	:QItemDelegate(parent), m_bEdit(false)
{
	m_itemVec.clear();
	m_itemVec = vec;
	m_bEdit = bEdit;
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
	Q_UNUSED(option);
	Q_UNUSED(index);

	QComboBox* editor = new QComboBox(parent);
	for(int i = 0; i < m_itemVec.size(); ++i)
	{
		editor->addItem(m_itemVec[i].desc);
	}

	editor->setEditable(m_bEdit);

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
	QString strValue = index.model()->data(index, Qt::EditRole).toString();  
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

	pCombobox->setEditable(m_bEdit);

	pCombobox->setCurrentText(strValue);
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
	
	QString text = pComboBox->currentText();

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
CDriveParaComboBoxDelegate::CDriveParaComboBoxDelegate(QObject *parent, QVector<IDDESC> &vec101, QVector<IDDESC> &vecModbus, QVector<IDDESC> &vec104)
	:QItemDelegate(parent)
{
	m_itemModbusVec = vecModbus;
	m_item101Vec = vec101;
	m_item104Vec = vec104;
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
CDriveParaComboBoxDelegate::~CDriveParaComboBoxDelegate()
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
QWidget * CDriveParaComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(option);
	
	QComboBox* editor = new QComboBox(parent);
	
	CChannelModel *pTmp = (CChannelModel *)index.model();

	QString strTmp = pTmp->index(index.row(), index.column() - 1).data(Qt::EditRole).toString();
	
	if (strTmp.compare("modbus", Qt::CaseInsensitive) == 0)
	{
		m_itemVec = m_itemModbusVec;
	}
	else if (strTmp.compare("101", Qt::CaseInsensitive) == 0)
	{
		m_itemVec = m_item101Vec;
	}
	else if (strTmp.compare("104", Qt::CaseInsensitive) == 0)
	{
		m_itemVec = m_item104Vec;
	}

	for (int i = 0; i < m_itemVec.size(); ++i)
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
void CDriveParaComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString strValue = index.model()->data(index, Qt::EditRole).toString();
	QComboBox *pCombobox = static_cast<QComboBox*>(editor);

	//根据value找key
	////////////////////////////////////////////////////////////////////////////
	////Modified by gw in 20141106
	////QString value = txtValueVec.value(text);

	QVector<IDDESC>::const_iterator ite = m_itemVec.constBegin();
	for (; ite != m_itemVec.constEnd(); ite++)
	{
		QString strID = ite->ID;
		if (strID.compare(strValue, Qt::CaseInsensitive) == 0)
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
void CDriveParaComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *pComboBox = static_cast<QComboBox*>(editor);
	QString strDescribe = pComboBox->currentText();

	if (strDescribe.isEmpty())
	{
		return;
	}

	QVector<IDDESC>::const_iterator ite = m_itemVec.constBegin();

	QString text;

	for (; ite != m_itemVec.constEnd(); ite++)
	{
		QString desc = ite->desc;
		if (desc.compare(strDescribe, Qt::CaseInsensitive) == 0)
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
void CDriveParaComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
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
void CDriveParaComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 myOption = option;
	//QString text = Items[index.row()];

	QString strText = index.model()->data(index, Qt::EditRole).toString();

	QString strDriverName = index.model()->index(index.row(), index.column() - 1).data(Qt::EditRole).toString();

	m_itemVec.clear();

	if (strDriverName.compare("modbus", Qt::CaseInsensitive) == 0)
	{
		m_itemVec = m_itemModbusVec;
	}
	else if (strDriverName.compare("101", Qt::CaseInsensitive) == 0)
	{
		m_itemVec = m_item101Vec;
	}
	else if (strDriverName.compare("104", Qt::CaseInsensitive) == 0)
	{
		m_itemVec = m_item104Vec;
	}
	else
	{
		m_itemVec.clear();
		strText.clear();
	}



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

	myOption.text.clear();

	QVector<IDDESC>::const_iterator ite = m_itemVec.constBegin();
	for (; ite != m_itemVec.constEnd(); ite++)
	{
		QString ID = ite->ID;
		if (ID.compare(strText, Qt::CaseInsensitive) == 0)
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
CLineEditDelegate::CLineEditDelegate(QObject *parent, uint nType, uint nCount)
	:QItemDelegate(parent)
{
	m_nCount = nCount;

	m_nType = nType;
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
#include <QDoubleSpinBox>
QWidget  *CLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,  
	const QModelIndex &index) const  
{
	Q_UNUSED(option);
	
	QLineEdit *editor = new QLineEdit(parent);  
	QRegExp regExp/*("[0-9]{0,10}")*/;

	QString strPattern;

	//bool bTmp = m_fieldType.contains(UNINT);
	
//	if(m_fieldType.contains(UNCHAR))
//	{
//		strPattern = QString("^([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$");
//	}
//	else if(m_fieldType.contains(UNSHORT))
//	{
//		strPattern = QString("^([0-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1-9][0-9][0-9][0-9]|([1-5][0-9][0-9][0-9][0-9])|(6[0-4][0-9][0-9][0-9])|(65[0-4][0-9][0-9])|(655[0-2][0-9])|(6553[0-5]))$");
//	}
//	else if(m_fieldType.contains(UNINT))
//	{
//		strPattern = QString("^([0-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1-9][0-9][0-9][0-9]|[1-9][0-9][0-9][0-9][0-9]|\
//[1-9][0-9][0-9][0-9][0-9][0-9]|\
//[1-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
//[1-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
//[1-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
//[1-3][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
//(4[0-1][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9])|\
//(42[0-8][0-9][0-9][0-9][0-9][0-9][0-9][0-9])|\
//(429[0-3][0-9][0-9][0-9][0-9][0-9][0-9])|\
//(4294[0-8][0-9][0-9][0-9][0-9][0-9])|\
//(42949[0-5][0-9][0-9][0-9][0-9])|\
//(429496[0-6][0-9][0-9][0-9])|\
//(4294967[0-1][0-9][0-9])|\
//(42949672[0-8][0-9])|\
//(429496729[0-5]))$");
//	}
	if(m_nType == LINESTRING)
	{
		strPattern = QString("^[\\s\\S]{0,%1}$").arg(m_nCount);
	}
	else if (m_nType == DOUBLETYPE)
	{
		//strPattern = QString("^[+-]?\\d{1,7}(?:\\.\\d{1,3}$|$)");
	
		strPattern = QString("[+-]?\\d{1,10}\\.\\d{0,%1}|\\d{1,10}").arg(m_nCount);
	}
	else if (m_nType == DEVICEADDRESS)
	{
		strPattern = QString(MYIPPORTREG);
	}
	else if (m_nType == IPADDRESS)
	{
		strPattern = QString(MYIPREG);
	}
	else if (m_nType == TAGNAMELINE)
	{
		strPattern = QString(TAGNAMEREG).arg(m_nCount);
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
#include "sortfiltermodel.h"
void CLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,  
	const QModelIndex &index) const  
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);  
	
	QString text = lineEdit->text();

	model->setData(index, text);

	//model->setData(index, text, Qt::EditRole);  
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
	Q_UNUSED(index);
	
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

		//不可以修改
		if (!m_bEdit)
		{
			return true;
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

CInputEditDelegate::CInputEditDelegate(QObject *parent, const QString &strReg)
	:QItemDelegate(parent), m_strReg("")
{
	m_strReg = strReg;
}


/*! \fn CButtonEditDelegate::CButtonEditDelegate(QObject *parent = 0)
********************************************************************************************************* 
** \brief CButtonEditDelegate::CButtonEditDelegate 
** \details button edit delegate
** \param parent 
** \return  
** \author gw
** \date 2016年10月13日 
** \note 
********************************************************************************************************/
CButtonEditDelegate::CButtonEditDelegate(QObject *parent)
	:QItemDelegate(parent)
{
	_state = QStyle::State_None;
}

CButtonEditDelegate::~CButtonEditDelegate()
{

}

QWidget *CButtonEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	
	auto m_btn = new CButtonEdit(parent);
	return m_btn;
}

void CButtonEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	//QString text = index.model()->data(index, Qt::EditRole).toString();

	//QWidget *pWidget = static_cast<QWidget*>(editor);

	Q_UNUSED(editor);
	Q_UNUSED(index);
}

void CButtonEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
	const QModelIndex &index) const
{
	QWidget *pWidget = static_cast<QWidget *>(editor);

	Q_UNUSED(pWidget);
	Q_UNUSED(model);
	Q_UNUSED(index);

	//model->setData(index, text, Qt::EditRole);  
}

void CButtonEditDelegate::updateEditorGeometry(QWidget *editor,
	const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	
	editor->setGeometry(option.rect);
}

//#include <QDialog>
//void CButtonEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
//	const QModelIndex &index) const
//{
//	QStyleOptionButton button;
//
//	// Set our button to fill the entire cell contents
//	int x, y, w, h;
//	QRect r = option.rect;//getting the rect of the cell
//	x = r.left() + r.width() - 30;//the X coordinate
//	y = r.top();//the Y coordinate
//	w = 30;//button width
//	h = 30;//button height
//	button.rect = QRect(x, y, w, h);
//	//button.rect = option.rect;
//
//	// Set our button state to enabled
//	button.state |= _state | QStyle::State_Enabled;
//
//
//	//if (button.state) {
//	//	painter->fillRect(button.rect, button.palette.highlight());
//	//}
//
//	QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
//	
//	//QStyleOptionButton button;
//	//QRect r = option.rect;//getting the rect of the cell
//	//int x, y, w, h;
//	//x = r.left() + r.width() - 30;//the X coordinate
//	//y = r.top();//the Y coordinate
//	//w = 30;//button width
//	//h = 30;//button height
//	//button.rect = QRect(x, y, w, h);
//	//button.text = "=^.^=";
//	//button.state = QStyle::State_Enabled | QStyle::State_MouseOver | QStyle::State_HasFocus |
//	//	QStyle::State_Selected | QStyle::State_Sunken;
//	//	
//
//	//QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
//
//	
//	//QStyleOptionToolButton button;
//
//	//QRect r = option.rect;//getting the rect of the cell
//	//int x, y, w, h;
//	//x = r.left() + r.width() - 30;//the X coordinate
//	//y = r.top();//the Y coordinate
//	//w = 30;//button width
//	//h = 30;//button height
//	//button.rect = QRect(x, y, w, h);
//	//button.text = "=^.^=";
//	//button.state = QStyle::State_Enabled | QStyle::State_Sunken | QStyle::State_HasFocus;
//
//	//QApplication::style()->drawControl(QStyle::CE_ToolButtonLabel, &button, painter);
//}


//bool CButtonEditDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
//{
//	//if (event->type() == QEvent::MouseButtonRelease)
//	{
//		QMouseEvent * e = (QMouseEvent *)event;
//		int clickX = e->x();
//		int clickY = e->y();
//
//		QRect r = option.rect;//getting the rect of the cell
//		int x, y, w, h;
//		x = r.left() + r.width() - 30;//the X coordinate
//		y = r.top();//the Y coordinate
//		w = 30;//button width
//		h = 30;//button height
//
//		if (clickX > x && clickX < x + w)
//		{
//			if (clickY > y && clickY < y + h)
//			{
//				if (event->type() == QEvent::MouseButtonPress)
//				{
//					_state = QStyle::State_Sunken;
//				}
//				else if (event->type() == QEvent::MouseButtonRelease) 
//				{
//					_state = QStyle::State_Raised;
//
//					QDialog * d = new QDialog();
//					d->setGeometry(0, 0, 100, 100);
//					d->show();
//				}
//			}
//		}
//	}
//
//	return true;
//}



#include <QDoubleSpinBox>
QWidget  *CInputEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	
	QLineEdit *editor = new QLineEdit(parent);
	QRegExp regExp/*("[0-9]{0,10}")*/;

	QString strPattern;

	strPattern = m_strReg;

	regExp.setPattern(strPattern);

	editor->setValidator(new QRegExpValidator(regExp, parent));

	return editor;
}

void CInputEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();

	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

	lineEdit->setText(text);
}

void CInputEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
	const QModelIndex &index) const
{
	Q_UNUSED(model);
	Q_UNUSED(index);
	Q_UNUSED(editor);

	//QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

	//QString text = lineEdit->text();


	//model->setData(index, text, Qt::EditRole);  
}

void CInputEditDelegate::updateEditorGeometry(QWidget *editor,
	const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);
	
	editor->setGeometry(option.rect);
}