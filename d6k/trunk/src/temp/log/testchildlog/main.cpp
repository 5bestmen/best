#include "testchildlog.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

void SetDefaultCodec()
{
	/*
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");

	//���úͶԱ����ļ�ϵͳ��дʱ���Ĭ�ϱ����ʽ
	QTextCodec::setCodecForLocale(codec);

	//���ô���tr����ʱ��Ĭ���ַ�������
	QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
	//QTextCodec::setCodecForTr(codec);

	//�����ַ���������QByteArray����QString����ʱʹ�õ�һ�ֱ��뷽ʽ
	QTextCodec::setCodecForCStrings(codec);
	*/
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
}

int main(int argc, char *argv[])
{
	SetDefaultCodec();

	QApplication a(argc, argv);
	testchildlog w;

	if(!w.Initialize())
		return 0;

	w.show();

	return a.exec();
}
