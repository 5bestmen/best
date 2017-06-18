/*! @file shm_service.h
<PRE>
********************************************************************************
模块名       :
文件名       :  shm_service.h
文件实现功能 :  共享内存服务接口
作者         :  Zhu Yongjin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     前置数据定义
*  @author    Zhu Yongjin
*  @version   1.0
*  @date      2016.10.18
*******************************************************************************/
#ifndef SHM_SERVICE_H
#define SHM_SERVICE_H

#include <ace/Singleton.h>
#include <ace/Array_Map.h>
#include <ace/SString.h>

#include <QString>

#include "shm_mgr.h"
#include "readxml.h"

class CMemPtr;

class CSHMService
{
    /*****************************************************************
    **描述:单实例相关类
    ****************************************************************/
    friend class ACE_Singleton<CSHMService, ACE_SYNCH_RW_MUTEX>;

public:
    static CSHMService* Instance();

    enum
    {
        ORDER,  // 大排行号存储
        HASH,   // hash存储
    };

public:
    // 内存服务初始化。
    // @para1 工程路径  @para2 内存初始化方式()
    bool Initialize(const QString& strProjectPath, unsigned int nMode);

    // 运行服务
    void Run();

    // 关闭服务
	void Shutdown();

private:

    // 建立全库共享内存
    bool BuildAllShmMem();

    // 建立单个节点共享内存
    std::shared_ptr<CShmMgr> BuildSingleNodeShmMem(QString strPoolPathName, const size_t size);

    // 打开共享内存
    bool OpenShmMem();
private:

    CSHMService();
    ~CSHMService();

private:

    // 加载xml
    CReadXml* m_pReadXml;

private:
    // key:共享内存文件名， value：内存管理指针
    typedef QMap<QString, std::shared_ptr<CShmMgr> > M_POOL_MAP;
    M_POOL_MAP m_poolMap;

    // key:节点名， value：单个节点内存
    ACE_Array_Map<QString, M_POOL_MAP> m_mapNode;

    // key:共享内存名,value:开辟的内存
    typedef QMap<QString, CMemPtr> M_MEM_PTR;
    // key:节点名， value：单个节点内存
    ACE_Array_Map<QString, M_MEM_PTR> m_mapMemPtr;

};

class CMemPtr
{
public:
    CMemPtr();
    ~CMemPtr();

public:

    void SetMemPtr(void* pMemData);

    void* GetMemPtr();

private:
    void* m_pMemData;
};

#endif