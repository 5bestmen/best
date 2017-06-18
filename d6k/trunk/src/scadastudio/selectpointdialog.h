#ifndef SELECTPOINTDIALOG_H
#define SELECTPOINTDIALOG_H

#include <QDialog>
#include "ui_selectpointdialog.h"

namespace Config
{
	class CFesConfigData;
	class CFesData;
	class CDevice;
	class CFesGroup;
	class CChannelGroup;
	class CUserVariableGroup;
}

class QStandardItemModel;
class CQuoteItem;
class QSortFilterProxyModel;
class CFesSelectPointModel;
class CTagAttMgr;

class CSelectPointDialog : public QDialog
{
	Q_OBJECT

	enum
	{
		NOSELECT = -1
	};
public:
	CSelectPointDialog(QWidget *parent, Config::CFesConfigData *pConfig, 
		const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName);
	~CSelectPointDialog();

	void closeEvent(QCloseEvent *)
	{
		return;
		this->deleteLater();
	}

private slots:
	void DoubleClicked(const QModelIndex &index);
	void SelectPoint(const QModelIndex &index);
	void OnOk();
	void OnCancel();
	void FilterChanged(QString text);
	void filterColumnChanged(int);

	void showMouseRightButton(const QPoint&);

	void reject();
private:
	void InitUi();
	void CreateFesUi(Config::CFesConfigData *pConfig);
	void CreateFesGroupUi(Config::CFesGroup *pFesGroup, CQuoteItem *pItem);
	void InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup);
	//根据前置文件添加装置
	void InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp);

	void InitVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	void InitSystemVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	void InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData);
	bool InitUserVariableGroup(CQuoteItem *pItem, Config::CUserVariableGroup *pGroup);

	void InitAttrCombo(int nType);
private:
	Ui::CSelectPointDialog ui;
	Config::CFesConfigData *m_pConfig;
	QStandardItemModel *m_pModel;
	CFesSelectPointModel *m_pFesSelectPointModel;
	QSortFilterProxyModel *m_pSort;

	bool m_bCloseDialogFlag;

	const std::vector<int>& m_arrAllowDataTypes;
	bool m_bFilterEnWrite;
	std::string & m_szNodeTagName;
	std::string &m_szTagName;
	std::string &m_szFiledName;

	int m_nType;

	CTagAttMgr *m_pTagAtt;
};

#endif // SELECTPOINTDIALOG_H
