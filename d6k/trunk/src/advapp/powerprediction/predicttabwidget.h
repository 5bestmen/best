#ifndef PREDICT_WIDGET_H
#define PREDICT_WIDGET_H

#include <QObject>
#include <QTableView>
#include <QWidget>

#include "../../../include/scadastudio/base.h"
#include "../../../include/scadastudio/tabwidget.h"
#include "../../../include/scadastudio/icore.h"
#include "predictdatainfo.h"
#include "predictmodel.h"


class CPredictView : public QTableView, public IBase
{
	Q_OBJECT

public:
	CPredictView(CPredictItem* pItem);
	~CPredictView();

public:
	virtual void Save();
	virtual void Refresh();
	virtual void SetModifyFlag(bool bFlag);
	virtual bool GetModifyFlag();

	CPredictItem* GetPdtItem() const;

private:
	CPredictItem* m_pItem;

};

class CPredictTabWidget : public QWidget 
{
	Q_OBJECT

public:
	CPredictTabWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPredictTabWidget();

public:
	void AddToTable(CPredictItem* pItem);
	void ReleaseData();
	void SetTabViewHead();

private:
	CPredictModel* m_pPredictModel;
	CTabWidget* m_pTabWidget;
	IMainModuleInterface *m_pCore;

};

#endif  //PREDICT_WIDGET_H