#ifndef LOGINXMLWRITER_H
#define LOGINXMLWRITER_H

#include <QObject>
#include <QXmlStreamWriter>
#include "seqdefine.h"

class CLogicXmlWriter
{
public:
    CLogicXmlWriter();
    ~CLogicXmlWriter();

    //����xml�ļ�
    bool UpdateXmlFile(const QString &strFilename, const QList<ITEMINFO>&lstItems, const QList<LINEROUNTER>&lstLines, const QList<COMMENTDATA>&lstComment);

private:
    //����item�ļ�
    bool UpdateItemData(QXmlStreamWriter &xmlWriter, const QList<ITEMINFO>&lstItems);
    //����line�ļ�
    bool UpdateLineData(QXmlStreamWriter &xmlWriter, const QList<LINEROUNTER>&lstLines);
    //����comment�ļ�
    bool UpdataCommentData(QXmlStreamWriter &xmlWriter, const QList<COMMENTDATA>&lstLines);

};

#endif // LOGINXMLWRITER_H
