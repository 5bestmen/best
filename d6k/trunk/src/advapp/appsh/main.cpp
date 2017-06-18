#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



	CShDialog w;
    w.show();

    return a.exec();
}
