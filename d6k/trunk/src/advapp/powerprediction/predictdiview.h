#ifndef PREDICT_DI_VIEW_H
#define PREDICT_DI_VIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"
#include "powerpredictconfig.h"
#include "predictditable.h"

class IMainModuleInterface;


class CPredictDiView : public QMainWindow, public IBase
{
	Q_OBJECT

public:

	CPredictDiView(QWidget* parent, CPredictDevice* pDeviceData, IMainModuleInterface* pCore);
	~CPredictDiView();

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

	CPredictDiTable* m_pTable;


};


#endif  //PREDICT_DI_VIEW_H

