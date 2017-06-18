#include "log_config.h"

#include "public.h"

#include "stl_util-inl.h"
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile> 
 
  
void CLogConfigMgr::LoadXml()
{
	// 在可执行文件目录下面 加载 log.xml文件
	QString szXmlPath = qApp->applicationDirPath();
	szXmlPath += "/";
	szXmlPath += "log.xml";

	QFile ConfigXml(szXmlPath);
	if (ConfigXml.open(QFile::ReadOnly | QFile::Text) == false)
	{
		ConfigXml.close();
		// log下
		QString szLog = "Open [ " + szXmlPath + " ] failed";
		qDebug() << szLog;
	//	LogString(szLog.toStdString().c_str(), 1);
		return;
	}

	// 解析数据
	QXmlStreamReader xmlReader(&ConfigXml);
	QString strTmp, szRet;

	while (!xmlReader.atEnd() && (!xmlReader.hasError()))
	{
		//遍历所有配置
		xmlReader.readNext();
		strTmp = xmlReader.name().toString();
		if (xmlReader.isStartElement())
		{
			if (xmlReader.name().toString() == "config")
			{

			}
			else if (xmlReader.name().toString() == "comm_log")
			{
				QString szText = xmlReader.attributes().value("name").toString();
				if (szText.isEmpty() == false)
				{
					auto pModule = GetModule(szText);
					if (pModule == nullptr)
					{
						pModule = AddNewModule(szText);
						Q_ASSERT(pModule);
					}
					if (pModule)
					{
  						int nRet = xmlReader.attributes().value("write_file").toInt();
						pModule->SetWriteFileFlag(nRet ? true : false);

 						szRet = xmlReader.attributes().value("format").toString();
  						if (szRet.compare(("txt"), Qt::CaseInsensitive) == 0)
   						{
							pModule->SetLogFormat(CModuleLogConfig::LOG_TXT); 
						}
						else if (szRet.compare(("sqlite"), Qt::CaseInsensitive) == 0)
						{
							pModule->SetLogFormat(CModuleLogConfig::LOG_SQLITE); 
						}
 
						nRet = xmlReader.attributes().value("file_number").toInt();
						pModule->SetFileNumber(nRet);
						nRet = xmlReader.attributes().value("file_length").toInt();
						pModule->SetFileLength(nRet);

// 						nRet = xmlReader.attributes().value("log_level").toInt();
// 						pModule->SetWriteLogLevel(nRet);

						nRet = xmlReader.attributes().value("svc_port").toInt();
						Q_ASSERT(nRet > 0);
						pModule->SetLogTaskPortNo(nRet);
						//	QString szLog = "Open [ " + szXmlPath + " ]  successd.";
						//	qDebug() << szLog;					 
					}
				}						 
			}
		}
	}
}
 