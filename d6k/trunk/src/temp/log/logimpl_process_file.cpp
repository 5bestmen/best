#include "LogImpl.h"

#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "ace/CDR_Stream.h"

#include "Utils/misc.h"

bool CLogImpl::Log2File()
{
	//处理缓存的所有日志
	LOGDATA logdata;

	char file[128];
	sprintf(file,"log/%s.log",(char*)m_strLogName.c_str());
	
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

		sprintf(buf,"Module:%s Level:%d Log:%s",strLogModule,nLogLevel,strLogText);

		Utils::Log2File(file,buf,true);
	}

	return true;
}
