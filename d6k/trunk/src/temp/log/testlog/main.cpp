#include "testlog.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

void SetDefaultCodec()
{
	/*
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");

	//设置和对本地文件系统读写时候的默认编码格式
	QTextCodec::setCodecForLocale(codec);

	//设置传给tr函数时的默认字符串编码
	QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
	//QTextCodec::setCodecForTr(codec);

	//用在字符常量或者QByteArray构造QString对象时使用的一种编码方式
	QTextCodec::setCodecForCStrings(codec);
	*/
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
}

int main(int argc, char *argv[])
{
	SetDefaultCodec();

	QApplication a(argc, argv);
	testlog w;

	if(!w.Initialize())
		return 0;

	w.show();
	
	return a.exec();
}
