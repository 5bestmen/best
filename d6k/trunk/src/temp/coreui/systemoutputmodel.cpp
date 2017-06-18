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
#include <QDebug>
#include <QTextCodec>

CSystemOutputModel::CSystemOutputModel(QObject *parent, int rowCount, int colCount)
	: QAbstractTableModel(parent)
{
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	this->m_rowCount = rowCount;
	this->m_colCount = colCount;
}
CSystemOutputModel::~CSystemOutputModel()
{
	ClearAll();
}
int	CSystemOutputModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return m_colCount;
}

int CSystemOutputModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_rowCount;
}

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

		if (nCol == 0)
		{
			return tr(pItem->m_strTime.c_str());
		}
		else if (nCol == 1)
		{
			return tr(pItem->m_strTag.c_str());
		}
		else if (nCol == 2)
		{
			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
			//return strMsg;
			return pItem->m_nMsg;
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

		if (nCol == 0)
		{
			return tr(pItem->m_strTime.c_str());
		}
		else if (nCol == 1)
		{
			return tr(pItem->m_strTag.c_str());
		}
		else if (nCol == 2)
		{
			//QString strMsg = QString::fromLocal8Bit((char*)(pItem->m_strMsg).c_str());
			//return strMsg;
			return pItem->m_nMsg;
		}

		return QVariant();
	}

	return QVariant();
}

int  CSystemOutputModel::AddItem(int nType, std::string strTag, QColor color)
{

	
	int nRow = m_items.size();

	if (nRow >= m_rowCount)
		ClearAll();


	CDataItem* pItem = new CDataItem(nType, strTag, color);

	m_items.push_back(pItem);

	beginResetModel();

	endResetModel();

	return m_items.size();
}

void CSystemOutputModel::ClearAll()
{
	std::vector <CDataItem*>::iterator iter = m_items.begin();
	for (; iter != m_items.end();)
	{
		CDataItem* pItem = *iter;
		if (pItem == NULL)
			continue;

		delete pItem;
		pItem = NULL;

		iter = m_items.erase(iter);
	}
}
