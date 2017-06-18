#ifndef LOGICXMLREADER_H
#define LOGICXMLREADER_H

#include <QObject>
#include <QXmlStreamReader>
#include <memory>
#include "seqdefine.h"

class QIODevice;
class CLogicObjectModel;
class CBlockProperty;
class CLineProperty;
class CCommentProperty;

class CLogicXmlReader
{
public:
    CLogicXmlReader();
    ~CLogicXmlReader();

    //
    bool OpenLogicXmlFile(const QString &fileName,std::shared_ptr<CLogicObjectModel> pObjectModel);
    //item
    void AnalyseLogicItemData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel);
    //line
    void AnalyseLogicLineData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel);
    //comment
    void AnalyseLogicCommentData(QXmlStreamReader &xmlReader, std::shared_ptr<CLogicObjectModel> pObjectModel);

};

#endif // LOGICXMLREADER_H
