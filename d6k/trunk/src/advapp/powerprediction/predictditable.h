#ifndef PREDICT_DI_TABLE_H
#define PREDICT_DI_TABLE_H

#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QIcon>

#include "powerpredictconfig.h"
#include "predictsortmodel.h"
#include "predictdimodel.h"

class CSortFilterModel;
class IMainModuleInterface;
class CPredictDiModel;

class CPredictDiTable : public QTableView
{
	Q_OBJECT

public:

	CPredictDiTable(QWidget* parent, std::vector <CDiInput *> &vecDi, IMainModuleInterface* pCore);
	~CPredictDiTable();

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
	CPredictDiModel* m_pModel;
	IMainModuleInterface* m_pCore;


};

#endif  //PREDICT_DI_TABLE_H
