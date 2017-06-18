/*! @file shm_service.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  shm_service.h
�ļ�ʵ�ֹ��� :  �����ڴ����ӿ�
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
    **����:��ʵ�������
    ****************************************************************/
    friend class ACE_Singleton<CSHMService, ACE_SYNCH_RW_MUTEX>;

public:
    static CSHMService* Instance();

    enum
    {
        ORDER,  // �����кŴ洢
        HASH,   // hash�洢
    };

public:
    // �ڴ�����ʼ����
    // @para1 ����·��  @para2 �ڴ��ʼ����ʽ()
    bool Initialize(const QString& strProjectPath, unsigned int nMode);

    // ���з���
    void Run();

    // �رշ���
	void Shutdown();

private:

    // ����ȫ�⹲���ڴ�
    bool BuildAllShmMem();

    // ���������ڵ㹲���ڴ�
    std::shared_ptr<CShmMgr> BuildSingleNodeShmMem(QString strPoolPathName, const size_t size);

    // �򿪹����ڴ�
    bool OpenShmMem();
private:

    CSHMService();
    ~CSHMService();

private:

    // ����xml
    CReadXml* m_pReadXml;

private:
    // key:�����ڴ��ļ����� value���ڴ����ָ��
    typedef QMap<QString, std::shared_ptr<CShmMgr> > M_POOL_MAP;
    M_POOL_MAP m_poolMap;

    // key:�ڵ����� value�������ڵ��ڴ�
    ACE_Array_Map<QString, M_POOL_MAP> m_mapNode;

    // key:�����ڴ���,value:���ٵ��ڴ�
    typedef QMap<QString, CMemPtr> M_MEM_PTR;
    // key:�ڵ����� value�������ڵ��ڴ�
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