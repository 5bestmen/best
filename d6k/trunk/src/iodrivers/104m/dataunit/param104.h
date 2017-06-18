

#ifndef _PARAM_104_H
#define _PARAM_104_H

#include <string>
using namespace std;

#include "asdu_104.h"

class CParam104
{
public:
	CParam104()
	{
		m_nAnalogStartAddr = 0x4000;
		m_nBinaryStartAddr = 0x00;
		m_nKwhStartAddr = 0;

		m_nAnalogTelectrlStartAddr = 0;
		m_nBinaryTelectrlStartAddr = 0;

		m_nAnalogTelectrlCommand = C_SE_NA_1;
		m_nBinaryTelectrlCommand = C_SC_NA_1;

		m_nCycleGeneralCall = 5 * 60;    //总召周期//秒
		m_nCycleKwhGeneralCall = 5 * 60;    //电镀总召周期//秒
		m_nCycleSyncTime = 5 * 60;    //对时周期//秒

		m_T1 = 15;
		m_T2 = 10;
		m_T3 = 20;

		m_nDefaultK = 12;
		m_nDefaultW = 8;
	}

public:
	int m_nParamID;
public:
	int m_nAnalogStartAddr;
	int m_nBinaryStartAddr;
	int m_nKwhStartAddr;
public:
	int m_nAnalogTelectrlStartAddr;
	int m_nBinaryTelectrlStartAddr;
public:
	int m_nAnalogTelectrlCommand;
	int m_nBinaryTelectrlCommand;

public:
	int m_T1;
	int m_T2;
	int m_T3;
public:
	int m_nDefaultK;
	int m_nDefaultW;
public:
	int m_nCycleGeneralCall;//总召周期//秒
	int m_nCycleKwhGeneralCall;//电镀总召周期//秒
	int m_nCycleSyncTime;//对时周期//秒
public:
	bool m_bIsSyncTime;   //是否对时
public:
	string Dump();
};


#endif //_PARAM_104_H

