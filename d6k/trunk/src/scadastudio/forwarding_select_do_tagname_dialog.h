#pragma once

#include <QDialog>
#include "ui_forwarding_select_do_tagname_dialog.h"

namespace Config
{
	class CFesData;
	class CChannelGroup;
	class CDevice;
}

class CQuoteItem;
class CForwardingDOSelectDataModel;

class CForwarding_select_do_tagname_dialog : public QDialog
{
	Q_OBJECT
		
public:
	CForwarding_select_do_tagname_dialog(QWidget *parent, Config::CFesData *pFes);
	~CForwarding_select_do_tagname_dialog();

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
	Ui::CForwarding_select_do_tagname_dialog ui;
	CForwardingDOSelectDataModel *m_pModel;
};
