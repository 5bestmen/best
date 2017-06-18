#ifndef PREDICT_WEATHER_WIDGET_H
#define PREDICT_WEATHER_WIDGET_H

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"

#include "predict_datainfo.h"
#include "predict_weather_table.h"

#include <QMainWindow>

class IMainModuleInterface;

class CPredictWeatherView : public QMainWindow, public IBase
{
	Q_OBJECT

public:

	CPredictWeatherView(QWidget* parent, CWeatherData* pWeaDatas, IMainModuleInterface* pCore);

	~CPredictWeatherView();

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

	CPredictWeatherTable* m_pTable;

	CPredictWeatherModel* m_pPrdtWeaModel;

	// tab¿Ø¼þ
	CTabWidget *m_pTableWidget;
};

#endif   // PREDICT_WEATHER_WIDGET_H