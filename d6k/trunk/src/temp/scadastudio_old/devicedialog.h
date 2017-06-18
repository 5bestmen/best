#ifndef DEVICEDIALOG_H
#define DEVICEDIALOG_H

#include <QMainWindow>
#include "ui_devicedialog.h"
#include "base.h"

class CTableView;
class CScadastudio;

class CDeviceDialog : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CDeviceDialog(QWidget *parent, CScadastudio *pUi, QString strDesc, QString strTableName, int nType);
	~CDeviceDialog();

	void Init();

	void closeEvent(QCloseEvent * event);

	virtual void Save()
	{

	}

private:


private:
	Ui::CDeviceDialog ui;

	QTabWidget *m_pTabDevice;
	QTabWidget *m_pTabCenter;

	CTableView *m_pAIView;
	CTableView *m_pDIView;
	CTableView *m_pAOView;
	CTableView *m_pDOView;

	CScadastudio *m_pUI;
};

#endif // DEVICEDIALOG_H
