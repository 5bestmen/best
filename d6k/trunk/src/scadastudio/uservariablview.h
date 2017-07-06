#ifndef USERVARIABLVIEW_H
#define USERVARIABLVIEW_H

#include <QMainWindow>
//#include "ui_uservariablview.h"
#include "scadastudio/base.h"

class IMainModuleInterface;

namespace Config
{
	class CUserVariableGroup;
	class CFesData;
}

class CUserVariableTable;
class QGridLayout;
class QLabel;
class QComboBox;
class QSpacerItem;
class QLineEdit;

class CUserVariablView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CUserVariablView(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData, Config::CUserVariableGroup *pVariable);
	~CUserVariablView();

	virtual void Save()
	{

	}

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

	void setupUi(QMainWindow *CUserVariablView);

private slots:
	void FilterChanged(QString text);
	void filterColumnChanged(int);

private:
	//Ui::CUserVariablView ui;

	CUserVariableTable *m_pTable;

	QWidget *centralWidget;
	QGridLayout *gridLayout;
	QLabel *label_2;
	QComboBox *comboBox;
	QLabel *label;
	QLineEdit *lineEdit;
	//CUserVariableTable *tableView;
	QSpacerItem *horizontalSpacer_2;
	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	IMainModuleInterface *m_pCore;
	Config::CUserVariableGroup *m_pVariable;
};

#endif // USERVARIABLVIEW_H
