#include "commplugin.h"
#include <QApplication>
#include <QTextCodec>
#include <qwidget.h>
#include <QTextEdit>
#include <QHBoxLayout>

//处理的基本逻辑是这样的：
//一个线程专门负责接收网络信息,接收后保存到内存中
//主线程获取保存的信息进行进行解析
//数据解析后发出信号供其他地方使用
//main函数为测试使用  最终是以插件形式加入框架中

//t0  t1  t2  t3 超时还没做
//只有单点遥信
//只有总召

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<"mainID"<<QThread::currentThreadId();
    CCommPlugin *w = new CCommPlugin;

    //测试
    ICommPlugin *icomm = w;
	icomm->StartRun("127.0.0.1", 2404);

    QWidget *sss = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(sss);
    TextEdit *ptested = new TextEdit;
	TextEdit *ptested2 = new TextEdit;
    layout->addWidget(ptested);
	layout->addWidget(ptested2);
    sss->setFixedSize(1000,600);
    sss->show();

    QObject::connect(icomm,SIGNAL(Signal_SocketError(QString)),ptested,SLOT(addError(QString)));
    QObject::connect(icomm,SIGNAL(Signal_SocketConnectSuccess()),ptested,SLOT(addLickSuccess()));
    QObject::connect(icomm,SIGNAL(Signal_AllCallRespond()),ptested,SLOT(addAllBackSuccess()));
    QObject::connect(icomm,SIGNAL(Signal_OnePointRemote(int,int,int)),ptested,SLOT(addOnePoint(int,int,int)));
    QObject::connect(icomm,SIGNAL(Signal_onePointDisRemote(int,int,int)),ptested,SLOT(addDisPoint(int,int,int)));

	QObject::connect(icomm, SIGNAL(Signal_DoublePointRemote(int, int, int)), ptested, SLOT(addDisPoint(int, int, int)));
	QObject::connect(icomm, SIGNAL(Signal_DoublePointDisRemote(int, int, int)), ptested, SLOT(addDisPoint(int, int, int)));

	QObject::connect(icomm, SIGNAL(Signal_AnalogNormal(int, int, float, int)), ptested2, SLOT(addAnalogPoint(int, int, float, int)));
	QObject::connect(icomm, SIGNAL(Signal_DisAnalogNormal(int, int, float, int)), ptested2, SLOT(addAnalogPoint(int, int, float, int)));

	QObject::connect(icomm, SIGNAL(Signal_AnalogShortFloat(int, int, float, int)), ptested2, SLOT(addAnalogPointFloat(int, int, float, int)));
	QObject::connect(icomm, SIGNAL(Signal_DisAnalogShortFloat(int, int, float, int)), ptested2, SLOT(addAnalogPointFloat(int, int, float, int)));

    return a.exec();
}
