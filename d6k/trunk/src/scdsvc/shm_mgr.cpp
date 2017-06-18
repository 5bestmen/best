
#include "shm_mgr.h"

#include <QString>
//#include <QFileInfo>
#include <QDebug>


CMgrData::CMgrData()
{
    memset(m_pBindPoolName, 0, sizeof(m_pBindPoolName));
}



CShmMgr::CShmMgr(const char* pShmName, const size_t nBlockSize /*= 0*/) : m_szShmName(pShmName),
        m_nSize(nBlockSize + sizeof(long long) + sizeof(CMgrData))
{
	Q_ASSERT(pShmName);
	Q_ASSERT(strlen(pShmName));

    m_pOptions = nullptr;
    m_pMMapAllocator = nullptr;

    Init();
}

CShmMgr::~CShmMgr()
{
    if (m_pOptions != nullptr)
    {
        m_pOptions = nullptr;
        m_pOptions = nullptr;
    }
    
    if (m_pMMapAllocator != nullptr)
    {
        m_pMMapAllocator = nullptr;
        m_pMMapAllocator = nullptr;
    }
}

bool CShmMgr::Init()
{
    // 参数2：NEVER_FIXED分配与位置无关的内存，移植性高。
    // 参数3：预先分配多大的内存空间。
    m_pOptions = new ACE_MMAP_Memory_Pool_Options(ACE_DEFAULT_BASE_ADDR, ACE_MMAP_Memory_Pool_Options::NEVER_FIXED, 1, 
        m_nSize + m_nSize / 3 + sizeof(ACE_Control_Block) + sizeof(CMgrData));

	m_pMMapAllocator = new MMAP_ALLOCATOR(m_szShmName.c_str(), 0, m_pOptions);

    return true;
}

void* CShmMgr::CreateShm(const size_t nSize)
{
    if (nSize > m_nSize)
    {
        // log
        return nullptr;
    }
    
    void* pData = m_pMMapAllocator->calloc(1, nSize);
    int e = 0;
    if (pData == nullptr)
    {
        e = ACE_OS::last_error();
        // log
    }
    return pData;
}

// 查找内存
/// Locate @a name and pass out parameter via pointer.  If found,
/// return 0, returns -1 if @a name isn't found.
int CShmMgr::FindMemData(const char* name, void*& pData)
{
    int nRet = 0;
    nRet = m_pMMapAllocator->find(name, pData);

    return nRet;
}

CMgrData* CShmMgr::GetMemData(const char* pPoolName)
{
    CMgrData* pMgrhead = 0;
    if (m_pMMapAllocator->find(pPoolName, (void*&)pMgrhead) == -1)
    {
        return nullptr;
    }

    return pMgrhead;
}

int CShmMgr::BindData(const char* name, void*& pData)
{
    CMgrData* pMgrhead = GetMemData(name);
    if (pMgrhead != nullptr)
    {
        ACE_OS::strcpy(pMgrhead->m_pBindPoolName, name);
        pMgrhead->m_pBindData = pData;

        return 1;
    }

    return m_pMMapAllocator->bind(name, pData);
}

void CShmMgr::ReleaseMemData(void*& pData)
{

}

void CShmMgr::ReleaseAll()
{

}






