#ifndef CLOSEPROJECTDIALOG_H
#define CLOSEPROJECTDIALOG_H

#include <QDialog>
#include "ui_closeprojectdialog.h"

class CCloseProjectDialog : public QDialog
{
	Q_OBJECT
public:
	enum
	{
		SaveProject, CancelProject, NotsaveProject = 666
	};
	CCloseProjectDialog(QWidget *parent = 0);
	~CCloseProjectDialog();

	int GetType()
	{
		return m_nType;
	}

public slots:
	void Notsave();
	void Save();
	void Cancel();

private:
	Ui::CCloseProjectDialog ui;
	int m_nType;
};

#endif // CLOSEPROJECTDIALOG_H
