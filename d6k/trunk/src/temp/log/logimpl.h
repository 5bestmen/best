
#ifndef _LOG_IMPL_H
#define _LOG_IMPL_H

#include "utils/queue.h"

#include "logcache.h"

#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"

class CLogImpl
{
public:
	CLogImpl(void);
	~CLogImpl(void);

	//Part1
	//////////////////////////////////////////////////////////////////////////
	//���Žӿ�
public:
	bool CreateLog(char* szLogName);
	bool DestroyLog();
	bool OpenLog(char* szLogName);
	bool CloseLog();

	bool LogMsg(char* szMoudleName,char* szLogTxt,int nLevel);
private:
	bool PrepareLog(char* szLogName);
	std::string			m_strLogName; //log����
	unsigned int	m_nLogID;     //logID(��ʶ�����ڴ�)

	//Part2
	//////////////////////////////////////////////////////////////////////////
	//���ղ�������־
private:
	bool StartCache();//�������ղ������߳�
	void StopCache(); //ֹͣ���ղ������߳�
private:
	bool CreateCache(); //������־�����ڴ滺�����
	bool OpenCache();   //����־�����ڴ滺�����
	void CloseCache();  //�ر���־�����ڴ滺�����
private:
	bool DoLogCache();  //��־��������߼�
	static void LogCacheThreadProc(void * lpParameter);//��־�����߳�
private:
	bool    m_bIsCacheing;    //�߳����б�־
	bool    m_bIsCacheStopped;//�߳��˳���־
private:
	enum{LOG_QUEUE_SIZE = 1000,};//�����ڴ���־���д�С(�����־����)
	CQueue<LOGDATA> m_logQueue;  //�����ڴ���־����

	//Part3
	//////////////////////////////////////////////////////////////////////////
	//��ʱ�������������־
private:
	void StartProcess();//�������洦���߳�
	void StopProcess(); //ֹͣ���洦���߳�
private:
	bool DoLogProcess();//��־��������߼�
	static void LogProcessThreadProc(void * lpParameter);//��־�����߳�

	bool Log2Sqlite();
	bool Log2File();
	bool m_bIsLog2File;
private:
	void InitSqlite();
	void UnInitSqlite();
	Kompex::SQLiteDatabase* m_pDatabase;
	Kompex::SQLiteStatement* m_pStmt;
private:
	bool    m_bIsProcessing;    //�߳����б�־
	bool    m_bIsProcessStopped;//�߳��˳���־
private:
	CLogCache m_logCache;//������־�������
	int m_nLastTime;	 //�ϴδ�����־ʱ��
	int m_nCurrTime;     //��ǰʱ��
	enum
	{
		LOG_HANDLE_CYCLE = 5,    //ÿ5�봦��һ����־
		LOG_HANDLE_COUNT = 1000, //ÿ�ﵽ1000������һ����־
	};

};

extern CLogImpl g_LogImpl;


#endif // _LOG_IMPL_H

