#ifndef PREDICT_STATION_WIDGET_H
#define PREDICT_STATION_WIDGET_H

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"

#include "predict_datainfo.h"
#include "predict_station_table.h"

#include <QMainWindow>

class IMainModuleInterface;

class CPredictStationView : public QMainWindow, public IBase
{
	Q_OBJECT

public:

	CPredictStationView(QWidget* parent, CPlantInfo* pStnDatas, IMainModuleInterface* pCore);

	~CPredictStationView();

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

	CPredictStationTable* m_pTable;

	CPredictStationModel* m_pPrdtStnModel;

	// tab¿Ø¼þ
	CTabWidget *m_pTableWidget; 
};

#endif   // PREDICT_STATION_WIDGET_H