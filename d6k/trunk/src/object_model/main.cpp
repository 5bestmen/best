#include "equipmentmodel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CModelFrame w;
    
	w.show();
	return a.exec();
}
