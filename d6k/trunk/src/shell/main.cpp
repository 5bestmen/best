#include "shell.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

int main(int argc,char * argv[]) 
{

	QApplication a(argc, argv);

	QCoreApplication::setApplicationName("loader.exe");
	QCoreApplication::setApplicationVersion("1.0.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("loader helper");
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addPositionalArgument("dll", QApplication::translate("main", "source dll to load"));
	parser.addPositionalArgument("proj", QCoreApplication::translate("main", "absolute project path & name"));
	parser.addPositionalArgument("node", QCoreApplication::translate("main", "node id"));
	parser.addPositionalArgument("channel", QCoreApplication::translate("main", "channel id"));


	QCommandLineOption dllOption("dll", "source dll to load", ".");
	parser.addOption(dllOption);

	QCommandLineOption projOption("proj", "absolute project path & name", ".");
	parser.addOption(projOption);

	QCommandLineOption nodeOption("node", "node id", ".");
	parser.addOption(nodeOption);

	QCommandLineOption channelOption("channel", "channel id", ".");
	parser.addOption(channelOption);

	parser.process(a);

	const QStringList args = parser.positionalArguments();

	qDebug() << parser.value(dllOption);
	qDebug() << parser.value(projOption);
	qDebug() << parser.value(nodeOption);
	qDebug() << parser.values(channelOption);



	CShell m_shell;

	m_shell.SetDllName(parser.value(dllOption).toStdString());

	m_shell.SetProjName(parser.value(projOption).toStdString());

	m_shell.SetNodeOccNo(parser.value(nodeOption).toInt());

	m_shell.SetOccNo(parser.value(channelOption).toInt());


	m_shell.Run();

	m_shell.Close();

	return a.exec();
}



