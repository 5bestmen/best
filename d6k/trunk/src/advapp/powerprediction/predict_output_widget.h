#ifndef PREDICT_OUTPUT_WIDGET_H
#define PREDICT_OUTPUT_WIDGET_H

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"

#include "predict_datainfo.h"
#include "predict_output_table.h"

#include <QMainWindow>

class IMainModuleInterface;

class CPredictOutputView : public QMainWindow, public IBase
{
	Q_OBJECT

public:

	CPredictOutputView(QWidget* parent, CWeatherData* pWeaDatas, IMainModuleInterface* pCore);

	~CPredictOutputView();

	virtual void Save()
	{

	}

	virtual void Refresh()
	{

	}

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return m_bModify;
	}

	virtual void Delete();

public:

	//void ShowWidget(CStationData* pStnDatas);

private:

	CPredictOutputTable* m_pTable;

	CPredictOutputModel* m_pPrdtOutputModel;

	// tab¿Ø¼þ
	CTabWidget *m_pTableWidget;
};

#endif   // PREDICT_WEATHER_WIDGET_H
