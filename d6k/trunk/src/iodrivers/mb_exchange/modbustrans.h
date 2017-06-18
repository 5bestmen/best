#ifndef MODBUSTRANS_H
#define MODBUSTRANS_H

#include <QString>
#include <QObject>
#include "socketmgr.h"
#include "msg_queue.h"

#define MB_ERROR_CODE_ADDRESS       -1 //��ַ����
#define MB_ERROR_CODE_FUNCODE     -2 //���������
#define MB_ERROR_CODE_LENGTH        -3 //�����д�
#define MB_ERROR_CODE_CRC              -4 //CRCУ��
#define MB_ERROR_CODE_EXCEPTION   -5 //�쳣��Ϣ

enum
{
	PER_FRAME_ANALOG = 120, //2bytes
	PER_FRAME_BINARY = 1920,//1bit
	PER_FRAME_YM = 60,//4bytes
};

enum
{
	MAX_ERRORS_TO_HALT = 10,    //ERROR�ﵽ��ֵ��������
};

enum
{
	BIT_ADDRESS = 6,	//����װ�õ�ַ�ֽ�
	BIT_FUNCODE = 7,	//���Ĺ������ֽ�
	BIT_DATA_LEN = 8,	//�������ݳ����ֽ�
	BIT_DATA_HEAD = 9,
};

enum TELETRCL_TYPES
{
	TELECTRL_REQUEST_SELECT = 0,//ң��Ԥ������
	TELECTRL_ACK_SELECT = 1,//ң��Ԥ��ȷ��
	TELECTRL_REQUEST_EXECUTE = 2,//ң��ִ������
	TELECTRL_ACK_EXECUTE = 3,//ң��ִ��ȷ��
	TELECTRL_REQUEST_UNSELECT = 4,//ң�س�������
	TELECTRL_ACK_UNSELECT = 5,//ң�س���ȷ��

	TELECTRL_REQUEST_GENERALCALL,//��������
	TELECTRL_ACK_GENERALCALL,//����ȷ��
	TELECTRL_ACK_GENERALCALL_OVER,//���ٽ���
	TELECTRL_REQUEST_SYNCTIME,//��ʱ����
	TELECTRL_ACK_SYNCTIME,//��ʱȷ��
	TELECTRL_REQUEST_KWHGENERALCALL,//�����������
	TELECTRL_ACK_KWHGENERALCALL,//�������ȷ��
};

class CModbusTrans:public QObject
{
	Q_OBJECT
public:
	CModbusTrans();
	CModbusTrans(CSocketMgr* pSocketMgr, CReadXML* pReadXML,int nDeviceID);
	~CModbusTrans();

public:
	//��������
	int SendData(int nDeviceID, int nDataType);
	//��������
	int RecvData(QByteArray bydata);

//	int HandleTrans();

	std::string FormatBuffer(char* pBuff, int nMsgLen);

	//��������
	void ProcessData(QByteArray RecvData);
	//����ң������
	void ParseBinary(QByteArray RecvData);
	//����ң������
	void ParseAnalog(QByteArray RecvData);
	//���������
	void ParseKwh(QByteArray RecvData);
	//����ң�����ر���
	void ParseYT(QByteArray RecvData);
	//����ң�ط��ر���
	void ParseYK(QByteArray RecvData);
	//��ȡ������
	int GetFuncode(int DeviceID, int nDataType);


public slots:
	//�������ݲۺ���
    void Slot_RecvNewData(QByteArray bydata);
	//��ʼ��������
	void Slot_ConnectScuess();
	//
	void SLot_SocketError(const QString &strError);


	// ת��ң��
	bool Slot_TransBinary(int nStationID, int nDeviceID, int DiID, int nValue);

	// ת��ң��
	bool Slot_TransAnalog(int nStationID, int nDeviceID, int AiID, int ncrtValue);


signals:
	//Signal DI���ݸ���
	void Signal_ShowDiValue(int StationID, int DeviceID, int DiID, int realData, QString DiName);
	//AI���ݸ���
 	void Signal_ShowAiValue(int StationID, int DeviceID, int AiID, int realData, QString AiName);
	//Kwh���ݸ���
	void Signal_ShowKwhValue(int StationID, int DeviceID, int KwhID, int realData, QString KwhName);
	//�豸����״̬����
	void Signal_ShowDeviceState(int DeviceID, QString DeviceName);
	//���ӳɹ�
	void Signal_ConnectSuccess(int nDevice,const QString &strDevName);
	//socket����
	void Signal_SocketError(int nDevice, const QString &strDevName,const QString &strError);


	//������
private:
	unsigned char m_funCode;

public:
	//����ң��ң������
	bool SendTeleCmd(int nDeviceID, int nDataID, int nDataType, float fDataValue);

	//����ң��ת����Ϣ
	int HandleYKTransInfo(STransInfo& transInfo, int nValue);

	//����ң��ת����Ϣ
	int HandleYTTransInfo(STransInfo& transInfo, int nValue);
	
	unsigned short CRC16(unsigned char* buf, unsigned long len);

private:
	//ʱ��ͬ��
	bool SyncTime();

private:
	//���ݷ��ͺͽ��ջ���
    #define BUFFER_SIZE 4096
 	unsigned char m_recv_buf[BUFFER_SIZE + 1];
 	unsigned char m_recv_len;
	unsigned char m_send_buf[BUFFER_SIZE + 1];
	unsigned char m_send_len;

	CReadXML* m_pReadXML;
	CSocketMgr* m_pSocketMgr;
	int m_nDeviceID;
	int m_nDataType;

};


#endif // MODBUSTRANS_H