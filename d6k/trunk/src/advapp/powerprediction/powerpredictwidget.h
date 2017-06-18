#ifndef POWER_PREDICT_WIDGET_H
#define POWER_PREDICT_WIDGET_H


#include <QTableView>
#include <QWidget>

#include "scadastudio/tabwidget.h"
#include "scadastudio/icore.h"
#include "scadastudio/base.h"
#include "powerpredictdatainfo.h"


class CPowerPredictView : public QTableView, public IBase
{
	Q_OBJECT

public:

	CPowerPredictView(CPointItem *pPowerPredictGroup);
	~CPowerPredictView();

public:
	virtual void Save();

	virtual void Refresh();

	virtual void SetModifyFlag(bool bFlag);

	virtual bool GetModifyFlag();

	CPointItem*  GetPointItem() const;

private:
	CPointItem*  m_pPointItem;


};




class CPowerPredictWidget : public QWidget
{
public:
	CPowerPredictWidget(CTabWidget* pTableWidget, IMainModuleInterface* pCore);
	~CPowerPredictWidget();

public:


private:







};
























#endif  //POWER_PREDICT_WIDGET_H