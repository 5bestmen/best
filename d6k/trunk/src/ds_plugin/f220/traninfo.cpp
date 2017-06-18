#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QFile>
#include "traninfo.h"


CTranInfo::CTranInfo()
{
}

CTranInfo::~CTranInfo()
{
}

FTPCONFIG CTranInfo::GetUploadInfo()
{
    return m_upLoadInfo;
}

FTPCONFIG CTranInfo::GetDownLoadInfo()
{
    return m_downLoadInfo;
}

TCPADDRESS CTranInfo::GetNetworkInfo()
{
    return m_address;
}

void CTranInfo::AnalyseTransInfoFile(const QString &strFilename)
{
    QFile tFile(strFilename);

    if (!tFile.exists())
    {
        return;
    }

    if (tFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        BeginAnalyse(&tFile);
        tFile.close();
    }
    else
    {
        QMessageBox::warning(0, QObject::tr("Waring"),QObject::tr("analyse file %1 error").arg(strFilename));
    }
}

void CTranInfo::WriteTransInfoFIle(TCPADDRESS tpcAddr, FTPCONFIG upInfo, FTPCONFIG downInfo)
{
    QFile tFile(m_strFilename);

    if (tFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter tWriter(&tFile);
        tWriter.setAutoFormatting(true);
        tWriter.writeStartDocument();
        tWriter.writeStartElement("conifg");

        tWriter.writeStartElement("mantanceIP");
        tWriter.writeCharacters(tpcAddr.strIp);
        tWriter.writeEndElement();

        tWriter.writeStartElement("mantancePort");
        tWriter.writeCharacters(QString::number(tpcAddr.nPort));
        tWriter.writeEndElement();
        //写入down
        WriteDownInfoFile(tWriter,downInfo);
        //写入 up
        WriteUpInfoFile(tWriter, upInfo);

        tWriter.writeEndElement();
        tWriter.writeEndDocument();

        tFile.close();
    }
    else
    {
        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("write to config file error"));
    }
}

void CTranInfo::BeginAnalyse(QIODevice *pDevice)
{
    QXmlStreamReader xmlReader(pDevice);

    while (!xmlReader.atEnd() && (!xmlReader.hasError()))
    {
        //遍历所有配置
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "mantanceIP")
            {
                m_address.strIp = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "mantancePort")
            {
                m_address.nPort = xmlReader.readElementText().toInt();
            }
            else if (xmlReader.name().toString() == "download")
            {
                AnalyseDownLoadData(xmlReader);
            }
            else if (xmlReader.name().toString() == "upload")
            {
                AnalyseUpdateData(xmlReader);
            }
        }
    }
}

void CTranInfo::AnalyseUpdateData(QXmlStreamReader & xmlReader)
{
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "ftpip")
            {
                m_upLoadInfo.strFtpIP = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "ftpport")
            {
                m_upLoadInfo.nPort = xmlReader.readElementText().toInt();
            }
            else if (xmlReader.name().toString() == "username")
            {
                m_upLoadInfo.strUsername = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "password")
            {
                m_upLoadInfo.strpassWd = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "upPath")
            {
                m_upLoadInfo.strfile = xmlReader.readElementText();
            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "upload")
            {
                break;
            }
        }
    }
}

void CTranInfo::AnalyseDownLoadData(QXmlStreamReader &xmlReader)
{
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "ftpip")
            {
                m_downLoadInfo.strFtpIP = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "ftpport")
            {
                m_downLoadInfo.nPort = xmlReader.readElementText().toInt();
            }
            else if (xmlReader.name().toString() == "username")
            {
                m_downLoadInfo.strUsername = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "password")
            {
                m_downLoadInfo.strpassWd = xmlReader.readElementText();
            }
            else if (xmlReader.name().toString() == "filename")
            {
                m_downLoadInfo.strfile = xmlReader.readElementText();
            }
        }

        if (xmlReader.isEndElement())
        {
            if (xmlReader.name().toString() == "download")
            {
                break;
            }
        }
    }
}

void CTranInfo::WriteDownInfoFile(QXmlStreamWriter &tWriter, const FTPCONFIG &downInfo)
{
    tWriter.writeStartElement("download");

    tWriter.writeStartElement("ftpip");
    tWriter.writeCharacters(downInfo.strFtpIP);
    tWriter.writeEndElement();

    tWriter.writeStartElement("ftpport");
    tWriter.writeCharacters(QString::number(downInfo.nPort));
    tWriter.writeEndElement();

    tWriter.writeStartElement("username");
    tWriter.writeCharacters(downInfo.strUsername);
    tWriter.writeEndElement();

    tWriter.writeStartElement("password");
    tWriter.writeCharacters(downInfo.strpassWd);
    tWriter.writeEndElement();

    tWriter.writeStartElement("filename");
    tWriter.writeCharacters(downInfo.strfile);
    tWriter.writeEndElement();

    tWriter.writeEndElement();
}

void CTranInfo::WriteUpInfoFile(QXmlStreamWriter &tWriter, const FTPCONFIG &upInfo)
{
    tWriter.writeStartElement("upload");

    tWriter.writeStartElement("ftpip");
    tWriter.writeCharacters(upInfo.strFtpIP);
    tWriter.writeEndElement();

    tWriter.writeStartElement("ftpport");
    tWriter.writeCharacters(QString::number(upInfo.nPort));
    tWriter.writeEndElement();

    tWriter.writeStartElement("username");
    tWriter.writeCharacters(upInfo.strUsername);
    tWriter.writeEndElement();

    tWriter.writeStartElement("password");
    tWriter.writeCharacters(upInfo.strpassWd);
    tWriter.writeEndElement();

    tWriter.writeStartElement("upPath");
    tWriter.writeCharacters(upInfo.strfile);
    tWriter.writeEndElement();

    tWriter.writeEndElement();
}

void CTranInfo::SetXmlFilename(const QString &strFilename)
{
    m_strFilename = strFilename;
}

void CTranInfo::BeginWriteXmlFIle()
{
    WriteTransInfoFIle(m_address, m_upLoadInfo, m_downLoadInfo);
}

void CTranInfo::SetUploadInfo(FTPCONFIG uploadinfo)
{
    m_upLoadInfo = uploadinfo;
}

void CTranInfo::SetDownLoadInfo(FTPCONFIG downloadinfo)
{
    m_downLoadInfo = downloadinfo;
}

void CTranInfo::SetTcpInfo(TCPADDRESS tcpinfo)
{
    m_address = tcpinfo;
}
