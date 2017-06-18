#ifndef _FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_DIALOG_H
#define _FES_DO_BLoCKINGSIGNALTAGNAME_SELECT_DIALOG_H

#include <QDialog>
#include "ui_fesDo_BlockingSignalTag_Select_Dialog.h"

namespace Config
{
	class CFesData;
	class CChannelGroup;
	class CDevice;
	class CUserVariableGroup;
}

class CQuoteItem;
class CFes_Do_BlockingSignalTag_Select_DeviceDI_Model;
class CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model;

class CFesDo_BlockingSignalTag_Select_Dialog : public QDialog
{
	Q_OBJECT

public:
	CFesDo_BlockingSignalTag_Select_Dialog(QWidget *parent,  Config::CFesData *pFes);
	~CFesDo_BlockingSignalTag_Select_Dialog();

	QString &GetTagName()
	{
		return m_strTagName;
	}

private:
	void InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup);
	void InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp);

	void InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitUserVariableGroup(CQuoteItem *pItem, Config::CUserVariableGroup *pGroup);

	private slots:
	void DoubleClicked(const QModelIndex &index);
	void OnOk();
	void OnCancel();

private:
	Ui::CFesDo_BlockingSignalTag_Select_Dialog ui;
	CFes_Do_BlockingSignalTag_Select_DeviceDI_Model *m_pDeviceDIModel;
	CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model *m_pFesUserVaraibleModel;
	QString m_strTagName;
};

#endif // !1
