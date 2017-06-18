#pragma once

#include "datatypes.h"
#include "fesapi/fescnst.h"
#include <QDialog>
#include <memory>
#include <QDialog>
#include "ui_aoutoperdialog.h"
class CDbSvc;
class QTimer;

class CAoutOperDialog : public QDialog
{
	Q_OBJECT

public:
	CAoutOperDialog(QWidget *parent, int32u nOccNo, std::shared_ptr<CDbSvc>& pMem);
	~CAoutOperDialog();

protected:
	void InitView();
protected Q_SLOTS:
	void slot_OnBtnAutoExec();
	void RefreshView();
private:
	Ui::CAoutOperDialog ui;
	int32u m_nAoutOccNo;
	std::shared_ptr<CDbSvc>& m_pMemDB;

	QTimer* m_pTimer;
};
