#ifndef DOUTOPERDIALOG_H
#define DOUTOPERDIALOG_H
#include "datatypes.h"
#include "fesapi/fescnst.h"
#include "ui_doutoperdialog.h"
#include <QDialog>
#include <memory>
class CDbSvc;
class QTimer;

class CDoutOperDialog : public QDialog 
{
	Q_OBJECT
public:
	CDoutOperDialog(QWidget *parent ,int32u nOccNo  , std::shared_ptr<CDbSvc>& pMem);
	~CDoutOperDialog();
protected:
	void InitView();	
protected Q_SLOTS:
	void RefreshView();
	void slot_on_BtnSelect();
	void slot_on_BtnPre();
	void slot_on_BtnExec();
	void slot_on_AutoExec();
private:
	Ui::CDoutOperDialog ui;
	int32u m_nDoutOccNo;
	std::shared_ptr<CDbSvc>& m_pMemDB;
	QTimer* m_pTimer;
};

#endif // DOUTOPERDIALOG_H
