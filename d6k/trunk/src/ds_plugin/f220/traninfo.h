#ifndef TRANINFO_H
#define TRANINFO_H
#include <QString>
#include <QXmlStreamReader>

struct FTPCONFIG
{
    QString strFtpIP;       //ftp ip��ַ
    int nPort;            // ftp port
    QString strUsername;    //ftp�û���
    QString strpassWd;      //����
    QString strfile;        //��Ҫ���ص��ļ������ϴ���·��
    FTPCONFIG()
    {
        nPort = 21;
    }
};

struct TCPADDRESS
{
    QString strIp;         //ip��ַ
    int nPort;             //�˿ں�
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
    //�����ļ�����
    void SetXmlFilename(const QString &strFilename);
    //��ȡ�ļ� ��������
    void AnalyseTransInfoFile(const QString &strFilename);
    //�����ļ�
    void BeginAnalyse(QIODevice *pDevice);
    //����update����
    void AnalyseUpdateData(QXmlStreamReader &xmlReader);
    //����download����
    void AnalyseDownLoadData(QXmlStreamReader &xmlReader);

    //��ʼд��
    void BeginWriteXmlFIle();
    //д���ļ���������
    void WriteTransInfoFIle(TCPADDRESS tpcAddr, FTPCONFIG upInfo,FTPCONFIG downInfo);
    //д��down����
    void WriteDownInfoFile(QXmlStreamWriter &tWriter,const FTPCONFIG &downInfo);
    //д��up����
    void WriteUpInfoFile(QXmlStreamWriter &tWriter,const FTPCONFIG &upInfo);

    //�ϴ���Ϣ
    FTPCONFIG GetUploadInfo();
    //������Ϣ
    FTPCONFIG GetDownLoadInfo();
    //tcp 
    TCPADDRESS GetNetworkInfo();
    //������Ϣ
    void SetUploadInfo(FTPCONFIG uploadinfo);
    //
    void SetDownLoadInfo(FTPCONFIG downloadinfo);
    //
    void SetTcpInfo(TCPADDRESS tcpinfo);
private:
    //�����ļ�����
    QString m_strFilename;
    //�ϴ�
    FTPCONFIG m_upLoadInfo;
    //����
    FTPCONFIG m_downLoadInfo;
    //tcp
    TCPADDRESS m_address;

};

#endif // TRANINFO_H
