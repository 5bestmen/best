#include "mb_exchange.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CMb_Exchange w;
	w.show();
	return a.exec();
}
