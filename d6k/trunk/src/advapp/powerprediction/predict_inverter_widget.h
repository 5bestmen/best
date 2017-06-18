#ifndef   PREDICT_INVERTER_WIDGET_H
#define   PREDICT_INVERTER_WIDGET_H

#include "scadastudio/base.h"
#include "scadastudio/tabwidget.h"

#include "predict_datainfo.h"
#include "predict_inverter_table.h"

#include <QMainWindow>

class IMainModuleInterface;

class CPredictInverterView : public QMainWindow, public IBase
{
	Q_OBJECT

public:

	CPredictInverterView(QWidget* parent, CInverterInfo* pInvterDatas, IMainModuleInterface* pCore);

	~CPredictInverterView();

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

	CPredictInverterTable* m_pTable;

	CPredictInverterModel* m_pPrdtStnModel;

	// tab¿Ø¼þ
	CTabWidget *m_pTableWidget;
};

#endif   // PREDICT_INVERTER_WIDGET_H
