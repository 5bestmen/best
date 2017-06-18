/*! @file
<PRE>
********************************************************************************
模块名       :  Modbus转发规约模块
文件名       :
文件实现功能 :
作者         :  HuangYuqing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
	//选择树节点
	void Slot_ClickTreeItem(QTreeWidgetItem *pItem, int nColumn);
	//更新DI数据槽函数
	void Slot_UpdateRecvDiData(int StationID, int DeviceID, int DiID, int realData, QString DiName);
	//更新AI数据槽函数
	void Slot_UpdateRecvAiData(int StationID, int DeviceID, int AiID, int realData, QString AiName);
	//更新Kwh数据槽函数
	void Slot_UpdateRecvKwhData(int StationID, int DeviceID, int KwhID, int realData, QString KwhName);
	//
	void Slot_ConnectSuccess(int nDeviceID, const QString &strDeviceName);
	//
	void Slot_SocketError(int nDeviceID, const QString &strDeviceName,const QString &strError);

	//右键菜单
	void Slot_ContextMenuEvent(const QPoint & cPoint);
	//重连
	void Slot_Reconnect();
	//断开连接
	void Slot_Disconnect();
	
signals:
	void Signal_CloseSocket(int nDeviceID);
	void Signal_ReconSocket(int nDeviceID, QString strIP, int nPort);
	void Signal_TransBinary(int nStationID, int nDeviceID, int nDiID, int nValue);
	void Signal_TransAnalog(int nStationID, int nDeviceID, int nAiID, int nValue);


private:
	//初始化
	bool Init();
	//加载xml配置文件
	bool LoadCfg();
	//生成树结构
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

    //发送过来的信号量
    //Device ID   
    QMap<int, QMap<int, BINARYSTRUCT> > m_mapBinaryDevice;
	QMap<int, QMap<int, ANALOGSTRUCT> > m_mapAnalogDevice;
public:

};

#endif // MB_EXCHANGE_H
