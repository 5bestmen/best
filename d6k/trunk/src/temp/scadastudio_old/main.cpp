#include "scadastudio.h"
#include <QtWidgets\qapplication.h>
#include <QTextCodec>
#include "singleton.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	//���ñ��룬һ����Window�����������GBK���룬��Linux�����������utf-8����
	//��ȡϵͳ���룬������ֲ���������
	//QTextCodec *codec = QTextCodec::codecForName("System");

	//���úͶԱ����ļ�ϵͳ��дʱ���Ĭ�ϱ����ʽ
	//QTextCodec::setCodecForLocale(codec);

	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	////���ô���tr����ʱ��Ĭ���ַ�������
	//QTextCodec::setCodecForTr(codec);

	////�����ַ���������QByteArray����QString����ʱʹ�õ�һ�ֱ��뷽ʽ
	//QTextCodec::setCodecForCStrings(codec);
	
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

	CScadastudio w;
	w.show();
	 
	//CSingleton *test = CSingleton::GetInstance();

	//dbapplitonapi *newtest = test->GetDbi();

	//QString strtMP = "select * from FES_CHANNEL where ID='1'";

	//if(newtest->exec(strtMP))
	//{	
	//	while(newtest->next())
	//	{
	//		QVariant var1 = newtest->getValue(0);
	//	}
	//}
	//else
	//{
	//	strtMP = newtest->lastError();
	//}

	return a.exec();
}
