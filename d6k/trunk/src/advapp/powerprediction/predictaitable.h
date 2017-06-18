#ifndef PREDICT_AI_TABLE_H
#define PREDICT_AI_TABLE_H

#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QIcon>

#include "powerpredictconfig.h"
#include "predictsortmodel.h"
#include "predictaimodel.h"

class CSortFilterModel;
class IMainModuleInterface;
class CPredictAiModel;

class CPredictAiTable : public QTableView
{
	Q_OBJECT

public:

	CPredictAiTable(QWidget* parent, std::vector <CAiInput *> &vecAi, IMainModuleInterface* pCore);
	~CPredictAiTable();

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
	CPredictAiModel* m_pModel;
	IMainModuleInterface* m_pCore;


};

#endif  //PREDICT_AI_TABLE_H
