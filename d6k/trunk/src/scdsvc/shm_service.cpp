
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
        // todo:·����д������xml��Ϣ.

        m_pReadXml = new CReadXml(strProjectPath);

        // ��xml�м�����Ϣ
        if (!m_pReadXml->LoadDataFromXml())
        {
            // log
            return false;
        }

        // ���ع����ڴ�
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

    // todo:һ���ڵ�һ���̡߳�
    // todo:�ڵ�����ai��di����һ���̡߳�
    // todo:���Կ��Ƕ��߳�ͬʱ���������ai��di�Ƚϴ�Ļ���
    // eg:size > 10��㡣
    for each (auto i in m_pReadXml->m_memData.m_mapFesInfo)
    { // i:����
        for each (auto j in i->m_mapNodeAndShmNameInfo.keys())
        { // j:�ڵ���
            if (j.isEmpty())
            {
                continue;
            }

            // Ŀ¼check
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

            // ���������ڴ�
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
