#ifndef CHANNELDIALOG_H
#define CHANNELDIALOG_H

#include "tableview.h"
#include <QMainWindow>

class CChannelDialog : public QMainWindow
{
	Q_OBJECT

public:
	CChannelDialog(QWidget *parent, QString strName);
	~CChannelDialog();

private:
	CTableView *m_pTalbeView;

	CDbapplitonapi *m_pDbi;
};

#endif // CHANNELDIALOG_H
