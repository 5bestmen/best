#include "channelparaconfigdialog.h"

CChannelParaConfigDialog::CChannelParaConfigDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	setWindowFlags(flags);
}

CChannelParaConfigDialog::~CChannelParaConfigDialog()
{

}
