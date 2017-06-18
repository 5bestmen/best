#ifndef LOGICXMLREADER_H
#define LOGICXMLREADER_H

#include <QObject>
#include <QXmlStreamReader>
#include "seqdefine.h"

class QIODevice;
class CLogicXmlReader : public QObject
{
    Q_OBJECT
public:
    CLogicXmlReader(QObject *parent=0);
    ~CLogicXmlReader();
    //解析文件
    bool OpenXmlFile(const QString &fileName);
    //解析
    void AnalyseXmlFile(QIODevice *pDevice);
    //解析item数据
    void AnalyseItemData(QXmlStreamReader &xmlReader);

    //解析line数据
    void AnalyseLineData(QXmlStreamReader &xmlReader);

    //解析comment数据 
    void AnalyseCommentData(QXmlStreamReader &xmlReader);
    //提供获取接口
    QList<ITEMINFO> GetItemInfo();
    //获取直线接口
    QList<LINEROUNTER> GetLineInfo();
    //获取comment接口
    QList<COMMENTDATA> GetCommentInfo();

private:
    //item信息
    QList<ITEMINFO> m_lstItemInfos;
    //line信息
    QList<LINEROUNTER> m_lstLineInfos;
    //comment信息
    QList<COMMENTDATA> m_lstComment;
};

#endif // LOGICXMLREADER_H
