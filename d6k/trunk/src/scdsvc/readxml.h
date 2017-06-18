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
    // 从xml文件中加载数据
    bool LoadDataFromXml();

private:
    // 从node.xml中获取fes信息
    bool GetFesInfoFrom_NodeXml(QDomElement& domElem);

    // 加载fes xml文件
    bool Load_FesXml();
    bool LoadSingleFesXml(const QString& strFesPathName, std::shared_ptr<CMemData> pMemData);
    // 概要信息
    void ReadFesInfo(std::shared_ptr<CMemData> pMemData);
    void ReadChannelInfo(std::shared_ptr<CMemData> pMemData);
    void ReadDev(CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadDevInfo(CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    // 数据点信息
    void ReadData(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);
    void ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel, std::shared_ptr<CMemData> pMemData);

    // 获取工作站信息
    void GetWorkstationInfo();

public:

    // 工程路径
    QString m_strProjectPath;

    // scada路径
    QString m_strScadaPath;

    // node配置名
    QString m_strNodeInfoName;

    // 当前节点名
    QString m_strCrtNodeName;

private:
    std::shared_ptr<CChannel>       m_pSharedChannel;
    std::shared_ptr<CChannelMgr>    m_pChannelMgr;

public:
    // fes、workstation配置文件全路径名.
    QSet<QString> m_setCfg;

    // 配置fes信息,key:group ID, value:节点名等信息
    QMap<QString, QVector<FesInfo> > m_mapFesInfo;

    // key:节点名, value:共享内存名
    QMap<QString, QVector<QString> > m_mapNodeAndShmNameInfo;

    // ai内容信息,key:共享内存名,value:对应的AI内容信息
    QVector<std::shared_ptr<AIN> > m_vecAi;
    QMap<QString, QVector<std::shared_ptr<AIN> > > m_mapAiInfo;

    // di内容信息,key:节点名,value:对应的DI内容信息
    QVector<std::shared_ptr<DIN> > m_vecDi;
    QMap<QString, QVector<std::shared_ptr<DIN> > > m_mapDiInfo;

    // 内存数据
    CGroupAndNode m_memData;

    // 配置工作站信息
    //QMap<QString, QVector<WorkStationInfo> > m_mapWorkStationInfo;

private:

    //流读取信息
    QXmlStreamReader m_XmlReader;
};

#endif
