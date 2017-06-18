#include <QMap>
#include <QDateTime>
#include <QFile>
#include <fstream>

#include "modbustrans.h"
#include "socketmgr.h"
#include "data_struct.h"
#include "readxml.h"
#include "log2file.h"

CModbusTrans::CModbusTrans(CSocketMgr* pSocketMgr, CReadXML* pReadXML,int nDeviceID) : m_pSocketMgr(pSocketMgr), m_pReadXML(pReadXML)
{	
	
	m_nDeviceID = nDeviceID;
	m_nDataType = 1;
	memset(m_recv_buf, 0, BUFFER_SIZE + 1);
	memset(m_send_buf, 0, BUFFER_SIZE + 1);
	connect(m_pSocketMgr->GetSocketClient(), SIGNAL(Signal_RecvNewData(QByteArray)),this,SLOT(Slot_RecvNewData(QByteArray)));
	connect(m_pSocketMgr->GetSocketClient(), SIGNAL(Signal_ConnectSuccess()), this, SLOT(Slot_ConnectScuess()));
	connect(m_pSocketMgr->GetSocketClient(), SIGNAL(Signal_SocketError(const QString &)), this, SLOT(SLot_SocketError(const QString &)));
	
}

CModbusTrans::~CModbusTrans()
{

}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::SendData
** \details 发送数据函数，发送报文
** \param  nDeviceID
** \param  nDataType
** \return int
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
int CModbusTrans::SendData(int nDeviceID, int nDataType)
{
	unsigned short nStartRegAddr = 0; //寄存器起始地址
	unsigned short nCount = 0;            //查询数目

	m_nDeviceID = nDeviceID;
	m_nDataType = nDataType;

	if (nDataType == DATATYPE_BINARY)
	{
		nCount = m_pReadXML->m_mapBinary[m_nDeviceID].size();
	}
	else
	{
		nCount = m_pReadXML->m_mapAnalog[m_nDeviceID].size();
	}
	//组织报文
	m_send_len = 6;


	m_send_buf[m_send_len++] = m_pReadXML->m_mapDevice[nDeviceID].Device_address;
	m_send_buf[m_send_len++] = GetFuncode(nDeviceID, nDataType);
	m_send_buf[m_send_len++] = MYHI8(nStartRegAddr);
	m_send_buf[m_send_len++] = MYLO8(nStartRegAddr);
	m_send_buf[m_send_len++] = MYHI8(nCount);
	m_send_buf[m_send_len++] = MYLO8(nCount);


	m_send_buf[0] = 0;
	m_send_buf[1] = 0;
	m_send_buf[2] = 0;
	m_send_buf[3] = 0;
	m_send_buf[4] = MYHI8(m_send_len - 6); //0
	m_send_buf[5] = MYLO8(m_send_len - 6);//6


	m_pSocketMgr->GetSocketClient()->WriteSocketData((const char *)m_send_buf, m_send_len);

	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
	
	std::string strSendData = FormatBuffer((char*)m_send_buf, m_send_len);

// 	//输出发送的报文数据
// 	qDebug() << "%s\n" << strSendData.c_str();

	char buf[128];
	sprintf(buf, "[Time]:%s %s [SEND]:%s", strdate.toStdString().c_str(), strTime.toStdString().c_str(), strSendData.c_str());
	char logfile[256];
	sprintf(logfile, "log/mb_exchange-[D%d].log", m_nDeviceID);
	MYLIB::Log2File(logfile, buf, true);

	

	return 0;
}

 /*! \fn 
 ********************************************************************************************************* 
 ** \brief  CModbusTrans::RecvData
 ** \details 接收数据函数，接收报文
 ** \param  bydata
 ** \return int
 ** \author HuangYuqing
 ** \date   2016年9月2日
 ** \note 
 ********************************************************************************************************/
 int CModbusTrans::RecvData(QByteArray bydata)
 {
 	int nResult = 1;

	int nFuncode = GetFuncode(m_nDeviceID, m_nDataType);
	char buf[128];
 	if (bydata.length() < 6)
 	{
 		nResult = MB_ERROR_CODE_LENGTH;	//收到的报文长度有误
 		
 	}
 	else if (bydata.length() != MYMAKE16(bydata[5], bydata[4]) + 6)
 	{
 		nResult = MB_ERROR_CODE_LENGTH;	//收到的报文长度有误
 	}
 	else if ((int)bydata[6] != (m_pReadXML->m_mapDevice[m_nDeviceID].Device_address))
  	{
 		nResult = MB_ERROR_CODE_ADDRESS; //收到的装置地址有误
  	}
 	else if ((int)bydata[7] != nFuncode)
 	{
 		if (bydata[7] & 0x80)
 		{
 			nResult = MB_ERROR_CODE_EXCEPTION; //
 
 			switch (m_recv_buf[7] & 0x7F)
 			{
 				case 0x01:
					qDebug() << "非法功能码！";
 					sprintf(buf, "recv result:非法功能码 0x01");
 					break;
 				case 0x02:
 					sprintf(buf, "recv result:非法数据地址 0x02");
 					break;
 				case 0x03:
 					sprintf(buf, "recv result:非法数据值 0x03");
 					break;
 				case 0x04:
 					sprintf(buf, "recv result:服务器故障 0x04");
 					break;
 				case 0x05:
 					sprintf(buf, "recv result:确认 0x05");
 					break;
 				case 0x06:
 					sprintf(buf, "recv result:服务器繁忙 0x06");
 					break;
 				case 0x0A:
 					sprintf(buf, "recv result:网关故障 0x0A");
 					break;
 				case 0x0B:
 					sprintf(buf, "recv result:网关故障 0x0B");
 					break;
 				default:
 					sprintf(buf, "recv result:未知错误");
 					break;
 			}
 		}
 		else
 		{
 			nResult = MB_ERROR_CODE_FUNCODE;
 			sprintf(buf, "recv result:功能码错误,接收功能码为：%d  请求功能码为:%d", m_recv_buf[7], nFuncode);
 		}
 	}
  	else
  	{
  		ProcessData(bydata);
  	}
 
	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	std::string strRecvData = FormatBuffer(bydata.data(), bydata.length());
// 	//输出收到的数据
// 	qDebug() << "%s\n" << strRecvData.c_str();

	sprintf(buf, "[Time]:%s %s [RECV]:%s", strdate.toStdString().c_str(), strTime.toStdString().c_str(), strRecvData.c_str());
	char logfile[256];
	sprintf(logfile, "log/mb_exchange-[D%d].log", m_nDeviceID);
	MYLIB::Log2File(logfile, buf, true);

 	return nResult;
 }

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::ProcessData
** \details 处理数据函数
** \param  RecvData
** \return void
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
void CModbusTrans::ProcessData(QByteArray RecvData)
{
	int Funcode;
	Funcode = RecvData[BIT_FUNCODE];

	if (Funcode == m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeAnalog)
	{
		ParseAnalog(RecvData);
	}
	else if (Funcode == m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeBinary)
	{
		ParseBinary(RecvData);
	}
	else if (Funcode == m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeKwh)
	{
		ParseKwh(RecvData);
	}
	else if (Funcode == m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeTelectrlAnalog)
	{
		ParseYT(RecvData);
	}
	else if (Funcode == m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeTelectrlBinary)
	{
		ParseYK(RecvData);
	}
	else
	{
		qDebug() << "非法功能码！";
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::ParseBinary
** \details 解析遥信数据
** \param  RecvData
** \return void
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
void CModbusTrans::ParseBinary(QByteArray RecvData)
{
	//遥信数据字节数
	int ucDataBytes = MYMAKE16(RecvData[5], RecvData[4]) - 3;
// 	//输出收到的遥信报文
// 	qDebug() << RecvData.toHex();

	int nDeviceIndex = 0;
	//BIT_DATA_HEAD 数据区开始
	for (int i = 0; i < ucDataBytes; i++)
	{
		unsigned char uChar = RecvData[BIT_DATA_HEAD + i];
		for (int j = 0; j < 8; j++)
		{
			unsigned char realData = (uChar >> j) & 0x01;
// 			//输出解析的数据值
// 			qDebug() << realData;
				
			int StationID = 1;
			int DeviceID = m_nDeviceID;
			int DiID = m_pReadXML->m_mapBinary[m_nDeviceID].value(nDeviceIndex).nDiID;
			QString DiName = m_pReadXML->m_mapBinary[m_nDeviceID].value(nDeviceIndex).strDiName;

			emit Signal_ShowDiValue(StationID, DeviceID, DiID, realData, DiName);

			nDeviceIndex++;
		}
		
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::ParseAnalog
** \details 解析遥测数据
** \param  RecvData
** \return void
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
void CModbusTrans::ParseAnalog(QByteArray RecvData)
{
	short realData;
	int nDeviceIndex = 0;

	int nAnalogCount = (MYMAKE16(RecvData[5], RecvData[4]) - 3) / 2; // 每两个字节一个analog

	//BIT_DATA_HEAD 数据区开始
	for (int i = 0; i < nAnalogCount; i++)
	{
		realData = RecvData[2 * i + BIT_DATA_HEAD] * 256 + RecvData[2 * i + 1 + BIT_DATA_HEAD];

		int StationID = 1;
		int DeviceID = m_nDeviceID;
		int AiID = m_pReadXML->m_mapAnalog[m_nDeviceID].value(nDeviceIndex).nAiID;
		QString AiName = m_pReadXML->m_mapAnalog[m_nDeviceID].value(nDeviceIndex).strAiName;
		
		emit Signal_ShowAiValue(StationID, DeviceID, AiID, realData, AiName);
		
		nDeviceIndex++;		
	}


}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::ParseKwh
** \details 解析电度数据
** \param  RecvData
** \return void
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
void CModbusTrans::ParseKwh(QByteArray RecvData)
{
	int realData;
	int nDeviceIndex = 0;

	int nKwhCount = (MYMAKE16(RecvData[5], RecvData[4]) -3) / 4; // 每四个字节一个kwh

	//BIT_DATA_HEAD 数据区开始
	for (int i = 0; i < nKwhCount; i++)
	{

		realData = MYMAKE32(MYMAKE16(RecvData[4 * i + 3 + BIT_DATA_HEAD], RecvData[4 * i + 2 + BIT_DATA_HEAD]), MYMAKE16(RecvData[4 * i + 1 + BIT_DATA_HEAD], RecvData[4 * i + BIT_DATA_HEAD]));

		int StationID = 1;
		int DeviceID = m_nDeviceID;
		int KwhID = m_pReadXML->m_mapKwh[m_nDeviceID].value(nDeviceIndex).nKwhID;
		QString KwhName = m_pReadXML->m_mapKwh[m_nDeviceID].value(nDeviceIndex).strKwhName;

		emit Signal_ShowKwhValue(StationID, DeviceID, KwhID, realData, KwhName);

		nDeviceIndex++;
	}


}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::ParseYT
** \details 解析遥调返回报文
** \param  RecvData
** \return void
** \author HuangYuqing
** \date   2016年9月3日
** \note 
********************************************************************************************************/
void CModbusTrans::ParseYT(QByteArray RecvData)
{
	int realData;

	//BIT_DATA_HEAD 数据区开始
	realData = MYMAKE32(MYMAKE16(RecvData[3 + BIT_DATA_HEAD], RecvData[2 + BIT_DATA_HEAD]), MYMAKE16(RecvData[1 + BIT_DATA_HEAD], RecvData[BIT_DATA_HEAD]));

}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::ParseYK
** \details 解析遥控返回报文
** \param  RecvData
** \return void
** \author HuangYuqing
** \date   2016年9月3日
** \note 
********************************************************************************************************/
void CModbusTrans::ParseYK(QByteArray RecvData)
{
	
	unsigned char realData;
	realData = ((int)RecvData[BIT_DATA_HEAD] == 0) ? 0 : 1;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::GetFuncode
** \details 获取相应的功能码
** \param  DeviceID
** \param  nDataType
** \return int
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
int CModbusTrans::GetFuncode(int DeviceID, int nDataType)
{
	int nFuncode = 3;
	if (nDataType == DATATYPE_ANALOG)
	{
		nFuncode = m_pReadXML->m_mapDevice[DeviceID].FuncodeAnalog;
	}
	else if (nDataType == DATATYPE_BINARY)
	{
		nFuncode = m_pReadXML->m_mapDevice[DeviceID].FuncodeBinary;
	}
	else
	{
		nFuncode = m_pReadXML->m_mapDevice[DeviceID].FuncodeKwh;
	}

	return nFuncode;
}


/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::SendTeleCmd
** \details 发送遥控遥调命令
** \param  nDeviceID
** \param  nDataID
** \param  nDataType
** \param  fDataValue
** \return bool
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
bool CModbusTrans::SendTeleCmd(int nDeviceID, int nDataID, int nDataType, float fDataValue)
{
	//组织报文
	m_send_len = 6;

	m_nDeviceID = nDeviceID;
	m_nDataType = nDataType;

	int nDeviceAddr = m_pReadXML->m_mapDevice[m_nDeviceID].Device_address;
	m_send_buf[m_send_len++] = nDeviceAddr;

	m_send_buf[m_send_len++] = ((m_nDataType == DATATYPE_ANALOG) ? m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeTelectrlAnalog : m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeTelectrlBinary);

	m_send_buf[m_send_len++] = MYHI8(nDataID);
	m_send_buf[m_send_len++] = MYLO8(nDataID);

	if (m_nDataType == DATATYPE_ANALOG)
	{
		m_send_buf[m_send_len++] = MYHI8(fDataValue);
		m_send_buf[m_send_len++] = MYLO8(fDataValue);
	}
	else
	{
		int nDataValue = (int)fDataValue;
		m_send_buf[m_send_len++] = (nDataValue == 1) ? 0xff : 0x00;
		m_send_buf[m_send_len++] = 0x00;
	}

	m_send_buf[0] = 0;
	m_send_buf[1] = 0;
	m_send_buf[2] = 0;
	m_send_buf[3] = 0;
	m_send_buf[4] = MYHI8(m_send_len - 6); //0
	m_send_buf[5] = MYLO8(m_send_len - 6);//6

	m_pSocketMgr->GetSocketClient()->WriteSocketData((const char *)m_send_buf, m_send_len);

	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	char buf[128];
	std::string strTelectl = FormatBuffer((char*)m_send_buf, m_send_len);

	sprintf(buf, "[Time]:%s %s [Device:%d]: %s", strdate.toStdString().c_str(), strTime.toStdString().c_str(), strTelectl.c_str());
	MYLIB::Log2File("log/mb_exchange_telectl", buf, true);

	return true;
}

unsigned short CModbusTrans::CRC16(unsigned char* buf, unsigned long len)
{
	unsigned short crc;
	unsigned short i, j, k;

	crc = 0xffff;

	for (i = 0; i < len; i++)
	{
		crc = crc ^ buf[i];
		for (j = 0; j < 8; j++)
		{
			k = crc & 01;
			crc = crc >> 1;
			if (k == 0) continue;
			crc = crc ^ 0xA001;
		}
	}
	return crc;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::SyncTime
** \details 同步时间函数
** \return bool
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
bool CModbusTrans::SyncTime()
{
	static int nLastMinute = 0;//上次对时
	int nSyncTimeCycle = 1;//一分钟对时一次

	QDateTime currentDateTime = QDateTime::currentDateTime();

	int nYear = currentDateTime.date().year() - 2000;//年(年减2000);
	int nMonth = currentDateTime.date().month();//月
	int nDay = currentDateTime.date().day();//日
	int nHour = currentDateTime.time().hour();//时
	int nMinute = currentDateTime.time().minute();//分
	int nSecond = currentDateTime.time().second();//秒

	int nSpan = nMinute - nLastMinute;
	if ((nSpan >= 0 && nSpan < nSyncTimeCycle) || (nSpan < 0 && (nSpan + 60 < nSyncTimeCycle)))
		return false;

	nLastMinute = nMinute;//保存同步时间

	//组织报文
	m_send_len = 6;

	m_send_buf[m_send_len++] = m_pReadXML->m_mapDevice[m_nDeviceID].Device_address;
	m_send_buf[m_send_len++] = 0x10;//funcode
	m_send_buf[m_send_len++] = MYHI8(0x0); //起始地址H
	m_send_buf[m_send_len++] = MYLO8(0x0); //起始地址L
	m_send_buf[m_send_len++] = MYHI8(0x03);//寄存器数目H
	m_send_buf[m_send_len++] = MYLO8(0x03);//寄存器数目L
	m_send_buf[m_send_len++] = 0x03 * 2;//字节长度


	m_send_buf[m_send_len++] = nYear;
	m_send_buf[m_send_len++] = nMonth;
	m_send_buf[m_send_len++] = nDay;
	m_send_buf[m_send_len++] = nHour;
	m_send_buf[m_send_len++] = nMinute;
	m_send_buf[m_send_len++] = nSecond;


	m_send_buf[0] = 0;
	m_send_buf[1] = 0;
	m_send_buf[2] = 0;
	m_send_buf[3] = 0;
	m_send_buf[4] = MYHI8(m_send_len - 6); //0
	m_send_buf[5] = MYLO8(m_send_len - 6);//6

	m_pSocketMgr->GetSocketClient()->WriteSocketData((const char *)m_send_buf, m_send_len);
	

	return true;
}

std::string CModbusTrans::FormatBuffer(char* pBuff, int nMsgLen)
{
	std::string strDebugData("");
	char szByte[10] = { '\0' };

	for (int i = 0; i < nMsgLen; i++)
	{
		sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
		strDebugData += szByte;
	}

	return strDebugData;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::Slot_RecvNewData
** \details 接收数据槽函数
** \param  bydata
** \return void
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
void CModbusTrans::Slot_RecvNewData(QByteArray bydata)
{
	RecvData(bydata);
	m_nDataType++;
	if (m_nDataType > 3)
	{
		m_nDataType = 1;
	}
	this->SendData(m_nDeviceID, m_nDataType);
}

/*! \fn 
********************************************************************************************************* 
** \brief  CModbusTrans::Slot_ConnectScuess
** \details socket连接成功槽函数
** \return void
** \author HuangYuqing
** \date   2016年9月2日
** \note 
********************************************************************************************************/
void CModbusTrans::Slot_ConnectScuess()
{
		this->SendData(m_nDeviceID, m_nDataType);
		emit Signal_ConnectSuccess(m_nDeviceID, m_pReadXML->m_mapDevice[m_nDeviceID].Device_name);
}

void CModbusTrans::SLot_SocketError(const QString &strError)
{
	emit Signal_SocketError(m_nDeviceID, m_pReadXML->m_mapDevice[m_nDeviceID].Device_name, strError);
	
}

bool CModbusTrans::Slot_TransBinary(int nStationID, int nDeviceID, int nDiID, int nValue)
{
	//组织报文
	m_send_len = 6;

	m_nDeviceID = nDeviceID;

	int nDeviceAddr = m_pReadXML->m_mapDevice[m_nDeviceID].Device_address;
	m_send_buf[m_send_len++] = nDeviceAddr;

	m_send_buf[m_send_len++] = m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeTelectrlBinary;

	QVector<SDiInfo>& vecDiInfo = m_pReadXML->m_mapBinary[m_nDeviceID];
	int yk_number = -1;
	for each(auto i in vecDiInfo)
	{
		if (i.nDiID == nDiID)
		{
			yk_number = i.nAddr;

			break;
		}
	}
	m_send_buf[m_send_len++] = MYHI8(yk_number);
	m_send_buf[m_send_len++] = MYLO8(yk_number);

	m_send_buf[m_send_len++] = (nValue == 1) ? 0xff : 0x00;
	m_send_buf[m_send_len++] = 0x00;


	m_send_buf[0] = 0;
	m_send_buf[1] = 0;
	m_send_buf[2] = 0;
	m_send_buf[3] = 0;
	m_send_buf[4] = MYHI8(m_send_len - 6); //0
	m_send_buf[5] = MYLO8(m_send_len - 6);//6

	m_pSocketMgr->GetSocketClient()->WriteSocketData((const char *)m_send_buf, m_send_len);

	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	char buf[128];
	std::string strTelectl = FormatBuffer((char*)m_send_buf, m_send_len);

	sprintf(buf, "[Time]:%s %s [Device:%d]: %s", strdate.toStdString().c_str(), strTime.toStdString().c_str(), nDeviceID, strTelectl.c_str());
	MYLIB::Log2File("log/mb_exchange_telectl_yk.log", buf, true);

	return true;
}

bool CModbusTrans::Slot_TransAnalog(int nStationID, int nDeviceID, int nAiID, int ncrtValue)
{
	//组织报文
	m_send_len = 6;

	m_nDeviceID = nDeviceID;

	int nDeviceAddr = m_pReadXML->m_mapDevice[m_nDeviceID].Device_address;
	m_send_buf[m_send_len++] = nDeviceAddr;

	m_send_buf[m_send_len++] = m_pReadXML->m_mapDevice[m_nDeviceID].FuncodeTelectrlAnalog;

	int yt_number = -1;
	QVector<SAiInfo> vecAiInfo = m_pReadXML->m_mapAnalog[m_nDeviceID];
	for each (auto i in vecAiInfo)
	{
		if (i.nAiID == nAiID)
		{
			yt_number = i.nAddr;
		}
	}
	m_send_buf[m_send_len++] = MYHI8(yt_number);
	m_send_buf[m_send_len++] = MYLO8(yt_number);

	m_send_buf[m_send_len++] = MYHI8(ncrtValue);
	m_send_buf[m_send_len++] = MYLO8(ncrtValue);

	m_send_buf[0] = 0;
	m_send_buf[1] = 0;
	m_send_buf[2] = 0;
	m_send_buf[3] = 0;
	m_send_buf[4] = MYHI8(m_send_len - 6); //0
	m_send_buf[5] = MYLO8(m_send_len - 6);//6

	m_pSocketMgr->GetSocketClient()->WriteSocketData((const char *)m_send_buf, m_send_len);

	QString strdate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	QString strTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	char buf[128];
	std::string strTelectl = FormatBuffer((char*)m_send_buf, m_send_len);

	sprintf(buf, "[Time]:%s %s [Device:%d]: %s", strdate.toStdString().c_str(), strTime.toStdString().c_str(), nDeviceID, strTelectl.c_str());
	MYLIB::Log2File("log/mb_exchange_telectl_yt.log", buf, true);

	return true;
}


