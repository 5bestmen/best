#ifndef CSORTFILTERMODEL_H
#define CSORTFILTERMODEL_H

#include <QObject>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <qcolor.h>
#include "tablemodel.h"

class CSortFilterModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	CSortFilterModel(QObject *parent = 0);
	~CSortFilterModel();

	bool lessThan ( const QModelIndex & left,  const QModelIndex & right ) const;
};

#endif // CSORTFILTERMODEL_H
