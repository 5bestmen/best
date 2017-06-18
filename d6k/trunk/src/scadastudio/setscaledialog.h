#ifndef SETSCALEDIALOG_H
#define SETSCALEDIALOG_H

#include <QDialog>
#include "ui_setscaledialog.h"

class CScaleModel;

class CSetScaleDialog : public QDialog
{
	Q_OBJECT

public:
	CSetScaleDialog(QWidget *parent = 0, CScaleModel *pModel = NULL);
	~CSetScaleDialog();

	void InitPara(CScaleModel *pModel);

	void SetType(int nType)
	{
		m_nType = nType;
	}

	int GetType()
	{
		return m_nType;
	}

public slots:
	void OnOk();

	void OnCancel();

	void OnAdd();

	void OnDele();

	void OnModify();
private:
	Ui::CSetScaleDialog ui;

	int m_nType;

	CScaleModel *m_pModel;
};

#endif // SETSCALEDIALOG_H
