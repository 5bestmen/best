#include "predictsortmodel.h"

CPredictSortProxyModel::CPredictSortProxyModel(QObject* parent /* = 0 */)
	: QSortFilterProxyModel(parent)
{



}

CPredictSortProxyModel::~CPredictSortProxyModel()
{


}

bool CPredictSortProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	bool ok, ok1;

	QString strLeft = left.data(Qt::EditRole).toString();

	QString strRight = right.data(Qt::EditRole).toString();

	double fltL = left.data(Qt::EditRole).toDouble(&ok);

	double fltR = right.data(Qt::EditRole).toDouble(&ok1);

	//Qt::SortOrder g_sortOrder = sortOrder();

	if (ok & ok1)
	{
		return fltL < fltR;
	}
	else
	{
		int nTmp = strLeft.localeAwareCompare(strRight);

		if (nTmp < 0)
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