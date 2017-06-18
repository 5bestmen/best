#include "LogImpl.h"

#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "ace/CDR_Stream.h"

#include "Utils/misc.h"


void CLogImpl::InitSqlite()
{
	char dbname[128];
	sprintf(dbname,"log/%s.db",(char*)m_strLogName.c_str());

	// create and open database
	m_pDatabase = new Kompex::SQLiteDatabase(dbname, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
	// create statement instance for sql queries/statements
	m_pStmt = new Kompex::SQLiteStatement(m_pDatabase);

	//时间列自动生成值
	m_pStmt->SqlStatement("CREATE TABLE if not exists LOG (module VARCHAR(50) NOT NULL, content VARCHAR(500), level INTEGER,logtime TIMESTAMP default (datetime('now', 'localtime')) )");
}

void CLogImpl::UnInitSqlite()
{
	// clean-up
	delete m_pStmt;
	delete m_pDatabase;

	m_pStmt = NULL;
	m_pDatabase = NULL;
}

bool CLogImpl::Log2Sqlite()
{
	//处理缓存的所有日志
	LOGDATA logdata;

	try
	{
		m_pStmt->BeginTransaction();

		char buf[1024];
		while(m_logCache.PopCache(logdata))
		{
			//写日志
			ACE_InputCDR cdr ((char*)(&logdata),sizeof(LOGDATA));
			CMsgLog log;
			cdr >> log;

			char* strLogText = (char*)(log.m_strLogText.GetMsg());
			char* strLogModule = (char*)(log.m_strLogModule.GetMsg());
			int	  nLogLevel = log.m_nLogLevel;

			sprintf(buf,"INSERT INTO LOG (module,content,level) VALUES ('%s', '%s', %d)",strLogModule,strLogText,nLogLevel);
			m_pStmt->SqlStatement(buf);
		}
					
		m_pStmt->CommitTransaction();
	}
	catch(Kompex::SQLiteException &exception) 
	{
		std::cerr << "Exception Occured: " << exception.GetString();
		std::cerr << "SQLite result code: " << exception.GetSqliteResultCode() << std::endl;
		m_pStmt->RollbackTransaction();
		std::cerr << "Rollback has been executed!" << std::endl;
		std::cerr << "This is our own catch() block!" << std::endl;

		//重新连接Sqlite
		UnInitSqlite();
		InitSqlite();
	}

	return true;
}

