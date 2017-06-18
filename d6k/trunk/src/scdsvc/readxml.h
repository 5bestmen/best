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
#ifndef READ_XML_H
#define READ_XML_H

#include <memory>

#include <QString>
#include <QMap>
#include <QSet>
#include <QDomDocument>
#include <QXmlStreamReader>

#include "data_struct.h"

class CReadXml
{
public:
    CReadXml(const QString& strProjectPath);
    ~CReadXml();

public:
    // ��xml�ļ��м�������
    bool LoadDataFromXml();

private:
    // ��node.xml�л�ȡfes��Ϣ
    bool GetFesInfoFrom_NodeXml(QDomElement& domElem);

    // ����fes xml�ļ�
    bool Load_FesXml();
    bool LoadSingleFesXml(const QString& strFesPathName, std::shared_ptr<CMemData> pMemData);
    // ��Ҫ��Ϣ
    void ReadFesInfo(std::shared_ptr<CMemData> pMemData);
    void ReadChannelInfo(std::shared_ptr<CMemData> pMemData);
    void ReadDev(CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadDevInfo(CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    // ���ݵ���Ϣ
    void ReadData(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);

    // ��ȡ����վ��Ϣ
    void GetWorkstationInfo();

public:

    // ����·��
    QString m_strProjectPath;

    // scada·��
    QString m_strScadaPath;

    // node������
    QString m_strNodeInfoName;

    // ��ǰ�ڵ���
    QString m_strCrtNodeName;

private:
    std::shared_ptr<CChannel>       m_pSharedChannel;
    std::shared_ptr<CChannelMgr>    m_pChannelMgr;

public:
    // fes��workstation�����ļ�ȫ·����.
    QSet<QString> m_setCfg;

    // ����fes��Ϣ,key:group ID, value:�ڵ�������Ϣ
    QMap<QString, QVector<FesInfo> > m_mapFesInfo;

    // key:�ڵ���, value:�����ڴ���
    QMap<QString, QVector<QString> > m_mapNodeAndShmNameInfo;

    // ai������Ϣ,key:�����ڴ���,value:��Ӧ��AI������Ϣ
    QVector<std::shared_ptr<AIN> > m_vecAi;
    QMap<QString, QVector<std::shared_ptr<AIN> > > m_mapAiInfo;

    // di������Ϣ,key:�ڵ���,value:��Ӧ��DI������Ϣ
    QVector<std::shared_ptr<DIN> > m_vecDi;
    QMap<QString, QVector<std::shared_ptr<DIN> > > m_mapDiInfo;

    // �ڴ�����
    CGroupAndNode m_memData;

    // ���ù���վ��Ϣ
    //QMap<QString, QVector<WorkStationInfo> > m_mapWorkStationInfo;

private:

    //����ȡ��Ϣ
    QXmlStreamReader m_XmlReader;
};

#endif
