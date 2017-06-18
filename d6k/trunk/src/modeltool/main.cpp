#include "modeltool.h"
#include <QtWidgets/QApplication>

#include <QUuid>  



int main(int argc, char *argv[])
{
    //{5b99dc59 - f2f4 - 4726 - a2a9 - 83de7edb697e}
    /*QUuid id = QUuid::createUuid();
    QString str("设备1");
    id = QUuid::createUuidV3(id, str);
    QString strId = id.toString();*/

	QApplication a(argc, argv);
	modeltool w;
	w.setWindowTitle(QStringLiteral("模型工具"));
    
	w.show();
	return a.exec();
}
