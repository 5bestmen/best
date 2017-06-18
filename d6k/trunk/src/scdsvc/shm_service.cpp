
#include "shm_service.h"

#include <memory>

#include <QDir>
#include <QString>
#include <QFileInfo>

CSHMService::CSHMService()
{
    m_pReadXml = nullptr;
}

CSHMService::~CSHMService()
{
    if (m_pReadXml != nullptr)
    {
        delete m_pReadXml;
        m_pReadXml = nullptr;
    }
}

CSHMService* CSHMService::Instance()
{
    CSHMService* pSHMSvr = ACE_Unmanaged_Singleton<CSHMService, ACE_SYNCH_RW_MUTEX>::instance();

    return pSHMSvr;
}

bool CSHMService::Initialize(const QString& strProjectPath, unsigned int nMode)
{
    if (strProjectPath.isEmpty())
    {
        return false;
    }

    if (nMode == ORDER)
    { // OCCNO
        // todo:路径先写死加载xml信息.

        m_pReadXml = new CReadXml(strProjectPath);

        // 从xml中加载信息
        if (!m_pReadXml->LoadDataFromXml())
        {
            // log
            return false;
        }

        // 加载共享内存
        if (!BuildAllShmMem())
        {
            return false;
        }
    }
    else
    { // hash

    }

    return true;
}


bool CSHMService::BuildAllShmMem()
{
    if (m_pReadXml == nullptr || m_pReadXml->m_strScadaPath.isEmpty())
    {
        return false;
    }

    // todo:一个节点一个线程。
    // todo:节点下面ai和di各起一个线程。
    // todo:可以考虑多线程同时创建，如果ai和di比较大的话。
    // eg:size > 10万点。
    for each (auto i in m_pReadXml->m_memData.m_mapFesInfo)
    { // i:组名
        for each (auto j in i->m_mapNodeAndShmNameInfo.keys())
        { // j:节点名
            if (j.isEmpty())
            {
                continue;
            }

            // 目录check
            QString strCrtNodePath = m_pReadXml->m_strScadaPath + "/" + j + "/";
            QString strRealPath = strCrtNodePath + REAL_FOLDER;
            QString strBackupPath = strCrtNodePath + BAK_FOLDER;
            QDir dir;
            if (!dir.exists(strRealPath))
            {
                bool bRes = dir.mkpath(strRealPath);
                if (!bRes)
                {
                    return false;
                }
            }

            if (!dir.exists(strBackupPath))
            {
                bool bRes = dir.mkpath(strBackupPath);
                if (!bRes)
                {
                    return false;
                }
            }

            // 创建共享内存
            // ai
            if (i->m_mapNodeAndShmNameInfo[j] == nullptr)
            {
                continue;
            }

            int nAiSize = i->m_mapNodeAndShmNameInfo[j]->m_vecAiMemData.size();
            if (nAiSize > 0)
            {
                QString strPoolPathName = m_pReadXml->m_strScadaPath + "/" + j + "/" + REAL_FOLDER + AI_MEM_NAME;
                size_t ullAiSize = sizeof(AIN) * nAiSize;

                std::shared_ptr<CShmMgr> pAiShmMgr = BuildSingleNodeShmMem(strPoolPathName, ullAiSize);
                if (pAiShmMgr == nullptr)
                {
                    return false;
                }

                M_POOL_MAP poolMap;
                poolMap[strPoolPathName] = pAiShmMgr;
                m_mapNode[j] = poolMap;

                void* pMemData = pAiShmMgr->CreateShm(ullAiSize);
                if (pMemData == nullptr)
                {
                    return false;
                }

                M_MEM_PTR memPtr;
                memPtr[strPoolPathName].SetMemPtr(pMemData);
                m_mapMemPtr[j] = memPtr;
            }

            // di
            int nDiSize = i->m_mapNodeAndShmNameInfo[j]->m_vecDiMemData.size();
            if (nDiSize > 0)
            {
                QString strPoolPathName = m_pReadXml->m_strScadaPath + "/" + j + "/" + REAL_FOLDER + DI_MEM_NAME;
                size_t ullDiSize = sizeof(DIN) * nDiSize;

                std::shared_ptr<CShmMgr> pDiShmMgr = BuildSingleNodeShmMem(strPoolPathName, ullDiSize);
                if (pDiShmMgr == nullptr)
                {
                    return false;
                }

                M_POOL_MAP poolMap;
                poolMap[strPoolPathName] = pDiShmMgr;
                m_mapNode[j] = poolMap;

                void* pMemData = pDiShmMgr->CreateShm(ullDiSize);
                if (pMemData == nullptr)
                {
                    return false;
                }

                pDiShmMgr->BindData(strPoolPathName.toStdString().c_str(), pMemData);

                M_MEM_PTR memPtr;
                memPtr[strPoolPathName].SetMemPtr(pMemData);
                m_mapMemPtr[j] = memPtr;
            }
        }
    }

    return true;
}

std::shared_ptr<CShmMgr> CSHMService::BuildSingleNodeShmMem(QString strPoolPathName, const size_t size)
{
    if (strPoolPathName.isEmpty() || size <= 0)
    {
        return nullptr;
    }

    std::shared_ptr<CShmMgr> pShmMgr = nullptr;
    pShmMgr = m_poolMap[strPoolPathName];
    if (pShmMgr == nullptr)
    {
        pShmMgr = std::make_shared<CShmMgr>(strPoolPathName.toStdString().c_str(), size);
        //m_poolMap[strPoolPathName] = pShmMgr;
    }

    /*void* pData = pShmMgr->CreateShm(size);
    if (pData == nullptr)
    {
        return nullptr;
    }

    pShmMgr->BindData(strPoolPathName.toStdString().c_str(), pData);

    return pData;*/

    return pShmMgr;
}


bool CSHMService::OpenShmMem()
{
    return true;
}



/******************************************  CMemPtr  ***********************************************/

CMemPtr::CMemPtr() : m_pMemData(nullptr)
{

}

CMemPtr::~CMemPtr()
{

}

void CMemPtr::SetMemPtr(void* pMemData)
{
    m_pMemData = pMemData;
}

void* CMemPtr::GetMemPtr()
{
    return m_pMemData;
}
