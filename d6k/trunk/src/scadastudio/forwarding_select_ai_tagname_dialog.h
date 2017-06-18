#ifndef FORWARDING_SELECT_AI_TAGNAME_DIALOG_H
#define FORWARDING_SELECT_AI_TAGNAME_DIALOG_H

#include <QDialog>
#include "ui_forwarding_select_ai_tagname_dialog.h"

namespace Config
{
	class CFesData;
	class CChannelGroup;
	class CDevice;
}

class CQuoteItem;
class CForwardingAISelectDataModel;

class CForwarding_select_ai_tagname_dialog : public QDialog
{
	Q_OBJECT

public:
	CForwarding_select_ai_tagname_dialog(QWidget *parent, Config::CFesData *pFes);
	~CForwarding_select_ai_tagname_dialog();

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
	Ui::CForwarding_select_ai_tagname_dialog ui;
	CForwardingAISelectDataModel *m_pModel;

};

#endif
