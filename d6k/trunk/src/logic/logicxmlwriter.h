#ifndef LOGINXMLWRITER_H
#define LOGINXMLWRITER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <memory>
#include "seqdefine.h"

class CLogicObjectModel;
class CBlockProperty;
class CLineProperty;
class CCommentProperty;

class CLogicXmlWriter
{
public:
    CLogicXmlWriter();
    ~CLogicXmlWriter();

    bool SaveDataToXml(const QString &strFilename,std::shared_ptr<CLogicObjectModel> pObjectModel);

private:

    void SaveItemData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CBlockProperty>> tBlock);
    //
    void SaveLineData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CLineProperty>> tLine);
    //
    void SaveCommentData(QXmlStreamWriter &xmlWriter, std::vector<std::shared_ptr<CCommentProperty>> tComment);

};

#endif // LOGINXMLWRITER_H
