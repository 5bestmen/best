/*! @file
<PRE>
********************************************************************************
模块名       :	排序model
文件名       :  sortfiltermodel.cpp
文件实现功能 :  排序model
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

#include "sortfiltermodel.h"

/*! \fn CSortFilterModel::CSortFilterModel(QObject *parent)
********************************************************************************************************* 
** \brief CSortFilterModel::CSortFilterModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CSortFilterModel::CSortFilterModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{

}

/*! \fn CSortFilterModel::~CSortFilterModel()
********************************************************************************************************* 
** \brief CSortFilterModel::~CSortFilterModel 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CSortFilterModel::~CSortFilterModel()
{

}

/*! \fn bool CSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
********************************************************************************************************* 
** \brief CSortFilterModel::lessThan 
** \details 排序
** \param left 
** \param right 
** \return bool 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
bool CSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	bool ok, ok1;

	QString strLeft = left.data(Qt::EditRole).toString();

	QString strRight =right.data(Qt::EditRole).toString();

	double fltL = left.data(Qt::EditRole).toDouble(&ok);

	double fltR = right.data(Qt::EditRole).toDouble(&ok1);

	Qt::SortOrder g_sortOrder = sortOrder();

		if(ok & ok1)
		{
			return fltL < fltR;
		}
		else
		{	
			int nTmp = strLeft.localeAwareCompare(strRight);
			
			if(nTmp < 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}


	return true;
}