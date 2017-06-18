/*! @file
<PRE>
********************************************************************************
ģ����       :  Modbusת����Լģ��
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  HuangYuqing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author  HuangYuqing
*  @version 1.0
*  @date    2016.8.18
*/
#ifndef MB_EXCHANGE_H
#define MB_EXCHANGE_H

#include <QtWidgets/QMainWindow>
#include "ui_mb_exchange.h"
#include "socketmgr.h"

#include "modbustrans.h"

struct BINARYSTRUCT
{
    int StationID;
    int DeviceID;
    int DiID;
    int realData;
    QString DiName;
};

struct ANALOGSTRUCT
{
	int StationID;
	int DeviceID;
	int AiID;
	int realData;
	QString AiName;
};

class CMb_Exchange : public QMainWindow
{
	Q_OBJECT

public:
	CMb_Exchange(QWidget *parent = 0);
	~CMb_Exchange();

    void InitPointTableData();
public slots:
	//ѡ�����ڵ�
	void Slot_ClickTreeItem(QTreeWidgetItem *pItem, int nColumn);
	//����DI���ݲۺ���
	void Slot_UpdateRecvDiData(int StationID, int DeviceID, int DiID, int realData, QString DiName);
	//����AI���ݲۺ���
	void Slot_UpdateRecvAiData(int StationID, int DeviceID, int AiID, int realData, QString AiName);
	//����Kwh���ݲۺ���
	void Slot_UpdateRecvKwhData(int StationID, int DeviceID, int KwhID, int realData, QString KwhName);
	//
	void Slot_ConnectSuccess(int nDeviceID, const QString &strDeviceName);
	//
	void Slot_SocketError(int nDeviceID, const QString &strDeviceName,const QString &strError);

	//�Ҽ��˵�
	void Slot_ContextMenuEvent(const QPoint & cPoint);
	//����
	void Slot_Reconnect();
	//�Ͽ�����
	void Slot_Disconnect();
	
signals:
	void Signal_CloseSocket(int nDeviceID);
	void Signal_ReconSocket(int nDeviceID, QString strIP, int nPort);
	void Signal_TransBinary(int nStationID, int nDeviceID, int nDiID, int nValue);
	void Signal_TransAnalog(int nStationID, int nDeviceID, int nAiID, int nValue);


private:
	//��ʼ��
	bool Init();
	//����xml�����ļ�
	bool LoadCfg();
	//�������ṹ
	void MakeTreeInfo();


private:
	Ui::mb_exchangeClass ui;
	CSocketMgr* m_pSocketMgr;

	QString m_strSendMsg;

	CReadXML* m_pReadXML;
	CModbusTrans* m_pModbusTrans;
	int m_nDeviceID;
	//DiID   rownum
	QMap<int, int> m_mapDevDiRow;
	//AiID	 rownum
	QMap<int, int> m_mapDevAiRow;
	//KwhID	 rownum
	QMap<int, int> m_mapDevKwhRow;

    //���͹������ź���
    //Device ID   
    QMap<int, QMap<int, BINARYSTRUCT> > m_mapBinaryDevice;
	QMap<int, QMap<int, ANALOGSTRUCT> > m_mapAnalogDevice;
public:

};

#endif // MB_EXCHANGE_H
