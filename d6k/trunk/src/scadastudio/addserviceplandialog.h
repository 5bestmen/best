#ifndef ADDSERVICEPLANDIALOG_H
#define ADDSERVICEPLANDIALOG_H

#include <QDialog>
#include "ui_addserviceplandialog.h"

namespace Config
{
	class CNodeConfigData;
	struct NODE_SERVICE_ROLE;
	class CNodeStandardTemplateServiceRole;
}

class CAddServicePlanDialog : public QDialog
{
	Q_OBJECT

public:
	CAddServicePlanDialog(QWidget *parent, Config::CNodeConfigData *pConfig);
	~CAddServicePlanDialog();

	Config::CNodeStandardTemplateServiceRole *GetTemplate()
	{
		return m_pTemplate;
	}

private slots:
	void ClickedTable(const QModelIndex &index);
	void OnOk();
	void OnCancel();
private:
	Ui::CAddServicePlanDialog ui;
	Config::CNodeConfigData *m_pConfig;
	Config::CNodeStandardTemplateServiceRole *m_pTemplate;
};

#endif