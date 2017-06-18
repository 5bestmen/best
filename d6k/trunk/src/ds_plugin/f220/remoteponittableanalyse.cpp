#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include "remoteponittableanalyse.h"

CRemotePonitTableAnalyse::CRemotePonitTableAnalyse(QObject *parent)
	: QObject(parent)
{

}

CRemotePonitTableAnalyse::~CRemotePonitTableAnalyse()
{

}

//打开文件
bool CRemotePonitTableAnalyse::OpenPointTableFile(const QString &strFileName)
{
	//清空所有数据
	m_lstBinary.clear();
	m_lstAnalog.clear();
	m_lstKWH.clear();
	m_lstControl.clear();

	if (strFileName.isEmpty())
	{
		QMessageBox::warning(0, tr("WARNING"), tr("read point table file error!"));
		return false;
	}

	QFile fPluginFile(strFileName);

	if (fPluginFile.open(QFile::ReadOnly | QFile::Text))
	{
		if (!AnalyseXmlData(&fPluginFile))
		{
			QMessageBox::warning(0, tr("WARNING"), tr("analyse file:%1error!").arg(strFileName));
			return false;
		}
		
		fPluginFile.close();
	}
	else
	{
		QMessageBox::warning(0, tr("WARNING"), tr("open file:%1error!").arg(strFileName));
		return false;
	}
	return true;
}

//解析文件
bool CRemotePonitTableAnalyse::AnalyseXmlData(QIODevice *pDevice)
{
	if (pDevice == NULL)
	{
		return false;
	}

	QString strType = "";

    m_lstBinaryCodes.clear();
    m_lstAnalogCodes.clear();
    m_lstKwhCodes.clear();
    m_lstControlCodes.clear();

	while (!pDevice->atEnd())
	{
		QString strLine = QString::fromLocal8Bit(pDevice->readLine());
		//类型
		strLine.remove(" ");
		if (strLine.contains("[") && strLine.contains("]"))
		{
			strType = strLine.remove("[").remove("]");
			strType.remove(strType.count()-1);
		}
		else
		{

			if (strType.contains(QStringLiteral("遥信")))
			{
				QStringList lstData = strLine.split(",");
				if (lstData.count() < 4)
				{
					continue;;
				}
				//遥信数据
				RPT S_Binary;
				S_Binary.NUM = lstData.at(0).simplified().toInt();
                S_Binary.GroupNum = lstData.at(1).simplified().toInt();
                S_Binary.NUM2 = m_lstBinary.count() + 1;
				strcpy(S_Binary.Destriber,lstData.at(3).toStdString().c_str());
				strcpy(S_Binary.Unit, lstData.at(4).toStdString().c_str());
                S_Binary.zoom = 0;
				m_lstBinary.append(S_Binary);

                m_mapBinaryDeviceIdGroup.insert(m_lstBinary.count(), S_Binary);
                //插入数据
                m_mapAllPoints[S_Binary.GroupNum][S_Binary.NUM] = S_Binary;
                if (!m_lstBinaryCodes.contains(lstData.at(1).simplified()))
                {
                    m_lstBinaryCodes.append(lstData.at(1).simplified());
                }
                
			}
			if (strType.contains(QStringLiteral("遥测")))
			{
				QStringList lstData = strLine.split(",");
				if (lstData.count() < 14)
				{
					continue;;
				}
				//遥信数据
				RPT S_Binary;
                S_Binary.NUM = lstData.at(0).simplified().toInt();
                S_Binary.GroupNum = lstData.at(1).simplified().toInt();
				S_Binary.NUM2 = m_lstAnalog.count() + 1;
				strcpy(S_Binary.Destriber, lstData.at(3).toStdString().c_str());
				strcpy(S_Binary.Unit, lstData.at(4).toStdString().c_str());
                //缩放
                S_Binary.zoom = lstData.at(8).toFloat();
                m_mapAnalogZoom.insert(S_Binary.NUM2, S_Binary.zoom);

				//b  k  uplimit downlimit
				S_Binary.fBanalog = lstData.at(9).toFloat();
				S_Binary.fKanalog = lstData.at(10).toFloat();
				S_Binary.fUpperLimit = lstData.at(11).toFloat();
				S_Binary.fDownLimit = lstData.at(12).toFloat();
				S_Binary.fThreshold = lstData.at(13).toFloat();

				m_lstAnalog.append(S_Binary);

                m_mapAnalogDeviceIdGroup.insert(m_lstAnalog.count(), S_Binary);
                m_mapAllPoints[S_Binary.GroupNum][S_Binary.NUM] = S_Binary;

                if (!m_lstAnalogCodes.contains(lstData.at(1).simplified()))
                {
                    m_lstAnalogCodes.append(lstData.at(1).simplified());
                }
			}
			if (strType.contains(QStringLiteral("遥脉")))
			{
				QStringList lstData = strLine.split(",");
				if (lstData.count() < 4)
				{
				    continue;
				}
				//遥脉数据
				RPT S_Binary;
                S_Binary.NUM = lstData.at(0).simplified().toInt();
                S_Binary.GroupNum = lstData.at(1).simplified().toInt();
				S_Binary.NUM2 = m_lstKWH.count() + 1;
				strcpy(S_Binary.Destriber, lstData.at(3).toStdString().c_str());
				strcpy(S_Binary.Unit, lstData.at(4).toStdString().c_str());
                S_Binary.zoom = 0;
				m_lstKWH.append(S_Binary);

                m_mapKwhDeviceIdGroup.insert(m_lstKWH.count(), S_Binary);

                m_mapAllPoints[S_Binary.GroupNum][S_Binary.NUM] = S_Binary;

                if (!m_lstKwhCodes.contains(lstData.at(1).simplified()))
                {
                    m_lstKwhCodes.append(lstData.at(1).simplified());
                }

			}
			if (strType.contains(QStringLiteral("遥控")))
			{
				QStringList lstData = strLine.split(",");
				if (lstData.count() < 4)
				{
					continue;;
				}
				//
				RPT S_Binary;
                S_Binary.NUM = lstData.at(0).simplified().toInt();
                S_Binary.GroupNum = lstData.at(1).simplified().toInt();
                S_Binary.NUM2 = m_lstControl.count()+1;
				strcpy(S_Binary.Destriber, lstData.at(3).toStdString().c_str());
				strcpy(S_Binary.Unit, lstData.at(4).toStdString().c_str());
                S_Binary.zoom = 0;
				m_lstControl.append(S_Binary);

                m_mapControlDeviceIdGroup.insert(m_lstControl.count(), S_Binary);
                m_mapAllPoints[S_Binary.GroupNum][S_Binary.NUM] = S_Binary;
                //m_lstControlCodes.append(lstData.at(1).simplified());

                if (!m_lstControlCodes.contains(lstData.at(1).simplified()))
                {
                    m_lstControlCodes.append(lstData.at(1).simplified());
                }
			}
		}


	}

	return true;
}



