#ifndef SCALEVIEW_H
#define SCALEVIEW_H

#include <QMainWindow>
//#include "config_data.h"
#include "scadastudio/base.h"

class CQuoteItem;
class CScale;
class CScaleView;
class CScaleTable;
namespace Config
{
	class CTransform;
	class CFesData;
}
class CTransform;

class CScaleView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScaleView(QWidget *parent, Config::CTransform *pTransform, CQuoteItem *pItem, CScale *pScaleModel
		, Config::CFesData *pFesConfig);
	~CScaleView();

	virtual void Save();

	virtual void Refresh();

	virtual void SetModifyFlag(bool bFlag)
	{
		Q_UNUSED(bFlag);
	}

	virtual bool GetModifyFlag()
	{
		return true;
	}

	virtual void Delete();

private:
	CScaleTable *m_pTable;
};

#endif // SCALEVIEW_H
