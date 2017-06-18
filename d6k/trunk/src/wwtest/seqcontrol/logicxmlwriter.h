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

    //更新xml文件
    bool UpdateXmlFile(const QString &strFilename, const QList<ITEMINFO>&lstItems, const QList<LINEROUNTER>&lstLines, const QList<COMMENTDATA>&lstComment);

private:
    //更新item文件
    bool UpdateItemData(QXmlStreamWriter &xmlWriter, const QList<ITEMINFO>&lstItems);
    //更新line文件
    bool UpdateLineData(QXmlStreamWriter &xmlWriter, const QList<LINEROUNTER>&lstLines);
    //更新comment文件
    bool UpdataCommentData(QXmlStreamWriter &xmlWriter, const QList<COMMENTDATA>&lstLines);

};

#endif // LOGINXMLWRITER_H
