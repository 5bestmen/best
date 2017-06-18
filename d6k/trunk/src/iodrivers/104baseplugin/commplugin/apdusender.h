/********************************************************************************
模块名       :  apdu接收类
文件名       :  apdusender.h
文件实现功能 :  发送104数据类
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef CAPDUSENDER_H
#define CAPDUSENDER_H
#include <QObject>
#include "asdu_info.h"


#ifndef MSB
#define MSB(x) (((x) >> 8) & 0xff)	/* most signif byte of 2-byte integer */
#define LSB(x) ((x)&0xff)			  /* least signif byte of 2-byte integer*/
#define MSW(x) (((x) >> 16) & 0xffff) /* most signif word of 2-word integer */
#define LSW(x) ((x)&0xffff)			  /* least signif byte of 2-word integer*/
#endif

class CCommPlugin;

class CApduSender: public QObject
{
    Q_OBJECT
public:
    CApduSender(QObject *parent=0);
    ~CApduSender();
    bool Send_U(int nType);
    bool Send_S();
    bool Send_S_BeforeClose(int nRecvNum);
    int Send_I(char* pBuff,int nLen);

    bool OnSendFrame(int nFrameType,int nUtype);

    bool OnTimerSend(int nFrameType,int nUtype);

    bool OnKwSend(int nFrameType);

    bool OnNumSend(int nFrameType);

    bool OnCommand(NBM_TELECTRL* pTelectrl);

    bool OnSendGeneralCall(NBM_TELECTRL* pTelectrl);

    bool FormatAPCI(char* pBuff,int nAsduLen);
	bool OnSendKwhGeneralCall(NBM_TELECTRL* pTelectrl);
	bool OnSendSyncTime(NBM_TELECTRL* pTelectrl);
	bool OnSendSetAnalogNormalizeRequest(NBM_TELECTRL* pTelectrl);
	bool OnSendSetAnalogScaleRequest(NBM_TELECTRL* pTelectrl);
	bool OnSendSetAnalogShortFloatRequest(NBM_TELECTRL* pTelectrl);
	bool OnSendSetBinarySPRequest(NBM_TELECTRL* pTelectrl);
	bool OnSendSetBinaryDPRequest(NBM_TELECTRL* pTelectrl);
    //复位进程
    bool OnSendSetResetProcessRequest(NBM_TELECTRL* pTelectrl);
    //时间同步手动设置
    bool OnSendSycsOwnTime(int nDeviceId,QDateTime &tSycsTime);

	//定值获取
	bool OnSendDevDataRequest(DEV_BASE *pRequestDz);
    //死区设置
    bool OnSendZoomDataRequest(ZOOM_BASE *pZoomData);
	//录波文件获取
    bool OnSendRecordCatalog(LB_DATA dLbData);
	//定值区切换
	bool OnSendFixAreaCode(ASDU200 *pRequestArea);
    //获取录波文件信息
    LB_DATA GetLbData() const;
    //选择文件
    bool OnSendChooseLbFile();
    //请求文件
    bool OnSendRequestFile(int nIndex);
	//定值取查询
	bool OnSendFixSearch();
    //
    void SetLbAllInfo(const QList<LB_DATA> &lstLbData);
    //最后结束
    void OnSendFileConformNode();
    //
    QList<LB_DATA> GetLbAllData();

	//召唤报文目录
	bool OnSendGetCatalogRequest(const FILE_CATALOG_REQUEST_1 &lbCatalog);

	//读文件激活
	bool OnSendReadFileAction(const FILE_ATTR_INFO& fileAttr);

	//写文件激活
	bool OnSendWriteFileAction(const FILE_ATTR_INFO& fileAttr);
	//写文件
	bool OnSendWriteFileData();

	//和校验
	unsigned char checkAllData(const char *data, int length);

	//获取目录
	const QString &GetFilePath()
	{
		return m_strFilePath;
	}

	//
	void ClearFilepath()
	{
		m_strFilePath.clear();
	}

	void SetFilePath(const QString &strFilePath)
	{
		m_strFilePath = strFilePath;
	}

public:
    enum { MAX_SERIAL_NUMBER = 32767};//序列号15位，最大值

    unsigned short  m_nSendNum;//本端发送了多少个I帧，初始为0，发送了1个加1
    int m_k; //发送了的I帧中，还有m_k个没有收到对端的确认
    int m_w;//接收到的I帧中，还有m_w个没有给对端发送确认
    //104
    CCommPlugin *m_pComm104Pln;
    //录波数据
    LB_DATA m_lbData;
    //所有需要下载的录波数据
    QList<LB_DATA> m_lstLbData;
	//文件发送目录
	QString m_strFilePath;
	//写出的文件内容
	QByteArray m_btWriteData;
};

#endif // CAPDUSENDER_H
