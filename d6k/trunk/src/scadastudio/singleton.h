//singleton.h
#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>

#include <QMutexLocker>

//#include "dbapplitonapi.h"

class CSingleton{
public:
	static CSingleton* GetInstance();

	//static CDbapplitonapi *GetDbi()
	//{
	//	return m_pDbi;
	//}

private:
	static CSingleton* m_pInstance;
	//static CDbapplitonapi *m_pDbi;

	int m_nTest;
};
#endif