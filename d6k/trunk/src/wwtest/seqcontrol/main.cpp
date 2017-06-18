#include "seqcontrolwgt.h"
#include <QApplication>
#include <vld.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CSeqControlWgt w;
    w.show();

    return a.exec();
}
