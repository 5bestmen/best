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
    //�����ļ�
    bool OpenXmlFile(const QString &fileName);
    //����
    void AnalyseXmlFile(QIODevice *pDevice);
    //����item����
    void AnalyseItemData(QXmlStreamReader &xmlReader);

    //����line����
    void AnalyseLineData(QXmlStreamReader &xmlReader);

    //����comment���� 
    void AnalyseCommentData(QXmlStreamReader &xmlReader);
    //�ṩ��ȡ�ӿ�
    QList<ITEMINFO> GetItemInfo();
    //��ȡֱ�߽ӿ�
    QList<LINEROUNTER> GetLineInfo();
    //��ȡcomment�ӿ�
    QList<COMMENTDATA> GetCommentInfo();

private:
    //item��Ϣ
    QList<ITEMINFO> m_lstItemInfos;
    //line��Ϣ
    QList<LINEROUNTER> m_lstLineInfos;
    //comment��Ϣ
    QList<COMMENTDATA> m_lstComment;
};

#endif // LOGICXMLREADER_H
