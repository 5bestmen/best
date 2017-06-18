
#ifndef _RECVER_H
#define _RECVER_H

#include "104cache.h"
#include "define_104.h"

#include "ihost.h"

class C104Impl;  //impl

class CRecver
{
public:
	CRecver(IHost* pHost, C104Impl* pImpl);
	virtual ~CRecver();

	//接收解析
public:
	bool OnReceive(char* pBuff, int nLen);//从接收缓冲区组织完整APDU
private:
	bool AnalyseApdu(char* pBuff, int nLength);
	bool AnalyseI(char* pBuff, int nLength);
	bool AnalyseU(char* pBuff, int nLength);
	bool AnalyseS(char* pBuff, int nLength);
	void AnalyseAsdu(char* pBuff, int nLength);

	//Control
private:
	void OnRecvAnalogNormalized(char* pBuff, int nLength);          //!<ASDU9
	bool OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen);    //!<带短时标的归一化测量值 ASDU10
	void OnRecvAnalogScaled(char* pBuff, int nLength);              //!<ASDU11
	bool OnRecvAnalogScaledShortTime(char* pBuff, int nLen);        //!<带短时标的标度化测量值 ASDU12
	void OnRecvAnalogShortFloat(char* pBuff, int nLength);          //!<ASDU13
	bool OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen);    //!<带短时标的短浮点数测量值 ASDU14
	bool OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen);    //!<不带品质描述词的归一化测量值 ASDU21
	bool OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen);     //!<带长时标的归一化测量值 ASDU34
	bool OnRecvAnalogScaledLongTime(char* pBuff, int nLen);         //!<带长时标的标度化测量值 ASDU35
	bool OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen);     //!<带长时标的短浮点数测量值 ASDU36
private:
	void OnRecvBinarySinglePoint(char* pBuff, int nLength);         //!<ASDU1
	void OnRecvBinaryDoublePoint(char* pBuff, int nLength);         //!<ASDU3
	void OnRecvBinaryGroupPoint(char* pBuff, int nLength);          //!<带变位检出的成组单点 ASDU20
	void OnRecvBinarySinglePointTime(char* pBuff, int nLength);     //!<ASDU30
	void OnRecvBinaryDoublePointTime(char* pBuff, int nLength);     //!<ASDU31
	void OnRecvBinarySinglePointShortTime(char* pBuff, int nLength);//!<ASDU2
	void OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength);//!<ASDU4

private:
	void OnRecvGeneralCallConfirm(char* pBuff, int nLength);        //!<ASDU100
	void OnRecvKwhGeneralCallConfirm(char* pBuff, int nLength);     //!<ASDU101
	void OnRecvSyncTimeConfirm(char* pBuff, int nLength);           //!<ASDU103
	void OnRecvKwh(char* pBuff, int nLength);                       //!<ASDU15
	void OnRecvKwhShortTime(char* pBuff, int nLength);              //!<ASDU16
	void OnRecvKwhLongTime(char* pBuff, int nLength);               //!<ASDU37
private://遥控(确认)
	void OnRecvSetAnalogNormalizeAck(char* pBuff, int nLength);     //!<ASDU48
	void OnRecvSetAnalogScaleAck(char* pBuff, int nLength);         //!<ASDU49
	void OnRecvSetAnalogShortFloatAck(char* pBuff, int nLength);    //!<ASDU50
	void OnRecvSetBinarySPAck(char* pBuff, int nLength);            //!<ASDU45
	void OnRecvSetBinaryDPAck(char* pBuff, int nLength);            //!<ASDU46        
private:
	C104Cache m_104Cache;
private:
	IHost* m_pHost;
	C104Impl*   m_pImpl;
};


#endif//_RECVER_H
