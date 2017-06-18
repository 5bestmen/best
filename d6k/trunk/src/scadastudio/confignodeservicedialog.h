#ifndef CONFIGNODESERVICEDIALOG_H
#define CONFIGNODESERVICEDIALOG_H

#include <QDialog>
#include "ui_confignodeserviceDialog.h"

namespace Config
{
	class CNodeConfigData;
	class CNode;
}

class CConfigNodeServiceTableModel;

class CConfigNodeServiceDialog : public QDialog
{
	Q_OBJECT

public:
	CConfigNodeServiceDialog(QWidget *parent, Config::CNodeConfigData *pConfig, Config::CNode *pNode);
	~CConfigNodeServiceDialog();

private slots:
	void ClickedTable(const QModelIndex &index);
	void ClickedAdd();
	void ClickedDelete();
	void ClickedOnOk();
	void ClickedOnCancel();
	void ClickedOnAddPlan();

private:
	Ui::CConfigNodeServiceDialog ui;
	Config::CNodeConfigData *m_pConfig;
	Config::CNode *m_pNode;
	CConfigNodeServiceTableModel *m_pModel;
};

#endif
