#ifndef COIL_DLG_H
#define COIL_DLG_H

#include <QDialog>
#include "ui_coildlg.h"
 
class CCoilDlg : public QDialog, public Ui_CCoilDlg
{
	Q_OBJECT

public:
	CCoilDlg(QWidget *parent = 0);
	//����ҳ�沼��
//	void SetWgtLayout();
//	void InitWgt(CRemotePonitTableAnalyse *pPointAnalyse);
	~CCoilDlg();
 
public slots:
 

private:
 
};

#endif // COIL_DLG_H
