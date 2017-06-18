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
    //设置树
    void SetTreeWgt(QTreeWidget *pTreeWgt);
    //初始化映射信息
    void InitDescInfo(const QString &strFilename);
    //分析104
    void AnalyseApdu(char *pBuff, int nLength);
    //分析  I报文
    void AnalyseI(char *pBuff, int nLength);
    //分析  U报文
    void AnalyseU(char *pBuff, int nLength);
    //解析xml文件
    void AnalyseXmlData(QIODevice *pDevice);
    //解析104节点信息
    void Analyse104NodeInfo(QXmlStreamReader &xmlReader);
    //增加子节点信息
    void InsertNodeInfo(QString strCode,QString strDesc);
    //字符转化 
    QString ChangeOxType(QByteArray &btData);
public:
    //遥信
    void OnRecvBinarySinglePoint(char* pBuff, int nLength);
    //双点遥信
    void OnRecvBinaryDoublePoint(char* pBuff, int nLength);
    //成组单点遥信
    void OnRecvBinaryGroupPoint(char* pBuff, int nLength);
    //SOE
    void OnRecvBinarySinglePointTime(char* pBuff, int nLength);
    //带短时标的单点信息  SOE
    void OnRecvBinarySinglePointShortTime(char* pBuff, int nLength);
    //
    void OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength);
    void OnRecvBinaryDoublePointTime(char* pBuff, int nLength);
    //总召
    void OnRecvGeneralCall(char* pBuff, int nLength);
    //对时
    void OnRecvSyncTime(char* pBuff, int nLength);
    //电度
    void OnRecvKwhGeneralCall(char* pBuff, int nLength);
public:
    //归一化遥测
    void OnRecvAnalogNormalized(char* pBuff, int nLength);//ASDU9
    //浮点型遥测
    void OnRecvAnalogShortFloat(char* pBuff, int nLength);//ASDU13 
    //带短时标的归一化测量值
    void OnRecvAnalogNormalizedShortTime(char* pBuff, int nLength);
    //标度化测量值
    void OnRecvAnalogScaled(char* pBuff, int nLength);
    //带短时标的标度化测量值
    void OnRecvAnalogScaledShortTime(char* pBuff, int nLength);
    //带短时标的短浮点数测量值
    void OnRecvAnalogShortFloatShortTime(char* pBuff, int nLength);
    //不带品质描述词的归一化测量值
    void OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLength);

    bool OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen);//带长时标的归一化测量值 ASDU34
    bool OnRecvAnalogScaledLongTime(char* pBuff, int nLen);//带长时标的标度化测量值 ASDU35
    bool OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen);//带长时标的短浮点数测量值 ASDU36

public:

    //累计量 ASDU15
    void OnRecvKwh(char* pBuff, int nLength);//ASDU15
    //带短时标的累计量
    void OnRecvKwhShortTime(char* pBuff, int nLength);//ASDU16
    //带长时标的累计量
    void OnRecvKwhLongTime(char* pBuff, int nLength);//ASDU37

    //遥控报文解析  单点
    void OnControlCommand(char* pBuff, int nLength);
    //遥控报文解析  双点
    void OnControlDoubleCommand(char* pBuff, int nLength);

    //定值 读取
    void OnRecvDevReadCommand(char* pBuff, int nLength);
    //定值 设置
    void OnRecvDevWriteCommand(char* pBuff, int nLength);
    //召唤目录响应
    void OnRecvResponseCatalog(char *pBuff, int nLength);
    //选择文件响应
    void OnRecvChooseFileResponse(char *pBuff, int nLength);
    //请求文件准备就绪
    void OnRectRequestFileReady(char *pBuff, int nLength);
    //节数据传输
    void OnRecvNodeData(char *pBuff, int nLength);
    //节最后的段
    void OnRecvLastNodePart(char *pBuff, int nLength);

public slots:
    //构造解析数据
    void Slot_ClickOneCaptureInfo(QByteArray &byData);

private:
    //解析数据树
    QTreeWidget *m_pTreeWgt;
    //详细信息根节点
    QTreeWidgetItem *m_pTopTreeItem;
    //传送编号 ->  描述
    QMap<int, QString> m_mapReason;
    //code编号
    QMap<int, QString> m_mapCodeDesc;
    //main对象
    IMainModule *m_pICore;
};

#endif // ANALYSE104MSG_H
