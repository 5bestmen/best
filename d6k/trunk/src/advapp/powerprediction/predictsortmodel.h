#ifndef PREDICT_SORT_MODEL_H
#define PREDICT_SORT_MODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QModelIndex>

class CPredictSortProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	CPredictSortProxyModel(QObject* parent = 0);
	~CPredictSortProxyModel();

	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

};



#endif  //PREDICT_SORT_MODEL_H
