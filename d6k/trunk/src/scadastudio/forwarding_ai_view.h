#ifndef FORWADING_AIVIEW_H
#define FORWADING_AIVIEW_H

#include <QMainWindow>
#include "scadastudio/base.h"

class IMainModuleInterface;
namespace Config
{
	class CForwardingChannelData;
	class CFesData;
}
class CQuoteItem;
class CChannel;
class CForwadingAITable;

class CForwardingAIView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CForwardingAIView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingAIView();

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
	CForwadingAITable *m_pTable;
};

#endif // FORWADING_AIVIEW_H
