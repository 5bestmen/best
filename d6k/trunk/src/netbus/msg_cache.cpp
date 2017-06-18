
#include "msg_cache.h"

CMsgCache::CMsgCache()
{
	memset(m_buf, 0, MAX_EMSG_L + 1);
	m_nCurPos = 0;
}

char* CMsgCache::GetDataBuff()
{
	return m_buf + 1;
}

size_t CMsgCache::GetDataLen()
{
	char* p = m_buf + 1;
	EMSG_BUF *pBuf = (EMSG_BUF*)(p);

	int nLen = pBuf->MsgDataSize;

	return nLen;
}

size_t CMsgCache::GetCurByteLength()
{
	return m_nCurPos;
}

size_t CMsgCache::GetNeedByteLength()
{
	size_t nDataLen = GetDataLen();
	if (nDataLen <= 0)
	{
		return 0;
	}

	if (nDataLen > MAX_EMSG_L)
	{
		nDataLen = MAX_EMSG_L;
	}

	return nDataLen - m_nCurPos + 1;
}

void CMsgCache::CopyData(const char* pBuff, size_t nLen)
{
	char* pFrom = m_buf + m_nCurPos;

	memcpy(pFrom, pBuff, nLen);

	m_nCurPos += nLen;

	m_buf[m_nCurPos] = '\0';
}

bool CMsgCache::IsEmpty()
{
	if (m_nCurPos == 0)
	{
		return true;
	}

	return false;
}

void CMsgCache::Clear()
{
	m_nCurPos = 0;
}
