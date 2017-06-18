/*! @file shm_mgr.h
<PRE>
********************************************************************************
模块名       :
文件名       :  shm_mgr.h
文件实现功能 :  创建共享内存
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
*  @date      2016.10.13
*******************************************************************************/
#ifndef SHM_MANAGER_H
#define SHM_MANAGER_H

#include "scdsvc_define.h"

#include <vector>
#include <memory> 

#include "ace/Global_Macros.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Pagefile_Memory_Pool.h"
#include "ace/Malloc.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"
#include "ace/Process_Mutex.h"
#include "ace/PI_Malloc.h"

typedef ACE_Allocator_Adapter< ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> > MMAP_ALLOCATOR;

// 内存数据段
class CMgrData
{
public:
    CMgrData();
	~CMgrData()
	{

	}

    char m_pBindPoolName[MAX_POOL_NAME_LEN + 1];    // 保存内存的绑定名
    void* m_pBindData;                              // 指向由Malloc分配绑定在poolname的指针
};

class CShmMgr
{
public:
    CShmMgr(const char* pShmName, const size_t blockSize = 0);
    ~CShmMgr();
public:    
      // 内存创建
    void* CreateShm(const size_t size);

    // 查找内存
    /// Locate @a name and pass out parameter via pointer.  If found,
    /// return 0, returns -1 if @a name isn't found.
    int FindMemData(const char* name, void*& pData);

    // 获取内存数据
    CMgrData* GetMemData(const char* pPoolName);

    // 绑定内存
    int BindData(const char* name, void*& pData);

    // 释放内存
    void ReleaseMemData(void*& pData);

    // 释放所有内存
    void ReleaseAll();
protected:
	// 分配器和选项初始化
	bool Init();

private:
    // ace共享内存分配器
    MMAP_ALLOCATOR* m_pMMapAllocator;

    // ace共享内存选项
    ACE_MMAP_Memory_Pool_Options* m_pOptions;

    // 共享内存名
	std::string m_szShmName;
  //  const char* m_pShmName;

    // 内存大小
    size_t m_nSize;
};


 

#endif // SHM_MANAGER_H