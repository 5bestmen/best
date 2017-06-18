#include <QDialog>
#include "ui_fes_di_select_doubledi_dialog.h"

namespace Config
{
	class CFesData;
	class CChannelGroup;
	class CDevice;
}

class CQuoteItem;
class CFes_DI_Select_DoubleDI_DataModel;
class CFes_di_select_doubledi_dialog : public QDialog
{
	Q_OBJECT

public:
	CFes_di_select_doubledi_dialog(QWidget *parent, Config::CFesData *pFes);
	~CFes_di_select_doubledi_dialog();

	QString &GetTagName()
	{
		return m_strTagName;
	}

private:
	void InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup);
	void InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp);

private slots:
	void DoubleClicked(const QModelIndex &index);
	void OnOk();
	void OnCancel();

public:
	QString m_strTagName;

private:
	Ui::fes_di_select_doubledi_dialog ui;
	CFes_DI_Select_DoubleDI_DataModel *m_pModel;
};
