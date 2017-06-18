/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出view
文件名       :  outputmessagerserver.cpp
文件实现功能 :  系统输出view
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
*  @date    2015.11.30
*/

#include "systemoutputmodel.h"
#include "logdefine.h"
#include "uimodule/systemoutputview.h"

#include <QDebug>

/*! \fn CSystemOutputModel::CSystemOutputModel(QObject *parent, int rowCount, int colCount)
********************************************************************************************************* 
** \brief CSystemOutputModel::CSystemOutputModel 
** \details 
** \param parent 
** \param rowCount 
** \param colCount 
** \return  
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
CSystemOutputModel::CSystemOutputModel(QObject *parent, int rowCount, int colCount)
	: QAbstractTableModel(parent)
{
	Q_UNUSED(parent);
	Q_UNUSED(rowCount);
	Q_UNUSED(colCount);
	
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	this->m_rowCount = 0;
	this->m_colCount = colCount;

	m_horizontal_header_list.append(tr("date"));
	m_horizontal_header_list.append(QStringLiteral("time"));
	m_horizontal_header_list.append(QStringLiteral("desc"));
	m_horizontal_header_list.append(QStringLiteral("module"));
	m_horizontal_header_list.append(QStringLiteral("thread id"));
	m_horizontal_header_list.append(QStringLiteral("process id"));
}
/*! \fn CSystemOutputModel::~CSystemOutputModel()
********************************************************************************************************* 
** \brief CSystemOutputModel::~CSystemOutputModel 
** \details 
** \return  
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
CSystemOutputModel::~CSystemOutputModel()
{
	ClearAll();
}
/*! \fn int	CSystemOutputModel::columnCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CSystemOutputModel::columnCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
int	CSystemOutputModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return m_colCount;
}

/*! \fn int CSystemOutputModel::rowCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CSystemOutputModel::rowCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
int CSystemOutputModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return (int)m_items.size();
}

/*! \fn QVariant CSystemOutputModel::data(const QModelIndex &index, int role) const
********************************************************************************************************* 
** \brief CSystemOutputModel::data 
** \details 
** \param index 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
QVariant CSystemOutputModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) { return QVariant(); }


	if (Qt::BackgroundColorRole == role)
	{
		//return  QVariant(QColor(Qt::black));
	}

	/*
	if(Qt::FontRole == role)
	{
	return QVariant(CConfig::Instance().GetUIFont());
	}
	*/

	//if (Qt::TextColorRole == role)
	//{
	//	int nRow = index.row();
	//	int nCol = index.column();

	//	if (nCol == 2 || nCol == 1)
	//	{
	//		if (nRow >= m_items.size() || nRow<0)
	//			return QVariant();

	//		CDataItem* pItem = m_items[nRow];

	//		return  QVariant(pItem->m_color);
	//	}
	//}

	if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_items.size() || nRow<0)
			return QVariant();

		CDataItem* pItem = m_items[nRow];

		if (nCol == Date)
		{
			return tr(pItem->m_strDate.c_str());
		}
		else if (nCol == Time)
		{
			return tr(pItem->m_strTime.c_str());
		}
		else if (nCol == Tag)
		{
			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
			//return strMsg;
			return pItem->m_strTag.c_str();
		}
		else if (nCol == Type)
		{
			return pItem->m_strType.c_str();
		}
		else if (nCol == ThreadId)
		{
			return pItem->m_nThreadID;
		}
		else if (nCol == ProcessID)
		{
			return pItem->m_nProcessID;
		}

		return QVariant();
	}
	else if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_items.size() || nRow < 0)
			return QVariant();

		CDataItem* pItem = m_items[nRow];

		if (nCol == Date)
		{
			return tr(pItem->m_strDate.c_str());
		}
		else if (nCol == Time)
		{
			return tr(pItem->m_strTime.c_str());
		}
		else if (nCol == Tag)
		{
			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
			//return strMsg;
			return pItem->m_strTag.c_str();
		}
		else if (nCol == Type)
		{
			return pItem->m_strType.c_str();
		}
		else if (nCol == ThreadId)
		{
			return pItem->m_nThreadID;
		}
		else if (nCol == ProcessID)
		{
			return pItem->m_nProcessID;
		}

		return QVariant();
	}

	return QVariant();
}

/*! \fn int  CSystemOutputModel::AddItem(MSG_LOG &log, QColor color)
********************************************************************************************************* 
** \brief CSystemOutputModel::AddItem 
** \details 添加项
** \param log 
** \param color 
** \return int 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
int  CSystemOutputModel::AddItem(const MSG_LOG &log, QColor& color)
{
	//int nRow = m_items.size();

	//if (nRow >= m_rowCount)
		//ClearAll();


	CDataItem* pItem = new CDataItem(log, color);

	int row = (int)m_items.size();
	beginResetModel();
	//beginInsertRows(QModelIndex(), row, row);

	m_items.push_back(pItem);

	endResetModel();
	//endInsertRows();

	//beginResetModel();

	//endResetModel();

	((CSystemOutputView *)this->parent())->scrollToBottom();

	return (int)m_items.size();
}

/*! \fn void CSystemOutputModel::ClearAll()
********************************************************************************************************* 
** \brief CSystemOutputModel::ClearAll 
** \details 
** \return void 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
void CSystemOutputModel::ClearAll()
{
	std::vector <CDataItem*>::iterator iter = m_items.begin();
	for (; iter != m_items.end();)
	{
		CDataItem* pItem = *iter;
		if (pItem == NULL)
		{
			++iter;
			continue;
		}


		delete pItem;
		pItem = NULL;

		iter = m_items.erase(iter);
	}
}

/*! \fn QVariant CSystemOutputModel::headerData(int section, Qt::Orientation orientation, int role) const
********************************************************************************************************* 
** \brief CSystemOutputModel::headerData 
** \details 
** \param section 
** \param orientation 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
QVariant CSystemOutputModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_horizontal_header_list.size() > section)
				return m_horizontal_header_list[section];
			else
				return section + 1;
		}
		else
		{
			return section + 1;
		}
	}

	return QVariant();
}