//遥信
QList<RPT> CRemotePonitTableAnalyse::GetBinaryData()
{
	return m_lstBinary;
}
//遥测
QList<RPT> CRemotePonitTableAnalyse::GetAnaloyData()
{
	return m_lstAnalog;
}
//遥脉
QList<RPT> CRemotePonitTableAnalyse::GetKwhData()
{
	return m_lstKWH;
}
//遥控
QList<RPT> CRemotePonitTableAnalyse::GetControlData()
{
	return m_lstControl;
}

QMap<int, QMap<int, RPT> > CRemotePonitTableAnalyse::GetAllPointInfo()
{
    return m_mapAllPoints;
}

QMap<int, float> CRemotePonitTableAnalyse::GetAnalogZoom()
{
    return m_mapAnalogZoom;
}

QMap<int, RPT> CRemotePonitTableAnalyse::GetBinaryIdGroup()
{
    return m_mapBinaryDeviceIdGroup;
}

QMap<int, RPT> CRemotePonitTableAnalyse::GetAnalogIdGroup()
{
    return m_mapAnalogDeviceIdGroup;
}

QMap<int, RPT> CRemotePonitTableAnalyse::GetKwhIdGroup()
{
    return m_mapKwhDeviceIdGroup;
}

QMap<int, RPT> CRemotePonitTableAnalyse::GetControlIdGroup()
{
    return m_mapControlDeviceIdGroup;
}
