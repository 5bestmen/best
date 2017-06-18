#ifndef PREDICT_PARAM_VIEW_H
#define PREDICT_PARAM_VIEW_H


#include <QMainWindow>
#include "scadastudio/base.h"
//#include "powerpredictconfig.h"
#include "predictparamtable.h"
#include "predictdatainfo.h"

class IMainModuleInterface;


class CPredictParamView : public QMainWindow, public IBase
{
	Q_OBJECT

public:

	CPredictParamView(QWidget* parent, CPredictDevice* pDeviceData, IMainModuleInterface* pCore);
	~CPredictParamView();

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

private:

	CPredictParamTable* m_pTable;


};








#endif  //PREDICT_PARAM_VIEW_H