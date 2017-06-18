#ifndef ADDSERVICEDIALOG_H
#define ADDSERVICEDIALOG_H

#include <QDialog>
#include "ui_addservicedialog.h"

namespace Config
{
	class CNodeConfigData;
	class CNode;
	struct NODE_SERVICE_ROLE;
}

class CAddServiceDialog : public QDialog
{
	Q_OBJECT

public:
	CAddServiceDialog(QWidget *parent, Config::CNodeConfigData *pConfig, Config::CNode *pNode);
	~CAddServiceDialog();

	std::vector<Config::NODE_SERVICE_ROLE *> &GetArr();

private slots:
	void OnOk();
	void OnCancel();
	void SelectTableItem(const QModelIndex &);

private:
	Ui::CAddServiceDialog ui;
	Config::CNodeConfigData *m_pConfig;
	std::vector<Config::NODE_SERVICE_ROLE *> m_vec;
	std::vector<Config::NODE_SERVICE_ROLE *> m_vecReturn;
};

#endif
