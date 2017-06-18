
#ifndef FES_MSGPUMP_H
#define FES_MSGPUMP_H

#include <memory>
 
class CMemDB;

class CFesMsgPump
{
public:
	CFesMsgPump(void);
	~CFesMsgPump(void);
public:
	bool Initialize(const char *pszDataPath, unsigned int nMode);
	void Run();
	void Shutdown();

public:
	 
};


#endif // FES_MSGPUMP_H

