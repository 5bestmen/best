#include "vmc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	vmc w;
	w.show();
	return a.exec();
}
