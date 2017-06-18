/*! @file
<PRE>
********************************************************************************
ģ����       :	����model
�ļ���       :  sortfiltermodel.cpp
�ļ�ʵ�ֹ��� :  ����model
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \date 2015��11��17�� 
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
** \date 2015��11��17�� 
** \note 
********************************************************************************************************/
CSortFilterModel::~CSortFilterModel()
{

}

/*! \fn bool CSortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
********************************************************************************************************* 
** \brief CSortFilterModel::lessThan 
** \details ����
** \param left 
** \param right 
** \return bool 
** \author gw
** \date 2015��11��17�� 
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