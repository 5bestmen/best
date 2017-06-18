#ifndef LOADFILETHREAD_H
#define LOADFILETHREAD_H

#include <QObject>


#define RECORDFILEPATH "/ini/f220/lbfile/"
#define HISTORY_DATA "/ini/f220/history/"
#define REMOTE_TABLE "/ini/f220/remotetable/"
#define CMD_HISTORY "./ini/f220/cmdhistory/"



#define RECORD_FIlE "tffs0"

class CLoadFileThread : public QObject
{
    Q_OBJECT

public:
    CLoadFileThread(QObject *parent=0);
    ~CLoadFileThread();
    void CreateNewFile(QString &strFileName, QByteArray &byData);
signals:
    void Signal_CreateNewFile(QString &strFileName,QByteArray &byData);

public slots:

void Slot_CreateNewFile(QString strFileName, QByteArray byData);

private:
    
};

#endif // LOADFILETHREAD_H
