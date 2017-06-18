#ifndef STANDARDVIEW_H
#define STANDARDVIEW_H

#include <QMainWindow>
#include <QPushButton>
#include "tableview.h"
#include "scadastudio/base.h"

class IMainWindow;

class CStandardView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CStandardView(IMainWindow *pUi, QString strTagName, QString descName, QString strTableName, int nType);
	~CStandardView();

	void closeEvent(QCloseEvent * event);

	virtual void Save()
	{

	}

public slots:
	void OnOk();

	void OnDelete();

private:
	CTableView *m_pTalbeView;

	//CDbapplitonapi *m_pDbi;

	QPushButton *m_pOkButton;
	QPushButton *m_pCancelButton;
	QPushButton *m_pDeleteButton;

	IMainWindow *m_pUi;
};

#endif // STANDARDVIEW_H
