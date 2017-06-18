#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QProcessEnvironment>

#if _MSC_VER >= 1600
#include <vld.h>
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
	a.setApplicationName("graph");
	a.setApplicationVersion("0.90");

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

// 	QTextCodec *codec = QTextCodec::codecForName("System");
// 	QTextCodec::setCodecForLocale(codec);

    MainWindow w;
	
	w.setWindowTitle(QStringLiteral("ͼ�α༭"));
	w.NewEmptyFile();
	w.showMaximized();

    return a.exec();
}
