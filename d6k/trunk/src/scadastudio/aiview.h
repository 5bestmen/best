#ifndef AIVIEW_H
#define AIVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"

class IMainModuleInterface;
namespace Config
{
	class CDevice;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CAITable;

class CAIView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CAIView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
		CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFes);
	~CAIView();

	virtual void Save()
	{

	}
	virtual void Refresh();

	//module file
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
	CAITable *m_pTable;
};

#endif // AIVIEW_H
