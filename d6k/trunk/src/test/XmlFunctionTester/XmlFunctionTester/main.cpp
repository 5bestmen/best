#include "xmlfunctiontester.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	XmlFunctionTester w;
	w.show();
	return a.exec();
}
