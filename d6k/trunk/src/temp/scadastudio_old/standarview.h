#ifndef STANDARDVIEW_H
#define STANDARDVIEW_H

#include <QMainWindow>
#include <QPushButton>
#include "tableview.h"
#include "base.h"


class CStandardView : public QMainWindow, public IBase
{
	Q_OBJECT

public:
	CStandardView(QWidget *parent, QString strDesc, QString strTableName, int nType);
	~CStandardView();

	void closeEvent(QCloseEvent * event);

	virtual void Save()
	{

	}

public slots:
	void test();

private:
	CTableView *m_pTalbeView;

	CDbapplitonapi *m_pDbi;

	QPushButton *m_pOkButton;
	QPushButton *m_pCancelButton;
	QPushButton *m_pDeleteButton;
};

#endif // STANDARDVIEW_H
