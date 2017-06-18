/*! @file shm_mgr.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  shm_mgr.h
�ļ�ʵ�ֹ��� :  ���������ڴ�
����         :  Zhu Yongjin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     ǰ�����ݶ���
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

// �ڴ����ݶ�
class CMgrData
{
public:
    CMgrData();
	~CMgrData()
	{

	}

    char m_pBindPoolName[MAX_POOL_NAME_LEN + 1];    // �����ڴ�İ���
    void* m_pBindData;                              // ָ����Malloc�������poolname��ָ��
};

class CShmMgr
{
public:
    CShmMgr(const char* pShmName, const size_t blockSize = 0);
    ~CShmMgr();
public:    
      // �ڴ洴��
    void* CreateShm(const size_t size);

    // �����ڴ�
    /// Locate @a name and pass out parameter via pointer.  If found,
    /// return 0, returns -1 if @a name isn't found.
    int FindMemData(const char* name, void*& pData);

    // ��ȡ�ڴ�����
    CMgrData* GetMemData(const char* pPoolName);

    // ���ڴ�
    int BindData(const char* name, void*& pData);

    // �ͷ��ڴ�
    void ReleaseMemData(void*& pData);

    // �ͷ������ڴ�
    void ReleaseAll();
protected:
	// ��������ѡ���ʼ��
	bool Init();

private:
    // ace�����ڴ������
    MMAP_ALLOCATOR* m_pMMapAllocator;

    // ace�����ڴ�ѡ��
    ACE_MMAP_Memory_Pool_Options* m_pOptions;

    // �����ڴ���
	std::string m_szShmName;
  //  const char* m_pShmName;

    // �ڴ��С
    size_t m_nSize;
};


 

#endif // SHM_MANAGER_H