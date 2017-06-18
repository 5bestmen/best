#ifndef CHANNELPARACONFIGDIALOG_H
#define CHANNELPARACONFIGDIALOG_H

#include <QDialog>
#include "ui_channelparaconfigdialog.h"

class CChannelParaConfigDialog : public QDialog
{
	Q_OBJECT

public:
	CChannelParaConfigDialog(QWidget *parent = 0);
	~CChannelParaConfigDialog();

private:
	Ui::CChannelParaConfigDialog ui;
};

#endif // CHANNELPARACONFIGDIALOG_H
