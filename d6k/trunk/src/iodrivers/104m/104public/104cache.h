
#ifndef _104_CACHE_H
#define _104_CACHE_H

#include <string.h>


class C104Cache
{
	enum
	{
		MAX_APDU_BUFFER_LEN = 256,
	};
private:
	char m_DataBuf[MAX_APDU_BUFFER_LEN];
	int  m_nCurPos;
public:
	C104Cache()
	{
		m_nCurPos = 0;
	}
public:
	char*   GetDataBuff();
	int     GetDataLen();
public:
	int GetCurByteLength();
	int GetNeedByteLength();
public:
	void CopyData(char* pBuff, int nLen);
	bool IsEmpty();
	void Clear();
};


#endif //_104_CACHE_H
