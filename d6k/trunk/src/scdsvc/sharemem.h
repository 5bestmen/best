/*! @file sharemem.h
<PRE>
********************************************************************************
模块名       :
文件名       :  sharemem.h
文件实现功能 :  共享内存
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     共享内存
*  @author    LiJin
*  @version   1.0
*  @date      2016.04.24 
*******************************************************************************/
#ifndef SHARE_MEM_H
#define SHARE_MEM_H
 
#include <memory>
#include <string>

#ifdef  USE_QT_SHM
  
class QSharedMemory;

/*! \class CShareMem  sharemem.h  "sharemem.h"
*  \brief 共享内存.
*/
class CShareMem
{
public:
	CShareMem();
	~CShareMem();
public:
	// 创建共享内存
	void *CreateShareMem(const char *szMemName,int nSize);
	// 获取
	void *GetShareMem(const char * szMemName);

	bool Lock();
	bool UnLock();
private:
	std::shared_ptr<QSharedMemory> m_pShm;
};

#else

#include "ace/Global_Macros.h"

#include "ace/OS.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Shared_Memory.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Shared_Memory_Pool.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"
#include "ace/PI_Malloc.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Mutex.h"
#include "ace/Process_Mutex.h"


class CShareMem
{
public:
	enum class MemOptions{ ALWAYS_FIXED=0,NEVER_FIXED=1,DEFAULT,};
	enum {
		MAX_NAME_LEN=256,
	};
	CShareMem();
	~CShareMem()
	{
		Destroy();
	}
public:
	// 创建共享内存
	void *CreateShareMem(const char *szMemName, int nSize);

	void  Destroy();
	// 获取
	void *GetShareMem(const char * szMemName);

	void ReleaseMem();

	void Lock(){}

	void UnLock(){}
protected:
	bool  Create(const char* szMemName, MemOptions nMode);
	
private:

	typedef ACE_Malloc_T <ACE_MMAP_MEMORY_POOL,
						  ACE_Process_Mutex,
						  ACE_PI_Control_Block> ALLOCATOR;


	ALLOCATOR* m_pAllocator;

private:
	char m_szDBName[MAX_NAME_LEN];
};
 
/** @}*/

#endif //  _ACE_MEM


#endif // SHARE_MEM_H