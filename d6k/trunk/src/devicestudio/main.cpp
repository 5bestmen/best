

#include <QApplication>
#include <QTranslator>
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "main_module_impl.h"
#include <QFile>
#include <QDebug>

//#ifdef ENABLE_VLD
#if _MSC_VER >= 1600
#define VLD_FORCE_ENABLE
// #include "vld.h"
#endif
//#endif // ENABLE_VLD

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//	int *p = new int[10];

	QTranslator qtTranslator;
	QString strRunPath = qApp->applicationDirPath();

	if (qtTranslator.load(strRunPath + "/locales/devicestudio-zh-CN.qm"))
	{
		a.installTranslator(&qtTranslator);
	}



	QString qss;

	QFile qssFile(":/devcese.qss");
	qssFile.open(QFile::ReadOnly);
	if (qssFile.isOpen())
	{
		qss = qssFile.readAll();
		qApp->setStyleSheet(qss);

		qssFile.close();
	}
	

	IMainModule *pMainModule = new CMainModule(nullptr);

	IMainWindow *pMainWindow = pMainModule->GetMainWindow();
	Q_ASSERT(pMainWindow);
	if (pMainWindow)
	{
		pMainWindow->show();
	}

	return a.exec();
}
