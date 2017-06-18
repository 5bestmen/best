#ifndef SCADAUSERVARIABLEVIEW_H
#define SCADAUSERVARIABLEVIEW_H

#include <QMainWindow>
//#include "ui_scadauservariableview.h"
#include "scadastudio/base.h"

class IMainModuleInterface;

namespace Config
{
	class CScadaVariableConfigData;
	class CScadaUserVariableGroup;
	class CScadaData;
}

class CScadaUserVariableTable;
class QGridLayout;
class QLabel;
class QComboBox;
class QSpacerItem;
class QLineEdit;

class CScadaUserVariableView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CScadaUserVariableView(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pScadaVirableConfig
		, Config::CScadaUserVariableGroup *pGroup, Config::CScadaData *pScada);
	~CScadaUserVariableView();

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

	void setupUi(QMainWindow *CScadaUserVariableView);

private slots:
	void filterColumnChanged(int column);
	void FilterChanged(QString text);
private:
	//Ui::CScadaUserVariableView ui;
	CScadaUserVariableTable *m_pTable;
	
	QWidget *centralWidget;
	QGridLayout *gridLayout;
	QLabel *label;
	QComboBox *comboBox;
	QLabel *label_2;
	QLineEdit *lineEdit;
	QSpacerItem *horizontalSpacer_2;
	QMenuBar *menuBar;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	IMainModuleInterface *m_pCore;
	Config::CScadaVariableConfigData *m_pScadaVirableConfig;
	Config::CScadaUserVariableGroup *m_pGroup;
	Config::CScadaData *m_pScada;
};

#endif // SCADAUSERVARIABLEVIEW_H
