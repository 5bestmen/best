#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include "loadfilethread.h"
#include "filetranswgt.h"

CLoadFileThread::CLoadFileThread(QObject *parent)
    : QObject(parent)
{

}

CLoadFileThread::~CLoadFileThread()
{

}

void CLoadFileThread::CreateNewFile(QString &strFileName, QByteArray &byData)
{
    emit Signal_CreateNewFile(strFileName, byData);
}

void CLoadFileThread::Slot_CreateNewFile(QString strFileName, QByteArray byData)
{
    if (strFileName.isEmpty())
    {
        return;
    }

    QString strRunPath = qApp->applicationDirPath();

	if (strFileName.contains("/tffs0//iec104sList.ini"))
	{
		//点表文件
		strFileName = strRunPath + REMOTE_TABLE + strFileName;

	}
	else if (strFileName.contains("/tffs0//soeHstry.bin"))
	{
		//历史数据
		strFileName = strRunPath + RECORDFILEPATH + strFileName;

	}
	else if (strFileName.contains("/tffs0//CmdHistory.txt"))
	{
		//操作历史
		strFileName = strRunPath + CMD_HISTORY + strFileName;
	}
	else if (strFileName.contains("/tffs0//COMTRADE"))
	{
		//录播文件
		strFileName = strRunPath + RECORDFILEPATH + strFileName;
	}
	else
	{
		strFileName = g_FileName;

		QStringList listPaths = strFileName.split("/");
		//最后一个文件名称不算


		QString strpath = strFileName.remove(listPaths.last());

		QDir tDir;

		tDir.setPath(strpath);

		if (!tDir.exists())
		{
			if (!tDir.mkpath(strpath))
			{
				qDebug() << tr("create % path error").arg(strFileName);
			}
		}

		QFile file(strFileName + listPaths.last());

		if (!file.open(QFile::WriteOnly))
		{
			qDebug() << file.errorString();
			return;
		}

		file.write(byData);
		file.close();

		return;
	}
	
	strFileName.remove("/tffs0/");
	//文件目录
	QStringList listPaths = strFileName.split("/");
	//最后一个文件名称不算

	
	QString strpath = strFileName.remove(listPaths.last());

	QDir tDir;

	tDir.setPath(strpath);

	if (!tDir.exists())
	{
		if (!tDir.mkpath(strpath))
		{
			qDebug() << tr("create % path error").arg(strFileName);
		}
	}

    QFile file(strFileName + listPaths.last());

    if (!file.open(QFile::WriteOnly))
    {
        qDebug() << file.errorString();
        return;
    }

	file.write(byData);
    file.close();
}

