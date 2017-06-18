#ifndef PREDICT_PARAM_TABLE_H
#define PREDICT_PARAM_TABLE_H

#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QIcon>

#include "powerpredictconfig.h"
#include "predictsortmodel.h"
#include "predictparammodel.h"

class CSortFilterModel;
class IMainModuleInterface;
class CPredictParamModel;

class CPredictParamTable : public QTableView
{
	Q_OBJECT

public:

	CPredictParamTable(QWidget* parent, std::vector <CParamInput *> &vecParam, IMainModuleInterface* pCore);
	~CPredictParamTable();

	public slots:

	void DoubleClicked(const QModelIndex& index);
	void ShowMouseRightButton(const QPoint& point);

signals:
	void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

private:

	void AddAnalogPoint(QModelIndex& index);
	void DeleteAnalogPoint(QModelIndex& index);

private:

	CPredictSortProxyModel* m_pPredictSortModel;
	CPredictParamModel* m_pModel;
	IMainModuleInterface* m_pCore;

};

#endif // PREDICT_PARAM_TABLE_H
