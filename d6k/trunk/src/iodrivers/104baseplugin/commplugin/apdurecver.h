/********************************************************************************
模块名       :  apdu接收类
文件名       :  apdurecver.h
文件实现功能 :  接收网络数据解析104数据
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef CAPDURECVER_H
#define CAPDURECVER_H
#include <QObject>
#include <QThread>
#include <QAbstractSocket>
#include <QDateTime>
#include <QThread>
#include "commplugin_global.h"
#include "104cache.h"
#include "asdu_info.h"


class CCommPlugin;
class CLoadFileThread;

class CApduRecver : public QObject
{
    Q_OBJECT
public:
    CApduRecver(QObject *parent=0);
    ~CApduRecver();
public:
    //解析数据
    bool OnReceive(char *pBuff, int nLen);
    bool AnalyseApdu(char* pBuff,int nLength);
    bool AnalyseI(char* pBuff,int nLength);
    void AnalyseAsdu(char* pBuff,int nLength);

    bool AnalyseU(char* pBuff,int nLength);
	//发送总召信息
	void SendRequestAllMsg();
	//

	void InsertToFileLists(const FILE_ATTR_INFO &fileInfo)
	{
		m_arrCatalogRespond.append(fileInfo);
	}

	void SetFileInfo(const FILE_ATTR_INFO &fileInfo)
	{
		m_fileAttrInfo = fileInfo;
	}

	void ClearFileInfos()
	{
		m_arrCatalogRespond.clear();
		m_lstFiles.clear();
	}
private:
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
    //复位进程
    void OnRecvResetProcess(char *pBuff, int nLength);
	//切换定值区
	void OnRecvSwitchFixArea(char *pBuff, int nLength);
	//读取当前定值区号
	void OnrecvReadCurrentFixArea(char *pBuff, int nLength);

	//文件服务处理
	void OnRecvFileAnalyseInfo(char *pBuff, int nLength);
	//目录召唤确认
	void OnRecvCatalogRespond(char *pBuff, int nLength);
	//读文件激活确认
	void OnRecvFileAction(char *pBuff, int nLength);

	//读文件数据传输
	void OnRecvFileData(char *pBuff, int nLength);
	//文件传输确认
	void OnRecvFileConform(char *pBuff, int nLength);

	//累计量 ASDU15
	void OnRecvKwh(char* pBuff, int nLength);//ASDU15
	//带短时标的累计量
	void OnRecvKwhShortTime(char* pBuff, int nLength);//ASDU16
	//带长时标的累计量
	void OnRecvKwhLongTime(char* pBuff, int nLength);//ASDU37
	//单点遥控反馈数据解析
	void OnRecvSetBinarySPAck(char* pBuff, int nLength);
	//双点遥控反馈数据解析
	void OnRecvSetBinaryDPAck(char* pBuff, int nLength);
	//参数读取
	void OnRecvDevReadRequestAck(char* pBuff, int nLength);
	//参数写入
	void OnRecvDevWriteRequestAck(char *pBuff, int nLength);
    //召唤目录响应
//     void OnRecvResponseCatalog(char *pBuff, int nLength);
//     //选择文件响应
//     void OnRecvChooseFileResponse(char *pBuff, int nLength);
//     //请求文件准备就绪
//     void OnRectRequestFileReady(char *pBuff, int nLength);
//     //节数据传输
//     void OnRecvNodeData(char *pBuff, int nLength);
//     //节最后的段
//     void OnRecvLastNodePart(char *pBuff, int nLength);
    //和校验
    unsigned char checkAllData(const unsigned char *data, int length);
    //故障事件
    void OnRecvDefalutMsg(char *pBuff, int nLength);
    //死区
    void OnRecvZoneMsg(char *pBuff, int nLength);
	//文件目录召唤相应
	void OnRecvCatalogRepond(char *pBuff, int nLength);
	//读文件激活相应
	void OnRecvReadActionResond(char *pBuff, int nLenght);
	//读数据文件传输
	void OnRecvReadFileRespond(char *pBuff, int nLength);
	//读文件传输确认
	void OnRecvReadConform(char *pBuff, int nLength);
	//写文件激活
	void OnRecvWriteAction(char *pBuff, int nLength);
	
	//写文件传输确认
	void OnRecvWriteConform(char *pBuff, int nLength);


signals:
    //单点连续遥信数据
    void Signal_OnePointRemote(int iDeviceID, int nPointID, int nValue);
    //单点非连续遥信数据
    void Signal_onePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//成组单点遥信  连续
	void Signal_BinaryGroupPoint(int iDeviceID, int nPointID, int nValue);
	//成组单点遥信  非连续
	void Signal_BinaryGroupDisPoint(int iDeviceID, int nPointID, int nValue);

	//双点连续遥信数据
	void Signal_DoublePointRemote(int iDeviceID, int nPointID, int nValue);
	//双点非连续遥信数据
	void Signal_DoublePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//带3个字节短时标的单点遥信   SOE
	void Signal_BinarySignalPointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//短时标双点遥信
    void Signal_BinaryDoublePointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//SOE
    void Signal_SoeBinarySinglePointTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//带长时标的双点信息
    void Signal_BinaryDoublePoint(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//累积量  电度  连续
	void Signal_KwhNormal(int iDeviceID, int nPointID, int nValue);
	//累计量  不连续
	void Signal_KwhDisNormal(int iDeviceID, int nPointID, int nValue);
	//带短时标的累积量   电度
	void Signal_KwhShortTimeNormal(int iDeviceID, int nPointID, float nValue);
	//带长时标的累积量  电度
	void Signal_KwhLongTimeNormal(int iDeviceID, int nPointID, float nValue);

	//归一化遥测连续数据
	void Signal_AnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//标度化遥测不连续数据
	void Signal_ScaledAnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//带短时标的归一化测量值
	void Signal_AnalogNormalShortTime(int iDeviceID, int nPointID, float nValue, int nQuality);

	//浮点型遥测连续数据
	void Signal_AnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//浮点型遥测不连续数据
	void Signal_DisAnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
    //总召唤确认
    void Signal_AllCallRespond();	
	//接收到的数据报文
	void Signal_recv16Data(QByteArray btData, int nLenght);
	//遥控反馈信息 0单点  1 双点      0:on 1:off         
	void Signal_ControlFeedBack(int iControlType,int iPointNum, int iOperateType,QString strStatus);
	//定值数据
	void Signal_DevReadBack(QMap<int,short> mapInfo);
	//设定数据响应   48/136    6/7   1/0  
	void Signal_devWriteBack(int iControlType,int iCot,int iQos);
    //故障
    void Signal_MalFuction(ASDUGZ gzData);
	//
    //死区
    void Signal_ZoneFeedBack(int nAddr,float fValue,int qpm,int nCot);
	//写文件
	void Signal_RecvFileData(QString strFileName, QByteArray byData);
	//定值
	void Signal_ReadFixData(DEV_BASE &devData);
	//文件目录
	void Signal_FIleCatalogINfo(QList<Catalog_Info>& lstFileInfo);

private:
    //104数据
    C104Cache m_104Cache;
    //104
    CCommPlugin *m_pComm104Pln;
//     //接长度
//     int m_nNodeLength;
//     //接收到一节完整的数据
//     QByteArray m_byNodeData;

//     //收到完整数据
    QByteArray m_AbyFileData;
//     //写文件
    CLoadFileThread *m_pFileLoad;
    //写文件线程
    QThread m_LoadThread;
	//召唤目录相应数据 属性部分
	QList<FILE_ATTR_INFO> m_arrCatalogRespond;
	//需要下载的信息
	FILE_ATTR_INFO m_fileAttrInfo;
	//
	QList<Catalog_Info> m_lstFiles;
public:
	//自定义文件名称
	QString m_strFileNmae;
};

#endif // CAPDURECVER_H
