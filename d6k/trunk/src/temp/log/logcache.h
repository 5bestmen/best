
#ifndef _LOG_CACHE_H
#define _LOG_CACHE_H

#include "utils\ThreadMutex.h"
#include "utils\AutoLock.h"

#include <queue>

#include "logdefine.h"

class CLogCache
{
public:
	CLogCache(void);
	~CLogCache(void);

public:
	void PushCache(LOGDATA& logdata);
	bool PopCache(LOGDATA& logdata);
	int GetSize();
private:
	//本地日志缓存队列
	std::queue<LOGDATA> m_cache;

	CThreadMutex m_logCacheMutex;
};


#endif // _LOG_CACHE_H

