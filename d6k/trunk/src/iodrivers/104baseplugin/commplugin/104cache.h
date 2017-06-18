/********************************************************************************
模块名       :  104缓存
文件名       :  104cache.h
文件实现功能 :  管理104数据
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/

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
    char m_buf[MAX_APDU_BUFFER_LEN];
    int  m_nCurPos;

public:
    C104Cache()
    {
        m_nCurPos = 0;
    }

public:
    char* GetDataBuff();
    int GetDataLen();

public:
    int GetCurByteLength();
    int GetNeedByteLength();

public:
    void CopyData(char* pBuff,int nLen);
    bool IsEmpty();
    void Clear();
};


#endif //_104_CACHE_H
