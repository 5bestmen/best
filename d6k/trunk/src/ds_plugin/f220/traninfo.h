#ifndef TRANINFO_H
#define TRANINFO_H
#include <QString>
#include <QXmlStreamReader>

struct FTPCONFIG
{
    QString strFtpIP;       //ftp ip地址
    int nPort;            // ftp port
    QString strUsername;    //ftp用户名
    QString strpassWd;      //密码
    QString strfile;        //需要下载的文件名或上传的路径
    FTPCONFIG()
    {
        nPort = 21;
    }
};

struct TCPADDRESS
{
    QString strIp;         //ip地址
    int nPort;             //端口号
    TCPADDRESS()
    {
        nPort = 5000;
    }

};

class CTranInfo
{
public:
    CTranInfo();      
    ~CTranInfo();     
    //设置文件名称
    void SetXmlFilename(const QString &strFilename);
    //读取文件 更改配置
    void AnalyseTransInfoFile(const QString &strFilename);
    //解析文件
    void BeginAnalyse(QIODevice *pDevice);
    //解析update数据
    void AnalyseUpdateData(QXmlStreamReader &xmlReader);
    //解析download数据
    void AnalyseDownLoadData(QXmlStreamReader &xmlReader);

    //开始写入
    void BeginWriteXmlFIle();
    //写入文件更改配置
    void WriteTransInfoFIle(TCPADDRESS tpcAddr, FTPCONFIG upInfo,FTPCONFIG downInfo);
    //写入down数据
    void WriteDownInfoFile(QXmlStreamWriter &tWriter,const FTPCONFIG &downInfo);
    //写入up数据
    void WriteUpInfoFile(QXmlStreamWriter &tWriter,const FTPCONFIG &upInfo);

    //上传信息
    FTPCONFIG GetUploadInfo();
    //下载信息
    FTPCONFIG GetDownLoadInfo();
    //tcp 
    TCPADDRESS GetNetworkInfo();
    //设置信息
    void SetUploadInfo(FTPCONFIG uploadinfo);
    //
    void SetDownLoadInfo(FTPCONFIG downloadinfo);
    //
    void SetTcpInfo(TCPADDRESS tcpinfo);
private:
    //配置文件名称
    QString m_strFilename;
    //上传
    FTPCONFIG m_upLoadInfo;
    //下载
    FTPCONFIG m_downLoadInfo;
    //tcp
    TCPADDRESS m_address;

};

#endif // TRANINFO_H
