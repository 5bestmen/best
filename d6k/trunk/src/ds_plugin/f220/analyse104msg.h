#ifndef ANALYSE104MSG_H
#define ANALYSE104MSG_H

#include <QObject>
#include <QMap>
#include <QXmlStreamReader>

#define CAUTUREIECDESC "/ini/f220/iecdesc.xml"

class QTreeWidget;
class QIODevice;
class IMainModule;
class QTreeWidgetItem;

class CAnalyse104Msg : public QObject
{
    Q_OBJECT

public:
    CAnalyse104Msg(IMainModule *pCore, QObject *parent = 0);
    ~CAnalyse104Msg();
    //������
    void SetTreeWgt(QTreeWidget *pTreeWgt);
    //��ʼ��ӳ����Ϣ
    void InitDescInfo(const QString &strFilename);
    //����104
    void AnalyseApdu(char *pBuff, int nLength);
    //����  I����
    void AnalyseI(char *pBuff, int nLength);
    //����  U����
    void AnalyseU(char *pBuff, int nLength);
    //����xml�ļ�
    void AnalyseXmlData(QIODevice *pDevice);
    //����104�ڵ���Ϣ
    void Analyse104NodeInfo(QXmlStreamReader &xmlReader);
    //�����ӽڵ���Ϣ
    void InsertNodeInfo(QString strCode,QString strDesc);
    //�ַ�ת�� 
    QString ChangeOxType(QByteArray &btData);
public:
    //ң��
    void OnRecvBinarySinglePoint(char* pBuff, int nLength);
    //˫��ң��
    void OnRecvBinaryDoublePoint(char* pBuff, int nLength);
    //���鵥��ң��
    void OnRecvBinaryGroupPoint(char* pBuff, int nLength);
    //SOE
    void OnRecvBinarySinglePointTime(char* pBuff, int nLength);
    //����ʱ��ĵ�����Ϣ  SOE
    void OnRecvBinarySinglePointShortTime(char* pBuff, int nLength);
    //
    void OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength);
    void OnRecvBinaryDoublePointTime(char* pBuff, int nLength);
    //����
    void OnRecvGeneralCall(char* pBuff, int nLength);
    //��ʱ
    void OnRecvSyncTime(char* pBuff, int nLength);
    //���
    void OnRecvKwhGeneralCall(char* pBuff, int nLength);
public:
    //��һ��ң��
    void OnRecvAnalogNormalized(char* pBuff, int nLength);//ASDU9
    //������ң��
    void OnRecvAnalogShortFloat(char* pBuff, int nLength);//ASDU13 
    //����ʱ��Ĺ�һ������ֵ
    void OnRecvAnalogNormalizedShortTime(char* pBuff, int nLength);
    //��Ȼ�����ֵ
    void OnRecvAnalogScaled(char* pBuff, int nLength);
    //����ʱ��ı�Ȼ�����ֵ
    void OnRecvAnalogScaledShortTime(char* pBuff, int nLength);
    //����ʱ��Ķ̸���������ֵ
    void OnRecvAnalogShortFloatShortTime(char* pBuff, int nLength);
    //����Ʒ�������ʵĹ�һ������ֵ
    void OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLength);

    bool OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen);//����ʱ��Ĺ�һ������ֵ ASDU34
    bool OnRecvAnalogScaledLongTime(char* pBuff, int nLen);//����ʱ��ı�Ȼ�����ֵ ASDU35
    bool OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen);//����ʱ��Ķ̸���������ֵ ASDU36

public:

    //�ۼ��� ASDU15
    void OnRecvKwh(char* pBuff, int nLength);//ASDU15
    //����ʱ����ۼ���
    void OnRecvKwhShortTime(char* pBuff, int nLength);//ASDU16
    //����ʱ����ۼ���
    void OnRecvKwhLongTime(char* pBuff, int nLength);//ASDU37

    //ң�ر��Ľ���  ����
    void OnControlCommand(char* pBuff, int nLength);
    //ң�ر��Ľ���  ˫��
    void OnControlDoubleCommand(char* pBuff, int nLength);

    //��ֵ ��ȡ
    void OnRecvDevReadCommand(char* pBuff, int nLength);
    //��ֵ ����
    void OnRecvDevWriteCommand(char* pBuff, int nLength);
    //�ٻ�Ŀ¼��Ӧ
    void OnRecvResponseCatalog(char *pBuff, int nLength);
    //ѡ���ļ���Ӧ
    void OnRecvChooseFileResponse(char *pBuff, int nLength);
    //�����ļ�׼������
    void OnRectRequestFileReady(char *pBuff, int nLength);
    //�����ݴ���
    void OnRecvNodeData(char *pBuff, int nLength);
    //�����Ķ�
    void OnRecvLastNodePart(char *pBuff, int nLength);

public slots:
    //�����������
    void Slot_ClickOneCaptureInfo(QByteArray &byData);

private:
    //����������
    QTreeWidget *m_pTreeWgt;
    //��ϸ��Ϣ���ڵ�
    QTreeWidgetItem *m_pTopTreeItem;
    //���ͱ�� ->  ����
    QMap<int, QString> m_mapReason;
    //code���
    QMap<int, QString> m_mapCodeDesc;
    //main����
    IMainModule *m_pICore;
};

#endif // ANALYSE104MSG_H
