#include "cfestester.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CFesTester w;
	w.show();
	return a.exec();
}
